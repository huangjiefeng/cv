/*
 * WengoPhone, a voice over Internet phone
 * Copyright (C) 2004-2007 Wengo
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

#ifndef OWUSERPROFILE_H
#define OWUSERPROFILE_H

#include "Profile.h"
#include "IMAccountManager.h"

#include <imwrapper/EnumPresenceState.h>

#include <model/account/wengo/WengoAccount.h>
#include <model/history/History.h>
#include <model/phoneline/IPhoneLine.h>
#include <model/webservices/callforward/WsCallForward.h>
#include <model/conference/CVMsgParser.h>

#include <thread/Condition.h>
#include <thread/RecursiveMutex.h>

#include <util/Event.h>
#include <util/String.h>
#include <util/Trackable.h>

#include <list>
#include <vector>
#include <Windows.h>

class ChatHandler;
class CoIpManager;
class ConnectHandler;
class Contact;
class ContactList;
class History;
class IMAccount;
class IMAccountManager;
class IMContactListHandler;
class IPhoneLine;
class NetworkObserver;
class PhoneCall;
class PresenceHandler;
class WenboxPlugin;
class WsInfo;
class WsSms;
class WsSoftUpdate;

//记录当前会议状态...zhenHua.sun 2010-09-08
enum ConferenceState
{
    NotInConference,        //没有加入任何会议
    IsInConference,     //加入了一个会议，正在会话中
    ExitingConference,   //用户正在退出会议
    WaitingPermission,  //正在等待控制服务器的许可，未加入会议
    JoinRejected,               //加入会议被拒绝
};

/**
 * Handle the profile of a User.
 *
 * @author Philippe Bernery
 */
class UserProfile : public Profile, public Trackable
{
    friend class UserProfileFileStorage;
    friend class UserProfileFileStorage1;
    friend class UserProfileXMLSerializer;
    friend class Connect;
public:
	Event<void ()> ExitMeetingSuccessEvent;//Add By LZY 2010-09-30 退出会议成功时发送通知事件
    /**
     * Login procedure is done, event with the procedure result.
     *
     * @see SipAccount::loginStateChangedEvent
     */
    Event<void (SipAccount&sender, EnumSipLoginState::SipLoginState state)> loginStateChangedEvent;

    /**
     * Network event while trying to connect a SipAccount.
     *
     * @see SipAccount::networkDiscoveryStateChangedEvent
     */
    Event<void (SipAccount&sender, SipAccount::NetworkDiscoveryState state)> networkDiscoveryStateChangedEvent;

    /**
     * Emitted when SipAccount has been connected
     */
    Event<void ()> sipAccountConnectedEvent;

    /**
     * Emitted when SipAccount failed to connect
     */
    Event<void (EnumSipLoginState::SipLoginState)> sipAccountConnectionFailedEvent;

    /**
     * The history has been loaded.
     *
     * @param sender this class
     * @param History History loaded
     */
    Event<void (UserProfile&sender, History&history)> historyLoadedEvent;

    /**
     * A PhoneLine has been created.
     *
     * @param sender this class
     * @param phoneLine PhoneLine created
     */
    Event<void (UserProfile&sender, IPhoneLine&phoneLine)> phoneLineCreatedEvent;

    /**
     * Sms has been created.
     *
     * @param sender this class
     * @param sms Sms created
     */
    Event<void (UserProfile&sender, WsSms&sms)> wsSmsCreatedEvent;

    /**
     * SoftUpdate has been created.
     *
     * @param sender this class
     * @param sms Sms created
     */
    Event<void (UserProfile&sender, WsSoftUpdate&softUpdate)> wsSoftUpdateCreatedEvent;

    /**
     * WsInfo has been created.
     *
     * @param sender this class
     * @param wsInfo WsInfo created
     */
    Event<void (UserProfile&sender, WsInfo&wsInfo)> wsInfoCreatedEvent;

    /**
     * WsCallForward has been created.
     *
     * @param sender this class
     * @param wsCallForward WsCallForward created
     */
    Event<void (UserProfile&sender, WsCallForward&wsCallForward)> wsCallForwardCreatedEvent;

    UserProfile();

    virtual ~UserProfile();

    /**
     * Initializes the UserProfile.
     */
    void init();

    /**
     * Return the profile directory
     * @return profile directory.
     */
    std::string getProfileDirectory() const;

    /**
     * Connects all this UserProfile accounts (SipAccount and IMAccount) set
     * to be automatically connected.
     */
    void connect();

    /**
     * Disconnects all accounts.
     */
    void disconnect();

    /** List of IPhoneLine. */
    typedef List <IPhoneLine*> PhoneLines;

    /**
     * Gets the list of PhoneLine.
     *
     * Only one PhoneLine is active at a time.
     *
     * @return the list of PhoneLine
     */
    const PhoneLines& getPhoneLineList() const
    {
        return _phoneLineList;
    }

    /**
     * Gets the active/current PhoneLine.
     *
     * Only one PhoneLine is active at a time
     *
     * @return the active PhoneLine
     */
    IPhoneLine* getActivePhoneLine() const
    {
        return _activePhoneLine;
    }

    /**
     * Gets the active phone call.
     *
     * Used for playing DTMF.
     *
     * @return active phone call or NULL
     */
    PhoneCall* getActivePhoneCall() const;

    /** Gets the ConnectHandler reference. */
    ConnectHandler& getConnectHandler()
    {
        return *_connectHandler;
    }

    /** Gets the PresenceHandler reference. */
    PresenceHandler& getPresenceHandler()
    {
        return *_presenceHandler;
    }

    /** Gets the ChatHandler reference. */
    ChatHandler& getChatHandler()
    {
        return *_chatHandler;
    }

    /**
     * Gets the IMAccountManager reference. Here we can safely derefence
     * the pointer as the IMAccountHandler is constructed in
     * the UserProfile constructor.
     */
    IMAccountManager& getIMAccountManager()
    {
        return *_imAccountManager;
    }

    /** Gets the IMContactListHandler reference. */
    IMContactListHandler& getIMContactListHandler()
    {
        return *_imContactListHandler;
    }

    /** Gets the ContactList reference. */
    ContactList& getContactList()
    {
        return *_contactList;
    }

    /**
     * Gets the History reference. Here we can safely derefence the pointer
     * as the History is constructed in the UserProfile constructor.
     */
    History& getHistory()
    {
        return *_history;
    }

    /** Gets the WenboxPlugin pointer. */
    WenboxPlugin* getWenboxPlugin()
    {
        return _wenboxPlugin;
    }

    /** Gets the WsInfo pointer. */
    WsInfo* getWsInfo()
    {
        return _wsInfo;
    }

    /** Gets the WengoPhone update WebService pointer. */
    WsSoftUpdate* getWsSoftUpdate()
    {
        return _wsSoftUpdate;
    }

    /** Gets the SMS WebService pointer. */
    WsSms* getWsSms()
    {
        return _wsSms;
    }

    /** Gets the WsCallForward WebService pointer. */
    WsCallForward* getWsCallForward()
    {
        return _wsCallForward;
    }

    /** Gets the CoIpManager. */
    CoIpManager& getCoIpManager()
    {
        return *_coIpManager;
    }

    /** True if history has been loaded. */
    bool isHistoryLoaded() const
    {
        return _historyLoaded;
    }

    /**
     * Loads the history.
     */
    bool loadHistory(const std::string& path);

    /**
     * Saves the history.
     */
    bool saveHistory(const std::string& path);

    /**
     * Sets the SipAccount of this UserProfile.
     *
     * The SipAccount os copied internally.
     *
     * @param SipAccount the SipAccount to set
     */
    void setSipAccount(const SipAccount& sipAccount, bool needInitialization = true);

    /**
     * Return true if a WengoAccount is active.
     *
     * @return true if a WengoAccount is active
     */
    bool hasWengoAccount() const;

    /**
     * Return true if a SipAccount is active.
     *
     * @return true if a SipAccount is active
     */
    bool hasSipAccount() const;

    /**
     * Gets the SipAccount.
     *
     * @return the SipAccount
     */
    SipAccount* getSipAccount() const
    {
        return _sipAccount;
    }

    /**
     * Gets the WengoAccount.
     *
     * @return the WengoAccount
     */
    WengoAccount* getWengoAccount() const
    {
        if(hasWengoAccount())
        {
            return dynamic_cast<WengoAccount*>(_sipAccount);
        }
        else
        {
            return NULL;
        }
    }

    /**
     * Adds an IMAccount to this UserProfile.
     * This method should currently not be called to add a Wengo
     * IMAccount. A Wengo IMAccount is created internally when setWengoAccount
     * is called. There is an assert to prevent that.
     *
     * The IMAccount is copied internally.
     *
     * @param imAccount the IMAccount to add
     */
    void addIMAccount(const IMAccount& imAccount);

    /**
     * Removes an IMAccount from this UserProfile.
     *
     * An IMAccount of type Wengo must currently not be removed by this method.
     * setWengoAccount must be used for this purpose.
     * There is an assert to prevent that.
     *
     * @param imAccount the IMAccount to remove
     */
    void removeIMAccount(const IMAccount& imAccount);

    /**
     * Updates an IMAccount from this UserProfile.
     *
     * An IMAccount of type Wengo must currently not be removed by this method.
     * setWengoAccount must be used for this purpose.
     * There is an assert to prevent that.
     *
     * @param imAccount the IMAccount to remove
     */
    void updateIMAccount(const IMAccount& imAccount);

    /**
     * @see IPhoneLine::makeCall()
     */
    EnumMakeCallError::MakeCallError makeCall(Contact& contact);

    /**
     * @see IPhoneLine::makeCall()
     */
    EnumMakeCallError::MakeCallError makeCall(const std::string& phoneNumber);

    /**
     * Start a instant messaging with a Contact.
     *
     * @param contact the Contact to talk to
     */
    void startIM(Contact& contact);

    /**
     * Changes alias of this user.
     *
     * @param alias the alias to set
     * @param imAccount the IMAccount to apply the alias to;
     *        pass NULL to set the alias to all IMAccount
     */
    void setAlias(const std::string& alias, IMAccount* imAccount);

    /**
     * Changes icon of this user.
     *
     * @param icon the icon to set
     * @param imAccount the IMAccount to apply the icon to
     *        pass NULL to set the icon to all IMAccount
     */
    void setIcon(const OWPicture& icon, IMAccount* account);

    /**
     * Gets the PresenceState of this UserProfile.
     */
    EnumPresenceState::PresenceState getPresenceState() const;

    /**
     * Checks if at least one IMAccount is in connected state.
     *
     * @return true if at least 1 IMAccount is connected; false otherwise
     */
    bool isConnected() const;

    /**
     * Changes the PresenceState of an IMAccount.
     *
     * @param presenceState the PresenceState to set
     * @param imAccountId the id of the IMAccount to apply the PresenceState.
     */
    void setPresenceState(EnumPresenceState::PresenceState presenceState, std::string imAccountId);

    /**
     * Changes the PresenceState of all IMAccount.
     *
     * @param presenceState the PresenceState to set
     */
    void setAllPresenceState(EnumPresenceState::PresenceState presenceState);

    /**
     * Gets the name of this UserProfile.
     *
     * The name is computed from the SipAccount. "Default" is returned if no
     * SipAccount is set. There should be only one Default UserProfile at
     * a time.
     */
    std::string getName() const
    {
        return _name;
    }

    /************************************************************************/
    /*Start Conference*/
    Event <void (UserProfile&sender, vector<ConfInfo*> _confList)> cvConfListReceivedEvent;
    Event <void (UserProfile&sender, vector<MemberInfo*> _memberList, std::string confName)> cvMemberListReceivedEvent;
    Event <void (UserProfile&sender, ConfInfo*conference, const std::string&from)> cvConfrenceInviteEvent;
    Event <void (UserProfile&sender)> cvPasswordIncorrectEvent;
    Event <void (UserProfile&sender)> cvKickedEvent;
    Event <void (UserProfile&sender, const std::string&Message, const std::string&from)> cvTextMsgReceivedEvent;
    Event <void (UserProfile&sender, const std::string&Message, const std::string&from)> cvWhiteBoardMsgReceivedEvent;
    Event <void (UserProfile&sender)> cvPermitSpeakEvent;
    //
    Event <void (UserProfile&sender)> cvHostUnShareScreenEvent;

    Event <void (UserProfile&sender, int callId, HWND&_previewHwnd)> setCallIdEvent;

	//拒绝加入会议事件，会议uri、拒绝原因
	Event<void (UserProfile&sender, const string& confFocusURI, const string & reason)> cvJoinRejectionEvent;

    void setCallIdEventHandler(int callId);
    void startConference(const std::string& confURI, const std::string& szContent);
    void exitConference(int& callId);

    /**
     * @brief 加入指定会议
     * @param index 会议列表对应的行索引
     * @param 会议URI
     * @return 成功则返回0，返回其他值则表示操作失败：
							-1  需要密码
							-2	 用户已经进入了一个会议，正在开会中（不能自动切换会议）
							-3  用户正在退出会议
							-4  参数错误，比如uri
							-5	 控制服务器不允许加入，可能是带宽原因
							其他	 未知原因
     */
    int makeJoinConfInvite(const int& index);
    int makeJoinConfInvite(const std::string& focus_URI);

    int media_send_start(char* AudioIP, int AudioPort, char* VideoIP, int VideoPort, HWND previewHwnd);
    void media_send_stop(char* AudioIP, int AudioPort, char* VideoIP, int VideoPort);
    int media_recv_start(string userId, char* AudioIP, int AudioPort, char* VideoIP, int VideoPort, char* codecMIME, HWND remoteHwnd, int videoWidth, int videoHeight);

    /**
     * @author zhenHua.sun
     * @date 2010-08-03
     * @brief 重载了media_recv_start函数，增加了音频编解码类型，音频采样频率，样本位数，声道数
     */
    int media_recv_start(string userId,
                         char* AudioIP,
                         int AudioPort,
                         const char* audioCodecMIME,
                         const int audioSampleRate,
                         const int audioBps,
                         const int audioChannels,
                         char* VideoIP,
                         int VideoPort,
                         char* codecMIME,
                         HWND remoteHwnd,
                         int videoWidth,
                         int videoHeight);


    int media_recv_start(int index, HWND remoteHwnd);
    void media_recv_stop(std::string userId);
    //屏幕共享
    int screen_share_send_start(char* ScreenIP, int ScreenPort, int x, int y, int width, int height);
    //取消桌面共享
    void screen_share_send_stop(char* ScreenIP, int ScreenPort);
    //接收桌面共享
    int screen_share_recv_start(char* ScreenIP, int ScreenPort, HWND remoteHwnd, int videoWidth, int videoHeight);
    //取消接收桌面共享
    void screen_share_recv_stop();
    void exit_conference();
    //设置接收按钮的状态
    bool setRecvButtonStatus(int index);

    /* zhenHua.sun 2010-08-03
    //从SDP获得端口、媒体类型等参数
    void getMediaParamFromSdp(int &aport,int &vport,char *mediaType,int &width,int &height,const char *sdp);
    */

    /**
     * @author zhenHua.sun
     * @date 2010-08-03
     * @brief 从SDP中获取媒体类型
     */
    void UserProfile::getMediaParamFromSdp(int& aport,
                                           int& vport,
                                           char* videoCodecMIME,
                                           int& width,
                                           int& height,
                                           char* audioCodecMIME,
                                           int& audioSampleRate,
                                           int& audioBitsPerSample,
                                           int& audioChannels,
                                           const char* sdp);

    //设置本地预览窗口句柄
    void setPreviewWin(HWND& Hwnd);

    Event <void (UserProfile&sender, const std::string&userId)> logoutReceivedEvent;

    //video窗口resize
    void resizeVideoWindow(const std::string& userId, const HWND& remoteHwnd, int fullscreenflag);

    Event <void (UserProfile&sender, const RtpStatItem&rtpstat)> rtpStatRecvEvent;
    void rtpStatRecvEventHandler(const RtpStatItem& rtpstat);

    Event <void (const TranStat&transtat)> tranStatRecvEvent;
    void tranStatRecvEventHandler(const TranStat& transtat);

    //不接收某路音视频
    void unReceiveMedia(const std::string& userId);
    //禁止发言
    void forbidSpeak();
    void forbidSpeak(string userId);
    //允许发言
    void permitSpeak();

    //检查密码，正确则发消息给Focus，错误则返回
    int checkPassword(int& row, std::string& password);

    //检查当前用户是否主持人
    bool hostOrNot();

    //与会人员发言状态，是否正在发言
    int memberSpeakStatus(int row);

    //指定发言人
    int setSpeaker(int row, bool permitOrForbid, int callId);

    //发送Info消息
    void makeInfoCall(int callId, char* szContent);

    //删除与会者
    int KickOut(int row, int callId);

    //发送文本消息
    void sendTextMsg(const int& receiverIndex, const std::string& szContent);
    //举手发言
    int handForSpeak(int row, bool handUpOrDown, int callId);


    //取出请求队列中的第一个人
    string getFirstMember(bool permitOrForbid);

    /**
    * Add by: hexianfu
    * for the telecontroller
    get the latest confinfo and memberinfo
    * 2009-10-29
    */
    vector<ConfInfo*>& getCurrentConfInfoList();

    vector<MemberInfo*>& getCurrentMemberInfoList();



    //控制音视频的起停...zhenHua.sun 2010-08-06
    /**
     * @brief 控制音频的发送
     * @param state 如果是true，则启动音频的发送，否则停止音频的发送
     */
    void controlAudioSend(bool state);
    /**
     * @brief 控制视频的发送
     * @param state 如果是true，则启动视频的发送，否则停止视频的发送
     */
    void controlVideoSend(bool state);

    /*新增方法*/

    //改变DSCP值
    void setSendDSCP(int dscp);

    //获取当前用户或sip账号的uri
    string getCurrentUserSipUri();

	//获取当前登录用户的用户名（SIP URI），从配置文件中读取，如果配置文件错误，则会读出空用户名
	string getCurrentUserName();

	//获取当前用户的成员信息
	MemberInfo * getCurrentUserMemberInfo();

	//获取当前会议状态
	ConferenceState getConferenceState()	const
	{
		return _confState;
	}

	//查看是否正在开会中
	bool checkIsInConference() 
	{
		return _confState ==IsInConference;
	}

	//判断当前会议是否组播会议
	bool checkCurrentIsMulticastConference();

	//获取当前会议的组播地址，如果会议是组播会议，则输出其IP地址等，返回ture，否则返回false
	bool getCurrentMulticastAddress(string& ipaddress);

	std::string joiningMeeting;//Add By LZY 2010-10-09 记录当前加入的会议名

private:    //私有方法

    //构造一条加入会议的sig消息
    string createJoinConferenceMsg(const  string& confUri);

    //加入一个新会议，进行退出当前会议、清空成员列表、启动媒体传输等操作
    int joinConference(const string& curFocusURI);

    //拒绝加入会议，比如控制服务器不允许
    int rejectToJoinConference(const string& curFocusURI, const string& reason);

    //发送一条进入会议是否成功的消息给控制服务器
    void sendLoginAckMsg(bool isSuccess);

    //发送RTP统计给QoS服务器
    void sendRTPStatToQoSServer(const RtpStatItem& rtpstat);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief   发送加入会议消息到QoS模拟器
    ///
    /// @author qhf
    /// @date   2010-08-18
    ///
    /// @param  curFocusURI URI of the current focus.
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    void sendQosJoinConfReport(const string& curFocusURI);

    //发送退出会议消息到QoS模拟器
    void sendQosExitConfReport(const string& curFocusURI);


private:
    /************************************************************************/
    /*  接收到的SIP消息处理函数，先对XML内容解析，再进行相关处理               */
    /************************************************************************/

	//收到所有的SIP消息
    void cvMessageReceivedEventHandler(const std::string& message, const std::string& fromUri);

    //收到ConfList命令SIP消息， 获取会议列表
    void recvConfListMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri);

    //接收到LoginResult命令SIP消息，登入会议
    void recvLoginResultCommandMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri);

    //接收到MemberList命令SIP消息，更新成员列表
    void recvMemberListCommandMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri);

    //接收到LoginNotify命令SIP消息，有人加入会议
    void recvLoginNotifyMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri);

    //接收到LogoutNotify命令SIP消息，有人退出会议
    void recvLogoutNotifyMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri);

    //接收到Invite命令SIP消息，受到邀请
    void recvInviteMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri);

    //接收到ApplyConfResult命令SIP消息，申请即时会议成功
    void recvApplyConfResultMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri);

    //接收到PasswordIncorrect命令SIP消息，密码不正确
    void recvPasswordIncorrectMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri);

    //接收到SetSpeaker命令SIP消息，指定发言人
    void recvSetSpeakerMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri);

    //接收到ForbidSpeaker命令SIP消息，取消发言人
    void recvForbidSpeakerMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri);

    //接收到Text命令SIP消息，收到文本消息
    void recvTextMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri);

    //接收到LoginResult命令SIP消息，被踢出会议
    void recvKickedMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri);

    //接收到WhiteBoard命令SIP消息，收到白板消息
    void recvWhiteBoardMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri);

    //接收到HandUp命令SIP消息，举手发言
    void recvHandUpMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri);

    //接收到HandDown命令SIP消息，取消举手
    void recvHandDownMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri);

    //接收到Receiver命令SIP消息，发送媒体流
    void recvReceiverMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri);

    //接收到ScreenReceiver命令SIP消息，结束媒体流
    void recvScreenReceiverMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri);

    //接收到LoginResult命令SIP消息，发送一路屏幕共享流
    void recvUnShareScreenMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri);

    //接收到Cancel2Receiver命令SIP消息，结束发送一路屏幕共享流
    void recvCancel2ReceiverMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri);

    //接收到ScreenCancel2Receiver命令SIP消息，取消屏幕共享
    void recvScreenCancel2ReceiverMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri);

    //接收到QoS命令SIP消息，qos相关命令
    void recvQoSMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri);

	//接收到QoS 模拟测试命令SIP消息
	void recvQoSTestMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri);

private: //私有变量

    //会议列表
    vector<ConfInfo*> _confList;
    //与会人员列表
    vector<MemberInfo*> _memberList;
 
    //会议状态
    ConferenceState _confState;

	//当前会议URI，包括正在进入，正在开会中、被拒绝加入、正在退出等状态
    string _focusURI;

    //主持人URI
    string _hostURI;

    //会议控制模式
    string _confMode;

    //会议title
    string _confTitle;

    ConfInfo* _currentConfInfo;     //当前会议信息指针，只有正在开会中(_confState= IsInConference)该指针才有效，其他时候为空

    //本地预览窗口句柄
    HWND _previewHwnd;


    //是否已经开启屏幕共享
    bool _isShareScreen;
    //记录发送的地址
    vector<ReceiverInfo> _sendIPList;
    //记录屏幕共享发送的地址
    vector<ReceiverInfo> _screenShareSendIPList;
    //屏幕共享的区域
    RECT _shareRect;
    //请求发言队列
    list<MemberInfo*>_requestForSpeakList;
    //发言队列
    list<MemberInfo*>_speakingList;

    /************************************************************************/

private:        //Wengo的一些原代码

    // Inherited from Profile
    virtual void setWengoPhoneId(const std::string & /*wengoPhoneId*/)
    {
    }
    virtual std::string getWengoPhoneId() const
    {
        return String::null;
    }
    ////

    /**
     * @see WsCallForward::wsCallForwardEvent
     */
    void wsCallForwardEventHandler(WsCallForward& sender, int id, WsCallForward::WsCallForwardStatus status);

    /**
     * Handles SipAccount::loginStateChangedEvent.
     */
    void loginStateChangedEventHandler(SipAccount& sender, EnumSipLoginState::SipLoginState state);
    void loginStateChangedEventHandlerThreadSafe(SipAccount& sender, EnumSipLoginState::SipLoginState state);

    /**
     * Handles SipAccount::networkDiscoveryStateChangedEvent
     */
    void networkDiscoveryStateChangedEventHandler(SipAccount& sender, SipAccount::NetworkDiscoveryState state);

    /**
     * Compute the name of the UserProfile from the SipAccount
     * and set the _name variable.
     */
    void computeName();

    /* Inherited from Profile */
    void setAlias(const std::string& alias)
    {
        _alias = alias;
    };

    void setIcon(const OWPicture& icon)
    {
        setIcon(icon, NULL);
    };
    /**/

    /**
     * Actually adds an IMAccount. Used internally and by addIMAccount after
     * checking the given IMAccount.
     */
    void _addIMAccount(const IMAccount& imAccount);

    /**
     * Actually removes an IMAccount. Used internally and by removeIMAccount after
     * checking the given IMAccount.
     */
    void _removeIMAccount(const IMAccount& imAccount);

    /**
     * Connect all IMAccounts.
     */
    void connectIMAccounts();

    /**
     * Disconnect all IMAccounts.
     */
    void disconnectIMAccounts();

    /**
     * Connect all SipAccounts.
     */
    bool connectSipAccounts();

    /**
     * Disconnect all SipAccounts.
     *
         * @param force if true, disconnect without doing any transactions
     */
    void disconnectSipAccounts(bool force = false);

    /**
     * Initializes the SipAccount.
     */
    void sipAccountInit();

    /**
     * Creates and adds a new PhoneLine given a SipAccount.
     *
     * This is a helper method.
     *
     * @param account SipAccount associated with the newly created PhoneLine
     */
    bool addPhoneLine(SipAccount& account);

    /**
     * find the wengo phone line
     */
    IPhoneLine* findWengoPhoneLine();

	/** The active/current PhoneLine. */
    IPhoneLine* _activePhoneLine;

    /** The active PhoneCall. */
    PhoneCall* _activePhoneCall;

    /** List of PhoneLine. */
    PhoneLines _phoneLineList;

    /** History. */
    History* _history;

    /** WengoPhone update system. */
    WsSoftUpdate* _wsSoftUpdate;

    /** SMS. */
    WsSms* _wsSms;

    /** WsInfo. */
    WsInfo* _wsInfo;

    /** WsCallForward. */
    WsCallForward* _wsCallForward;

    //TODO: create a list of SipAccount
    SipAccount* _sipAccount;

    /** Wenbox. */
    WenboxPlugin* _wenboxPlugin;

    RecursiveMutex _mutex;

    /**
     * True if the UserProfile must connect after intialization of the
     * SipAccount
     */
    bool _sipAccountMustConnectAfterInit;

    bool _sipAccountReadyToConnect;

    bool _historyLoaded;

    IMAccountManager* _imAccountManager;

    IMContactListHandler* _imContactListHandler;

    ConnectHandler* _connectHandler;

    PresenceHandler* _presenceHandler;

    ChatHandler* _chatHandler;

    ContactList* _contactList;

    CoIpManager* _coIpManager;

    /** Name of the UserProfile. */
    std::string _name;
};

#endif  //OWUSERPROFILE_H
