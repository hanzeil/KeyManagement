//
// Created by Hanzeil on 16-11-30.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//


#ifndef KEYMANAGEMENT_SIGNATURE_H
#define KEYMANAGEMENT_SIGNATURE_H

#include "skf_type.h"
#include "SKF.h"
#include "base_type.h"
#include "server_authen.h"
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <vector>

namespace usb_key {

    class Signature {
    public:
        Signature(const Signature &) = delete;

        Signature &operator=(const Signature &)= delete;

        void Connect();

        void DisConnect();

        explicit Signature();

        ~Signature();

        bool VerifySignedData(
                const std::vector<unsigned char> &cert,
                const std::vector<unsigned char> &data,
                const std::vector<unsigned char> &signed_data);

    private:
        u32 OpenApp(DEVHANDLE hDev, char *szName, HAPPLICATION *phApp, char *szPin);

        u32 Enum_Cnt(HAPPLICATION *hApp, char **pszList);

        u32 OpenContainerForExportCer(DEVHANDLE hDev, HAPPLICATION hApp, char *szName, HCONTAINER *phCtn);

        u32 GetCert(HCONTAINER hCon, int bFlag, ECCPUBLICKEYBLOB *public_key);

        u32 Enum_App(DEVHANDLE *hDev, char **pszList);

        u32 OpenDev(char *szName, DEVHANDLE *phDev);

        DEVHANDLE hDev_ = NULL;
        HAPPLICATION hApp_ = NULL;
        HCONTAINER hCtn_ = NULL;
        char * sz_name_ = NULL;
    };
}


#endif //KEYMANAGEMENT_SIGNATURE_H
