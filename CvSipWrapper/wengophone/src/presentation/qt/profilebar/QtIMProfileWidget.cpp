#include "stdafx.h"
/*
 * WengoPhone, a voice over Internet phone
 * Copyright (C) 2004-2006  Wengo
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

#include "QtIMProfileWidget.h"

#include "ui_IMProfileWidget.h"

#include "QtIMAccountPresenceMenuManager.h"

#include <presentation/qt/QtIMAccountMonitor.h>
#include <presentation/qt/QtIMAccountHelper.h>
#include <presentation/qt/profile/QtProfileDetails.h>
#include <presentation/qt/imaccount/QtIMAccountManager.h>
#include <presentation/PFactory.h>

#include <control/CWengoPhone.h>
#include <control/profile/CUserProfile.h>
#include <control/profile/CUserProfileHandler.h>

#include <model/profile/UserProfile.h>

#include <util/Logger.h>
#include <util/SafeDelete.h>
#include <thread/ThreadEvent.h>

#include <qtutil/LanguageChangeEventFilter.h>
#include <qtutil/SafeConnect.h>
#include <qtutil/PixmapMerging.h>

#include <QtGui/QHBoxLayout>
#include <QtGui/QMenu>
#include <QtGui/QPainter>

#include "imaccount/QtIMAccountSettings.h"

const int ACCOUNT_ICON_WIDTH = 28;
const int ACCOUNT_ICON_HEIGHT = 22;

static QPixmap createAccountButtonPixmap(EnumIMProtocol::IMProtocol imProtocol, QtIMAccountMonitor::ConnectionState connectionState, EnumPresenceState::PresenceState presenceState) {
	QString name = ":pics/protocols/";

	// Append protocol name
	switch (imProtocol) {
	case EnumIMProtocol::IMProtocolMSN:
		name += "msn";
		break;

	case EnumIMProtocol::IMProtocolYahoo:
		name += "yahoo";
		break;

	case EnumIMProtocol::IMProtocolAIM:
		name += "aim";
		break;

	case EnumIMProtocol::IMProtocolICQ:
		name += "icq";
		break;

	case EnumIMProtocol::IMProtocolJabber:
		name += "jabber";
		break;

	case EnumIMProtocol::IMProtocolWengo:
		name += "wengo";
		break;
	
	case EnumIMProtocol::IMProtocolSIPSIMPLE:
	case EnumIMProtocol::IMProtocolSIP:
		name += "sip";
		break;
	
	case EnumIMProtocol::IMProtocolUnknown:
	case EnumIMProtocol::IMProtocolAll:
		LOG_FATAL("Protocol should not be " + EnumIMProtocol::toString(imProtocol));
	}

	// Special cases: Disconnected or failure
	if (connectionState == QtIMAccountMonitor::StateDisconnected 
		|| connectionState == QtIMAccountMonitor::StateFailure) {
		name += "_off";
	}

	// Load pix
	name += ".png";
	QPixmap protocolPix;
	if (!protocolPix.load(name)) {
		LOG_WARN(QString("Could not load icon " + name).toStdString());
		return QPixmap();
	}

	// Load overlay
	QPixmap overlay;
	// Append connection state
	switch (connectionState) {
	case QtIMAccountMonitor::StateConnected:
		{
			QString presenceString = QString::fromStdString(EnumPresenceState::toString(presenceState));
			overlay.load(
				QString(":pics/protocols/overlay_presence_%1.png").arg(presenceString)
				);
		}
		break;

	case QtIMAccountMonitor::StateConnecting:
		overlay.load(":pics/protocols/overlay_progress.png");
		break;

	case QtIMAccountMonitor::StateFailure:
		overlay.load(":pics/protocols/overlay_error.png");
		break;

	case QtIMAccountMonitor::StateDisconnected:
		break;
	}

	// Paint overlay
	QPixmap pix(ACCOUNT_ICON_WIDTH, ACCOUNT_ICON_HEIGHT);
	pix.fill(QColor(0,0,0,0));
	QPainter painter(&pix);
	painter.drawPixmap(pix.width() - protocolPix.width(), 0, protocolPix);
	if (!overlay.isNull()) {
		painter.drawPixmap(0, pix.height() - overlay.height(), overlay);
	}
	painter.end();

	return pix;
}


QtIMProfileWidget::QtIMProfileWidget(QWidget * parent)
	: QObject(parent)
	, _cUserProfile(0)
	, _qtImAccountMonitor(0) {

	_imProfileWidget = new QWidget(parent);

	_ui = new Ui::IMProfileWidget();
	_ui->setupUi(_imProfileWidget);

	// Init addIMAccountButton
	// FIXME: This is copied from QtIMAccountManager ctor and should be
	// factorized
	QMenu * addIMAccountMenu = new QMenu(_ui->addIMAccountButton);
	SAFE_CONNECT(addIMAccountMenu, SIGNAL(triggered(QAction *)), SLOT(addIMAccount(QAction *)));

	addIMAccountMenu->addAction(QIcon(":pics/protocols/msn.png"),
				QtEnumIMProtocol::toString(QtEnumIMProtocol::IMProtocolMSN));
	addIMAccountMenu->addAction(QIcon(":pics/protocols/aim.png"),
				QtEnumIMProtocol::toString(QtEnumIMProtocol::IMProtocolAIM));
	addIMAccountMenu->addAction(QIcon(":pics/protocols/icq.png"),
				QtEnumIMProtocol::toString(QtEnumIMProtocol::IMProtocolICQ));
	addIMAccountMenu->addAction(QIcon(":pics/protocols/yahoo.png"),
				QtEnumIMProtocol::toString(QtEnumIMProtocol::IMProtocolYahoo));
	addIMAccountMenu->addAction(QIcon(":pics/protocols/jabber.png"),
				QtEnumIMProtocol::toString(QtEnumIMProtocol::IMProtocolJabber));
	addIMAccountMenu->addAction(QIcon(":pics/protocols/jabber.png"),
				QtEnumIMProtocol::toString(QtEnumIMProtocol::IMProtocolGoogleTalk));
	_ui->addIMAccountButton->setPopupMode(QToolButton::InstantPopup);
	_ui->addIMAccountButton->setMenu(addIMAccountMenu);
	////

	LANGUAGE_CHANGE(_imProfileWidget);

	//Widget connections
	SAFE_CONNECT(_ui->aliasLineEdit, SIGNAL(returnPressed()), SLOT(aliasTextChanged()));
	SAFE_CONNECT(_ui->avatarButton, SIGNAL(clicked()), SLOT(changeAvatarClicked()));

	//Init _accountFrame
	_accountFrame = new QFrame();
	_accountFrameLayout = new QHBoxLayout(_accountFrame);
	_accountFrameLayout->setMargin(0);
	_accountFrameLayout->setSpacing(0);

	// Change size constraint to make sure the frame get resized when we add
	// buttons to it
	_accountFrameLayout->setSizeConstraint(QLayout::SetFixedSize);
	_ui->accountScrollFrame->setChild(_accountFrame);
	_ui->accountScrollFrame->setScrollStep(ACCOUNT_ICON_WIDTH / 2);
	_imProfileWidget->setEnabled(false);
}

QtIMProfileWidget::~QtIMProfileWidget() {
	OWSAFE_DELETE(_ui);
}

void QtIMProfileWidget::init(CUserProfile* cUserProfile, QtIMAccountMonitor* qtImAccountMonitor) {
	_cUserProfile = cUserProfile;
	_qtImAccountMonitor = qtImAccountMonitor;

	//UserProfile changed event connection
	_cUserProfile->getUserProfile().profileChangedEvent +=
		boost::bind(&QtIMProfileWidget::profileChangedEventHandler, this);

	// IMAccountMonitor connections
	SAFE_CONNECT_TYPE(_qtImAccountMonitor, SIGNAL(imAccountAdded(QString)),
		SLOT(addAccountButton(QString)), Qt::QueuedConnection);
	SAFE_CONNECT_TYPE(_qtImAccountMonitor, SIGNAL(imAccountRemoved(QString)),
		SLOT(removeAccountButton(QString)), Qt::QueuedConnection);
	SAFE_CONNECT_TYPE(_qtImAccountMonitor, SIGNAL(imAccountUpdated(QString)),
		SLOT(updateAccountButton(QString)), Qt::QueuedConnection);

	_imProfileWidget->setEnabled(true);
	updateWidgets();
}

QWidget * QtIMProfileWidget::getWidget() const {
	return _imProfileWidget;
}

void QtIMProfileWidget::addAccountButton(QString imAccountId) {
	// Check the button does not already exists
	AccountIdToButton::const_iterator it = _accountIdToButton.find(imAccountId);
	if (it != _accountIdToButton.end()) {
		LOG_WARN("there is already a button for account " + imAccountId.toStdString());
		return;
	}

	// Look for the account
	IMAccount * imAccount =
		_cUserProfile->getUserProfile().getIMAccountManager().getIMAccount(imAccountId.toStdString());

	if (!imAccount) {
		LOG_WARN("Account does not exist");
		return;
	}

	// Recreate all buttons. This is a bit brutal, but adding an account is not
	// done very often, so it's not that bad.
	// If it becomes necessary to just add the button, the
	// addAccountButton(const IMAccount*) should be modified to insert the
	// button at the correct position
	createAccountButtons();
}

void QtIMProfileWidget::addAccountButton(const IMAccount* account) {
	// Create button

	QString id = QString::fromStdString(account->getUUID());
	AccountIdToButton::const_iterator it = _accountIdToButton.find(id);
	if (it != _accountIdToButton.end()) {
		LOG_WARN("there is already a button for account " + id.toStdString());
		return;
	}

	QToolButton* button = new QToolButton(_accountFrame);
	button->setAutoRaise(true);
	button->setIconSize(QSize(ACCOUNT_ICON_WIDTH, ACCOUNT_ICON_HEIGHT));
	button->setPopupMode(QToolButton::InstantPopup);

	// Add menu
	QMenu* menu = new QMenu(button);
	QAction* action = menu->addAction(QString::fromStdString(account->getLogin()));
	action->setEnabled(false);
	menu->addAction(action);
	menu->addSeparator();

	QtIMAccountPresenceMenuManager* manager = new QtIMAccountPresenceMenuManager(menu, *_cUserProfile, account->getUUID());
	manager->addPresenceActions(menu);
	button->setMenu(menu);

	// Integrate button
	_accountFrameLayout->addWidget(button);

	_accountIdToButton[id] = button;
	updateAccountButton(id);
}

void QtIMProfileWidget::removeAccountButton(QString imAccountId) {
	AccountIdToButton::iterator it = _accountIdToButton.find(imAccountId);
	if (it == _accountIdToButton.end()) {
		LOG_WARN("No button exists for account with id " + imAccountId.toStdString());
		return;
	}
	OWSAFE_DELETE(it->second);
	_accountIdToButton.erase(it);
	updateAddIMAccountButton();
}

void QtIMProfileWidget::updateAccountButton(QString imAccountId) {
	// Look for the corresponding button
	AccountIdToButton::const_iterator it = _accountIdToButton.find(imAccountId);
	if (it == _accountIdToButton.end()) {
		// Button does not exist
		LOG_WARN("There is no button for account " + imAccountId.toStdString());
		return;
	}
	QToolButton* button = it->second;

	// Look for the account
	IMAccount * imAccount =
		_cUserProfile->getUserProfile().getIMAccountManager().getIMAccount(imAccountId.toStdString());

	if (!imAccount) {
		// Account does not exist, this probably means the account has been
		// removed
		LOG_DEBUG("Account " + imAccountId.toStdString() + " does not exist");
		return;
	}

	// Get account info
	EnumIMProtocol::IMProtocol imProtocol = imAccount->getProtocol();
	QString protocolString = QString::fromStdString(EnumIMProtocol::toString(imProtocol));
	QString login = QString::fromStdString(imAccount->getLogin());
	EnumPresenceState::PresenceState presenceState = imAccount->getPresenceState();

	OWSAFE_DELETE(imAccount);

	QtIMAccountMonitor::IMAccountInfoAutoPtr info = _qtImAccountMonitor->getIMAccountInfo(imAccountId);
	QtIMAccountMonitor::ConnectionState connectionState = info->connectionState();
	QString message = info->message();
	// Init button
	QPixmap protocolPix = createAccountButtonPixmap(imProtocol, connectionState, presenceState);
	button->setIcon(protocolPix);

	QString toolTip = tr("%1 (%2)\n%3", "%1 is login, %2 is protocol, %3 is status")
		.arg(login)
		.arg(protocolString)
		.arg(message);
	button->setToolTip(toolTip);
}

void QtIMProfileWidget::changeAvatarClicked() {
	QtProfileDetails qtProfileDetails(*_cUserProfile, _cUserProfile->getUserProfile(), _imProfileWidget, tr("Edit My Profile"));
	//TODO UserProfile must be updated if QtProfileDetails was accepted
	qtProfileDetails.changeUserProfileAvatar();
	updateAvatar();
}

void QtIMProfileWidget::updateWidgets() {
	if (!_cUserProfile) {
		return;
	}

	if (!_cUserProfile->getUserProfile().getAlias().empty()) {
		_ui->aliasLineEdit->setText(QString::fromUtf8(_cUserProfile->getUserProfile().getAlias().c_str()));
	}

	//aliasTextChanged();

	updateAvatar();

	createAccountButtons();
}

void QtIMProfileWidget::createAccountButtons() {
	// Remove existing buttons
	resetAccountButton();

	// Sort accounts, using the same order as the meta presence
	IMAccountList imAccountList = _cUserProfile->getUserProfile().getIMAccountManager().getIMAccountListCopy();
	QtIMAccountHelper::QtIMAccountPtrVector imAccountPtrVector;
	QtIMAccountHelper::copyListToPtrVector(imAccountList, &imAccountPtrVector);
	std::sort(imAccountPtrVector.begin(), imAccountPtrVector.end(), QtIMAccountHelper::compareIMAccountPtrs);

	// Updates IMAccounts icons status
	QtIMAccountHelper::QtIMAccountPtrVector::const_iterator
		it = imAccountPtrVector.begin(),
		end = imAccountPtrVector.end();

	for (; it!=end; ++it) {
		addAccountButton(*it);
	}

	updateAddIMAccountButton();
}

// FIXME: This is copied from QtIMAccountManager ctor and should be
// factorized
void QtIMProfileWidget::addIMAccount(QAction * action) {
	QString protocolName = action->text();
	QtEnumIMProtocol::IMProtocol imProtocol = QtEnumIMProtocol::toIMProtocol(protocolName);
	QtIMAccountSettings(_cUserProfile->getUserProfile(), imProtocol, _imProfileWidget);
}

void QtIMProfileWidget::updateAvatar() {
	std::string backgroundPixmapFilename = ":/pics/avatar_background.png";
	std::string foregroundPixmapData = _cUserProfile->getUserProfile().getIcon().getData();

	_ui->avatarButton->setIcon(PixmapMerging::merge(foregroundPixmapData, backgroundPixmapFilename));
}

void QtIMProfileWidget::updateAddIMAccountButton() {
	IMAccountList imAccountList = _cUserProfile->getUserProfile().getIMAccountManager().getIMAccountListCopy();
	if (imAccountList.size() >= 2) {
		_ui->addIMAccountButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
	} else {
		_ui->addIMAccountButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	}
	
}

void QtIMProfileWidget::showImAccountManager() {
	QtIMAccountManager imAccountManager(_cUserProfile->getUserProfile(),
		true, _imProfileWidget);
}

void QtIMProfileWidget::aliasTextChanged() {
	//Update alias text
	std::string alias(_ui->aliasLineEdit->text().toUtf8().constData());
	_cUserProfile->getUserProfile().setAlias(alias, NULL);

	_ui->aliasLineEdit->update();
}

void QtIMProfileWidget::profileChangedEventHandler() {
	typedef ThreadEvent0<void ()> MyThreadEvent;
	MyThreadEvent * event = new MyThreadEvent(boost::bind(&QtIMProfileWidget::updateWidgets, this));
	PFactory::postEvent(event);
}

void QtIMProfileWidget::languageChanged() {
	_ui->retranslateUi(_imProfileWidget);
	updateWidgets();
}

void QtIMProfileWidget::resetAccountButton() 
{
	AccountIdToButton::iterator
		buttonIt = _accountIdToButton.begin(),
		buttonEnd = _accountIdToButton.end();
	for (; buttonIt!=buttonEnd; ++buttonIt) {
		delete buttonIt->second;
	}
	_accountIdToButton.clear();
}

void QtIMProfileWidget::reset() 
{
	resetAccountButton();

	_cUserProfile = 0;
	_qtImAccountMonitor = 0;
}

HWND QtIMProfileWidget::getAvatarButtonWnd()
{
	return _ui->avatarButton->winId();
}
