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

#ifndef OWQTLOGINDIALOG_H
#define OWQTLOGINDIALOG_H

#include <model/account/SipAccount.h>

#include <vector>
#include <string>

#include <QtGui/QDialog>

class CUserProfileHandler;
class QStackedWidget;
class QtWengoPhone;
class ILogin;
class QMutex; //Add by hexianfu 2009-11-27
class QtAddSIPAccount;//Add by hexianfu 2009-11-27
/**
 * Qt login window.
 *
 * @author Tanguy Krotoff
 * @author Mathieu Stute
 * @author Xavier Desjardins
 */
class QtLoginDialog : public QDialog {
	Q_OBJECT
public:

	enum LoginDialogPage {
		AddAccount,
		AddWengoAccount,
		AddSIPAccount,
		LogAccount
	};

	QtLoginDialog(QtWengoPhone * qtWengoPhone, CUserProfileHandler & cUserProfileHandler);

	~QtLoginDialog();

	void changePage(int newPage, std::string sipAccountName = "");

	void setInfoMessage();

	void setErrorMessage(const QString & message);
	
	QtWengoPhone * getQtWengoPhone() {
		return _qtWengoPhone;
	}

	ILogin* currentPage() const;

	/**
	* Add by: hexianfu
	* get the current page index
	* 2009-11-10
	*/
 //   int getCurrentPageIndex();

    QtAddSIPAccount * getAddSipAccountPage() const;

	//void teleChangePage(int newPage/*,std::string sipAccountName*/);

	Q_SIGNALS:

	//void teleChangePageSinal(int newPage/*,const QString& sipAccountName*/);

	//void sendCurrentPageSinal(ILogin * _currentPage);
	/***************************/
    
public Q_SLOTS:
	/**
	 * login cancelled -- enable toolbar menu.
	 */
        void loginRejected();

	/**
	 * Init login entry and display a message on the login to say that the
	 * login/password is invalid.
	 */
	void setInvalidAccount(const SipAccount & sipAccount);

	/**
	 * Add the given SipAccount to the profile list and select it as default.
	 */
	void setValidAccount(SipAccount sipAccount);

	//void teleChangePageSlot(int newPage/*,const QString& sipAccountName*/); //Add by hexianfu 09-9-11

private:

	QtWengoPhone * _qtWengoPhone;

	QStackedWidget * _centralWidget;

	std::vector<ILogin *> _loginPage;

	QtAddSIPAccount * _qtAddSipAccount;
    
	/**
	* Add by: hexianfu
	* mutex between the main thread and teleController therad
	* 2009-11-9
	*/
	//QMutex * _mutex;
	/**********************/

};

#endif	//OWQTLOGINDIALOG_H
