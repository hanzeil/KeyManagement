//
// Created by root on 16-11-30.
//
// Author root.
//


#include "server_authen.h"

namespace handler {
    namespace usb_key {


        typedef struct {
            char s1[50], s2[50];
        } TLV;

        typedef struct {
            char s1[50], s2[5000];
        } TLV2;

        struct SignatureAlgorithm {
            TLV algorithm;
            TLV parameters;
        };

        struct subjectPublicKey {
            TLV algorithm;
            TLV parameters;
            TLV2 PKey;
        };

        struct signatureArray {
            char s1[50], s2[50];
        } sA[7], is[6];

        struct SignatureValue {
            TLV2 signatureValue;
        };

        struct TbsCertificate {
            TLV version;
            TLV serialNumber;
            struct SignatureAlgorithm signature;
            struct signatureArray issuer_[1];
            TLV validity[2];
            struct signatureArray subject_[1];
            struct subjectPublicKey subjectPublicKeyInfo;
            TLV issuerUniqueID;
            TLV subjectUniqueID;
            TLV extensions;
        };

///证书ca.cer的结构
        struct X509cer {
            struct TbsCertificate cat;
            struct SignatureAlgorithm casa;
            struct SignatureValue casv;
        } ca_cer;



        static int datalen = 0;
        static int publen = 0;
        static unsigned char pubkey[256];
        char s[5000];
        static int nc = 0, tis = 0;
        int bk = 1;
        int btag = 1;//0-隐式  1-显式
#define BUFFERLEN 2048
        static unsigned char filearray[BUFFERLEN];
        static int myindex = 0;
        char realkey[4] = {0};

        Len L_init(int len, int tag) {
            Len l;
            l.len = len;
            l.tag = tag;
            return l;
        }

        int parce_cer(unsigned char *cerdata, int cerlen, unsigned char *signkey) {
#if 0
            //char *filename="ca.cer";
           //char *filename="2_cer.cer";
           char *filename="signature_cer.cer";
           fp=fopen(filename,"rb");
           if(fp==NULL){
               puts("can't open the file!");
               exit(0);
           }

           int file_len=0;
           fseek(fp,0L,SEEK_END);
           file_len=ftell(fp);
           fseek(fp,0L,SEEK_SET);
           printf("file_len=%d\n",file_len);
           fread(filearray,file_len,1,fp);
#endif

            //sAfill();
            // isFill();
            int pubkeylen = 256;
            memcpy(filearray, cerdata, cerlen);
            tlv();
            memset(signkey, 0x0, 132);
            memcpy(signkey, &pubkeylen, 4);
            memcpy(signkey + 36, pubkey + 2, 32);
            memcpy(signkey + 100, pubkey + 34, 32);


            nc = 0;
            tis = 0;
            bk = 1;
            myindex = 0;
            //fclose(fp);
            //output();
            return 0;
        }

        void fill(int n) {
            int i = 0;
            switch (n) {//表示第几次调用tlv
                case 4:
                    strcpy(ca_cer.cat.version.s1, "version:   ");
                    if (strcmp(s, "0") == 0) strcpy(s, "v1");
                    else if (strcmp(s, "1") == 0) strcpy(s, "v2");
                    else strcpy(s, "v3");
                    strcpy(ca_cer.cat.version.s2, s);
                    break;
                case 5:
                    strcpy(ca_cer.cat.serialNumber.s1, "serialNumber:   ");
                    strcpy(ca_cer.cat.serialNumber.s2, s);
                    break;
                case 7:
                    strcpy(ca_cer.cat.signature.algorithm.s1, "name of algorithm of signature:   ");
                    for (i = 0; i < 4; i++) {
                        if (strcmp(s, sA[i].s1) == 0) {
                            strcpy(ca_cer.cat.signature.algorithm.s2, sA[i].s2);
                            break;
                        }
                    }
                    break;
                case 8:
                    strcpy(ca_cer.cat.signature.parameters.s1, "parameters of signature:   ");
                    strcpy(ca_cer.cat.signature.parameters.s2, s);
                    break;
                case 12:
                    for (i = 0; i < 4; i++) {
                        if (strcmp(s, is[i].s1) == 0) {
                            strcpy(ca_cer.cat.issuer_[i].s1, is[i].s2);
                            strcat(ca_cer.cat.issuer_[i].s1, "of issuer:\t");
                            tis = i;
                            break;
                        }
                    }
                    break;
                case 13:
                    strcpy(ca_cer.cat.issuer_[tis].s2, s);
                    break;
#if 0
                case 16:
                    for(i=0;i<6;i++){
                        if(strcmp(s,is[i].s1)==0){
                            strcpy(ca_cer.cat.issuer_[i].s1,is[i].s2);
                            strcat(ca_cer.cat.issuer_[i].s1,"of issuer:\t");
                            tis=i;
                            break;
                        }
                    }
                    break;
                case 17:
                    strcpy(ca_cer.cat.issuer_[tis].s2,s);
                    break;
                case 20:
                    for(i=0;i<6;i++){
                        if(strcmp(s,is[i].s1)==0){
                            strcpy(ca_cer.cat.issuer_[i].s1,is[i].s2);
                            strcat(ca_cer.cat.issuer_[i].s1,"of issuer:\t");
                            tis=i;
                            break;
                        }
                    }
                    break;
                case 21:
                    strcpy(ca_cer.cat.issuer_[tis].s2,s);
                    break;

                case 24:
                    for(i=0;i<6;i++){
                        if(strcmp(s,is[i].s1)==0){
                            strcpy(ca_cer.cat.issuer_[i].s1,is[i].s2);
                            strcat(ca_cer.cat.issuer_[i].s1,"of issuer:\t");
                            tis=i;
                            break;
                        }
                    }
                    break;
                case 25:
                    strcpy(ca_cer.cat.issuer_[tis].s2,s);
                    break;
                case 28:
                    for(i=0;i<6;i++){
                        if(strcmp(s,is[i].s1)==0){
                            strcpy(ca_cer.cat.issuer_[i].s1,is[i].s2);
                            strcat(ca_cer.cat.issuer_[i].s1,"of issuer:\t");
                            tis=i;
                            break;
                        }
                    }
                    break;
                case 29:
                    strcpy(ca_cer.cat.issuer_[tis].s2,s);
                    break;
                case 32:
                    for(i=0;i<6;i++){
                        if(strcmp(s,is[i].s1)==0){
                            strcpy(ca_cer.cat.issuer_[i].s1,is[i].s2);
                            strcat(ca_cer.cat.issuer_[i].s1,"of issuer:\t");
                            tis=i;
                            break;
                        }
                    }
                    break;
                case 33:
                    strcpy(ca_cer.cat.issuer_[tis].s2,s);
                    break;
#endif
                case 23:
                    strcpy(ca_cer.cat.validity[0].s1, "the begin of validity:    ");
                    strcpy(ca_cer.cat.validity[0].s2, s);
                    break;
                case 24:
                    strcpy(ca_cer.cat.validity[1].s1, "the end of validity:    ");
                    strcpy(ca_cer.cat.validity[1].s2, s);
                    break;
                case 28:
                    for (i = 0; i < 4; i++) {
                        if (strcmp(s, is[i].s1) == 0) {
                            strcpy(ca_cer.cat.subject_[i].s1, is[i].s2);
                            strcat(ca_cer.cat.subject_[i].s1, "of subject:\t");
                            tis = i;
                            break;
                        }
                    }
                    break;
                case 29:
                    strcpy(ca_cer.cat.subject_[tis].s2, s);
                    break;
/*
         case 33:
             for(i=0;i<6;i++){
                 if(strcmp(s,is[i].s1)==0){
                     strcpy(ca_cer.cat.subject_[i].s1,is[i].s2);
                     strcat(ca_cer.cat.subject_[i].s1,"of subject:\t");
                     tis=i;
                     break;
                 }
             }
             break;
         case 34:
             strcpy(ca_cer.cat.subject_[tis].s2,s);
             break;
         case 37:
             for(i=0;i<6;i++){
                 if(strcmp(s,is[i].s1)==0){
                     strcpy(ca_cer.cat.subject_[i].s1,is[i].s2);
                     strcat(ca_cer.cat.subject_[i].s1,"of subject:\t");
                     tis=i;
                     break;
                 }
             }
             break;
         case 38:
             strcpy(ca_cer.cat.subject_[tis].s2,s);
             break;
         case 41:
             for(i=0;i<6;i++){
                 if(strcmp(s,is[i].s1)==0){
                     strcpy(ca_cer.cat.subject_[i].s1,is[i].s2);
                     strcat(ca_cer.cat.subject_[i].s1,"of subject:\t");
                     tis=i;
                     break;
                 }
             }
             break;
         case 42:
             strcpy(ca_cer.cat.subject_[tis].s2,s);
             break;
         case 45:
             for(i=0;i<6;i++){
                 if(strcmp(s,is[i].s1)==0){
                     strcpy(ca_cer.cat.subject_[i].s1,is[i].s2);
                     strcat(ca_cer.cat.subject_[i].s1,"of subject:\t");
                     tis=i;
                     break;
                 }
             }
             break;
         case 46:
             strcpy(ca_cer.cat.subject_[tis].s2,s);
             break;
         case 49:
             for(i=0;i<6;i++){
                 if(strcmp(s,is[i].s1)==0){
                     strcpy(ca_cer.cat.subject_[i].s1,is[i].s2);
                     strcat(ca_cer.cat.subject_[i].s1,"of subject:\t");
                     tis=i;
                     break;
                 }
             }
             break;
         case 50:
             strcpy(ca_cer.cat.subject_[tis].s2,s);
             break;
*/

#if 1
                case 48:
                    strcpy(ca_cer.cat.subjectPublicKeyInfo.algorithm.s1, "name of algorithm of subjectPublicKey:   ");
                    for (i = 0; i < 7; i++) {
                        if (strcmp(s, sA[i].s1) == 0) {
                            strcpy(ca_cer.cat.subjectPublicKeyInfo.algorithm.s2, sA[i].s2);
                            break;
                        }
                    }
                    break;
                case 49:
                    strcpy(ca_cer.cat.subjectPublicKeyInfo.parameters.s1,
                           "parameters of algorithm of subjectPublicKey:   ");
                    strcpy(ca_cer.cat.subjectPublicKeyInfo.parameters.s2, s);
                    break;
#endif
                case 50:
                    strcpy(ca_cer.cat.subjectPublicKeyInfo.PKey.s1, "subjectPublicKey:   ");
                    strcpy(ca_cer.cat.subjectPublicKeyInfo.PKey.s2, s);

                    publen = datalen;
                    // printf("pubkey len = %d\n", publen);

                    memcpy(pubkey, s, publen);    /// out
                    break;
                case 61:
                    strcpy(ca_cer.casa.algorithm.s1, "name of signatureAlgorithm:   ");
                    for (i = 0; i < 7; i++) {
                        if (strcmp(s, sA[i].s1) == 0) {
                            strcpy(ca_cer.casa.algorithm.s2, sA[i].s2);
                            break;
                        }
                    }
                    break;
                case 59:
                    strcpy(ca_cer.casa.parameters.s1, "parameters of signatureAlgorithm:   ");
                    strcpy(ca_cer.casa.parameters.s2, s);
                    break;
                case 79:
                    strcpy(ca_cer.casv.signatureValue.s1, "signatureValue:   ");
                    strcpy(ca_cer.casv.signatureValue.s2, s);
                    bk = 0;
                    break;
            }
        }

        Len tlv() {
            int i = 0;
            if (bk == 0) return L_init(1000, 0);
            nc++;
            //printf("the %dth recall tlv\n",nc);
            bool b = true;
            //unsigned char type=fgetc(fp);//type
            //unsigned char len0=fgetc(fp);//len

            unsigned char type = filearray[myindex++];//type
            unsigned char len0 = filearray[myindex++];//len
            int len = len0;
            int lem = 0;
            //printf("type = %x,len0 = %x\n",type,len0);
            if (type < 0xa0) {
                if (type == 1) {
                    unsigned char vc = filearray[myindex++];//fgetc(fp);
                    if (vc == 0)
                        strcpy(s, "FALSE");
                    else
                        strcpy(s, "TRUE");
                }
                else if (type == 2 || type == 3 || type == 4) {
                    if (len0 > 0x80) {
                        int tn2 = len0 - 0x80;
                        unsigned char tl;
                        len = 0;
                        for (i = 0; i < tn2; i++) {
                            tl = filearray[myindex++];//fgetc(fp);
                            len *= 256;
                            len += tl;
                        }
                    }
                    bitfill(len);
                }
                else if (type == 5) {
                    strcpy(s, "NULL");
                }
                else if (type == 6) {
#if 1
                    strcpy(s, "");
                    int dd = len0;
                    unsigned char tl = filearray[myindex++];//fgetc(fp);
                    int d = tl / 40;
                    char ts2[10];
                    sprintf(ts2, "%d", d);
                    strcat(s, ts2);
                    strcat(s, ".");
                    d = tl - d * 40;
                    sprintf(ts2, "%d", d);
                    strcat(s, ts2);
                    for (i = 1; i < dd; i++) {
                        strcat(s, ".");
                        i--;
                        int t = 0;
                        while (1) {
                            tl = filearray[myindex++];//fgetc(fp);
                            i++;
                            bool b2 = false;
                            if (tl & 0x80) {
                                b2 = true;
                            }
                            if (b2) {
                                tl &= 0x7f;
                            }
                            t *= 128;
                            t += tl;
                            if (!b2) break;
                        }
                        sprintf(ts2, "%d", t);
                        strcat(s, ts2);
                    }
#endif
                }
                else if (type == 0xc) {
                    int issuerlen = len0;
                    //fread(s,1,issuerlen,fp);
                    memcpy(s, filearray, issuerlen);
                    myindex += issuerlen;
                    s[issuerlen] = '\0';
                }
                else if (type == 0x13 || type == 0x17 || type == 0x18) {
                    int d = len0;
                    //fread(s,1,d,fp);
                    memcpy(s, filearray, d);
                    myindex += d;
                    s[d] = '\0';
                }
                else if (type == 0x30 || type == 0x31) {
                    b = false;
                    if (len0 > 0x80) {
                        len = 0;
                        len0 -= 0x80;
                        unsigned char tl;
                        for (i = 0; i < len0; i++) {
                            tl = filearray[myindex++];//fgetc(fp);
                            len *= 256;
                            len += tl;
                        }
                    }
                    int dlen = len;
                    //printf("the %dth len = %x\n",nc,len);
                    while (dlen > 0) {
                        //printf("xxxxxxxxxxxx\n");
                        int tmplength = tlv().len;
                        dlen -= tmplength;
                        //printf("yyyyyyyyyyyy,first dlen=%d\n",dlen);
                    }
                }
                else {
                    printf("the cer has errors!\n");
                    exit(0);
                }
            }
            else    ///type>=0xa0
            {
                b = false;
                lem = type - 0xa0;
                if (len0 > 0x80) {
                    int tn2 = len0 - 0x80;
                    unsigned char tl;
                    len = 0;
                    for (i = 0; i < tn2; i++) {
                        tl = filearray[myindex++];//fgetc(fp);
                        len *= 256;
                        len += tl;
                    }
                }
                if (btag) {
                    //这里做个简化，对扩展域进行忽略处理。
                    if (nc == 67)  //fseek(fp,len,SEEK_CUR);
                    {
                        /*nothing to be done*/
                    }
                    else tlv();
                }
                else {
                    //这里不作具体实现，依具体类型的证书而定
                }
            }
            if (b) fill(nc);
            return L_init(len, lem);
        }

        void bitfill(int dd) {
            memcpy(s, filearray + myindex, dd);
            myindex += dd;
            //LOG_DATA(s,dd);
            s[dd] = '\0';
            datalen = dd;
        }

        void output() {
            puts("ca.cer parser as follows:");
            printf("[version]  %s%s\n", ca_cer.cat.version.s1, ca_cer.cat.version.s2);
            //printf("[serialNumber]  %s%s\n",ca_cer.cat.serialNumber.s1,ca_cer.cat.serialNumber.s2);
            //printf("[signature.algorithm]  %s%s\n",ca_cer.cat.signature.algorithm.s1,ca_cer.cat.signature.algorithm.s2);
            //printf("[signature.parameters]  %s%s\n",ca_cer.cat.signature.parameters.s1,ca_cer.cat.signature.parameters.s2);
            //printf("[issuer ID]  issuer\n%s%s\n%s%s\n%s%s\n%s%s\n%s%s\n%s%s\n",ca_cer.cat.issuer_[0].s1,ca_cer.cat.issuer_[0].s2,ca_cer.cat.issuer_[1].s1,ca_cer.cat.issuer_[1].s2,ca_cer.cat.issuer_[2].s1,ca_cer.cat.issuer_[2].s2,ca_cer.cat.issuer_[3].s1,ca_cer.cat.issuer_[3].s2,ca_cer.cat.issuer_[4].s1,ca_cer.cat.issuer_[4].s2,ca_cer.cat.issuer_[5].s1,ca_cer.cat.issuer_[5].s2);
            //printf("[validity]  validity:      %s-%s\n",ca_cer.cat.validity[0].s2,ca_cer.cat.validity[1].s2);
            //printf("[subject]  subject\n%s%s\n%s%s\n%s%s\n%s%s\n%s%s\n%s%s\n",ca_cer.cat.subject_[0].s1,ca_cer.cat.subject_[0].s2,ca_cer.cat.subject_[1].s1,ca_cer.cat.subject_[1].s2,ca_cer.cat.subject_[2].s1,ca_cer.cat.subject_[2].s2,ca_cer.cat.subject_[3].s1,ca_cer.cat.subject_[3].s2,ca_cer.cat.subject_[4].s1,ca_cer.cat.subject_[4].s2,ca_cer.cat.subject_[5].s1,ca_cer.cat.subject_[5].s2);
            //printf("[subjectPublicKeyInfo.algorithm]  %s%s\n",ca_cer.cat.subjectPublicKeyInfo.algorithm.s1,ca_cer.cat.subjectPublicKeyInfo.algorithm.s2);
            //printf("[subjectPublicKeyInfo.parameters]  %s%s\n",ca_cer.cat.subjectPublicKeyInfo.parameters.s1,ca_cer.cat.subjectPublicKeyInfo.parameters.s2);
            printf("[subjectPublicKeyInfo]  %s%s\n", ca_cer.cat.subjectPublicKeyInfo.PKey.s1,
                   ca_cer.cat.subjectPublicKeyInfo.PKey.s2);
            //printf("[issuerUniqueID]  issuerUniqueID:     无\n");
            //printf("[subjectUniqueID]  subjectUniqueID:     无\n");
            //printf("[extendsions]  extendsions:     省略\n");
            //printf("[casa.algorithm]  %s%s\n",ca_cer.casa.algorithm.s1,ca_cer.casa.algorithm.s2);
            //printf("[casa.parameters]  %s%s\n",ca_cer.casa.parameters.s1,ca_cer.casa.parameters.s2);
            printf("[casv.signatureValue]  %s%s\n", ca_cer.casv.signatureValue.s1, ca_cer.casv.signatureValue.s2);
        }

        int Parser_cmd(unsigned char *cmd, int cmdlen, AUTHEN_DATA *auth_data) {
            printf("in Parser_cmd\n");
            char adminkey[4] = {0x35, 0x37, 0x33, 0x46};
            char userkey[4] = {0x36, 0x30, 0x33, 0x46};
            unsigned short len, len1;
            unsigned int sumlen = 0;
            len = 2;
            len1 = *((unsigned short *) cmd);
            auth_data->devIDlen = len1;
            memcpy(auth_data->devID, cmd + len, len1);
            sumlen = sumlen + len1 + 2;

            //printf("len1=%d\n",len1);
            //LOG_DATA(auth_data->devID,len1);
            //LOG("\n\n");
            //LOG_DATA(adminkey,4);
            //LOG("\n\n");
            //LOG_DATA((auth_data->devID+20),4);
            //LOG("\n\n");

            int ret = -1;
            int ret1 = -1;
            memcpy(realkey, auth_data->devID + 20, 4);

            ret = memcmp(realkey, adminkey, 4);
            ret1 = memcmp(realkey, userkey, 4);

            if ((ret == 0) || (ret1 == 0)) {
                /*nothing to be done!*/
            }
            else {
                printf("no adminstrator right!\n");
                return -1;

            }


            len = sumlen;
            len1 = *((unsigned short *) (cmd + len));
            auth_data->randlen = len1;
            memcpy(auth_data->rand, cmd + len + 2, len1);
            sumlen = sumlen + len1 + 2;


            len = sumlen;
            len1 = *((unsigned short *) (cmd + len));
            auth_data->siglen = len1;
            memcpy(auth_data->signature, cmd + len + 2, len1);
            sumlen = sumlen + len1 + 2;


            len = sumlen;
            len1 = *((unsigned short *) (cmd + len));
            auth_data->cerlen = len1;
            memcpy(auth_data->sender_cer, cmd + len + 2, len1);
            sumlen = sumlen + len1 + 2;

            if (cmdlen != sumlen + 2) {
                printf("cmdlen error!\n");
                return -1;
            }
            return 0;
        }
    }
}