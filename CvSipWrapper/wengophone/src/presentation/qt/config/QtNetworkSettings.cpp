#include "stdafx.h"
#include ".\QtNetworkSettings.h"
#include "ui_NetworkSettings.h"
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

QtNetworkSettings::QtNetworkSettings(void)
{
	_ui = new Ui::NetWorkSetting;
	_ui->setupUi(this);

	SAFE_CONNECT(_ui->saveButton, SIGNAL(clicked()), SLOT(saveConfig()));
	SAFE_CONNECT(_ui->addrTypeComboBox, SIGNAL(currentIndexChanged(int)), SLOT(setIPList(int)));
	_ui->isMulticastComboBox->hide();
	_ui->label_3->hide();

	readConfig();
}

QtNetworkSettings::~QtNetworkSettings(void)
{
	OWSAFE_DELETE(_ui);
}

void QtNetworkSettings::saveConfig()
{
	int ipVersion;
	bool isMulticast;
	Config & config = ConfigManager::getInstance().getCurrentConfig();
	if (_ui->addrTypeComboBox->currentIndex()==0)
		ipVersion = 2;//IPv4
	else
		ipVersion = 23;//IPv6
	config.set(Config::IP_VERSION, ipVersion);
	
	if(_ui->isMulticastComboBox->currentIndex()==0)
		isMulticast = true;
	else
		isMulticast = false;
	config.set(Config::IS_MULTICAST, isMulticast);
	config.set(Config::IP_ADDRESS, std::string(_ui->IPComboBox->currentText().toLocal8Bit()));
	QMessageBox::warning(this, tr("@product@"),
		tr("\351\207\215\345\220\257\347\250\213\345\272\217\351\205\215\347\275\256\346\211\215\347\224\237\346\225\210!"), QMessageBox::Ok);
	accept();
}

void QtNetworkSettings::readConfig()
{
	Config & config = ConfigManager::getInstance().getCurrentConfig();
	int ipVersion = config.getIPVersion();
	if(ipVersion==2)
		_ui->addrTypeComboBox->setCurrentIndex(0);
	else
		_ui->addrTypeComboBox->setCurrentIndex(1);

	bool isMulticast = config.getIsMulticast();
	if(isMulticast)
		_ui->isMulticastComboBox->setCurrentIndex(0);
	else
		_ui->isMulticastComboBox->setCurrentIndex(1);

	NetworkSetting setting;
	_ui->IPComboBox->clear();
	_ui->IPComboBox->addItems(setting.getIPList(ipVersion));
	//_ui->IPComboBox->setCurrentIndex(_ui->IPComboBox->findText(QString::fromLocal8Bit(config.getIPAddress().c_str())));
	_ui->IPComboBox->setCurrentIndex(0);
}

void QtNetworkSettings::setIPList(int index)
{
	NetworkSetting setting;
	int ipVersion;
	if (index==0)
		ipVersion = 2;//IPv4
	else
		ipVersion = 23;//IPv6
	_ui->IPComboBox->clear();
	_ui->IPComboBox->addItems(setting.getIPList(ipVersion));
}
