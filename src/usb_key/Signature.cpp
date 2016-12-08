//
// Created by Hanzeil on 16-11-30.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//
// The description of this file is in the header file.
//


#include "Signature.h"

namespace usb_key {
    Signature::Signature() {
        u32 len, r;
        r = SKF_EnumDev(1, sz_name_, &len);
        if (r) {
            return;
        }
        sz_name_ = (char *) malloc(len);
        if (sz_name_ == NULL) {
            return;
        }

        r = SKF_EnumDev(1, sz_name_, &len);
        if (r) {
            free(sz_name_);
            return;
        }
    }

    void Signature::Connect() {
        auto r = OpenDev(sz_name_, &hDev_);
        if (r) {
            std::cout << "Open Dev ERROR" << std::endl;
            return;
        }

        r = OpenApp(hDev_, NULL, &hApp_, "111111");
        if (r) {
            SKF_DisconnectDev(hDev_);
            std::cout << "Open App ERROR" << std::endl;
            return;
        }
        r = OpenContainerForExportCer(hDev_, hApp_, NULL, &hCtn_);
        if (r) {
            SKF_CloseApplication(hApp_);
            SKF_DisconnectDev(hDev_);
            std::cout << "Open Container ERROR" << std::endl;
            return;
        }
    }

    void Signature::DisConnect() {
        SKF_CloseContainer(hCtn_);
        SKF_CloseApplication(hApp_);
        SKF_DisconnectDev(hDev_);
    }

    Signature::~Signature() {
        free(sz_name_);
    }

    bool Signature::VerifySignedData(const std::vector<unsigned char> &cert,
                                     const std::vector<unsigned char> &data,
                                     const std::vector<unsigned char> &signed_data) {
        // 证书的指针形式
        auto cert_point = new unsigned char[cert.size()];
        for (std::size_t i = 0; i < cert.size(); i++) {
            cert_point[i] = cert[i];
        }
        // 数据的指针形式
        auto data_point = new unsigned char[data.size()];
        for (std::size_t i = 0; i < data.size(); i++) {
            data_point[i] = data[i];
        }
        // 签名数据的skf标准格式
        ECCSIGNATUREBLOB signed_data_skf;
        for (std::size_t i = 0; i < signed_data.size(); i++) {
            ((unsigned char *) &signed_data_skf)[i] =
                    signed_data[i];
        }
        // 证书的公钥 skf标准格式
        ECCPUBLICKEYBLOB public_key_skf;
        parce_cer(cert_point, (int) cert.size(),
                  (unsigned char *) &public_key_skf);

        // 调试用数据
        BYTE pbData[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
        auto r = SKF_ExtECCVerify(hDev_, &public_key_skf, data_point, data.size(), &signed_data_skf);

        delete cert_point;
        delete data_point;

        if (r) {
            return false;
        }
        return true;
    }

    u32 Signature::OpenDev(char *szName, DEVHANDLE *phDev) {
        char *szList = NULL;
        u32 r, len;
        DEVHANDLE hDev;
        DEVINFO info;

        if (szName == NULL) {
            r = SKF_EnumDev(1, szList, &len);
            if (r) {
                return r;
            }
            szList = (char *) malloc(len);
            if (szList == NULL) {
                return 1;
            }

            r = SKF_EnumDev(1, szList, &len);
            if (r) {
                free(szList);
                return r;
            }
            szName = szList;
        }

        r = SKF_ConnectDev(szName, &hDev);
        if (szList) free(szList);
        if (r) {
            return r;
        }

        *phDev = hDev;
        return 0;
    }

    u32 Signature::Enum_App(DEVHANDLE *hDev, char **pszList) {
        u32 len;
        char *szList;

        u32 r = SKF_EnumApplication(hDev, NULL, &len);
        if (r) {
            return r;
        }

        szList = (char *) malloc(len);
        if (szList == NULL) {
            return 1;
        }

        r = SKF_EnumApplication(hDev, szList, &len);
        if (r) {
            free(szList);
            return r;
        }

        *pszList = szList;
        return r;
    }

    u32 Signature::OpenApp(DEVHANDLE hDev, char *szName, HAPPLICATION *phApp, char *szPin) {
        HAPPLICATION hApp;
        char *szList = NULL;
        u32 r = 0, len;

        if (szName == NULL) {
            int i = 1, j;
            r = Enum_App((DEVHANDLE *) hDev, &szList);
            if (r) return r;

            szName = szList;
            /*
            if (szName && *szName)
                printf("-- Please select an application NUMBER to open\n");

            while (szName && *szName) {
                printf("%d %s\n", i++, szName);
                szName += strlen(szName) + 1;
            }

            scanf("%d", &i);
            */
            i = 0;

            szName = szList;
            for (j = 1; j < i; j++) {
                szName += strlen(szName) + 1;
            }
        }

        r = SKF_OpenApplication(hDev, szName, &hApp);
        if (szList) free(szList);
        if (r) {
            return 1;
        }
        r = SKF_VerifyPIN(hApp, USER_TYPE, szPin, &len);
        if (r) {
            SKF_CloseApplication(hApp);
            return 1;
        }
        *phApp = hApp;
        return 0;
    }

    u32 Signature::Enum_Cnt(HAPPLICATION *hApp, char **pszList) {
        char *szList = NULL;
        u32 r, len;

        r = SKF_EnumContainer(hApp, NULL, &len);
        if (r) {
            return r;
        }

        szList = (char *) malloc(len);
        if (szList == NULL) {
            return 1;
        }

        r = SKF_EnumContainer(hApp, szList, &len);
        if (r) {
            free(szList);
            return r;
        }

        *pszList = szList;
        return 0;
    }

    u32 Signature::OpenContainerForExportCer(DEVHANDLE hDev, HAPPLICATION hApp, char *szName, HCONTAINER *phCtn) {
        HCONTAINER hCtn;
        char *szList = NULL;
        u32 r, t;

        if (szName == NULL) {
            int i = 1, j;
            r = Enum_Cnt((HAPPLICATION *) hApp, &szList);
            if (r) return r;

            szName = szList;
            /*
            if (szName && *szName)
                printf("-- Please select a container NUMBER to open\n");

            while (szName && *szName) {
                printf("%d %s\n", i++, szName);
                szName += strlen(szName) + 1;
            }

            scanf("%d", &i);
            */
            i = 1;

            szName = szList;
            for (j = 1; j < i; j++) {
                szName += strlen(szName) + 1;
            }
        }

        r = SKF_OpenContainer(hApp, szName, &hCtn);
        if (szList) free(szList);
        if (r) {
            return r;
        }

        r = SKF_GetContainerType(hCtn, &t);
        if (r) {
            SKF_CloseContainer(hCtn);
            return r;
        }

        *phCtn = hCtn;
        return 0;
    }

    u32 Signature::GetCert(HCONTAINER hCon, int bFlag, ECCPUBLICKEYBLOB *public_key) {
        u32 ret, len;
        u8 *data = NULL;

        ret = SKF_ExportCertificate(hCon, bFlag, data, &len);
        if (ret) {
        }
        else {
            data = (u8 *) malloc(len);
            if (data == NULL) {
                return -1;
            }
            ret = SKF_ExportCertificate(hCon, bFlag, data, &len);
            printf("Certificate:\n");
            FILE *fp = fopen("cert_cer", "wb");
            fwrite(data, len, 1, fp);
            fclose(fp);

            if (ret) {
            }
            else {
                parce_cer(data, len, (unsigned char *) public_key);
            }
            free(data);
        }

        return 0;
    }
}
