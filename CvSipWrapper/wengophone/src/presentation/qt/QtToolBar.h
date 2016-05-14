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

#ifndef OWQTTOOLBAR_H
#define OWQTTOOLBAR_H

#include <QtCore/QObject>

#include <util/NonCopyable.h>

#include <model/webservices/url/WsUrl.h>

class QtWengoPhone;
class CWengoPhone;

class QAction;
class QMenu;
class QWidget;

namespace Ui { class WengoPhoneWindow; }

/**
 * Tool bar and menu actions from the main window.
 *
 * @author Tanguy Krotoff
 */
class QtToolBar : public QObject, NonCopyable {
	Q_OBJECT
public:

	QtToolBar(QtWengoPhone & qtWengoPhone, Ui::WengoPhoneWindow * qtWengoPhoneUi, QWidget * parent);

	~QtToolBar();

	void closeConfRoom();//Add By LZY 2010-11-04 关闭视频界面

	QWidget * getWidget() const;


	/**
	 * Makes sure the toggle actions (like show/hide hidden contacts and
	 * show/hide groups) are correctly translated
	 */
	void retranslateUi();

	/**
	 * Configure toolbar according to the settings from the config file
	 */
	void updateToolBar();
    
	/**
	* Add by: hexianfu
	* for the telecontroller
	* 2009-10-21
	*/
	void teleCreateConferenceCall();

	//void teleLogOff();//Delete By LZY 2010-09-26

	//void teleQuit();//Delete By LZY 2010-09-26

	bool getToolBarState();

	//void teleGlobalService();//Delete By LZY 2010-09-26
	/******************************/

Q_SIGNALS:
	void configChanged(QString key);

	/**
	* Add by: hexianfu
	* for the telecontroller
	* 2009-10-21
	*/
	//void teleCreateConferenceCallSignal();
	/********************************/

public Q_SLOTS:

	void showWengoAccount();

	void editMyProfile();

	void addContact();

	void showConfig();

	void showWengoForum();

	void showAbout();

	void sendSms();

	void showWengoFAQ();

	void showWengoServices();

	void showHideContactGroups();

	void showHideOfflineContacts();

	void createConferenceCall();

	void clearHistoryOutgoingCalls();

	void clearHistoryIncomingCalls();

	void clearHistoryMissedCalls();

	void clearHistoryRejectedCalls();

	void clearHistoryChatSessions();

	void clearHistorySms();

	void clearHistoryAll();

	void searchWengoContact();

	void logOff();

	void acceptCall();

	void holdResumeCall();

	void hangUpCall();

	void showChatWindow();

	void showFileTransferWindow();

	void toggleVideo();

	void slotConfigChanged(QString key);
	
	void userProfileIsInitialized();

	void enableMenuBar();

	void updateMenuActions();

	void tryingToConnect();

private Q_SLOTS:

	/**
	 * the userProfile has been deleted
	 */
	void userProfileDeleted();

private:
	void updateActionsAccordingToUserStatus();

	/**
	 * Ugly code to remove.
	 */
	int findFirstCallTab();

	void updateShowHideContactGroupsAction();

	void updateShowHideOfflineContactsAction();

	void updateToggleVideoAction();

	void configChangedEventHandler(const std::string & key);

	/**
	 * If page exists, connect action to receiver::slot, otherwise, remove
	 * action from any widget it's connected to (menu, toolbar...)
	 */
	void connectOrHide(WsUrl::Page page, QAction* action, QObject* receiver, const char* slot);


	/**
	 * If condition is true, connect action to receiver::slot, otherwise,
	 * remove action from any widget it's connected to (menu, toolbar...)
	 */
	void connectOrHide(bool condition, QAction* action, QObject* receiver, const char* slot);
	
	QtWengoPhone & _qtWengoPhone;

	CWengoPhone & _cWengoPhone;

	Ui::WengoPhoneWindow * _ui;

	bool _userIsLogged;

	//是否在已开启会议窗口//Add by zhuang 09-04-13
	bool isStartConferenceWindow;
};

#endif	//OWQTTOOLBAR_H
