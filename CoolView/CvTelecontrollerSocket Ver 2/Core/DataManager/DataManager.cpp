/*
 * 各遥控器独立数据和全局数据管理类 cpp文件
 * 2011-12-08   V1.0.0.1    By Lzy 1、因DataTree模块增加了存储控制而对本模块的添加操作做相应的支持调整
 *                                 2、添加操作增加对重名数据的检查，防止加入重名数据
 *                                 3、把原来Add，Modify，Get，Delete，Clear五类操作函数简化为Set，Get两类操作函数
 * 2011-12-07   V1.0.0.0    By Lzy
 * 2011-12-05   V0.0.0.0    By Lzy 创建文件
 */

#include "DataManager.h"
#include <string>

CDataManager *CDataManager::m_pDataManager = NULL;

//Set、Get操作函数的头段通用代码
#define OPERATION_PRE_CODE(GLoc,OLoc)\
    if (pszDataName == NULL)\
        return false;\
    if (TryEnterCS(this->m_pSection,this->m_bObjectDestruct) == false)\
        return false;\
    if (pszAccountName == NULL)\
        this->m_szPath = GLoc;\
    else\
    {\
        this->m_szPath = pszAccountName;\
        this->m_szPath.append(OLoc);\
    }\
    this->m_szPath.append(pszDataName);\

/*
 * 【公开】【静态】获取对象单例指针
 * @return CDataManager *,返回单例指针
 */
CDataManager *CDataManager::GetInstance()
{
    if (CDataManager::m_pDataManager == NULL)
        CDataManager::m_pDataManager = new CDataManager;
    return CDataManager::m_pDataManager;
}

/*
 * 【公开】【静态】释放单例对象
 * @return void
 */
void CDataManager::ReleaseInstance()
{
    if (CDataManager::m_pDataManager != NULL)
    {
        delete CDataManager::m_pDataManager;
        CDataManager::m_pDataManager = NULL;
    }
}

/*
 * 【私有】类构造函数
 */
CDataManager::CDataManager()
{
    this->m_pDataTree = CDataTree::GetInstance();
    this->m_pDataTree->LoadFromXML(LOAD_SAVE_DATA_FILE_PATH);
    this->m_bObjectDestruct = false;
    this->m_pSection = CreateCS();
}

/*
 * 【私有】类析构函数
 */
CDataManager::~CDataManager()
{
    this->m_bObjectDestruct = true;
    EnterCS(this->m_pSection);
    this->m_pDataTree->SaveToXML(LOAD_SAVE_DATA_FILE_PATH);
    CDataTree::ReleaseInstance();
    ReleaseCS(this->m_pSection);
}

/*
 * 【公开】设置long型数据
 * @param long lValue,新增的long型数据值
 * @param bool bCanSave,指定新增的数据是否最后会存储到硬盘中
 * @param const char *pszDataName,新增数据的数据名
 * @param const char *pszAccountName = NULL,数据属区，访问特定遥控器的专属数据使用该遥控器的sip帐号名区分，默认的NULL值表示访问所有遥控器共享的数据
 * @return bool,返回操作是否成功，成功返回true
 */
bool CDataManager::SetDataLong(long lValue,bool bCanSave,const char *pszDataName,const char *pszAccountName)
{
    OPERATION_PRE_CODE("Global/L/","_/L/")
    bool bResult;
    this->m_pDataTree->GetDataLong(this->m_szPath.c_str(),0,&bResult);
    if (bResult == true)
    {
        bResult = this->m_pDataTree->ModifyDataLong(this->m_szPath.c_str(),lValue);
        if (bResult == true)
            this->m_pDataTree->SetSaveFlag(this->m_szPath.c_str(),bCanSave,false);
    }
    else
        bResult = this->m_pDataTree->AddDataLong(this->m_szPath.c_str(),lValue,bCanSave,true);
    ExitCS(this->m_pSection);
    return bResult;
}

/*
 * 【公开】设置浮点型数据
 * @param double fValue,新增的浮点型数据值
 * @param bool bCanSave,指定新增的数据是否最后会存储到硬盘中
 * @param const char *pszDataName,新增数据的数据名
 * @param const char *pszAccountName = NULL,数据属区，访问特定遥控器的专属数据使用该遥控器的sip帐号名区分，默认的NULL值表示访问所有遥控器共享的数据
 * @return bool,返回操作是否成功，成功返回true
 */
bool CDataManager::SetDataFloat(double fValue,bool bCanSave,const char *pszDataName,const char *pszAccountName)
{
    OPERATION_PRE_CODE("Global/F/","_/F/")
    bool bResult;
    this->m_pDataTree->GetDataFloat(this->m_szPath.c_str(),0.0,&bResult);
    if (bResult == true)
    {
        bResult = this->m_pDataTree->ModifyDataFloat(this->m_szPath.c_str(),fValue);
        if (bResult == true)
            this->m_pDataTree->SetSaveFlag(this->m_szPath.c_str(),bCanSave,false);
    }
    else
        bResult = this->m_pDataTree->AddDataFloat(this->m_szPath.c_str(),fValue,bCanSave,true);
    ExitCS(this->m_pSection);
    return bResult;
}

/*
 * 【公开】设置字符串型数据
 * @param const char *pszValue,新增的字符串型数据值
 * @param bool bCanSave,指定新增的数据是否最后会存储到硬盘中
 * @param const char *pszDataName,新增数据的数据名
 * @param const char *pszAccountName = NULL,数据属区，访问特定遥控器的专属数据使用该遥控器的sip帐号名区分，默认的NULL值表示访问所有遥控器共享的数据
 * @return bool,返回操作是否成功，成功返回true
 */
bool CDataManager::SetDataString(const char *pszValue,bool bCanSave,const char *pszDataName,const char *pszAccountName)
{
    OPERATION_PRE_CODE("Global/S/","_/S/")
    bool bResult;
    const char *pszResult = this->m_pDataTree->GetDataStr(this->m_szPath.c_str());
    if (pszResult != NULL)
    {
        bResult = this->m_pDataTree->ModifyDataStr(this->m_szPath.c_str(),pszValue);
        if (bResult == true)
            this->m_pDataTree->SetSaveFlag(this->m_szPath.c_str(),bCanSave,false);
    }
    else
        bResult = this->m_pDataTree->AddDataStr(this->m_szPath.c_str(),pszValue,bCanSave,true);
    ExitCS(this->m_pSection);
    return bResult;
}

/*
 * 【公开】获取long型数据
 * @param const char *pszDataName,要获取数据的数据名
 * @param const char *pszAccountName = NULL,要获取数据的属区，访问特定遥控器的专属数据使用该遥控器的sip帐号名区分，默认的NULL值表示访问所有遥控器共享的数据
 * @param long lDefault = 0,当获取失败时返回的默认值
 * @return long,返回获取到的值，获取失败时将返回lDefault的值
 */
long CDataManager::GetDataLong(const char *pszDataName,const char *pszAccountName,long lDefault)
{
    OPERATION_PRE_CODE("Global/L/","_/L/")
    long lResult = this->m_pDataTree->GetDataLong(this->m_szPath.c_str(),lDefault);
    ExitCS(this->m_pSection);
    return lResult;
}

/*
 * 【公开】获取浮点型数据
 * @param const char *pszDataName,要获取数据的数据名
 * @param const char *pszAccountName = NULL,要获取数据的属区，访问特定遥控器的专属数据使用该遥控器的sip帐号名区分，默认的NULL值表示访问所有遥控器共享的数据
 * @param double fDefault = 0.0,当获取失败时返回的默认值
 * @return double,返回获取到的值，获取失败时将返回fDefault的值
 */
double CDataManager::GetDataFloat(const char *pszDataName,const char *pszAccountName,double fDefault)
{
    OPERATION_PRE_CODE("Global/F/","_/F/")
    double fResult = this->m_pDataTree->GetDataFloat(this->m_szPath.c_str(),fDefault);
    ExitCS(this->m_pSection);
    return fResult;
}

/*
 * 【公开】获取字符串型数据
 * @param const char *pszDataName,要获取数据的数据名
 * @param const char *pszAccountName = NULL,要获取数据的属区，访问特定遥控器的专属数据使用该遥控器的sip帐号名区分，默认的NULL值表示访问所有遥控器共享的数据
 * @return const char *,返回获取到的字符串指针，获取失败时将返回NULL
 */
const char *CDataManager::GetDataString(const char *pszDataName,const char *pszAccountName)
{
    OPERATION_PRE_CODE("Global/S/","_/S/")
    const char *pszResult = this->m_pDataTree->GetDataStr(this->m_szPath.c_str());
    ExitCS(this->m_pSection);
    return pszResult;
}
