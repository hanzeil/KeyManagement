//
// Created by Hanzeil on 16-11-30.
//
// Copyright (c) 2016 航天二院爱威公司. All rights reserved.
//
// Author Hanzeil.
//


#ifndef KEYMANAGEMENT_TEST_CLIENT_SIGNATURE_H
#define KEYMANAGEMENT_TEST_CLIENT_SIGNATURE_H

#include "skf_type.h"
#include "SKF.h"
#include "base_type.h"
#include "ServerAuthen.h"
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <vector>


class Signature {
public:
    Signature(const Signature &) = delete;

    Signature &operator=(const Signature &)= delete;

    explicit Signature();

    ~Signature();

    bool VerifySignedData(
            std::vector<unsigned char> &cert,
            std::vector<unsigned char> &data,
            std::vector<unsigned char> &signed_data);

    std::vector<unsigned char> SignData(std::vector<unsigned char> &data);

    std::vector<unsigned char> GetSignCert();

    std::vector<unsigned char> GetEncCert();

private:
    u32 OpenApp(DEVHANDLE hDev, char *szName, HAPPLICATION *phApp, char *szPin);

    u32 Enum_Cnt(HAPPLICATION *hApp, char **pszList);

    u32 OpenContainerForExportCer(DEVHANDLE hDev, HAPPLICATION hApp, char *szName, HCONTAINER *phCtn);

    std::vector<unsigned char> GetCert(HCONTAINER hCon, int bFlag);

    u32 Enum_App(DEVHANDLE *hDev, char **pszList);

    u32 OpenDev(DEVHANDLE *phDev);

    DEVHANDLE hDev_;
    HAPPLICATION hApp_;
    HCONTAINER hCtn_;
    char *sz_pin_ = const_cast<char *>("111111");
    static char *sz_name_;
};


#endif //KEYMANAGEMENT_TEST_CLIENT_SIGNATURE_H
