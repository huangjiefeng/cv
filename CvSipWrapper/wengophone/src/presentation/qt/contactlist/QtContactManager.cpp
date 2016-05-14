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

#include "QtContactManager.h"

#include "QtContactWidget.h"
#include "QtContactList.h"
#include "QtContactPixmap.h"
#include "QtContactInfo.h"
#include "QtContactListManager.h"
#include "QtContactTreeKeyFilter.h"
#include "QtConferenceAction.h"

#include <presentation/qt/QtWengoPhone.h>
#include <presentation/qt/profile/QtProfileDetails.h>

#include <control/CWengoPhone.h>
#include <control/contactlist/CContactList.h>
#include <control/profile/CUserProfile.h>
#include <control/profile/CUserProfileHandler.h>

#include <model/config/Config.h>
#include <model/config/ConfigManager.h>
#include <model/phoneline/PhoneLine.h>
#include <model/phonecall/PhoneCall.h>
#include <model/profile/UserProfile.h>

#include <sipwrapper/EnumPhoneCallState.h>

#include <util/WebBrowser.h>
#include <util/Logger.h>
#include <util/SafeDelete.h>

#include <qtutil/SafeConnect.h>
#include <qtutil/LanguageChangeEventFilter.h>

#include <QtGui/QtGui>

QtContactManager::QtContactManager(CUserProfile & cUserProfile, CWengoPhone & cWengoPhone,
	QtContactList & qtContactList, QObject * parent, QTreeWidget * target)
	: QObject(parent),
	_cWengoPhone(cWengoPhone),
	_cUserProfile(cUserProfile),
	_qtContactList(qtContactList) {

	LANGUAGE_CHANGE(this);
	languageChanged();

	_tree = target;
	_previous = NULL;
	_lastClicked = NULL;
	_sortContacts = true;
	_menu = NULL;
	_timerId = -1;
	_sortTimerId = -1;
	_showTimerId = -1;
	_waitForDoubleClick = false;
	_canSort = true;
	_wantSort = false;
	_canShow = true;
	_wantShow = false;

	QtContactListManager::getInstance()->setTreeWidget(target);
	target->setMouseTracking(true);
	QtContactTreeKeyFilter * keyFilter = new QtContactTreeKeyFilter(this, target);

	SAFE_CONNECT(target, SIGNAL(itemSelectionChanged()), SLOT(treeViewSelectionChanged()));
	SAFE_CONNECT(target, SIGNAL(itemClicked(QTreeWidgetItem *, int)), SLOT(itemClicked(QTreeWidgetItem *, int)));

	SAFE_CONNECT(keyFilter, SIGNAL(openItem(QTreeWidgetItem *)), SLOT(openContactInfo(QTreeWidgetItem *)));
	SAFE_CONNECT(keyFilter, SIGNAL(closeItem(QTreeWidgetItem *)), SLOT(closeContactInfo()));
	SAFE_CONNECT(keyFilter, SIGNAL(deleteItem(QTreeWidgetItem *)), SLOT(deleteContact()));
	SAFE_CONNECT(keyFilter, SIGNAL(enterPressed(QTreeWidgetItem *)),SLOT(defaultAction(QTreeWidgetItem *)));

	QtWengoPhone * qtWengoPhone = QtContactManager::_cWengoPhone._qtWengoPhone;//Modify By LZY 2010-09-28

	SAFE_CONNECT_RECEIVER(this, SIGNAL(inviteToConferenceClicked(QString, PhoneCall *)),
		qtWengoPhone, SLOT(addToConference(QString, PhoneCall *)));
}

void QtContactManager::startSMS() {
	QtContactListManager * ul = QtContactListManager::getInstance();
	QTreeWidgetItem * item = _tree->currentItem();
	if (ul && item) {
		ul->startSMS(item->text(0));
	}
}

void QtContactManager::startChat() {
	QtContactListManager * ul = QtContactListManager::getInstance();
	QTreeWidgetItem * item = _tree->currentItem();
	if (ul && item) {
		ul->startChat(item->text(0));
	}
}

void QtContactManager::sendFile() {
	QtContactListManager * ul = QtContactListManager::getInstance();
	QTreeWidgetItem * item = _tree->currentItem();
	if (ul && item) {
		ul->sendFile(item->text(0));
	}
}

void QtContactManager::editContact() {
	QTreeWidgetItem * item = _tree->currentItem();
	if (item) {
		editContact(item->text(0));
	}
}

void QtContactManager::editContact(QString contactId) {
	ContactProfile contactProfile = _cUserProfile.getCContactList().getContactProfile(contactId.toStdString());
	QtProfileDetails qtProfileDetails(_cUserProfile, contactProfile, _tree, tr("Edit Contact"));
	if (qtProfileDetails.show()) {
		_cUserProfile.getCContactList().updateContact(contactProfile);
	}
}

void QtContactManager::deleteContact() {
	QTreeWidgetItem * item = _tree->currentItem();
	if (!item) {
		return;
	}

	if (QMessageBox::question(_tree,
		tr("Delete contact"),
		tr("Do you really want to delete this contact?"),
		tr("&Yes"),
		tr("&No"),
		QString(),
		0, 1) == 0) {

		_cUserProfile.getCContactList().removeContact(item->text(0).toStdString());
	}
}

void QtContactManager::treeViewSelectionChanged() {
	if (_button == Qt::NoButton) {
		closeContactInfo();
	}
}

void QtContactManager::closeContactInfo() {
	if (_previous) {
		if (!_previous->parent()) {
			//It's a group
			return;
		}

		_tree->closePersistentEditor(_previous);
		QtContactListManager * ul = QtContactListManager::getInstance();
		ul->setOpenStatus(_previous->text(0), false);
		_previous->setSizeHint(0, QSize(-1, ul->getHeight(_previous->text(0))));
		_previous = NULL;

		_tree->viewport()->update();
		_tree->doItemsLayout();
	}
}

void QtContactManager::openContactInfo(QTreeWidgetItem * item) {
	if (_previous) {
		closeContactInfo();
	}

	if (!item) {
		return;
	}

	if (item->parent()) {

		QtContactListManager * ul = QtContactListManager::getInstance();

		//Does not expand if contact is part of the WDeal contact group
		QtContact * qtContact = ul->getContact(item->text(0));
		if (qtContact->getGroupName() == "WDeal") {
			return;
		}
		///

		_previous = item;
		ul->setOpenStatus(_previous->text(0), true);
		item->setSizeHint(0, QSize(-1, ul->getHeight(item->text(0))));
		_tree->openPersistentEditor(item);
		_tree->viewport()->update();
		_tree->doItemsLayout();
		_tree->scrollToItem (item);
	}
}

void QtContactManager::itemClicked(QTreeWidgetItem * item, int) {

	if ((_lastClicked == item) && (_waitForDoubleClick)) {
		//defaultAction(item);		将鼠标的双击操作进行了屏蔽zhenHua.sun 2010-09-06
		_button = Qt::NoButton;		
		return;
	}

	_waitForDoubleClick = true;
	if (_timerId != -1) {
		killTimer(_timerId);
	}
	_timerId = startTimer(qApp->doubleClickInterval ());

	_lastClicked = item;
	QRect widgetSize = _tree->rect();
	QPoint mousepos = _tree->mapFromGlobal(QCursor::pos());

	if (!item->parent()) {
		groupClicked(item);
		return;
	}

	QtContactListManager * ul = QtContactListManager::getInstance();
	if (ul->getButton(item->text(0)) == Qt::RightButton) {
		//itemRightClicked(item);	右键点击目前仿佛有点问题，先屏蔽掉 zhenHua.sun 2010-09-06
		return;
	}

	if (item->parent()) {
		if (_previous == item) {
			closeContactInfo();
		} else {
			openContactInfo(item);
		}
	}

	_tree->viewport()->update();
	_button = Qt::NoButton;

}

void QtContactManager::itemRightClicked(QTreeWidgetItem * item) {
	OWSAFE_DELETE(_menu);
	_menu = createMenu();
	_menu->popup(QCursor::pos());

	//clearSelection () is bugged ! We have to clear the selection ourself
	clearTreeSelection();
	_tree->setItemSelected(item, true);
	_button = Qt::NoButton;
}

void QtContactManager::groupClicked(QTreeWidgetItem * item) {
	if (_button == Qt::RightButton) {
		groupRightClicked(item->text(0));
		return;
	}

	if (_tree->isItemExpanded(item)) {
		_tree->collapseItem(item);
	} else {
		_tree->expandItem(item);
	}
}

void QtContactManager::clearTreeSelection() {
	QList <QTreeWidgetItem * > selectedList = _tree->selectedItems();
	QList <QTreeWidgetItem * >::iterator it;
	for (it = selectedList.begin(); it != selectedList.end(); it ++) {
		_tree->setItemSelected((*it), false);
	}
}

void QtContactManager::defaultAction(QTreeWidgetItem * item) {
	_button = Qt::NoButton;
	if (!item) {
		return;
	}
	Config & config = ConfigManager::getInstance().getCurrentConfig();
	QtContactListManager * ul = QtContactListManager::getInstance();
	QString userId = item->text(0);
	ContactProfile contactProfile = _qtContactList.getCContactList().getContactProfile(userId.toStdString());

	if (config.getGeneralClickStartChat()) {
		ul->startChat(userId);
		return;
	}

	// Start free call by default
	if (contactProfile.hasFreeCall() || contactProfile.hasVoiceMail()) {
		ul->startFreeCall(userId);
	} else if (config.getGeneralClickCallCellPhone() && contactProfile.hasCall()) {
		ul->startCall(userId);
	}
}

void QtContactManager::safeUserStateChanged() {
	OWSAFE_DELETE(_menu);
	redrawContacts();
}

void QtContactManager::userStateChanged() {
	safeUserStateChanged();
}

void QtContactManager::sortContacts(bool bypassTimer) {
	safeSortContacts(bypassTimer);
}

void QtContactManager::safeSortContacts(bool bypassTimer) {
	if (!_sortContacts) {
		return;
	}

	if (!bypassTimer) {
		if (_canSort) {
			_canSort = false;
			if (_sortTimerId != -1) {
				killTimer(_sortTimerId);
			}
			_sortTimerId = startTimer(1000);
			_wantSort = false;
		} else {
			_wantSort = true;
			return;
		}
	}

	int topCount = _tree->topLevelItemCount();
	for (int topIndex = 0; topIndex < topCount; ++topIndex) {
		QTreeWidgetItem* groupItem = _tree->topLevelItem(topIndex);
		int count = groupItem->childCount();
		QtContactInfoList contactInfoList;
		for (int index = 0; index < count; ++index) {
			QTreeWidgetItem* item = groupItem->child(index);
			QtContactInfo info = QtContactInfo(item, groupItem, item->text(0), index);
			contactInfoList.append(info);
		}

		qSort(contactInfoList);

		Q_FOREACH(QtContactInfo info, contactInfoList) {
			QTreeWidgetItem* item = info.getItem();
			groupItem->takeChild(groupItem->indexOfChild(item));
			groupItem->insertChild(groupItem->childCount(), item);
		}
	}
}

bool QtContactManager::canShowUser(const ContactProfile * cprofile) {
	Config & config = ConfigManager::getInstance().getCurrentConfig();
	if (config.getShowOfflineContacts()) {
		return true;
	}

	// Enforce rules from http://dev.openwengo.org/trac/openwengo/trac.cgi/wiki/ContactPresenceGrid
	// version 9
	EnumPresenceState::PresenceState presenceState = cprofile->getPresenceState();
	if (presenceState == EnumPresenceState::PresenceStateOffline
		|| presenceState == EnumPresenceState::PresenceStateUnavailable
		|| presenceState == EnumPresenceState::PresenceStateUnknown)
	{
		QtContact qtContact(cprofile, _cWengoPhone, 0);

		QtContactPixmap::ContactPixmap pixmap = qtContact.getStatus();
		switch (pixmap) {
		case QtContactPixmap::ContactNoStatusSIP:
		case QtContactPixmap::ContactNoStatus:
		case QtContactPixmap::ContactWDeal:
		case QtContactPixmap::ContactTestCall:
			return true;
		case QtContactPixmap::ContactUnknown:
		case QtContactPixmap::ContactOffline:
			return false;
		default:
			LOG_WARN("Contact status for user '%s' should not be %d (presenceState=%s)",
				cprofile->getDisplayName().c_str(),
				pixmap,
				EnumPresenceState::toString(presenceState).c_str());
			return true;
		}
	}
	return true;
}

void QtContactManager::redrawContacts() {
	if (_canShow) {
		_canShow = false;
		if (_showTimerId != -1) {
			killTimer(_showTimerId);
		}
		_showTimerId = startTimer(500);
		_wantShow = false;
	} else {
		_wantShow = true;
		return;
	}

 	ContactProfile cprofile;
	QtContactListManager * ul = QtContactListManager::getInstance();
	std::vector<std::string> contactIds = _qtContactList.getCContactList().getContactIds();
	std::vector<std::string>::iterator it;
	//First, remove all item from the treeview and all users from userlist
	_tree->clear();
	ul->clear();
	_previous = 0;

	for (it = contactIds.begin(); it != contactIds.end(); it++) {
		cprofile = _qtContactList.getCContactList().getContactProfile((*it));
		if (canShowUser(&cprofile)) {
			//Create new group if needed
			QTreeWidgetItem * group = createGroupItem(QString::fromStdString(cprofile.getGroupId()));

			//We have the group, now add users to the group
			QTreeWidgetItem * newContact = NULL;
			QtContact * qtContact = NULL;
			QString contactName;
			newContact = new QTreeWidgetItem(group);
			newContact->setText(0, QString::fromStdString((*it)));
			newContact->setFlags(newContact->flags() | Qt::ItemIsEditable);
			qtContact = new QtContact((*it), _cWengoPhone, _tree);
			ul->addContact(qtContact);
		}
	}

	safeSortContacts(true);
}

QMenu * QtContactManager::createConferenceMenu() {
	PhoneLine * phoneLine = dynamic_cast <PhoneLine *>
		(_cWengoPhone.getCUserProfileHandler().getCUserProfile()->getUserProfile().getActivePhoneLine());
	QMenu * menu = new QMenu(_trStringInviteToConference);
	if (phoneLine) {
		PhoneLine::PhoneCallList phoneCallList = phoneLine->getPhoneCallList();
		PhoneLine::PhoneCallList::iterator it;
		for (it = phoneCallList.begin(); it != phoneCallList.end(); it++) {
			if ((*it)->getState() != EnumPhoneCallState::PhoneCallStateClosed) {
				QtConferenceAction * action = new
					QtConferenceAction(QString::fromStdString((*it)->getPeerSipAddress().getUserName()), menu);
				action->setPhoneCall((*it));
				SAFE_CONNECT(action, SIGNAL(triggered(bool)), SLOT(inviteToConference()));
				menu->addAction(action);
			}
		}
	}
	return menu;
}

void QtContactManager::inviteToConference() {
	QObject * source = sender();
	if (source) {
		QtConferenceAction * action = dynamic_cast < QtConferenceAction * > (source);
		QtContactListManager * ul = QtContactListManager::getInstance();
		//The current selected item
		QTreeWidgetItem * item = _tree->currentItem();
		QString contactId = item->text(0);
		if (ul->contains(contactId)) {
			QString phone = ul->getPreferredNumber(contactId);
			inviteToConferenceClicked(phone, action->getPhoneCall());
		}
	} else {
		LOG_FATAL("don't call this function directly");
	}
}

QMenu * QtContactManager::createMenu() {
	QAction * action = NULL;
	QString contactId = _tree->currentItem()->text(0);
	QMenu * menu = new QMenu(dynamic_cast <QWidget *> (parent()));
	QtContactListManager * ul = QtContactListManager::getInstance();

	//Call menu
	QMenu * callMenu = menu->addMenu(QIcon(":/pics/actions/accept-phone.png"), _trStringCall);

	if (!ul->getMobilePhone(contactId).isEmpty()) {
		action = callMenu->addAction( _trStringMobilePhone);
		SAFE_CONNECT(action, SIGNAL(triggered(bool)), SLOT(startMobileCall()));
	}

	if (!ul->getHomePhone(contactId).isEmpty()) {
		action = callMenu->addAction(_trStringHomePhone);
		SAFE_CONNECT(action, SIGNAL(triggered(bool)), SLOT(startHomeCall()));
	}

	/*
	if (!ul->getWorkPhone(contactId).isEmpty()) {
		action = callMenu->addAction(_trStringWorkPhone);
		SAFE_CONNECT(action, SIGNAL(triggered(bool)), SLOT(startWorkCall()));
	}
	*/

	if (!ul->getWengoPhoneNumber(contactId).isEmpty()) {
		action = callMenu->addAction(_trStringWengoPhone);
		SAFE_CONNECT(action, SIGNAL(triggered(bool)), SLOT(startWengoCall()));
	}

	if (!ul->hasPhoneNumber(contactId)) {
		callMenu->setEnabled(false);
	}

	action = menu->addAction(QIcon(":/pics/actions/chat.png"), _trStringStartChat);
	SAFE_CONNECT(action, SIGNAL(triggered(bool)), SLOT(startChat()));
	if (!ul->hasIM(contactId)) {
		action->setEnabled(false);
	}

	action = menu->addAction(QIcon(":/pics/actions/send-sms-16.png"), _trStringSendSMS);
	SAFE_CONNECT(action, SIGNAL(triggered(bool)), SLOT(startSMS()));
	if (ul->getMobilePhone(contactId).isEmpty()) {
		action->setEnabled(false);
	}
	
#ifdef DISABLE_SMS
	action ->setVisible(false);
#endif

	action = menu->addAction(QIcon(":/pics/actions/send_file.png"), _trStringSendFile);
	SAFE_CONNECT(action, SIGNAL(triggered(bool)), SLOT(sendFile()));
	if (!ul->hasFileTransfer(contactId)) {
		action->setEnabled(false);
	}

	//FIXME Desactivated for the moment due to a crash
	//menu->addMenu(createConferenceMenu());

	menu->addSeparator();

	QString website = ul->getWebsite(contactId);
	action = menu->addAction(QIcon(":/pics/contact/home.png"), _trStringWebsite);
	SAFE_CONNECT(action, SIGNAL(triggered(bool)), SLOT(goToWebsite()));
	if (website.isEmpty()) {
		action->setEnabled(false);
	}

	menu->addSeparator();

	action = menu->addAction(_trStringEditContact);
	SAFE_CONNECT(action, SIGNAL(triggered(bool)), SLOT(editContact()));

	action = menu->addAction(_trStringDeleteContact);
	SAFE_CONNECT(action, SIGNAL(triggered(bool)), SLOT(deleteContact()));

	//menu->addSeparator();
	//menu->addAction(_trStringBlockContact);
	//menu->addAction(_trStringForwardToCellPhone);

	return menu;
}

void QtContactManager::startMobileCall() {
	QtContactListManager * ul = QtContactListManager::getInstance();
	QTreeWidgetItem * item = _tree->currentItem();
	if (ul && item) {
		QtContact * qtContact = ul->getContact(item->text(0));
		qtContact->startCall(qtContact->getMobilePhone());
	}
}
/*
* @author zhenHua.sun
* @date 2010-09-02
* @brief 开始sip呼叫
*/
void QtContactManager::startFreeCall()
{
	QtContactListManager *ul = QtContactListManager::getInstance();
	QTreeWidgetItem* item = _tree->currentItem();
	if (ul && item) {
		QtContact * qtContact = ul->getContact(item->text(0));
		if( qtContact!=NULL )
			qtContact->startFreeCall();
	}
}

void QtContactManager::startHomeCall() {
	QtContactListManager * ul = QtContactListManager::getInstance();
	QTreeWidgetItem * item = _tree->currentItem();
	if (ul && item) {
		QtContact * qtContact = ul->getContact(item->text(0));
		qtContact->startCall(qtContact->getHomePhone());
	}
}

void QtContactManager::startWorkCall() {
	QtContactListManager * ul = QtContactListManager::getInstance();
	QTreeWidgetItem * item = _tree->currentItem();
	if (ul && item) {
		QtContact * qtContact = ul->getContact(item->text(0));
		qtContact->startCall(qtContact->getWorkPhone());
	}
}

void QtContactManager::startWengoCall() {
	QtContactListManager * ul = QtContactListManager::getInstance();
	QTreeWidgetItem * item = _tree->currentItem();
	if (ul && item) {
		QtContact * qtContact = ul->getContact(item->text(0));
		qtContact->startCall(qtContact->getWengoPhoneNumber());
	}
}

void QtContactManager::goToWebsite() {
	QtContactListManager * ul = QtContactListManager::getInstance();
	QTreeWidgetItem * item = _tree->currentItem();
	if (ul && item) {
		QtContact * qtContact = ul->getContact(item->text(0));
		QString website = qtContact->getWebsite();
		if (!website.contains("http://")) {
			website = "http://" + website;
		}
		WebBrowser::openUrl(website.toStdString());
	}
}

QTreeWidgetItem* QtContactManager::createGroupItem(const QString & contactGroupId) {
	QTreeWidgetItem* item;
	QList < QTreeWidgetItem * > list;
	if (groupsAreHidden()) {
		list = _tree->findItems(QtContactList::DEFAULT_GROUP_NAME, Qt::MatchExactly);
	} else {
		list =_tree->findItems(contactGroupId, Qt::MatchExactly);
	}
	//If no group exists, create the group
	if (list.isEmpty()) {
		item = new QTreeWidgetItem(_tree);
		if (groupsAreHidden()) {
			item->setText(0, QtContactList::DEFAULT_GROUP_NAME);
		} else {
			item->setText(0, contactGroupId);
		}
		_tree->setItemExpanded(item, true);
	} else {
		item = list[0];
	}

	return item;
}

void QtContactManager::addContact(const QString & contactId) {
	QtContactListManager * ul = QtContactListManager::getInstance();
	QtContact * qtContact = new QtContact(contactId.toStdString(), _cWengoPhone, _tree);
	ul->addContact(qtContact);

	ContactProfile contactProfile = _qtContactList.getCContactList().getContactProfile(contactId.toStdString());
	QTreeWidgetItem * groupItem = createGroupItem(QString::fromStdString(contactProfile.getGroupId()));

	int contactCount = groupItem->childCount();

	QString userName = qtContact->getUserName().toUpper();

	int contactIndex;
	for (contactIndex = 0; contactIndex < contactCount; ++contactIndex) {
		QTreeWidgetItem * childItem = groupItem->child(contactIndex);
		QtContact* childQtContact = ul->getContact(childItem->text(0));
		if (childQtContact->getUserName().toUpper() > userName) {
			break;
		}
	}

	QTreeWidgetItem * item = new QTreeWidgetItem();
	item->setText(0, contactId);
	item->setFlags(item->flags() | Qt::ItemIsEditable);
	groupItem->insertChild(contactIndex, item);
}

void QtContactManager::removeContact(const QString & contactId) {
	QtContactListManager * ul = QtContactListManager::getInstance();

	QTreeWidgetItem* contactItem = findContactItem(contactId);
	if (!contactItem) {
		// Already removed
		return;
	}

	if (_previous == contactItem) {
		_previous = 0;
	}

	QTreeWidgetItem* groupItem = contactItem->parent();

	QtContact* qtContact = ul->getContact(contactId);
	if (qtContact) {
		ul->removeContact(qtContact);
	} else {
		LOG_WARN("Could not find contact to remove in QtContactListManager");
	}
	delete contactItem;
#if QT_VERSION < 0x40300
	// This is needed on Qt 4.2 and lower to workaround this bug:
	// Assuming the list looks like this:
	// ( ) A
	// ( ) B
	//     +---+ ...
	//     |   | ...
	//     +---+
	//
	// if 'A' gets hidden, the 'B' line moves up, but its QtContactWidget stays
	// were it was.
	_tree->doItemsLayout();
#endif

	if (groupItem->childCount() == 0) {
		delete groupItem;
	}
}

void QtContactManager::setMouseButton(Qt::MouseButton button) {
	_button = button;
}

void QtContactManager::moveContact(const std::string & dstGroupId,
	const std::string & srcGroupId, const std::string & contactId) {

	LOG_DEBUG("moving contact=" + contactId +
		" from=" + srcGroupId +
		" to=" + dstGroupId);

	//If groups are hidden, there is nothing to move...
	if (groupsAreHidden()) {
		return;
	}

	//Looking for the contact in the destination group
	//If it is inside this group nothing is done
	QTreeWidgetItem * group = findGroupItem(QString::fromStdString(dstGroupId));
	if (!group) {
		return;
	}
	if (findContactInGroup(group, QString::fromStdString(contactId))) {
		return;
	}

	removeContact(QString::fromStdString(contactId));
	addContact(QString::fromStdString(contactId));
}

QTreeWidgetItem * QtContactManager::findGroupItem(const QString & groupId) const {
	QList < QTreeWidgetItem * > list = _tree->findItems(groupId, Qt::MatchExactly);
	if (list.isEmpty()) {
		return NULL;
	}

	return list[0];
}

QTreeWidgetItem * QtContactManager::findContactItem(const QString & contactId) const {
	QList < QTreeWidgetItem * > list = _tree->findItems(contactId, Qt::MatchExactly | Qt::MatchRecursive);
	if (list.isEmpty()) {
		return 0;
	}

	return list[0];
}

QtContact * QtContactManager::findContactInGroup(const QTreeWidgetItem * group, const QString & contactId) const {
	QtContactListManager * ul = QtContactListManager::getInstance();

	int count = group->childCount();
	for (int i = 0; i < count; i++) {
		QtContact * qtContact = ul->getContact(group->child(i)->text(0));
		if (qtContact->getId() == contactId) {
			return qtContact;
		}
	}

	return NULL;
}

void QtContactManager::timerEvent(QTimerEvent * event) {
	if (event->timerId() == _sortTimerId) {
		killTimer(_sortTimerId);
		_sortTimerId = -1;
		_canSort = true;
		if (_wantSort) {
			safeSortContacts(true);
			_wantSort = false;
		}
		return;
	}

	if (event->timerId() == _showTimerId) {
		killTimer(_showTimerId);
		_showTimerId = -1;
		_canShow = true;
		if (_wantShow) {
			redrawContacts();
			_wantShow = false;
		}
		return;
	}

	if (event->timerId() == _timerId) {
		_waitForDoubleClick = false;
		killTimer(_timerId);
		return;
	}

	QObject::timerEvent(event);
}

bool QtContactManager::groupsAreHidden() const {
	Config & config = ConfigManager::getInstance().getCurrentConfig();
	return !config.getShowGroups();
}

void QtContactManager::languageChanged() {
	_trStringCall = tr("Call");
	_trStringMobilePhone = tr("Mobile phone");
	_trStringHomePhone = tr("Home phone");
	_trStringWorkPhone = tr("Work phone");
	_trStringWengoPhone = tr("@product@");
	_trStringSendSMS = tr("Send SMS");
	_trStringWebsite = tr("Go to website");
	_trStringSendFile = tr("Send file");
	_trStringEditContact = tr("Edit contact");
	_trStringDeleteContact = tr("Delete contact");
	_trStringBlockContact = tr("Block contact");
	_trStringForwardToCellPhone = tr("Forward to mobile phone");
	_trStringStartChat = tr("Start chat");
	_trStringInviteToConference = tr("Invite to conference");
}

void QtContactManager::updateContact(const std::string& contactId) {
	QtContactListManager * ul = QtContactListManager::getInstance();

	QTreeWidgetItem* item = findContactItem(QString::fromStdString(contactId));
	bool oldVisible = item != 0;

	ContactProfile profile = _qtContactList.getCContactList().getContactProfile(contactId);
	bool newVisible = canShowUser(&profile);

	if (oldVisible && !newVisible) {
		// Remove
		removeContact(QString::fromStdString(contactId));
	} else if (!oldVisible && newVisible) {
		// Add
		addContact(QString::fromStdString(contactId));
	} else if (oldVisible && newVisible) {
		// Update
		ul->contactUpdated(QString::fromStdString(contactId));

		// Repaint only the item. We adjust the rect to avoid repainting
		// neighbour items. It does not matter because QtContact::paint repaint
		// the whole item, regardless of the area to update.
		QRect rect = _tree->visualItemRect(item);
		rect.adjust(0, 1, 0, -1);
		_tree->viewport()->update(rect);
		////

		QtContactWidget* widget = _tree->findChild<QtContactWidget*>();
		if (widget) {
			widget->updateButtons();
			widget->update();
		}
	}
}
