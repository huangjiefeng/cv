/*
 * 字符串作为索引键，指针作为索引值的索引表    cpp文件 By Lzy
 * 2011-11-26   V1.0.1.1    把获取HashNode的代码封入inline函数_GetHashNodeByKey中，以减少代码重复
 * 2011-11-25   V1.0.1.0    删除了对SimpleMemPool的依赖（原本在这里使用SimpleMemPool没什么意义，还浪费内存） By Lzy
 * 2011-08-29   V1.0.0.0    By Lzy
 */

#include "StrHashMap.h"

/*
 * 【公共】构造函数
 * @param unsigned long lcapability,Hash表容量，由于是使用静态Hash表，因此容量设置大小对性能有影响
 */
CStrHashMap::CStrHashMap(unsigned long lcapability)
{
    if (lcapability < 10)
        lcapability = 10;
    this->m_lHashSize = lcapability;
    this->m_pHash = (map<string,void *> **)malloc(sizeof(void *) * lcapability);
    if (this->m_pHash != NULL)
        memset(this->m_pHash,0,sizeof(void *) * this->m_lHashSize);
}

/*
 * 【公共】析构函数，释放内部资源
 */
CStrHashMap::~CStrHashMap()
{
    if (this->m_pHash != NULL)
    {
        for (unsigned int nLoopVar = 0;nLoopVar < this->m_lHashSize;++nLoopVar)
        {
            if (this->m_pHash[nLoopVar] != NULL)
                delete this->m_pHash[nLoopVar];
        }
        free(this->m_pHash);
    }
}

/*
 * 【公共】插入新的键值对
 * @param const char *pszKey,插入的新字符串键
 * @param void *pValue,插入的新值
 * @return bool,返回插入是否成功
 */
bool CStrHashMap::Insert(const char *pszKey,void *pValue)
{
    if (this->m_pHash != NULL && pszKey != NULL)
    {
        unsigned long lHashNum = this->_HashString(pszKey);
        lHashNum = lHashNum % this->m_lHashSize;
        map<string,void *> *pHashNode = this->m_pHash[lHashNum];
        if (pHashNode == NULL)
        {
            pHashNode = new map<string,void *>;
            this->m_pHash[lHashNum] = pHashNode;
        }
        pHashNode->insert(pair<string,void *>(pszKey,pValue));
        return true;
    }
    return false;
}

/*
 * 【公共】修改指定键下的值
 * @param const char *pszKey,要修改值对应的字符串键
 * @param void *pNewValue,设置的新值
 * @return bool,返回是否修改成功
 */
bool CStrHashMap::Modify(const char *pszKey,void *pNewValue)
{
    if (this->m_pHash != NULL && pszKey != NULL)
    {
        map<string,void *> *pHashNode = this->_GetHashNodeByKey(pszKey);
        if (pHashNode != NULL)
        {
            map<string,void *>::iterator stMapI = pHashNode->find(pszKey);
            if (stMapI != pHashNode->end())
            {
                stMapI->second = pNewValue;
                return true;
            }
        }
    }
    return false;
}

/*
 * 【公共】删除指定的键
 * @param const char *pszKey,要删除的字符串键
 * @return bool,返回删除是否成功
 */
bool CStrHashMap::Delete(const char *pszKey)
{
    if (this->m_pHash != NULL && pszKey != NULL)
    {
        map<string,void *> *pHashNode = this->_GetHashNodeByKey(pszKey);
        if (pHashNode != NULL)
        {
            map<string,void *>::iterator stMapI = pHashNode->find(pszKey);
            if (stMapI != pHashNode->end())
            {
                pHashNode->erase(stMapI);
                return true;
            }
        }
    }
    return false;
}

/*
 * 【公共】清空所有键值信息
 * @return void
 */
void CStrHashMap::Clear()
{
    if (this->m_pHash != NULL)
    {
        for (unsigned int nLoopVar =0;nLoopVar < this->m_lHashSize;++nLoopVar)
        {
            if (this->m_pHash[nLoopVar] != NULL)
                this->m_pHash[nLoopVar]->clear();
        }
    }
}

/*
 * 【公共】查找指定键下对应的值
 * @param const char *pszKey,要查找的字符串键
 * @return void *,返回查找到的值，查找失败返回NULL
 */
void *CStrHashMap::Find(const char *pszKey)
{
    if (this->m_pHash != NULL && pszKey != NULL)
    {
        map<string,void *> *pHashNode = this->_GetHashNodeByKey(pszKey);
        if (pHashNode != NULL)
        {
            map<string,void *>::iterator stMapI = pHashNode->find(pszKey);
            if (stMapI != pHashNode->end())
                return stMapI->second;
        }
    }
    return false;
}

/*
 * 【私有】把字符串转成数字处理
 * @param const char *pszKey,要转换的字符串
 * @return unsigned long,返回转换后的值 注意：该值并非惟一的
 */
unsigned long CStrHashMap::_HashString(const char *pszStr)
{
    const char *schar = pszStr;
    unsigned long Total = 0;
    while ((*schar) != '\0')
    {
        Total += (*schar);
        schar ++;
    }
    return Total;
}

/*
 * 【私有】【内联】根据字符串键值获取对应的Hash节点
 * @param const char *pszKey,要匹配的字符串键值，传入值必须保证有效
 * @return map<string,void *> *,返回查找到的Hash节点
 */
inline map<string,void *> * CStrHashMap::_GetHashNodeByKey(const char *pszKey)
{
    unsigned long lHashNum = this->_HashString(pszKey);
    lHashNum = lHashNum % this->m_lHashSize;
    return this->m_pHash[lHashNum];
}