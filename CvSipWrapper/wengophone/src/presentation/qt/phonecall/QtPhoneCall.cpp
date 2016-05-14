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

#include "QtPhoneCall.h"

#include "ui_PhoneCallWidget.h"

#ifdef XV_HWACCEL
	#include "QtVideoXV.h"
#endif

//zhenHua.sun 2010-09-17
#include "..\conference\QtToolWidget.h"
#include "..\conference\QtSipCall.h"
//end

#include "QtVideoQt.h"
#include "QtPhoneCallEventFilter.h"
#include "../toaster/QtCallToaster.h"
#include "../QtDialpad.h"

#include "QtAudioSmileysWindow.h"

#include <presentation/qt/QtWengoPhone.h>
#include <presentation/qt/QtToolBar.h>
#include <presentation/qt/statusbar/QtStatusBar.h>
#include <presentation/qt/contactlist/QtContactList.h>
#include <presentation/qt/profile/QtProfileDetails.h>
#include "presentation/qt/profilebar/QtProfileBar.h"
//#include "presentation/qt/profilebar/QtIMProfileWidget.h"		精简zhenHua.sun 2010-09-03
#include <presentation/qt/conference/QtConferenceCallWidget.h>
#include <presentation/qt/conference/QtConferenceRoomWidget.h>

#include <control/CWengoPhone.h>
#include <control/profile/CUserProfile.h>
#include <control/profile/CUserProfileHandler.h>
#include <control/phonecall/CPhoneCall.h>

#include <model/config/Config.h>
#include <model/config/ConfigManager.h>
#include <model/contactlist/ContactProfile.h>
#include <model/phonecall/ConferenceCall.h>
#include <model/phonecall/PhoneCall.h>
#include <model/phoneline/PhoneLine.h>
#include <model/profile/AvatarList.h>

#include <sipwrapper/CodecList.h>
#include <sipwrapper/SipWrapper.h>

#include <cutil/global.h>

#include <util/Logger.h>
#include <util/SafeDelete.h>

#include <qtutil/SafeConnect.h>
#include <qtutil/WidgetRatioEnforcer.h>
#include <qtutil/WidgetUtils.h>

#include <QtCore/QMutex>
#include <QtCore/QTime>
#include <QtCore/QTimer>

#include <QtGui/QMenu>
#include <QtGui/QPainter>

// If a PSTN number is shorter than this length, we assume it's a special sip
// account, like 333 for Wengo
static const int PSTN_NUMBER_MIN_LENGTH = 4;

static const int LOCAL_FRAME_WIDTH = 64;
static const int LOCAL_FRAME_HEIGHT = 64;
static const int LOCAL_FRAME_MARGIN = 7;

// See wifo/phapi/phmedia-video.c
static const int NOWEBCAM_FILL_VALUE = 128;

static const int NOWEBCAM_SAMPLE_COUNT = 200;

// Minimum width of buttons shown when we are in "talking" state. Defined to avoid square buttons
// under MacOS X
static const int TALKING_BUTTON_MIN_WIDTH = 50;

/**
 * Helper function to determine if image is a real webcam or not:
 * If the remote user has no webcam, then wifo will send us a yuv image filled
 * with NOWEBCAM_FILL_VALUE bytes.
 */
static bool isRealWebcamVideoFrame(piximage* image) {
	if (image->palette != PIX_OSI_YUV420P) {
		return true;
	}

	int step = (image->width * image->height * 3 / 2) / NOWEBCAM_SAMPLE_COUNT;

	for (int sample=0; sample < NOWEBCAM_SAMPLE_COUNT; ++sample) {
		if (image->data[sample * step] != NOWEBCAM_FILL_VALUE) {
			return true;
		}
	}

	return false;
}

static void setButtonAction(QPushButton* button, QAction* action) {
	button->disconnect();
	SAFE_CONNECT_RECEIVER(button, SIGNAL(clicked()), action, SLOT(trigger()));
	button->setIcon(action->icon());
	button->setEnabled(action->isEnabled());
	button->setToolTip(action->toolTip());
}

static void flashMainWindow(QWidget* window) {
	if (window->isActiveWindow()) {
		return;
	}

	if (!window->isVisible()) {
		// Make sure the window appears on the taskbar,
		// otherwise it won't flash...
		//window->showMinimized();
		window->showMaximized();     //modified by dhx 2010-10-28
	}

	WidgetUtils::flashWindow(window);
}

QtPhoneCall::QtPhoneCall(CPhoneCall & cPhoneCall)
	: QObject(NULL),
	_cPhoneCall(cPhoneCall) {

	_qtWengoPhone = QtPhoneCall::_cPhoneCall.getCWengoPhone()._qtWengoPhone;//Modify By LZY 2010-09-28
	_videoWindow = NULL;
	_closed = false;
	_duration = 0;

	_callToaster = NULL;
	_remoteVideoFrame = NULL;
	_localVideoFrame = NULL;
	_hold = false;
	_videoMode = VM_None;

	_mutex = new QMutex(QMutex::Recursive);

	_phoneCallWidget = new QWidget(NULL);

	_ui = new Ui::PhoneCallWidget();
	_ui->setupUi(_phoneCallWidget);
	_ui->encryptionFrame->hide();
	//_ui->videoContainer->hide();
	_ui->labelStack->setCurrentWidget(_ui->avatarPage);
	_ui->buttonStack->setCurrentWidget(_ui->ringingPage);

	_phoneCallWidget->setAutoFillBackground(true);

	std::string tmpDisplayName = _cPhoneCall.getPhoneCall().getPeerSipAddress().getDisplayName();
	QString userName = QString::fromUtf8(tmpDisplayName.c_str());

	if (userName.isEmpty()) {
		userName = QString::fromStdString(_cPhoneCall.getPhoneCall().getPeerSipAddress().getUserName());
	}
	QPixmap avatarPixmap = loadContactAvatar(userName);
	initAvatarLabel(avatarPixmap);


	Config & config = ConfigManager::getInstance().getCurrentConfig();

	// On MacOSX getVideoEnable causes quite a lot of work, like listing the
	// available devices. Therefore we keep the result instead of asking it
	// everytime it's needed.
	_videoEnabledInConfig = config.getVideoEnable();

	//init flip
	PhoneLine & phoneLine = dynamic_cast < PhoneLine & > (_cPhoneCall.getPhoneCall().getPhoneLine());
	phoneLine.flipVideoImage(config.getVideoFlipEnable());
	////

	userName = getDisplayName(userName);

#ifndef OS_MACOSX
	//With Qt4.2, the button background is set to _phoneCallWidget background
	//color on Windows and Linux, setting autoFillBackground to true prevents
	//this.
	//We don't want to do this on OSX, because on this platform setting
	//autoFillBackground to true paints horizontal stripes behind the buttons.
	Q_FOREACH(QPushButton* button, _phoneCallWidget->findChildren<QPushButton*>()) {
		button->setAutoFillBackground(true);
	}
	////
#endif

	Q_FOREACH(QPushButton* button, _ui->talkingPage->findChildren<QPushButton*>()) {
		button->setMinimumWidth(TALKING_BUTTON_MIN_WIDTH);
	}

	// Set bold ourself: if we do it from Designer it alters the font name (at
	// least with Qt 4.1)
	QFont font(_ui->nickNameLabel->font());
	font.setBold(true);
	_ui->nickNameLabel->setFont(font);

	_ui->nickNameLabel->setText(userName);

	updateNickNameToolTip();

	_ui->statusLabel->setToolTip(tr("Status"));

	// Accept call
	SAFE_CONNECT(_ui->acceptButton, SIGNAL(clicked()), SLOT(acceptCall()));

	// Reject call
	SAFE_CONNECT(_ui->rejectButton, SIGNAL(clicked()), SLOT(rejectCall()));

	// Tweak ui when we are the caller
	if (!isIncoming()) {
		_ui->acceptButton->hide();
		_ui->rejectButton->setText(tr("Ca&ncel"));
	}

	// Hang up
	_actionHangupCall = new QAction(QIcon(":/pics/actions/hangup-phone.png"), tr("Hang-up"), _phoneCallWidget);
	SAFE_CONNECT(_actionHangupCall, SIGNAL(triggered()), SLOT(rejectCall()));
	setButtonAction(_ui->hangupButton, _actionHangupCall);
	
	//Hold
	_actionHold = new QAction(_phoneCallWidget);
	SAFE_CONNECT(_actionHold, SIGNAL(triggered()), SLOT(holdOrResume()));
	_ui->holdButton->hide();
	//Add contact
	_actionAddContact = new QAction(QIcon(":/pics/actions/add-contact.png"), tr("Add contact"), _phoneCallWidget);
	SAFE_CONNECT(_actionAddContact, SIGNAL(triggered()), SLOT(addContact()));
	setButtonAction(_ui->addContactButton, _actionAddContact);
	_ui->addContactButton->hide();
	////

	SAFE_CONNECT(_ui->dialpadButton, SIGNAL(toggled(bool)), SLOT(toggleDialpad(bool)) );
	_ui->dialpadButton->hide();
	SAFE_CONNECT(_ui->smileysButton, SIGNAL(toggled(bool)), SLOT(toggleSmileys(bool)) );
	SAFE_CONNECT(_ui->createConf, SIGNAL(clicked()), SLOT(createConf()) );
	_ui->createConf->hide();


	//Computes the call duration
	_callTimer = new QTimer(_phoneCallWidget);
	SAFE_CONNECT(_callTimer, SIGNAL(timeout()), SLOT(updateCallDuration()));

	QtPhoneCallEventFilter * filter = new QtPhoneCallEventFilter(_phoneCallWidget);
	_phoneCallWidget->installEventFilter(filter);

	flashMainWindow(_qtWengoPhone->getWidget());
	showToaster(userName, avatarPixmap);

	if (!_cPhoneCall.getPhoneCall().getConferenceCall()) {
		_qtWengoPhone->addPhoneCall(this);

		//zhenHua.sun 2010-09-17
		preview_win_hwnd = _qtWengoPhone->getQtToolWidget()->getQtSipCall()->getSipCallPreviewWnd();
	} else {
		hideConfButton();
		_qtWengoPhone->addToConference(this);
	}

	updateStatusLabel();
	updateHoldAction();

	//update toolbar (menus)
	if (_qtWengoPhone) {
		_qtWengoPhone->getQtToolBar().updateMenuActions();
	}
	////

	_ui->acceptButton->setFocus();
}

QtPhoneCall::~QtPhoneCall() {
	if (_remoteVideoFrame) {
		pix_free(_remoteVideoFrame);
	}
	if (_localVideoFrame) {
		pix_free(_localVideoFrame);
	}

	OWSAFE_DELETE(_ui);
	OWSAFE_DELETE(_mutex);
}

void QtPhoneCall::initAvatarLabel(const QPixmap& pixmap) {
	if (pixmap.hasAlpha()) {
		// Paint avatar over a background if it's transparent (doing so for
		// full opaque images looks ugly)
		QPixmap bg(":/pics/phonecall/avatar_background.png");
		QPainter painter(&bg);
		painter.drawPixmap(
			(bg.width() - pixmap.width()) / 2,
			(bg.height() - pixmap.height()) / 2,
			pixmap);
		painter.end();
		_ui->avatarLabel->setPixmap(bg);
	} else {
		_ui->avatarLabel->setPixmap(pixmap);
	}

	_ui->labelStack->setCurrentWidget(_ui->avatarPage);
}

QPixmap QtPhoneCall::loadContactAvatar(const QString& userName) {

	QPixmap pixmap;

	QtContactList * qtContactList = _qtWengoPhone->getQtContactList();
	if (qtContactList) {
		CContactList & cContactList = qtContactList->getCContactList();

		IMContact imContact(EnumIMProtocol::IMProtocolWengo, userName.toStdString());
		std::string contactId = cContactList.findContactThatOwns(imContact);
		ContactProfile contactProfile = cContactList.getContactProfile(contactId);

		std::string data = contactProfile.getIcon().getData();
		if (!data.empty()) {
			pixmap.loadFromData((uchar *) data.c_str(), data.size());
		}
	}

	if (pixmap.isNull()) {
		// User is not in contact list, or has no avatar defined, use default
		// avatar instead
		std::string data = AvatarList::getInstance().getDefaultAvatarPicture().getData();
		pixmap.loadFromData((uchar*) data.c_str(), data.size());
	}

	return pixmap;
}


void QtPhoneCall::updateHoldAction() {
	if (_hold) {
		_actionHold->setToolTip(tr("Resume"));
		_actionHold->setIcon(QIcon(":/pics/actions/resume-phone.png"));
	} else {
		_actionHold->setToolTip(tr("Hold"));
		_actionHold->setIcon(QIcon(":/pics/actions/hold-phone.png"));
	}
	setButtonAction(_ui->holdButton, _actionHold);
}


void QtPhoneCall::updateNickNameToolTip() {
	QString toolTip = QString::fromStdString(_cPhoneCall.getPeerSipAddress());
	if (!_codecs.isEmpty()) {
		toolTip += QString(" (%1)").arg(_codecs);
	}
	_ui->nickNameLabel->setToolTip(toolTip);
}


QString QtPhoneCall::getDisplayName(QString str) {
	QString tmp;

	int begin = str.indexOf("sip:", 0, Qt::CaseInsensitive);
	if (begin == -1) {
		// Not found, return ...
		return str;
	}
	begin += 4;
	int end = str.indexOf("@", begin, Qt::CaseInsensitive);
	if (end == -1) {
		//Not found, return ...
		return str;
	}
	tmp = str.mid(begin, end - begin);
	return tmp;
}

void QtPhoneCall::stateChangedEvent(EnumPhoneCallState::PhoneCallState state) {
	std::string codecs;
	if (_cPhoneCall.getAudioCodecUsed() != CodecList::AudioCodecError) {
		codecs += CodecList::toString(_cPhoneCall.getAudioCodecUsed());
	}
	if (_cPhoneCall.getVideoCodecUsed() != CodecList::VideoCodecError) {
		codecs += "/" + CodecList::toString(_cPhoneCall.getVideoCodecUsed());
	}
	_codecs = QString::fromStdString(codecs);
	updateNickNameToolTip();

	updateStatusLabel();

	//update toolbar (menus)
	if (_qtWengoPhone) {
		_qtWengoPhone->getQtToolBar().updateMenuActions();
	}

	switch (state) {
	case EnumPhoneCallState::PhoneCallStateTalking:
		_ui->buttonStack->setCurrentWidget(_ui->talkingPage);
		_duration = 0;
		_callTimer->start(1000);
		_actionHangupCall->setEnabled(true);

		_ui->encryptionFrame->setVisible(isCallEncrypted());

		startedTalking(this);
		break;

	case EnumPhoneCallState::PhoneCallStateDialing:
	case EnumPhoneCallState::PhoneCallStateRinging:
	case EnumPhoneCallState::PhoneCallStateRingingStart:
	case EnumPhoneCallState::PhoneCallStateRingingStop:
	case EnumPhoneCallState::PhoneCallStateIncoming:
		_actionHangupCall->setEnabled(true);
		break;

	case EnumPhoneCallState::PhoneCallStateHold:
		_hold = true;
		updateHoldAction();
		break;

	case EnumPhoneCallState::PhoneCallStateResumed:
		_hold = false;
		updateHoldAction();
		break;

	case EnumPhoneCallState::PhoneCallStateUnknown:
	case EnumPhoneCallState::PhoneCallStateError:
	case EnumPhoneCallState::PhoneCallStateMissed:
	case EnumPhoneCallState::PhoneCallStateRedirected:
		break;

	case EnumPhoneCallState::PhoneCallStateClosed:
		LOG_FATAL("should never reach this case since PPhoneCall::close() is done for this purpose");
		break;

	default:
		LOG_FATAL("unknown PhoneCallState=" + EnumPhoneCallState::toString(state));
	}
}

/************************************************************************///Modify by zhuang 08-12-28
/*
void QtPhoneCall::initVideo(piximage* remoteVideoFrame, piximage* localVideoFrame) {
	_remoteVideoFrame = remoteVideoFrame;
	_localVideoFrame = localVideoFrame;

	if (!isRealWebcamVideoFrame(remoteVideoFrame)) {
		if (_videoEnabledInConfig) {
			_videoMode = VM_LocalOnly;
		} else {
			_videoMode = VM_None;
		}
		return;
	}

	// We only get there if we receive a real video from the remote user

	if (_videoEnabledInConfig) {
		_videoMode = VM_Both;
	} else {
		_videoMode = VM_RemoteOnly;
	}
	// Hide avatar label to save space for the video
	//_ui->avatarLabel->hide();

#ifdef XV_HWACCEL
	if (config.getXVideoEnable()) {
		_videoWindow = new QtVideoXV(_phoneCallWidget, remoteVideoFrame->width, remoteVideoFrame->height,
			localVideoFrame->width, localVideoFrame->height);
		if (!_videoWindow->isInitialized()) {
			OWSAFE_DELETE(_videoWindow);
		}
	}
#endif

	// We can fallback from a failure in QtVideoXV
	if (!_videoWindow) {
		_videoWindow = new QtVideoQt(_ui->videoContainer);
		SAFE_CONNECT(_videoWindow, SIGNAL(toggleFlipVideoImageSignal()), SLOT(toggleFlipVideoImage()));
	}

	// Init videoContainer
	double ratio = remoteVideoFrame->width / double(remoteVideoFrame->height);
	_ui->videoContainer->setRatio(ratio);
	_ui->videoContainer->setChild(_videoWindow->getWidget());
	QTimer::singleShot(0, _videoWindow->getWidget(), SLOT(show()));
	//_ui->videoContainer->show();
	_ui->labelStack->setCurrentWidget(_ui->videoPage);
}
*/
void QtPhoneCall::initVideo(piximage* remoteVideoFrame, piximage* localVideoFrame) {

	int frame_width = 352;
	int frame_height = 288;
	// We can fallback from a failure in QtVideoXV
	if (!_videoWindow) {
		_videoWindow = new QtVideoQt(_ui->videoContainer);
		SAFE_CONNECT(_videoWindow, SIGNAL(toggleFlipVideoImageSignal()), SLOT(toggleFlipVideoImage()));
	}
	// Init videoContainer
	double ratio = frame_width / double(frame_height);
	_ui->videoContainer->setRatio(ratio);
	_ui->videoContainer->setChild(_videoWindow->getWidget());
	QTimer::singleShot(0, _videoWindow->getWidget(), SLOT(show()));
	//_ui->videoContainer->show();
	
	_ui->labelStack->setCurrentWidget(_ui->videoPage);
}

/***********************************************************************************************/

void QtPhoneCall::videoFrameReceivedEvent(piximage * remoteVideoFrame, piximage * localVideoFrame) {

	QMutexLocker locker(_mutex);

	//FIXME hack to prevent a crash
	if (_closed) {
		return;
	}

	if (_videoMode == VM_None) {
		initVideo(remoteVideoFrame, localVideoFrame);
	}

	if (_videoMode == VM_RemoteOnly || _videoMode == VM_Both) {
		_videoWindow->showImage(remoteVideoFrame, localVideoFrame);
	}

	if (_videoMode == VM_LocalOnly) {
		QImage image;
		QSize size(_localVideoFrame->width, _localVideoFrame->height);
		size.scale(LOCAL_FRAME_WIDTH, LOCAL_FRAME_HEIGHT, Qt::KeepAspectRatio);
		if (size.width() & 1) {
			size.setWidth(size.width() + 1);
		}
		if (size.height() & 1) {
			size.setHeight(size.height() + 1);
		}
		QtVideoQt::convertPixImageToQImage(_localVideoFrame, size, &image);
		QPixmap avatarPix = *_ui->avatarLabel->pixmap();
		QPainter painter(&avatarPix);
		painter.drawImage(
			avatarPix.width() - size.width() - LOCAL_FRAME_MARGIN,
			avatarPix.height() - size.height() - LOCAL_FRAME_MARGIN,
			image);
		painter.end();
		_ui->avatarLabel->setPixmap(avatarPix);
	}
}

/************************************************************************///Add by zhuang 08-12-26
/*                                                                      *///modify by zhuang 09-01-06
void QtPhoneCall::videoFrameReceivedEvent(void * video_hwnd, piximage * localVideoFrame) {

	QMutexLocker locker(_mutex);

	//FIXME hack to prevent a crash
	if (_closed) {
		return;
	}
//	media_cbk_s *media_callbk = (media_cbk_s *)video_hwnd;
	initVideo(NULL, NULL);
//	qt_media_callbak.ResizeCbk = media_callbk->ResizeCbk;

	_videoWindow->getWinId(video_hwnd,preview_win_hwnd);
	QString title = _ui->nickNameLabel->text();
	_videoWindow->setUserName(_ui->nickNameLabel->text());
//	*video_hwnd =(HWND)_videoWindow->getWidget()->winId();
	//if (_videoMode == VM_None) {
	//	initVideo(remoteVideoFrame, localVideoFrame);
	//}

	//if (_videoMode == VM_RemoteOnly || _videoMode == VM_Both) {
	//	_videoWindow->showImage(remoteVideoFrame, localVideoFrame);
	//}

	//if (_videoMode == VM_LocalOnly) {
	//	QImage image;
	//	QSize size(_localVideoFrame->width, _localVideoFrame->height);
	//	size.scale(LOCAL_FRAME_WIDTH, LOCAL_FRAME_HEIGHT, Qt::KeepAspectRatio);
	//	if (size.width() & 1) {
	//		size.setWidth(size.width() + 1);
	//	}
	//	if (size.height() & 1) {
	//		size.setHeight(size.height() + 1);
	//	}
	//	QtVideoQt::convertPixImageToQImage(_localVideoFrame, size, &image);
	//	QPixmap avatarPix = *_ui->avatarLabel->pixmap();
	//	QPainter painter(&avatarPix);
	//	painter.drawImage(
	//		avatarPix.width() - size.width() - LOCAL_FRAME_MARGIN,
	//		avatarPix.height() - size.height() - LOCAL_FRAME_MARGIN,
	//		image);
	//	painter.end();
	//	_ui->avatarLabel->setPixmap(avatarPix);
	//}
}

//���nickNameLabel����
QString QtPhoneCall::getNickNameLabelText()
{
	return  _ui->nickNameLabel->text();
}
/************************************************************************/

void QtPhoneCall::acceptCall() {
	_ui->statusLabel->setText(tr("Initialization..."));
	_cPhoneCall.accept();
	if (_callToaster) {
		_callToaster->close();
	}
}

void QtPhoneCall::rejectCall() {
	LOG_DEBUG("phone call hangup");
	switch (_cPhoneCall.getState()) {
	case EnumPhoneCallState::PhoneCallStateClosed:
	case EnumPhoneCallState::PhoneCallStateError:
	case EnumPhoneCallState::PhoneCallStateResumed:
	case EnumPhoneCallState::PhoneCallStateTalking:
	case EnumPhoneCallState::PhoneCallStateDialing:
	case EnumPhoneCallState::PhoneCallStateRinging:
	case EnumPhoneCallState::PhoneCallStateRingingStart:
	case EnumPhoneCallState::PhoneCallStateRingingStop:
	case EnumPhoneCallState::PhoneCallStateIncoming:
	case EnumPhoneCallState::PhoneCallStateHold:
	case EnumPhoneCallState::PhoneCallStateRedirected:
		_cPhoneCall.hangUp();
		break;
	default:
		LOG_DEBUG("call rejected");
	}
}

void QtPhoneCall::holdOrResume() {
	/*if (_hold) {
	_cPhoneCall.resume();
	} else {
	_cPhoneCall.hold();
	}
	updateHoldAction();
	*/    //wzf 08-12-29
}

void QtPhoneCall::updateCallDuration() {
	_duration++;
	updateStatusLabel();
}

void QtPhoneCall::updateStatusLabel() {
	QString text;
	switch (_cPhoneCall.getState()) {
	case EnumPhoneCallState::PhoneCallStateError:
		text = tr("Error");
		break;

	case EnumPhoneCallState::PhoneCallStateResumed:
	case EnumPhoneCallState::PhoneCallStateTalking:
		if (_cPhoneCall.getPhoneCall().getConferenceCall()) {
			text = tr("Talking - Conference");
		} else {
			text = tr("Talking");
		}
		break;

	case EnumPhoneCallState::PhoneCallStateDialing:
		text = tr("Dialing");
		break;

	case EnumPhoneCallState::PhoneCallStateRinging:
		text = tr("Ringing");
		break;

	case EnumPhoneCallState::PhoneCallStateRingingStart:
		text = tr("Ringing");
		break;

	case EnumPhoneCallState::PhoneCallStateIncoming:
		text = tr("Incoming Call");
		break;

	case EnumPhoneCallState::PhoneCallStateHold:
		text = tr("Hold");
		break;

	case EnumPhoneCallState::PhoneCallStateRedirected:
		text = tr("Redirected");
		break;

	default:
		break;
	}
	_ui->statusLabel->setText(text);

	QTime time;
	time = time.addSecs(_duration);
	_ui->durationLabel->setText(time.toString(Qt::TextDate));
}

bool QtPhoneCall::isIncoming() const {
	return (_cPhoneCall.getState() == EnumPhoneCallState::PhoneCallStateIncoming);
}

void QtPhoneCall::showToaster(const QString & userName, const QPixmap& pixmap) {
	Config & config = ConfigManager::getInstance().getCurrentConfig();

	if (!isIncoming()) {
		return;
	}

	//Shows toaster for incoming chats?
	if (!config.getNotificationShowToasterOnIncomingCall()) {
		return;
	}

	OWSAFE_DELETE(_callToaster);
	_callToaster = new QtCallToaster();
	_callToaster->setMessage(userName);
	_callToaster->setPixmap(pixmap);

	SAFE_CONNECT(_callToaster, SIGNAL(pickUpButtonClicked()), SLOT(acceptCall()));
	SAFE_CONNECT(_callToaster, SIGNAL(hangUpButtonClicked()), SLOT(rejectCall()));
	_callToaster->show();
}

void QtPhoneCall::close() {
	//FIXME hack to prevent a crash
	_closed = true;

	if (_callToaster) {
		_callToaster->close();
	}

	_callTimer->disconnect();
	_callTimer->stop();
	OWSAFE_DELETE(_callTimer);

	_actionHangupCall->setEnabled(false);
	_ui->statusLabel->setText(tr("Closed"));
	if (_videoWindow) {
		if (_videoWindow->isFullScreen()) {
			_videoWindow->unFullScreen();
		}
	}

	//update toolbar (menus)
	if (_qtWengoPhone) {
		_qtWengoPhone->getQtToolBar().updateMenuActions();
	}
	////

	//TODO: disconnect from flipWebcamButtonClicked
	OWSAFE_DELETE(_videoWindow);
	OWSAFE_DELETE(_phoneCallWidget);
	deleteLater();
}

void QtPhoneCall::toggleFlipVideoImage() {
	Config & config = ConfigManager::getInstance().getCurrentConfig();
	PhoneLine & phoneLine = dynamic_cast < PhoneLine & > (_cPhoneCall.getPhoneCall().getPhoneLine());
	bool flip = !config.getVideoFlipEnable();
	phoneLine.flipVideoImage(flip);
	config.set(Config::VIDEO_ENABLE_FLIP_KEY, flip);
}

/**
 * Helper function trying to determine if a username is a PSTN number.
 */
static bool isPSTNUserName(const QString& userName) {
	for (int pos=0; pos < userName.size(); ++pos) {
		if (!userName[0].isDigit()) {
			return false;
		}
	}

	return userName.size() >= PSTN_NUMBER_MIN_LENGTH;
}

void QtPhoneCall::addContact() {
	CWengoPhone & cWengoPhone = _cPhoneCall.getCWengoPhone();
	CUserProfile* cUserProfile =  cWengoPhone.getCUserProfileHandler().getCUserProfile();
	if (!cUserProfile) {
		LOG_WARN("No user profile defined. This should not happen");
		return;
	}

	ContactProfile contactProfile;
	QtProfileDetails qtProfileDetails(*cUserProfile, contactProfile, _phoneCallWidget->window(), tr("Add a Contact"));

	// Fill some QtProfileDetails fields
	PhoneCall& call = _cPhoneCall.getPhoneCall();
	SipAddress sipAddress = call.getPeerSipAddress();

	QString rawSipAddress = QString::fromStdString(sipAddress.getRawSipAddress());
	Config & config = ConfigManager::getInstance().getCurrentConfig();
	QString wengoRealm = QString::fromStdString( config.getWengoRealm() );
	if (rawSipAddress.contains(wengoRealm)) {
		// Wengo server
		QString userName;
		userName = QString::fromUtf8(sipAddress.getDisplayName().c_str());
		if (userName.isEmpty()) {
			userName = QString::fromUtf8(sipAddress.getUserName().c_str());
		}
		if (isPSTNUserName(userName)) {
			// User name is the phone number
			qtProfileDetails.setHomePhone(userName);
		} else {
			// User name is the name of the Wengo account
			qtProfileDetails.setWengoName(userName);
		}
	} else {
		// External SIP server
		QString address = QString::fromStdString(sipAddress.getSipAddress());
		if (address.startsWith("sip:")) {
			address = address.mid(4);
		}
		qtProfileDetails.setSipAddress(address);
	}
	////

	if (qtProfileDetails.show()) {
		// WARNING: if the user hang up while the dialog is shown, 'this' won't
		// be valid anymore. Therefore we must not use any field of 'this'
		// here.
		cUserProfile->getCContactList().addContact(contactProfile);
	}
}

bool QtPhoneCall::isCallEncrypted() {
	return _cPhoneCall.isCallEncrypted();
}


/**
 * Hide the other audio smileys window if it's open and uncheck its button
 */
static void hideOtherPopupWindow(QPushButton* button, QtAudioSmileysWindow* window) {
	button->blockSignals(true);
	button->setChecked(false);
	button->blockSignals(false);
	if (window) {
		window->hide();
	}
}

/**
 * Show an audio smileys window
 */
static void togglePopupWindow(QtAudioSmileysWindow* window, bool on) {
	if (on) {
		window->show();
	} else {
		if (window) {
			window->hide();
		}
	}
}

void QtPhoneCall::toggleDialpad(bool on) {
	if (_ui->smileysButton->isChecked()) {
		hideOtherPopupWindow(_ui->smileysButton, _smileysWindow.get());
	}
	if (on) {
		if (!_dialpadWindow.get()) {
			CDtmfThemeManager& manager = _qtWengoPhone->getCWengoPhone().getCDtmfThemeManager();
			_dialpadWindow.reset( new QtAudioSmileysWindow(manager, _qtWengoPhone) );
			_dialpadWindow->dialpad()->setThemeMode(QtDialpad::ThemeModeDefaultOnly);
			_dialpadWindow->setButton(_ui->dialpadButton);
		}
	}

	togglePopupWindow(_dialpadWindow.get(), on);
}

void QtPhoneCall::toggleSmileys(bool on) {
	/************************************************************************///modify by zhuang 09-01-07
	/*����video����                                                         */
	/*
	if (_ui->dialpadButton->isChecked()) {
		hideOtherPopupWindow(_ui->dialpadButton, _dialpadWindow.get());
	}
	if (on) {
		if (!_smileysWindow.get()) {
			CDtmfThemeManager& manager = _qtWengoPhone->getCWengoPhone().getCDtmfThemeManager();
			_smileysWindow.reset( new QtAudioSmileysWindow(manager, _qtWengoPhone) );
			_smileysWindow->dialpad()->setThemeMode(QtDialpad::ThemeModeAudioSmileysOnly);
			_smileysWindow->setButton(_ui->smileysButton);
		}
	}

	togglePopupWindow(_smileysWindow.get(), on);*/
	QString iconName;
	if (on)
	{
		iconName = ":/pics/video_attach.png";
	}
	else
	{
		iconName = ":/pics/video_detach.png";
	}
	if(_videoWindow!=NULL)
		_videoWindow->toggleAttachDetach();
//	_ui->smileysButton->setIcon(QIcon(iconName));
	/************************************************************************/
}

void QtPhoneCall::createConf()
{
	QString address = QString::fromStdString(_cPhoneCall.getPeerSipAddress());
	
	if(address.contains("@"))
		address = address.section("@",0,0);

	if (address.startsWith("sip:")) {
		address = address.mid(4);
	}

	//

	QtConferenceCallWidget conferenceDialog(_qtWengoPhone->getWidget(), _qtWengoPhone->getCWengoPhone(), &_cPhoneCall.getPhoneCall().getPhoneLine(),false);
	conferenceDialog.setFirstPeer(address.toStdString());
	if(conferenceDialog.exec())
	{
		if(address != conferenceDialog.getSecondPeer())
		{
			_ui->createConf->hide();
			_qtWengoPhone->addToConference(conferenceDialog.getSecondPeer(),&_cPhoneCall.getPhoneCall());
		}
	}
}

void QtPhoneCall::hideConfButton()
{
	_ui->createConf->hide();
}
