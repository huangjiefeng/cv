/*
 * 各遥控器独立数据和全局数据管理类 头文件
 * 2011-12-08   V1.0.0.1    By Lzy 1、因DataTree模块增加了存储控制而对本模块的添加操作做相应的支持调整
 *                                 2、添加操作增加对重名数据的检查，防止加入重名数据
 *                                 3、把原来Add，Modify，Get，Delete，Clear五类操作函数简化为Set，Get两类操作函数
 * 2011-12-07   V1.0.0.0    By Lzy
 * 2011-12-05   V0.0.0.0    By Lzy 创建文件
 */

#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#define LOAD_SAVE_DATA_FILE_PATH "cfg\\Data.xml" //定义读入/和写入数据的xml文件的位置

#include "DataTree.h"
#include "CommonThread.h"

using namespace thread_operation;
using namespace std;

class CDataManager
{
public:
    //------函数------
    static CDataManager *GetInstance();//获取对象单例指针
    static void ReleaseInstance();//释放单例对象

    long GetDataLong(const char *pszDataName,const char *pszAccountName = NULL,long lDefault = 0);//获取long型数据
    double GetDataFloat(const char *pszDataName,const char *pszAccountName = NULL,double fDefault = 0.0);//获取浮点型数据
    const char *GetDataString(const char *pszDataName,const char *pszAccountName = NULL);//获取字符串型数据

    bool SetDataLong(long lValue,bool bCanSave,const char *pszDataName,const char *pszAccountName = NULL);//设置long型数据
    bool SetDataFloat(double fValue,bool bCanSave,const char *pszDataName,const char *pszAccountName = NULL);//设置浮点型数据
    bool SetDataString(const char *pszValue,bool bCanSave,const char *pszDataName,const char *pszAccountName = NULL);//设置字符串型数据

private:
    //------函数------
    CDataManager();
    ~CDataManager();
    CDataManager(const CDataManager &){};
    const CDataManager *operator =(const CDataManager &){};
    //------变量------
    static CDataManager *m_pDataManager;//对象单例
    CDataTree *m_pDataTree;//存放CDataTree实例指针
    string m_szPath;//专作为分析路径的缓存变量使用
    CRITICAL_SECTION *m_pSection;//操作临界区
    bool m_bObjectDestruct;//指示对象是否要被析构，用于控制临界区的退出
};

#endif
