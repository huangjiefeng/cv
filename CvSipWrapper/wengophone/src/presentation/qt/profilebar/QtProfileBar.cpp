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

#include "QtProfileBar.h"

//#include "QtIMProfileWidget.h"		精简 zhenHua.sun 2010-09-03
#include "QtEventWidget.h"
//#include "QtCreditWidget.h"			精简 zhenHua.sun 2010-10-29

#include <presentation/qt/QtIMAccountHelper.h>
#include <presentation/qt/QtIMAccountMonitor.h>
#include <presentation/qt/QtPresenceMenuManager.h>
#include <presentation/qt/QtPresencePixmapHelper.h>
#include <presentation/qt/QtWengoPhone.h>
#include <presentation/qt/profile/QtUserProfile.h>
#include <presentation/qt/profile/QtProfileDetails.h>

#include <control/CWengoPhone.h>
#include <control/history/CHistory.h>
#include <control/profile/CUserProfile.h>
#include <control/profile/CUserProfileHandler.h>

#include <model/account/wengo/WengoAccount.h>
#include <model/connect/ConnectHandler.h>
#include <model/profile/AvatarList.h>
#include <model/profile/UserProfile.h>

#include <imwrapper/IMAccount.h>

#include <util/Logger.h>

#include <qtutil/WengoStyleLabel.h>
#include <qtutil/SafeConnect.h>

#include <QtGui/QApplication>
#include <QtGui/QGridLayout>
#include <QtGui/QMenu>
#include <QtGui/QPainter>
#include <QtCore/QMetaType>

static const char* STATUS_LABEL_OFF_TEMPLATE = ":/pics/profilebar/bar_start_status_%1.png";
static const char* STATUS_LABEL_ON_TEMPLATE = ":/pics/profilebar/bar_on_start_status_%1.png";

// These constants are used to find the position of the state "ball" in the
// profilebar pixmap
static const int STATUS_LABEL_CENTERX = 26;
static const int STATUS_LABEL_CENTERY = 32;
static const int STATUS_LABEL_RADIUS  = 10;

// Defines position of the expand indicator in the picture. used in
// createPixmapWithExpandIndicator
static const int EXPAND_INDICATOR_OFFSET = 4;


/**
 * This helper functions blit an expand indicator (or contract if @expand is
 * false) over the pixmap whose name is @name
 */
static QPixmap createPixmapWithExpandIndicator(const QString& name, bool expand) {
	QPixmap bg(":/pics/profilebar/" + name);

	QString indicatorName;
	if (expand) {
		indicatorName = "indicator_expand.png";
	} else {
		indicatorName = "indicator_contract.png";
	}
	QPixmap indicator(":/pics/profilebar/" + indicatorName);

	QPainter painter(&bg);

	int top = (bg.height() - indicator.height()) / 2;
	painter.drawPixmap(EXPAND_INDICATOR_OFFSET, top, indicator);

	return bg;
}


QtProfileBar::QtProfileBar(QWidget * parent)
	: QFrame(parent),
	_cUserProfile(0),
	_cWengoPhone(0),
	_qtImAccountMonitor(0) {

	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	mainLayout->setMargin(0);
	mainLayout->setSpacing(0);

	//The layout containing our three "labels" (status, nickname and credit)
	QHBoxLayout* labelLayout = new QHBoxLayout();
	labelLayout->setMargin(0);
	labelLayout->setSpacing(0);

	mainLayout->addLayout(labelLayout);

	//The status widget
	_statusLabel = new WengoStyleLabel(this);
	_statusLabel->setToolTip(tr("Click here to change your status"));
	_statusLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

	//Nickname label
	_nicknameLabel = new WengoStyleLabel(this, WengoStyleLabel::Normal);
#ifdef DISABLE_VOICE_MAIL
	_nicknameLabel->setPixmaps(
					QPixmap(), //no start
					createPixmapWithExpandIndicator("bar_end.png", true), //end
					//createPixmapWithExpandIndicator("bar_separator.png", true), //end
					QPixmap(":/pics/profilebar/bar_fill.png"), //fill
					QPixmap(),  //no start
					//createPixmapWithExpandIndicator("bar_on_separator_left.png", false), //end
					createPixmapWithExpandIndicator("bar_end.png", false), //end
					QPixmap(":/pics/profilebar/bar_on_fill.png") //fill
					);
#else	
	_nicknameLabel->setPixmaps(
					QPixmap(), //no start
					createPixmapWithExpandIndicator("bar_separator.png", true), //end
					QPixmap(":/pics/profilebar/bar_fill.png"), //fill
					QPixmap(),  //no start
					createPixmapWithExpandIndicator("bar_on_separator_left.png", false), //end
					QPixmap(":/pics/profilebar/bar_on_fill.png") //fill
					);
#endif
	_nicknameLabel->setTextColor(Qt::white);

	//The credit label
	_creditLabel = new WengoStyleLabel(this);
	_creditLabel->setPixmaps(QPixmap(), //no start
					createPixmapWithExpandIndicator("bar_end.png", true),
					QPixmap(":/pics/profilebar/bar_fill.png"),
					QPixmap(),  //no start
					createPixmapWithExpandIndicator("bar_on_end.png", false),
					QPixmap(":/pics/profilebar/bar_on_fill.png")
					);
	_creditLabel->setTextColor(Qt::white);

	//Add the labels
	labelLayout->addWidget(_statusLabel);
	labelLayout->addWidget(_nicknameLabel);
	labelLayout->addWidget(_creditLabel);

	// 精简 zhenHua.sun 2010-09-03
	////create internal widgets
	//_qtImProfileWidget = new QtIMProfileWidget(this);

	///************************************************************************///Modify by zhuang 09-01-07
	///*��ʼ��ʱ��Ĭ����ʾ                                                    */
	//_qtImProfileWidget->getWidget()->setVisible(false);
	///************************************************************************/
	//_qtImProfileWidget->getWidget()->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	//mainLayout->addWidget(_qtImProfileWidget->getWidget());

/** 精简   zhenHua.sun 2010-10-29
	_creditWidget = new QtCreditWidget(this);
	_creditWidget->getWidget()->setVisible(false);
	_creditWidget->getWidget()->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	mainLayout->addWidget(_creditWidget->getWidget());

	//init label tracking vars
	_currentLabel = 0;
	//_labelWidget[_nicknameLabel] = _qtImProfileWidget->getWidget();精简 zhenHua.sun 2010-09-03
	_labelWidget[_creditLabel] = _creditWidget->getWidget();
	////
*/
	//Objects connection
	SAFE_CONNECT(_statusLabel, SIGNAL(clicked()), SLOT(statusClicked()));
	SAFE_CONNECT(_nicknameLabel, SIGNAL(clicked()), SLOT(nicknameClicked()));
	SAFE_CONNECT(_creditLabel, SIGNAL(clicked()), SLOT(creditClicked()));

	qRegisterMetaType<IMAccount>("IMAccount");

	SAFE_CONNECT_TYPE(this, SIGNAL(wsInfoWengosEvent(float)),
		SLOT(wsInfoWengosEventSlot(float)), Qt::QueuedConnection);
	SAFE_CONNECT_TYPE(this, SIGNAL(wsInfoVoiceMailEvent(int)),
		SLOT(wsInfoVoiceMailEventSlot(int)), Qt::QueuedConnection);
	SAFE_CONNECT_TYPE(this, SIGNAL(wsInfoLandlineNumberEvent(const QString &)),
		SLOT(wsInfoLandlineNumberEventSlot(const QString &)), Qt::QueuedConnection);
	SAFE_CONNECT_TYPE(this, SIGNAL(wsCallForwardInfoEvent(const QString &)),
		SLOT(wsCallForwardInfoEventSlot(const QString &)), Qt::QueuedConnection);
	SAFE_CONNECT_TYPE(this, SIGNAL(phoneLineCreatedEvent()),
		SLOT(phoneLineCreatedEventSlot()), Qt::QueuedConnection);
	
#ifdef DISABLE_VOICE_MAIL
	_creditLabel->hide();
#endif

	reset();
	/************************************************************************///Add by zhuang 09-01-07
	/*��ʼ��ʱ��Ĭ����ʾ                                                    */
	//_nicknameLabel->setSelected(true);
	//_labelWidget[_nicknameLabel]->show();
	//_currentLabel = _nicknameLabel;
	//����״̬�� by zhuang 2009-11-18
	hide();
	/************************************************************************/
}

QtProfileBar::~QtProfileBar() {
}

void QtProfileBar::init(CWengoPhone* cWengoPhone, CUserProfile* cUserProfile, QtIMAccountMonitor* qtImAccountMonitor) {
	_cWengoPhone = cWengoPhone;
	_cUserProfile = cUserProfile;
	_qtImAccountMonitor = qtImAccountMonitor;

	// Init creditLabel
	if (_cUserProfile->getUserProfile().hasWengoAccount()) {
		_creditLabel->setText(QChar(0x20ac) + QString(" 0.00"));
	} else {
		_creditLabel->setText(tr("Voice mail"));
	}

	// Init widgets
	//_qtImProfileWidget->init(_cUserProfile, _qtImAccountMonitor);	精简 zhenHua.sun 2010-09-03
	//_creditWidget->init(cWengoPhone, &_cUserProfile->getUserProfile());	精简 zhenHua.sun 2010-10-29

	// Connect _cUserProfile
	_cUserProfile->getUserProfile().wsInfoCreatedEvent +=
		boost::bind(&QtProfileBar::wsInfoCreatedEventHandler, this, _1, _2);

	_cUserProfile->getUserProfile().phoneLineCreatedEvent +=
		boost::bind(&QtProfileBar::phoneLineCreatedEventHandler, this, _1, _2);

	// Connect qtImAccountMonitor
	SAFE_CONNECT_TYPE(_qtImAccountMonitor, SIGNAL(imAccountAdded(QString)),
		SLOT(updateStatusLabel()), Qt::QueuedConnection);
	SAFE_CONNECT_TYPE(_qtImAccountMonitor, SIGNAL(imAccountRemoved(QString)),
		SLOT(updateStatusLabel()), Qt::QueuedConnection);
	SAFE_CONNECT_TYPE(_qtImAccountMonitor, SIGNAL(imAccountUpdated(QString)),
		SLOT(updateStatusLabel()), Qt::QueuedConnection);

	//Check if events already occured
	//FIXME: must not use model class
	if (_cUserProfile->getUserProfile().getActivePhoneLine()) {
		phoneLineCreatedEventHandler(_cUserProfile->getUserProfile(), *_cUserProfile->getUserProfile().getActivePhoneLine());
	}

	if (_cUserProfile->getUserProfile().getWsInfo()) {
		wsInfoCreatedEventHandler(_cUserProfile->getUserProfile(), *_cUserProfile->getUserProfile().getWsInfo());
	}
	////

	updateStatusLabel();
	setEnabled(true);
}

void QtProfileBar::reset() {
	setEnabled(false);
	_cUserProfile = 0;
	_qtImAccountMonitor = 0;

	_statusLabel->setPixmaps(
					QPixmap(":/pics/profilebar/bar_start_status_offline.png"),
					QPixmap(),
					QPixmap(),
					QPixmap(":/pics/profilebar/bar_on_start_status_offline.png"),
					QPixmap(),
					QPixmap()
					);

	_nicknameLabel->setText("");
	_creditLabel->setText("");

	//_qtImProfileWidget->reset();		精简 zhenHua.sun 2010-09-03
}

void QtProfileBar::updateStatusLabel() {
	if(_cUserProfile)
	{
	// Create pixmap
		IMAccountList accountList = _cUserProfile->getUserProfile().getIMAccountManager().getIMAccountListCopy();
		QtPresencePixmapHelper helper(accountList);
		QPixmap off = helper.createPixmap(STATUS_LABEL_OFF_TEMPLATE, 
			STATUS_LABEL_CENTERX, 
			STATUS_LABEL_CENTERY,
			STATUS_LABEL_RADIUS);
		QPixmap on = helper.createPixmap(STATUS_LABEL_ON_TEMPLATE,
			STATUS_LABEL_CENTERX, 
			STATUS_LABEL_CENTERY,
			STATUS_LABEL_RADIUS);

		_statusLabel->setPixmaps(off,
						QPixmap(), //no end
						QPixmap(), //no fill
						on,
						QPixmap(),  //no end
						QPixmap()
						); //no fill
		_statusLabel->update();

		// Sort account using the same order as in the status pixmap
		QtIMAccountHelper::QtIMAccountPtrVector vector;
		QtIMAccountHelper::copyListToPtrVector(accountList, &vector);
		std::sort(vector.begin(), vector.end(), QtIMAccountHelper::compareIMAccountPtrs);
		////

		// Create tooltip
		QStringList tooltipLines;
		QtIMAccountHelper::QtIMAccountPtrVector::const_iterator
			it = vector.begin(),
			end = vector.end();

		QList<std::string> list;

		for (;it!=end; ++it) 
		{
			const IMAccount* imAccount = *it;
			if(!list.contains(imAccount->getUUID()))
			{
				QString login = QString::fromStdString(imAccount->getLogin());
				QString protocol = QString::fromStdString( EnumIMProtocol::toString(imAccount->getProtocol()) );
				QString id = QString::fromStdString(imAccount->getUUID());
				QtIMAccountMonitor::IMAccountInfoAutoPtr info = _qtImAccountMonitor->getIMAccountInfo(id);

				if(info.get())
				{
					QString line = QString("<b>%1 (%2):</b> %3").arg(login).arg(protocol).arg(info->message());
					tooltipLines.append(line);
				}
				list.append(imAccount->getUUID());
			}
		}
		_statusLabel->setToolTip(tooltipLines.join("<br>\n"));
	}
}

void QtProfileBar::statusClicked() {

	// no presence when user got a SIPAccount without presence and no IMAccount
	if (_cUserProfile) {
	
		UserProfile & userProfile = _cUserProfile->getUserProfile();
		SipAccount * sipAccount = userProfile.getSipAccount();
		
		if (userProfile.getIMAccountManager().getIMAccountListCopy().size() <= 1) {
			if (!sipAccount || !sipAccount->isPIMEnabled()) {
				return;
			}
		}
	}
	////
	
	// by default we crete the menu
	createStatusMenu();
}

void QtProfileBar::nicknameClicked() {
	toggleLabel(_nicknameLabel);
}

void QtProfileBar::creditClicked() {
	toggleLabel(_creditLabel);
}

void QtProfileBar::toggleLabel(WengoStyleLabel* label) {
	setUpdatesEnabled(false);
	Q_ASSERT(_cUserProfile);
	Q_ASSERT(label);
	if (_currentLabel) {
		// Hide current
		_currentLabel->setSelected(false);
		_labelWidget[_currentLabel]->hide();
	}

	if (label == _currentLabel) {
		// Clicked on current, no more _currentLabel
		_currentLabel = 0;
	} else {
		// Clicked on another label, there is a new _currentLabel
		label->setSelected(true);
		Q_ASSERT(_labelWidget.contains(label));
		
		// Make sure all internal widgets are at the same height
		// Only do so for wengo accounts because in the case of a SIP account,
		// the credit widget is quite empty
		if (_cUserProfile->getUserProfile().hasWengoAccount()) {
			//int imProfileHeight = _qtImProfileWidget->getWidget()->minimumSizeHint().height();	精简 zhenHua.sun 2010-09-03
			//int creditHeight = _creditWidget->getWidget()->minimumSizeHint().height();
			//int height = qMax(imProfileHeight, creditHeight);
			//_qtImProfileWidget->getWidget()->setFixedHeight(height);	精简 zhenHua.sun 2010-09-03
			//_creditWidget->getWidget()->setFixedHeight(creditHeight);		zhenHua.sun 2010-10-29
		}
		
		_labelWidget[label]->show();
		_currentLabel = label;
	}

	setUpdatesEnabled(true);
}

void QtProfileBar::createStatusMenu() {
	QMenu* menu = new QMenu(this);
	QtPresenceMenuManager* manager = new QtPresenceMenuManager(menu, 
		&_cWengoPhone->getCUserProfileHandler(),
		_cWengoPhone->_qtWengoPhone//Modify By LZY 2010-09-28
	);

	QAction* action = menu->addAction(tr("Set global presence to:"));
	action->setEnabled(false);
	manager->addPresenceActions(menu);

	QPoint p = _statusLabel->pos();
	p.setY(p.y() + _statusLabel->rect().bottom() - 18);
	menu->popup(mapToGlobal(p));
}

void QtProfileBar::wsInfoCreatedEventHandler(UserProfile & sender, WsInfo & wsInfo) {
	wsInfo.wsInfoWengosEvent += boost::bind(&QtProfileBar::wsInfoWengosEventHandler, this, _1, _2, _3, _4);
	wsInfo.wsInfoVoiceMailEvent += boost::bind(&QtProfileBar::wsInfoVoiceMailEventHandler, this, _1, _2, _3, _4);
	wsInfo.wsInfoLandlineNumberEvent += boost::bind(&QtProfileBar::wsInfoLandlineNumberEventHandler, this, _1, _2, _3, _4);
	wsInfo.wsCallForwardInfoEvent += boost::bind(&QtProfileBar::wsCallForwardInfoEventHandler, this, _1, _2, _3, _4, _5, _6, _7, _8);

	wsInfo.getWengosCount(true);
	wsInfo.getUnreadVoiceMail(true);
	wsInfo.getCallForwardInfo(true);
	wsInfo.getLandlineNumber(true);
	wsInfo.execute();
}

void QtProfileBar::wsInfoWengosEventHandler(WsInfo & sender, int id, WsInfo::WsInfoStatus status, float wengos) {
	if (status == WsInfo::WsInfoStatusOk) {
		wsInfoWengosEvent(wengos);
	}
}

void QtProfileBar::wsInfoVoiceMailEventHandler(WsInfo & sender, int id, WsInfo::WsInfoStatus status, int voicemail) {
	if (status == WsInfo::WsInfoStatusOk) {
		wsInfoVoiceMailEvent(voicemail);
	}
}

void QtProfileBar::wsInfoLandlineNumberEventHandler(WsInfo & sender, int id, WsInfo::WsInfoStatus status, std::string number) {
	if (status == WsInfo::WsInfoStatusOk) {
		wsInfoLandlineNumberEvent(QString::fromStdString(number));
	}
}

void QtProfileBar::wsCallForwardInfoEventHandler(WsInfo & sender, int id, WsInfo::WsInfoStatus status,
	WsInfo::WsInfoCallForwardMode mode, bool voicemail, std::string dest1, std::string dest2, std::string dest3) {

	if (status == WsInfo::WsInfoStatusOk) {

		switch(mode) {
		case WsInfo::WsInfoCallForwardModeVoicemail:
			wsCallForwardInfoEvent(tr("active") + " (" + tr("voicemail") + ")");
			break;
		case WsInfo::WsInfoCallForwardModeNumber:
			wsCallForwardInfoEvent(tr("active") + " (" + QString::fromStdString(dest1) + ")");
			break;
		case WsInfo::WsInfoCallForwardModeDisabled:
			wsCallForwardInfoEvent(tr("inactive"));
			break;
		case WsInfo::WsInfoCallForwardModeUnauthorized:
			wsCallForwardInfoEvent(tr("unauthorized"));
			break;
		}
	}
}

void QtProfileBar::phoneLineCreatedEventHandler(UserProfile & sender, IPhoneLine & phoneLine) {
	phoneLineCreatedEvent();
}

void QtProfileBar::phoneLineCreatedEventSlot() {
	if(_cUserProfile)
	{
		_nicknameLabel->setText("  " + QString::fromLocal8Bit(_cUserProfile->getUserProfile().getSipAccount()->getDisplayName().c_str()));
	}
}

void QtProfileBar::paintEvent(QPaintEvent * event) {
	if (_currentLabel) {
		QRect r = rect();

		QLinearGradient lg(QPointF(1, r.top()), QPointF(1, r.bottom()));

		lg.setColorAt(0, palette().color(QPalette::Window));
		QColor dest = palette().color(QPalette::Window);

		float red = ((float) dest.red()) / 1.3f;
		float blue = ((float) dest.blue()) / 1.3f;
		float green = ((float) dest.green()) / 1.3f;

		dest = QColor((int) red, (int) green, (int) blue);
		lg.setColorAt(1, dest);

		QPainter painter(this);
		painter.fillRect(r, QBrush(lg));
		painter.end();
	} else {
		QFrame::paintEvent(event);
	}
}

void QtProfileBar::wsInfoWengosEventSlot(float wengos) {
	//0x20ac is the unicode code for the euros currency symbol
	_creditLabel->setText(QString() + QChar(0x20ac) + QString(" %1").arg(wengos));
}

void QtProfileBar::wsInfoVoiceMailEventSlot(int count) {
	// FIXME: Show VoiceMail info somewhere
}

void QtProfileBar::wsInfoLandlineNumberEventSlot(const QString & number) {
	// FIXME: Show landline number somewhere
}

void QtProfileBar::wsCallForwardInfoEventSlot(const QString & mode) {
	//_creditWidget->setCallForwardMode(mode);
}
