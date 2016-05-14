/*
 * 字符串作为索引键，指针作为索引值的索引表    头文件 By Lzy
 * 2011-11-26   V1.0.1.1    把获取HashNode的代码封入inline函数_GetHashNodeByKey中，以减少代码重复
 * 2011-11-25   V1.0.1.0    删除了对SimpleMemPool的依赖（原本在这里使用SimpleMemPool没什么意义，还浪费内存） By Lzy
 * 2011-08-29   V1.0.0.0    By Lzy
 */

#ifndef STR_HASH_MAP
#define STR_HASH_MAP

#include <map>
#include <string>
#include <stdlib.h>
#include <string.h>

using namespace std;

class CStrHashMap
{
public:
    CStrHashMap(unsigned long lcapability);
    ~CStrHashMap();
    bool Insert(const char *pszKey,void *pValue);//插入新的键值对
    bool Modify(const char *pszKey,void *pNewValue);//修改指定键下的值
    bool Delete(const char *pszKey);//删除指定的键
    void Clear();//清空键值信息
    void *Find(const char *pszKey);//查找指定键下对应的值
private:
    //------变量------
    map<string,void *> **m_pHash;//存放Hash表
    unsigned long m_lHashSize;//Hash表大小
    //------函数------
    unsigned long _HashString(const char *pszStr);//把字符串转成数字处理
    inline map<string,void *> *_GetHashNodeByKey(const char *pszKey);//根据键值获取对应的Hash节点
};

#endif
