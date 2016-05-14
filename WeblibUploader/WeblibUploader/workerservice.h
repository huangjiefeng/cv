#ifndef _WORKERSERVICE_H_
#define _WORKERSERVICE_H_

#include <QObject>
#include "commonstruts.h"
#include "dllexports.h"

class FileUploader;

class WEBLIB_DLLSPEC WorkerService : public QObject
{
	Q_OBJECT

public:
	static WorkerService &getInstance()
	{
		static WorkerService g_instance;
		return g_instance;
	}

	~WorkerService();
public slots:
	void slotStartUpload();
	void slotStopUpload();

	WeblibSettingInfo readOrignalSettings();					//获取配置文件中的参数值
	void slotSetWeblibSetting(const WeblibSettingInfo& info);	//设置上传参数，但不会写入配置文件

	//以下两个方法需要返回值
	WeblibSettingInfo getWeblibSetting();	//获取当前实时设置的参数值
	bool saveSettings();					//执行配置文件写入操作
private slots:
	void loginFailed(const QString &info);

signals:
	void signalItemStateChanged(ItemOperate oper, const UploadItem &item);
	void signalShowErrorInfo(const QString &info);
private:
	WorkerService();
	WorkerService(WorkerService const &param);
	WorkerService &operator = (WorkerService const &param);

private:
	FileUploader *m_uploader;
};

#endif