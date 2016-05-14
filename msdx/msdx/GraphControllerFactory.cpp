#include "IGraphController.h"
#include "SendGraphController.h"
#include "SmallGraphController.h"
#include "RecvGraphController.h"
#include "TestGraphController.h"
#include "FilePlayGraphController.h"

using namespace msdx;

GraphControllerFactory &GraphControllerFactory::GetInstance()
{
    //使用C++11静态初始化特性保证单例初始化过程的线程安全
    static GraphControllerFactory instance;
    return instance;
}

GraphControllerFactory::GraphControllerFactory(void)
{
}


GraphControllerFactory::~GraphControllerFactory(void)
{
}

ISendGraphController * GraphControllerFactory::CreateSendGraphController()
{
    SendGraphController *controller = new SendGraphController; 
    return static_cast<ISendGraphController*>(controller);
}

ISendGraphController * GraphControllerFactory::CreateTestSendGraphController()//hjf 2015.12.15
{
	SendGraphController *controller = new TestSendGraphController; 
	return static_cast<ISendGraphController*>(controller);
}

ISmallGraphController * GraphControllerFactory::CreateSmallGraphController()
{
    SmallGraphController *controller = new SmallGraphController;
    return static_cast<ISmallGraphController*>(controller);
}

IRecvGraphController * GraphControllerFactory::CreateRecvGraphController()
{
    RecvGraphController *controller = new RecvGraphController;
    return static_cast<IRecvGraphController*>(controller);
}

ITestGraphController * GraphControllerFactory::CreateTestGraphController()
{
    TestGraphController * controller = new TestGraphController;
    return static_cast<ITestGraphController*>(controller);
}

IFilePlayGraphController * GraphControllerFactory::CreateFilePlayGraphController()
{
    FilePlayGraphController * controller = new FilePlayGraphController;
    return static_cast<IFilePlayGraphController*>(controller);
}

