/*
 * 管理脚本外部数据（存放于XML文件中）类 cpp文件 By Lzy
 * 2011-12-21   V2.0.1.1    修正新增或修改字符串数据时当传入的数据值指针为NULL会崩溃的问题，现把传入NULL数据指针视为清空字符串的操作
 * 2011-12-08   V2.0.1.0    1、数据节点增加指向父数据节点的指针，访问器增加上级访问函数
 *                          2、增加回写数据控制（可控制哪些数据在保存时不写入到xml文件中）
 * 2011-12-04   V2.0.0.0    大幅度调整存储结构及各种实现，增加了删除操作和保存数据到文件的操作
 * 2011-09-05   V1.0.0.0    创建版本信息
 */

#include "DataTree.h"
#include <string.h>

#ifdef WIN32
#define snprintf sprintf_s
#endif

using namespace simple_mem_pool;

CDataTree *CDataTree::m_pInstance = NULL;

/*
 * 【公开】【静态】获取类的单例指针
 * @return CDataTree *,返回类的单实例指针
 */
CDataTree *CDataTree::GetInstance()
{
    if (CDataTree::m_pInstance == NULL)
        CDataTree::m_pInstance = new CDataTree();
    return CDataTree::m_pInstance;
}

/*
 * 【公开】【静态】析构单例
 * @return void
 */
void CDataTree::ReleaseInstance()
{
    if (CDataTree::m_pInstance != NULL)
    {
        delete CDataTree::m_pInstance;
        CDataTree::m_pInstance = NULL;
    }
}

/*
 * 【公开】访问同级的访问器指向的最后一个有效数据的下一个数据，当已经到达尾部时，数据类型（可用GetType获得）值会被设置为Type_None
 * @return void
 */
void CDataTree::SViewNode::MoveNext()
{
    STreeNode *pNext;
    if (this->m_pLink->m_pNext != NULL)
    {
        pNext = this->m_pLink->m_pNext;
        this->m_pLink = pNext;
    }
    else
        pNext = NULL;
    this->_SetData(pNext);
}

/*
 * 【公开】访问同级的访问器指向的最后一个有效数据的上一个数据，当已经到达头部时，数据类型（可用GetType获得）值会被设置为Type_None
 * @return void
 */
void CDataTree::SViewNode::MovePre()
{
    STreeNode *pPre;
    if (this->m_pLink->m_pPre != NULL)
    {
        pPre = this->m_pLink->m_pPre;
        this->m_pLink = pPre;
    }
    else
        pPre = NULL;
    this->_SetData(pPre);
}

/*
 * 【公开】访问当前数据的下级首个数据，只有当前数据类型值为Type_Table时使用此函数才有效，如果下级没有数据或当前数据类型不是Type_Table，则数据类型值会被设置为Type_None
 * @return void
 */
void CDataTree::SViewNode::MoveSub()
{
    STreeNode *pSub;
    if (this->m_pLink->m_eDataType == CDataTree::Type_Table && this->m_pLink->m_uData.m_pTable != NULL)
    {
        pSub = (STreeNode *)(this->m_pLink->m_uData.m_pTable);
        this->m_pLink = pSub;
    }
    else
        pSub = NULL;
    this->_SetData(pSub);
}

/*
 * 【公开】访问当前数据的上级数据，如果上级没有数据，则数据类型值会被设置为Type_None
 * @return void
 */
void CDataTree::SViewNode::MoveParent()
{
    STreeNode *pParent;
    if (this->m_pLink->m_pParent != NULL && this->m_pLink->m_pParent->m_pParent != NULL)//后一个判断是防止访问到了隐藏的头指针
    {
        pParent = this->m_pLink->m_pParent;
        this->m_pLink = pParent;
    }
    else
        pParent = NULL;
    this->_SetData(pParent);
}

/*
 * 【私有】构造函数，初始化基本数据
 */
CDataTree::CDataTree()
{
    this->m_oHeadNode.m_eDataType = CDataTree::Type_Table;
    this->m_oHeadNode.m_uData.m_pTable = NULL;
    this->m_oHeadNode.m_pNext = NULL;
    this->m_oHeadNode.m_pPre = NULL;
    this->m_oHeadNode.m_bCanSave = true;
    this->m_oHeadNode.m_pParent = NULL;
    this->m_oHeadNode.m_szName[0] = '\0';
    this->m_szXMLFilePath[0] = '\0';
    this->m_pDataMen = CreateMemPool(sizeof(STreeNode));
    this->m_pHash = new CStrHashMap(HASH_MAP_SIZE);
}

/*
 * 【私有】析构函数，释放对象
 */
CDataTree::~CDataTree()
{
    ReleaseMemPool(&this->m_pDataMen);
    delete this->m_pHash;
}

/*
 * 【私有】分析XML中所有同级节点，并据此创建数据
 * @param const TiXmlElement *pXMLElement,同级XML节点中的首节点，函数会检查此参数值的有效性
 * @param unsigned long lPathLoc,当前使用到的属性路径长度（也即下一字符填入的索引位）
 * @param char *pszPath,路径字符串指针，如果传入为NULL，则由函数创建
 * @param STreeNode *pParent,上级节点指针
 * @return STreeNode *,返回首个成功创建的数据节点指针
 */
CDataTree::STreeNode *CDataTree::_AnalyzeXMLElement(const TiXmlElement *pXMLElement,unsigned long lPathLoc,char *pszPath,STreeNode *pParent)
{
    const char *pszXMLValue;
    char *pszOrgPath = pszPath;
    STreeNode *pFirstNode = NULL;//本次函数调用中首个创建的节点
    STreeNode *pLastNode = NULL;//上一次处理的节点
    STreeNode *pPresentNode = NULL;//指向当前创建数据节点的地址
    //1、未有缓冲区时创建缓冲区
    if (pszOrgPath == NULL)
    {
        pszOrgPath = new char[MAX_DATATREE_PROPERTY_PATH_LENGTH];
        pszOrgPath[0] = '\0';
    }
    if (pszOrgPath == NULL)
        return pFirstNode;
    //2、设置属性路径写入位置
    char *pszPropertyPath = &(pszOrgPath[lPathLoc]);
    //3、XML节点分析
    while (pXMLElement != NULL)
    {
        //3.1、分配内存
        if (pPresentNode == NULL)
        {
            pPresentNode = (STreeNode *)AllocMemory(this->m_pDataMen);
            if (pPresentNode == NULL)
            {
                if (pszPath == NULL)
                    delete[] pszOrgPath;
                return pFirstNode;
            }
        }
        //3.2、数据名设置
        pszXMLValue = pXMLElement->Attribute("name");
        if (pszXMLValue != NULL)
            strncpy(pPresentNode->m_szName,pszXMLValue,MAX_DATATREE_NAME_LENGTH);
        else
            pPresentNode->m_szName[0] = '\0';
        //3.3、数据类型分析
        pszXMLValue = pXMLElement->Attribute("type");
        if (pszXMLValue != NULL)
        {
            if (strcmp(pszXMLValue,"long") == 0)
            {
                pPresentNode->m_eDataType = CDataTree::Type_Long;
                pszXMLValue = pXMLElement->Attribute("value");
                if (pszXMLValue != NULL)
                    pPresentNode->m_uData.m_lData = atol(pszXMLValue);
                else
                    pPresentNode->m_uData.m_lData = 0;
            }
            else if (strcmp(pszXMLValue,"float") == 0)
            {
                pPresentNode->m_eDataType = CDataTree::Type_Float;
                pszXMLValue = pXMLElement->Attribute("value");
                if (pszXMLValue != NULL)
                    pPresentNode->m_uData.m_fData = atof(pszXMLValue);
                else
                    pPresentNode->m_uData.m_fData = 0.0;
            }
            else if (strcmp(pszXMLValue,"string") == 0)
            {
                pPresentNode->m_eDataType = CDataTree::Type_String;
                pszXMLValue = pXMLElement->Attribute("value");
                if (pszXMLValue != NULL)
                    strncpy(pPresentNode->m_uData.m_szData,pszXMLValue,MAX_DATATREE_STRING_VALUE_LENGTH);
                else
                    pPresentNode->m_uData.m_szData[0] = '\0';
            }
            else if (strcmp(pszXMLValue,"table") == 0)
            {
                pPresentNode->m_eDataType = CDataTree::Type_Table;
                unsigned long lBackupPathLoc = lPathLoc;
                char * pszBackupPath = pszPropertyPath;
                //3.3.t.1、填充当前属性名
                strncpy(pszPropertyPath,pPresentNode->m_szName,MAX_DATATREE_PROPERTY_PATH_LENGTH - lPathLoc);
                unsigned long nNameLen = (unsigned long)strlen(pPresentNode->m_szName);//存在转换隐患，但暂时无法处理 By Lzy
                lPathLoc += nNameLen;
                pszPropertyPath += nNameLen;
                //3.3.t.2、填充路径后缀
                *pszPropertyPath = DIV_PATH_CHAR;
                ++pszPropertyPath;
                ++lPathLoc;
                pszPropertyPath[lPathLoc] = '\0';
                //3.3.t.3、分析下层数据
                pPresentNode->m_uData.m_pTable = this->_AnalyzeXMLElement(pXMLElement->FirstChildElement(),lPathLoc,pszOrgPath,pPresentNode);
                //3.3.t.4、还原
                lPathLoc = lBackupPathLoc;
                pszPropertyPath = pszBackupPath;
            }
            else
            {
                printf ("Unknow Type \"%s\" In \"%s\"\n",pszXMLValue,pszOrgPath);
                pXMLElement = pXMLElement->NextSiblingElement();
                continue;//不能识别的类型，跳过
            }
            if (pLastNode != NULL)
                pLastNode->m_pNext = pPresentNode;
            pPresentNode->m_pPre = pLastNode;
            pPresentNode->m_pNext = NULL;
            pPresentNode->m_pParent = pParent;
            pPresentNode->m_bCanSave = true;
            pLastNode = pPresentNode;
            if (pFirstNode == NULL)
                pFirstNode = pPresentNode;
            pPresentNode = NULL;
            strncpy(pszPropertyPath,pLastNode->m_szName,MAX_DATATREE_PROPERTY_PATH_LENGTH - lPathLoc);
            this->m_pHash->Insert(pszOrgPath,pLastNode);
        }
        else
            printf ("UnSet Type In \"%s\"\n",pszOrgPath);
        pXMLElement = pXMLElement->NextSiblingElement();
    }
    if (pszPath == NULL)
        delete[] pszOrgPath;
    return pFirstNode;
}

/*
 * 【公开】读取配置文件
 * @param const char *pszXMLFilePath,配置文件的路径
 * @return bool,返回是否读取成功
 */
bool CDataTree::LoadFromXML(const char *pszXMLFilePath)
{
    if (pszXMLFilePath == NULL)
        return false;
    if (strcmp(pszXMLFilePath,this->m_szXMLFilePath) != 0)
    {
        //加载XML文件
        TiXmlDocument pXMLReader(pszXMLFilePath);
        if (pXMLReader.LoadFile() == false)
            return false;
        TiXmlElement *pXMLRoot = pXMLReader.RootElement();
        if (pXMLRoot == NULL)
            return false;
        strncpy(this->m_szXMLFilePath,pszXMLFilePath,MAX_XML_FILE_PATH);
        //清理上次加载配置后创建的资源
        this->m_pHash->Clear();
        RecycleMemory(this->m_pDataMen);//回收内存
        //分析XML并创建数据
        this->m_oHeadNode.m_uData.m_pTable = this->_AnalyzeXMLElement(pXMLRoot->FirstChildElement(),0,NULL,&(this->m_oHeadNode));
    }
    return true;
}

/*
 * 【公开】把当前标记为可存储的数据储存到XML文件中（本函数可能有编码和换行符问题）
 * @param const char *pszXMLFilePath = NULL,存储路径，支持绝对路径和相对路径，如果使用默认值NULL，则写回原本载入时指定的XML文件，如果原本没载入过XML文件但此参数又没设置，则函数必定返回false
 * @return bool,返回写入结果，写入成功时返回true
 */
bool CDataTree::SaveToXML(const char *pszXMLFilePath)
{
    if (pszXMLFilePath == NULL)
    {
        if (this->m_szXMLFilePath[0] != '\0')
            pszXMLFilePath = this->m_szXMLFilePath;
        else
            return false;
    }
    FILE *fHandle = fopen(pszXMLFilePath,"w");
    if (fHandle != NULL)
    {
        char szLineBuffer[MAX_WRITE_XML_LINE_LEN];
        fwrite("<?xml version=\"1.0\" encoding=\"GBK\" standalone=\"yes\" ?>\n<conf>\n",62,1,fHandle);
        STreeNode *pViewNode = (STreeNode *)this->m_oHeadNode.m_uData.m_pTable;
        szLineBuffer[0] = '\t';
        this->_WriteXML(fHandle,pViewNode,szLineBuffer,1);
        fwrite("</conf>\n",8,1,fHandle);
        fclose(fHandle);
        return true;
    }
    return false;
}

/*
 * 【公开】获取数据值访问器
 * @param const char *pszSearchPath,数据路径（查找键）
 * @param CDataTree::SViewNode &rViewNode,访问器的引用，在该路径下获取成功时将把信息写入此引用中
 * @return bool,返回获取结果，成功获取返回true
 */
bool CDataTree::GetDataVisitor(const char *pszSearchPath,CDataTree::SViewNode &rViewNode)
{
    STreeNode *pFindNode = (STreeNode *)this->m_pHash->Find(pszSearchPath);
    if (pFindNode != NULL)
    {
        rViewNode.m_pLink = pFindNode;
        rViewNode._SetData(pFindNode);
        return true;
    }
    else
        return false;
}

/*
 * 【公开】获取long型数据值
 * @param const char *pszSearchPath,数据路径（查找键）
 * @param long lDefault = 0,当查找失败时（失败的情况包括该路径下的数据类型不是long型）返回的值
 * @param bool *bGetResult = NULL,如果需要知道数据是否获取成功，在这里传入一个bool变量地址，函数会把获取结果填入在传入地址所在变量中，获取成功时填入true，否则填入false
 * @return long,返回数据值，如果获取数据失败，返回的将是lDefault的值，只有传入了bGetResult参数才可以确定当前获取的数据是否指定路径里的数据
 */
long CDataTree::GetDataLong(const char *pszSearchPath,long lDefault,bool *bGetResult)
{
    STreeNode *pFindNode = (STreeNode *)this->m_pHash->Find(pszSearchPath);
    if (pFindNode != NULL && pFindNode->m_eDataType == CDataTree::Type_Long)
    {
        if (bGetResult != NULL)
            *bGetResult = true;
        return pFindNode->m_uData.m_lData;
    }
    else
    {
        if (bGetResult != NULL)
            *bGetResult = false;
        return lDefault;
    }
}

/*
 * 【公开】获取浮点型数据值
 * @param const char *pszSearchPath,数据路径（查找键）
 * @param double fDefault = 0.0,当查找失败时（失败的情况包括该路径下的数据类型不是浮点型）返回的值
 * @param bool *bGetResult = NULL,如果需要知道数据是否获取成功，在这里传入一个bool变量地址，函数会把获取结果填入在传入地址所在变量中，获取成功时填入true，否则填入false
 * @return double,返回数据值，如果获取数据失败，返回的将是fDefault的值，只有传入了bGetResult参数才可以确定当前获取的数据是否指定路径里的数据
 */
double CDataTree::GetDataFloat(const char *pszSearchPath,double fDefault,bool *bGetResult)
{
    STreeNode *pFindNode = (STreeNode *)this->m_pHash->Find(pszSearchPath);
    if (pFindNode != NULL && pFindNode->m_eDataType == CDataTree::Type_Float)
    {
        if (bGetResult != NULL)
            *bGetResult = true;
        return pFindNode->m_uData.m_fData;
    }
    else
    {
        if (bGetResult != NULL)
            *bGetResult = false;
        return fDefault;
    }
}

/*
 * 【公开】获取字符串型数据值
 * @parma const char *pszSearchPath,数据路径（查找键）
 * @return const char *,返回指向字符串的const指针，查找失败返回NULL
 */
const char * CDataTree::GetDataStr(const char *pszSearchPath)
{
    STreeNode *pFindNode = (STreeNode *)this->m_pHash->Find(pszSearchPath);
    if (pFindNode != NULL && pFindNode->m_eDataType == CDataTree::Type_String)
        return pFindNode->m_uData.m_szData;
    else
        return NULL;
}

/*
 * 【公开】通过访问器修改特定数据的值
 * @info 本函数只能修改类型为Type_Long,Type_Float,Type_String的数据值
 * @param SViewNode &rViewNode,访问器引用，根据访问器当前绑定的数据位置进行数据值修改
 * @return bool,返回修改是否成功，成功返回true
 */
bool CDataTree::ModifyDataVisitor(SViewNode &rViewNode)
{
    STreeNode *pFindNode = rViewNode.m_pLink;
    if (pFindNode != NULL && pFindNode->m_eDataType == rViewNode.m_eDataType)
    {
        switch (pFindNode->m_eDataType)
        {
        case CDataTree::Type_Long:
            pFindNode->m_uData.m_lData = rViewNode.m_uViewData.m_lData;
            break;
        case CDataTree::Type_Float:
            pFindNode->m_uData.m_fData = rViewNode.m_uViewData.m_fData;
            break;
        case CDataTree::Type_String:
            if ( (&(pFindNode->m_uData.m_szData[0])) > rViewNode.m_uViewData.m_pszData || (&(pFindNode->m_uData.m_szData[0])) + MAX_DATATREE_STRING_VALUE_LENGTH <= rViewNode.m_uViewData.m_pszData)
                strncpy(pFindNode->m_uData.m_szData,rViewNode.m_uViewData.m_pszData,MAX_DATATREE_STRING_VALUE_LENGTH);
            else
                return false;
            break;
        default:
            return false;
        }
        return true;
    }
    else
        return false;
}

/*
 * 【公开】修改long型数据的值
 * @info 当要通过本函数改动数据类型为Type_Table的数据为Type_Long，原数据下的所有子数据会被删除
 * @param const char *pszSearchPath,数据路径（查找键）
 * @param long lNewLong,要设置的新long型值
 * @param bool bCanChangeType = false,指定当要修改的数据类型与当前给出类型不一致时，是否允许继续修改（允许改变原来数据的类型），默认操作是不允许
 * @return bool,返回修改是否成功，成功返回true
 */
bool CDataTree::ModifyDataLong(const char *pszSearchPath,long lNewLong,bool bCanChangeType)
{
    STreeNode *pFindNode = (STreeNode *)this->m_pHash->Find(pszSearchPath);
    if (pFindNode != NULL && (bCanChangeType == true || pFindNode->m_eDataType == CDataTree::Type_Long))
    {
        if (pFindNode->m_eDataType == CDataTree::Type_Table)
            this->_DeleteData(pszSearchPath,true);
        pFindNode->m_eDataType = CDataTree::Type_Long;
        pFindNode->m_uData.m_lData = lNewLong;
        return true;
    }
    return false;
}

/*
 * 【公开】修改浮点型数据的值
 * @info 当要通过本函数改动数据类型为Type_Table的数据为Type_Float，原数据下的所有子数据会被删除
 * @param const char *pszSearchPath,数据路径（查找键）
 * @param double fNewFloat,要设置的新浮点型值
 * @param bool bCanChangeType = false,指定当要修改的数据类型与当前给出类型不一致时，是否允许继续修改（允许改变原来数据的类型），默认操作是不允许
 * @return bool,返回修改是否成功，成功返回true
 */
bool CDataTree::ModifyDataFloat(const char *pszSearchPath,double fNewFloat,bool bCanChangeType)
{
    STreeNode *pFindNode = (STreeNode *)this->m_pHash->Find(pszSearchPath);
    if (pFindNode != NULL && (bCanChangeType == true || pFindNode->m_eDataType == CDataTree::Type_Float))
    {
        if (pFindNode->m_eDataType == CDataTree::Type_Table)
            this->_DeleteData(pszSearchPath,true);
        pFindNode->m_eDataType = CDataTree::Type_Float;
        pFindNode->m_uData.m_fData = fNewFloat;
        return true;
    }
    return false;
}

/*
 * 【公开】修改字符串型数据的值
 * @info 当要通过本函数改动数据类型为Type_Table的数据为Type_String，原数据下的所有子数据会被删除
 * @param const char *pszSearchPath,数据路径（查找键）
 * @param const char *pszNewString,要设置的新字符串值
 * @param bool bCanChangeType = false,指定当要修改的数据类型与当前给出类型不一致时，是否允许继续修改（允许改变原来数据的类型），默认操作是不允许
 * @return bool,返回修改是否成功，成功返回true
 */
bool CDataTree::ModifyDataStr(const char *pszSearchPath,const char *pszNewString,bool bCanChangeType)
{
    STreeNode *pFindNode = (STreeNode *)this->m_pHash->Find(pszSearchPath);
    if (pFindNode != NULL && (bCanChangeType == true || pFindNode->m_eDataType == CDataTree::Type_String))
    {
        if (pFindNode->m_eDataType == CDataTree::Type_Table)
            this->_DeleteData(pszSearchPath,true);
        pFindNode->m_eDataType = CDataTree::Type_String;
        if (pszNewString != NULL)
            strncpy(pFindNode->m_uData.m_szData,pszNewString,MAX_DATATREE_STRING_VALUE_LENGTH);
        else
            pFindNode->m_uData.m_szData[0] = '\0';
        return true;
    }
    return false;
}

/*
 * 【私有】删除指定路径及其下的所有数据，可指定是否保留指定路径处的节点，对于出现路径重复的情况，会出现该对象无法直接访问（删除、修改）的情况，此时只有在删除父级对象时才能将该对象删除
 * @param const char *pszSearchPath,数据路径（查找键）
 * @param bool bReserve,指明指定路径处的数据节点是否保留，当出于节点类型变更需要而做的删除操作应该把此参数设置为true
 * @return void
 */
void CDataTree::_DeleteData(const char *pszSearchPath,bool bReserve)
{
    STreeNode *pFindNode = (STreeNode *)this->m_pHash->Find(pszSearchPath);
    char szSearchPath[MAX_DATATREE_PROPERTY_PATH_LENGTH];
    if (pFindNode != NULL)
    {
        strncpy(szSearchPath,pszSearchPath,MAX_DATATREE_PROPERTY_PATH_LENGTH);
        if (pFindNode->m_eDataType == CDataTree::Type_Table)
            this->_RecycleRL(szSearchPath,strlen(szSearchPath),(STreeNode *)pFindNode->m_uData.m_pTable);
        if (bReserve == false)
        {
            if (pFindNode->m_pPre != NULL)
                pFindNode->m_pPre->m_pNext = pFindNode->m_pNext;
            else if (pFindNode == (STreeNode *)this->m_oHeadNode.m_uData.m_pTable)
                this->m_oHeadNode.m_uData.m_pTable = pFindNode->m_pNext;
            if (pFindNode->m_pNext != NULL)
                pFindNode->m_pNext->m_pPre = pFindNode->m_pPre;
            this->m_pHash->Delete(pszSearchPath);
            RecycleMemory(this->m_pDataMen,pFindNode);
        }
    }
}

/*
 * 【私有】递归回收与当前节点同级的节点（包括当前节点自身）及属于这些节点之下的子节点
 * @param char *pszParentPath,上级节点的路径，函数执行时有可能修改其内容
 * @param int nLen,pszParentPath中内容的长度
 * @param CDataTree::STreeNode *pNode,指定遍历回收的起点节点，必须保证传入地址指向一个有效节点
 * @return void
 */
void CDataTree::_RecycleRL(char *pszParentPath,int nLen,CDataTree::STreeNode *pNode)
{
    STreeNode *pTmpNode;
    while (pNode->m_pPre != NULL)
        pNode = pNode->m_pPre;
    char *pOrgLoc = pszParentPath + nLen;
    *pOrgLoc = '/';
    ++pOrgLoc;
    *pOrgLoc = '\0';
    while (pNode != NULL)
    {
        strncpy(pOrgLoc,pNode->m_szName,MAX_DATATREE_PROPERTY_PATH_LENGTH - nLen - 1);
        if (pNode->m_eDataType == CDataTree::Type_Table)
            this->_RecycleRL(pszParentPath,nLen + strlen(pNode->m_szName) + 1,(STreeNode *)pNode->m_uData.m_pTable);
        pTmpNode = pNode;
        pNode = pNode->m_pNext;
        this->m_pHash->Delete(pszParentPath);
        RecycleMemory(this->m_pDataMen,pTmpNode);
    }
    --pOrgLoc;
    *pOrgLoc = '\0';
}

/*
 * 【公开】添加新的long型数据值
 * @param const char *pszNewPath,新数据的路径
 * @param long lValue,添加的数据值
 * @param bool bCanSave,是否允许该节点在写入文件操作中被存储，如果设置为可被存储（true），与该节点关联的上层节点也会被同时置为可被存储
 * @param bool bAutoCreatePath = false,指定当新路径的上层不存在时是否自动创建上层数据节点，如果设置为false，则pszNewPath给出的路径中的上层部分（如路径“a/b/c”中，a/b数据节点则为c的上层数据节点）必须存在
 * @return bool,返回添加结果，成功时返回true
 */
bool CDataTree::AddDataLong(const char *pszNewPath,long lValue,bool bCanSave,bool bAutoCreatePath)
{
    STreeNode *pPresentNode = this->_PreSetPath(pszNewPath,bAutoCreatePath);
    if (pPresentNode != NULL)
    {
        pPresentNode->m_eDataType = CDataTree::Type_Long;
        pPresentNode->m_uData.m_lData = lValue;
        this->_SetSaveFlag(pszNewPath,bCanSave,false);
        return true;
    }
    return false;
}

/*
 * 【公开】添加新的浮点型数据值
 * @param const char *pszNewPath,新数据的路径
 * @param double fValue,添加的数据值
 * @param bool bCanSave,是否允许该节点在写入文件操作中被存储，如果设置为可被存储（true），与该节点关联的上层节点也会被同时置为可被存储
 * @param bool bAutoCreatePath = false,指定当新路径的上层不存在时是否自动创建上层数据节点，如果设置为false，则pszNewPath给出的路径中的上层部分（如路径“a/b/c”中，a/b数据节点则为c的上层数据节点）必须存在
 * @return bool,返回添加结果，成功时返回true
 */
bool CDataTree::AddDataFloat(const char *pszNewPath,double fValue,bool bCanSave,bool bAutoCreatePath)
{
    STreeNode *pPresentNode = this->_PreSetPath(pszNewPath,bAutoCreatePath);
    if (pPresentNode != NULL)
    {
        pPresentNode->m_eDataType = CDataTree::Type_Float;
        pPresentNode->m_uData.m_fData = fValue;
        this->_SetSaveFlag(pszNewPath,bCanSave,false);
        return true;
    }
    return false;
}

/*
 * 【公开】添加新的字符串型数据值
 * @param const char *pszNewPath,新数据的路径
 * @param const char *pszValue,添加的数据值
 * @param bool bCanSave,是否允许该节点在写入文件操作中被存储，如果设置为可被存储（true），与该节点关联的上层节点也会被同时置为可被存储
 * @param bool bAutoCreatePath = false,指定当新路径的上层不存在时是否自动创建上层数据节点，如果设置为false，则pszNewPath给出的路径中的上层部分（如路径“a/b/c”中，a/b数据节点则为c的上层数据节点）必须存在
 * @return bool,返回添加结果，成功时返回true
 */
bool CDataTree::AddDataStr(const char *pszNewPath,const char *pszValue,bool bCanSave,bool bAutoCreatePath)
{
    STreeNode *pPresentNode = this->_PreSetPath(pszNewPath,bAutoCreatePath);
    if (pPresentNode != NULL)
    {
        pPresentNode->m_eDataType = CDataTree::Type_String;
        if (pszValue != NULL)
            strncpy(pPresentNode->m_uData.m_szData,pszValue,MAX_DATATREE_STRING_VALUE_LENGTH);
        else
            pPresentNode->m_uData.m_szData[0] = '\0';
        this->_SetSaveFlag(pszNewPath,bCanSave,false);
        return true;
    }
    return false;
}

/*
 * 【公开】添加新的子数据节点
 * @param const char *pszNewPath,新数据的路径
 * @param bool bCanSave,是否允许该节点在写入文件操作中被存储，如果设置为可被存储（true），与该节点关联的上层节点也会被同时置为可被存储
 * @param bool bAutoCreatePath = false,指定当新路径的上层不存在时是否自动创建上层数据节点，如果设置为false，则pszNewPath给出的路径中的上层部分（如路径“a/b/c”中，a/b数据节点则为c的上层数据节点）必须存在
 * @return bool,返回添加结果，成功时返回true
 */
bool CDataTree::AddDataTable(const char *pszNewPath,bool bCanSave,bool bAutoCreatePath)
{
    STreeNode *pPresentNode = this->_PreSetPath(pszNewPath,bAutoCreatePath);
    if (pPresentNode != NULL)
    {
        pPresentNode->m_eDataType = CDataTree::Type_Table;
        pPresentNode->m_uData.m_pTable = NULL;
        this->_SetSaveFlag(pszNewPath,bCanSave,false);
        return true;
    }
    return false;
}

/*
 * 【私有】路径预处理，确定路径上各层是否存在，并初始化子数据节点
 * @param const char *pszPath,要检查的路径字符串
 * @param bool bAutoCreatePath,指定是否自动创建不路径中缺少的部分，但如果该路径上层部分是存在的，但类型不是Type_Table，则无法处理，只能返回false
 * @return STreeNode *,如果预处理成功，将返回初步初始化后的子数据节点（仅类型和值未初始化），否则返回NULL
 */
CDataTree::STreeNode *CDataTree::_PreSetPath(const char *pszPath,bool bAutoCreatePath)
{
    char szPath[MAX_DATATREE_PROPERTY_PATH_LENGTH];
    strncpy(szPath,pszPath,MAX_DATATREE_PROPERTY_PATH_LENGTH);
    char *pszView = szPath - 1;//为配合循环才要在这里减1，实际访问是从szPath开始的
    const char *pszLastDiv = NULL;
    STreeNode *pLastFindNode = &(this->m_oHeadNode);
    STreeNode *pPresentFindNode;
    STreeNode *pPresentNode;
    do
    {
        ++pszView;
        if (*pszView == DIV_PATH_CHAR || *pszView == '\0')
        {
            char chBackup = *pszView;
            *pszView = '\0';
            pPresentFindNode = (STreeNode *)this->m_pHash->Find(szPath);
            if (pPresentFindNode != NULL && chBackup != '\0')
                pLastFindNode = pPresentFindNode;
            else
            {
                if (chBackup == '\0' && pPresentFindNode != NULL)
                    return NULL;
                if (bAutoCreatePath == true || chBackup == '\0')
                {
                    if (pLastFindNode->m_eDataType != CDataTree::Type_Table)
                        return NULL;
                    pPresentNode = (STreeNode *)AllocMemory(this->m_pDataMen);
                    if (pPresentNode == NULL)
                        return NULL;
                    pPresentNode->m_eDataType = CDataTree::Type_Table;
                    pPresentNode->m_pNext = (STreeNode *)pLastFindNode->m_uData.m_pTable;
                    if (pLastFindNode->m_uData.m_pTable != NULL)
                        ((STreeNode *)pLastFindNode->m_uData.m_pTable)->m_pPre = pPresentNode;
                    pLastFindNode->m_uData.m_pTable = pPresentNode;
                    pPresentNode->m_pParent = pLastFindNode;
                    pPresentNode->m_pPre = NULL;
                    pPresentNode->m_bCanSave = false;
                    pPresentNode->m_uData.m_pTable = NULL;
                    if (pszLastDiv != NULL)
                        pszLastDiv++;
                    else
                        pszLastDiv = szPath;
                    strncpy(pPresentNode->m_szName,pszLastDiv,MAX_DATATREE_NAME_LENGTH);
                    this->m_pHash->Insert(szPath,pPresentNode);
                    pLastFindNode = pPresentNode;
                }
                else
                    return NULL;
            }
            *pszView = chBackup;
            pszLastDiv = pszView;
        }
    }while (*pszView != '\0');
    return pPresentNode;
}

/*
 * 【私有】递归遍历数据节点并把标记为可存储的数据写入XML文件中
 * @param FILE *fHandle,有效的文件指针
 * @param STreeNode *pNode,遍历节点的指针
 * @param char *pszBuffer,缓冲区指针
 * @parma int nUsedSize,缓冲区已用空间
 * @return void
 */
void CDataTree::_WriteXML(FILE *fHandle,STreeNode *pNode,char *pszBuffer,int nUsedSize)
{
    char *pszBgUseBuf = pszBuffer + nUsedSize;
    int nLeftSize = MAX_WRITE_XML_LINE_LEN - nUsedSize;
    if (nLeftSize <= 0)
        return;
    while (pNode != NULL)
    {
        if (pNode->m_bCanSave == true)
        {
            switch (pNode->m_eDataType)
            {
            case CDataTree::Type_Long:
                if (pNode->m_szName[0] != '\0')
                    snprintf(pszBgUseBuf,nLeftSize,"<property name=\"%s\" type=\"long\" value=\"%ld\"/>\n",pNode->m_szName,pNode->m_uData.m_lData);
                else
                    snprintf(pszBgUseBuf,nLeftSize,"<property type=\"long\" value=\"%ld\"/>\n",pNode->m_uData.m_lData);
                break;
            case CDataTree::Type_Float:
                if (pNode->m_szName[0] != '\0')
                    snprintf(pszBgUseBuf,nLeftSize,"<property name=\"%s\" type=\"float\" value=\"%f\"/>\n",pNode->m_szName,pNode->m_uData.m_fData);
                else
                    snprintf(pszBgUseBuf,nLeftSize,"<property type=\"float\" value=\"%f\"/>\n",pNode->m_uData.m_fData);
                break;
            case CDataTree::Type_String:
                if (pNode->m_szName[0] != '\0')
                    snprintf(pszBgUseBuf,nLeftSize,"<property name=\"%s\" type=\"string\" value=\"%s\"/>\n",pNode->m_szName,pNode->m_uData.m_szData);
                else
                    snprintf(pszBgUseBuf,nLeftSize,"<property type=\"string\" value=\"%s\"/>\n",pNode->m_uData.m_szData);
                break;
            case CDataTree::Type_Table:
                if (pNode->m_szName[0] != '\0')
                    snprintf(pszBgUseBuf,nLeftSize,"<property name=\"%s\" type=\"table\">\n",pNode->m_szName);
                else
                    snprintf(pszBgUseBuf,nLeftSize,"<property type=\"table\">\n");
                fwrite(pszBuffer,strlen(pszBuffer),1,fHandle);
                pszBgUseBuf[0] = '\t';
                this->_WriteXML(fHandle,(STreeNode *)pNode->m_uData.m_pTable,pszBuffer,nUsedSize + 1);
                snprintf(pszBgUseBuf,nLeftSize,"</property>\n");
                fwrite(pszBuffer,strlen(pszBuffer),1,fHandle);
                pNode = pNode->m_pNext;
                continue;
                break;
            }
            fwrite(pszBuffer,strlen(pszBuffer),1,fHandle);
        }
        pNode = pNode->m_pNext;
    }
}

/*
 * 【私有】设置存储标记
 * @param const char *pszPath,要修改的存储节点路径，如果修改为可存储到文件（bSavaFlag=true），
 *                            则该路径上所有节点均会被置为可存储，但如果是修改为不可存储，则只是指定节点会被修改。
 *                            此参数允许设置为NULL，如果此时bSetSub设置为true，则表示修改涉及所有节点，否则传入NULL没意义，函数将直接返回true
 * @param bool bSaveFlag,新的存储标记
 * @param bool bSetSub,指明是否把指定路径下所有子节点也应该新的存储标记
 * @return bool,返回设置结果，设置成功返回true
 */
bool CDataTree::_SetSaveFlag(const char *pszPath,bool bSaveFlag,bool bSetSub)
{
    STreeNode *pFindNode = (STreeNode *)this->m_pHash->Find(pszPath);
    if (pFindNode != NULL)
    {
        if (bSaveFlag == false)
        {
            pFindNode->m_bCanSave = false;
            return true;
        }
        else
        {
            STreeNode *pViewNode = pFindNode;
            while (pViewNode != NULL)
            {
                pViewNode->m_bCanSave = true;
                pViewNode = pViewNode->m_pParent;
            }
        }
    }
    else if (pszPath == NULL && bSetSub == true)
        pFindNode = &(this->m_oHeadNode);
    else
        return false;
    if (bSetSub == true && pFindNode->m_eDataType == CDataTree::Type_Table)
        this->_SetSubSaveFlag(pFindNode,bSaveFlag);
    return true;
}

/*
 * 【私有】递归设置指定节点的子节点存储标记
 * @param STreeNode *pNode,要设置的节点的父节点，必须保证该节点为有效的Type_Table类型节点
 * @param bool bSaveFlag,要设置的存储标记
 * @return void
 */
void CDataTree::_SetSubSaveFlag(STreeNode *pNode,bool bSaveFlag)
{
    STreeNode *pSubNode = (STreeNode *)pNode->m_uData.m_pTable;
    while (pSubNode->m_pPre != NULL)
        pSubNode = pSubNode->m_pPre;
    while (pSubNode != NULL)
    {
        pSubNode->m_bCanSave = bSaveFlag;
        if (pSubNode->m_eDataType == CDataTree::Type_Table)
            this->_SetSubSaveFlag(pSubNode,bSaveFlag);
        pSubNode = pSubNode->m_pNext;
    }
}
