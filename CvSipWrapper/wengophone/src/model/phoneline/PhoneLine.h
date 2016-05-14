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

#ifndef OWPHONELINE_H
#define OWPHONELINE_H

#include "IPhoneLine.h"

#include <model/account/SipAccount.h>

#include <util/Trackable.h>

#include <map>

class PhoneLineState;
class PhoneCall;
class ConferenceCall;
class WengoPhone;
class SipAddress;
class SipCallbacks;
class SipWrapper;
class SipPresenceState;

class PhoneLine : public IPhoneLine, public Trackable {
public:

	PhoneLine(SipAccount & sipAccount, UserProfile & userProfile);

	~PhoneLine();

	bool init();

	std::string getMySipAddress() const;

	EnumMakeCallError::MakeCallError makeCall(const std::string & phoneNumber, ConferenceCall * conferenceCall = NULL);

	void acceptCall(int callId);

	void rejectCall(int callId);

	void closeCall(int callId);

	void holdCall(int callId);

	void resumeCall(int callId);

	void blindTransfer(int callId, const std::string & sipAddress);

	void playSoundFile(int callId, const std::string & soundFile);

	void playDtmf(int callId, char dtmf);

	CodecList::AudioCodec getAudioCodecUsed(int callId);

	CodecList::VideoCodec getVideoCodecUsed(int callId);

	SipWrapper & getSipWrapper() const {
		return *_sipWrapper;
	}

	SipAccount & getSipAccount() {
		return _sipAccount;
	}

	bool connect();

	void disconnect(bool force = false);

	void setPhoneCallState(int callId, EnumPhoneCallState::PhoneCallState state, const SipAddress & sipAddress);

	int getLineId() const {
		return _lineId;
	}

	void setState(EnumPhoneLineState::PhoneLineState state);

	const PhoneLineState & getState() const {
		return *_state;
	}

	UserProfile & getUserProfile() const {
		return _userProfile;
	}

	PhoneCall * getPhoneCall(int callId) /*const*/;

	PhoneCall * getActivePhoneCall() const {
		return _activePhoneCall;
	}

	PhoneCallList getPhoneCallList() const;

	bool isConnected() const;

	void flipVideoImage(bool flip);

	void setCallsEncryption(bool enable);

	bool isCallEncrypted(int callId);

	/**
	 * @see IPhoneLine::hasPendingcalls
	 */
	bool hasPendingCalls() const;

	/**
	 * @see IPhoneLine::closePendingCalls
	 */
	void closePendingCalls();

	/************************************************************************/
	/*Start Conference*/
	void startConference(const std::string & confURI,const std::string & szContent);
	void cvMessageReceivedEventHandler(const std::string & message,const std::string & from);

	void rtpStatRecvEventHandler( const RtpStatItem &rtpstat );

	void tranStatRecvEventHandler( const TranStat & transtat ); 


	EnumMakeCallError::MakeCallError makeJoinConfInvite(const std::string & phoneNumber,const std::string & szContent);

	int media_recv_start(std::string userId,char * AudioIP,int AudioPort,char * VideoIP,int VideoPort,char *codecMIME,HWND remoteHwnd,int videoWidth,int videoHeight);
	
	//重载
	int media_send_start(char * AudioIP,int AudioPort,char * VideoIP,int VideoPort,HWND previewHwnd,int videoWidth,int videoHeight);
	
	void media_send_stop(char * AudioIP,int AudioPort,char * VideoIP,int VideoPort);
	int media_recv_start(std::string userId,char * AudioIP,int AudioPort,const char* audioCodecMIME , const int audioSampleRate , const int audioBps , const int audioChannels,char * VideoIP,int VideoPort,char *codecMIME,HWND remoteHwnd,int videoWidth,int videoHeight);
	void media_recv_stop(std::string userId);
	//屏幕共享
	int screen_share_send_start(char * ScreenIP,int ScreenPort,int x,int y,int width,int height);
	//取消桌面共享
	void screen_share_send_stop(char * ScreenIP,int ScreenPort);
	//接收桌面共享
	int screen_share_recv_start(char * ScreenIP,int ScreenPort,HWND remoteHwnd,int videoWidth,int videoHeight);
	//取消接收桌面共享
	void screen_share_recv_stop();
	void exit_conference(const std::string & contactId);
	//video窗口resize
	void resizeVideoWindow(const std::string &userId,const HWND &remoteHwnd,int fullscreenflag);

	//禁止发言
	void forbidSpeak(char * AudioIP,int AudioPort);
	void forbidSpeak(char * userId);
	//允许发言
	void permitSpeak(char * AudioIP,int AudioPort);

	void makeInfoCall(int callId,char * szContent);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief	 向QOS服务器发送内容
	///
	/// @author	qhf
	///
	/// @param	focusURI	URI of the focus. 
	/// @param	szContent	The size content. 
	////////////////////////////////////////////////////////////////////////////////////////////////////
	void sendQosPara(const std::string & focusURI,const std::string & szContent);
	
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
	
	/**
	 *	设置dscp值
	 * @param dscp 有效值是0-63
	 */
	void setDscp(const int dscp); 

	//发送一条sip消息给目标uri，一般是request消息，返回MessageId
	 int sendSipMessage(string remoteURI, string msgContent);


private:

	/**
	 * Creates and sets the SIP stack.
	 */
	bool initSipWrapper();

	/**
	 * Configures the SIP stack.
	 *
	 * Configures Audio devices, AEC, half-duplex ect...
	 */
	void configureSipWrapper();

	/**
	 * Puts all the PhoneCall in the hold state except one.
	 *
	 * Code factorization.
	 *
	 * @param callId phone call that won't be put in hold state
	 */
	void holdCallsExcept(int callId);

	/**
	 * Puts all the PhoneCall in the hold state.
	 *
	 * @see holdCallsExcept()
	 */
	void holdAllCalls();

	/**
	 * A call has been closed.
	 *
	 * Code factorization.
	 *
	 * @param callId phone call being closed
	 */
	void callClosed(int callId);

	/**
	 * Checks if a callId is valid.
	 *
	 * Makes an assertion if the callId is not valid.
	 *
	 * @param callId to check
	 */
	void checkCallId(int callId);

	/** SIP implementation. */
	SipWrapper * _sipWrapper;

	SipCallbacks * _sipCallbacks;

	typedef std::map < int, PhoneCall * > PhoneCalls;

	/** Map of PhoneCall. */
	PhoneCalls _phoneCallMap;

	/** Active PhoneCall. */
	PhoneCall * _activePhoneCall;

	/** SipAccount associated with this PhoneLine. */
	SipAccount & _sipAccount;

	/** Line id of this PhoneLine. */
	int _lineId;

	/** State of the PhoneLine. */
	PhoneLineState * _state;

	/** Defines the vector of PhoneLineState. */
	typedef List < PhoneLineState * > PhoneLineStateList;

	/** List of PhoneLineState. */
	PhoneLineStateList _phoneLineStateList;

	UserProfile & _userProfile;
};

#endif	//OWPHONELINE_H
