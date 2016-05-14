/*
 * 支持断点续传、分段传输的文件上传、下载的服务器端模块，可支持对4G以上文件的操作（最大同时传输文件数为5000）
 * 2012-05-17   V1.0.0.0    初版
 * 2012-05-09   V0.0.0.0    创建文件
 */

#ifdef LINUX
#define _FILE_OFFSET_BITS 64
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include "FileUD.h"
#include "SimpleMemPool.h"
#include "json.h"
using namespace simple_mem_pool;
#ifdef WIN32
#define FILE_STAT _stati64
#define STAT_STRUCT struct _stat64
#define FILE_SEEK _fseeki64_nolock
#define FILE_WRITE _fwrite_nolock
#else
#define FILE_STAT stat
#define STAT_STRUCT struct stat
#define FILE_SEEK fseek
#define FILE_WRITE fwrite
#endif

#ifdef WIN32
#pragma warning ( disable : 4996 )
#endif

//------定义错误码------
//  ------RegUpdateTrans、RegDownloadTrans函数的错误码------
#define CFUD_UPDATE_SET_PATH_NULL -1 //上传位置参数为NULL
#define CFUD_UPDATE_FILE_SIZE_ERROR -2 //上传文件大小值异常
#define CFUD_FILE_PATH_IS_DIR -3 //给出的路径仅是目录路径
#define CFUD_FILE_INFO_PATH_IS_DIR -4 //给出的指向续传信息文件的路径仅是目录路径
#define CFUD_FILE_PATH_SAME -5 //指定的传输文件和续传信息文件为相同文件，这里必须指定两个不同的文件
#define CFUD_UPDATE_FILE_EXIST -6 //指定的上传位置存在同名文件，且未指定允许覆盖
#define CFUD_FILE_OPERATION_ERROR -7 //对要传输文件的操作失败
#define CFUD_INFO_FILE_PATH_NULL -8 //续传信息记录文件的路径参数为NULL
#define CFUD_ERROR_UNKNOW -9 //未知错误
#define CFUD_OBJECT_RELEASE -10 //类实例即将析构，当前执行的操作被终止
#define CFUD_INFO_FILE_OPERATION_ERROR -11 //对上传文件的续传信息记录文件操作失败
#define CFUD_FILE_UPDATING -12 //指定要上传的文件正在被上传中
#define CFUD_TRANS_TASK_FULL -13 //当前正在传输的文件数达到了类实例可容纳的最大值（该值在实例创建时的nMaxTransNum参数中指定）
#define CFUD_UPDATE_BLOCK_COUNT_ERROR -14 //上传文件的分块数目异常
#define CFUD_ALLOC_MEMORY_FAILED -15 //分配内存失败
#define CFUD_CREATE_THREAD_FAILED -16 //创建线程失败
#define CFUD_DOWNLOAD_FILE_NOT_EXIST -17 //要下载的文件不存在

//  ------传输连接返回给客户端的传输权限码检测结果（“ResultCode字段”）------
#define CFUD_TRANS_PASS 0 //没有发生错误，同时表示传输权限码检测通过
#define CFUD_TRANS_JSON_ERROR 1 //发送的数据无法用json方式解析
#define CFUD_TRANS_NO_TRANS_CODE 2 //缺少传输权限码
#define CFUD_TRANS_CODE_UNAVAILABLE 3 //传输权限号无效
#define CFUD_TRANS_OBJECT_RELEASE 4 //模块即将析构
#define CFUD_TRANS_CONNECTION_FULL 5 //该文件的并发传输数已满

//------定义连接返回的JSON数据“Command”字段中的值，这里的定义值不允许随意修改------
#define JSON_COMMAND_ID_RESULT 0 //返回的消息为对传输权限值的处理结果
#define JSON_COMMAND_SET_TRANS_RANGE 1 //返回的消息为告知客户端连接传输的范围值
#define JSON_COMMAND_WAIT_TRANS_RANGE 2 //返回的消息为要求客户端连接提供下载范围值

CFileUpDownServ *CFileUpDownServ::m_pCFUDInstance = NULL;

/*
 * 【静态】【公开】创建类实例，如果类实例已创建则直接返回
 * @addition info,nMaxTransNum是有范围限制的，设置有效范围为[1-5000]，不在这范围内的按最近值修正
 * @param in int nListenPort,传输监听的端口号
 * @param in short snMaxTransNum,支持并发传输的文件数量（上传、下载文件量合算）
 * @param in unsigned int uWaitOverTime,文件传输信息注册后等待客户端发起传输连接的最大等待时间（时间单位为一次扫描线程的扫描循环），设置值必须大于0（函数自动修正）
 * @return CFileUpDownServ *,返回创建好的类实例，返回NULL表示创建失败
 */
CFileUpDownServ *CFileUpDownServ::CreateInstance(int nListenPort,short snMaxTransNum,unsigned int uWaitOverTime)
{
    if (CFileUpDownServ::m_pCFUDInstance == NULL)
        CFileUpDownServ::m_pCFUDInstance = new CFileUpDownServ(nListenPort,snMaxTransNum,uWaitOverTime);
    return CFileUpDownServ::m_pCFUDInstance;
}

/*
 * 【静态】【公开】如果单例对象存在则释放单例对象
 * @return void
 */
void CFileUpDownServ::ReleaseInstance()
{
    delete CFileUpDownServ::m_pCFUDInstance;
}

/*
 * 【私有】类构造函数
 * @addition info,nMaxTransNum是有范围限制的，设置有效范围为[1-5000]，不在这范围内的按最近值修正
 * @param in int nListenPort,传输监听的端口号
 * @param in short snMaxTransNum,支持并发传输的文件数量（上传、下载文件量合算）
 * @param in unsigned int uWaitOverTime,文件传输信息注册后等待客户端发起传输连接的最大等待时间（时间单位为一次扫描线程的扫描循环），设置值必须大于0（函数自动修正）
 */
CFileUpDownServ::CFileUpDownServ(int nListenPort,short snMaxTransNum,unsigned int uWaitOverTime)
{
    this->m_bIsRelease = false;
    this->m_pUpdatePartListMemPoolLock = NULL;
    this->m_pUpdatePartListMemPool = NULL;
    this->m_pTransNodeArrayList = NULL;
    this->m_pConnectionMemPool = NULL;
    this->m_pConnectionMemPoolLock = NULL;
    this->m_pAdjustNodeLock = CreateCS();
    this->m_pUpdatePartListMemPoolLock = CreateCS();
    this->m_pConnectionMemPoolLock = CreateCS();
    if (this->m_pAdjustNodeLock == NULL || 
        this->m_pUpdatePartListMemPoolLock == NULL || 
        this->m_pConnectionMemPoolLock == NULL)
    {
        printf("[E]CFileUpDownServ:Create CREATICAL_SECTION Failed\n");
        goto tag_ConstructError;
    }
    if (uWaitOverTime < 0)
        this->m_uWaitOverTime = 1;
    else
        this->m_uWaitOverTime = uWaitOverTime;
    this->m_pUpdatePartListMemPool = CreateMemPool(sizeof(SUpdatePartNode));
    this->m_pConnectionMemPool = CreateMemPool(sizeof(void *),NULL,_ForReleaseDownloadConnection);
    if (this->m_pUpdatePartListMemPool == NULL || this->m_pConnectionMemPool == NULL)
    {
        printf("[E]CFileUpDownServ:Create MemPool Failed\n");
        goto tag_ConstructError;
    }
    //1、创建传输信息数组
    if (snMaxTransNum < 1)
        snMaxTransNum = 1;
    if (snMaxTransNum > 5000)
        snMaxTransNum = 5000;
    this->m_pTransNodeArrayList = NULL;
    this->m_pTransNodeArrayList = (STransNode *)malloc(sizeof(STransNode) * snMaxTransNum);
    if (this->m_pTransNodeArrayList == NULL)
    {
        printf("[E]CFileUpDownServ:Create Array Failed\n");
        goto tag_ConstructError;
    }
    this->m_snTransNodeArrayListCap = snMaxTransNum;
    this->m_nNextUsingIndex = -1;
    this->m_nNextFreeIndex = 0;
    for (short snLoopVar = 0;snLoopVar < snMaxTransNum;++snLoopVar)
    {
        STransNode *pTmpViewNode = &(this->m_pTransNodeArrayList[snLoopVar]);
        pTmpViewNode->m_pCurConnectionLock = CreateCS();//省略错误检测
        pTmpViewNode->m_pReadWriteFileLock = CreateCS();//省略错误检测
        new (&pTmpViewNode->m_szInfoFilePath) string;//placement new
        pTmpViewNode->m_pTransFile = NULL;
        pTmpViewNode->m_pUpdatePartList = NULL;
        pTmpViewNode->m_uTimeLeft = 0;
        pTmpViewNode->m_uCurConnectionCount = 0;
        pTmpViewNode->m_nNextIndex = snLoopVar + 1;
    }
    this->m_pTransNodeArrayList[snMaxTransNum - 1].m_nNextIndex = -1;
    CFileUpDownServ::m_pCFUDInstance = this;//为了下面启动的线程不会出现问题而加入的语句
    //2、启动超时检测线程
    if (CreateThreadManager(_RefreshWaitTimeoutThread,true) == NULL)
        printf("[W]CFileUpDownServ:Can Not Create Thread For Refreshing Wait Time Out\n");
    //3、创建网络服务器端监听
    CCommonSocket *pCommonSocket = CCommonSocket::GetInstance();
    if (pCommonSocket == NULL)
    {
        printf("[E]CFileUpDownServ:Network Initialize Failed\n");
        goto tag_ConstructError;
    }
#ifdef CFUDS_ENABLED_IPV4
    this->m_pTCPServV4 = pCommonSocket->CreateSocket(Version4,TCP_Protocol);
    if (this->m_pTCPServV4 != NULL)
    {
        if (pCommonSocket->BindSocket(this->m_pTCPServV4,NULL,nListenPort) == SOCKET_ERROR)
            printf("[W]CFileUpDownServ:Bind IPv4 Socket Failed\n");
        if (CreateThreadManager(_SocketManageThread,true,this->m_pTCPServV4) == NULL)
            printf("[W]CFileUpDownServ:Create IPv4 Socket Manage Thread Failed\n");
    }
    else
        printf("[W]CFileUpDownServ:IPv4 Socket Initialize Failed\n");
#endif
#ifdef CFUDS_ENABLED_IPV6
    this->m_pTCPServV6 = pCommonSocket->CreateSocket(Version6,TCP_Protocol);
    if (this->m_pTCPServV6 != NULL)
    {
        if (pCommonSocket->BindSocket(this->m_pTCPServV6,NULL,nListenPort) == SOCKET_ERROR)
            printf("[W]CFileUpDownServ:Bind IPv6 Socket Failed\n");
        if (CreateThreadManager(_SocketManageThread,true,this->m_pTCPServV6) == NULL)
            printf("[W]CFileUpDownServ:Create IPv6 Socket Manage Thread Failed\n");
    }
    else
        printf("[W]CFileUpDownServ:IPv6 Socket Initialize Failed\n");
#endif
    return;
    //E、错误处理
tag_ConstructError:
    this->m_bIsRelease = true;
    if (this->m_pAdjustNodeLock != NULL)
        ReleaseCS(this->m_pAdjustNodeLock);
    if (this->m_pUpdatePartListMemPoolLock != NULL)
        ReleaseCS(this->m_pUpdatePartListMemPoolLock);
    if (this->m_pUpdatePartListMemPool != NULL)
        ReleaseMemPool(&this->m_pUpdatePartListMemPool);
    if (this->m_pConnectionMemPool != NULL)
        ReleaseMemPool(&this->m_pConnectionMemPool,NULL);
    if (this->m_pConnectionMemPoolLock != NULL)
        ReleaseCS(this->m_pConnectionMemPoolLock);
    if (this->m_pTransNodeArrayList != NULL)
    {
        for (short snLoopVar = 0;snLoopVar < snMaxTransNum;++snLoopVar)
        {
            STransNode *pTmpViewNode = &(this->m_pTransNodeArrayList[snLoopVar]);
            ReleaseCS(pTmpViewNode->m_pCurConnectionLock);
            ReleaseCS(pTmpViewNode->m_pReadWriteFileLock);
            pTmpViewNode->m_szInfoFilePath.~string();//placement new 对应的析构操作
        }
    }
}

/*
 * 【私有】类析构函数
 */
CFileUpDownServ::~CFileUpDownServ()
{
    if (this->m_bIsRelease == true)
        return;
    this->m_bIsRelease = true;
    //1、关闭服务器端监听
    CCommonSocket *pCommonSocket = CCommonSocket::GetInstance();
    if (pCommonSocket != NULL)
    {
#ifdef CFUDS_ENABLED_IPV4
        if (this->m_pTCPServV4 != NULL)
            pCommonSocket->CloseSocket(this->m_pTCPServV4);
#endif
#ifdef CFUDS_ENABLED_IPV4
        if (this->m_pTCPServV6 != NULL)
            pCommonSocket->CloseSocket(this->m_pTCPServV6);
#endif
        ;//特意留空，防止上面两段语句都不存在的情况
    }
    //2、释放传输信息数组及内存池资源
    ReleaseMemPool(&this->m_pConnectionMemPool,pCommonSocket);//要先释放这个内存池是为了关闭所有下载连接
    if (this->m_pTransNodeArrayList != NULL)
    {
        while (1)
        {
            while (this->m_nNextUsingIndex != -1)
                Sleep_Thread(5);
            EnterCS(this->m_pAdjustNodeLock);
            if (this->m_nNextUsingIndex != -1)
                ExitCS(this->m_pAdjustNodeLock);
            else
                break;
        }
        for (short snLoopVar = 0;snLoopVar < this->m_snTransNodeArrayListCap;++snLoopVar)
        {
            this->m_pTransNodeArrayList[snLoopVar].m_szInfoFilePath.~string();//placement new 对应的析构操作
            ReleaseCS(this->m_pTransNodeArrayList[snLoopVar].m_pCurConnectionLock);
        }
        free(this->m_pTransNodeArrayList);
    }
    ReleaseMemPool(&this->m_pUpdatePartListMemPool);
    //3、释放临界区
    ReleaseCS(this->m_pAdjustNodeLock);
    ReleaseCS(this->m_pUpdatePartListMemPoolLock);
    ReleaseCS(this->m_pConnectionMemPoolLock);
}

/*
 * 【静态】【私有】【回调函数】内存池释放下载连接时调用的函数
 * @param in void *pObject,要释放的连接的标识
 * @param in void *pCommonSocket,CCommonSocket类的实例指针
 * @return void
 */
void CFileUpDownServ::_ForReleaseDownloadConnection(void *pObject,void *pCommonSocket)
{
    if (pCommonSocket != NULL && (*(void **)(pObject)) != NULL)
    {
        ((CCommonSocket *)pCommonSocket)->CloseSocket(*(void **)(pObject));
        *(void **)(pObject) = NULL;
    }
}

/*
 * 【公开】设置上传信息，并返回上传权限码，该权限码在传输前需要验证
 * @addition info,1、关于bIsOverWrite参数的额外补充：如果指定的文件存在，但属于未上传完成的文件（文件大小与要上传的文件必须匹配），则会启动续传，而不是覆盖
 *                2、上传分块数snBlockCount只是对文件分块的一个建议值，模块实际操作时可能会有所调整
 * @param in const char *pszFilePath,上传文件的存放路径
 * @param in const char *pszInfoFilePath,记录上传文件续传信息的文件的存放路径，每个上传文件都必须唯一对应一个续传记录文件
 * @param in long long llFileSize,上传文件的大小
 * @param in unsigned int uMaxConnectionCount,上传该文件时允许同时发起的最大上传连接数，如果设置为0，则等同于放弃传输操作
 * @param in unsigned short snBlockCount,上传文件的分块数。如果该文件是续传状态，则沿用初次建立时的设置，此参数无效
 * @param in bool bIsOverWrite,如果该文件已经存在，是否覆盖该文件
 * @param out long long &rllFinished,接收已完成传输的数据量（字节为单位），如果文件不是续传状态，则填入的数据为0
 * @return int,如果返回值>=0，则表示设置成功，返回值为上传权限码，如果返回值小于0则表示返回的是错误码，详细错误码定义在本文件前面有说明
 */
int CFileUpDownServ::RegUpdateTrans(const char *pszFilePath,const char *pszInfoFilePath,long long llFileSize,unsigned int uMaxConnectionCount,unsigned short snBlockCount,bool bIsOverWrite,long long &rllFinished)
{
    //1、检查参数
    if (pszFilePath == NULL)
        return CFUD_UPDATE_SET_PATH_NULL;
    if (pszInfoFilePath == NULL)
        return CFUD_INFO_FILE_PATH_NULL;
    if (llFileSize < 0)
        return CFUD_UPDATE_FILE_SIZE_ERROR;
    if (snBlockCount < 1)
        return CFUD_UPDATE_BLOCK_COUNT_ERROR;
    size_t nPathMaxLen = strlen(pszInfoFilePath);
    if (nPathMaxLen < strlen(pszFilePath))
        nPathMaxLen = strlen(pszFilePath);
    if (strncmp(pszFilePath,pszInfoFilePath,nPathMaxLen) == 0)
        return CFUD_FILE_PATH_SAME;
    //2、锁定节点分配过程并检查是否有空闲信息存储节点可分配，创建管理线程
    int nReturnValue = 0;
    if (TryEnterCS(this->m_pAdjustNodeLock,this->m_bIsRelease) == false)
        return CFUD_OBJECT_RELEASE;
    void *pNodeManageThread = CreateThreadManager(_UpdateNodeManageThread,true,(void *)this->m_nNextFreeIndex,true);
    if (pNodeManageThread == NULL)
    {
        nReturnValue = CFUD_CREATE_THREAD_FAILED;
        goto tag_Update_Set_Error;
    }
    if (this->m_nNextFreeIndex == -1)
    {
        nReturnValue = CFUD_TRANS_TASK_FULL;
        goto tag_Update_Set_Error;
    }
    //3、检查文件是否存在
    STAT_STRUCT sFileInfo;//存放文件信息
    bool bCanContinueTrans = false;//记录文件是否可以续传
    if (FILE_STAT(pszFilePath,&sFileInfo) == 0)//检查要上传的文件是否存在
    {
        if ((sFileInfo.st_mode & S_IFDIR) == S_IFDIR)//检查给出的路径是否仅是目录路径
        {
            nReturnValue = CFUD_FILE_PATH_IS_DIR;
            goto tag_Update_Set_Error;
        }
        if (sFileInfo.st_size == llFileSize)//检查要上传文件大小与指定文件大小是否相同
        {
            if (FILE_STAT(pszInfoFilePath,&sFileInfo) == 0)//检查上传文件对应的续传信息记录文件是否存在
            {
                if ((sFileInfo.st_mode & S_IFDIR) == S_IFDIR)//检查给出的续传信息文件路径是否仅是目录路径
                {
                    nReturnValue = CFUD_FILE_INFO_PATH_IS_DIR;
                    goto tag_Update_Set_Error;
                }
                bCanContinueTrans = true;
            }
        }
        if (bCanContinueTrans == false && bIsOverWrite == false)
        {
            nReturnValue = CFUD_UPDATE_FILE_EXIST;
            goto tag_Update_Set_Error;
        }
    }
    //4、初始化分块大小信息
    long long llBlockSize = llFileSize / snBlockCount;
    if (llFileSize < snBlockCount)
        ++llBlockSize;
    //5、获取传输信息节点并初始化部分信息（不包括链表和文件句柄调整部分）
    STransNode *pTargetNode = &(this->m_pTransNodeArrayList[this->m_nNextFreeIndex]);
    this->m_uRandSeed += (unsigned int)time(NULL);
    srand(this->m_uRandSeed);
    pTargetNode->m_bIsUp = true;
    pTargetNode->m_llBlockSize = llBlockSize;
    pTargetNode->m_llFinishedBlockData = 0;
    pTargetNode->m_snIdentify = rand();
    if (pTargetNode->m_snIdentify < 0)
        pTargetNode->m_snIdentify *= -1;//此举是为了防止返回的传输权限码在32位int上出现负数（本模块要求int至少为32位长度）
    pTargetNode->m_szInfoFilePath = pszInfoFilePath;
    //pTargetNode->m_uCurConnectionCount = 0; //必然为0，否则说明出现了错误
    //pTargetNode->m_pUpdatePartList = NULL; //必然为NULL，否则说明出现了错误
    pTargetNode->m_uMaxConnectionCount = uMaxConnectionCount;
    pTargetNode->m_uTimeLeft = this->m_uWaitOverTime;
    pTargetNode->m_pNewSocketIdentify = NULL;
    //6、检查续传文件的上传标记，看文件是否正在上传中，[否]的状态下则更新续传文件上传标记，并载入上传信息
    FILE *pTmpF = fopen(pszInfoFilePath,"r+b");
    bool bIsReading = true;
    if (pTmpF != NULL)//如果文件存在，则检查首字节信息
    {
        if (fread(&bIsReading,sizeof(bool),1,pTmpF) == 1)
        {
            if (bIsReading == false)
            {
                bIsReading = true;
                if (FILE_SEEK(pTmpF,0,SEEK_SET) != 0)
                    nReturnValue = CFUD_INFO_FILE_OPERATION_ERROR;;
                if (nReturnValue < 0 || FILE_WRITE(&bIsReading,sizeof(bool),1,pTmpF) != 1)//写入上传中标记
                    nReturnValue = CFUD_INFO_FILE_OPERATION_ERROR;
                if (nReturnValue < 0 || fread(&(pTargetNode->m_llBlockSize),sizeof(long long),1,pTmpF) != 1)//获取最小分块大小
                    nReturnValue = CFUD_INFO_FILE_OPERATION_ERROR;
                if (nReturnValue < 0 || fread(&(pTargetNode->m_llFinishedBlockData),sizeof(long long),1,pTmpF) != 1)//获取已完成传输的分块的数据总量
                    nReturnValue = CFUD_INFO_FILE_OPERATION_ERROR;
                if (nReturnValue < 0 || fread(&snBlockCount,sizeof(snBlockCount),1,pTmpF) != 1)//获取已划分的区段数
                    nReturnValue = CFUD_INFO_FILE_OPERATION_ERROR;
                SUpdatePartNode *pPreNode = NULL;
                if (TryEnterCS(this->m_pUpdatePartListMemPoolLock,this->m_bIsRelease) == false)
                {
                    nReturnValue = CFUD_OBJECT_RELEASE;
                    fclose(pTmpF);
                    goto tag_Update_Set_Error;
                }
                while (snBlockCount > 0 && nReturnValue >= 0)
                {
                    SUpdatePartNode *pNewNode = (SUpdatePartNode *)AllocMemory(this->m_pUpdatePartListMemPool);
                    if (pNewNode != NULL)
                    {
                        if (fread(pNewNode,sizeof(SUpdatePartNode),1,pTmpF) != 1)
                            nReturnValue = CFUD_INFO_FILE_OPERATION_ERROR;
                        pNewNode->m_pSocket = NULL;
                        pNewNode->m_pNext = NULL;
                        if (pTargetNode->m_pUpdatePartList == NULL)
                            pTargetNode->m_pUpdatePartList = pNewNode;
                        if (pPreNode != NULL)
                            pPreNode->m_pNext = pNewNode;
                        pPreNode = pNewNode;
                    }
                    else
                        nReturnValue = CFUD_ALLOC_MEMORY_FAILED;
                    --snBlockCount;
                }
                if (nReturnValue < 0) //出现错误时释放链表
                {
                    pPreNode = pTargetNode->m_pUpdatePartList;
                    while (pPreNode != NULL)
                    {
                        pTargetNode->m_pUpdatePartList = pPreNode->m_pNext;
                        RecycleMemory(this->m_pUpdatePartListMemPool,pPreNode);
                        pPreNode = pTargetNode->m_pUpdatePartList;
                    }
                }
                ExitCS(this->m_pUpdatePartListMemPoolLock);
            }
            else
                nReturnValue = CFUD_FILE_UPDATING;
        }
        else
            bCanContinueTrans = false;
        fclose(pTmpF);
        if (nReturnValue < 0)
            goto tag_Update_Set_Error;
    }
    //7、初始化要上传的文件及续传信息文件
    if (bCanContinueTrans == false)
    {
        //7.1、创建要上传的文件并分配硬盘空间
        pTmpF = fopen(pszFilePath,"wb");
        if (pTmpF == NULL)
        {
            nReturnValue = CFUD_FILE_OPERATION_ERROR;
            goto tag_Update_Set_Error;
        }
        if (FILE_SEEK(pTmpF,llFileSize - 1,SEEK_SET) != 0)
            nReturnValue = CFUD_FILE_OPERATION_ERROR;
        char bFillByte = 0;//填充值
        if (nReturnValue < 0 || FILE_WRITE(&bFillByte,sizeof(char),1,pTmpF) != 1)
            nReturnValue = CFUD_FILE_OPERATION_ERROR;
        fclose(pTmpF);
        if (nReturnValue < 0)
            goto tag_Update_Set_Error;
        //7.2、创建上传文件对应的续传信息文件
        pTmpF = fopen(pszInfoFilePath,"wb");
        if (pTmpF == NULL)
        {
            nReturnValue = CFUD_INFO_FILE_OPERATION_ERROR;
            goto tag_Update_Set_Error;
        }
        bIsReading = true;
        if (FILE_WRITE(&bIsReading,sizeof(bool),1,pTmpF) != 1)//写入上传中标记
            nReturnValue = CFUD_INFO_FILE_OPERATION_ERROR;
        if (nReturnValue < 0 || FILE_WRITE(&llBlockSize,sizeof(llBlockSize),1,pTmpF) != 1)//写入最小分块大小
            nReturnValue = CFUD_INFO_FILE_OPERATION_ERROR;
        if (nReturnValue < 0 || FILE_WRITE(&(pTargetNode->m_llFinishedBlockData),sizeof(long long),1,pTmpF) != 1)//写入已完成传输块的合计容量
            nReturnValue = CFUD_INFO_FILE_OPERATION_ERROR;
        snBlockCount = 1;
        if (nReturnValue < 0 || FILE_WRITE(&snBlockCount,sizeof(snBlockCount),1,pTmpF) != 1)//写入划分区段数量
            nReturnValue = CFUD_INFO_FILE_OPERATION_ERROR;
        if (nReturnValue >= 0)//写入区段信息
        {
            if (TryEnterCS(this->m_pUpdatePartListMemPoolLock,this->m_bIsRelease) == false)
            {
                fclose(pTmpF);
                nReturnValue = CFUD_OBJECT_RELEASE;
                goto tag_Update_Set_Error;
            }
            pTargetNode->m_pUpdatePartList = (SUpdatePartNode *)AllocMemory(this->m_pUpdatePartListMemPool);
            if (pTargetNode->m_pUpdatePartList != NULL)
            {
                pTargetNode->m_pUpdatePartList->m_llStart = 1;
                pTargetNode->m_pUpdatePartList->m_llEnd = llFileSize;
                pTargetNode->m_pUpdatePartList->m_llFinished = 0;
                pTargetNode->m_pUpdatePartList->m_pSocket = NULL;
                pTargetNode->m_pUpdatePartList->m_pNext = NULL;
                if (FILE_WRITE(pTargetNode->m_pUpdatePartList,sizeof(SUpdatePartNode),1,pTmpF) != 1)
                {
                    RecycleMemory(this->m_pUpdatePartListMemPool,pTargetNode->m_pUpdatePartList);
                    pTargetNode->m_pUpdatePartList = NULL;
                    nReturnValue = CFUD_INFO_FILE_OPERATION_ERROR;
                }
            }
            else
                nReturnValue = CFUD_ALLOC_MEMORY_FAILED;
            ExitCS(this->m_pUpdatePartListMemPoolLock);
        }
        fclose(pTmpF);
        if (nReturnValue < 0)
            goto tag_Update_Set_Error;
    }
    //8、填充传输文件句柄
    pTmpF = fopen(pszFilePath,"r+b");
    if (pTmpF == NULL)
    {
        nReturnValue = CFUD_FILE_OPERATION_ERROR;
        goto tag_Update_Set_Error;
    }
    else
        pTargetNode->m_pTransFile = pTmpF;
    //9、计算已完成传输的数据量
    rllFinished = pTargetNode->m_llFinishedBlockData;
    SUpdatePartNode *pViewUpdatePartNode = pTargetNode->m_pUpdatePartList;
    while (pViewUpdatePartNode != NULL)
    {
        rllFinished += pViewUpdatePartNode->m_llFinished;
        pViewUpdatePartNode = pViewUpdatePartNode->m_pNext;
    }
    //10、计算传输权限码和调整在用节点
    nReturnValue = this->m_nNextFreeIndex;//计算传输权限码
    this->m_nNextFreeIndex = pTargetNode->m_nNextIndex;
    pTargetNode->m_nNextIndex = this->m_nNextUsingIndex;
    this->m_nNextUsingIndex = nReturnValue;
    nReturnValue += (int)(pTargetNode->m_snIdentify) << 16;//计算标识码
    Start_Thread(pNodeManageThread);
    ExitCS(this->m_pAdjustNodeLock);
    return nReturnValue;
tag_Update_Set_Error:
    //Error、错误处理，临界区释放
    if (pNodeManageThread != NULL)
        ReleaseThreadManager(pNodeManageThread);
    pTmpF = fopen(pszInfoFilePath,"r+b");
    if (pTmpF != NULL)
    {
        bIsReading = false;
        FILE_WRITE(&bIsReading,sizeof(bool),1,pTmpF);
        fclose(pTmpF);
    }
    else
        printf("[W]CFileUpDownServ:\"%s\" May Be Destroyed\n",pszInfoFilePath);
    ExitCS(this->m_pAdjustNodeLock);
    if (nReturnValue >= 0)
        nReturnValue = CFUD_ERROR_UNKNOW;
    return nReturnValue;
}

/*
 * 【公开】设置下载信息，并返回下载权限码，该权限码在传输前需要验证
 * @param in const char *pszFilePath,要下载文件的位置
 * @param in unsigned int uMaxConnectionCount,允许的最大并发下载数，如果设置为0，则等同于放弃传输操作
 * @param out long long &rllFileSize,接收下载文件的大小，字节为单位
 * @return int,如果返回值>=0，则表示设置成功，返回值为下载权限码，如果返回值小于0则表示返回的是错误码，详细错误码定义在本文件前面有说明
 */
int CFileUpDownServ::RegDownloadTrans(const char *pszFilePath,unsigned int uMaxConnectionCount,long long &rllFileSize)
{
    //1、检查参数
    if (pszFilePath == NULL)
        return CFUD_UPDATE_SET_PATH_NULL;
    //2、锁定节点分配过程并检查是否有空闲信息存储节点可分配
    int nReturnValue = 0;
    if (TryEnterCS(this->m_pAdjustNodeLock,this->m_bIsRelease) == false)
        return CFUD_OBJECT_RELEASE;
    if (this->m_nNextFreeIndex == -1)
    {
        nReturnValue = CFUD_TRANS_TASK_FULL;
        goto tag_Download_Set_Error;
    }
    void *pNodeManageThread = CreateThreadManager(_DownloadNodeManageThread,true,(void *)this->m_nNextFreeIndex,true);
    if (pNodeManageThread == NULL)
    {
        nReturnValue = CFUD_CREATE_THREAD_FAILED;
        goto tag_Download_Set_Error;
    }
    //3、获取文件的大小
    STAT_STRUCT sFileInfo;//存放文件信息
    if (FILE_STAT(pszFilePath,&sFileInfo) == 0)//检查要上传的文件是否存在
    {
        if ((sFileInfo.st_mode & S_IFDIR) == S_IFDIR)//检查给出的路径是否仅是目录路径
        {
            nReturnValue = CFUD_FILE_PATH_IS_DIR;
            goto tag_Download_Set_Error;
        }
        rllFileSize = sFileInfo.st_size;//填入要下载文件的大小
    }
    else
    {
        nReturnValue = CFUD_DOWNLOAD_FILE_NOT_EXIST;
        goto tag_Download_Set_Error;
    }
    //4、初始化传输节点信息并打开要传输的文件
    STransNode *pTargetNode = &(this->m_pTransNodeArrayList[this->m_nNextFreeIndex]);
    this->m_uRandSeed += (unsigned int)time(NULL);
    srand(this->m_uRandSeed);
    pTargetNode->m_bIsUp = false;
    pTargetNode->m_snIdentify = rand();
    if (pTargetNode->m_snIdentify < 0)
        pTargetNode->m_snIdentify *= -1;//此举是为了防止返回的传输权限码在32位int上出现负数（本模块要求int至少为32位长度）
    //pTargetNode->m_uCurConnectionCount = 0; //必然为0，否则说明出现了错误
    pTargetNode->m_uMaxConnectionCount = uMaxConnectionCount;
    pTargetNode->m_uTimeLeft = this->m_uWaitOverTime;
    pTargetNode->m_pNewSocketIdentify = NULL;
    FILE *pTmpF = fopen(pszFilePath,"rb");
    if (pTmpF == NULL)
    {
        nReturnValue = CFUD_FILE_OPERATION_ERROR;
        goto tag_Download_Set_Error;
    }
    else
        pTargetNode->m_pTransFile = pTmpF;
    //5、计算传输权限码和调整在用节点
    nReturnValue = this->m_nNextFreeIndex;//计算传输权限码
    this->m_nNextFreeIndex = pTargetNode->m_nNextIndex;
    pTargetNode->m_nNextIndex = this->m_nNextUsingIndex;
    this->m_nNextUsingIndex = nReturnValue;
    nReturnValue += (int)(pTargetNode->m_snIdentify) << 16;//计算标识码
    Start_Thread(pNodeManageThread);
    ExitCS(this->m_pAdjustNodeLock);
    return nReturnValue;
tag_Download_Set_Error:
    //Error、错误处理
    if (pNodeManageThread != NULL)
        ReleaseThreadManager(pNodeManageThread);
    ExitCS(this->m_pAdjustNodeLock);
    if (nReturnValue >= 0)
        nReturnValue = CFUD_ERROR_UNKNOW;
    return nReturnValue;
}

/*
 * 【静态】【私有】【线程函数】管理单个上传节点的线程函数
 * @param in void *pIndex,所管理的传输节点的索引值
 * @return void *,恒返回NULL
 */
void *CFileUpDownServ::_UpdateNodeManageThread(void *pIndex)
{
    CFileUpDownServ *pInstance = CFileUpDownServ::GetInstance();
    CCommonSocket *pCommonSocket = CCommonSocket::GetInstance();//不检查其返回值，常规情况下不会为NULL
    int nIndex = (int)pIndex;
    STransNode *pManageNode = &(pInstance->m_pTransNodeArrayList[nIndex]);
    SUpdatePartNode *pListViewNext,*pListViewPre,*pListTmp;
    SUpdatePartNode *pFreeLink = NULL;//暂存要释放的区块节点
    //1、主流程
    while (pInstance->m_bIsRelease == false && pManageNode->m_uTimeLeft > 0)
    {
        //1.1、检查并处理新连接
        if (pManageNode->m_pNewSocketIdentify != NULL)
        {
            pListViewPre = NULL;
            pListViewNext = pManageNode->m_pUpdatePartList;
            while (pListViewNext != NULL && pListViewNext->m_pSocket != NULL)
            {
                pListViewPre = pListViewNext;
                pListViewNext = pListViewNext->m_pNext;
            }
            if (pListViewNext != NULL)//如果不满足此条件，说明已没有未开始传输的分块
            {
                if ((pListViewNext->m_llEnd - pListViewNext->m_llStart + 1) / pManageNode->m_llBlockSize >= 2)
                {
                    if (TryEnterCS(pInstance->m_pUpdatePartListMemPoolLock,pInstance->m_bIsRelease) == false)
                        break;
                    pListTmp = (SUpdatePartNode *)AllocMemory(pInstance->m_pUpdatePartListMemPool);
                    ExitCS(pInstance->m_pUpdatePartListMemPoolLock);
                    if (pListTmp != NULL)
                    {
                        pListTmp->m_llStart = pListViewNext->m_llStart;
                        pListTmp->m_llEnd = pListViewNext->m_llStart + pManageNode->m_llBlockSize - 1;
                        pListTmp->m_llFinished = 0;
                        pListTmp->m_pNext = pListViewNext;
                        pListTmp->m_pSocket = NULL;
                        pListViewNext->m_llStart = pListTmp->m_llEnd + 1;
                        if (pListViewPre != NULL)
                            pListViewPre->m_pNext = pListTmp;
                        else
                            pManageNode->m_pUpdatePartList = pListTmp;
                        pListViewNext = pListTmp;
                    }
                    else
                    {
                        printf("[E]CFileUpDownServ:Alloc Memory Failed In _UpdateNodeManageThread\n");
                        pListViewNext = NULL;
                    }
                }
                if (pListViewNext != NULL)
                    pListViewNext->m_pSocket = pManageNode->m_pNewSocketIdentify;
            }
            else//没有空闲分块的情况下关闭连接
                pCommonSocket->CloseSocket(pManageNode->m_pNewSocketIdentify);
            pManageNode->m_pNewSocketIdentify = NULL;
        }
        //1.2、检查是否有已经传输完成的块
        pListViewPre = NULL;
        pListTmp = pManageNode->m_pUpdatePartList;
        while (pListTmp != NULL)
        {
            if (pListTmp->m_llFinished == pListTmp->m_llEnd - pListTmp->m_llStart + 1)//已传输完成
            {
                pManageNode->m_llFinishedBlockData += pListTmp->m_llFinished;
                pListViewNext = pListTmp->m_pNext;
                while (pListViewNext != NULL && pListViewNext->m_pSocket != NULL)//查找下一未开始传输的区块
                    pListViewNext = pListViewNext->m_pNext;
                bool bIsUnUsedBlock = false;
                void *pTmpSockIdentify = pListTmp->m_pSocket;
                pListTmp->m_pSocket = NULL;
                if (pListViewNext != NULL)//还有未开始传输的区块
                {
                    if ((pListViewNext->m_llEnd - pListViewNext->m_llStart + 1) / pManageNode->m_llBlockSize >= 2)//检查区块是否可分
                    {
                        pListTmp->m_llStart = pListViewNext->m_llStart;
                        pListTmp->m_llEnd = pListViewNext->m_llStart + pManageNode->m_llBlockSize - 1;
                        pListTmp->m_llFinished = 0;
                        pListViewNext->m_llStart = pListTmp->m_llEnd + 1;
                        pListViewNext = pListTmp;//此语句用于把区块不可分的情况与可分的情况统一
                    }
                    else
                        bIsUnUsedBlock = true;//区块不可分，直接让连接下载最后的区块，让已完成的区块回收（加入回收暂存链表中）
                    pListViewNext->m_pSocket = pTmpSockIdentify;
                }
                else
                {
                    pCommonSocket->CloseSocket(pTmpSockIdentify);//关闭连接
                    bIsUnUsedBlock = true;//已经没有未开始传输的区块，当前已完成区块回收
                }
                if (bIsUnUsedBlock == true)
                {
                    if (pListViewPre != NULL)
                        pListViewPre->m_pNext = pListTmp->m_pNext;
                    else
                        pManageNode->m_pUpdatePartList = pListTmp->m_pNext;
                    pListTmp->m_pNext = pFreeLink;
                    pFreeLink = pListTmp;
                    break;
                }
            }
            pListViewPre = pListTmp;
            pListTmp = pListTmp->m_pNext;
        }
        //1.3、判断是否已完成上传
        if (pManageNode->m_pUpdatePartList == NULL)
            break;
        else
            Sleep_Thread(5);
    }
    //2、回写续传数据，清理节点内资源
    //2.1、关闭连接并阻止新连接的加入，等待所有连接退出后关闭上传文件句柄
    pManageNode->m_uMaxConnectionCount = 0;
    pListViewNext = pManageNode->m_pUpdatePartList;
    while (pListViewNext != NULL)
    {
        if (pListViewNext->m_pSocket != NULL)
            pCommonSocket->CloseSocket(pListViewNext->m_pSocket);//关闭连接
        pListViewNext = pListViewNext->m_pNext;
    }
    while (pManageNode->m_uCurConnectionCount != 0)
        Sleep_Thread(5);
    fclose(pManageNode->m_pTransFile);
    pManageNode->m_pTransFile = NULL;
    //2.2、回写续传数据或删除续传信息文件
    if (pManageNode->m_pUpdatePartList != NULL)
    {
        bool bWriteError = false;
        FILE *pInfoFile = fopen(pManageNode->m_szInfoFilePath.c_str(),"r+b");
        if (pInfoFile != NULL)
        {
            if (FILE_WRITE(&bWriteError,sizeof(bool),1,pInfoFile) != 1)//回写上传结束标记
                bWriteError = true;
            if (bWriteError == true || FILE_WRITE(&(pManageNode->m_llBlockSize),sizeof(long long),1,pInfoFile) != 1)//回写最小分块容量
                bWriteError = true;
            if (bWriteError == true || FILE_WRITE(&(pManageNode->m_llFinishedBlockData),sizeof(long long),1,pInfoFile) != 1)//回写完成传输的分块的总容量
                bWriteError = true;
            if (bWriteError == true || FILE_SEEK(pInfoFile,sizeof(unsigned short),SEEK_CUR) != 0)//先跳过对分块数的写入
                bWriteError = true;
            unsigned short snTotalBlock = 0;
            pListViewNext = pManageNode->m_pUpdatePartList;
            while (bWriteError == false && pListViewNext != NULL)
            {
                if (bWriteError == true || FILE_WRITE(pListViewNext,sizeof(SUpdatePartNode),1,pInfoFile) != 1)//写入分块信息
                    bWriteError = true;
                pListViewNext = pListViewNext->m_pNext;
                ++snTotalBlock;
            }
            if (bWriteError == true || FILE_SEEK(pInfoFile,sizeof(bool) + sizeof(long long) * 2,SEEK_SET) != 0)//定位分块数写入的位置
                bWriteError = true;
            if (bWriteError == true || FILE_WRITE(&snTotalBlock,sizeof(unsigned short),1,pInfoFile) != 1)//写入分块数
                bWriteError = true;
            fclose(pInfoFile);
            if (bWriteError == true)
                printf("[E]CFileUpDownServ:Info File Write Error In _UpdateNodeManageThread,The File May Be Destroyed\n");
        }
        else
            printf("[E]CFileUpDownServ:Info File Can Not Open In _UpdateNodeManageThread\n");
    }
    else
    {
#ifdef WIN32
        string szCommand = "del ";
#else
        string szCommand = "rm -f ";
#endif
        szCommand += pManageNode->m_szInfoFilePath;
        system(szCommand.c_str());
    }
    //2.3、清理分块链表
    if (TryEnterCS(pInstance->m_pUpdatePartListMemPoolLock,pInstance->m_bIsRelease) == true)
    {
        while (pFreeLink != NULL)
        {
            pListTmp = pFreeLink;
            pFreeLink = pFreeLink->m_pNext;
            RecycleMemory(pInstance->m_pUpdatePartListMemPool,pListTmp);
        }
        pListViewNext = pManageNode->m_pUpdatePartList;
        while (pListViewNext != NULL)
        {
            pListTmp = pListViewNext;
            pListViewNext = pListViewNext->m_pNext;
            RecycleMemory(pInstance->m_pUpdatePartListMemPool,pListTmp);
        }
        ExitCS(pInstance->m_pUpdatePartListMemPoolLock);
    }
    pManageNode->m_pUpdatePartList = NULL;
    //3、把节点放回到可用队列中
    EnterCS(pInstance->m_pAdjustNodeLock);
    STransNode *pViewNode = &(pInstance->m_pTransNodeArrayList[pInstance->m_nNextUsingIndex]);
    if (pViewNode == pManageNode)
        pInstance->m_nNextUsingIndex = pManageNode->m_nNextIndex;
    else
    {
        while (&(pInstance->m_pTransNodeArrayList[pViewNode->m_nNextIndex]) != pManageNode)//这里能保证此语句不会出现无效索引值
            pViewNode = &(pInstance->m_pTransNodeArrayList[pViewNode->m_nNextIndex]);
        pViewNode->m_nNextIndex = pManageNode->m_nNextIndex;
    }
    pManageNode->m_nNextIndex = pInstance->m_nNextFreeIndex;
    pInstance->m_nNextFreeIndex = nIndex;
    ExitCS(pInstance->m_pAdjustNodeLock);
    return NULL;
}

/*
 * 【静态】【私有】【线程函数】更新所有传输节点等待超时值的线程函数
 * @param in void *pNull,不会用到的参数
 * @return void *,恒返回NULL
 */
void *CFileUpDownServ::_RefreshWaitTimeoutThread(void *pNull)
{
    CFileUpDownServ *pInstance = CFileUpDownServ::GetInstance();
    while (pInstance->m_bIsRelease == false)
    {
        for (short snLoopVar = 0;snLoopVar < pInstance->m_snTransNodeArrayListCap;++snLoopVar)
        {
            if (pInstance->m_pTransNodeArrayList[snLoopVar].m_uTimeLeft > 0 && pInstance->m_pTransNodeArrayList[snLoopVar].m_uCurConnectionCount == 0)
                --pInstance->m_pTransNodeArrayList[snLoopVar].m_uTimeLeft;
            if (pInstance->m_bIsRelease == true)
                break;
        }
        Sleep_Thread(CFUDS_CHECK_GAP_TIME);
    }
    return NULL;
}

/*
 * 【静态】【私有】【线程函数】管理单个传输服务器端连接的线程函数
 * @param in void *pSocket,线程所管理的服务器端标识
 * @return void *,恒返回NULL
 */
void *CFileUpDownServ::_SocketManageThread(void *pSocket)
{
    CFileUpDownServ *pInstance = CFileUpDownServ::GetInstance();
    CCommonSocket *pCommonSocket = CCommonSocket::GetInstance();
    if (pCommonSocket == NULL)
        return NULL;
    while (pInstance->m_bIsRelease == false)
    {
        pCommonSocket->ListenSocket(pSocket);
        void *pNewSocket = pCommonSocket->AcceptSocket(pSocket);
        if (pNewSocket != NULL)
        {
            if (CreateThreadManager(_TransThread,true,pNewSocket) == NULL)//启动新线程管理该传输连接
                pCommonSocket->CloseSocket(pNewSocket);
        }
        Sleep_Thread(5);
    }
    return NULL;
}

/*
 * 【静态】【私有】【线程函数】管理传输连接的线程
 * @param in void *pSocket,要管理的传输连接
 * @return void *,恒返回NULL
 */
void *CFileUpDownServ::_TransThread(void *pSocket)
{
    char bRecvBuf[CFUDS_RECV_BUF];//接收数据的缓冲区大小
    CCommonSocket *pCommonSocket = CCommonSocket::GetInstance();
    if (pCommonSocket == NULL)
        return NULL;
    CFileUpDownServ *pInstance = CFileUpDownServ::GetInstance();
    Json::Reader oDataRead;
    Json::Value oJObject;//解析和打包json数据用的对象
    Json::FastWriter oJWriter;
    string szReturn;//存放回送客户端的数据
    STransNode *pTargetNode = NULL;//存放文件传输信息的节点
    //0、为能关闭客户端而记录客户端连接标识
    void *ppConnection = NULL;//从内存池分配的用于存放无法在类析构时从阻塞中返回的连接的标识号
    if (TryEnterCS(pInstance->m_pConnectionMemPoolLock,pInstance->m_bIsRelease) == true)
    {
        ppConnection = AllocMemory(pInstance->m_pConnectionMemPool);
        if (ppConnection == NULL)
        {
            pCommonSocket->CloseSocket(pSocket);
            return NULL;
        }
        else
            *(void **)(ppConnection) = pSocket;
        ExitCS(pInstance->m_pConnectionMemPoolLock);
    }
    else
    {
        pCommonSocket->CloseSocket(pSocket);
        return NULL;
    }
    //1、等待客户端发送传输标识
    if (pCommonSocket->ReceiveSocket(pSocket,bRecvBuf,CFUDS_RECV_BUF) <= 0)
    {
        if (TryEnterCS(pInstance->m_pConnectionMemPoolLock,pInstance->m_bIsRelease) == true)
        {
            RecycleMemory(pInstance->m_pConnectionMemPool,ppConnection,pCommonSocket);
            ExitCS(pInstance->m_pConnectionMemPoolLock);
        }
        return NULL;
    }
    int nResultCode = CFUD_TRANS_PASS;
    int nIndex = -1;//传输节点索引
    if ( ! oDataRead.parse(bRecvBuf,oJObject))
        nResultCode = CFUD_TRANS_JSON_ERROR;
    else
    {
        if (oJObject["TID"].isNull() == false) //检查是否存在传输权限码的字段
        {
            int nTransCode = oJObject["TID"].asInt();
            nIndex = nTransCode & 0x0ffff;
            if (nIndex < pInstance->m_snTransNodeArrayListCap && nIndex >= 0)
            {
                if ((nTransCode >> 16) == pInstance->m_pTransNodeArrayList[nIndex].m_snIdentify)
                {
                    pTargetNode = &(pInstance->m_pTransNodeArrayList[nIndex]);
                    //检查是否已达到并发连接上限
                    if (TryEnterCS(pTargetNode->m_pCurConnectionLock,pInstance->m_bIsRelease) == true)
                    {
                        if (pTargetNode->m_uCurConnectionCount < pTargetNode->m_uMaxConnectionCount)
                            ++pTargetNode->m_uCurConnectionCount;
                        else
                            nResultCode = CFUD_TRANS_CONNECTION_FULL;
                        ExitCS(pTargetNode->m_pCurConnectionLock);
                    }
                    else
                        nResultCode = CFUD_TRANS_OBJECT_RELEASE;
                }
            }
            if (pTargetNode == NULL && nResultCode == CFUD_TRANS_PASS)
                nResultCode = CFUD_TRANS_CODE_UNAVAILABLE;
        }
        else
            nResultCode = CFUD_TRANS_NO_TRANS_CODE;
    }
    oJObject.clear();
    oJObject["Command"] = JSON_COMMAND_ID_RESULT;
    oJObject["ResultCode"] = nResultCode;
    szReturn = oJWriter.write(oJObject);
    pCommonSocket->SendSocket(pSocket,szReturn.c_str(),szReturn.length());//由于这里传输的为纯英文和数字，那些字符在这里都使用了单字节存储，所以字符串长度等于实际要发送的字节数
    if (nResultCode != CFUD_TRANS_PASS)
    {
        if (TryEnterCS(pInstance->m_pConnectionMemPoolLock,pInstance->m_bIsRelease) == true)
        {
            RecycleMemory(pInstance->m_pConnectionMemPool,ppConnection,pCommonSocket);
            ExitCS(pInstance->m_pConnectionMemPoolLock);
        }
        return NULL;
    }
    //2、选择要传输的区间（分上传、下载两种情况）
    char bFileBuf[CFUDS_READ_WRITE_BUF];//写入文件的数据的缓冲区大小，当缓冲区满或是连接异常断开时其数据就会写入到文件
    if (pTargetNode->m_bIsUp == true) //客户端上传文件
    {
        //2.A.0、删除前面记录的客户端连接标识
        *(void **)ppConnection = NULL;
        EnterCS(pInstance->m_pConnectionMemPoolLock);
        RecycleMemory(pInstance->m_pConnectionMemPool,ppConnection,NULL);
        ExitCS(pInstance->m_pConnectionMemPoolLock);
        //2.A.1、设置新连接标志
        EnterCS(pTargetNode->m_pCurConnectionLock);
        while (pTargetNode->m_pNewSocketIdentify != NULL)//等待传输节点管理线程完成操作
            Sleep_Thread(5);
        pTargetNode->m_pNewSocketIdentify = pSocket;
        ExitCS(pTargetNode->m_pCurConnectionLock);
        int nReceiveLen = 1;//接收数据的长度
        while (pInstance->m_bIsRelease == false && nReceiveLen > 0)
        {
            //2.A.2、等待并查找上传区间分配
            SUpdatePartNode *pTransRangeNode = NULL;
            bool bHasUnAllocRange = true;//标识是否尚有未分配的区间
            while (pTransRangeNode == NULL && bHasUnAllocRange == true && pInstance->m_bIsRelease == false)
            {
                pTransRangeNode = pTargetNode->m_pUpdatePartList;
                bHasUnAllocRange = false;
                while (pTransRangeNode != NULL)
                {
                    if (pTransRangeNode->m_pSocket != pSocket)
                    {
                        if (pTransRangeNode->m_pSocket == NULL)
                            bHasUnAllocRange = true;
                        pTransRangeNode = pTransRangeNode->m_pNext;
                    }
                    else
                        break;
                }
                Sleep_Thread(5);
            }
            if (pTransRangeNode == NULL && bHasUnAllocRange == false)
                break;
            //2.A.3、根据上传区间信息传输数据（如果没有空余传输区间分配，连接关闭的工作由传输节点管理线程完成）
            if (pTransRangeNode != NULL && pInstance->m_bIsRelease == false && pTransRangeNode->m_llFinished < pTransRangeNode->m_llEnd - pTransRangeNode->m_llStart + 1)
            {
                //2.A.3.1、通知客户端要上传的区间（区间上、下限都为全包含关系）
                oJObject.clear();
                oJObject["Command"] = JSON_COMMAND_SET_TRANS_RANGE;
                long long llRange = pTransRangeNode->m_llStart + pTransRangeNode->m_llFinished;
                oJObject["Start_1"] = (int)(llRange & 0x0ffff);//填入64位数中从低位算起0-15位
                oJObject["Start_2"] = (int)((llRange >> 16) & 0x0ffff);//填入64位数中从低位算起16-31位
                oJObject["Start_3"] = (int)((llRange >> 32) & 0x0ffff);//填入64位数中从低位算起32-47位
                oJObject["Start_4"] = (int)((llRange >> 48) & 0x0ffff);//填入64位数中从低位算起48-63位
                llRange = pTransRangeNode->m_llEnd;
                oJObject["End_1"] = (int)(llRange & 0x0ffff);//填入64位数中从低位算起0-15位
                oJObject["End_2"] = (int)((llRange >> 16) & 0x0ffff);//填入64位数中从低位算起16-31位
                oJObject["End_3"] = (int)((llRange >> 32) & 0x0ffff);//填入64位数中从低位算起32-47位
                oJObject["End_4"] = (int)((llRange >> 48) & 0x0ffff);//填入64位数中从低位算起48-63位
                szReturn = oJWriter.write(oJObject);
                pCommonSocket->SendSocket(pSocket,szReturn.c_str(),szReturn.length());//由于这里传输的为纯英文和数字，那些字符在这里都使用了单字节存储，所以字符串长度等于实际要发送的字节数
                //2.A.3.2、上传
                int nWriteIndex = 0;//文件数据缓存区的写指针
                int nRecvIndex;//接收数据缓存区的读指针
                bool bFinishOneBlock = false;//标识是否已完成一个数据块的传输
                do
                {
                    //2.A.3.2.1 、接收数据
                    nReceiveLen = pCommonSocket->ReceiveSocket(pSocket,bRecvBuf,CFUDS_RECV_BUF);
                    nRecvIndex = 0;
                    //2.A.3.2.2、把数据放入写文件缓存区，并写入文件
                    do
                    {
                        //2.A.3.2.2.1、确定要放入写文件缓存区的数据量
                        if (nRecvIndex < nReceiveLen)
                        {
                            int nCopyLen = nReceiveLen - nRecvIndex;
                            if (nCopyLen > CFUDS_READ_WRITE_BUF - nWriteIndex)
                                nCopyLen = CFUDS_READ_WRITE_BUF - nWriteIndex;
                            if (nCopyLen + nWriteIndex > pTransRangeNode->m_llEnd - pTransRangeNode->m_llStart + 1 - pTransRangeNode->m_llFinished)//收到的数据量大于该区块的需求余量
                            {
                                nCopyLen = pTransRangeNode->m_llEnd - pTransRangeNode->m_llStart + 1 - pTransRangeNode->m_llFinished - nWriteIndex;
                                nReceiveLen = nCopyLen;//丢弃多余的数据
                            }
                            memcpy(bFileBuf + nWriteIndex,bRecvBuf + nRecvIndex,nCopyLen);
                            nRecvIndex += nCopyLen;
                            nWriteIndex += nCopyLen;
                        }
                        //2.A.3.2.2.2、把数据写入文件
                        if (nWriteIndex > 0 && (nReceiveLen <= 0 || nWriteIndex == CFUDS_READ_WRITE_BUF || nWriteIndex + pTransRangeNode->m_llFinished == pTransRangeNode->m_llEnd - pTransRangeNode->m_llStart + 1))
                        {
                            EnterCS(pTargetNode->m_pReadWriteFileLock);
                            if (FILE_SEEK(pTargetNode->m_pTransFile,pTransRangeNode->m_llStart + pTransRangeNode->m_llFinished - 1,SEEK_SET) == 0
                                 && FILE_WRITE(bFileBuf,nWriteIndex,1,pTargetNode->m_pTransFile) == 1)//此语句利用了if内条件从前往后判断的特点编写
                                fflush(pTargetNode->m_pTransFile);
                            else
                                printf("[E]CFileUpDownServ:Write File Error,Match Info File Is %s\n",pTargetNode->m_szInfoFilePath.c_str());
                            if (pTransRangeNode->m_llFinished + nWriteIndex == pTransRangeNode->m_llEnd - pTransRangeNode->m_llStart + 1)
                                bFinishOneBlock = true;
                            pTransRangeNode->m_llFinished += nWriteIndex;
                            nWriteIndex = 0;
                            ExitCS(pTargetNode->m_pReadWriteFileLock);
                        }
                    }while(nRecvIndex < nReceiveLen);
                }while(nReceiveLen > 0 && bFinishOneBlock == false && pInstance->m_bIsRelease == false);
            }
        }
    }
    else //客户端下载文件
    {
        bool bHasError = false;//出错标记
        while (pInstance->m_bIsRelease == false && bHasError == false)
        {
            //2.B.1、要求客户端发送要下载的区间值
            oJObject.clear();
            oJObject["Command"] = JSON_COMMAND_WAIT_TRANS_RANGE;
            szReturn = oJWriter.write(oJObject);
            pCommonSocket->SendSocket(pSocket,szReturn.c_str(),szReturn.length());//由于这里传输的为纯英文和数字，那些字符在这里都使用了单字节存储，所以字符串长度等于实际要发送的字节数
            //2.B.2、等待客户端的下载区间消息
            if (pCommonSocket->ReceiveSocket(pSocket,bRecvBuf,CFUDS_RECV_BUF) <= 0 || oDataRead.parse(bRecvBuf,oJObject) == false)
                bHasError = true;
            else
            {
                //2.B.3、组合传输范围值
                long long llStart,llEnd;
                if (oJObject["Start_1"].isNull() == false && oJObject["Start_2"].isNull() == false &&
                    oJObject["Start_3"].isNull() == false && oJObject["Start_4"].isNull() == false)
                {
                    llStart = oJObject["Start_1"].asInt();
                    llStart += (long long)(oJObject["Start_2"].asInt()) << 16;
                    llStart += (long long)(oJObject["Start_3"].asInt()) << 32;
                    llStart += (long long)(oJObject["Start_4"].asInt()) << 48;
                }
                else
                    bHasError = true;
                if (oJObject["End_1"].isNull() == false && oJObject["End_2"].isNull() == false &&
                    oJObject["End_3"].isNull() == false && oJObject["End_4"].isNull() == false)
                {
                    llEnd = oJObject["End_1"].asInt();
                    llEnd += (long long)(oJObject["End_2"].asInt()) << 16;
                    llEnd += (long long)(oJObject["End_3"].asInt()) << 32;
                    llEnd += (long long)(oJObject["End_4"].asInt()) << 48;
                }
                else
                    bHasError = true;
                //2.B.4、定位并发送客户端请求的数据
                while (pInstance->m_bIsRelease == false && llStart <= llEnd && bHasError == false)
                {
                    int nReadIndex = 0;//文件数据缓存区的读指针
                    int nReadLen = llEnd - llStart + 1;//暂存读取文件的数据量和发送成功的字节数
                    if (nReadLen > CFUDS_READ_WRITE_BUF)
                        nReadLen = CFUDS_READ_WRITE_BUF;
                    if (bHasError == false && nReadLen > 0)
                    {
                        EnterCS(pTargetNode->m_pReadWriteFileLock);
                        if (FILE_SEEK(pTargetNode->m_pTransFile,llStart - 1,SEEK_SET) == 0 && 
                            fread(bFileBuf,nReadLen,1,pTargetNode->m_pTransFile) == 1)//此语句利用了if内条件从前往后判断的特点编写
                        {
                            ExitCS(pTargetNode->m_pReadWriteFileLock);
                            llStart += nReadLen;
                            //2.B.4.x、发送读出的文件数据，直到发送结束
                            while (pInstance->m_bIsRelease == false && bHasError == false && nReadIndex < nReadLen)
                            {
                                int nSendLen = pCommonSocket->SendSocket(pSocket,bFileBuf + nReadIndex,nReadLen - nReadIndex);//通过nReadLen接收实际的发送数据量
                                if (nSendLen >= 0)
                                    nReadIndex += nSendLen;
                                if (nSendLen < 0)
                                    bHasError = true;
                            }
                        }
                        else
                        {
                            ExitCS(pTargetNode->m_pReadWriteFileLock);
                            bHasError = true;
                            printf("[E]CFileUpDownServ:Read File Error\n");
                        }
                    }
                }
            }
        }
        //2.B.5、释放节点
        if (ppConnection != NULL && TryEnterCS(pInstance->m_pConnectionMemPoolLock,pInstance->m_bIsRelease) == true)
        {
            RecycleMemory(pInstance->m_pConnectionMemPool,ppConnection,pCommonSocket);
            ExitCS(pInstance->m_pConnectionMemPoolLock);
        }
    }
    //3、连接数减1
    EnterCS(pTargetNode->m_pCurConnectionLock);
    --pTargetNode->m_uCurConnectionCount;
    ExitCS(pTargetNode->m_pCurConnectionLock);
    return NULL;
}

/*
 * 【静态】【私有】【线程函数】管理单个下载节点的线程函数
 * @param in void *pIndex,要管理的节点的索引
 * @return void *,恒返回NULL
 */
void *CFileUpDownServ::_DownloadNodeManageThread(void *pIndex)
{
    CFileUpDownServ *pInstance = CFileUpDownServ::GetInstance();
    int nIndex = (int)pIndex;
    STransNode *pManageNode = &(pInstance->m_pTransNodeArrayList[nIndex]);
    //等待节点回收信号
    while (pInstance->m_bIsRelease == false && pManageNode->m_uTimeLeft > 0)
        Sleep_Thread(5);
    pManageNode->m_uMaxConnectionCount = 0;
    while (pManageNode->m_uCurConnectionCount != 0)
        Sleep_Thread(5);
    fclose(pManageNode->m_pTransFile);
    pManageNode->m_pTransFile = NULL;
    //回收传输信息节点
    EnterCS(pInstance->m_pAdjustNodeLock);
    STransNode *pViewNode = &(pInstance->m_pTransNodeArrayList[pInstance->m_nNextUsingIndex]);
    if (pViewNode == pManageNode)
        pInstance->m_nNextUsingIndex = pManageNode->m_nNextIndex;
    else
    {
        while (&(pInstance->m_pTransNodeArrayList[pViewNode->m_nNextIndex]) != pManageNode)//这里能保证此语句不会出现无效索引值
            pViewNode = &(pInstance->m_pTransNodeArrayList[pViewNode->m_nNextIndex]);
        pViewNode->m_nNextIndex = pManageNode->m_nNextIndex;
    }
    pManageNode->m_nNextIndex = pInstance->m_nNextFreeIndex;
    pInstance->m_nNextFreeIndex = nIndex;
    ExitCS(pInstance->m_pAdjustNodeLock);
    return NULL;
}
