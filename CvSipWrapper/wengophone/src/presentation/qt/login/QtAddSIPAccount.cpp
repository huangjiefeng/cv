#include "stdafx.h"

/*
 * WengoPhone, a voice over Internet phone
 * Copyright (C) 2004-2007  Wengo
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "QtAddSIPAccount.h"

#include "QtAddAccountUtils.h"
#include "QtLoginDialog.h"

#include "ui_AddSIPAccount.h"

#include <presentation/qt/QtWengoPhone.h>

#include <control/profile/CUserProfileHandler.h>

#include <control/CWengoPhone.h>//Add By LZY 2010-09-21

#include <model/account/wengo/WengoAccount.h>
#include <model/config/ConfigManager.h>
#include <model/config/Config.h>
#include <model/profile/UserProfile.h>
#include <model/profile/UserProfileFileStorage.h>
#include <model/profile/UserProfileHandler.h>
#include <model/webservices/url/WsUrl.h>
#include <model/config/NetworkSetting.h>

#include <qtutil/LanguageChangeEventFilter.h>
#include <qtutil/WidgetBackgroundImage.h>
#include <qtutil/SafeConnect.h>

#include <util/Logger.h>
#include <util/SafeDelete.h>

#include <webcam/WebcamDriver.h>
#include <sound/AudioDeviceManager.h>
#include <sound/Sound.h>
#include <sound/VolumeControl.h>
#include <sound/AudioDevice.h>
#include <qtutil/StringListConvert.h>

#include <QtGui/QtGui>

static AudioDevice getAudioDeviceFromComboBox(QComboBox* comboBox, const std::list<AudioDevice> deviceList) {
	std::string concatString = qPrintable(comboBox->itemData(comboBox->currentIndex()).toString());
	for (std::list<AudioDevice>::const_iterator it = deviceList.begin();
		it != deviceList.end();
		++it) {
			if (it->getData().toString() == concatString) {
				return *it;
			}
		}
		return AudioDevice();
}

QtAddSIPAccount::QtAddSIPAccount(QtLoginDialog * qtLoginDialog, QWidget* parent, CUserProfileHandler & cUserProfileHandler)
	:ILogin(qtLoginDialog, cUserProfileHandler)
	, _reducedHeight(0) {

	_ui = new Ui::AddSIPAccount();
	_ui->setupUi(this);

	//默认域名服务器为ser.scut.edu.cn
	_ui->realm->setText("ser.scut.edu.cn");

	//对输入内容进行过滤 , zhenHua.sun 2010-08-19
	QRegExp loginExgExp(tr("^[0-9a-zA-Z\\._-]+$"));
	_ui->login->setValidator( new QRegExpValidator(loginExgExp,this ) );

	QRegExp domainExgExp(tr("^[0-9a-zA-Z-_]+(\\.[0-9a-zA-Z]+)*$"));
	_ui->realm->setValidator( new QRegExpValidator(domainExgExp,this ) );

	LANGUAGE_CHANGE(this);

//	WidgetBackgroundImage::setBackgroundImage(_ui->loginLabel, ":/pics/conference/setup_banner.jpg", WidgetBackgroundImage::AdjustHeight);

	// mandatory lines
	SAFE_CONNECT(_ui->login, SIGNAL(textChanged(QString)), SLOT(aMandatoryLineHasChanged()));
	SAFE_CONNECT(_ui->realm, SIGNAL(textChanged(QString)), SLOT(aMandatoryLineHasChanged()));
	////
		
	SAFE_CONNECT(_ui->advButton, SIGNAL(toggled(bool)), SLOT(showAdvancedSettings(bool)));
	SAFE_CONNECT(_ui->loginButton, SIGNAL(clicked()), SLOT(loginClicked()));
	SAFE_CONNECT(_ui->backButton, SIGNAL(clicked()), SLOT(goBack()));
	SAFE_CONNECT_RECEIVER(_ui->cancelButton, SIGNAL(clicked()), _loginDialog, SLOT(reject()));
	//SAFE_CONNECT(this, SIGNAL(teleConnectSinal()), SLOT(loginClicked())); //Add by hexianfu 09-11-11
	showAdvancedSettings(false);

	aMandatoryLineHasChanged();

	SAFE_CONNECT(_ui->addrTypeComboBox, SIGNAL(currentIndexChanged(int)), SLOT(setIPList(int)));
	//隐藏
	_ui->groupBox->hide();
	_ui->helpLabel->hide();
	_ui->backButton->hide();
	_ui->label_4->hide();
	_ui->GSGroupBox->hide();
	Config & config = ConfigManager::getInstance().getCurrentConfig();
	_ui->rememberPasswordCheckBox->setChecked(config.getPasswordRemember());
	_ui->autoConnectCheckBox->setChecked(config.getAutoConnect());

	_webcamDriver = WebcamDriver::getInstance();
	readNetworkConfig();
	readAVConfig();
	readGlobalServiceConfig();		//zhenHua.sun 2010-08-18

	QtAddSIPAccount::_qtWengoPhone=_cUserProfileHandler._cWengoPhone._qtWengoPhone;//Add By LZY 2010-09-27
	SAFE_CONNECT_RECEIVER(_qtWengoPhone,SIGNAL(ActionLoginSignal(QString)),this,SLOT(teleLoginCommand(QString)));//Add By LZY 2010-09-19 绑定响应Login动作Signal的函数
}

QtAddSIPAccount::~QtAddSIPAccount() {
	
	disconnect(_qtWengoPhone,SIGNAL(ActionLoginSignal(QString)),this,SLOT(teleLoginCommand(QString)));//Add By LZY 2010-09-19 解绑响应Login动作Signal的函数
	OWSAFE_DELETE(_ui);
}

void QtAddSIPAccount::loginClicked() {
	QtAddSIPAccount::_qtWengoPhone->SetUIState(UI_S_LOGIN_BEGIN);//Add By LZY 2010-09-30 用于通知遥控器界面状态变化
	Config & config = ConfigManager::getInstance().getCurrentConfig();

	//std::string accountname = _ui->accountname->text().toStdString();
	std::string login = _ui->login->text().toStdString();
	std::string password = _ui->password->text().toStdString();
	std::string realm = _ui->realm->text().toStdString();
	//std::string displayname = _ui->displayname->text().toLocal8Bit();
	//std::string registerServer = _ui->registerServer->text().toStdString();
	int registerPort = config.getSipRegisterPort();
	//std::string proxyServer = _ui->proxyServer->text().toStdString();
	int proxyPort = _ui->proxyPort->text().toInt();
	//
	std::string registerServer = realm;
	std::string proxyServer = registerServer;
	std::string accountname = login + "@" + registerServer;
	std::string displayname = accountname;
	//

	//fill ampty lines
	if (password.empty()) {
		password = login;
	}
	if (registerServer.empty()) {
		registerServer = realm;
	}
	if (proxyServer.empty()) {
		proxyServer = registerServer;
	}
	if (proxyPort==0) {
		proxyPort = 5060;
	}
	if (accountname.empty()) {
		accountname = login + "@" + registerServer;
	}
	if (displayname.empty()) {
		displayname = accountname;
	}
	////

	//create sip account
	SipAccount sipAccount(login, login, password, realm,
		registerServer, registerPort, proxyServer, proxyPort,
		true/*_ui->rememberPassword->isChecked()*/,
		_ui->activateSIMPLE->isChecked());
	sipAccount.setDisplayName(displayname);
	sipAccount.setVisibleName(accountname);
	////
	
	// Looking for the selected profile
	if (!_cUserProfileHandler.userProfileExists(sipAccount.getFullIdentity())) {
		// The selected profile does not exist. Creating a new one.
		UserProfile userProfile;

		Config & config = ConfigManager::getInstance().getCurrentConfig();
		QtAddAccountUtils::addTestContacts(&userProfile,
			config.getSipAudioTestCall(),
			config.getSipVideoTestCall()
			);

		userProfile.setSipAccount(sipAccount, false /* needInitialization */);
		UserProfileFileStorage userProfileStorage(userProfile);
		userProfileStorage.save(sipAccount.getFullIdentity());
	}
	_cUserProfileHandler.setCurrentUserProfile(sipAccount.getFullIdentity(), sipAccount);
	//
	config.set(Config::SIP_ACCOUNT_PASSWORDREMEMBER_KEY,_ui->rememberPasswordCheckBox->isChecked());
	config.set(Config::SIP_ACCOUNT_AUTOCONNECT_KEY,_ui->autoConnectCheckBox->isChecked());
	saveNetworkConfig();
	saveAVConfig();
	saveGlobalServiceConfig();		//zhenHua.sun 2010-08-18
	_loginDialog->accept();
}

void QtAddSIPAccount::setInfoMessage() {
	//setLoginLabel(tr("Please fill in the information which will<br/>allow your softphone to connect to your<br/>SIP account"));
	setLoginLabel(tr(""));
}

void QtAddSIPAccount::setErrorMessage(const QString & message) {
	setLoginLabel(QString("<font color=\"red\">%1</font>").arg(message));
}

void QtAddSIPAccount::setLoginLabel(const QString & message) {
	QString loginLabel = QString("<span style=\"font-size:16pt;\">" + 
                                  tr("\346\254\242\350\277\216\347\231\273\345\275\225CoolView") + "</span><br/>%1").arg(message);
	_ui->loginLabel->setText(loginLabel);
}

void QtAddSIPAccount::languageChanged() {
	_ui->retranslateUi(this);
}

void QtAddSIPAccount::load(const SipAccount & sipAccount) {
	if (sipAccount.getType() == SipAccount::SipAccountTypeBasic) {
		//loads the sip acccount
		_ui->login->setText(QString::fromStdString(sipAccount.getIdentity()));
		_ui->password->setText(QString::fromStdString(sipAccount.getPassword()));
		_ui->realm->setText(QString::fromStdString(sipAccount.getRealm()));
		_ui->displayname->setText(QString::fromStdString(sipAccount.getDisplayName()));
		_ui->registerServer->setText(QString::fromStdString(sipAccount.getRegisterServerHostname()));
		_ui->proxyServer->setText(QString::fromStdString(sipAccount.getSIPProxyServerHostname()));
		//_ui->registerPort->setText(QString::number(sipAccount.getRegisterServerPort()));
		_ui->proxyPort->setText(QString::number(sipAccount.getSIPProxyServerPort()));
		_ui->accountname->setText(QString::fromStdString(sipAccount.getVisibleName()));
		_ui->activateSIMPLE->setChecked(sipAccount.isPIMEnabled());
		setInfoMessage();
	} else {
		LOG_DEBUG("SIP getType() = SipAccount::SipAccountTypeWengo");
		//_loginDialog->changePage(QtLoginDialog::AddWengoAccount, sipAccount);
	}
}

void QtAddSIPAccount::load(std::string sipAccountName) {
	//load userprofile
	UserProfile * userProfile = _cUserProfileHandler.getUserProfileHandler().getUserProfile(sipAccountName);
	if (userProfile && userProfile->hasSipAccount()) {
		//tests account type
		if (!userProfile->hasWengoAccount()) {
			SipAccount * sipAccount = userProfile->getSipAccount();
			//loads the sip acccount
			_ui->login->setText(QString::fromStdString(sipAccount->getIdentity()));
			_ui->password->setText(QString::fromStdString(sipAccount->getPassword()));
			_ui->realm->setText(QString::fromStdString(sipAccount->getRealm()));
			_ui->displayname->setText(QString::fromLocal8Bit(sipAccount->getDisplayName().c_str()));
			_ui->registerServer->setText(QString::fromStdString(sipAccount->getRegisterServerHostname()));
			_ui->proxyServer->setText(QString::fromStdString(sipAccount->getSIPProxyServerHostname()));
			//_ui->registerPort->setText(QString::number(sipAccount->getRegisterServerPort()));
			_ui->proxyPort->setText(QString::number(sipAccount->getSIPProxyServerPort()));
			_ui->accountname->setText(QString::fromStdString(sipAccount->getVisibleName()));
			_ui->activateSIMPLE->setChecked(sipAccount->isPIMEnabled());
			setInfoMessage();
		} else {
			//should be loaded as a wengo account
			_loginDialog->changePage(QtLoginDialog::AddWengoAccount, sipAccountName);
		}
	} else {
		_loginDialog->changePage(QtLoginDialog::AddAccount);
	}
}

void QtAddSIPAccount::initPage() {
	_ui->accountname->setFocus();
	_ui->loginButton->setDefault(true);
	setInfoMessage();

	Config & config = ConfigManager::getInstance().getCurrentConfig();
	_ui->realm->setText(QString::fromStdString(config.getSipRealm()));
	_ui->registerServer->setText(QString::fromStdString(config.getSipRegisterServer()));
	_ui->proxyServer->setText(QString::fromStdString(config.getSipProxyServer()));
	_ui->proxyPort->setText(QString::number(config.getSipProxyPort()));
	_ui->activateSIMPLE->setChecked(config.getSipSimpleSupport());
}

void QtAddSIPAccount::aMandatoryLineHasChanged() {
	
	std::string login = _ui->login->text().toStdString();
	std::string realm = _ui->realm->text().toStdString();
	
	if (	!login.empty() &&
			!realm.empty() 
	) {
		_ui->loginButton->setEnabled(true);
	} else {
		_ui->loginButton->setEnabled(false);
	}
}


void QtAddSIPAccount::showAdvancedSettings(bool show) {
	if (show && _reducedHeight == 0) {
		// First time we show the advanced settins. Store dialog height, will
		// be used if the user hide the advanced settings
		_reducedHeight = _loginDialog->height();
	}

	_ui->GSGroupBox->setVisible(show);
	_ui->frame->setVisible(show);
	_ui->label_server->setVisible(show);
	_ui->label_proxy->setVisible(show);
	_ui->registerServer->setVisible(show);
	_ui->proxyServer->setVisible(show);
	_ui->label_proxyport->setVisible(show);
	_ui->proxyPort->setVisible(show);

	if (!show && _reducedHeight != 0) {
		// Make sure height is reduced to the size before expanding settings
		_loginDialog->setMinimumHeight(_reducedHeight);
		_loginDialog->resize(_loginDialog->width(), _reducedHeight);
	}

	if (show) {
		// If we try to show advanced settings a second time, the height will
		// remain _reducedHeight, this code makes sure we get back to the old
		// size
		this->layout()->invalidate();
		_loginDialog->layout()->invalidate();
		int height = _loginDialog->layout()->minimumSize().height();
		_loginDialog->resize(_loginDialog->width(), height);
	}
}

void QtAddSIPAccount::readNetworkConfig()
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
	_ui->IPComboBox->setCurrentIndex(0);
}

void QtAddSIPAccount::saveNetworkConfig()
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
}

void QtAddSIPAccount::readAVConfig()
{
	Config & config = ConfigManager::getInstance().getCurrentConfig();
	_ui->WebcamComboBox->clear();
	_ui->WebcamComboBox->addItems(StringListConvert::toQStringList(_webcamDriver->getDeviceList()));
	_ui->WebcamComboBox->setCurrentIndex(_ui->WebcamComboBox->findText(QString::fromLocal8Bit(config.getVideoWebcamDevice().c_str())));

	//inputDeviceList
	_ui->AudioInputComboBox->clear();


	std::list<AudioDevice> inputDeviceList = AudioDeviceManager::getInstance().getInputDeviceList();
	for (std::list<AudioDevice>::const_iterator it = inputDeviceList.begin();
		it != inputDeviceList.end();
		++it) {
			if( it==inputDeviceList.begin() )
				continue;				//不要第一个，因为directshow中无法得到 zhenHua.sun 2010-10-27
			_ui->AudioInputComboBox->addItem(
				QString::fromLocal8Bit((*it).getName().c_str()),
				QString::fromLocal8Bit((*it).getData().toString().c_str())
				);
		}

		QString currentInputDeviceId = 
			QString::fromLocal8Bit(config.getAudioInputDeviceId().toString().c_str());
		int index = _ui->AudioInputComboBox->findData( currentInputDeviceId );
		_ui->AudioInputComboBox->setCurrentIndex(
			index>0 ? index : 0
			);
		////

		//outputDeviceList
		_ui->AudioOutputComboBox->clear();

		std::list<AudioDevice> outputDeviceList = AudioDeviceManager::getInstance().getOutputDeviceList();
		for (std::list<AudioDevice>::const_iterator it = outputDeviceList.begin();
			it != outputDeviceList.end();
			++it) {
				if( it==outputDeviceList.begin() )
					continue;		//不要第一个，因为directshow中无法得到 zhenHua.sun 2010-10-27
				_ui->AudioOutputComboBox->addItem(
					QString::fromLocal8Bit((*it).getName().c_str()),
					QString::fromLocal8Bit((*it).getData().toString().c_str())
					);
			}

			QString currentOutputDeviceId = 
				QString::fromLocal8Bit(config.getAudioOutputDeviceId().toString().c_str());
			index = _ui->AudioOutputComboBox->findData( currentOutputDeviceId );
			_ui->AudioOutputComboBox->setCurrentIndex(
				index>0 ? index : 0
				);
			////

}

void QtAddSIPAccount::saveAVConfig()
{
	int width = 176;
	int height = 144;
	int bitrate = 1960;
	std::string codecSerial = "XVID/MPEG2";
	Config & config = ConfigManager::getInstance().getCurrentConfig();
	config.set(Config::VIDEO_ENABLE_KEY, true);
	config.set(Config::VIDEO_WEBCAM_DEVICE_KEY, std::string(_ui->WebcamComboBox->currentText().toLocal8Bit()));
	_webcamDriver->getCodecParameter(std::string(_ui->WebcamComboBox->currentText().toLocal8Bit()),width,height,bitrate,codecSerial);
	//config.set(Config::VIDEO_FRAME_WIDTH_KEY,352);
	//config.set(Config::VIDEO_FRAME_HEIGHT_KEY,288); 视频大小会被重置，应该采用配置文件的值

	config.set(Config::VIDEO_BITRATE_KEY,bitrate);
	config.set(Config::VIDEO_CODEC_SERIAL,codecSerial);

	AudioDevice device = getAudioDeviceFromComboBox(_ui->AudioInputComboBox, AudioDeviceManager::getInstance().getInputDeviceList());
	config.set(Config::AUDIO_INPUT_DEVICEID_KEY, device.getData());

	device = getAudioDeviceFromComboBox(_ui->AudioOutputComboBox, AudioDeviceManager::getInstance().getOutputDeviceList());
	config.set(Config::AUDIO_OUTPUT_DEVICEID_KEY, device.getData());
}

void QtAddSIPAccount::readGlobalServiceConfig()
{
	Config & config = ConfigManager::getInstance().getCurrentConfig();
	_ui->Ipv4GSLineEdit->setText(QString(config.getGlobalService().c_str()));
	_ui->Ipv6GSLineEdit->setText(QString(config.getGlobalService().c_str()));
}

void QtAddSIPAccount::saveGlobalServiceConfig()
{
	Config & config = ConfigManager::getInstance().getCurrentConfig();
	config.set(Config:: SIP_GLOBAL_SERVICE, _ui->Ipv4GSLineEdit->text().toStdString());
}

void QtAddSIPAccount::setIPList(int index)
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

//-------修改&注释 By LZY 2010-09-19-------
/*
 * 响应Login动作事件的函数，在处理完基本的赋值后会调用[登陆]按钮对应的函数
 * @param QString LoginStr,传入的登陆参数
 * @return void
 */
/**
* Add by: hexianfu
* for the telecontroller to control the login
* 2009-11-10
*/
void QtAddSIPAccount::teleLoginCommand(QString LoginStr)
{
	CMessageFrame LoginParam(LoginStr.toStdString().c_str());
	_ui->login->setText(QString::fromLatin1(LoginParam.readLine(true)));
	_ui->password->setText(QString::fromLatin1(LoginParam.readLine(true)));
	_ui->realm->setText(QString::fromLatin1(LoginParam.readLine(true)));
	LoginParam.readLine(false);//Display Name,与User Name相同，无用参数，略过
	int RememberPwd = atoi(LoginParam.readLine(true));
	int IPversion = atoi(LoginParam.readLine(true));
	int IsMuticast = atoi(LoginParam.readLine(true));

	if(RememberPwd == 1)
		_ui->rememberPasswordCheckBox->setChecked(true);
	else
		_ui->rememberPasswordCheckBox->setChecked(false);

    if(IPversion == 0)
		_ui->addrTypeComboBox->setCurrentIndex(0);
	else
	   _ui->addrTypeComboBox->setCurrentIndex(1);

	if(IsMuticast == 1)
		_ui->isMulticastComboBox->setCurrentIndex(0);
	else
		_ui->isMulticastComboBox->setCurrentIndex(1);

	loginClicked();
}
//-------End 修改&注释 By LZY--------------

/*************************************/
