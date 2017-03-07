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

char *Signature::sz_name_ = NULL;

Signature::Signature() {

    auto r = OpenDev(&hDev_);
    if (r) {
        SKF_CloseContainer(hCtn_);
        SKF_CloseApplication(hApp_);
        SKF_DisconnectDev(hDev_);
        std::cout << "USB_KEY ERROR" << std::endl;
    }

    r = OpenApp(hDev_, NULL, &hApp_, sz_pin_);
    if (r) {
        SKF_CloseContainer(hCtn_);
        SKF_CloseApplication(hApp_);
        SKF_DisconnectDev(hDev_);
        std::cout << "USB_KEY ERROR" << std::endl;
    }
    r = OpenContainerForExportCer(hDev_, hApp_, NULL, &hCtn_);
    if (r) {
        SKF_CloseContainer(hCtn_);
        SKF_CloseApplication(hApp_);
        SKF_DisconnectDev(hDev_);
        std::cout << "USB_KEY ERROR" << std::endl;
    }

}

Signature::~Signature() {
    SKF_CloseContainer(hCtn_);
    SKF_CloseApplication(hApp_);
    SKF_DisconnectDev(hDev_);
}

bool Signature::VerifySignedData(std::vector<unsigned char> &cert, std::vector<unsigned char> &data,
                                 std::vector<unsigned char> &signed_data) {
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

std::vector<unsigned char> Signature::SignData(std::vector<unsigned char> &data) {
    unsigned char rand[32] = {0};
    for (size_t i = 0; i < 16; i++) {
        rand[i] = data[i];
    }
    ECCSIGNATUREBLOB pSignature;
    SKF_ECCSignData(hCtn_, rand, 32, &pSignature);
    std::vector<unsigned char> result(sizeof(pSignature));
    for (std::size_t i = 0; i < sizeof(pSignature); i++) {
        result[i] = *((unsigned char *) &pSignature + i);
    }
    return result;
}

std::vector<unsigned char> Signature::GetSignCert() {
    return GetCert(hCtn_, 1);
}

std::vector<unsigned char> Signature::GetEncCert() {
}

u32 Signature::OpenDev(DEVHANDLE *phDev) {
    char *szList = NULL;
    u32 r, len;
    DEVHANDLE hDev;
    DEVINFO info;

    if (Signature::sz_name_ == NULL) {
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
        Signature::sz_name_ = strdup(szList);
    }

    r = SKF_ConnectDev(Signature::sz_name_, &hDev);
    if (szList) free(szList);
    if (r) {
        return r;
    }

    r = SKF_GetDevInfo(hDev, &info);
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
        i = 5;

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

std::vector<unsigned char> Signature::GetCert(HCONTAINER hCon, int bFlag) {
    u32 ret, len;
    u8 *data = NULL;

    ret = SKF_ExportCertificate(hCon, bFlag, data, &len);
    if (ret) {
        std::runtime_error e("Export cert error");
        throw (e);
    }
    else {
        auto data = (u8 *) malloc(len);
        ret = SKF_ExportCertificate(hCon, bFlag, data, &len);
        std::vector<unsigned char> result(data,
                                          data + len);
        free(data);
        return result;
    }

}
