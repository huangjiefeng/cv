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

#include "QtContactList.h"

#include "ui_ContactList.h"

#include "ContactGroupPopupMenu.h"
#include "QtContactListManager.h"
#include "QtContactManager.h"
#include "QtContactPixmap.h"
#include "QtContactTreeMouseFilter.h"
#include "QtTreeViewDelegate.h"

#include <presentation/qt/QtWengoPhone.h>

#include <control/CWengoPhone.h>
#include <control/profile/CUserProfileHandler.h>
#include <control/profile/CUserProfile.h>     //add by li.zhennig
#include <control/contactlist/CContactList.h>

#include <model/contactlist/Contact.h>
#include <model/contactlist/ContactGroup.h>
#include <model/contactlist/ContactProfile.h> // add by li.zhenning
#include <model/profile/UserProfile.h>      // add by li.zhenning

#include <util/Logger.h>
#include <util/SafeDelete.h>
#include <util/StringList.h>

#include <qtutil/SafeConnect.h>

#include <QtGui/QtGui>

const QString QtContactList::DEFAULT_GROUP_NAME = "WENGO2006CLISTHIDE";
const QString QtContactList::STATUS_NOSTATUS_PIXMAP = ":/pics/status/unknown-wengo.png";
const QString QtContactList::STATUS_NOSTATUS_SIP_PIXMAP = ":/pics/status/unknown-sip.png";
const QString QtContactList::STATUS_UNKNOW_PIXMAP = ":/pics/status/unknown.png";
const QString QtContactList::STATUS_ONLINE_PIXMAP = ":/pics/status/online.png";
const QString QtContactList::STATUS_OFFLINE_PIXMAP = ":/pics/status/offline.png";
const QString QtContactList::STATUS_DND_PIXMAP = ":/pics/status/donotdisturb.png";
const QString QtContactList::STATUS_INVISIBLE_PIXMAP = ":/pics/status/invisible.png";
const QString QtContactList::STATUS_AWAY_PIXMAP = ":/pics/status/away.png";
const QString QtContactList::STATUS_WDEAL_PIXMAP = ":/pics/status/wdeal.png";
const QString QtContactList::STATUS_TESTCALL_PIXMAP = ":/pics/status/testcall.png";
const QString QtContactList::STATUS_GROUP_OPEN_PIXMAP = ":/pics/group_open.png";
const QString QtContactList::STATUS_GROUP_CLOSE_PIXMAP = ":/pics/group_close.png";

static inline QPixmap scalePixmap(const QString name) {
	return QPixmap(name).scaled(QSize(16, 16), Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

QtContactList::QtContactList(CContactList & cContactList, CWengoPhone & cWengoPhone)
	: QObject(NULL),
	_cContactList(cContactList),
	_cWengoPhone(cWengoPhone) {

	_waitingForModel = false;

	_contactListWidget = new QWidget(NULL);

	_ui = new Ui::ContactList();
	_ui->setupUi(_contactListWidget);

#if defined(OS_MACOSX)
	_contactListWidget->layout()->setMargin(10);
#endif

	//treeWidget
	_ui->treeWidget->setAcceptDrops(true);
	_ui->treeWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	_ui->treeWidget->setProperty("showDropIndicator", QVariant(true));
	_ui->treeWidget->setDragEnabled(true);
	_ui->treeWidget->setAlternatingRowColors(false);
	_ui->treeWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
	_ui->treeWidget->setSelectionBehavior(QAbstractItemView::SelectItems);
	_ui->treeWidget->setIndentation(0);
	_ui->treeWidget->setRootIsDecorated(false);

	//icons
	QtContactPixmap * spx = QtContactPixmap::getInstance();
	spx->setPixmap(QtContactPixmap::ContactUnknown, scalePixmap(STATUS_UNKNOW_PIXMAP));
	spx->setPixmap(QtContactPixmap::ContactNoStatus, scalePixmap(STATUS_NOSTATUS_PIXMAP));
	spx->setPixmap(QtContactPixmap::ContactNoStatusSIP, scalePixmap(STATUS_NOSTATUS_SIP_PIXMAP));
	spx->setPixmap(QtContactPixmap::ContactOnline, scalePixmap(STATUS_ONLINE_PIXMAP));
	spx->setPixmap(QtContactPixmap::ContactOffline, scalePixmap(STATUS_OFFLINE_PIXMAP ));
	spx->setPixmap(QtContactPixmap::ContactDND, scalePixmap(STATUS_DND_PIXMAP));
	spx->setPixmap(QtContactPixmap::ContactInvisible, scalePixmap(STATUS_INVISIBLE_PIXMAP));
	spx->setPixmap(QtContactPixmap::ContactAway, scalePixmap(STATUS_AWAY_PIXMAP));
	spx->setPixmap(QtContactPixmap::ContactWDeal, scalePixmap(STATUS_WDEAL_PIXMAP));
	spx->setPixmap(QtContactPixmap::ContactTestCall, scalePixmap(STATUS_TESTCALL_PIXMAP));

	//Group icons
	spx->setPixmap(QtContactPixmap::ContactGroupOpen, QPixmap(STATUS_GROUP_OPEN_PIXMAP));
	spx->setPixmap(QtContactPixmap::ContactGroupClose, QPixmap(STATUS_GROUP_CLOSE_PIXMAP));

	_contactManager = new QtContactManager(*_cWengoPhone.getCUserProfileHandler().getCUserProfile(),
		_cWengoPhone, *this, _ui->treeWidget, _ui->treeWidget);

	QtContactTreeMouseFilter * qtContactTreeMouseFilter =
		new QtContactTreeMouseFilter(_cContactList, _ui->treeWidget, _ui->treeWidget);
	SAFE_CONNECT_RECEIVER(qtContactTreeMouseFilter, SIGNAL(mouseClicked(Qt::MouseButton)),
		_contactManager, SLOT(setMouseButton(Qt::MouseButton)));
	SAFE_CONNECT(qtContactTreeMouseFilter, SIGNAL(mergeContacts(QString, QString)),
		SLOT(mergeContactsSlot(QString, QString)));

	QtTreeViewDelegate * delegate = new QtTreeViewDelegate(_cWengoPhone,
		_contactManager, _ui->treeWidget);
	delegate->setParent(_ui->treeWidget->viewport());

	_ui->treeWidget->setItemDelegate(delegate);
	_ui->treeWidget->setUniformRowHeights(false);
	_ui->treeWidget->header()->hide();

	//Popup Menus
	_contactGroupPopupMenu = new ContactGroupPopupMenu(_cContactList, _ui->treeWidget);
	SAFE_CONNECT(_contactManager, SIGNAL(groupRightClicked(const QString &)),
		SLOT(groupRightClickedSlot(const QString &)));

	QtWengoPhone * qtWengoPhone = QtContactList::_cWengoPhone._qtWengoPhone;//Modify By LZY 2010-09-28
	qtWengoPhone->setQtContactList(this);
	_ui->treeWidget->viewport()->setFocus();

	initContent();
}

QtContactList::~QtContactList() {
	QtContactListManager::getInstance()->clear();
	OWSAFE_DELETE(_contactListWidget);
	OWSAFE_DELETE(_ui);
}

void QtContactList::initContent() {
	StringList contacts = _cContactList.getContactIds();
	StringList::const_iterator it;

	for (it = contacts.begin(); it != contacts.end(); ++it) {
		contactAddedEvent(*it);
	}
}

void QtContactList::updatePresentation() {
	_contactManager->userStateChanged();
	_ui->treeWidget->viewport()->update();
}

void QtContactList::contactGroupAddedEvent(const std::string & groupId) {
	QString tmp = QString::fromStdString(groupId);
	if (_contactManager->groupsAreHidden()) {
		tmp = DEFAULT_GROUP_NAME;
	}
	addGroup(tmp);
}

void QtContactList::contactGroupRemovedEvent(const std::string & groupId) {
	QList <QTreeWidgetItem *> list = _ui->treeWidget->findItems(QString::fromStdString(groupId), Qt::MatchExactly);
	if (!list.isEmpty()) {
		_ui->treeWidget->takeTopLevelItem(_ui->treeWidget->indexOfTopLevelItem(list[0]));
	}
}

void QtContactList::contactGroupRenamedEvent(const std::string & groupId) {
	updatePresentation();
}

void QtContactList::contactAddedEvent(const std::string & contactId) {
	_contactManager->updateContact(contactId);
}

void QtContactList::contactRemovedEvent(const std::string & contactId) {
	_contactManager->removeContact(QString::fromStdString(contactId));
	updatePresentation();
	_waitingForModel = false;
}

void QtContactList::contactMovedEvent(const std::string & dstGroupId,
	const std::string & srcGroupId, const std::string & contactId) {

	_contactManager->moveContact(dstGroupId, srcGroupId, contactId);
}

void QtContactList::contactChangedEvent(const std::string & contactId) {
	_contactManager->updateContact(contactId);
	contactChangedEventSignal(QString::fromStdString(contactId));
}

void QtContactList::addGroup(QString contactGroupId) {
	QList <QTreeWidgetItem *> list;

	list = _ui->treeWidget->findItems(contactGroupId, Qt::MatchExactly);
	if (list.isEmpty()) {
		QTreeWidgetItem * group = new QTreeWidgetItem(_ui->treeWidget);
		group->setText(0, contactGroupId);
		_ui->treeWidget->setItemExpanded(group, true);
	}
}

CContactList & QtContactList::getCContactList() const {
	return _cContactList;
}

void QtContactList::groupRightClickedSlot(const QString & groupId) {
	_contactGroupPopupMenu->showMenu(groupId);
}

void QtContactList::mergeContactsSlot(QString dstContact, QString srcContact) {
	if (!_waitingForModel) {
		_waitingForModel = true;

		ContactProfile dstContactProfile = _cContactList.getContactProfile(dstContact.toStdString());
		ContactProfile srcContactProfile = _cContactList.getContactProfile(srcContact.toStdString());
		if (QMessageBox::question(_contactListWidget,
			tr("@product@ - Merge Contacts"),
			tr("Merge %1 with %2?")
			.arg(QString::fromStdString(dstContactProfile.getDisplayName()))
			.arg(QString::fromStdString(srcContactProfile.getDisplayName())),
			tr("&Yes"), tr("&No"),
			QString(), 0, 1) == 0) {
			_cContactList.merge(dstContact.toStdString(), srcContact.toStdString());
		} else {
			_waitingForModel = false;
		}
	}
}

/***
* Add by li.zhennig at 2010-08-04  modify at 2010-08-16
**/
std::string QtContactList::getContacts()
{
	std::string strContacts;
	strContacts = "<ContactList>\n";
	StringList contactIds = _cContactList.getContactIds();
	int nCount =  contactIds.size();
	for(int i = 0; i < nCount; i++)
	{
		std::string contactId = contactIds.at(i);
		std::string firstName = _cContactList.getContactProfile(contactId).getFirstName();
		std::string lastName = _cContactList.getContactProfile(contactId).getLastName();
		std::string presentState = EnumPresenceState::toString(_cContactList.getContactProfile(contactId).getPresenceState());
		std::string groupId = _cContactList.getContactProfile(contactId).getGroupId();
		std::string groupName = _cContactList.getContactGroupName(groupId);
		strContacts += "<Contact>\n";
		strContacts += "<ContactId>" + contactId + "</ContactId>\n";
		strContacts += "<FirstName>" + firstName + "</FirstName>\n";
		strContacts += "<LastName>" + lastName + "</LastName>\n";
		strContacts += "<SipAccount>" + getSipAccount(contactId) + "</SipAccount>\n";
		strContacts += "<PresentState>" + presentState + "</PresentState>\n";
		strContacts += "<GroupName>" + groupName + "</GroupName>\n";
		strContacts += "</Contact>\n";
	}
	strContacts += "</ContactList>\n";
	return strContacts;
}

/***
* Add by li.zhennig at 2010-08-04
**/
std::string QtContactList::getContactInfo(std::string contactId)
{
	std::string strContactInfo;
	strContactInfo = "<ContactInfo>\n";
	ContactProfile contactProfile = _cContactList.getContactProfile(contactId);
	strContactInfo += "<FirstName>" + contactProfile.getFirstName() + "</FirstName>\n";
	strContactInfo += "<LastName>" + contactProfile.getLastName() + "</LastName>\n";
	strContactInfo += "<SipAccount>" + getSipAccount(contactId) + "</SipAccount>\n";
	strContactInfo += "<HomePhone>" + contactProfile.getHomePhone() + "</HomePhone>\n";
	strContactInfo += "<MobilePhone>" + contactProfile.getMobilePhone() + "</MobilePhone>\n";
	strContactInfo += "<Email>" + contactProfile.getPersonalEmail() + "</Email>\n";


	StreetAddress address = contactProfile.getStreetAddress();
	strContactInfo += "<City>" + address.getCity() + "</City>\n";

	
	char chSex[2];
	itoa((int)contactProfile.getSex(),chSex,10);
	std::string sex = chSex;
	strContactInfo += "<Sex>" +  sex + "</Sex>\n";

	Date date = contactProfile.getBirthdate();
	strContactInfo += "<Birthday>" +  date.toString() + "</Birthday>\n";

	strContactInfo += "<GroupName>" + _cContactList.getContactGroupName(contactProfile.getGroupId()) + "</GroupName>";

	strContactInfo += "</ContactInfo>\n";
	return strContactInfo;
}

/***
* Add by li.zhennig at 2010-08-09
**/
void QtContactList::setSipAccount(std::string sipAccount, ContactProfile &contactProfile)
{
	QString currentSipAccount = QString::fromStdString(sipAccount);
	if (!currentSipAccount.contains("@")) {
		SipAccount * sipaccount = _cWengoPhone.getCUserProfileHandler().getCUserProfile()->getUserProfile().getSipAccount();
		if (sipaccount) {
			currentSipAccount += QString("@") + QString::fromStdString(sipaccount->getRealm());
		}
	}

	std::string beforecontactId;
	const IMContactSet & imContactSet = contactProfile.getIMContactSet();
	IMContactSet::const_iterator it;
	for (it = imContactSet.begin(); it != imContactSet.end(); it++) 
	{
		if ((*it).getProtocol() == EnumIMProtocol::IMProtocolSIP)
		{
			IMContact * imContact = (IMContact *)(&(*it));
			beforecontactId = imContact->getDisplayContactId();
			if(beforecontactId != currentSipAccount.toStdString())
				break;
		}
	}
	if(it != imContactSet.end())
	{
		IMContact oldImContact(EnumIMProtocol::IMProtocolSIP,beforecontactId);
		contactProfile.removeIMContact(oldImContact);
	}

	IMContact newImContact(EnumIMProtocol::IMProtocolSIP, currentSipAccount.toStdString());
	//IMAccount to associate with the IMContact
	IMAccountList imAccounts =
		_cWengoPhone.getCUserProfileHandler().getCUserProfile()->getUserProfile().getIMAccountManager().getIMAccountsOfProtocol(EnumIMProtocol::IMProtocolSIP);
	IMAccountList::const_iterator it_ = imAccounts.begin();
	if (it_ != imAccounts.end()) {
		IMAccount * imAccount =
			_cWengoPhone.getCUserProfileHandler().getCUserProfile()->getUserProfile().getIMAccountManager().getIMAccount((*it_).getUUID());
		newImContact.setIMAccount(imAccount);
		OWSAFE_DELETE(imAccount);
	}
	contactProfile.addIMContact(newImContact);
}

/***
* Add by li.zhennig at 2010-08-09
**/
std::string QtContactList::getSipAccount(std::string contactId)
{
	ContactProfile contactProfile = _cContactList.getContactProfile(contactId);
	std::string sipAccount = "";
	const IMContactSet & imContactSet = contactProfile.getIMContactSet();
	for (IMContactSet::const_iterator it = imContactSet.begin(); it != imContactSet.end(); it++) 
	{
		if ((*it).getProtocol() == EnumIMProtocol::IMProtocolSIP)
		{
			IMContact * imContact = (IMContact *)(&(*it));
			sipAccount = imContact->getDisplayContactId();
			break;
		}
	}
	return sipAccount;
}

