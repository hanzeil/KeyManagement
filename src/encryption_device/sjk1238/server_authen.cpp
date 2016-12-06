#ifndef SERVER_AUTHEN_H
#define SERVER_AUTHEN_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct L {
  int len,tag;
} Len;

typedef struct {
	char s1[50],s2[50];
} TLV;

typedef struct {
	char s1[50],s2[5000];
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
	char s1[50],s2[50];
} sA[7],is[6];

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

static int datalen = 0;
static int publen = 0;
static unsigned char pubkey[256];
char s[5000];
static int nc=0,tis=0;
int bk=1;
int btag=1;//0-隐式  1-显式

void sAfill();//绑定OID
void isFill();//绑定RDN
void fill(int);//switch结构，把证书结构的各字段调用tlv函数的序号与证书结构内容绑定一起，对ca_cer结构进行填充
Len tlv();//TLV匹配的递归
void bitfill(int);//从文件里获取连续字节码(字符串)，赋给字符串s
void output();//依次输出ca_cer内容

#define BUFFERLEN 2048
static unsigned char filearray[BUFFERLEN];
static int myindex = 0;

/*
	[pos]:	init a L
	[ret]:
			struct Len 
*/
Len L_init(int len,int tag)
{
	Len l;
	l.len = len;
	l.tag = tag;
	return l;
}

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
int parce_cer(unsigned char *cerdata, int cerlen, unsigned char *signkey)
{
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
	memcpy(filearray,cerdata,cerlen);
	tlv();
	memset(signkey,0x0,132);
	memcpy(signkey,&pubkeylen,4);
	memcpy(signkey + 36,  pubkey+2, 32);
	memcpy(signkey + 100,  pubkey + 34, 32);


	nc=0;
	tis=0;
	bk=1;
	myindex = 0;
	 //fclose(fp);
	 //output();
	 return 0;
}

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
void fill(int n){
	 int i=0;
     switch(n){//表示第几次调用tlv
         case 4:
             strcpy(ca_cer.cat.version.s1,"version:   ");
             if(strcmp(s,"0")==0)   	strcpy(s,"v1");
             else if(strcmp(s,"1")==0)  strcpy(s,"v2");
             else    					strcpy(s,"v3");
             strcpy(ca_cer.cat.version.s2, s);
             break;
         case 5:
             strcpy(ca_cer.cat.serialNumber.s1,"serialNumber:   ");
             strcpy(ca_cer.cat.serialNumber.s2, s);
             break;
         case 7:
             strcpy(ca_cer.cat.signature.algorithm.s1,"name of algorithm of signature:   ");
             for(i=0;i<4;i++){
                 if(strcmp(s, sA[i].s1)==0){
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
             for(i=0;i<4;i++){
                 if(strcmp(s, is[i].s1)==0){
                     strcpy(ca_cer.cat.issuer_[i].s1, is[i].s2);
                     strcat(ca_cer.cat.issuer_[i].s1, "of issuer:\t");
                     tis=i;
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
             for(i=0;i<4;i++){
                 if(strcmp(s, is[i].s1)==0){
                     strcpy(ca_cer.cat.subject_[i].s1, is[i].s2);
                     strcat(ca_cer.cat.subject_[i].s1, "of subject:\t");
                     tis=i;
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
             strcpy(ca_cer.cat.subjectPublicKeyInfo.algorithm.s1,"name of algorithm of subjectPublicKey:   ");
             for(i=0;i<7;i++){
                 if(strcmp(s, sA[i].s1)==0){
                     strcpy(ca_cer.cat.subjectPublicKeyInfo.algorithm.s2, sA[i].s2);
                     break;
                 }
             }
             break;
         case 49:
             strcpy(ca_cer.cat.subjectPublicKeyInfo.parameters.s1, "parameters of algorithm of subjectPublicKey:   ");
             strcpy(ca_cer.cat.subjectPublicKeyInfo.parameters.s2, s);
             break;
#endif
         case 50:
             strcpy(ca_cer.cat.subjectPublicKeyInfo.PKey.s1, "subjectPublicKey:   ");
             strcpy(ca_cer.cat.subjectPublicKeyInfo.PKey.s2, s);

			publen = datalen;

			memcpy(pubkey, s, publen);	/// out
             break;
         case 61:
             strcpy(ca_cer.casa.algorithm.s1, "name of signatureAlgorithm:   ");
             for(i=0;i<7;i++){
                 if(strcmp(s,sA[i].s1)==0){
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
             bk=0;
             break;
     }
}

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
Len tlv()
{
	int i=0;
	if(bk==0)   return L_init(1000,0);
	nc++;
	//printf("the %dth recall tlv\n",nc);
	bool b=true;
	//unsigned char type=fgetc(fp);//type
	//unsigned char len0=fgetc(fp);//len

	unsigned char type=filearray[myindex++];//type
	unsigned char len0=filearray[myindex++];//len
	int len=len0;
	int lem=0;
	//printf("type = %x,len0 = %x\n",type,len0);
	if(type<0xa0)
	{
		if(type==1)
		{
			unsigned char vc=filearray[myindex++];//fgetc(fp);
			if(vc==0)
				strcpy(s,"FALSE");
			else
				strcpy(s,"TRUE");
		}
		else if(type==2 || type==3 || type==4)
		{
			if(len0>0x80)
			{
				int tn2=len0-0x80;
				unsigned char tl;
				len=0;
				for(i=0;i<tn2;i++)
				{
					tl=filearray[myindex++];//fgetc(fp);
					len*=256;
					len+=tl;
				}
			}
			bitfill(len);
		}
		else if(type==5)
		{
			strcpy(s,"NULL");
		}
		else if(type==6)
		{
#if 1
			strcpy(s,"");
			int dd=len0;
			unsigned char tl=filearray[myindex++];//fgetc(fp);
			int d=tl/40;
			char ts2[10];
			sprintf(ts2,"%d",d);
			strcat(s,ts2);
			strcat(s,".");
			d=tl-d*40;
			sprintf(ts2,"%d",d);
			strcat(s,ts2);
			for(i=1;i<dd;i++)
			{
				strcat(s,".");
				i--;
				int t=0;
				while(1)
				{
					tl=filearray[myindex++];//fgetc(fp);
					i++;
					bool b2=false;
					if(tl&0x80)
					{
						b2=true;
					}
					if(b2)
					{
						tl&=0x7f;
					}
					t*=128;
					t+=tl;
					if(!b2) break;
				}
				sprintf(ts2,"%d",t);
				strcat(s,ts2);
			}
#endif
		}
		else if(type==0xc)
		{
			int issuerlen=len0;
			//fread(s,1,issuerlen,fp);
			memcpy(s,filearray,issuerlen);
			myindex+=issuerlen;
			s[issuerlen]='\0';
		}
		else if(type==0x13 || type==0x17 || type==0x18)
		{
			int d=len0;
			//fread(s,1,d,fp);
			memcpy(s,filearray,d);
			myindex+=d;
			s[d]='\0';
		}
		else if(type==0x30||type==0x31)
		{
			b=false;
			if(len0>0x80)
			{
				len=0;
				len0-=0x80;
				unsigned char tl;
				for(i=0;i<len0;i++)
				{
					tl=filearray[myindex++];//fgetc(fp);
					len*=256;
					len+=tl;
				}
			}
			int dlen=len;
			//printf("the %dth len = %x\n",nc,len);
			while(dlen>0)
			{
				//printf("xxxxxxxxxxxx\n");
				int tmplength = tlv().len;
				dlen-=tmplength;
				//printf("yyyyyyyyyyyy,first dlen=%d\n",dlen);
			}
		}
		else
		{
			printf("the cer has errors!\n");
			exit(0);
		}
	}
	else	///type>=0xa0
	{
		b=false;
		lem=type-0xa0;
		if(len0>0x80)
		{
			int tn2=len0-0x80;
			unsigned char tl;
			len=0;
			for(i=0;i<tn2;i++)
			{
				tl=filearray[myindex++];//fgetc(fp);
				len*=256;
				len+=tl;
			}
		}
		if(btag)
		{
			//这里做个简化，对扩展域进行忽略处理。
			if(nc==67)  //fseek(fp,len,SEEK_CUR);
			{
				/*nothing to be done*/
			}
			else    tlv();
		}
		else
		{
			//这里不作具体实现，依具体类型的证书而定
		}
	}
	if(b)   fill(nc);
	return L_init(len,lem);
}

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
void bitfill(int dd){
    memcpy(s,filearray+myindex,dd);
    myindex+=dd;
    //LOG_DATA(s,dd);
    s[dd]='\0';
    datalen = dd;
}

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

void output(){
     puts("ca.cer parser as follows:");
     printf("[version]  %s%s\n",ca_cer.cat.version.s1,ca_cer.cat.version.s2);
     //printf("[serialNumber]  %s%s\n",ca_cer.cat.serialNumber.s1,ca_cer.cat.serialNumber.s2);
     //printf("[signature.algorithm]  %s%s\n",ca_cer.cat.signature.algorithm.s1,ca_cer.cat.signature.algorithm.s2);
     //printf("[signature.parameters]  %s%s\n",ca_cer.cat.signature.parameters.s1,ca_cer.cat.signature.parameters.s2);
     //printf("[issuer ID]  issuer\n%s%s\n%s%s\n%s%s\n%s%s\n%s%s\n%s%s\n",ca_cer.cat.issuer_[0].s1,ca_cer.cat.issuer_[0].s2,ca_cer.cat.issuer_[1].s1,ca_cer.cat.issuer_[1].s2,ca_cer.cat.issuer_[2].s1,ca_cer.cat.issuer_[2].s2,ca_cer.cat.issuer_[3].s1,ca_cer.cat.issuer_[3].s2,ca_cer.cat.issuer_[4].s1,ca_cer.cat.issuer_[4].s2,ca_cer.cat.issuer_[5].s1,ca_cer.cat.issuer_[5].s2);
     //printf("[validity]  validity:      %s-%s\n",ca_cer.cat.validity[0].s2,ca_cer.cat.validity[1].s2);
     //printf("[subject]  subject\n%s%s\n%s%s\n%s%s\n%s%s\n%s%s\n%s%s\n",ca_cer.cat.subject_[0].s1,ca_cer.cat.subject_[0].s2,ca_cer.cat.subject_[1].s1,ca_cer.cat.subject_[1].s2,ca_cer.cat.subject_[2].s1,ca_cer.cat.subject_[2].s2,ca_cer.cat.subject_[3].s1,ca_cer.cat.subject_[3].s2,ca_cer.cat.subject_[4].s1,ca_cer.cat.subject_[4].s2,ca_cer.cat.subject_[5].s1,ca_cer.cat.subject_[5].s2);
     //printf("[subjectPublicKeyInfo.algorithm]  %s%s\n",ca_cer.cat.subjectPublicKeyInfo.algorithm.s1,ca_cer.cat.subjectPublicKeyInfo.algorithm.s2);
     //printf("[subjectPublicKeyInfo.parameters]  %s%s\n",ca_cer.cat.subjectPublicKeyInfo.parameters.s1,ca_cer.cat.subjectPublicKeyInfo.parameters.s2);
     printf("[subjectPublicKeyInfo]  %s%s\n",ca_cer.cat.subjectPublicKeyInfo.PKey.s1,ca_cer.cat.subjectPublicKeyInfo.PKey.s2);
     //printf("[issuerUniqueID]  issuerUniqueID:     无\n");
     //printf("[subjectUniqueID]  subjectUniqueID:     无\n");
     //printf("[extendsions]  extendsions:     省略\n");
     //printf("[casa.algorithm]  %s%s\n",ca_cer.casa.algorithm.s1,ca_cer.casa.algorithm.s2);
     //printf("[casa.parameters]  %s%s\n",ca_cer.casa.parameters.s1,ca_cer.casa.parameters.s2);
     printf("[casv.signatureValue]  %s%s\n",ca_cer.casv.signatureValue.s1,ca_cer.casv.signatureValue.s2);
}

char realkey[4] = {0};

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
int Parser_cmd(unsigned char *cmd, int cmdlen,AUTHEN_DATA *auth_data)
{
	printf("in Parser_cmd\n");
	char adminkey[4] 	= {0x35,0x37,0x33,0x46};
	char userkey[4] 	= {0x36,0x30,0x33,0x46};
	unsigned short len,len1;
	unsigned int sumlen = 0;
	len = 2;
	len1= *((unsigned short*)cmd);
	auth_data->devIDlen = len1;
	memcpy(auth_data->devID,cmd+len ,len1);
	sumlen=sumlen+len1+2;

	//printf("len1=%d\n",len1);
	//LOG_DATA(auth_data->devID,len1);
	//LOG("\n\n");
	//LOG_DATA(adminkey,4);
	//LOG("\n\n");
	//LOG_DATA((auth_data->devID+20),4);
	//LOG("\n\n");

	int ret = -1;
	int ret1 = -1;
	memcpy(realkey,auth_data->devID+20,4);

	ret = memcmp(realkey,adminkey,4);
	ret1 = memcmp(realkey,userkey,4);

	if((ret==0)||(ret1==0))
	{
		/*nothing to be done!*/
	}
	else
	{
		printf("no adminstrator right!\n");
		return -1;

	}


	len = sumlen;
	len1 = *((unsigned short*)(cmd+len));
	auth_data->randlen = len1;
	memcpy(auth_data->rand,cmd+len+2,len1);
	sumlen=sumlen+len1+2;


	len = sumlen;
	len1 = *((unsigned short*)(cmd+len));
	auth_data->siglen = len1;
	memcpy(auth_data->signature,cmd+len+2,len1);
	sumlen=sumlen+len1+2;


	len = sumlen;
	len1 = *((unsigned short*)(cmd+len));
	auth_data->cerlen = len1;
	memcpy(auth_data->sender_cer,cmd+len+2,len1);
	sumlen=sumlen+len1+2;

	if(cmdlen!=sumlen+2)
	{
		printf("cmdlen error!\n");
		return -1;
	}
	return 0;
}

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

