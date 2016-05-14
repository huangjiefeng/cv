#include "stdafx.h"
#include "QtGlobalServiceSetting.h"
#include "ui_globalServiceSetting.h"
#include <util/SafeDelete.h>
#include <qtutil/SafeConnect.h>
#include "QtWengoPhone.h"
#include <control/profile/CUserProfile.h>
#include <control/profile/CUserProfileHandler.h>
#include <control/CWengoPhone.h>
#include <model/config/ConfigManager.h>
#include <model/config/Config.h>
#include <model/config/NetworkSetting.h>
#include <QtGui/QtGui>


QtGlobalServiceSetting::QtGlobalServiceSetting(CWengoPhone * cWengoPhone)
{
	_ui = new Ui::GlobalServiceDialog;
	_ui->setupUi(this);

	SAFE_CONNECT(_ui->saveButton, SIGNAL(clicked()), SLOT(saveConfig()));
	readConfig();

	QtGlobalServiceSetting::_qtWengoPhone = cWengoPhone->_qtWengoPhone;//Add By LZY 2010-09-26
}

QtGlobalServiceSetting::~QtGlobalServiceSetting(void)
{
	OWSAFE_DELETE(_ui);
}

void QtGlobalServiceSetting::saveConfig()
{

	//-------Modify By LZY 2010-09-26-------
	string Addr=_ui->globalServiceV4->text().toStdString();
	QtGlobalServiceSetting::_qtWengoPhone->SaveGlobalSetting(Addr,Addr);
	//-------End of Modify By LZY-----------
//	config.set(Config:: SIP_GLOBAL_SERVICE_V6, _ui-> globalServiceV6->text());

	//QMessageBox::warning(this, tr("@product@"),
	//	tr("\351\207\215\345\220\257\347\250\213\345\272\217\351\205\215\347\275\256\346\211\215\347\224\237\346\225\210!"), QMessageBox::Ok);
	accept();
}

void QtGlobalServiceSetting::readConfig()
{
	Config & config = ConfigManager::getInstance().getCurrentConfig();
	_ui->globalServiceV4->setText(QString(config.getGlobalService().c_str()));
	_ui->globalServiceV6->setText(QString(config.getGlobalService().c_str()));

}