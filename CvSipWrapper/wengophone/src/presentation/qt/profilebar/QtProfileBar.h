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

#ifndef QTPROFILEBAR_H
#define QTPROFILEBAR_H

#include <model/webservices/info/WsInfo.h>

#include <imwrapper/EnumPresenceState.h>
#include <imwrapper/IMAccount.h>

#include <util/Trackable.h>

#include <QtGui/QFrame>
#include <QtGui/QPixmap>
#include <QtCore/QMap>

class QGridLayout;

class QtIMAccountMonitor;
class QtIMProfileWidget;
class QtEventWidget;
class QtCreditWidget;

class CHistory;
class CUserProfile;
class CWengoPhone;
class ConnectHandler;
class IPhoneLine;
class PresenceHandler;
class UserProfile;
class WengoStyleLabel;

/**
 * Profile bar: shows the user profile at the top of the main window.
 *
 * TODO refactor, class too big, no .ui file ect...
 *
 * @author Mathieu Stute
 * @author Tanguy Krotoff
 */
class QtProfileBar : public QFrame, public Trackable {
	Q_OBJECT
public:

	QtProfileBar(QWidget * parent);

	void init(CWengoPhone*, CUserProfile*, QtIMAccountMonitor*);

	void reset();

	~QtProfileBar();

public Q_SLOTS:

	void statusClicked();

	void nicknameClicked();

	void creditClicked();

Q_SIGNALS:
	void wsInfoWengosEvent(float wengos);
	void wsInfoVoiceMailEvent(int count);
	void wsInfoLandlineNumberEvent(const QString & number);
	void wsCallForwardInfoEvent(const QString & mode);
	void phoneLineCreatedEvent();

private Q_SLOTS:

	void wsInfoWengosEventSlot(float wengos);
	void wsInfoVoiceMailEventSlot(int count);
	void wsInfoLandlineNumberEventSlot(const QString & number);
	void wsCallForwardInfoEventSlot(const QString & mode);
	void phoneLineCreatedEventSlot();

	void updateStatusLabel();

private:

	/**
	 * Initializes widgets content.
	 */
	void init();

	void wsInfoCreatedEventHandler(UserProfile & sender, WsInfo & wsInfo);

	void wsInfoWengosEventHandler(WsInfo & sender, int id, WsInfo::WsInfoStatus status, float wengos);

	void wsInfoVoiceMailEventHandler(WsInfo & sender, int id, WsInfo::WsInfoStatus status, int voicemail);

	void wsInfoLandlineNumberEventHandler(WsInfo & sender, int id, WsInfo::WsInfoStatus status, std::string number);

	void wsCallForwardInfoEventHandler(WsInfo & sender, int id, WsInfo::WsInfoStatus status,
		WsInfo::WsInfoCallForwardMode mode, bool voicemail, std::string dest1, std::string dest2, std::string dest3);

	void phoneLineCreatedEventHandler(UserProfile & sender, IPhoneLine & phoneLine);

	void createStatusMenu();

	void paintEvent(QPaintEvent * event);

	void toggleLabel(WengoStyleLabel*);

	WengoStyleLabel * _statusLabel;

	WengoStyleLabel * _nicknameLabel;

	WengoStyleLabel * _creditLabel;

	WengoStyleLabel * _currentLabel;

	QMap<WengoStyleLabel*, QWidget*> _labelWidget;

	QtIMProfileWidget * _qtImProfileWidget;

	QtCreditWidget * _creditWidget;

	QPixmap _statusPixmap;

	CUserProfile * _cUserProfile;

	CWengoPhone * _cWengoPhone;

	QtIMAccountMonitor* _qtImAccountMonitor;

public:
	QtIMProfileWidget * getQtIMProfileWidget(){return _qtImProfileWidget;}
};

#endif	//QTPROFILEBAR_H
