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

#ifndef OWQTWENGOPHONE_H
#define OWQTWENGOPHONE_H

#include <model/phoneline/EnumMakeCallError.h>
#include <model/network/NetworkProxyDiscovery.h>

#include <imwrapper/EnumPresenceState.h>

#include <qtutil/QObjectThreadSafe.h>

#include <cutil/global.h>
#include <util/Trackable.h>

#include <QtGui/QMainWindow>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QTabWidget>
#include <QtGui/QPushButton>

#include <string>
#include <memory>

class CWengoPhone;
class Config;
class ContactInfo;
class IMContact;
class PPhoneLine;
class PhoneCall;
class PresenceHandler;
class QtCallBar;
class QtContactCallListWidget;
class QtContactList;
class QtFileTransfer;
class QtHistoryWidget;
class QtIdle;
class QtLanguage;
class QtPhoneCall;
class QtProfileBar;
class QtSms;
class QtStatusBar;
class QtToolBar;
class QtSystray;
class QtToaster;
class QtBrowserWidget;
class QtDialpad;
class UserProfile;
class QtPhoneComboBox;
class QtIMAccountMonitor;
class QtConferenceRoomWidget;

class QtLoginDialog; //Add by hexianfu  09-11-9
class ILogin;  //Add by hexianfu  09-11-27
class QMessageBox;//Add by hexianfu  10-4-20

class QtToolWidget; //zhenHua.sun 2010-08-19

#if (defined OS_WINDOWS) && (QT_EDITION == QT_EDITION_DESKTOP)
class QtWebDirectory;
#endif

class QWidget;
class QMenu;


namespace Ui { class WengoPhoneWindow; }

using namespace std;//Add By LZY 2010-09-27

//-------Add By LZY 2010-09-20_2010-09-25-------
//   -   延后返回动作列表相关值定义
#define ACTION_LIST_LEN 9 //列表长度
#define ACTION_LOGIN 0 //Login动作
#define ACTION_ENTERMEETING 1 //EnterMeeting动作
#define ACTION_CONTACTMEMBER 2 //ContactMember动作
#define ACTION_QUITMEETING 3 //QuitMeeting动作
#define ACTION_LOGOFF 4 //LogOff动作
#define ACTION_RAISEHAND 5 //RaiseHand动作
#define ACTION_PERMITSPEAK 6 //PermitSpeak动作
#define ACTION_KICKMEMBER 7 //KickMember动作
#define ACTION_CREATEMEETING 8 //CreateMeeting动作
//-------End of Add By LZY-----------

//-------Add By LZY 2010-09-20-------
//   -   Action执行结果定义
#define ARESULT_SUCCESS 0 //动作执行成功
#define ARESULT_RUNNING 1 //同样的动作正在执行中（延后返回动作会有此返回值）
#define ARESULT_FAIL 2 //动作执行失败（如登陆失败等）
#define ARESULT_FORBIDDEN 3 //在当前的终端状态下该动作不允许执行（延后返回动作会有此返回值，如未登陆状态下想刷新会议列表将会返回此消息）
//-------End of Add By LZY-----------

//-------Add By LZY 2010-09-19_2010-10-05-------
//   -   终端界面状态值定义，4字节，高16位按位分配，表示界面的驻态（程序维持着的状态），低16位按值分配，表示界面的瞬态（操作完成瞬间）
//    ---驻态值定义（按位）
#define UI_S_ALL 4294901760 //高16位全定义1，表示包含所有驻态
#define UI_S_INIT_BEGIN 2147483648 //终端初始化中
#define UI_S_LOGOFF 1073741824 //终端处于未登陆状态
#define UI_S_LOGOFF_BEGIN 536870912 //终端正在登出中
#define UI_S_LOGIN_BEGIN 268435456 //终端正在登陆中
#define UI_S_LOGIN 134217728 //终端已登陆
#define UI_S_JOINMEETING_BEGIN 67108864 //终端正在加入某个会议
#define UI_S_MEETING 33554432 //终端已加入某个会议
#define UI_S_EXITMEETING_BEGIN 16777216 //终端正在退出会议
//    ---瞬态值定义（按值，范围[1-65535]）
#define UI_D_ALL 65535 //低16位全定义1，表示包含所有瞬态
#define UI_D_CONTACTMEMBER_SUCCESS 1 //动作ContactMember执行完成
#define UI_D_CONTACTMEMBER_ERROR 2 //动作ContactMember执行中遇到错误
#define UI_D_RAISEHAND_SUCCESS 3 //动作RaiseHand执行完成
#define UI_D_PERMITSPEAK_SUCCESS 4 //动作PermitSpeak执行完成
#define UI_D_PERMITSPEAK_ERROR 5 //动作PermitSpeak执行中遇到错误
#define UI_D_KICKMEMBER_SUCCESS 6 //动作KickMember执行完成
#define UI_D_KICKMEMBER_ERROR 7 //动作KickMember执行中遇到错误
#define UI_D_CREATEMEETING_SUCCESS 8 //动作CreateMeeting执行完成
#define UI_D_LOCALVIDEO_SEND_START 9 //动作LocalVideoSetting允许本地视频发送
#define UI_D_LOCALVIDEO_SEND_STOP 10 //动作LocalVideoSetting禁止本地视频发送
#define UI_D_LOCALAUDIO_SEND_START 11 //动作LocalAudioSetting允许本地视频发送
#define UI_D_LOCALAUDIO_SEND_STOP 12 //动作LocalAudioSetting禁止本地视频发送
//-------End of Add By LZY-----------

//-------Add By LZY 2010-10-06-------------
#define CHANGE_MEETINGLIST 0 //会议列表变更
#define CHANGE_MEMBERLIST 1 //与会人员列表变更
#define CHANGE_LAYOUT 2 //视频布局变更
#define CHANGE_UI 3 //界面驻态变更
#define CHANGE_INVITE 4 //收到邀请
#define CHANGE_KICK 5 //被踢出会议
#define CHANGE_CONTACT 6 //联系人信息发生变化
#define CHANGE_LOCALVIDEO 7 //本地视频的发送设置发生了变化
#define CHANGE_LOCALAUDIO 8 //本地音频的发送设置发生了变化
//-------End of Add By LZY----------------
/**
 * Qt Presentation component for WengoPhone.
 *
 * @author Tanguy Krotoff
 */
class QtWengoPhone : public QObjectThreadSafe, public Trackable {
	Q_OBJECT

		friend class QtLoginDialog;       //Add by dhx 10-10-25

public:

	QtWengoPhone(CWengoPhone & cWengoPhone);

	~QtWengoPhone();

	/**
	 * @brief 获取主窗口
	 */
	QMainWindow*  getWengoPhoneWindow(){  return _wengoPhoneWindow; }


	void addPhoneCall(QtPhoneCall * qtPhoneCall);

	void addToConference(QtPhoneCall * qtPhoneCall);

	/************************************************************************/
	/*加入会议模式*/
	void addToConference();
	void deleteConference();
	/************************************************************************/

	void updatePresentation();

	void dialpad(const std::string & tone);

	void connectionStatusEventHandler(int totalSteps, int currentStep, const std::string & infoMsg);

	QWidget * getWidget() const;

	void setQtDialpad(QtDialpad * qtDialpad);

	void setQtContactList(QtContactList * qtContactList);
	QtContactList * getQtContactList() const;

	void setQtHistoryWidget(QtHistoryWidget * qtHistoryWidget);

	void setQtSms(QtSms * qtSms);
	QtSms * getQtSms() const;

	QtStatusBar & getQtStatusBar() const;

	QtFileTransfer * getFileTransfer() const;

	QtToolBar & getQtToolBar() const;

	QtCallBar & getQtCallBar() const;

	QtSystray & getQtSystray() const;

	QtLanguage & getQtLanguage() const;

	QtProfileBar * getQtProfileBar() const;

	CWengoPhone & getCWengoPhone() const;

	QtConferenceRoomWidget * getQtConferenceRoomWidget();

#if (defined OS_WINDOWS) && (QT_EDITION == QT_EDITION_DESKTOP)
	QtWebDirectory * getQtWebDirectory() const;
#endif

	void setChatWindow(QWidget * chatWindow);
	QWidget * getChatWindow() const;
        void closeChatWindow ();

	void showHistory();

	void ensureVisible();

	void showAddContact(ContactInfo contactInfo);

	virtual void bringMainWindowToFront();

	void installQtBrowserWidget();

	void uninstallQtBrowserWidget();

	QtBrowserWidget * getQtBrowserWidget() const;

	/**
	* Add by: hexianfu
	* for the telecontroller commandProcess
	* 2009-10-22
	*/
	QtLoginDialog & getQtLoginDialog() const;

	void setQtLoginDialog(QtLoginDialog * qtLoginDialog);

	/**
	 * @author zhenHua.sun
	 * @date 2010-08-19
	 * @brief 设置工具菜单栏
	 */
	void setQtToolWidget( QtToolWidget* qtToolWidget );
	QtToolWidget* getQtToolWidget() const;
	
	int AddInviteMsg(QMessageBox *MsgBoxPointer);//Add By LZY 2010-09-27 添加新的邀请信息框指针到消息框指针队列中

	char *getPrivateMeetPwd();

	void exitEvent();//Move By LZY 2010-09-27

	CWengoPhone & _cWengoPhone;//Move By LZY From Private To Public 2010-09-25

	//-------Add By LZY 2010-09-19_2010-09-30-------
	//---函数---
	void SendInfo(char *Message);//2010-11-02 发送信息给teleController，目前使用DBus机制管理发送，不需要锁
	void sendTeleStateChanged(int StateIndex,const char *AddInfo=NULL);//发送终端主动通告的消息
	void ExitMeetingSuccess();//响应UserProfile发出的ExitMeetingSuccessEvent事件，更改UI_State_Code设置值
	void SetUIState(unsigned long StateCode);//设置界面状态标记，并发送相应信息给teleController，当界面状态变化了，应该调用本函数
	void commandProcess(const char *recvedMess);//动作处理函数
	void SaveGlobalSetting(string IPv4Addr,string IPv6Addr);//保存GlobalSetting地址值，界面与遥控器统一调用的接口
	void RemoveAllMsg();//移除所有当前正在显示的信息框（邀请确认，注销确认等），所有信息框将执行reject操作

	//---信号---
Q_SIGNALS:
	void RefreshAudioOutSliderSignal(int Value);//更新控制界面中输出音频滚动条的信号，由于QtAudioSetting中没有特定的接口设置，只好加入这个Signal来解决
	void RefreshAudioInSliderSignal(int Value);//更新控制界面中输入音频滚动条的信号，由于QtAudioSetting中没有特定的接口设置，只好加入这个Signal来解决
	void ActionLoginSignal(QString LoginParam);//触发Login动作的信号
	void ActionRefreshMeetingListSignal();//触发RefreshMeetingList动作的信号
	void ActionEnterMeetingSignal(QString EnterMeetingParam);//触发EnterMeeting动作的信号
	void ActionContactMemberSignal(QString ContactMemberName);//触发ContactMember动作的信号
	void ActionStopMemberSignal(QString userName);//触发StopMember动作的信号
	void ActionQuitMeetingSignal();//触发QuitMeeting动作的信号
	void ActionLogOffSignal();//触发LogOff动作的信号
	void ActionLocalControlSignal();//触发LocalControl动作的信号
	void ActionSetVolumeSignal(int VolumeValue,char VolumeType);//触发SetVolume动作的信号
	void ActionRaiseHandSignal();//触发RaiseHand动作的信号
	void ActionPermitSpeakSignal(QString userName);//触发PermitSpeak动作的信号
	void ActionKickMemberSignal(QString userName);//触发KickMember动作的信号
	void ActionSetLayoutTypeSignal(QString LayoutType);//触发SetLayoutType动作的信号
	void ActionInviteMembersSignal(QString InviteMembers);//触发InviteMembers动作的信号
	void ActionAcceptInviteSignal(int InviteIdentifyNum,int MethodNum);//触发AcceptInvite动作的信号
	void ActionCreateMeetingSignal(QString Title,QString Description,int ControlModeNum,int JoinModeNum,QString Psw);//触发CreateMeeting动作的信号
	void ActionLocalVideoSettingSignal();//触发LocalVideoSetting动作的信号
	void ActionLocalAudioSettingSignal();//触发LocalAudioSetting动作的信号
	//-------End of Add By LZY----------

public Q_SLOTS:

	void currentUserProfileWillDieEventHandlerSlot();

	void userProfileInitializedEventHandlerSlot();

	void hangUpButtonClicked();

	void prepareToExitApplication();

	void addToConference(QString phoneNumber, PhoneCall * targetCall);

	void setCurrentUserProfileEventHandlerSlot();

	/*
	* Add by huixin.du for delete conference tab  2010-09-14
	* 检测dialWidget中是否还有呼叫
	*/
    void hideDialWidget();

	//-------Add By LZY 2010-09-28_2010-10-05-------
	void RemoveInviteMsg(int IdentifyNum,int AdditionMethod=0);//移除指定索引位置的信息框指针，如果AdditionMethod不为0，将在移除前对该消息框执行进一步的操作
	void CreateMeeting(QString Title,QString Description,int ControlModeNum,int JoinModeNum,QString Psw);//创建会议用函数，遥控器和界面通用
	//-------End of Add By LZY-----------

private Q_SLOTS:

	void callButtonClicked();

	void enableCallButton();

	void phoneComboBoxClicked();

	void closeWindow();

	void languageChanged();

	void setActiveTabBeforeCallCurrent();

Q_SIGNALS:

	/**
	 * Current user profile has been deleted.
	 *
	 * Graphical components should be re-initialized:
	 * QtContactList, QtHistoryWidget, QtProfileBar are removed.
	 * QtSystray, QtBrowserWidget have to be re-initialized.
	 */
	void userProfileDeleted();

private:

	//-------Add By LZY 2010-09-19_2010-10-05-------
	//---结构体---
	struct ActionListNode//动作队列结构及相关变量定义，队列中的每一格都与一个需要延后返回的动作绑定，绑定值由本文件开头的一组值定义
	{
		int IndexNum;//teleController中动作接收等待队列对应的索引值
		int AvailableNum;//teleController中动作接收等待队列对应的有效值
		bool Using;//使用中标记，由于对此变量置true只会在一条线程中发生，所以不用考虑互斥问题
		ActionListNode()
		{
			IndexNum=0;
			AvailableNum=0;
			Using=false;
		}
	};

	//---函数---
	void sendTeleResult(const char* actionName,int Result,int TIndex,int TAvailable,const char * Response=NULL);//立即返回的Action发送动作结果函数
	void ResponseDelayAction(string ActionName,int ActionIndex,int Result=ARESULT_SUCCESS,char * ResponsePointer=NULL);//处理延后响应Action的函数，函数会自动判断传入的ActionIndex所对应Action是否需要发送回复信息
	bool CheckDelayAction(char *ActionName,unsigned long EnabledStateCode,int ActionListIndex,char * RunMode,int TIndex,int TAvailableNum);//检查延后返回动作是否满足执行条件，当不满足时自动向teleController发送动作返回
	bool CheckImitAction(char *ActionName,unsigned long EnabledStateCode,char *RunMode,int TIndex,int TAvailableNum);//检查立即返回动作是否满足执行条件，当不满足时自动向teleController发送动作返回

	//---变量---
	unsigned int UI_State_Code;//记录当前界面所处的状态，便于分析动作是否执行，各值定义由本文件前面的“UI_”开头的预定义值定义
	ActionListNode ActionList[ACTION_LIST_LEN];
	//-------End of Add By LZY----------

	void initUi();

	void initThreadSafe();

	//-------Add By LZY 2010-09-27-------
	void LockInviteList(long LockNum);//InviteList互斥访问锁加锁函数
	bool UnLockInviteList(long UnLockNum);//InviteList互斥访问锁解锁函数
	//-------End of Add By LZY-----------

	/**
	 * Set the geometry of the mainwindow (position and size).
	 *
	 * Load last size and position. If the mainwindow is not visible use default values.
	 */
	void mainWindowGeometry(Config & config);

	/**
	 * Initializes pickup and hangup buttons inside the main window.
	 *
	 * This is called by QtPhoneCall to re-initialize the buttons
	 * since QtPhoneCall modifies the behaviour of this buttons.
	 */
	void initCallButtons();

	void updatePresentationThreadSafe();

	void proxyNeedsAuthenticationEventHandler(NetworkProxyDiscovery & sender, NetworkProxy networkProxy);

	void proxyNeedsAuthenticationEventHandlerThreadSafe(NetworkProxy networkProxy);

	void wrongProxyAuthenticationEventHandler(NetworkProxyDiscovery & sender, NetworkProxy networkProxy);

	void makeCallErrorEventHandler(EnumMakeCallError::MakeCallError);

	void makeCallErrorEventHandlerThreadSafe(EnumMakeCallError::MakeCallError);


#ifdef OS_MACOSX
	void fixMacOSXMenus();
#endif

	void loadStyleSheets();

	/** Direct link to the control. */

	Ui::WengoPhoneWindow * _ui;

	QMainWindow * _wengoPhoneWindow;

	QtSystray * _qtSystray;

	QtSms * _qtSms;

	QtContactList * _qtContactList;

	QtHistoryWidget * _qtHistoryWidget;

	QtContactCallListWidget * _qtContactCallListWidget;

	QtIdle * _qtIdle;

	QtLanguage * _qtLanguage;

	QtToolBar * _qtToolBar;

	QtStatusBar * _qtStatusBar;

	QtFileTransfer * _qtFileTransfer;

	QtBrowserWidget * _qtBrowserWidget;

	int _qtBrowserWidgetTabIndex;

	QWidget * _chatWindow;

	QWidget * _activeTabBeforeCall;

	QTabWidget * _dialTabWidget;      //huixin.du 2010-09-15

	QPushButton * _toolButton;

	QString currentActionName; // Add by li.zhenning for Action Timeout at 2010-08-19

	QtLoginDialog *_qtLoginDialog;

	//-------Add By LZY 2010-09-27-------
	struct InviteListNode
	{
		QMessageBox * MsgPointer;//邀请信息框指针
		int IdentifyNum;//节点唯一识别号
	};
	InviteListNode * InviteList;
	int InviteListCount;//统计当前创建的InviteList节点数
	int InviteListNextIdentify;//下一个可用的Identify值
	long InviteListLockNum;//InviteList互斥锁锁值，0表示未加锁
	//-------End of Add By LZY-----------

	std::string teleInviteConference;

	char *privateMeetingPwd;

	std::auto_ptr<QtIMAccountMonitor> _qtIMAccountMonitor;
	QtConferenceRoomWidget * m_qtConferenceRoomWidget;


#if (defined OS_WINDOWS) && (QT_EDITION == QT_EDITION_DESKTOP)
	QtWebDirectory * _qtWebDirectory;
#endif

	//工具菜单栏  zhenHua.sun 2010-08-19
	QtToolWidget* _qtToolWidget;
};

#endif	//OWQTWENGOPHONE_H
