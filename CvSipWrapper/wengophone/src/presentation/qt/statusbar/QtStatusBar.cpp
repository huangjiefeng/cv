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

#include "QtStatusBar.h"

#include "ui_StatusBarWidget.h"

#include <QtGui/QMovie>
#include <QtGui/QSlider>
#include <QtGui/QStatusBar>

#include <model/config/ConfigManager.h>
#include <model/config/Config.h>
#include <model/phoneline/IPhoneLine.h>
#include <model/phoneline/PhoneLineState.h>
#include <model/profile/UserProfile.h>

#include <presentation/qt/QtWengoPhone.h>
#include <presentation/qt/QtToolBar.h>

#include <sound/VolumeControl.h>
#include <sound/AudioDevice.h>
#include <qtutil/DropDownWindow.h>
#include <qtutil/LanguageChangeEventFilter.h>
#include <qtutil/SafeConnect.h>
#include <util/Logger.h>
#include <util/SafeDelete.h>

static const char * MNG_FORMAT = "MNG";

QtStatusBar::QtStatusBar(QtWengoPhone* qtWengoPhone, QStatusBar * statusBar)
	: QWidget(statusBar)
	{

	_ui = new Ui::StatusBarWidget();
	_ui->setupUi(this);
	_statusBar = statusBar;
	_statusBar->addPermanentWidget(this, 1);

	_audioOutputWindow = 0;
	_outputVolumeSlider = 0;

	_audioInputWindow = 0;
	_inputVolumeSlider = 0;

	SAFE_CONNECT(this, SIGNAL(audioOutputConfigChanged()), SLOT(updateAudioOutputUi()));
	SAFE_CONNECT(this, SIGNAL(audioInputConfigChanged()), SLOT(updateAudioInputUi()));


	LANGUAGE_CHANGE(this);

	createAudioOutputWindow();
	createAudioInputWindow();

	//sip label
	_sipConnectionMovie = new QMovie(":/pics/statusbar/network_status_connecting.mng", MNG_FORMAT, this);
	_sipConnectionMovie->start();
	_ui->sipLabel->setMovie(_sipConnectionMovie);
	////

	//sound buttons
	Config & config = ConfigManager::getInstance().getCurrentConfig();
	config.valueChangedEvent += boost::bind(&QtStatusBar::checkSoundConfig, this, _1);
	////

	//status bar re-initialization
	SAFE_CONNECT(qtWengoPhone, SIGNAL(userProfileDeleted()), SLOT(userProfileDeleted()));

	init();

	QtStatusBar::_qtWengoPhone=qtWengoPhone;//Add By LZY 2010-09-26
}

QtStatusBar::~QtStatusBar() {
	OWSAFE_DELETE(_ui);
}

void QtStatusBar::showMessage(const QString & message, int timeout) {
	_statusBar->showMessage(message, timeout);
}

void QtStatusBar::checkSoundConfig(const std::string & key) {
	if (key == Config::AUDIO_OUTPUT_DEVICEID_KEY) {
		updateAudioOutputUi();
	} else if (key == Config::AUDIO_INPUT_DEVICEID_KEY) {
		updateAudioInputUi();
	}
}

void QtStatusBar::updateAudioOutputUi() {
	Config & config = ConfigManager::getInstance().getCurrentConfig();
	VolumeControl outputVolumeControl(config.getAudioOutputDeviceId());
	QString message;
	bool error = false;

	if (outputVolumeControl.isMuted()) {
		error = true;
		message = tr("Output volume is muted");
	} else if (outputVolumeControl.getLevel() == 0) {
		error = true;
		message = tr("Output volume is 0");
	}
	if (error) {
		_ui->audioOutputButton->setIcon(QPixmap(":/pics/statusbar/output_volume_error.png"));
		_ui->audioOutputButton->setToolTip(message);
	} else {
		_ui->audioOutputButton->setIcon(QPixmap(":/pics/statusbar/output_volume_ok.png"));
		_ui->audioOutputButton->setToolTip(tr("Click to adjust output volume"));
	}

	if (_outputVolumeSlider) {
		initVolumeSlider(_outputVolumeSlider);
	}
}

void QtStatusBar::updateAudioInputUi() {
	Config & config = ConfigManager::getInstance().getCurrentConfig();
	VolumeControl inputVolumeControl(config.getAudioInputDeviceId());
	QString message;
	bool error = false;

	if (inputVolumeControl.isMuted()) {
		error = true;
		message = tr("Input volume is muted");
	} else if (inputVolumeControl.getLevel() == 0) {
		error = true;
		message = tr("Input volume is 0");
	}
	if (error) {
		_ui->audioInputButton->setIcon(QPixmap(":/pics/statusbar/input_volume_error.png"));
		_ui->audioInputButton->setToolTip(message);
	} else {
		_ui->audioInputButton->setIcon(QPixmap(":/pics/statusbar/input_volume_ok.png"));
		_ui->audioInputButton->setToolTip(tr("Click to adjust input volume"));
	}

	if (_inputVolumeSlider) {
		initVolumeSlider(_inputVolumeSlider);
	}
}

void QtStatusBar::userProfileDeleted() {
	QPixmap pixmap(":/pics/statusbar/network_status_error.png");
	_ui->sipLabel->setPixmap(pixmap);
	_ui->sipLabel->setToolTip(tr("Not connected"));
}

void QtStatusBar::updatePhoneLineState(EnumPhoneLineState::PhoneLineState state) {

		QString tooltip;
		QString pixmap;

		//Stops animated pixmap
		OWSAFE_DELETE(_sipConnectionMovie);
		switch (state) {
		case EnumPhoneLineState::PhoneLineStateUnknown:
			tooltip = tr("Not connected");
			pixmap = ":/pics/statusbar/network_status_error.png";
			break;

		case EnumPhoneLineState::PhoneLineStateServerError:
			tooltip = tr("An error occured: Server error.");
			pixmap = ":/pics/statusbar/network_status_error.png";
			break;

		case EnumPhoneLineState::PhoneLineStateTimeout:
			tooltip = tr("An error occured: Timeout.");
			pixmap = ":/pics/statusbar/network_status_error.png";
			break;

		case EnumPhoneLineState::PhoneLineStateAuthenticationError:
			tooltip = tr("An error occured: Authentication error.");
			pixmap = ":/pics/statusbar/network_status_error.png";
			break;

		case EnumPhoneLineState::PhoneLineStateOk:
			tooltip = tr("Register done");
			pixmap = ":/pics/statusbar/network_status_ok.png";
			break;

		case EnumPhoneLineState::PhoneLineStateClosed:
			tooltip = tr("Unregister done");
			pixmap = ":/pics/statusbar/network_status_error.png";
			break;

		case EnumPhoneLineState::PhoneLineStateProgress:
			_sipConnectionMovie = new QMovie(":/pics/statusbar/network_status_connecting.mng", MNG_FORMAT, this);
			_sipConnectionMovie->start();
			_ui->sipLabel->setMovie(_sipConnectionMovie);
			_ui->sipLabel->setToolTip(tr("Connecting"));
			return;

		default:
			LOG_FATAL("unknown state=" + EnumPhoneLineState::toString(state));
		};

		_ui->sipLabel->setPixmap(pixmap);
		_ui->sipLabel->setToolTip(tooltip);
}

void QtStatusBar::init() {
	updateAudioOutputUi();
	updateAudioInputUi();
}

void QtStatusBar::languageChanged() {
	_ui->retranslateUi(this);
	init();
}

static DropDownWindow* createDropDownWindow(QAbstractButton* button) {
	DropDownWindow* window = new DropDownWindow(button);

	window->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
	int width = button->minimumSize().width();
	window->setFixedWidth(width);

	return window;
}

static QSlider* createSlider(DropDownWindow* window) {
	QSlider* slider = new QSlider(window);

	// Set to range documented in VolumeControl class
	slider->setMinimum(0);
	slider->setMaximum(100);
	slider->setFixedHeight(100);

	QHBoxLayout* layout = new QHBoxLayout(window);
	layout->setMargin(2);
	layout->addWidget(slider);

	// A bit bigger than the slider so that it has some space above and below it
	window->setFixedHeight(110);

	return slider;
}

void QtStatusBar::createAudioOutputWindow() {
	_audioOutputWindow = createDropDownWindow(_ui->audioOutputButton);
	_outputVolumeSlider = createSlider(_audioOutputWindow);
	initVolumeSlider(_outputVolumeSlider);

	SAFE_CONNECT(_outputVolumeSlider, SIGNAL(valueChanged(int)),
		SLOT(setVolume(int)) );
}

void QtStatusBar::createAudioInputWindow() {
	_audioInputWindow = createDropDownWindow(_ui->audioInputButton);
	_inputVolumeSlider = createSlider(_audioInputWindow);
	initVolumeSlider(_inputVolumeSlider);

	SAFE_CONNECT(_inputVolumeSlider, SIGNAL(valueChanged(int)),
		SLOT(setVolume(int)) );
}

void QtStatusBar::initVolumeSlider(QSlider* slider) {
	Config & config = ConfigManager::getInstance().getCurrentConfig();
	StringList deviceId;
	if (slider == _outputVolumeSlider) {
		deviceId = config.getAudioOutputDeviceId();
	} else {
		deviceId = config.getAudioInputDeviceId();
	}
	AudioDevice audioDevice(deviceId);
	VolumeControl volumeControl(audioDevice);

	slider->blockSignals(true);
	slider->setValue(volumeControl.getLevel());
	slider->blockSignals(false);
}

//-------Modify By LZY 2010-09-26-------
void QtStatusBar::setVolume(int value) 
{
	Config & config = ConfigManager::getInstance().getCurrentConfig();
	StringList deviceId;
	if (sender() == _outputVolumeSlider)//ÑïÉùÆ÷ out
		deviceId = config.getAudioOutputDeviceId();
	else//Âó¿Ë·ç in
		deviceId = config.getAudioInputDeviceId();
	AudioDevice audioDevice(deviceId);
	VolumeControl volumeControl(audioDevice);
	volumeControl.setLevel(value);

	// FIXME: Remove this once/if the AudioDevice gets capable of notifying us
	// of volume changes (from us and from the outside world).
	if (sender() == _outputVolumeSlider) {
		updateAudioOutputUi();
	} else {
		updateAudioInputUi();
	}
}
//-------End of Modify By LZY---------
