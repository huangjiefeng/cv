/*
 * 管理脚本外部数据（存放于XML文件中）类 头文件 By Lzy
 * 2011-12-21   V2.0.1.1    修正新增或修改字符串数据时当传入的数据值指针为NULL会崩溃的问题，现把传入NULL数据指针视为清空字符串的操作
 * 2011-12-08   V2.0.1.0    1、数据节点增加指向父数据节点的指针，访问器增加上级访问函数
 *                          2、增加回写数据控制（可控制哪些数据在保存时不写入到xml文件中）
 * 2011-12-04   V2.0.0.0    大幅度调整存储结构及各种实现，增加了删除操作和保存数据到文件的操作
 * 2011-09-05   V1.0.0.0    创建版本信息
 */

#ifndef DATATREE_H
#define DATATREE_H

#ifdef WIN32
#pragma warning ( disable : 4996 )
#endif

#include <stdio.h>
#include "tinyxml.h"
#include "StrHashMap.h"
#include "SimpleMemPool.h"

#define DIV_PATH_CHAR '/' //指定划分路径层的字符
#define HASH_MAP_SIZE 1000//定义创建的HashMap的大小
#define MAX_XML_FILE_PATH 256 //XML数据配置文件中文件路径最大允许长度
#define MAX_DATATREE_STRING_VALUE_LENGTH 256 //字符串类型的数据中允许的最大字符串长度
#define MAX_DATATREE_NAME_LENGTH 50 //数据名允许的最大字符串长度
#define MAX_DATATREE_PROPERTY_PATH_LENGTH 512 //读取XML时数据路径允许的最大长度
#define MAX_WRITE_XML_LINE_LEN 512 //写入文件时单行XML的最大长度（缓冲区长度）

class CDataTree
{
public:
    //------枚举类型------
    enum EDataType //数据类型枚举
    {
        Type_Long,//long类型
        Type_Float,//浮点类型
        Type_String,//字符串类型，长度限制为MAX_XML_CONFIG_STRING_PROPERTY_VALUE_LENGTH
        Type_Table,//组合类型，其下可包含其它数据类型
        Type_None,//无效节点，此值只有在外部使用SViewNode结构体进行同级遍历时会出现，表示遍历结束
    };
private:
    //------联合体类型------
    union UData //属性数据值存储union
    {
       long m_lData;//Type_Long
       double m_fData;//Type_Float
       char m_szData[MAX_DATATREE_STRING_VALUE_LENGTH];//Type_String
       void *m_pTable;//Type_Table，指向下级数据中最后一个加入的数据
    };
    //------结构体类型------
    struct STreeNode //属性数据存储节点结构
    {
        EDataType m_eDataType;//存储的数据类型
        UData m_uData;//存储的数据值
        char m_szName[MAX_DATATREE_NAME_LENGTH];//存储当前属性的属性名，如果不存在则为空字符串
        bool m_bCanSave;//指明本节点是否在执行写入到文件操作时可以被存储到文件
        STreeNode *m_pParent;//上级存储节点
        STreeNode *m_pNext;//同级中下一个存储节点
        STreeNode *m_pPre;//同级中上一个存储节点
    };
public:
    //------联合体类型------
    union UViewData //供外部访问数据时用的数据存储union
    {
        long m_lData;
        double m_fData;
        const char *m_pszData;
    };
    //------结构体类型------
    struct SViewNode
    {
        UViewData m_uViewData;//数据值
        const char *m_pzsName;//数据名
        //------函数------

        /*
         * 【公开】【内联】获取当前数据的类型
         * @return EDataType,返回当前数据的类型值
         */
        EDataType GetType() {return this->m_eDataType;}
        
        void MoveNext();//访问同级的访问器指向的最后一个有效数据的下一个数据，当已经到达尾部时，数据类型（可用GetType获得）值会被设置为Type_None
        void MovePre();//访问同级的访问器指向的最后一个有效数据的上一个数据，当已经到达头部时，数据类型（可用GetType获得）值会被设置为Type_None
        void MoveSub();//访问当前数据的下级首个数据，只有当前数据类型值为Type_Table时使用此函数才有效，如果下级没有数据或当前数据类型不是Type_Table，则数据类型值会被设置为Type_None
        void MoveParent();//访问当前数据的上级数据，如果上级没有数据，则数据类型值会被设置为Type_None

    private:
        //------变量------
        EDataType m_eDataType;//数据类型
        STreeNode *m_pLink;//当前数据关联的树节点
        //------函数------

        /*
         * 【私有】【内联】设置数据内容
         * @param STreeNode *pTarget,树节点指针
         * @return void
         */
        void _SetData(STreeNode *pTarget)
        {
            if (pTarget != NULL)
            {
                this->m_eDataType = pTarget->m_eDataType;
                this->m_pzsName = pTarget->m_szName;
                switch (pTarget->m_eDataType)
                {
                case CDataTree::Type_Long:
                    this->m_uViewData.m_lData = pTarget->m_uData.m_lData;
                    break;
                case CDataTree::Type_Float:
                    this->m_uViewData.m_fData = pTarget->m_uData.m_fData;
                    break;
                case CDataTree::Type_String:
                    this->m_uViewData.m_pszData = pTarget->m_uData.m_szData;
                    break;
                }
            }
            else
                this->m_eDataType = CDataTree::Type_None;
        }

        friend class CDataTree;
    };
    //------函数------
    static CDataTree *GetInstance();//获取类的单例指针
    static void ReleaseInstance();//析构单例
    bool LoadFromXML(const char *pszXMLFilePath);//从XML文件读取数据
    bool SaveToXML(const char *pszXMLFilePath = NULL);//储存标记为可存储的数据到XML文件中（本函数可能有编码和换行符问题）

    bool GetDataVisitor(const char *pszSearchPath,SViewNode &rViewNode);//获取数据值访问器
    long GetDataLong(const char *pszSearchPath,long lDefault,bool *bGetResult = NULL);//获取long型数据值
    double GetDataFloat(const char *pszSearchPath,double fDefault,bool *bGetResult = NULL);//获取浮点型数据值
    const char * GetDataStr(const char *pszSearchPath);//获取字符串型数据值

    bool ModifyDataVisitor(SViewNode &rViewNode);//通过访问器修改特定数据的值
    bool ModifyDataLong(const char *pszSearchPath,long lNewLong,bool bCanChangeType = false);//修改long型数据的值
    bool ModifyDataFloat(const char *pszSearchPath,double fNewFloat,bool bCanChangeType = false);//修改浮点型数据的值
    bool ModifyDataStr(const char *pszSearchPath,const char *pszNewString,bool bCanChangeType = false);//修改字符串型数据的值

    bool AddDataLong(const char *pszNewPath,long lValue,bool bCanSave,bool bAutoCreatePath = false);//添加新的long型数据值
    bool AddDataFloat(const char *pszNewPath,double fValue,bool bCanSave,bool bAutoCreatePath = false);//添加新的浮点型数据值
    bool AddDataStr(const char *pszNewPath,const char *pszValue,bool bCanSave,bool bAutoCreatePath = false);//添加新的字符串型数据值
    bool AddDataTable(const char *pszNewPath,bool bCanSave,bool bAutoCreatePath = false);//添加新的子数据节点
    
    /*
     * 【公开】【内联】删除指定路径及其下的所有数据
     * @param const char *pszSearchPath,要删除的数据所在路径（查找键）
     * @return void
     */
    void DeleteData(const char *pszSearchPath)
    {
        this->_DeleteData(pszSearchPath,false);
    }

    /*
     * 【公开】【内联】清空全部数据
     * @return void
     */
    void ClearAllData()
    {
        this->m_pHash->Clear();
        simple_mem_pool::RecycleMemory(this->m_pDataMen);//回收内存
        this->m_oHeadNode.m_uData.m_pTable = NULL;
    }

    /*
     * 【公开】【内联】设置存储标记
     * @param const char *pszPath,要修改的存储节点路径，如果修改为可存储到文件（bSavaFlag=true），
     *                            则该路径上所有节点均会被置为可存储，但如果是修改为不可存储，则只是指定节点会被修改。
     *                            此参数允许设置为NULL，如果此时bSetSub设置为true，则表示修改涉及所有节点，否则传入NULL没意义，函数将直接返回true
     * @param bool bSaveFlag,新的存储标记
     * @param bool bSetSub,指明是否把指定路径下所有子节点也应该新的存储标记
     * @return bool,返回设置结果，设置成功返回true
     */
    bool SetSaveFlag(const char *pszPath,bool bSaveFlag,bool bSetSub)
    {
        return this->_SetSaveFlag(pszPath,bSaveFlag,bSetSub);
    }

private:
    //------函数------
    ~CDataTree();
    CDataTree();
    CDataTree(const CDataTree&){};
    STreeNode *_AnalyzeXMLElement(const TiXmlElement *pXMLElement,unsigned long lPathLoc,char *pszPath,STreeNode *pParent);//分析XML中所有同级节点，并据此创建数据
    void _DeleteData(const char *pszSearchPath,bool bReserve);//删除指定路径及其下的所有数据，可指定是否保留指定路径处的节点
    void _RecycleRL(char *pszParentPath,int nLen,STreeNode *pNode);//递归回收与当前节点同级的节点（包括当前节点自身）及属于这些节点之下的子节点
    STreeNode *_PreSetPath(const char *pszPath,bool bAutoCreatePath);//检查给定的路径是否完整（路径上各节点存在，如果设置了自动创建路径，则函数执行过程中会自动创建父数据节点）
    void _WriteXML(FILE *fHandle,STreeNode *pNode,char *pszBuffer,int nUsedSize);//递归遍历数据节点并把标记为可存储的数据写入XML文件中
    bool _SetSaveFlag(const char *pszPath,bool bSaveFlag,bool bSetSub);//设置存储标记
    void _SetSubSaveFlag(STreeNode *pNode,bool bSaveFlag);//递归设置指定节点的子节点存储标记
    
    //------变量------
    static CDataTree *m_pInstance;//类实例
    char m_szXMLFilePath[MAX_XML_FILE_PATH];//存放当前已读入的XML文件的文件名及路径
    CStrHashMap *m_pHash;//数据索引表指针
    void *m_pDataMen;//数据储存内存管理指针
    STreeNode m_oHeadNode;//首节点
};

#endif
