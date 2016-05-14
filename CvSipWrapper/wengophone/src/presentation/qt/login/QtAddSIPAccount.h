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

#ifndef OWQTADDSIPACCOUNT_H
#define OWQTADDSIPACCOUNT_H

#include "ILogin.h"
#include <control/MessageFrame.h>//Add By LZY 2010-09-21
#include "../QtWengoPhone.h"//Add By LZY 2010-09-27

class CUserProfileHandler;
class QtLoginDialog;
class QWidget;
class IWebcamDriver;

namespace Ui { class AddSIPAccount; }

/**
 * Qt login window.
 *
 * @author Tanguy Krotoff
 * @author Mathieu Stute
 * @author Xavier Desjardins
 */
class QtAddSIPAccount : public ILogin {
	Q_OBJECT
public:

	QtAddSIPAccount(QtLoginDialog * qtLoginDialog, QWidget* parent, CUserProfileHandler & cUserProfileHandler);

	~QtAddSIPAccount();

	void readNetworkConfig();

	void saveNetworkConfig();

	void readAVConfig();

	void saveAVConfig();

	
	void readGlobalServiceConfig();

	void saveGlobalServiceConfig();

	virtual void load(const SipAccount & sipAccount);

	virtual void load(std::string sipAccountName);

	virtual void initPage();

private Q_SLOTS:

	void loginClicked();

	void aMandatoryLineHasChanged();

	virtual void languageChanged();

	void showAdvancedSettings(bool state);

	void setIPList(int index);

	void teleLoginCommand(QString LoginStr);//Add By LZY 2010-09-28 处理Login动作Slot

private:

	virtual void setInfoMessage();

	virtual void setErrorMessage(const QString & message);

	virtual void setLoginLabel(const QString & message);

	Ui::AddSIPAccount * _ui;

	int _reducedHeight;

	IWebcamDriver * _webcamDriver;

	QtWengoPhone * _qtWengoPhone;//Add By LZY 2010-09-27
};

#endif	//OWQTADDSIPACCOUNT_H
