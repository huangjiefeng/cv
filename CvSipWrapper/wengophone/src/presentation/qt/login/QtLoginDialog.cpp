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

#include "QtLoginDialog.h"

#include "QtAddAccount.h"
#include "QtAddWengoAccount.h"
#include "QtAddSIPAccount.h"
#include "QtLogAccount.h"

#include <presentation/qt/QtWengoPhone.h>
#include <presentation/qt/QtToolBar.h>

#include <control/profile/CUserProfileHandler.h>

#include <model/account/SipAccount.h>
#include <model/config/ConfigManager.h>
#include <model/config/Config.h>

#include <qtutil/SafeConnect.h>
#include <util/SafeDelete.h> //Add by heixanfu 09-11-27

#include <util/Logger.h>


#include <util/String.h>

#include <QtGui/QtGui>
//#include <QtCore/QMutex> //Add by heixanfu 09-11-27

QtLoginDialog::QtLoginDialog(QtWengoPhone * qtWengoPhone, CUserProfileHandler & cUserProfileHandler)
	:QDialog(qtWengoPhone->getWidget()),
	_qtWengoPhone(qtWengoPhone) {

	//_mutex = new QMutex(QMutex::Recursive);//Add by heixanfu 09-11-27
	_qtWengoPhone->_wengoPhoneWindow->hide();   //Add by dhx 10-10-25

	_centralWidget = new QStackedWidget(this);

	_loginPage.push_back(new QtAddAccount(this, _centralWidget, cUserProfileHandler));
	_loginPage.push_back(new QtAddWengoAccount(this, _centralWidget, cUserProfileHandler));
	_qtAddSipAccount = new QtAddSIPAccount(this, _centralWidget, cUserProfileHandler); //Add by hexianfu
	//_loginPage.push_back(new QtAddSIPAccount(this, _centralWidget, cUserProfileHandler));
	_loginPage.push_back(_qtAddSipAccount);   //Add by hexianfu
	_loginPage.push_back(new QtLogAccount(this, _centralWidget, cUserProfileHandler));
    SAFE_CONNECT(this, SIGNAL(rejected()), SLOT(loginRejected()));

    //SAFE_CONNECT_TYPE(this, SIGNAL(teleChangePageSinal(int/*,const QString&*/)),SLOT(teleChangePageSlot(int/*,const QString&*/)), Qt::QueuedConnection);//Add by: hexianfu 2009-11-10
	//SAFE_CONNECT_RECEIVER(this,SIGNAL(sendCurrentPageSinal(ILogin *)),_qtWengoPhone,SLOT(getCurrentPageSlot(ILogin *)));//Add by: hexianfu 2009-11-27

    _centralWidget->addWidget(_loginPage[AddAccount]);
    _centralWidget->addWidget(_loginPage[AddWengoAccount]);
    _centralWidget->addWidget(_loginPage[AddSIPAccount]);
	_centralWidget->addWidget(_loginPage[LogAccount]);
	
	setWindowTitle(tr("CoolView - \347\231\273\345\275\225"));

    QVBoxLayout * layout = new QVBoxLayout(this);
    layout->addWidget(_centralWidget);
	resize(306,250);
	changePage(LogAccount);
	QtLoginDialog::_qtWengoPhone->SetUIState(UI_S_LOGOFF);//Add By LZY 2010-09-30 用于通知遥控器界面状态变化
}

QtLoginDialog::~QtLoginDialog() {
	//OWSAFE_DELETE(_qtAddSipAccount);
}

/**
* modified by: hexianfu
* to support the teleController control the Login process
* 2009-11-27
*/
void QtLoginDialog::changePage(int newPage, std::string sipAccountName) {
	if ( (newPage >= 0) && (newPage < _centralWidget->count())) 
	{
		int page = newPage ;
		
	#ifndef CUSTOM_ACCOUNT
		if( page == 0)
		{
			page = 2;
		}
	#endif
		
		_centralWidget->setCurrentIndex(page);
		_loginPage[page]->initPage();
		if (!sipAccountName.empty()) {
			_loginPage[page]->load(sipAccountName);
		}
	}
	/*************************************/// Add by hexianfu 09-11-27
	//if((_qtWengoPhone->isSendLoginCommand)&&(newPage==2))
	//{
	//	ILogin *_iLogin = currentPage();
	//	sendCurrentPageSinal(_iLogin);
	//}
	/*************************************/
}

ILogin* QtLoginDialog::currentPage() const {
	//QMutexLocker locker(_mutex);

	return _loginPage[_centralWidget->currentIndex()];

}

void QtLoginDialog::loginRejected() {
    _qtWengoPhone->getQtToolBar().enableMenuBar();
}

void QtLoginDialog::setInvalidAccount(const SipAccount & sipAccount) {
	if (sipAccount.getType() ==  SipAccount::SipAccountTypeWengo) {
		//wengo account
		changePage(AddWengoAccount);
		_loginPage[AddWengoAccount]->load(sipAccount);
	} else {
		//"generic" sip account
		changePage(AddSIPAccount);
		_loginPage[AddSIPAccount]->load(sipAccount);
	}
}

void QtLoginDialog::setValidAccount(SipAccount sipAccount) {
	changePage(LogAccount);
	_loginPage[LogAccount]->load(sipAccount);
	_loginPage[LogAccount]->setInfoMessage();
}

void QtLoginDialog::setInfoMessage() {
	_loginPage[_centralWidget->currentIndex()]->setInfoMessage();
}

void QtLoginDialog::setErrorMessage(const QString & message) {
	_loginPage[_centralWidget->currentIndex()]->setErrorMessage(message);
}
/**
* Add by: hexianfu
* get the current page index
* 2009-11-10
*/
//int QtLoginDialog::getCurrentPageIndex()
//{
//	return _centralWidget->currentIndex();
//}
//void QtLoginDialog::teleChangePageSlot(int newPage/*,const QString& sipAccountName*/)
//{
//   changePage(newPage/*,sipAccountName.toStdString()*/);
//}
//void QtLoginDialog::teleChangePage(int newPage/*,const std::string sipAccountName*/)
//{
//	teleChangePageSinal(newPage /*,QString::fromStdString(sipAccountName)*/);
//}
//
QtAddSIPAccount * QtLoginDialog::getAddSipAccountPage() const
{
	return _qtAddSipAccount;
}
/***************************/
