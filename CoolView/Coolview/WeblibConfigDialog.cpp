#include "weblibconfigdialog.h"
#include "util\ini\Ini.h"
#include "util\ini\TxConfig.h"

#include <QString>
#include <QIntValidator>

#include <string>


// Definition of help method

const int BUF_SIZE = 256;

static std::string GetAbsolutePath( const std::string& strRelative )
{
	char szAppPath[200] = {0};
	char szAppDir[200] = {0};

	if(!::GetModuleFileName(0, szAppPath, sizeof(szAppPath) - 1))
	{
		printf("failed to get module file name!\n");
		return "";
	}

	// Extract directory
	strncpy(szAppDir, szAppPath, strrchr(szAppPath, '\\') - szAppPath);
	szAppDir[strlen(szAppDir)] = '\0';

	std::string strDest;
	if (strRelative.empty())
	{
		strDest = szAppDir;
	}
	// Absolute path input
	else if (strRelative.size() > 2 && strRelative[1] == ':')
	{
		strDest = strRelative;
	}
	// Relative path
	else
	{
		if (strRelative[0] == '\\')
		{
			strDest = std::string(szAppDir) + strRelative;
		}
		else
		{
			strDest = std::string(szAppDir) + "\\" + strRelative;
		}
	}

	return strDest;

}


// Definition of members

WeblibConfigDialog::WeblibConfigDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.resetButton, SIGNAL(clicked()), this, SLOT(loadConfig()));
	connect(ui.saveButton, SIGNAL(clicked()), this, SLOT(saveConfig()));
	connect(ui.cancelButton, SIGNAL(clicked()), this, SLOT(close()));

	ui.groupIdEdit->setValidator(new QIntValidator(0, 100000000, this));
	ui.scanIntervalLineEdit->setValidator(new QIntValidator(0, 100000000, this));

	loadConfig();
}

WeblibConfigDialog::~WeblibConfigDialog()
{

}

void WeblibConfigDialog::saveConfig()
{
	CIni * ini = new CIni();
	ini->SetPathName(GetAbsolutePath(CTxConfig::getInstance().GetUploadIni()).c_str());

	QByteArray bytes;

	bytes = ui.userNameLineEdit->text().toLocal8Bit();
	ini->WriteString(CTxConfig::getInstance().GetUploadIniSec(), "Account", bytes.data());

	bytes = ui.passwordLineEdit->text().toLocal8Bit();
	ini->WriteString(CTxConfig::getInstance().GetUploadIniSec(), "Password", bytes.data());

	bytes = ui.serverLineEdit->text().toLocal8Bit();
	ini->WriteString(CTxConfig::getInstance().GetUploadIniSec(), "WebLibUrl", bytes.data());

	bytes = ui.groupIdEdit->text().toLocal8Bit();
	ini->WriteString(CTxConfig::getInstance().GetUploadIniSec(), "GroupId", bytes.data());

	// convert second to ms
	bytes = (ui.scanIntervalLineEdit->text() + "000").toLocal8Bit();
	ini->WriteString(CTxConfig::getInstance().GetUploadIniSec(), "Interval", bytes.data());

	// 将上传位置同步到Weblib上传客户端的配置文件
	std::string uploadPath = GetAbsolutePath(CTxConfig::getInstance().GetUploadPath());
	ini->WriteString(CTxConfig::getInstance().GetUploadIniSec(), CTxConfig::getInstance().GetUploadIniKey(), uploadPath.c_str());

	ini->WriteString(CTxConfig::getInstance().GetUploadIniSec(), "DirTimeOut", "120000");

	delete ini;

	// close dialog
	close();
}

void WeblibConfigDialog::loadConfig()
{
	CIni * ini = new CIni();
	ini->SetPathName(GetAbsolutePath(CTxConfig::getInstance().GetUploadIni()).c_str());

	char buf[BUF_SIZE] = {0};

	ini->GetString(CTxConfig::getInstance().GetUploadIniSec(), "Account", buf, BUF_SIZE, "");
	ui.userNameLineEdit->setText(buf);

	ini->GetString(CTxConfig::getInstance().GetUploadIniSec(), "Password", buf, BUF_SIZE, "");
	ui.passwordLineEdit->setText(buf);

	ini->GetString(CTxConfig::getInstance().GetUploadIniSec(), "WebLibUrl", buf, BUF_SIZE, "http://");
	ui.serverLineEdit->setText(buf);

	ini->GetString(CTxConfig::getInstance().GetUploadIniSec(), "GroupId", buf, BUF_SIZE, "");
	ui.groupIdEdit->setText(buf);

	int interval = ini->GetInt(CTxConfig::getInstance().GetUploadIniSec(), "Interval", 60000);
	// convert ms to second
	ui.scanIntervalLineEdit->setText(QString::number(interval / 1000));

	delete ini;
}
