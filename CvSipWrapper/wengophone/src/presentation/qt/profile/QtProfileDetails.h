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

#ifndef OWQTPROFILEDETAILS_H
#define OWQTPROFILEDETAILS_H

#include <util/NonCopyable.h>

#include <QtCore/QObject>

class UserProfile;
class ContactProfile;
class CUserProfile;
class Profile;
class QtIMContactManagerInterface;

class QImage;
class QWidget;
class QDialog;
namespace Ui { class ProfileDetails; }

/**
 * Profile/Contact/UserProfile details window.
 *
 * @see Profile
 * @see UserProfile
 * @see Contact
 * @author Tanguy Krotoff
 */
class QtProfileDetails : public QObject, NonCopyable {
	Q_OBJECT
public:

	QtProfileDetails(CUserProfile & cUserProfile, ContactProfile & contactProfile, QWidget * parent, const QString & windowTitle);

	QtProfileDetails(CUserProfile & cUserProfile, UserProfile & userProfile, QWidget * parent, const QString & windowTitle);

	int show();

	QDialog * getWidget() const {
		return _profileDetailsWindow;
	}

	void setWengoName(const QString & wengoName);

	void setGroup(const QString & group);

	void setFirstName(const QString & firstName);

	void setLastName(const QString & lastName);

	void setCountry(const QString & country);

	void setCity(const QString & city);

	void setState(const QString & state);

	void setWebsite(const QString & website);

	void setHomePhone(const QString & homePhone);

	void setSipAddress(const QString & sipAddress);

public Q_SLOTS:

	void changeUserProfileAvatar();

private Q_SLOTS:

	void updateSaveButtonState();

	void saveContact();

	void saveUserProfile();

	void cancelButtonClicked();

	/**
	 * Switch from simple mode to advanced mode.
	 *
	 * The button advanced from QtSimpleIMContactManager has been clicked.
	 * Changes _ui->imStackedWidget widget.
	 */
	void advancedButtonClicked();

	/**
	 * Shows the contact web page.
	 */
	void websiteButtonClicked();

	/**
	 * Shows the contact email.
	 */
	void emailButtonClicked();

private:

	ContactProfile & fillAndGetContactProfile();

	void init(QWidget * parent);

	/** Populates the country list widget. */
	void populateCountryList();

	void readProfile();

	void updateAvatarButton();

	void saveProfile();

	void setAvatarImage(const QString& path);

	Ui::ProfileDetails * _ui;

	QDialog * _profileDetailsWindow;

	CUserProfile & _cUserProfile;

	Profile & _profile;

	QtIMContactManagerInterface * _qtIMContactManager;

	/**
	 * QtProfileDetails shows a Contact rather than a UserProfile if true.
	 */
	bool _showContact;
};

#endif	//QTPROFILEDETAILS_H
