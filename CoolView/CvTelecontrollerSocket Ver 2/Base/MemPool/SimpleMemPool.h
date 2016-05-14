/*
 * 定长对象的内存池管理模块（非线程安全） 头文件 By Lzy
 * 2011-11-28   V2.0.1.0    1、调整数组链实现，使得回收对象的时间效率也是近乎常数级
 *                          2、新增内存对象索引功能
 *                          3、删除AllocMemory和RecycleMemory中在参数1从“**”改成“*”后残留的代码
 *                          4、调整结构体声明的命名，使其与私有函数相区分
 * 2011-11-27   V2.0.0.1    自定义的初始化/释放函数中增加用户自定义参数的传入，各函数参数表也因此做了相应修改
 * 2011-11-25   V2.0.0.0    修改函数接口，使用数组链索引管理内存块内的对象分配，使得单个对象的释放成为可能，支持自动调用初始化函数 By Lzy
 * 2011-08-25   V1.0.0.0    By Lzy
 */

#ifndef SIMPLE_MEM_POOL
#define SIMPLE_MEM_POOL

#include <stdlib.h>

/*
 * 【函数指针定义】对内存池对象初始化、释放时调用的函数的函数指针
 * @param void *pObject,要初始化/释放的对象的指针
 * @param void *pParam,用户自定义参数，该参数在调用本模块函数时可选择是否传入
 * @return void
 */
typedef void (* ObjectStructFunc)(void *pObject,void *pParam);

namespace simple_mem_pool
{
    void *CreateMemPool(int nObjectSize,ObjectStructFunc fInitializeFunc = NULL,ObjectStructFunc fReleaseFunc = NULL);//创建内存池
    void *AllocMemory(void *pManageP,void *pParam = NULL,int *pGetIndex = NULL);//分配指定大小内存空间
    void RecycleMemory(void *pManageP,void *pRecycleObject = NULL,void *pParam = NULL);//回收内存池中已分配的对象
    void RecycleMemory(void *pManageP,int nRecycleIndex,void *pParam = NULL);//回收内存池中已分配的对象
    void *FindMemory(void *pManageP,int nFindIndex);//根据索引值查找内存对象
    void ReleaseMemPool(void **pManageP,void *pParam = NULL);//释放内存池
}
#endif
