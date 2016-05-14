#include "stdafx.h"
#include ".\qtconferenceinvite.h"

#include <presentation/qt/contactlist/QtContactMenu.h>

#include <model/phoneline/IPhoneLine.h>
#include <model/phonecall/ConferenceCall.h>

#include <util/SafeDelete.h>
#include <util/Logger.h>
#include <qtutil/WidgetBackgroundImage.h>
#include <qtutil/SafeConnect.h>
#include "QtWengoPhone.h"
#include <control/profile/CUserProfile.h>
#include <control/profile/CUserProfileHandler.h>
#include <control/CWengoPhone.h>
#include <QtGui/QtGui>

QtConferenceInvite::QtConferenceInvite(QWidget * parent, CWengoPhone & cWengoPhone, int &callId,bool startConference)
: QDialog(parent),  _cWengoPhone(cWengoPhone),_callId(callId),_startConference(startConference)  {

	_currentPeer = 1;

	_ui = new Ui::ConferenceInvite();
	_ui->setupUi(this);

	_menu = new QMenu(this);
	_wengoMenu = new QMenu(_menu);
	_wengoMenu->setTitle(tr("Call @company@"));
	_sipMenu = new QMenu(_menu);
	_sipMenu->setTitle(tr("Call SIP"));
	_landLineMenu = new QMenu(_menu);
	_landLineMenu->setTitle(tr("Call land line"));
	_mobileMenu = new QMenu(_menu);
	_mobileMenu->setTitle(tr("Call Mobile"));
	//_menu->addMenu(_wengoMenu);
	//_menu->addMenu(_landLineMenu);
	//_menu->addMenu(_mobileMenu);
	_menu->addMenu(_sipMenu);

	WidgetBackgroundImage::setBackgroundImage(_ui->headerLabel, ":/pics/conference/setup_banner.jpg", WidgetBackgroundImage::AdjustSize);

	SAFE_CONNECT(_ui->startButton, SIGNAL(clicked()), SLOT(startClicked()));
	SAFE_CONNECT(_ui->cancelButton, SIGNAL(clicked()), SLOT(reject()));
	SAFE_CONNECT(_ui->peer1ToolButton, SIGNAL(clicked()), SLOT(peer1ToolButtonClicked()));

	SAFE_CONNECT(_wengoMenu, SIGNAL(triggered(QAction *)), SLOT(updateLineEdit(QAction *)));
	SAFE_CONNECT(_landLineMenu, SIGNAL(triggered(QAction *)), SLOT(updateLineEdit(QAction *)));
	SAFE_CONNECT(_mobileMenu, SIGNAL(triggered(QAction *)), SLOT(updateLineEdit(QAction *)));
	SAFE_CONNECT(_sipMenu, SIGNAL(triggered(QAction *)), SLOT(updateLineEdit(QAction *)));
}

QtConferenceInvite::~QtConferenceInvite() {
	OWSAFE_DELETE(_ui);
}

void QtConferenceInvite::setFirstPeer(std::string peer) {
	_ui->phoneNumber1LineEdit->setText(QString::fromStdString(peer));
	_ui->phoneNumber1LineEdit->setReadOnly(true);
}

void QtConferenceInvite::startClicked() {

	if ((!_ui->phoneNumber1LineEdit->text().isEmpty())) 
	{

		if(_startConference)
		{
			CUserProfile * cUserProfile = _cWengoPhone.getCUserProfileHandler().getCUserProfile();
			QStringList inviteList = _ui->phoneNumber1LineEdit->text().split('/');
			QString szContent = "<?xml version=\"1.0\"?><coolview command=\"Invite\">";
			for (int i=0;i<inviteList.count();i++)
			{
				szContent = szContent+"<uri>"+inviteList[i]+"</uri>";
			}
			szContent += "</coolview>";
			
			char *body = new char[szContent.length()+1];
			strcpy(body,szContent.toStdString().c_str());
			cUserProfile->makeInfoCall(_callId,body);
		}
		accept();

	} else {
		if (_ui->phoneNumber1LineEdit->text().isEmpty()) {
			_ui->phoneNumber1LineEdit->setFocus();
		}
	}
}


void QtConferenceInvite::peer1ToolButtonClicked() {
	_currentPeer = 1;
	showMenu(_ui->peer1ToolButton->pos());
}


void QtConferenceInvite::showMenu(QPoint point) {
	point.setX(point.x());
	populateMenus();
	_menu->popup(mapToGlobal(point));
}

void QtConferenceInvite::populateMenus() {
	_sipMenu->clear();
	_wengoMenu->clear();
	_landLineMenu->clear();
	_mobileMenu->clear();

	//QtContactMenu::populateMobilePhoneMenu(_mobileMenu, _cWengoPhone);
	//QtContactMenu::populateWengoCallMenu(_wengoMenu, _cWengoPhone);
	//QtContactMenu::populateHomePhoneMenu(_landLineMenu, _cWengoPhone);
	QtContactMenu::populateSipCallMenu(_sipMenu, _cWengoPhone);
}

void QtConferenceInvite::updateLineEdit(QAction * action) {
	if (action) {
		QString data = action->data().toString();
		QString InviteList = _ui->phoneNumber1LineEdit->text();
		switch (_currentPeer) {
		case 1:
			if(InviteList.length()>0)
				InviteList += "/";
			InviteList += data;
			_ui->phoneNumber1LineEdit->setText(InviteList);
			break;
		default:
			LOG_FATAL("Unknown _currentPeer");
		}
	}
}