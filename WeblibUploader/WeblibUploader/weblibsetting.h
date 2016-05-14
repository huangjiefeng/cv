#ifndef _WEBLIBSETTING_H_
#define _WEBLIBSETTING_H_

#include <QApplication>
#include <QDataStream>
#include <QDir>
#include <QFile>
#include <commonstruts.h>

#define		UPLOADER_FILE_DIR		"cfg"
#define		UPLOADER_FILE_NAME		"WeblibUploader.setting"
#define		IS_SETTING_READY		WeblibSetting::getInstance().isSettingInitialized()
#define		RELATIVE_FILE_PATH		WeblibSetting::getInstance().getRelativeFilePath()
#define		SCAN_DIR_LEVEL			WeblibSetting::getInstance().getScanDirLevel()
#define 	FILE_SCAN_INTERVAL		WeblibSetting::getInstance().getFileScanInterval()
#define		CREATE_DIR_TIMEOUT		WeblibSetting::getInstance().getCreateDirTimeout()
#define		UPLOAD_RESPONSE_TIMEOUT	WeblibSetting::getInstance().getUploadResponseTimeout()
#define		WEBLIB_GROUPNAME		WeblibSetting::getInstance().getWeblibGroupName()
#define		WEBLIB_GROUPID			WeblibSetting::getInstance().getWeblibGroupId()
#define		WEBLIB_ACCOUNT			WeblibSetting::getInstance().getWeblibAccount()
#define		WEBLIB_PASSWORD			WeblibSetting::getInstance().getWeblibPassword()
#define		WEBLIB_TMP_FILENAME		WeblibSetting::getInstance().getWeblibTmpFileName()
#define		WEBLIB_BASEURL			WeblibSetting::getInstance().getWeblibBaseUrl()
#define		REQUEST_PER_MNANAGER	WeblibSetting::getInstance().getRequestPerManager()
#define		UPLOAD_RETRY_TIMES		WeblibSetting::getInstance().getUploadRetryTimes()
#define		UPLOAD_THREAD_LIMIT		WeblibSetting::getInstance().getUploadThreadLimit()
#define		MAX_CONNECTION_MANAGER	WeblibSetting::getInstance().getMaxConnectionManager()
#define		UPLOAD_BLOCK_SIZE		WeblibSetting::getInstance().getUploadBlockSize()
#define		UPLOAD_SPEED_LIMIT		WeblibSetting::getInstance().getUploadSpeedLimit()
#define		WEBLIB_ISCREATEUSERDIR	WeblibSetting::getInstance().getCreateUserDir()

class WeblibSetting
{
public:
	static WeblibSetting &getInstance()
	{
		static WeblibSetting instance;
		return instance;
	}

	const QString &getRelativeFilePath()
	{
		return m_relativeFilePath;
	}
	void setRelativeFilePath(const QString &str)
	{
		m_relativeFilePath = str;
	}
	const int &getScanDirLevel()
	{
		return m_scanDirLevel;
	}
	void setScanDirLevel(const int &val)
	{
		m_scanDirLevel = val;
	}
	const int &getFileScanInterval()
	{
		return m_fileScanInterval;
	}
	void setFileScanInterval(const int &val)
	{
		m_fileScanInterval = val;
	}
	const int &getCreateDirTimeout()
	{
		return m_createDirTimeout;
	}
	void setCreateDirTimeout(const int &val)
	{
		m_createDirTimeout = val;
	}
	const int &getUploadResponseTimeout()
	{
		return m_uploadResponseTimeout;
	}
	void setUploadResponseTimeout(const int &val)
	{
		m_uploadResponseTimeout = val;
	}
	const QString &getWeblibGroupName()
	{
		return m_weblibGroupName;
	}
	void setWeblibGroupName(const QString &str)
	{
		m_weblibGroupName = str;
	}
	const int &getWeblibGroupId()
	{
		return m_weblibGroupId;
	}
	void setWelibGroupId(const int &val)
	{
		m_weblibGroupId = val;
	}
	const QString &getWeblibAccount()
	{
		return m_weblibAccount;
	}
	void setWeblibAccount(const QString &str)
	{
		m_weblibAccount = str;
	}
	const QString &getWeblibPassword()
	{
		return m_weblibPassword;
	}
	void setWeblibPassword(const QString &str)
	{
		m_weblibPassword = str;
	}
	const QString &getWeblibTmpFileName()
	{
		return m_weblibTmpFilename;
	}
	void setWelibTmpFileName(const QString &str)
	{
		m_weblibTmpFilename = str;
	}
	const QString &getWeblibBaseUrl()
	{
		return m_weblibBaseurl;
	}
	void setWelibBaseUrl(const QString &str)
	{
		m_weblibBaseurl = str;
	}
	const int &getRequestPerManager()
	{
		return m_requestPerManager;
	}
	void setRequestPerManager(const int &val)
	{
		m_requestPerManager = val;
	}
	const int &getUploadRetryTimes()
	{
		return m_uploadRetryTimes;
	}
	void setUploadRetryTimes(const int &val)
	{
		m_uploadRetryTimes = val;
	}
	const int &getUploadThreadLimit()
	{
		return m_uploadThreadLimit;
	}
	void setUploadThreadLimit(const int &val)
	{
		m_uploadThreadLimit = val;
	}
	const int &getMaxConnectionManager()
	{
		return m_maxConnectionManager;
	}
	void setMaxConnectionManager(const int &val)
	{
		m_maxConnectionManager = val;
	}
	const qint64 &getUploadBlockSize()
	{
		return m_uploadBlockSize;
	}
	void setUploadBlockSize(const qint64 &val)
	{
		m_uploadBlockSize = val;
	}
	const qint64 &getUploadSpeedLimit()
	{
		return m_uploadSpeedLimit;
	}
	void setUploadSpeedLimit(const qint64 &val)
	{
		m_uploadSpeedLimit = val;
	}
	bool getCreateUserDir()
	{
		return m_createUserDir;
	}
	void setCreateUserDir(bool createdir)
	{
		m_createUserDir = createdir;
	}
	bool isSettingInitialized()
	{
		return m_initialized;
	}

	friend QDataStream &operator>>(QDataStream &in, WeblibSetting &obj)
	{
		in >> obj.m_relativeFilePath >> obj.m_scanDirLevel >> obj.m_fileScanInterval
			>> obj.m_createDirTimeout >> obj.m_uploadResponseTimeout >> obj.m_weblibGroupId
			>> obj.m_weblibGroupName >> obj.m_weblibAccount >> obj.m_weblibPassword
			>> obj.m_weblibTmpFilename >> obj.m_weblibBaseurl >> obj.m_requestPerManager
			>> obj.m_uploadRetryTimes >> obj.m_uploadThreadLimit >> obj.m_maxConnectionManager
			>> obj.m_uploadBlockSize >> obj.m_uploadSpeedLimit >> obj.m_createUserDir;
		return in;
	}
	friend QDataStream &operator<<(QDataStream &out, WeblibSetting &obj)
	{
		out << obj.m_relativeFilePath << obj.m_scanDirLevel << obj.m_fileScanInterval
			<< obj.m_createDirTimeout << obj.m_uploadResponseTimeout << obj.m_weblibGroupId
			<< obj.m_weblibGroupName << obj.m_weblibAccount << obj.m_weblibPassword
			<< obj.m_weblibTmpFilename << obj.m_weblibBaseurl << obj.m_requestPerManager
			<< obj.m_uploadRetryTimes << obj.m_uploadThreadLimit << obj.m_maxConnectionManager
			<< obj.m_uploadBlockSize << obj.m_uploadSpeedLimit << obj.m_createUserDir;
		return out;
	}

	void loadConfigFile()
	{
		QString localdir = QApplication::applicationDirPath() + "/" + UPLOADER_FILE_DIR;
		QDir cfgDir(localdir);
		if (!cfgDir.exists()) {
			return;
		}
		QString settingfile = localdir + "/" + UPLOADER_FILE_NAME;
		QFile fileinfo(settingfile);
		if(!fileinfo.exists())
			return;
		if(!fileinfo.open(QIODevice::ReadOnly))
			return;
		QDataStream in(&fileinfo);
		in >> *this;
		fileinfo.close();
		m_initialized = true;
	}

	void readConfigFile(WeblibSettingInfo &info)
	{
		WeblibSetting tmp;
	
		QString localdir = QApplication::applicationDirPath() + "/" + UPLOADER_FILE_DIR;
		QDir cfgDir(localdir);
		if (!cfgDir.exists()) {
			return;
		}
		QString settingfile = localdir + "/" + UPLOADER_FILE_NAME;
		QFile fileinfo(settingfile);
		if(!fileinfo.exists())
			return;
		if(!fileinfo.open(QIODevice::ReadOnly))
			return;
		QDataStream in(&fileinfo);
		in >> tmp;
		fileinfo.close();

		info.account = tmp.getWeblibAccount();
		info.connecttimeout = tmp.getUploadResponseTimeout();
		info.filepath = tmp.getRelativeFilePath();
		info.groupid = tmp.getWeblibGroupId();
		info.groupname = tmp.getWeblibGroupName();
		info.iscreateudir = tmp.getCreateUserDir();
		info.password = tmp.getWeblibPassword();
		info.scaninterval = tmp.getFileScanInterval();
		info.scanlevel = tmp.getScanDirLevel();
		info.threads = tmp.getUploadThreadLimit();
		info.uploadspeed = tmp.getUploadSpeedLimit();
		info.url = tmp.getWeblibBaseUrl();
	}

	bool saveConfigFile()
	{
		QString localdir = QApplication::applicationDirPath() + "/" + UPLOADER_FILE_DIR;
		QDir cfgDir(localdir);
		if (!cfgDir.exists() && !cfgDir.mkpath(localdir)) {
			return false;
		}
		QString settingfile = localdir + "/" + UPLOADER_FILE_NAME;
		QFile fileinfo(settingfile);
		if(!fileinfo.open(QIODevice::WriteOnly))
		{
			return false;
		}
		QDataStream out(&fileinfo);
		out << WeblibSetting::getInstance();
		fileinfo.close();
		m_initialized = true;
		return true;
	}

private:
	WeblibSetting(WeblibSetting const &param);
	WeblibSetting &operator = (WeblibSetting const &param);

	WeblibSetting()
	{
		m_initialized = false;
		m_relativeFilePath = "/txcache/upload";
		m_scanDirLevel = 0;
		m_fileScanInterval = 120000;
		m_createDirTimeout = 120000;
		m_uploadResponseTimeout = 120000;
		m_weblibGroupId = -1;
		m_weblibGroupName = "";
		m_weblibAccount = "";
		m_weblibPassword = "";
		m_weblibTmpFilename = "_weblib_uploaded";
		m_weblibBaseurl = "";
		m_requestPerManager = 5;
		m_uploadRetryTimes = 2;
		m_uploadThreadLimit = 3;
		m_maxConnectionManager = 8;
		m_uploadBlockSize = 16384;
		m_uploadSpeedLimit = 5120000;
		m_createUserDir = false;
	}

private:
	bool m_initialized;

	QString m_relativeFilePath;
	int m_scanDirLevel;
	int m_fileScanInterval;
	int m_createDirTimeout;
	int m_uploadResponseTimeout;
	int m_weblibGroupId;
	QString m_weblibGroupName;
	QString m_weblibAccount;
	QString m_weblibPassword;

	QString m_weblibTmpFilename;
	QString m_weblibBaseurl;
	int m_requestPerManager;
	int m_uploadRetryTimes;
	int m_uploadThreadLimit;
	int m_maxConnectionManager;
	qint64 m_uploadBlockSize;
	qint64 m_uploadSpeedLimit;
	bool m_createUserDir;
};

#endif