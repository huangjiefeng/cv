#include "stdafx.h"
/*
 * WengoPhone, a voice over Internet phone
 * Copyright (C) 2007  Wengo
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

#include "QtAppearanceSettings.h"

#include "ui_AppearanceSettings.h"

#include <model/config/ConfigManager.h>
#include <model/config/Config.h>
#include <model/config/EnumToolBarMode.h>

#include <presentation/qt/QtWengoPhone.h>
#include <presentation/qt/chat/QtChatTheme.h>

#include <qtutil/SafeConnect.h>
#include <qtutil/WidgetUtils.h>
#include <util/Logger.h>
#include <util/SafeDelete.h>

#include <QtCore/QTime>

static const int THEMELIST_MINIMUM_WIDTH = 150;

QtAppearanceSettings::QtAppearanceSettings(QWidget * parent)
	: QWidget(parent) {

	_ui = new Ui::AppearanceSettings();
	_ui->setupUi(this);

	SAFE_CONNECT(_ui->themeListWidget, SIGNAL(currentRowChanged(int)),
		SLOT(updatePreview()) );

	readConfig();
	updatePreview();
}

QtAppearanceSettings::~QtAppearanceSettings() {
	OWSAFE_DELETE(_ui);
}

void QtAppearanceSettings::updatePreview() {
	_ui->themePreviewBrowser->clear();

	QListWidgetItem* item = _ui->themeListWidget->currentItem();
	if (!item) {
		return;
	}

	int row = _ui->themeListWidget->row(item);
	QString theme = _themeList[row];
	_ui->themePreviewBrowser->setTheme(theme);

	_ui->themePreviewBrowser->insertMessage("self", tr("Me"), tr("Hello"), QTime(12, 34));
	_ui->themePreviewBrowser->insertMessage("self", tr("Me"), tr("How are you?"), QTime(12, 34));
	_ui->themePreviewBrowser->insertMessage("other", tr("Other"), tr("I am fine"), QTime(12, 40));
	_ui->themePreviewBrowser->insertMessage("other", tr("Other"), tr("Have to go"), QTime(12, 50));
	_ui->themePreviewBrowser->insertMessage("other", tr("Other"), tr("Bye"), QTime(12, 51));
	_ui->themePreviewBrowser->insertStatusMessage("Other has left", QTime(12, 51));
}

QString QtAppearanceSettings::getName() const {
	return tr("Appearance");
}

QString QtAppearanceSettings::getTitle() const {
	return tr("Appearance Settings");
}

QString QtAppearanceSettings::getIconName() const {
	return "appearance";
}

void QtAppearanceSettings::saveConfig() {
	Config & config = ConfigManager::getInstance().getCurrentConfig();

	// Toolbar
	EnumToolBarMode::ToolBarMode mode;
	if (_ui->hiddenRadioButton->isChecked()) {
		mode = EnumToolBarMode::ToolBarModeHidden;
	} else if (_ui->iconsOnlyRadioButton->isChecked()) {
		mode = EnumToolBarMode::ToolBarModeIconsOnly;
	} else {
		mode = EnumToolBarMode::ToolBarModeTextUnderIcons;
	}
	std::string text = EnumToolBarMode::toString(mode);
	config.set(Config::GENERAL_TOOLBARMODE_KEY, text);

	// Theme
	QListWidgetItem* item = _ui->themeListWidget->currentItem();
	if (item) {
		int row = _ui->themeListWidget->row(item);
		QString theme = _themeList[row];
		config.set(Config::APPEARANCE_CHATTHEME_KEY, theme.toStdString());
	} else {
		LOG_WARN("No current chat theme");
	}
}

void QtAppearanceSettings::readConfig() {
	Config & config = ConfigManager::getInstance().getCurrentConfig();
	std::string text = config.getToolBarMode();
	EnumToolBarMode::ToolBarMode toolBarMode = EnumToolBarMode::toToolBarMode(text);

	// Toolbar
	if (toolBarMode == EnumToolBarMode::ToolBarModeHidden) {
		_ui->hiddenRadioButton->setChecked(true);
	} else if (toolBarMode == EnumToolBarMode::ToolBarModeIconsOnly) {
		_ui->iconsOnlyRadioButton->setChecked(true);
	} else {
		_ui->textUnderIconsRadioButton->setChecked(true);
	}

	// Theme
	QString currentTheme = QString::fromUtf8(config.getChatTheme().c_str());
	_themeList = QtChatTheme::getThemeList();
	Q_FOREACH(QString themeDir, _themeList) {
		QListWidgetItem* item = new QListWidgetItem(_ui->themeListWidget);

		QString name = themeDir;
		// Prettify the name a bit
		name[0] = name[0].toUpper();
		item->setText(name);

		_ui->themeListWidget->addItem(item);
		if (themeDir == currentTheme) {
			_ui->themeListWidget->setCurrentItem(item);
		}
	}

	int width = WidgetUtils::computeListViewMinimumWidth(_ui->themeListWidget);
	if (width < THEMELIST_MINIMUM_WIDTH) {
		width = THEMELIST_MINIMUM_WIDTH;
	}
	_ui->themeListWidget->setFixedWidth(width);
}
