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

#ifndef OWQTPHONECALL_H
#define OWQTPHONECALL_H

#include <presentation/PPhoneCall.h>

#include <pixertool/pixertool.h>

#include <QtCore/QObject>

#include <memory>
#include <Windows.h>





class PhoneCall;
class CPhoneCall;
class QtVideo;
class QtWengoPhone;
class QtCallToaster;
class QtAudioSmileysWindow;

class QMenu;
class QAction;
class QTimer;
class QWidget;
class QMutex;
class QPixmap;
namespace Ui { class PhoneCallWidget; }

/**
 * Qt Presentation component for PhoneCall.
 *
 * @author Tanguy Krotoff
 * @author Jerome Wagner
 * @author Mathieu Stute
 * @author Philippe Bernery
 */
class QtPhoneCall : public QObject, public PPhoneCall {
	Q_OBJECT
public:
	enum VideoMode { VM_None, VM_LocalOnly, VM_RemoteOnly, VM_Both };

	QtPhoneCall(CPhoneCall & cPhoneCall);

	virtual ~QtPhoneCall();

	void updatePresentation() { }

	QWidget * getWidget() const {
		return _phoneCallWidget;
	}

	CPhoneCall & getCPhoneCall() const {
		return _cPhoneCall;
	}

	bool isIncoming() const;

	void stateChangedEvent(EnumPhoneCallState::PhoneCallState state);

	void videoFrameReceivedEvent(piximage * remoteVideoFrame, piximage * localVideoFrame);

	/************************************************************************///Add by zhuang 08-12-26
	/*                                                                      */
	void videoFrameReceivedEvent(void *video_hwnd, piximage * localVideoFrame);
	QString getNickNameLabelText();
	/************************************************************************/

	void close();

	void hideConfButton();
	/**
	 * @see PhoneCall::isCallEncrypted
	 */
	bool isCallEncrypted();

Q_SIGNALS:
	void startedTalking(QtPhoneCall*);

public Q_SLOTS:

	void holdOrResume();

	void acceptCall();

	void rejectCall();

	void toggleFlipVideoImage();

private Q_SLOTS:

	void updateCallDuration();
	void updateStatusLabel();

	void addContact();

	void toggleSmileys(bool);

	void toggleDialpad(bool);

	void createConf();

private:
	void initVideo(piximage* remoteVideoFrame, piximage* localVideoFrame);

	void showToaster(const QString & userName, const QPixmap& avatarPixmap);

	static QString getDisplayName(QString str);

	QMenu * createMenu() const;

	QMenu * createInviteMenu() const;

	void updateHoldAction();

	void updateNickNameToolTip();

	QPixmap loadContactAvatar(const QString& userName);

	void initAvatarLabel(const QPixmap& pixmap);

	Ui::PhoneCallWidget * _ui;

	QWidget * _phoneCallWidget;

	CPhoneCall & _cPhoneCall;

	QtWengoPhone * _qtWengoPhone;

	QtVideo * _videoWindow;

	QAction * _actionHangupCall;

	QAction * _actionHold;

	QAction * _actionAddContact;

	/** Computes the call duration. */
	QTimer * _callTimer;

	int _duration;

	bool _hold;

	VideoMode _videoMode;

	QString _codecs;

	//buffers to free in desctructor
	piximage * _remoteVideoFrame;
	piximage * _localVideoFrame;

	//FIXME hack in order to prevent a crash
	bool _closed;
	
	bool _videoEnabledInConfig;

	/** protect videoFrameReceivedEvent */
	QMutex * _mutex;

	QtCallToaster * _callToaster;

	std::auto_ptr<QtAudioSmileysWindow> _dialpadWindow;

	std::auto_ptr<QtAudioSmileysWindow> _smileysWindow;

	/************************************************************************///Add by zhuang 09-01-06
	/*previewµÄ¾ä±ú                                                         */
	HWND preview_win_hwnd;
	/************************************************************************/

};

#endif	//OWQTPHONECALL_H
