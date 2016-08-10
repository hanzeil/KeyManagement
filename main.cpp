#include <iostream>
#include <iomanip>
#include <swsds.h>
#include <cstring>
#include "DBFactoryInterface.h"
#include "MysqlFactory.h"
#include "DBProductInterface.h"
#include <chrono>

using namespace std;

#define MAX_BLOCK_LEN 100

int main() {
    Camera c;
    unsigned char key[] = "Hello world";
    Key k(c, key);
    DBFactoryInterface *factory = new MysqlFactory();
    DBProductInterface *db = factory->createProduct();
    db->connect("keymanagement", "keymanagement");
    db->insert_key(k);
    /*
    unsigned char key[128] = "Hello World";
    Key k("key1", key);
    std::cout<<db->insert_key(k);
    */
    //std::cout<<"d"<<std::endl;
    /*
    int iRet = -1;
    void *pDevHandle = NULL;//设备句柄
    void *pSesHandle = NULL;//会话句柄
    void *pKeyHandle = NULL; //会话密钥句柄
    unsigned int uiKeyIndex = 1; //存储公钥的索引值
    unsigned int uiAlgID = SGD_SMS4_ECB;//可以替换为密码卡支持的任意一种对称算法识；
    unsigned char ucIV[16] = {0};
    ECCCipher pubKey;
    iRet = SDF_OpenDevice(&pDevHandle);//打开设备句柄
    if (iRet)
        return iRet;
    iRet = SDF_OpenSession(pDevHandle, &pSesHandle);//打开会话句柄
    if (iRet)
        return iRet;
    //生成会话密钥并用ECC加密输出
    iRet = SDF_GenerateKeyWithIPK_ECC(pSesHandle, uiKeyIndex, 256, &pubKey, &pKeyHandle);
    if (iRet)
        return iRet;
    //导入会话密钥并用内部ECC私钥解密
    iRet = SDF_ImportKeyWithISK_ECC(pSesHandle, uiKeyIndex, &pubKey, &pKeyHandle);
    if (iRet)
        return iRet;
    iRet = SDF_CloseSession(pSesHandle);//关闭会话句柄
    if (iRet)
        return iRet;
    iRet = SDF_CloseDevice(pDevHandle);//关闭设备句柄
    if (iRet)
        return iRet;
    std::cout << "over" << std::endl;
     */
    return 0;


}