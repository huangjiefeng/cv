/*
 * 定长对象的内存池管理模块（非线程安全） cpp文件 By Lzy
 * 2011-11-28   V2.0.1.0    1、调整数组链实现，使得回收对象的时间效率也是近乎常数级
 *                          2、新增内存对象索引功能
 *                          3、删除AllocMemory和RecycleMemory中在参数1从“**”改成“*”后残留的代码
 *                          4、调整结构体声明的命名，使其与私有函数相区分
 * 2011-11-27   V2.0.0.1    自定义的初始化/释放函数中增加用户自定义参数的传入，各函数参数表也因此做了相应修改
 * 2011-11-25   V2.0.0.0    修改函数接口，使用数组链索引管理内存块内的对象分配，使得单个对象的释放成为可能，支持自动调用初始化函数 By Lzy
 * 2011-08-25   V1.0.0.0    By Lzy
 */

#include "SimpleMemPool.h"

#define MALLOC_SIZE 50 //定义每次创建的内存块能存放多少个特定长度的对象建议不要超过1000

#define NODE_END -1 //不可改动值，表示指向的目标为空
#define NODE_UNDEFINE -2 //不可改动值，表示内容未定义

namespace simple_mem_pool
{

    struct S_MemBlockNode;

    struct S_MemoryManager //内存池描述节点
    {
        S_MemBlockNode *m_pHead;//内存块管理结构链表的头结构指针
        S_MemBlockNode *m_pPre;//最近一次成功分配内存时指向的内存块节点
        int m_nObjectSize;//内存块存放的对象的固定大小
        int m_nBlockNodeCount;//管理的S_MemBlockNode的数量，用于辅助实现索引定位对象块的功能
        int m_nPreBlockNodeIndex;//当前m_pPre指向的内存块的索引值，用于辅助实现索引定位对象块的功能
        ObjectStructFunc m_pInitializeFunc;//初始化对象时要调用的函数的指针
        ObjectStructFunc m_pReleaseFunc;//释放对象时要调用的函数的指针
    };

    struct S_MemBlockNode //内存块管理节点
    {
        void *m_pBlock;//单块内存的指针
        void *m_pTail;//内存的尾块地址
        struct _ObjectManagerNode
        {
            int m_nNextAvailable;
            int m_nNextUsing;
            int m_nPreUsing;
        }m_nControlLink[MALLOC_SIZE];//分配控制链
        int m_nNextFreeIndex;//下一个可分配索引值
        int m_nFirstUsingIndex;//首个使用中的索引值
        S_MemBlockNode *m_pNextNode;//下一个管理节点地址
    };

    /*
     * 【仅本文件内代码使用】内存块回收
     * @param S_MemoryManager *pManageNode,有效的内存管理指针，函数不做检查
     * @param S_MemBlockNode *pNode,要回收的内存块的指针
     * @param bool bWithLinkAdjust,是否同时调整使用记录链
     * @param void *pParam,用户自定义参数，如果定义了释放函数，将作为该函数的第二个参数传入
     * @return void
     */
    void _BlockRecycle(S_MemoryManager *pManageNode,S_MemBlockNode *pBlockNode,bool bWithLinkAdjust,void *pParam)
    {
        ObjectStructFunc pReleaseFunc = pManageNode->m_pReleaseFunc;
        if (pReleaseFunc != NULL || bWithLinkAdjust)
        {
            int nViewIndex = pBlockNode->m_nFirstUsingIndex;
            while (nViewIndex >= 0)
            {
                if (pReleaseFunc != NULL)
                    pReleaseFunc(&(((char *)(pBlockNode->m_pBlock))[pManageNode->m_nObjectSize * nViewIndex]),pParam);
                if (bWithLinkAdjust)
                {
                    S_MemBlockNode::_ObjectManagerNode * pTmpControlNode = &(pBlockNode->m_nControlLink[nViewIndex]);
                    pTmpControlNode->m_nNextAvailable = pBlockNode->m_nNextFreeIndex;;
                    pBlockNode->m_nNextFreeIndex = nViewIndex;
                    nViewIndex = pTmpControlNode->m_nNextUsing;
                    pTmpControlNode->m_nNextUsing = NODE_UNDEFINE;
                    pTmpControlNode->m_nPreUsing = NODE_UNDEFINE;
                }
                else
                    nViewIndex = pBlockNode->m_nControlLink[nViewIndex].m_nNextUsing;
            }
            pBlockNode->m_nFirstUsingIndex = NODE_END;
        }
    }

    /*
     * 【公开】创建内存池
     * @param int nObjectSize,指定该内存负责分配的定长对象的大小，一个内存管理指针只能管理一种长度的对象的分配
     * @param ObjectStructFunc fInitializeFunc = NULL,为内存池分配对象初始化的函数指针
     * @param ObjectStructFunc fReleaseFunc = NULL,内存池内对象释放时调用的额外处理函数（类似析构函数），不需要额外处理则维持默认值
     * @return void *,返回内存池管理指针，创建失败时返回NULL
     */
    void *CreateMemPool(int nObjectSize,ObjectStructFunc fInitializeFunc,ObjectStructFunc fReleaseFunc)
    {
        if (nObjectSize < 1)
            return NULL;
        S_MemoryManager *pManageNode = new S_MemoryManager;
        if (pManageNode != NULL)//不保证new失败时一定抛出bad_alloc异常，所以做指针有效性检查
        {
            pManageNode->m_nObjectSize = nObjectSize;
            pManageNode->m_pHead = NULL;
            pManageNode->m_pPre = NULL;
            pManageNode->m_pInitializeFunc = fInitializeFunc;
            pManageNode->m_pReleaseFunc = fReleaseFunc;
            pManageNode->m_nBlockNodeCount = 0;
        }
        return pManageNode;
    }

    /*
     * 【公开】从内存池分配指定大小内存空间
     * @param void *pManageP,有效的内存池管理指针，可用CreateMenPool创建
     * @param void *pParam = NULL,用户自定义的参数，如果有设置初始化函数，将作为第二个参数传入给该函数
     * @param int *pGetIndex = NULL,可用于接收当前返回内存块的索引值
     * @return void *,返回分配到的内存的地址，出现错误将返回NULL
     */
    void *AllocMemory(void *pManageP,void *pParam,int *pGetIndex)
    {
        if (pManageP == NULL)
            return NULL;
        S_MemoryManager *pManageNode = (S_MemoryManager *)pManageP;
        S_MemBlockNode *pMemBlockNode = NULL;
        S_MemBlockNode::_ObjectManagerNode * pTmpControlNode;
        int nPreBlockIndex;    
        //查找有足够空间分配的内存块
        pMemBlockNode = pManageNode->m_pPre;
        nPreBlockIndex = pManageNode->m_nPreBlockNodeIndex;
        while (pMemBlockNode != NULL)
        {
            if (pMemBlockNode->m_nNextFreeIndex  == NODE_END)
            {
                pMemBlockNode = pMemBlockNode->m_pNextNode;
                --nPreBlockIndex;
            }
            else
            {
                pManageNode->m_pPre = pMemBlockNode;
                pManageNode->m_nPreBlockNodeIndex = nPreBlockIndex;
                break;
            }
        }
        if (pMemBlockNode == NULL)
        {
            //创建新内存块
            pMemBlockNode = new S_MemBlockNode;
            if (pMemBlockNode == NULL)
                return NULL;
            pMemBlockNode->m_pBlock = malloc(pManageNode->m_nObjectSize * MALLOC_SIZE);
            if (pMemBlockNode->m_pBlock != NULL)
            {
                for (int nLoopVar = 0;nLoopVar < MALLOC_SIZE;++nLoopVar)
                {
                    pTmpControlNode = &(pMemBlockNode->m_nControlLink[nLoopVar]);
                    pTmpControlNode->m_nNextAvailable = nLoopVar + 1;
                    pTmpControlNode->m_nNextUsing = NODE_UNDEFINE;
                    pTmpControlNode->m_nPreUsing = NODE_UNDEFINE;
                }
                pMemBlockNode->m_nControlLink[MALLOC_SIZE - 1].m_nNextAvailable = NODE_END;
                pMemBlockNode->m_nNextFreeIndex = 0;
                pMemBlockNode->m_pTail = (char *)pMemBlockNode->m_pBlock + pManageNode->m_nObjectSize * (MALLOC_SIZE - 1);
                pMemBlockNode->m_pNextNode = pManageNode->m_pHead;
                pMemBlockNode->m_nFirstUsingIndex = NODE_END;
                pManageNode->m_pHead = pMemBlockNode;
                pManageNode->m_pPre = pMemBlockNode;
                nPreBlockIndex = pManageNode->m_nBlockNodeCount;
                pManageNode->m_nPreBlockNodeIndex = nPreBlockIndex;
                pManageNode->m_nBlockNodeCount++;
            }
            else
            {
                delete pMemBlockNode;
                return NULL;
            }
        }
        //分配内存
        int nTmpIndex = pMemBlockNode->m_nNextFreeIndex;
        void *pReturnBlock = &(((char *)(pMemBlockNode->m_pBlock))[pManageNode->m_nObjectSize * nTmpIndex]);
        pTmpControlNode = &(pMemBlockNode->m_nControlLink[nTmpIndex]);
        pMemBlockNode->m_nNextFreeIndex = pTmpControlNode->m_nNextAvailable;
        pTmpControlNode->m_nNextAvailable = NODE_UNDEFINE;
        pTmpControlNode->m_nPreUsing = NODE_END;
        if (pMemBlockNode->m_nFirstUsingIndex != NODE_END)
            pMemBlockNode->m_nControlLink[pMemBlockNode->m_nFirstUsingIndex].m_nPreUsing = nTmpIndex;
        pTmpControlNode->m_nNextUsing = pMemBlockNode->m_nFirstUsingIndex;
        pMemBlockNode->m_nFirstUsingIndex = nTmpIndex;
        if (pManageNode->m_pInitializeFunc != NULL)
            pManageNode->m_pInitializeFunc(pReturnBlock,pParam);
        if (pGetIndex != NULL)
            *pGetIndex = nPreBlockIndex * MALLOC_SIZE + nTmpIndex;
        return pReturnBlock;
    }

    /*
     * 【公开】回收内存池中已分配的对象
     * @param void *pManageP,有效的内存池管理指针
     * @param void *pRecycleObject = NULL,要回收对象的指针，如果设置为NULL，则回收所有对象
     * @param void *pParam = NULL,用户自定义参数，如果定义了释放函数，将作为该函数的第二个参数传入
     * @return void
     */
    void RecycleMemory(void *pManageP,void *pRecycleObject,void *pParam)
    {
        if (pManageP == NULL)
            return;
        S_MemoryManager *pManageNode = (S_MemoryManager *)pManageP;
        S_MemBlockNode *pNode = pManageNode->m_pHead;
        while (pNode != NULL)
        {
            if (pRecycleObject != NULL)
            {
                if (pNode->m_pBlock <= pRecycleObject && pNode->m_pTail >= pRecycleObject)
                {
                    int nRecycleIndex = ((char *)pRecycleObject - (char *)pNode->m_pBlock) / pManageNode->m_nObjectSize;
                    S_MemBlockNode::_ObjectManagerNode * pTmpControlNode = &(pNode->m_nControlLink[nRecycleIndex]);
                    if (pTmpControlNode->m_nNextAvailable != NODE_UNDEFINE)
                        return;
                    if (pManageNode->m_pReleaseFunc != NULL)
                        pManageNode->m_pReleaseFunc(pRecycleObject,pParam);
                    if (pTmpControlNode->m_nNextUsing != NODE_END)
                        pNode->m_nControlLink[pTmpControlNode->m_nNextUsing].m_nPreUsing = pTmpControlNode->m_nPreUsing;
                    if (pTmpControlNode->m_nPreUsing != NODE_END)
                        pNode->m_nControlLink[pTmpControlNode->m_nPreUsing].m_nNextUsing = pTmpControlNode->m_nNextUsing;
                    else
                        pNode->m_nFirstUsingIndex = pTmpControlNode->m_nNextUsing;
                    pTmpControlNode->m_nNextUsing = NODE_UNDEFINE;
                    pTmpControlNode->m_nPreUsing = NODE_UNDEFINE;
                    pTmpControlNode->m_nNextAvailable = pNode->m_nNextFreeIndex;
                    pNode->m_nNextFreeIndex = nRecycleIndex;
                    return;
                }
            }
            else
                simple_mem_pool::_BlockRecycle(pManageNode,pNode,true,pParam);
            pNode = pNode->m_pNextNode;
        }
    }

    /*
     * 【公开】回收内存池中已分配的对象
     * @param void *pManageP,有效的内存池管理指针
     * @param int nRecycleIndex,要回收对象的索引
     * @param void *pParam = NULL,用户自定义参数，如果定义了释放函数，将作为该函数的第二个参数传入
     * @return void
     */
    void RecycleMemory(void *pManageP,int nRecycleIndex,void *pParam)
    {
        if (pManageP == NULL)
            return;
        S_MemoryManager *pManageNode = (S_MemoryManager *)pManageP;
        int nBlockIndex = pManageNode->m_nBlockNodeCount - nRecycleIndex / MALLOC_SIZE;
        int nObjectIndex = nRecycleIndex % MALLOC_SIZE;
        if (nBlockIndex < 1 || nBlockIndex > pManageNode->m_nBlockNodeCount)
            return;
        S_MemBlockNode *pNode = pManageNode->m_pHead;
        while (pNode != NULL)
        {
            if (nBlockIndex > 1)
            {
                pNode = pNode->m_pNextNode;
                --nBlockIndex;
            }
            else
                break;
        }
        if (pNode != NULL)
        {
            S_MemBlockNode::_ObjectManagerNode * pTmpControlNode = &(pNode->m_nControlLink[nObjectIndex]);
            if (pTmpControlNode->m_nNextAvailable != NODE_UNDEFINE)
                return;
            if (pManageNode->m_pReleaseFunc != NULL)
                pManageNode->m_pReleaseFunc(&(((char *)(pNode->m_pBlock))[nObjectIndex * pManageNode->m_nObjectSize]),pParam);
            if (pTmpControlNode->m_nNextUsing != NODE_END)
                pNode->m_nControlLink[pTmpControlNode->m_nNextUsing].m_nPreUsing = pTmpControlNode->m_nPreUsing;
            if (pTmpControlNode->m_nPreUsing != NODE_END)
                pNode->m_nControlLink[pTmpControlNode->m_nPreUsing].m_nNextUsing = pTmpControlNode->m_nNextUsing;
            else
                pNode->m_nFirstUsingIndex = pTmpControlNode->m_nNextUsing;
            pTmpControlNode->m_nNextUsing = NODE_UNDEFINE;
            pTmpControlNode->m_nPreUsing = NODE_UNDEFINE;
            pTmpControlNode->m_nNextAvailable = pNode->m_nNextFreeIndex;
            pNode->m_nNextFreeIndex = nRecycleIndex;
        }
    }

    /*
     * 【公开】释放内存池
     * @param void **pManageP,内存池管理指针的引用
     * @param void *pParam = NULL,用户自定义参数，如果定义了释放函数，将作为该函数的第二个参数传入
     * @return void
     */
    void ReleaseMemPool(void **pManageP,void *pParam)
    {
        if (pManageP == NULL)
            return;
        S_MemoryManager *pManageNode = (S_MemoryManager *)(*pManageP);
        if (pManageNode == NULL)
            return;
        S_MemBlockNode *pNode = pManageNode->m_pHead;
        S_MemBlockNode *pTmp;
        while (pNode != NULL)
        {
            simple_mem_pool::_BlockRecycle(pManageNode,pNode,false,pParam);
            free(pNode->m_pBlock);
            pTmp = pNode;
            pNode = pNode->m_pNextNode;
            delete pTmp;
        }
        delete pManageNode;
        *pManageP = NULL;
    }

    /*
     * 【公开】根据索引值查找内存对象
     * @param void *pManageP,有效的内存池管理指针
     * @param int nFindIndex
     * @return void
     */
    void *FindMemory(void *pManageP,int nFindIndex)
    {
        if (pManageP == NULL)
            return NULL;
        S_MemoryManager *pManageNode = (S_MemoryManager *)pManageP;
        int nBlockIndex = pManageNode->m_nBlockNodeCount - nFindIndex / MALLOC_SIZE;
        int nObjectIndex = nFindIndex % MALLOC_SIZE;
        if (nBlockIndex < 1 || nBlockIndex > pManageNode->m_nBlockNodeCount)
            return NULL;
        S_MemBlockNode *pNode = pManageNode->m_pHead;
        while (pNode != NULL)
        {
            if (nBlockIndex > 1)
            {
                pNode = pNode->m_pNextNode;
                --nBlockIndex;
            }
            else
                break;
        }
        if (pNode != NULL)
        {
            S_MemBlockNode::_ObjectManagerNode * pTmpControlNode = &(pNode->m_nControlLink[nObjectIndex]);
            if (pTmpControlNode->m_nNextAvailable != NODE_UNDEFINE)
                return NULL;
            else
                return &(((char *)(pNode->m_pBlock))[nObjectIndex * pManageNode->m_nObjectSize]);
        }
        else
            return NULL;
    }
}
