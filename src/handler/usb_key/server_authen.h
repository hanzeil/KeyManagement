#ifndef SERVER_AUTHEN_H
#define SERVER_AUTHEN_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "skf_type.h"


extern DEVHANDLE hDev;

namespace handler {

    namespace usb_key {


        typedef struct L {
            int len, tag;
        } Len;

        typedef struct authen_data {
            unsigned short randlen;
            unsigned short devIDlen;
            unsigned short siglen;
            unsigned short cerlen;
            unsigned char devID[32];
            unsigned char rand[20];
            unsigned char signature[256];
            unsigned char sender_cer[2048];
        } AUTHEN_DATA;

        void sAfill();//绑定OID
        void isFill();//绑定RDN
        void fill(int);//switch结构，把证书结构的各字段调用tlv函数的序号与证书结构内容绑定一起，对ca_cer结构进行填充
        Len tlv();//TLV匹配的递归
        void bitfill(int);//从文件里获取连续字节码(字符串)，赋给字符串s
        void output();//依次输出ca_cer内容


/*
	[pos]:	init a L
	[ret]:
			struct Len 
*/
        Len L_init(int len, int tag);

/*
	[pos]:	parse certificate to get public key
	[out]:
			signkey --------
	[_o_]:
			filearray[] ---------
	[_i_]:
			pubkey[] --------
	[mark]:
			signkey:  (format: offset-length )
					  0-4, 36-32, 100-32
*/
        int parce_cer(unsigned char *cerdata, int cerlen, unsigned char *signkey);

#if 0
        void sAfill(){
             strcpy(sA[0].s1,"1.2.840.10040.4.1");
             strcpy(sA[0].s2,"DSA");
             strcpy(sA[1].s1,"1.2.840.10040.4.3");
             strcpy(sA[1].s2,"sha1DSA");
             strcpy(sA[2].s1,"1.2.840.113549.1.1.1");
             strcpy(sA[2].s2,"RSA");
             strcpy(sA[3].s1,"1.2.840.113549.1.1.2");
             strcpy(sA[3].s2,"md2RSA");
             strcpy(sA[4].s1,"1.2.840.113549.1.1.3");
             strcpy(sA[4].s2,"md4RSA");
             strcpy(sA[5].s1,"1.2.840.113549.1.1.4");
             strcpy(sA[5].s2,"md5RSA");
             strcpy(sA[6].s1,"1.2.840.113549.1.1.5");
             strcpy(sA[6].s2,"sha1RSA");
        }

        void isFill(){
             strcpy(is[0].s1,"2.5.4.6");
             strcpy(is[0].s2,"Country ");
             strcpy(is[1].s1,"2.5.4.8");
             strcpy(is[1].s2,"Sate or province name ");
             strcpy(is[2].s1,"2.5.4.7");
             strcpy(is[2].s2,"Locality ");
             strcpy(is[3].s1,"2.5.4.10");
             strcpy(is[3].s2,"Organization name ");
             strcpy(is[4].s1,"2.5.4.11");
             strcpy(is[4].s2,"Organizational Unit name ");
             strcpy(is[5].s1,"2.5.4.3");
             strcpy(is[5].s2,"Common Name ");
        }
#endif

/*
	[pos]:	via param(n)  fill global var
	[_o_]:
			pubkey ------------
			ca_cer ------------
	[_i_]:
			s ---------
			sA[] ---------
			tis --------
			is[] --------
*/

/*
	[pos]:	parse type-length-value
	[_o_]:
			myindex ---------
			nc ------- 
			s -------
	[_i_]:
			bk --------
			filearray[] ---------
*/

/*
	[pos]:	fill data
	[_o_]:
			s --------
			datalen -------
	[_io_]:
			myindex ------
	[_i_]:
			filearray ------
*/

#if 0
        void bitfill(int dd){
             int i=0;
             strcpy(s,"");
             for(i=0;i<dd;i++){
                 unsigned char tl=filearray[myindex++];//fgetc(fp);
                 int d=tl;
                 char ts2[10];
                 sprintf(ts2,"%02x",d);
                 strcat(s,ts2);
             }
        }
#endif



/*
	[pos]:	parse cmd
	[out]:
			auth_data ---------
	[_o_]:
			realkey[] ---------
	[mark]:
			cmd format:  len1(2), devID(len1){+20,realkey(4)}, 
						 len2(2), rand(len2), 
						 len3(2), sign(len3), 
						 len4(2), cer(len4)
*/
        int Parser_cmd(unsigned char *cmd, int cmdlen, AUTHEN_DATA *auth_data);

/*
	[pos]:	DevID auth	
	[ret]:
			0 ------- OK;  ~0 ------- ERR
	[mark]:
			data format:	len(2), data(len), 
*/
/*
int DevID_auth(unsigned char *cmdin,unsigned char *cmdout,int *cmdout_len)
{
	int ret = -1;
	int cmdlen=0;
	AUTHEN_DATA auth_data;

	unsigned char pubdata[256],sigdata[256];
#if 0
	if((0xF0 == cmd[0])&&(0xA0 == cmd[0]))
	{
		cmdlen=*(unsigned short*)(&cmd[1]);
	}
	else
	{
		return -1;
	}
#endif

	//cmdlen=*(unsigned short*)cmdin;
	cmdlen = cmdin[1]*256+cmdin[0];
	printf("cmdlen = %d\n",cmdlen);
	if(Parser_cmd(cmdin+2,cmdlen,&auth_data))
	{
		printf("parser_cmd error!\n");
		return -1;
	}

	//memcpy(filearray,auth_data.sender_cer,auth_data.cerlen);
	parce_cer(auth_data.sender_cer,auth_data.cerlen,pubdata);

#if 0
	//int pubkeylen = strlen(ca_cer.cat.subjectPublicKeyInfo.PKey.s2);
	memcpy(pubdata,pubkey+2,publen-2);
	//printf("pubkeylen=%d\n",publen);
	//LOG_DATA(pubdata,publen-2);

	//int signaturelen = strlen(ca_cer.casv.signatureValue.s2);
	//memcpy(pubdata,ca_cer.casv.signatureValue.s2+2,pubkeylen-2);

	memset(pubdata,0x0,256);

	int pubkeylen = 256;
	memcpy(pubdata,&pubkeylen,4);
	memcpy(pubdata + 36,  pubkey+2, 32);
	memcpy(pubdata + 100,  pubkey + 34, 32);
#endif

	//LOG_DATA(pubdata,256);

	printf("signature len is %d\n",sizeof(ECCSIGNATUREBLOB));

	ret = SKF_LockDev(hDev, -1);							///+SKF
	if(ret)
	{
		LOG("SKF_LockDev error1 0x%x[%s]\n", ret, __func__);
		return 1;
	}

	ret = SKF_ExtECCVerify(hDev, pubdata,					///+SKF
			auth_data.rand, auth_data.randlen,
			(ECCSIGNATUREBLOB*)auth_data.signature);
	if(ret)
	{
		printf("SKF_ExtECCVerify error : 0x%x\n", ret);
		return ret;
	}
	else
	{
		printf("Signature Verify OK\n");
		ret=generate_auth_data(cmdout,cmdout_len);
		if(ret)
		{
			printf("generate_auth_data error!\n");
			ret = SKF_UnlockDev(hDev);
			if(ret)
			{
				LOG("SKF_LockDev error2 0x%x[%s]\n", ret, __func__);
				return 1;
			}
			return ret;
		}
	}
	ret = SKF_UnlockDev(hDev);								///-SKF
	if(ret)
	{
		LOG("SKF_LockDev error3 0x%x[%s]\n", ret, __func__);
		return 1;
	}
	return 0;
}
*/
/*
int DevID_auth(unsigned char *cmdin,int *cmdout_len)
{
	
}
*/

    }
}
#if 0
void output(){
     puts("ca.cer解析如下：");
     printf("【版本】%s%s\n",ca_cer.cat.version.s1,ca_cer.cat.version.s2);
     printf("【序列号】%s%s\n",ca_cer.cat.serialNumber.s1,ca_cer.cat.serialNumber.s2);
     printf("【签名算法】%s%s\n",ca_cer.cat.signature.algorithm.s1,ca_cer.cat.signature.algorithm.s2);
     printf("【签名算法的参数】%s%s\n",ca_cer.cat.signature.parameters.s1,ca_cer.cat.signature.parameters.s2);
     printf("【签发者标识信息】issuer\n%s%s\n%s%s\n%s%s\n%s%s\n%s%s\n%s%s\n",ca_cer.cat.issuer_[0].s1,ca_cer.cat.issuer_[0].s2,ca_cer.cat.issuer_[1].s1,ca_cer.cat.issuer_[1].s2,ca_cer.cat.issuer_[2].s1,ca_cer.cat.issuer_[2].s2,ca_cer.cat.issuer_[3].s1,ca_cer.cat.issuer_[3].s2,ca_cer.cat.issuer_[4].s1,ca_cer.cat.issuer_[4].s2,ca_cer.cat.issuer_[5].s1,ca_cer.cat.issuer_[5].s2);
     printf("【有效期】validity:      %s-%s\n",ca_cer.cat.validity[0].s2,ca_cer.cat.validity[1].s2);
     printf("【主体标识信息】subject\n%s%s\n%s%s\n%s%s\n%s%s\n%s%s\n%s%s\n",ca_cer.cat.subject_[0].s1,ca_cer.cat.subject_[0].s2,ca_cer.cat.subject_[1].s1,ca_cer.cat.subject_[1].s2,ca_cer.cat.subject_[2].s1,ca_cer.cat.subject_[2].s2,ca_cer.cat.subject_[3].s1,ca_cer.cat.subject_[3].s2,ca_cer.cat.subject_[4].s1,ca_cer.cat.subject_[4].s2,ca_cer.cat.subject_[5].s1,ca_cer.cat.subject_[5].s2);
     printf("【公钥的加密算法】%s%s\n",ca_cer.cat.subjectPublicKeyInfo.algorithm.s1,ca_cer.cat.subjectPublicKeyInfo.algorithm.s2);
     printf("【公钥的加密算法参数】%s%s\n",ca_cer.cat.subjectPublicKeyInfo.parameters.s1,ca_cer.cat.subjectPublicKeyInfo.parameters.s2);
     printf("【公钥数据】%s%s\n",ca_cer.cat.subjectPublicKeyInfo.PKey.s1,ca_cer.cat.subjectPublicKeyInfo.PKey.s2);
     printf("【签发者唯一标识符】issuerUniqueID:     无\n");
     printf("【主体唯一标识符】subjectUniqueID:     无\n");
     printf("【扩展】extendsions:     省略\n");
     printf("【签名算法】%s%s\n",ca_cer.casa.algorithm.s1,ca_cer.casa.algorithm.s2);
     printf("【签名算法的参数】%s%s\n",ca_cer.casa.parameters.s1,ca_cer.casa.parameters.s2);
     printf("【签名结果】%s%s\n",ca_cer.casv.signatureValue.s1,ca_cer.casv.signatureValue.s2);
}
#endif

#endif

