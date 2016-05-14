#ifndef __FTPFILETRANS__
#define __FTPFILETRANS__

#include<windows.h>
#include<string>
using namespace std;

struct FtpFile {
	const char *filename;
	FILE *stream;
};

class FtpOperator
{
public:
	static FtpOperator *getInstance()
	{
		if(NULL == FtpOperator::m_pInstance)
			FtpOperator::m_pInstance = new FtpOperator();
		return FtpOperator::m_pInstance;
	}

public:
	virtual ~FtpOperator();

	//设置FTP地址及相应的账号信息，并初始化FTP连接
	HRESULT SetFtpConnectInfo(const string &url, const string &name, const string &pwd);
	//上传文件
	HRESULT FtpFileUpload(const string &remotepath, const string &localpath, const long &timeout, const long &tries);
	//下载文件，targetpath为服务器相对路径，savepath为本地存储路径
	HRESULT FtpFileDownload(const string &targetpath, const string &savepath);

private:
	FtpOperator();
	class CGarbo			//它的唯一工作就是在析构函数中删除CSingleton的实例
	{
	public:
		~CGarbo()
		{
			if (FtpOperator::m_pInstance)
				delete FtpOperator::m_pInstance;
		}
	}; 
	static CGarbo Garbo;	//定义一个静态成员，在程序结束时，系统会调用它的析构函数
private:
	struct dlParam
	{
		string request;
		string userinfo;
		string savepath;
	};

	struct ulParam
	{
		string remotepath;
		string localpath;
		string userinfo;
		long timeout;
		long tries;
	};

	static unsigned __stdcall download_thread(PVOID pParam);
	static unsigned __stdcall upload_thread(PVOID pParam);
	//下载文件的回调函数
	static size_t my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream);

	static size_t getcontentlengthfunc(void *ptr, size_t size, size_t nmemb, void *stream);
	static size_t discardfunc(void *ptr, size_t size, size_t nmemb, void *stream);
	static size_t readfunc(void *ptr, size_t size, size_t nmemb, void *stream);

private:
	static FtpOperator *m_pInstance;
	string m_ftpurl;
	string m_username;
	string m_userpwd;
};

string &string_replace(string &str, const string &old_value, const string &new_value);
#endif