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

#ifndef OWCUSERPROFILE_H
#define OWCUSERPROFILE_H

#include <control/chat/CChatHandler.h>
#include <control/contactlist/CContactList.h>
#include <control/wenbox/CWenboxPlugin.h>

#include <model/account/SipAccount.h>
#include <model/phoneline/EnumMakeCallError.h>
#include <model/conference/CVMsgParser.h>

#include <imwrapper/EnumIMProtocol.h>
#include <imwrapper/IMAccountList.h>

#include <util/Trackable.h>

#include <set>
#include <Windows.h>
#include "RtpStat.h"		  //RTP统计头文件


class CHistory;
class CPhoneLine;
class CSms;
class CSoftUpdate;
class CWengoPhone;
class CWsCallForward;
class History;
class IMAccount;
class IPhoneLine;
class PhoneCall;
class PUserProfile;
class UserProfile;
class WengoAccount;
class WsCallForward;
class WsSms;
class WsSoftUpdate;
class PresenceHandler;

/**
 * Control layer for UserProfile.
 *
 * FIXME: 'init()' must be called on UserProfile before constructing CUserProfile.
 *
 * @author Philippe Bernery
 */
class CUserProfile : public Trackable {
public:

	Event <void (CUserProfile & sender, EnumMakeCallError::MakeCallError)> makeCallErrorEvent;


	CUserProfile(UserProfile & userProfile, CWengoPhone & cWengoPhone);

	~CUserProfile();

	/**
	 * @see UserProfile::disconnect
	 */
	void disconnect();

	/**
	 * @see UserProfile::makeCall()
	 */
	void makeContactCall(const std::string & contactId);

	/**
	 * @see UserProfile::makeCall()
	 */
	void makeCall(const std::string & phoneNumber);

	/**
	 * @see UserProfile::startIM
	 */
	void startIM(const std::string & contactId);

	/**
	 * @see UserProfile::setSipAccount
	 */
	void setSipAccount(const SipAccount & sipAccount);

	/**
	 * Gets the active phone call.
	 *
	 * Used for playing DTMF.
	 *
	 * @return active phone call or NULL
	 */
	PhoneCall * getActivePhoneCall() const;

	/**
	 * Gets the CHistory.
	 *
	 * @return the CHistory
	 */
	CHistory * getCHistory() const {
		return _cHistory;
	}

	/**
	 * Gets the CContactList.
	 *
	 * @return the CContactList
	 */
	CContactList & getCContactList() {
		return _cContactList;
	}

	/**
	 * @return the CWsCallForward object.
	 */
	CWsCallForward * getCWsCallForward() {
		return _cWsCallForward;
	}

	/**
	 * @return the CWengoPhone object.
	 */
	CWengoPhone & getCWengoPhone() {
		return _cWengoPhone;
	}

	/**
	 * Gets the UserProfile.
	 *
	 * TODO: This method should not exist (because it allows the GUI to access
	 * the model directly) and so should be removed ASAP.
	 */
	UserProfile & getUserProfile() {
		return _userProfile;
	}

	/**
	 * Gets the Presentation layer.
	 */
	PUserProfile * getPresentation() {
		return _pUserProfile;
	}

	CPhoneLine * getCPhoneLine() {
		return _cPhoneLine;
	}

	/************************************************************************/
	/*Start Conference*/
	void startConference(const std::string & confURI,const std::string & szContent);
	void applyConference(const std::string & confURI,const std::string & szContent);

	void exitConference(int &callId);

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
	int makeJoinConfInvite(const int & index);
	int makeJoinConfInvite(const std::string & focusURI);

	Event <void (CUserProfile & sender, vector<ConfInfo *> ConfList)> cvConfListReceivedEvent;
	Event <void (CUserProfile & sender, vector<MemberInfo *> MemberList,std::string confTitle)> cvMemberListReceivedEvent;
	Event <void (CUserProfile & sender, ConfInfo *conference,const std::string & from)> cvConfrenceInviteEvent;
	Event <void (CUserProfile & sender)> cvPasswordIncorrectEvent;
	Event <void (CUserProfile & sender)> cvKickedEvent;
	Event <void (CUserProfile & sender, int callId,HWND &previewHwnd)> setCallIdEvent;
	Event <void (CUserProfile & sender, const std::string & Message,const std::string & from)> cvTextMsgReceivedEvent;
	Event <void (CUserProfile & sender, const std::string & Message,const std::string & from)> cvWhiteBoardMsgReceivedEvent;
	//允许发言后出发此事件，将按下的“举手”按钮弹起
	Event <void (CUserProfile & sender)> cvPermitSpeakEvent;
	//
	Event <void (CUserProfile & sender)> cvHostUnShareScreenEvent;


	//拒绝加入会议事件，会议uri、拒绝原因
	Event<void (CUserProfile&sender, const string& confFocusURI, const string & reason)> cvJoinRejectionEvent;

	void cvJoinRejectionEventHandler( const string& confFocusURI, const string & reason);

	void cvConfListReceivedEventHandler(vector<ConfInfo *> ConfList);
	void cvMemberListReceivedEventHandler(vector<MemberInfo *> MemberList,std::string confTitle);
	void cvConfrenceInviteEventHandler( ConfInfo *conference,const std::string & from);
	void cvPasswordIncorrectEventHandler();
	void cvKickedEventHandler();
	void setCallIdEventHandler(int callId,HWND &previewHwnd);
	void cvTextMsgReceivedEventHandler(const std::string & Message,const std::string & from);
	void cvWhiteBoardMsgReceivedEventHandler(const std::string & Message,const std::string & from);
	void cvPermitSpeakEventHandler();
	void cvHostUnShareScreenEventHandler();
	
	
	int media_send_start(char * AudioIP,int AudioPort,char * VideoIP,int VideoPort,HWND previewHwnd);
	void media_send_stop(char * AudioIP,int AudioPort,char * VideoIP,int VideoPort);
	int media_recv_start(char * AudioIP,int AudioPort,char * VideoIP,int VideoPort,char *codecMIME,HWND remoteHwnd);
	int media_recv_start(int index,HWND remoteHwnd);
	void media_recv_stop(std::string userId);
	//屏幕共享
	int screen_share_send_start(char * ScreenIP,int ScreenPort,int x,int y,int width,int height);
	//取消桌面共享
	void screen_share_send_stop(char * ScreenIP,int ScreenPort);
	//接收桌面共享
	int screen_share_recv_start(char * ScreenIP,int ScreenPort,HWND remoteHwnd,int videoWidth,int videoHeight);
	//取消接收桌面共享
	void screen_share_recv_stop();
	void exit_conference();

	Event <void (CUserProfile & sender, const std::string & userId)> logoutReceivedEvent;
	void logoutReceivedEventHandler( const std::string & userId);

	//设置接收按钮的状态
	bool setRecvButtonStatus(int index);
	//设置本地预览窗口句柄
	void setPreviewWin(HWND  &Hwnd);
	//video窗口resize
	void resizeVideoWindow(const std::string &userId,const HWND &remoteHwnd,int fullscreenflag);

	Event <void (CUserProfile & sender, const RtpStatItem &rtpstat)> rtpStatRecvEvent;
	void rtpStatRecvEventHandler(const RtpStatItem &rtpstat);

	Event <void (const TranStat &transtat)> tranStatRecvEvent;
	void tranStatRecvEventHandler( const TranStat & transtat ); 


	//检查密码，正确则发消息给Focus，错误则返回
	int checkPassword(int &row,std::string &password);
	//检查当前用户是否主持人
	bool hostOrNot();

	//与会人员发言状态，是否正在发言
	int memberSpeakStatus(int row);
	//指定发言人
	int setSpeaker(int row,bool permitOrForbid,int callId);

	//发送Info消息
	void makeInfoCall(int callId,char * szContent);

	//删除与会者
	int KickOut(int row,int callId);

	//发送文本消息
	void sendTextMsg(const int &receiverIndex,const std::string &szContent);
	//举手发言
	int handForSpeak(int row,bool handUpOrDown,int callId);
	/************************************************************************/

	/**
	* Add by: hexianfu
	* for the telecontroller
	get the latest confinfo and memberinfo
	* 2009-10-29
	*/
	vector<ConfInfo *>& getCurrentConfInfoList();

	vector<MemberInfo *>& getCurrentMemberInfoList();
	/***********************************/


	//控制音视频的起停...zhenHua.sun 2010-08-06
	/**
	 * @brief 控制音频的发送
	 * @param state 如果是true，则启动音频的发送，否则停止音频的发送
	 */
	void controlAudioSend( bool state );
	/**
	 * @brief 控制视频的发送
	 * @param state 如果是true，则启动视频的发送，否则停止视频的发送
	 */
	void controlVideoSend( bool state );
	//end

private:

	/**
	 * @see UserProfile::loginStateChangedEvent
	 */
	void loginStateChangedEventHandler(SipAccount & sender,
		EnumSipLoginState::SipLoginState state);

	/**
	 * @see UserProfile::networkDiscoveryStateChangedEvent
	 */
	void networkDiscoveryStateChangedEventHandler(SipAccount & sender,
		SipAccount::NetworkDiscoveryState state);

	/**
	 * @see PresenceHandler::authorizationRequestEvent
	 */
	void authorizationRequestEventHandler(PresenceHandler & sender,
		const IMContact & imContact, const std::string & message);

	/**
	 * @see PresenceHandler::authorizationRequestEvent
	 */
	void incomingSubscribeEventHandler(PresenceHandler & sender,
		const std::string & imaccountId,int sid,const std::string & from,const std::string & evtType);

	/**
	 * @see UserProfile::historyLoadedEvent
	 */
	void historyLoadedEventHandler(History & sender);

	/**
	 * @see UserProfile::phoneLineCreatedEvent
	 */
	void phoneLineCreatedEventHandler(UserProfile & sender, IPhoneLine & phoneLine);

	/**
	 * @see UserProfile::wsSmsCreatedEvent
	 */
	void wsSmsCreatedEventHandler(UserProfile & sender, WsSms & wsSms);

	/**
	 * @see UserProfile::wsSoftUpdateCreatedEvent
	 */
	void wsSoftUpdateCreatedEventHandler(UserProfile & sender, WsSoftUpdate & wsSoftUpdate);

	/**
	 * @see UserProfile::wsCallForwardCreatedEvent
	 */
	void wsCallForwardCreatedEventHandler(UserProfile & sender, WsCallForward & wsCallForward);

	/**
	 * @see disconnect()
	 */
	void disconnectThreadSafe();

	/**
	 * @see makeCall()
	 */
	void makeContactCallThreadSafe(std::string contactId);

	/**
	 * @see makeCall()
	 */
	void makeCallThreadSafe(std::string phoneNumber);

	/**
	 * @see startIM
	 */
	void startIMThreadSafe(std::string contactId);

	/**
	 * @see setSipAccount
	 */
	void setSipAccountThreadSafe(SipAccount sipAccount);

	/************************************************************************/
	/*Start Conference*/
	void startConferenceThreadSafe(std::string & confURI,std::string & szContent);
	void exitConferenceThreadSafe(int &callId);
	void media_send_start_ThreadSafe(char * AudioIP,int AudioPort,char * VideoIP,int VideoPort,HWND previewHwnd);
	/************************************************************************/

	UserProfile & _userProfile;

	CWengoPhone & _cWengoPhone;

	PUserProfile * _pUserProfile;

	CContactList _cContactList;

	CHistory * _cHistory;

	CWsCallForward * _cWsCallForward;

	CWenboxPlugin _cWenboxPlugin;

	CChatHandler _cChatHandler;

	CSms * _cSms;

	CSoftUpdate * _cSoftUpdate;

	CPhoneLine * _cPhoneLine;

};

#endif	//OWCUSERPROFILE_H
