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

#include "QtNotificationSettings.h"

#include "ui_NotificationSettings.h"
#include "ui_NotificationSettingsItem.h"

#include <model/config/ConfigManager.h>
#include <model/config/Config.h>

#include <sound/AudioDeviceManager.h>
#include <sound/Sound.h>

#include <util/File.h>
#include <util/Logger.h>
#include <util/SafeDelete.h>

#include <qtutil/DesktopService.h>
#include <qtutil/SafeConnect.h>

#include <QtGui/QFileDialog>
#include <QtGui/QWidget>

QtNotificationSettingsItem::QtNotificationSettingsItem(const std::string& configKey,
	const std::string& soundFile, const QString& caption)
	: QWidget()
	, _configKey(configKey)
	, _soundFile(QString::fromStdString(soundFile))
	, _caption(caption) {
	_ui = new Ui::NotificationSettingsItem;
}

QtNotificationSettingsItem::~QtNotificationSettingsItem() {
	OWSAFE_DELETE(_ui);
}

void QtNotificationSettingsItem::init(QWidget* parent) {
	setParent(parent);
	_ui->setupUi(this);

	// add an icon to browseButton
	QPixmap pixBrowse = DesktopService::getInstance()->desktopIconPixmap(DesktopService::FolderIcon, 16);
	_ui->browseButton->setIcon(pixBrowse);
	////
	
	SAFE_CONNECT(_ui->browseButton, SIGNAL(clicked()), SLOT(browseSounds()));
	SAFE_CONNECT(_ui->playButton, SIGNAL(clicked()), SLOT(playSound()));
	SAFE_CONNECT(_ui->soundLineEdit, SIGNAL(textChanged(const QString&)), SLOT(updatePlayButton()));
	SAFE_CONNECT(_ui->checkBox, SIGNAL(toggled(bool)), SLOT(updatePlayButton()));

	_ui->checkBox->setText(_caption);
	_ui->checkBox->setChecked(!_soundFile.isEmpty());
	_ui->frame->setEnabled(_ui->checkBox->isChecked());
	_ui->soundLineEdit->setText(_soundFile);
}

void QtNotificationSettingsItem::browseSounds() {
	Config & config = ConfigManager::getInstance().getCurrentConfig();

	QString file = QFileDialog::getOpenFileName(this,
				tr("Choose a sound file"),
				QString::fromStdString(config.getResourcesDir()) + "sounds",
				tr("Sounds") + " (*.wav)");

	std::string tmp = file.toStdString();
	tmp = File::convertPathSeparators(tmp);
	file = QString::fromStdString(tmp);

	if (!file.isEmpty()) {
		_ui->soundLineEdit->setText(file);
	}
}

void QtNotificationSettingsItem::playSound() {
	std::string soundFile = _ui->soundLineEdit->text().toStdString();
	Sound::play(soundFile, AudioDeviceManager::getInstance().getDefaultOutputDevice());
}

void QtNotificationSettingsItem::saveConfig() {
	Config & config = ConfigManager::getInstance().getCurrentConfig();
	
	std::string name;
	if (_ui->checkBox->isChecked()) {
		name = _ui->soundLineEdit->text().toStdString();
	}
	config.setResource(_configKey, name);
}

void QtNotificationSettingsItem::updatePlayButton() {
	_ui->playButton->setEnabled(!_ui->soundLineEdit->text().isEmpty());
}

QtNotificationSettings::QtNotificationSettings(QWidget * parent)
	: QWidget(parent) {

	_ui = new Ui::NotificationSettings();
	_ui->setupUi(this);

	QVBoxLayout* layout = new QVBoxLayout(_ui->contentFrame);
	layout->setMargin(0);
	layout->setSpacing(0);

	Config & config = ConfigManager::getInstance().getCurrentConfig();
	_list
		<< new QtNotificationSettingsItem(
			Config::AUDIO_INCOMINGCALL_FILE_KEY,
			config.getAudioIncomingCallFile(),
			tr("Incoming call (ringtone)"))

		<< new QtNotificationSettingsItem(
			Config::AUDIO_CALLCLOSED_FILE_KEY,
			config.getAudioCallClosedFile(),
			tr("Call closed (hang up tone)"))

		<< new QtNotificationSettingsItem(
			Config::AUDIO_INCOMINGCHAT_FILE_KEY,
			config.getAudioIncomingChatFile(),
			tr("Incoming chat"))

		<< new QtNotificationSettingsItem(
			Config::AUDIO_IMACCOUNTCONNECTED_FILE_KEY,
			config.getAudioIMAccountConnectedFile(),
			tr("IM account connected"))

		<< new QtNotificationSettingsItem(
			Config::AUDIO_IMACCOUNTDISCONNECTED_FILE_KEY,
			config.getAudioIMAccountDisconnectedFile(),
			tr("IM account disconnected"))

		<< new QtNotificationSettingsItem(
			Config::AUDIO_CONTACTONLINE_FILE_KEY,
			config.getAudioContactOnlineFile(),
			tr("Contact online"))
		;

	Q_FOREACH(QtNotificationSettingsItem* item, _list) {
		item->init(_ui->contentFrame);
		layout->addWidget(item);
	}
}

QtNotificationSettings::~QtNotificationSettings() {
	OWSAFE_DELETE(_ui);
}

void QtNotificationSettings::readConfig() {
	// Nothing to do, all done in CTor
}

QString QtNotificationSettings::getName() const {
	return tr("Notifications");
}

QString QtNotificationSettings::getTitle() const {
	return tr("Notifications Settings");
}

QString QtNotificationSettings::getIconName() const {
	return "notifications";
}

void QtNotificationSettings::saveConfig() {
	Q_FOREACH(QtNotificationSettingsItem* item, _list) {
		item->saveConfig();
	}
}
