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

#include "QtChatWidget.h"
#include "QtChatUtils.h"
#include "chatroom/QtChatRoomInviteDlg.h"
#include "emoticons/QtEmoticonsWidget.h"

#include <coipmanager/CoIpManager.h>
#include <filesessionmanager/SendFileSession.h>

#include <model/config/Config.h>
#include <model/config/ConfigManager.h>
#include <model/WengoPhone.h>
#include <model/profile/Profile.h>
#include <model/profile/UserProfile.h>

#include <control/CWengoPhone.h>
#include <control/contactlist/CContactList.h>
#include <control/profile/CUserProfile.h>
#include <presentation/qt/QtWengoPhone.h>
#include <presentation/qt/contactlist/QtContactList.h>
#include <presentation/qt/contactlist/QtContactListManager.h>
#include <presentation/qt/filetransfer/QtFileTransfer.h>

#include <imwrapper/Account.h>
#include <imwrapper/EnumIMProtocol.h>
#include <imwrapper/IMContactSet.h>

#include <util/File.h>
#include <util/Logger.h>
#include <util/SafeDelete.h>
#include <cutil/global.h>
#include <qtutil/SafeConnect.h>

#include <string>

#include <QtCore/QTime>
#include <QtCore/QTimer>

#include <QtGui/QColorDialog>
#include <QtGui/QFontDialog>
#include <QtGui/QKeyEvent>
#include <QtGui/QPainter>

static const int CHAT_STOPPED_TYPING_DELAY = 1000;

static const int EMOTICON_OFFSET_X = 13;
static const int EMOTICON_OFFSET_Y = 30;

QtChatWidget::QtChatWidget(CChatHandler & cChatHandler,
	QtWengoPhone * qtWengoPhone, int sessionId,
	QWidget * parent) :
	QWidget(parent),
	_cChatHandler(cChatHandler),
	_qtWengoPhone(qtWengoPhone) {

	//Default nickname for testing purpose
	_nickName = "Wengo";
	_sessionId = sessionId;
	_imChatSession = NULL;

	_bold = false;
	_italic = false;
	_underline = false;

	_isContactConnected = true;

	//setup ui
	_ui.setupUi(this);
	QPixmap pix = _ui.sendButton->icon().pixmap(50,50);
	QPainter painter(&pix);
	painter.setPen(Qt::white);
	painter.drawText(pix.rect(),Qt::AlignCenter,tr("send"));
	painter.end();
	_ui.sendButton->setIcon(QIcon(pix));
	_ui.editActionBar->setBackgroundRole(QPalette::Base);
	_ui.editFrame->setBackgroundRole(QPalette::Base);
	////

	//creates sub widgets
	_emoticonsWidget = new EmoticonsWidget(this);
	////

	//install event filters
	_ui.chatHistory->installEventFilter(this);
	_ui.chatEdit->installEventFilter(this);
	////
	
	//timer
	_isTyping = false;
	_stoppedTypingTimer = new QTimer(this);
	_stoppedTypingTimer->setSingleShot(true);
	SAFE_CONNECT_RECEIVER(_stoppedTypingTimer, SIGNAL(timeout()), this, SLOT(stoppedTypingSlot()));
	////

	//signal connection
	SAFE_CONNECT(_ui.sendButton, SIGNAL(clicked()), SLOT(sendMessage()));
	SAFE_CONNECT(_ui.editActionBar, SIGNAL(fontLabelClicked()), SLOT(changeFont()));
	SAFE_CONNECT(_ui.editActionBar, SIGNAL(emoticonsLabelClicked()), SLOT(chooseEmoticon()));
	SAFE_CONNECT(_ui.editActionBar, SIGNAL(fontColorLabelClicked()), SLOT(changeFontColor()));
	SAFE_CONNECT(_ui.editActionBar, SIGNAL(boldLabelClicked()), SLOT(boldClickedSlot()));
	SAFE_CONNECT(_ui.editActionBar, SIGNAL(italicLabelClicked()), SLOT(italicClickedSlot()));
	SAFE_CONNECT(_ui.editActionBar, SIGNAL(underlineLabelClicked()), SLOT(underlineClickedSlot()));
	SAFE_CONNECT(_emoticonsWidget, SIGNAL(emoticonClicked(QtEmoticon)), SLOT(emoticonSelected(QtEmoticon)));
	SAFE_CONNECT_RECEIVER(_emoticonsWidget, SIGNAL(closed()), _ui.chatEdit, SLOT(setFocus()));
	SAFE_CONNECT(_ui.chatEdit, SIGNAL(textChanged()), SLOT(chatEditTextChanged()));
	SAFE_CONNECT(_ui.chatEdit, SIGNAL(fileDragged(const QString &)), SLOT(sendFileToSession(const QString &)));
	SAFE_CONNECT(this, SIGNAL(contactAddedEventSignal(const IMContact &)),
		SLOT(contactAddedEventSlot(const IMContact &)));
	SAFE_CONNECT(this, SIGNAL(contactRemovedEventSignal(const IMContact &)),
		SLOT(contactRemovedEventSlot(const IMContact &)));
	SAFE_CONNECT(_ui.avatarFrameButton, SIGNAL(clicked()), SLOT(avatarFrameButtonClicked()));

	QtContactList * qtContactList = _qtWengoPhone->getQtContactList();
	SAFE_CONNECT(qtContactList, SIGNAL(contactChangedEventSignal(QString)), SLOT(contactChangedSlot(QString)));

	_cChatHandler.getCUserProfile().getUserProfile().profileChangedEvent +=
		boost::bind(&QtChatWidget::profileChangedEventHandler, this);
	SAFE_CONNECT_TYPE(this, SIGNAL(profileChangedEventHandlerSignal()), 
		SLOT(updateUserAvatar()), Qt::QueuedConnection);
}

QtChatWidget::~QtChatWidget() {
	_imChatSession->close();
	
	_stoppedTypingTimer->stop();
}

void QtChatWidget::profileChangedEventHandler() {
	profileChangedEventHandlerSignal();
}

void QtChatWidget::changeFont() {
	bool ok;
	QFont font = QFontDialog::getFont(&ok, _ui.chatEdit->currentFont(), this);
	if (ok) {
		_currentFont = font;
		_ui.chatEdit->setCurrentFont(font);
	}
	_ui.chatEdit->setFocus();
}

void QtChatWidget::changeFontColor() {
	bool ok;
	QRgb color = QColorDialog::getRgba(_ui.chatEdit->textColor().rgba(), &ok, this);
	if (ok) {
		_currentColor = QColor(color);
		_ui.chatEdit->setTextColor(_currentColor);
	}
	_ui.chatEdit->setFocus();
}

void QtChatWidget::boldClickedSlot() {
	_bold = !_bold;
	if (_bold) {
		_ui.chatEdit->setFontWeight(QFont::Bold);
	} else {
		_ui.chatEdit->setFontWeight(QFont::Normal);
	}
}

void QtChatWidget::italicClickedSlot() {
	_italic = !_italic;
	_ui.chatEdit->setFontItalic(_italic);
}

void QtChatWidget::underlineClickedSlot() {
	_underline = !_underline;
	_ui.chatEdit->setFontUnderline(_underline);
}

void QtChatWidget::chooseEmoticon() {
	QPoint p = _ui.editActionBar->pos();
	QPoint offset(EMOTICON_OFFSET_X, EMOTICON_OFFSET_Y);
	p += offset;
	_emoticonsWidget->move(mapToGlobal(p));
	_emoticonsWidget->show();
}

void QtChatWidget::setVisible(bool visible) {
	QWidget::setVisible(visible);
	if (visible) {
		_ui.chatEdit->setFocus();
	}
}

void QtChatWidget::showInviteDialog() {
	if (canDoMultiChat()) {
		QtChatRoomInviteDlg dlg(*_imChatSession,
			_cChatHandler.getCUserProfile().getCContactList(), this);
		dlg.exec();
	}
}

void QtChatWidget::contactAddedEventHandler(IMChatSession & sender, const IMContact & imContact) {
	contactAddedEventSignal(imContact);
}

void QtChatWidget::contactRemovedEventHandler(IMChatSession & sender, const IMContact & imContact) {
	contactRemovedEventSignal(imContact);
}

void QtChatWidget::contactAddedEventSlot(const IMContact & imContact) {

	QtContactList * qtContactList = _qtWengoPhone->getQtContactList();
	CContactList & cContactList = qtContactList->getCContactList();
	std::string contactId = cContactList.findContactThatOwns(imContact);

	if (_ui.avatarFrame->getContactIDList().contains(QString::fromStdString(contactId))) {
		LOG_DEBUG("" + imContact.getContactId() + " deja dans la session !");
		return;
	}

	ContactProfile profile = cContactList.getContactProfile(contactId);

	std::string data = profile.getIcon().getData();
	QPixmap pixmap;
	pixmap.loadFromData((uchar *)data.c_str(), data.size());
	_ui.avatarFrame->addRemoteContact(
		QString::fromStdString(contactId),
		QString::fromUtf8(profile.getDisplayName().c_str()),
		QString::fromStdString(imContact.getContactId()),
		pixmap
	);

	QString imContactId = QString::fromStdString(imContact.getContactId());
	addStatusMessage(tr("%1 has joined the chat").arg(imContactId));
}

void QtChatWidget::contactRemovedEventSlot(const IMContact & imContact) {
	QString imContactId = QString::fromStdString(imContact.getContactId());
	addStatusMessage(tr("%1 has left the chat").arg(imContactId));

	QtContactList * qtContactList = _qtWengoPhone->getQtContactList();
	CContactList & cContactList = qtContactList->getCContactList();
	std::string contactId = cContactList.findContactThatOwns(imContact);
	_ui.avatarFrame->removeRemoteContact(QString::fromStdString(contactId));
}

void QtChatWidget::emoticonSelected(QtEmoticon emoticon) {
	_ui.chatEdit->insertHtml(emoticon.getHtml());
	_ui.chatEdit->ensureCursorVisible();
}

void QtChatWidget::avatarFrameButtonClicked() {
	if (_ui.avatarFrame->isVisible()) {
		hideAvatarFrame();
	} else {
		showAvatarFrame();
	}
}

void QtChatWidget::hideAvatarFrame() {
	_ui.avatarFrame->hide();
	_ui.avatarFrameButton->setIcon(QIcon(":pics/chat/show_avatar_frame.png"));
}

void QtChatWidget::showAvatarFrame() {
	_ui.avatarFrame->show();
	_ui.avatarFrameButton->setIcon(QIcon(":pics/chat/hide_avatar_frame.png"));
}

void QtChatWidget::addToHistory(const QString & contactId, const QString & senderName, const QString & str, const QTime & time) {
	QTime tmp;
	if (time.isNull()) {
		tmp = QTime::currentTime();
	} else {
		tmp = time;
	}
	_ui.chatHistory->insertMessage(contactId, senderName, str, tmp);
}

void QtChatWidget::addStatusMessage(const QString & statusMessage) {
	_ui.chatHistory->insertStatusMessage(statusMessage, QTime::currentTime());
}

void QtChatWidget::sendMessage() {
	QRegExp htmlDocumentToHtmlSnippet(".*<body[^>]*>\\s*<p[^>]*>(.*)</p>\\s*</body>\\s*</html>");
	QRegExp pTag("<p[^>]*>");

	//Drop empty message
	if (_ui.chatEdit->toPlainText().isEmpty()) {
		return;
	}

	QString html = _ui.chatEdit->toHtml();
	// toHtml() returns an HTML document, complete with html and body tags.
	// This regexp strip those to produce an HTML snippet, which can be
	// concatenated to the existing history
	html.replace(htmlDocumentToHtmlSnippet, "\\1");
	html.replace(pTag, "<div>");
	html.replace("</p>", "</div>");

	addToHistory("self", _nickName, html);

	IMAccount * imAccount =
		_cChatHandler.getCUserProfile().getUserProfile().getIMAccountManager().getIMAccount(_imChatSession->getIMChat().getIMAccountId());
	QString message = QtChatUtils::encodeMessage(imAccount->getProtocol(), html);
	OWSAFE_DELETE(imAccount);
	_imChatSession->sendMessage(message.toUtf8().constData());
	////

	_isTyping = true;
	_ui.chatEdit->clear();
	_ui.chatEdit->setFocus();
	_ui.chatEdit->ensureCursorVisible();
	
	//Not typing anymore
	_imChatSession->changeTypingState(IMChat::TypingStateStopTyping);
	_isTyping = false;
	_stoppedTypingTimer->stop();
	////
}

void QtChatWidget::setIMChatSession(IMChatSession * imChatSession) {
	IMAccount * imAccount =
		_cChatHandler.getCUserProfile().getUserProfile().getIMAccountManager().getIMAccount(imChatSession->getIMChat().getIMAccountId());

	_imChatSession = imChatSession;
	_emoticonsWidget->initButtons(QtChatUtils::getProtocol(imAccount->getProtocol()));
	
	_ui.chatHistory->setProtocol(imAccount->getProtocol());

	OWSAFE_DELETE(imAccount);

	updateAvatarFrame();
	updateUserAvatar();
	if (_imChatSession->getIMContactSet().size() == 1) {
		// It's useless to show the avatar frame if there is only one contact,
		// since we can see its avatar when he talks.
		hideAvatarFrame();
	}

	_imChatSession->contactAddedEvent +=
		boost::bind(&QtChatWidget::contactAddedEventHandler, this, _1, _2);
	_imChatSession->contactRemovedEvent +=
		boost::bind(&QtChatWidget::contactRemovedEventHandler, this, _1, _2);
	_imChatSession->changeTypingState(IMChat::TypingStateNotTyping);
}

void QtChatWidget::chatEditTextChanged() {
	// change typing state to Typing
	if (!_isTyping) {
		_imChatSession->changeTypingState(IMChat::TypingStateTyping);
		_isTyping = true;
	}
	////

	// manage timers
	_stoppedTypingTimer->start(CHAT_STOPPED_TYPING_DELAY);
	////
}

void QtChatWidget::stoppedTypingSlot() {
	_imChatSession->changeTypingState(IMChat::TypingStateStopTyping);
	_isTyping = false;
}

void QtChatWidget::updateAvatarFrame() {
	QMutexLocker locker(&_mutex);

	QtContactList * qtContactList = _qtWengoPhone->getQtContactList();
	CContactList & cContactList = qtContactList->getCContactList();

	IMContactSet imContactSet = _imChatSession->getIMContactSet();
	IMContactSet::iterator it;
	for (it = imContactSet.begin(); it != imContactSet.end(); it++) {

		std::string contactId = cContactList.findContactThatOwns(*it);
		ContactProfile profile = cContactList.getContactProfile(contactId);

		std::string data = profile.getIcon().getData();
		QPixmap pixmap;
		if (data.size() > 0) {
			pixmap.loadFromData((uchar *)data.c_str(), data.size());
		}
		QString displayName = QString::fromUtf8(profile.getDisplayName().c_str());

		_ui.chatHistory->setAvatarPixmap(
			QString::fromStdString(contactId),
			pixmap);
		_ui.avatarFrame->addRemoteContact(
			QString::fromStdString(contactId),
			displayName,
			QString::fromStdString((*it).getContactId()),
			pixmap
		);
	}
}

void QtChatWidget::updateUserAvatar() {

	QPixmap pixmap;
	UserProfile& userProfile = _cChatHandler.getCUserProfile().getUserProfile();
	std::string myData = userProfile.getIcon().getData();
	pixmap.loadFromData((uchar *)myData.c_str(), myData.size());

	_ui.chatHistory->setAvatarPixmap("self", pixmap);
	_ui.avatarFrame->setUserPixmap(pixmap);
}

void QtChatWidget::sendFileToSession(const QString & filename) {
	Config & config = ConfigManager::getInstance().getCurrentConfig();

	if (!canDoFileTransfer()) {

		QtContactList * qtContactList = _qtWengoPhone->getQtContactList();
		CContactList & cContactList = qtContactList->getCContactList();
		ContactProfile contactProfile = cContactList.getContactProfile(_contactId.toStdString());

		if (contactProfile.getFirstWengoId().empty()) {

			QString myMess = tr("Your file can not be sent: your contact must be connected on the @company@ network. ");
			//myMess += tr("An automatic message has been sent to him");
			addStatusMessage(myMess);

			QString hisMess = "<i>";
			QString url = QString::fromStdString(config.getCompanyWebSiteUrl());
			hisMess += tr("Your contact wishes to send a file with @company@. ");
			hisMess += tr("Go to %1 to install it").arg(url);
			hisMess += "</i>";
			_imChatSession->sendMessage(hisMess.toStdString());
		}
		return;
	}

	QtContactList * qtContactList = _qtWengoPhone->getQtContactList();
	CContactList & cContactList = qtContactList->getCContactList();
	QtFileTransfer * qtFileTransfer = _qtWengoPhone->getFileTransfer();
	if (qtFileTransfer) {
		qtFileTransfer->createSendFileSession(_imChatSession->getIMContactSet(), filename, cContactList);
	}
}

void QtChatWidget::saveHistoryAsHtml() {
	_ui.chatHistory->saveHistoryAsHtml();
}

void QtChatWidget::setContactConnected(bool connected) {
	QtContactList * qtContactList = _qtWengoPhone->getQtContactList();
	CContactList & cContactList = qtContactList->getCContactList();
	ContactProfile profile = cContactList.getContactProfile(_contactId.toStdString());

	QString contactName;
	if (!profile.getShortDisplayName().empty()) {
		contactName = QString::fromUtf8(profile.getShortDisplayName().c_str());
	} else {
		contactName = QString::fromUtf8(profile.getDisplayName().c_str());
	}

	if (connected && !_isContactConnected) {
		addStatusMessage(QString(tr("%1 is connected.")).arg(contactName));
	} else if (!connected && _isContactConnected) {
		addStatusMessage(QString(tr("%1 is disconnected.")).arg(contactName));
	}

	_isContactConnected = connected;
}

void QtChatWidget::contactChangedSlot(QString contactId) {

	QtContactList * qtContactList = _qtWengoPhone->getQtContactList();
	CContactList & cContactList = qtContactList->getCContactList();
	ContactProfile profile = cContactList.getContactProfile(contactId.toStdString());
	std::string data = profile.getIcon().getData();
	QPixmap pixmap;
	pixmap.loadFromData((uchar *)data.c_str(), data.size());
	_ui.avatarFrame->updateContact(contactId, pixmap, QString::fromStdString(profile.getDisplayName()));
}

bool QtChatWidget::canDoFileTransfer() {
	QtContactList * qtContactList = _qtWengoPhone->getQtContactList();
	CContactList & cContactList = qtContactList->getCContactList();
	ContactProfile contactProfile = cContactList.getContactProfile(_contactId.toStdString());

	if (!contactProfile.getFirstWengoId().empty() && contactProfile.isAvailable()) {
		IMContact imContact = contactProfile.getFirstAvailableWengoIMContact();
		if ( (imContact.getPresenceState() != EnumPresenceState::PresenceStateOffline) &&
				(imContact.getPresenceState() != EnumPresenceState::PresenceStateUnknown) &&
				(imContact.getPresenceState() != EnumPresenceState::PresenceStateUnavailable)) {
	
				return true;
		}
	}
	return false;
}

/**
 * Helper method to clone a key event, so that it can be send to another widget
 */
static QKeyEvent* cloneKeyEvent(QKeyEvent* event) {
	return new QKeyEvent(event->type(), event->key(), event->modifiers(), event->text());
}

bool QtChatWidget::eventFilter(QObject* object, QEvent* event) {
	if (object == _ui.chatHistory && event->type() == QEvent::KeyPress) {
		return historyKeyPressEventFilter(static_cast<QKeyEvent*>(event));
	}
	if (object == _ui.chatEdit && event->type() == QEvent::KeyPress) {
		return editKeyPressEventFilter(static_cast<QKeyEvent*>(event));
	}
	return false;
}

bool QtChatWidget::historyKeyPressEventFilter(QKeyEvent* event) {
	// Set focus on edit widget if the user types a "printable" character
	if (event->text().size() > 0 && event->text().at(0).isPrint() && _ui.chatEdit->isEnabled()) {
		_ui.chatEdit->setFocus();
		QKeyEvent* newEvent = cloneKeyEvent(event);
		QApplication::postEvent(_ui.chatEdit, newEvent);
		return true;
	}
	return false;
}

bool QtChatWidget::editKeyPressEventFilter(QKeyEvent* event) {
	int key = event->key();
	
	if (key == Qt::Key_PageUp || key == Qt::Key_PageDown) {
		QKeyEvent* newEvent = cloneKeyEvent(event);
		QApplication::postEvent(_ui.chatHistory, newEvent);
		return true;
	}

	// Send message with Enter key, unless a modifier is pressed
	if ((key == Qt::Key_Enter || key == Qt::Key_Return) && 
		(event->modifiers() == Qt::NoModifier || event->modifiers() == Qt::KeypadModifier)) 
	{
		sendMessage();
		return true;
	}

#ifdef OS_MACOSX
	static const int REAL_CTRL_MODIFIER = Qt::MetaModifier;
#else
	static const int REAL_CTRL_MODIFIER = Qt::ControlModifier;
#endif
	bool realCtrlPressed = event->modifiers() & REAL_CTRL_MODIFIER;

	if (key == Qt::Key_Tab && realCtrlPressed) {
		// We use realCtrlPressed because on MacOS, Qt::ControlModifier
		// corresponds to the Command key and Command + Tab is used to switch
		// between applications, (like Alt + Tab on Windows)
		ctrlTabPressed();
		return true;
	}

	return false;
}
