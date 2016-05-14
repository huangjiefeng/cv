#include "workerservice.h"
#include "weblibsetting.h"
#include "connectionmanager.h"
#include "fileuploader.h"
#include "log.h"

WorkerService::WorkerService()
{
	WeblibSetting::getInstance().loadConfigFile();

	//重定向流，将DCritical信息记入日志
	qInstallMessageHandler(LogClass::customMessageHandler);

	m_uploader = new FileUploader();
	connect(m_uploader, SIGNAL(error(const QString &)), this, SIGNAL(signalShowErrorInfo(const QString &)));
	connect(m_uploader, SIGNAL(signalItemStateChanged(ItemOperate, const UploadItem &)), this, SIGNAL(signalItemStateChanged(ItemOperate, const UploadItem &)));
	
	ConnectionManager *manager = &ConnectionManager::getInstance();
	connect(manager, SIGNAL(loginFailed(const QString &)), this, SLOT(loginFailed(const QString &)));
	manager->weblibLogin();
}

WorkerService::~WorkerService()
{
	disconnect(&ConnectionManager::getInstance(), SIGNAL(loginFailed(const QString &)), this, SLOT(loginFailed(const QString &)));
	disconnect(&ConnectionManager::getInstance(), SIGNAL(loginSuccess()), m_uploader, SLOT(startAll()));
	if(m_uploader != NULL)
	{
		disconnect(m_uploader, SIGNAL(signalItemStateChanged(ItemOperate, const UploadItem &)), this, SIGNAL(signalItemStateChanged(ItemOperate, const UploadItem &)));
		disconnect(m_uploader, SIGNAL(error(const QString &)), this, SIGNAL(signalShowErrorInfo(const QString &)));
		m_uploader->stopAll();
		m_uploader->deleteLater();
		delete m_uploader;
		m_uploader = NULL;
	}
}

void WorkerService::slotSetWeblibSetting(const WeblibSettingInfo& info)
{
	WeblibSetting *obj = &WeblibSetting::getInstance();
	obj->setWelibBaseUrl(info.url);
	obj->setWeblibAccount(info.account);
	obj->setWeblibPassword(info.password);
	obj->setRelativeFilePath(info.filepath);
	obj->setScanDirLevel(info.scanlevel);
	obj->setWeblibGroupName(info.groupname);
	obj->setWelibGroupId(info.groupid);
	obj->setUploadResponseTimeout(info.connecttimeout);
	obj->setCreateDirTimeout(info.connecttimeout);
	obj->setFileScanInterval(info.scaninterval);
	obj->setUploadSpeedLimit(info.uploadspeed);
	obj->setUploadThreadLimit(info.threads);
	obj->setCreateUserDir(info.iscreateudir);
}

void WorkerService::slotStartUpload()
{
	ConnectionManager *manager = &ConnectionManager::getInstance();
	if(manager->getLoginStatus() == true)
		m_uploader->startAll();
	else
		connect(manager, SIGNAL(loginSuccess()), m_uploader, SLOT(startAll()));
}

void WorkerService::slotStopUpload()
{
	m_uploader->stopAll();
	disconnect(&ConnectionManager::getInstance(), SIGNAL(loginSuccess()), m_uploader, SLOT(startAll()));
}

WeblibSettingInfo WorkerService::getWeblibSetting()
{
	WeblibSettingInfo info;
	info.url = WEBLIB_BASEURL;
	info.account = WEBLIB_ACCOUNT;
	info.connecttimeout = UPLOAD_RESPONSE_TIMEOUT;
	info.filepath = RELATIVE_FILE_PATH;
	info.scanlevel = SCAN_DIR_LEVEL;
	info.groupname = WEBLIB_GROUPNAME;
	info.groupid = WEBLIB_GROUPID;
	info.password = WEBLIB_PASSWORD;
	info.scaninterval = FILE_SCAN_INTERVAL;
	info.threads = UPLOAD_THREAD_LIMIT;
	info.uploadspeed = UPLOAD_SPEED_LIMIT;
	info.iscreateudir = WEBLIB_ISCREATEUSERDIR;
	return info;
}

void WorkerService::loginFailed(const QString &info)
{
	emit signalShowErrorInfo(info);

	//重试
	QTimer::singleShot(60000, &ConnectionManager::getInstance(), SLOT(weblibLogin()));
}

bool WorkerService::saveSettings()
{
	if(WeblibSetting::getInstance().saveConfigFile())
	{
		ConnectionManager::getInstance().weblibLogin();
		return true;
	}
	else
		return false;
}

WeblibSettingInfo WorkerService::readOrignalSettings()
{
	WeblibSettingInfo info;
	WeblibSetting::getInstance().readConfigFile(info);
	return info;
}