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

#include "QtProfileDetails.h"

#include "ui_ProfileDetails.h"

#include <imwrapper/IMContactSet.h>

#include <presentation/qt/imcontact/QtAdvancedIMContactManager.h>
#include <presentation/qt/imcontact/QtSimpleIMContactManager.h>
//#include <presentation/qt/imaccount/QtIMAccountManager.h>			精简 zhenHua.sun 2010-09-03

#include <control/contactlist/CContactList.h>
#include <control/profile/CUserProfile.h>

#include <model/config/ConfigManager.h>
#include <model/config/Config.h>
#include <model/contactlist/ContactProfile.h>
#include <model/profile/UserProfile.h>

#include <cutil/global.h>

#include <qtutil/DesktopService.h>
#include <qtutil/ImageSelector.h>
#include <qtutil/PixmapMerging.h>
#include <qtutil/SafeConnect.h>
#include <qtutil/StringListConvert.h>

#include <util/CountryList.h>
#include <util/Logger.h>
#include <util/WebBrowser.h>
#include <util/SafeDelete.h>

#include <QtGui/QtGui>

#ifdef OS_WINDOWS
	#include <windows.h>
#endif

static const char * PNG_FORMAT = "PNG";

QtProfileDetails::QtProfileDetails(CUserProfile & cUserProfile,
	ContactProfile & contactProfile, QWidget * parent, const QString & windowTitle)
	: QObject(parent),
	_cUserProfile(cUserProfile),
	_profile(contactProfile) {

	init(parent);
	_profileDetailsWindow->setWindowTitle(tr("@product@") + " - " + windowTitle);

	//FIXME we should keep in memory the UUID of the group
	std::vector< std::pair<std::string, std::string> > tmp = _cUserProfile.getCContactList().getContactGroups();
	for (std::vector< std::pair<std::string, std::string> >::const_iterator it = tmp.begin();
		it != tmp.end(); ++it) {

		_ui->groupComboBox->addItem(QString::fromUtf8((*it).second.c_str()), QString::fromStdString((*it).first.c_str()));
		std::string str1 = (*it).first;
		std::string str2 = contactProfile.getGroupId();
		if (str1 == str2) {
			_ui->groupComboBox->setCurrentIndex(_ui->groupComboBox->findText(QString::fromUtf8((*it).second.c_str())));
		}
	}

	//QtSimpleIMContactManager
	_qtIMContactManager = new QtSimpleIMContactManager(contactProfile, _cUserProfile, _profileDetailsWindow);
	int index = _ui->imStackedWidget->addWidget(_qtIMContactManager->getWidget());
	_ui->imStackedWidget->setCurrentIndex(index);
	
	SAFE_CONNECT(_qtIMContactManager, SIGNAL(contentHasChanged()), SLOT(updateSaveButtonState()));
	SAFE_CONNECT(_ui->mobilePhoneLineEdit, SIGNAL(textChanged(const QString &)), SLOT(updateSaveButtonState()));
	SAFE_CONNECT(_ui->homePhoneLineEdit, SIGNAL(textChanged(const QString &)), SLOT(updateSaveButtonState()));

	//saveButton
	updateSaveButtonState();
	SAFE_CONNECT(_ui->saveButton, SIGNAL(clicked()), SLOT(saveContact()));

	//avatarPixmapButton
	_ui->avatarPixmapButton->setToolTip("");
	_ui->avatarPixmapButton->setEnabled(false);
	_ui->avatarPixmapButton->hide();
	//_ui->myProfileLabel->hide();
	_ui->groupBox_4->hide();
	_ui->advancedButton->hide();


	//对输入内容进行过滤  zhenHua.sun 2010-08-19
	QRegExp loginNameExp(tr("^[0-9a-zA-Z]+([\\._-][0-9a-zA-Z]+)*@[0-9a-zA-Z]+(\\.[0-9a-zA-Z]+)*$"));
	QRegExp phoneExp(tr("^[0-9]+$"));
	QRegExp mailExp(tr("^[0-9a-zA-Z]+([\\._-][0-9a-zA-Z]+)*@[0-9a-zA-Z]+(\\.[0-9a-zA-Z]+)*$"));
	_ui->mobilePhoneLineEdit->setValidator( new QRegExpValidator( phoneExp , this ));
	_ui->homePhoneLineEdit->setValidator( new QRegExpValidator(phoneExp ,this ));
	_ui->emailLineEdit->setValidator( new QRegExpValidator(mailExp ,this ));
	

}

QtProfileDetails::QtProfileDetails(CUserProfile & cUserProfile,
	UserProfile & userProfile, QWidget * parent, const QString & windowTitle)
	: QObject(parent),
	_cUserProfile(cUserProfile),
	_profile(userProfile) {

	init(parent);
	_profileDetailsWindow->setWindowTitle(tr("@product@") + " - " + windowTitle);

	//Not needed for UserProfile
	_ui->groupLabel->hide();
	_ui->groupComboBox->hide();

	/* 精简 zhenHua.sun 2010-09-03
	//QtIMAccountManager
	QtIMAccountManager * qtIMAccountManager =
		new QtIMAccountManager((UserProfile &) _profile, false, _profileDetailsWindow);
	int index = _ui->imStackedWidget->addWidget(qtIMAccountManager->getWidget());
	_ui->imStackedWidget->setCurrentIndex(index);
	_ui->advancedButton->hide();
	/// no more simple mode
	*/

	//saveButton
	SAFE_CONNECT(_ui->saveButton, SIGNAL(clicked()), SLOT(saveUserProfile()));

	//avatarPixmapButton
	SAFE_CONNECT(_ui->avatarPixmapButton, SIGNAL(clicked()), SLOT(changeUserProfileAvatar()));
}

void QtProfileDetails::init(QWidget * parent) {
	_profileDetailsWindow = new QDialog(parent);

	_ui = new Ui::ProfileDetails();
	_ui->setupUi(_profileDetailsWindow);

	populateCountryList();

	SAFE_CONNECT(_ui->cancelButton, SIGNAL(clicked()), SLOT(cancelButtonClicked()));
	SAFE_CONNECT(_ui->advancedButton, SIGNAL(clicked()), SLOT(advancedButtonClicked()));
	SAFE_CONNECT(_ui->websiteButton, SIGNAL(clicked()), SLOT(websiteButtonClicked()));
	SAFE_CONNECT(_ui->emailButton, SIGNAL(clicked()), SLOT(emailButtonClicked()));

	_qtIMContactManager = NULL;

	readProfile();
}

void QtProfileDetails::populateCountryList() {
	_ui->countryComboBox->addItems(StringListConvert::toQStringList(CountryList::getCountryList()));
}

int QtProfileDetails::show() {
	return _profileDetailsWindow->exec();
}

void QtProfileDetails::readProfile() {
	_ui->firstNameLineEdit->setText(QString::fromUtf8(_profile.getFirstName().c_str()));
	_ui->lastNameLineEdit->setText(QString::fromUtf8(_profile.getLastName().c_str()));
	_ui->genderComboBox->setCurrentIndex((int) _profile.getSex());

	Date date = _profile.getBirthdate();
	_ui->birthDate->setDate(QDate(date.getYear(), date.getMonth(), date.getDay()));

	StreetAddress address = _profile.getStreetAddress();
	_ui->countryComboBox->setCurrentIndex(_ui->countryComboBox->findText(QString::fromUtf8(address.getCountry().c_str())));
	_ui->cityLineEdit->setText(QString::fromUtf8(address.getCity().c_str()));
	_ui->stateLineEdit->setText(QString::fromUtf8(address.getStateProvince().c_str()));

	_ui->mobilePhoneLineEdit->setText(QString::fromStdString(_profile.getMobilePhone()));
	_ui->homePhoneLineEdit->setText(QString::fromStdString(_profile.getHomePhone()));

	_ui->emailLineEdit->setText(QString::fromStdString(_profile.getPersonalEmail()));
	_ui->webLineEdit->setText(QString::fromStdString(_profile.getWebsite()));

	_ui->notesEdit->setPlainText(QString::fromUtf8(_profile.getNotes().c_str()));

	updateAvatarButton();
}

void QtProfileDetails::updateAvatarButton() {
	std::string backgroundPixmapFilename = ":/pics/avatar_background.png";
	std::string foregroundPixmapData = _profile.getIcon().getData();

	_ui->avatarPixmapButton->setIcon(PixmapMerging::merge(foregroundPixmapData, backgroundPixmapFilename));
}

void QtProfileDetails::saveProfile() {
	_profile.setFirstName(_ui->firstNameLineEdit->text().toUtf8().data());

	_profile.setLastName(_ui->lastNameLineEdit->text().toUtf8().data());

	QDate date = _ui->birthDate->date();
	_profile.setBirthdate(Date(date.day(), date.month(), date.year()));

	_profile.setSex((EnumSex::Sex) _ui->genderComboBox->currentIndex());

	StreetAddress address;
	address.setCountry(_ui->countryComboBox->currentText().toUtf8().data());
	address.setStateProvince(_ui->stateLineEdit->text().toUtf8().data());
	address.setCity(_ui->cityLineEdit->text().toUtf8().data());

	_profile.setStreetAddress(address);

	_profile.setMobilePhone(_ui->mobilePhoneLineEdit->text().toStdString());
	_profile.setHomePhone(_ui->homePhoneLineEdit->text().toStdString());

	_profile.setPersonalEmail(_ui->emailLineEdit->text().toStdString());
	_profile.setWebsite(_ui->webLineEdit->text().toStdString());

	_profile.setNotes(_ui->notesEdit->toPlainText().toUtf8().constData());
}

ContactProfile & QtProfileDetails::fillAndGetContactProfile() {
	
	if (_qtIMContactManager) {
		_qtIMContactManager->saveIMContacts();
	}

	saveProfile();

	return (ContactProfile &) _profile;
}

void QtProfileDetails::saveContact() {

	if (_ui->groupComboBox->currentText().isEmpty()) {
		QMessageBox::warning(_profileDetailsWindow,
			tr("@product@ - No Group Selected"),
			tr("A contact must have a group, please set a group."),
			QMessageBox::NoButton,
			QMessageBox::NoButton,
			QMessageBox::Ok);

		return;
	}
	
	ContactProfile & contactProfile = fillAndGetContactProfile();

	int index = _ui->groupComboBox->findText(_ui->groupComboBox->currentText());
	QVariant groupId;
	groupId = _ui->groupComboBox->itemData(index);
	//If the group does not exist
	if (groupId.isNull()) {
		std::string groupName = std::string(_ui->groupComboBox->currentText().toUtf8().data());
		_cUserProfile.getCContactList().addContactGroup(groupName);
		groupId = QString::fromStdString(_cUserProfile.getCContactList().getContactGroupIdFromName(groupName));
	}

	contactProfile.setGroupId(groupId.toString().toStdString());

	_profileDetailsWindow->accept();
}

void QtProfileDetails::saveUserProfile() {
	saveProfile();

	_profileDetailsWindow->accept();
}

void QtProfileDetails::cancelButtonClicked() {
	_profileDetailsWindow->reject();
}

void QtProfileDetails::changeUserProfileAvatar() {
	Config & config = ConfigManager::getInstance().getCurrentConfig();
	QString startDir = QString::fromStdString(config.getResourcesDir()) + "pics/avatars";

	ImageSelector avatarSelector(_profileDetailsWindow);
	avatarSelector.setWindowTitle(tr("Select your avatar"));
	avatarSelector.setCurrentDir(startDir);
	
	// Add Wengo avatar dir
	int iconSize = ImageSelector::START_DIR_ICON_SIZE;
	QPixmap wengoPixmap = QPixmap(startDir + "/default-avatar.png");
	wengoPixmap = wengoPixmap.scaled(iconSize, iconSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	avatarSelector.addStartDirItem(startDir, tr("@company@ Avatars"), wengoPixmap);
	
	// Add desktop dirs
	DesktopService* service = DesktopService::getInstance();
	QStringList startDirList = service->startFolderList();
	Q_FOREACH(QString startDir, startDirList) {
		QString name = service->userFriendlyNameForPath(startDir);
		QPixmap pix = service->pixmapForPath(startDir, iconSize);
		avatarSelector.addStartDirItem(startDir, name, pix);
	}
	////
	
	if (!avatarSelector.exec()) {
		return;
	}

	setAvatarImage(avatarSelector.path());
	updateAvatarButton();

#ifdef OS_WINDOWS
	BringWindowToTop(_profileDetailsWindow->winId());
#endif
}

void QtProfileDetails::setAvatarImage(const QString& path) {
	//Size of Wengo avatars
	QSize size(96, 96);
	//Image is scaled to not save a big picture in userprofile.xml
	QImage image(path);
	image = image.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);

	QBuffer buffer;
	buffer.open(QIODevice::ReadWrite);
	image.save(&buffer, PNG_FORMAT);
	buffer.close();

	QByteArray byteArray = buffer.data();
	std::string data(byteArray.data(), byteArray.size());

	OWPicture picture = OWPicture::pictureFromData(data);
	QFileInfo fileInfo(path);
	picture.setFilename(fileInfo.fileName().toStdString());

	_profile.setIcon(picture);
}

void QtProfileDetails::advancedButtonClicked() {

	// save modifications before changing of mode
	fillAndGetContactProfile();
	////

	if (_qtIMContactManager->getContactManagerMode() == QtIMContactManagerInterface::contactManagerModeSimple) {
		//Simple mode -> advanced mode
		_ui->advancedButton->setText(tr("<< Simple"));
		
		//QtAdvancedIMContactManager
		QtAdvancedIMContactManager * qtIMContactManager =
			new QtAdvancedIMContactManager((ContactProfile &) _profile, _cUserProfile, _profileDetailsWindow);

		_ui->imStackedWidget->removeWidget(_qtIMContactManager->getWidget());
		OWSAFE_DELETE(_qtIMContactManager);
		
		int index = _ui->imStackedWidget->addWidget(qtIMContactManager->getWidget());
		_ui->imStackedWidget->setCurrentIndex(index);
		_qtIMContactManager = qtIMContactManager;
		SAFE_CONNECT(_qtIMContactManager, SIGNAL(contentHasChanged()), SLOT(updateSaveButtonState()));
		
	} else /*if (_qtIMContactManager->getContactManagerMode() == QtIMContactManagerInterface::contactManagerModeAdvanced)*/ {
		//Advanced mode -> simple mode
		_ui->advancedButton->setText(tr("Advanced >>"));
		
		//QtSimpleIMContactManager
		QtSimpleIMContactManager * qtIMContactManager = 
			new QtSimpleIMContactManager((ContactProfile &) _profile, _cUserProfile, _profileDetailsWindow);

		_ui->imStackedWidget->removeWidget(_qtIMContactManager->getWidget());
		OWSAFE_DELETE(_qtIMContactManager);
		
		int index = _ui->imStackedWidget->addWidget(qtIMContactManager->getWidget());
		_ui->imStackedWidget->setCurrentIndex(index);
		_qtIMContactManager = qtIMContactManager;
		SAFE_CONNECT(_qtIMContactManager, SIGNAL(contentHasChanged()), SLOT(updateSaveButtonState()));
	}
}

void QtProfileDetails::websiteButtonClicked() {
	String website = _ui->webLineEdit->text().toStdString();
	if (!website.empty()) {
		if (!website.contains("http://")) {
			website = "http://" + website;
		}
		WebBrowser::openUrl(website);
	}
}

void QtProfileDetails::emailButtonClicked() {
	std::string email = _ui->emailLineEdit->text().toStdString();
	if (!email.empty()) {
		WebBrowser::openUrl("mailto:" + email);
	}
}

void QtProfileDetails::setGroup(const QString & group) {
	_ui->groupComboBox->setEditText(group);
}

void QtProfileDetails::setFirstName(const QString & firstName) {
	_ui->firstNameLineEdit->setText(firstName);
}

void QtProfileDetails::setLastName(const QString & lastName) {
	_ui->lastNameLineEdit->setText(lastName);
}

void QtProfileDetails::setCountry(const QString & country) {
	_ui->countryComboBox->setEditText(country);
}

void QtProfileDetails::setCity(const QString & city) {
	_ui->cityLineEdit->setText(city);
}

void QtProfileDetails::setState(const QString & state) {
	_ui->stateLineEdit->setText(state);
}

void QtProfileDetails::setWebsite(const QString & website) {
	_ui->webLineEdit->setText(website);
}

void QtProfileDetails::setHomePhone(const QString & homePhone) {
	_ui->homePhoneLineEdit->setText(homePhone);
}

void QtProfileDetails::setWengoName(const QString & wengoName) {
	
	_qtIMContactManager->setWengoName(wengoName);
}

void QtProfileDetails::setSipAddress(const QString & sipAddress) {
	_qtIMContactManager->setSipAddress(sipAddress);
}

void QtProfileDetails::updateSaveButtonState() {

	if ((!_ui->mobilePhoneLineEdit->text().isEmpty()) ||
		(!_ui->homePhoneLineEdit->text().isEmpty()) ||
		_qtIMContactManager->couldBeSaved()
	   ) {
		_ui->saveButton->setEnabled(true);
	} else {
		_ui->saveButton->setEnabled(false);
	}
}
