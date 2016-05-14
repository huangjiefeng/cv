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

#include "PhoneLine.h"

#include "PhoneLineStateUnknown.h"
#include "PhoneLineStateProgress.h"
#include "PhoneLineStateOk.h"
#include "PhoneLineStateClosed.h"
#include "PhoneLineStateTimeout.h"
#include "PhoneLineStateServerError.h"
#include "PhoneLineStateAuthenticationError.h"

#include <model/SipCallbacks.h>
#include <model/config/ConfigManager.h>
#include <model/config/Config.h>
#include <model/history/History.h>
#include <model/phonecall/PhoneCall.h>
#include <model/profile/UserProfile.h>
#include <sipwrapper/SipWrapper.h>
#include <sipwrapper/SipWrapperFactory.h>
#include <sipwrapper/EnumNatType.h>

#include <sound/AudioDevice.h>
#include <util/Logger.h>
#include <util/SafeDelete.h>
#include <util/Path.h>		  
#include <string>
using namespace std;		
#include <cstring>

//zhenHua.sun 2010-09-01
#include <RunningProfile.h>
#include <SessionManager.h>

PhoneLine::PhoneLine(SipAccount & sipAccount, UserProfile & userProfile)
	: _sipAccount(sipAccount),
	_userProfile(userProfile) {

	static PhoneLineStateUnknown stateUnknown;
	_phoneLineStateList += &stateUnknown;
	_state = &stateUnknown;

	static PhoneLineStateProgress stateProgress;
	_phoneLineStateList += &stateProgress;

	static PhoneLineStateOk stateOk;
	_phoneLineStateList += &stateOk;

	static PhoneLineStateClosed stateClosed;
	_phoneLineStateList += &stateClosed;

	static PhoneLineStateTimeout stateTimeout;
	_phoneLineStateList += &stateTimeout;

	static PhoneLineStateServerError stateServerError;
	_phoneLineStateList += &stateServerError;

	static PhoneLineStateAuthenticationError stateAuthenticationError;
	_phoneLineStateList += &stateAuthenticationError;

	_activePhoneCall = NULL;

	_sipWrapper = NULL;
	_sipCallbacks = NULL;
	_lineId = SipWrapper::VirtualLineIdError;

	Config & config = ConfigManager::getInstance().getCurrentConfig();
	config.valueChangedEvent
		+= boost::bind(&PhoneLine::configureSipWrapper, this);
}

PhoneLine::~PhoneLine() {
	disconnect();

	//Do not need to delete everything (_phoneLineStateList)
	//since states are static inside the constructor
	_state = NULL;

	OWSAFE_DELETE(_sipCallbacks);

	//取消绑定事件
	_sipWrapper->tranStatRecvEvent -=boost::bind(&PhoneLine::tranStatRecvEventHandler,this,_1);
	_sipWrapper->rtpStatRecvEvent -=boost::bind(&PhoneLine::rtpStatRecvEventHandler,this,_2);


	_sipWrapper = NULL;

	OWSAFE_DELETE(_activePhoneCall);
}

bool PhoneLine::init() {
	_sipWrapper = SipWrapperFactory::getFactory().createSipWrapper();
	_sipCallbacks = new SipCallbacks(*_sipWrapper, _userProfile);
	_lineId = SipWrapper::VirtualLineIdError;

	//事件解绑
	_sipWrapper->tranStatRecvEvent +=boost::bind(&PhoneLine::tranStatRecvEventHandler,this,_1);
	_sipWrapper->rtpStatRecvEvent +=boost::bind(&PhoneLine::rtpStatRecvEventHandler,this,_2);

	return initSipWrapper();
}

std::string PhoneLine::getMySipAddress() const {
	return "sip:" + _sipAccount.getIdentity() + "@" + _sipAccount.getRealm();
}

EnumMakeCallError::MakeCallError PhoneLine::makeCall(const std::string & phoneNumber, ConferenceCall * conferenceCall) {
	int width,height,bitrate;
	std::string codecSerial;
	if (!_sipAccount.isConnected()) {
		LOG_ERROR("SipAccount not connected");
		return EnumMakeCallError::NotConnected;
	}

	if (phoneNumber.empty()) {
		LOG_ERROR("empty phone number");
		return EnumMakeCallError::EmptyPhoneNumber;
	}

#if 0   /*wzf 08-12-29*/
	for (PhoneCalls::iterator it = _phoneCallMap.begin(); it != _phoneCallMap.end(); ++it) {
		PhoneCall * phoneCall = (*it).second;

		if (phoneCall) {
			EnumPhoneCallState::PhoneCallState state = phoneCall->getState();
			if (state != EnumPhoneCallState::PhoneCallStateHold &&
				state != EnumPhoneCallState::PhoneCallStateClosed) {

				return EnumMakeCallError::CallNotHeld;
			}
		}
	}
#endif

	SipAddress sipAddress = SipAddress::fromString(
		phoneNumber, _sipAccount.getRealm()
	);
	/************************************************************************///Add by zhuang 09-01-11
	/*判断呼叫的用户是否正在通话中 */
	for (PhoneCalls::iterator it = _phoneCallMap.begin(); it != _phoneCallMap.end(); ++it) {
		PhoneCall * phoneCall = (*it).second;

		if (phoneCall) {
			SipAddress callPeerSipAddress = phoneCall->getPeerSipAddress();
			if (callPeerSipAddress.getUserName()==sipAddress.getUserName()) {

				return EnumMakeCallError::RepeatCall;
				}
		}
	}
	/************************************************************************/
	//Puts all the PhoneCall in the hold state before to create a new PhoneCall
	//holdAllCalls();

	Config & config = ConfigManager::getInstance().getCurrentConfig();
	bool enableVideo = config.getVideoEnable();

	// wzf 08-12-30
	if (enableVideo) {
		// the video device
			_sipWrapper->setVideoDevice(config.getVideoWebcamDevice());

			//设置crossbar的名称与输入类型。针对高清视频 ...zhenHua.sun 2010-07-17
			_sipWrapper->setCrossBar( config.getCrossbarName() , config.getCrossbarInputType() );
	}else
	{
		_sipWrapper->setVideoDevice("");
	}
	
	PhoneCall * phoneCall = new PhoneCall(*this, sipAddress);
	phoneCall->setConferenceCall(conferenceCall);
	_activePhoneCall = phoneCall;
	width = config.getVideoFrameWidth();
	height = config.getVideoFrameHeight();
	bitrate = config.getVideoBitRate();
	codecSerial = config.getVideoCodecSerial();
	int callId = _sipWrapper->makeCall2(
		_lineId, sipAddress.getRawSipAddress(), enableVideo,width,height,bitrate,codecSerial.c_str()
	);
	_sipWrapper->setVideoWidthAndHeight(width,height);
	if (callId < 0) {
		//FIXME should be replaced by a LOG_FATAL()
		LOG_ERROR("couldn't place the call, SipWrapper returned an error=" +
			String::fromNumber(callId));
		return EnumMakeCallError::SipError;
	}

	phoneCall->setCallId(callId);

	//Adds the PhoneCall to the list of PhoneCall
	_phoneCallMap[callId] = phoneCall;

	phoneCall->setState(EnumPhoneCallState::PhoneCallStateDialing);

	//Sends the event a new PhoneCall has been created
	phoneCallCreatedEvent(*this, *phoneCall);

	//History: create a HistoryMemento for this outgoing call
	HistoryMemento * memento = new HistoryMemento(
		HistoryMemento::OutgoingCall, sipAddress.getSipAddress(), callId);
	_userProfile.getHistory().addMemento(memento);

	return EnumMakeCallError::NoError;
}

/************************************************************************/
/*��Focus����Invite*/
EnumMakeCallError::MakeCallError PhoneLine::makeJoinConfInvite(const std::string & phoneNumber,const std::string & szContent)
{
	int width,height,bitrate;
	std::string codecSerial;
	if (!_sipAccount.isConnected()) {
		LOG_ERROR("SipAccount not connected");
		return EnumMakeCallError::NotConnected;
	}

	if (phoneNumber.empty()) {
		LOG_ERROR("empty phone number");
		return EnumMakeCallError::EmptyPhoneNumber;
	}

	SipAddress sipAddress = SipAddress::fromString(
		phoneNumber, _sipAccount.getRealm()
		);

	Config & config = ConfigManager::getInstance().getCurrentConfig();
	bool enableVideo = config.getVideoEnable();
	codecSerial = config.getVideoCodecSerial();
	width = config.getVideoFrameWidth();
	height = config.getVideoFrameHeight();
	// wzf 08-12-30
	if (enableVideo) {
		// the video device
		_sipWrapper->setVideoDevice(config.getVideoWebcamDevice());

		//设置crossbar的名称与输入类型。针对高清视频 ...zhenHua.sun 2010-07-17
		_sipWrapper->setCrossBar( config.getCrossbarName() , config.getCrossbarInputType() );

		codecSerial = config.getVideoCodecSerial();
	}else
	{
		_sipWrapper->setVideoDevice("");
		codecSerial = "";
	}
	_sipWrapper->setVideoWidthAndHeight(config.getVideoFrameWidth(),config.getVideoFrameHeight());
	int callId = _sipWrapper->makeJoinConfInvite(_lineId, sipAddress.getRawSipAddress(),szContent,codecSerial.c_str(),width,height);
	if (callId < 0) {
		//FIXME should be replaced by a LOG_FATAL()
		LOG_ERROR("couldn't place the call, SipWrapper returned an error=" +
			String::fromNumber(callId));
		return EnumMakeCallError::SipError;
	}

	///记录用户进入的会议......zhenHua.sun 2010-12-06
	RunningProfile::getInstance().getSessionManager().addConfSession( phoneNumber, callId );

	setCallIdEvent(*this, callId);
	return EnumMakeCallError::NoError;
}
/************************************************************************/

bool PhoneLine::connect() {

	if (_sipAccount.isConnected()) {
		return true; // Nothing to do if the line is already connected!
	}
	Config & config = ConfigManager::getInstance().getCurrentConfig();
	//IPv4 or IPv6
	int address_family = config.getIPVersion();
	_lineId = _sipWrapper->addVirtualLine(
		_sipAccount.getDisplayName(),
		_sipAccount.getUsername(),
		_sipAccount.getIdentity(),
		_sipAccount.getPassword(),
		_sipAccount.getRealm(),
		_sipAccount.getSIPProxyServerHostname(),
		_sipAccount.getRegisterServerHostname(),
		address_family);
	if (_lineId < 0) {
		LOG_ERROR("VirtualLine Creation Failed");
		return false;
	}
	_sipAccount.setVLineID(_lineId);

	if (_sipWrapper->registerVirtualLine(_lineId) != 0) {
		LOG_ERROR("Couldn't register virtual line");
		return false;
	}

	LOG_DEBUG("connect username=" + _sipAccount.getUsername()
		+ " server=" + _sipAccount.getRegisterServerHostname()
		+ " lineId=" + String::fromNumber(_lineId));
	return true;
}

void PhoneLine::disconnect(bool force) {
	if (_lineId != SipWrapper::VirtualLineIdError) {
		_sipAccount.setConnected(false);
		_sipWrapper->removeVirtualLine(_lineId, force);
	}
}

void PhoneLine::checkCallId(int callId) {
	PhoneCall * phoneCall = getPhoneCall(callId);
	if (!phoneCall) {
		LOG_FATAL("unknown phone call callId=" + String::fromNumber(callId));
	}
}

void PhoneLine::acceptCall(int callId) {
	//FIXME crash in some cases
	//checkCallId(callId);
	//modify by wzf
	//holdCallsExcept(callId);

	Config & config = ConfigManager::getInstance().getCurrentConfig();
	bool enableVideo = config.getVideoEnable();
	/************************************************************************///Add by zhuang 08-12-24
	/*�������ļ����width��height��bitrate*/
	int video_width = config.getVideoFrameWidth();
	int video_height = config.getVideoFrameHeight();
	int video_bitrate = config.getVideoBitRate();
	std::string codecSerial = config.getVideoCodecSerial();
	/************************************************************************/

	if (enableVideo) {
		//Sets the video device
		_sipWrapper->setVideoDevice(config.getVideoWebcamDevice());
	}
	/************************************************************************///Modify by zhuang 08-12-24
	/*_sipWrapper->acceptCall(callId, enableVideo);*/
	_sipWrapper->setVideoWidthAndHeight(video_width,video_height);
	_sipWrapper->acceptCall2(callId, enableVideo,video_width,video_height,video_bitrate,codecSerial.c_str());
	/***********************************************************************/
	LOG_DEBUG("call accepted callId=" + String::fromNumber(callId));
}

void PhoneLine::rejectCall(int callId) {
	//FIXME crash in some cases
	//checkCallId(callId);

	_sipWrapper->rejectCall(callId);
	//History: retrieve the memento and change its state to rejected
	_userProfile.getHistory().updateCallState(callId, HistoryMemento::RejectedCall);
	LOG_DEBUG("call rejected callId=" + String::fromNumber(callId));
}

void PhoneLine::closeCall(int callId) {
	checkCallId(callId);
	_sipWrapper->closeCall(callId);
	LOG_DEBUG("call closed callId=" + String::fromNumber(callId));
}

void PhoneLine::holdCall(int callId) {
	checkCallId(callId);
	_sipWrapper->holdCall(callId);
	LOG_DEBUG("call hold callId=" + String::fromNumber(callId));
}

void PhoneLine::resumeCall(int callId) {
	checkCallId(callId);
	_sipWrapper->resumeCall(callId);
	LOG_DEBUG("call resumed callId=" + String::fromNumber(callId));
}

void PhoneLine::blindTransfer(int callId, const std::string & sipAddress) {
	checkCallId(callId);
	SipAddress sipUri = SipAddress::fromString(
		sipAddress, getSipAccount().getRealm()
	);
	_sipWrapper->blindTransfer(callId, sipUri.getRawSipAddress());
	LOG_DEBUG("call transfered to=" + sipAddress);
}

void PhoneLine::playSoundFile(int callId, const std::string & soundFile) {
	//No check
	//checkCallId(callId);
	_sipWrapper->playSoundFile(callId, soundFile);
}

void PhoneLine::playDtmf(int callId, char dtmf) {
	//No check
	//checkCallId(callId);
	_sipWrapper->playDtmf(callId, dtmf);
}


CodecList::AudioCodec PhoneLine::getAudioCodecUsed(int callId) {
	return _sipWrapper->getAudioCodecUsed(callId);
}

CodecList::VideoCodec PhoneLine::getVideoCodecUsed(int callId) {
	return _sipWrapper->getVideoCodecUsed(callId);
}

void PhoneLine::setPhoneCallState(int callId, EnumPhoneCallState::PhoneCallState state, const SipAddress & sipAddress) {
	LOG_DEBUG("call state changed callId=" + String::fromNumber(callId) +
		" state=" + EnumPhoneCallState::toString(state) +
		" from=" + sipAddress.getSipAddress());

	//Saves the last state
	EnumPhoneCallState::PhoneCallState lastState = EnumPhoneCallState::PhoneCallStateUnknown;
	PhoneCall * phoneCall = getPhoneCall(callId);
	if (phoneCall) {

		lastState = phoneCall->getState();

		if (phoneCall->getState() == state) {
			//We are already in this state
			//Prevents the state to be applied 2 times in a row
			return;
		}

		phoneCall->setState(state);
	}

	// This should not replace the state machine pattern PhoneCallState/ PhoneLineState
	switch (state) {

	case EnumPhoneCallState::PhoneCallStateUnknown:
		break;

	case EnumPhoneCallState::PhoneCallStateError:
		callClosed(callId);
		break;

	case EnumPhoneCallState::PhoneCallStateResumed:
		holdCallsExcept(callId);
		_activePhoneCall = getPhoneCall(callId);
		break;

	case EnumPhoneCallState::PhoneCallStateTalking:
		_activePhoneCall = getPhoneCall(callId);
		break;

	case EnumPhoneCallState::PhoneCallStateDialing:
		break;

	case EnumPhoneCallState::PhoneCallStateRinging:
		break;

	case EnumPhoneCallState::PhoneCallStateClosed:
		if (lastState == EnumPhoneCallState::PhoneCallStateIncoming) {
			//History: retrieve the memento and change its state to missed
			_userProfile.getHistory().updateCallState(
				callId, HistoryMemento::MissedCall);
			LOG_DEBUG("call missed callId=" + String::fromNumber(callId));
		}
		callClosed(callId);
		break;

	case EnumPhoneCallState::PhoneCallStateIncoming: {
		//Sends SIP code 180
		//TODO automatically??
		_sipWrapper->sendRingingNotification(callId,
			ConfigManager::getInstance().getCurrentConfig().getVideoEnable());

		PhoneCall * phoneCall = new PhoneCall(*this, sipAddress);
		phoneCall->setCallId(callId);

		//Adds the PhoneCall to the list of PhoneCall
		_phoneCallMap[callId] = phoneCall;

		phoneCall->setState(state);

		_activePhoneCall = phoneCall;

		//Sends the event a new PhoneCall has been created
		phoneCallCreatedEvent(*this, *phoneCall);

		//History: create a HistoryMemento for this incoming call
		std::string displayName = sipAddress.getDisplayName();
		if (displayName.empty()) {
			displayName = sipAddress.getUserName();
		}
		HistoryMemento * memento = new HistoryMemento(
				HistoryMemento::IncomingCall, displayName, callId);
		_userProfile.getHistory().addMemento(memento);

		break;
	}

	case EnumPhoneCallState::PhoneCallStateHold:
		break;

	case EnumPhoneCallState::PhoneCallStateMissed:
		//History: retrieve the memento and change its state to missed
		_userProfile.getHistory().updateCallState(callId, HistoryMemento::MissedCall);
		LOG_DEBUG("call missed callId=" + String::fromNumber(callId));
		break;

	case EnumPhoneCallState::PhoneCallStateRedirected:
		break;

	case EnumPhoneCallState::PhoneCallStateRingingStart:
		break;

	case EnumPhoneCallState::PhoneCallStateRingingStop:
		break;

	default:
		LOG_FATAL("unknown PhoneCallState=" + EnumPhoneCallState::toString(state));
	}
}

void PhoneLine::callClosed(int callId) {
	//If it crashes inside checkCallId() this is probably due
	//to a duplicated phCALLCLOSED message
	//checkCallId(callId);

	PhoneCall * phoneCall = getPhoneCall(callId);
	if (!phoneCall) {
		//Phone call has been already closed
		return;
	}

	if (_activePhoneCall == phoneCall) {
		_activePhoneCall = NULL;
	}

	//Deletes the PhoneCall that is closed now
	//delete phoneCall;

	//History: update the duration of the memento associated to this phonecall
	_userProfile.getHistory().updateCallDuration(callId,
	 	phoneCall->getDuration());

	//Removes it from the list of PhoneCall
	_phoneCallMap.erase(callId);

	phoneCallClosedEvent(*this, *phoneCall);
}

void PhoneLine::holdCallsExcept(int callId) {
	for (PhoneCalls::iterator it = _phoneCallMap.begin(); it != _phoneCallMap.end(); ++it) {
		PhoneCall * phoneCall = (*it).second;
		if (phoneCall) {
			if (phoneCall->getCallId() != callId &&
				!phoneCall->getConferenceCall()) {

				phoneCall->hold();
			}
		}
	}
}

void PhoneLine::holdAllCalls() {
	holdCallsExcept(SipWrapper::CallIdError);
}

void PhoneLine::setState(EnumPhoneLineState::PhoneLineState state) {
	LOG_DEBUG("PhoneLineState=" + EnumPhoneLineState::toString(state));

	for (unsigned i = 0; i < _phoneLineStateList.size(); i++) {
		PhoneLineState * phoneLineState = _phoneLineStateList[i];
		if (phoneLineState->getCode() == state) {
			if (_state->getCode() != phoneLineState->getCode()) {
				_state = phoneLineState;
				_state->execute(*this);
				
				if (state == EnumPhoneLineState::PhoneLineStateAuthenticationError) {
					return;
				}
				
				LOG_DEBUG("line state changed lineId=" +
					String::fromNumber(_lineId) + " state=" +
					EnumPhoneLineState::toString(_state->getCode()));
				stateChangedEvent(*this, state);
				return;
			} else {
				//Stay in the same state
				return;
			}
		}
	}

	LOG_FATAL("unknown PhoneLineState=" + String::fromNumber(state));
}

PhoneCall * PhoneLine::getPhoneCall(int callId) {
	return _phoneCallMap[callId];
}

IPhoneLine::PhoneCallList PhoneLine::getPhoneCallList() const {
	PhoneCallList phoneCallList;

	for (PhoneCalls::const_iterator it = _phoneCallMap.begin(); it != _phoneCallMap.end(); ++it) {
		PhoneCall * phoneCall = (*it).second;
		if (phoneCall) {
			phoneCallList += phoneCall;
		}
	}
	return phoneCallList;
}

bool PhoneLine::initSipWrapper() {
	Config & config = ConfigManager::getInstance().getCurrentConfig();
	//IPv4 or IPv6
	int address_family = config.getIPVersion();
	configureSipWrapper();
	_sipWrapper->init(address_family==2?0:1);
	return _sipWrapper->isInitialized();
}

void PhoneLine::configureSipWrapper() {
	//TODO check if Settings keys have changed
	Config & config = ConfigManager::getInstance().getCurrentConfig();

	//Setting plugin path
	string pluginPath = Path::getApplicationDirPath() + config.getCodecPluginPath();
	_sipWrapper->setPluginPath(pluginPath);

	//Setting proxy
	string proxyServer = config.getNetworkProxyServer();
	if (!proxyServer.empty()) {
		unsigned proxyPort = config.getNetworkProxyPort();
		string proxyLogin = config.getNetworkProxyLogin();
		string proxyPassword = config.getNetworkProxyPassword();
		_sipWrapper->setProxy(proxyServer, proxyPort, 
			proxyLogin, proxyPassword);
	}

	//Setting HTTP tunnel
	if (_sipAccount.isHttpTunnelNeeded()) {
		if (_sipAccount.httpTunnelHasSSL()) {
			_sipWrapper->setTunnel(_sipAccount.getHttpsTunnelServerHostname(),
				_sipAccount.getHttpsTunnelServerPort(), _sipAccount.httpTunnelHasSSL());
		} else {
			_sipWrapper->setTunnel(_sipAccount.getHttpTunnelServerHostname(),
				_sipAccount.getHttpTunnelServerPort(), _sipAccount.httpTunnelHasSSL());
		}
	}

	//Setting SIP proxy
	_sipWrapper->setSIP(_sipAccount.getSIPProxyServerHostname(),
		_sipAccount.getSIPProxyServerPort(),
		_sipAccount.getLocalSIPPort());

	//Setting NAT
	string natType = config.getNetworkNatType();
	_sipWrapper->setNatType(EnumNatType::toNatType(natType));

	//Setting video quality
	_sipWrapper->setVideoQuality(EnumVideoQuality::toVideoQuality(
		config.getVideoQuality()));
	//������Ƶ�ߺͿ�
	_sipWrapper->setVideoWidthAndHeight(config.getVideoFrameWidth(),config.getVideoFrameHeight());

	//Sets audio codec preferences
	_sipWrapper->setAudioCodecList(config.getAudioCodecList());

	//Setting audio devices
	_sipWrapper->setCallOutputAudioDevice(AudioDevice(config.getAudioOutputDeviceId()));
	_sipWrapper->setCallInputAudioDevice(AudioDevice(config.getAudioInputDeviceId()));
	_sipWrapper->setRingerOutputAudioDevice(AudioDevice(config.getAudioRingerDeviceId()));

	//AEC + half duplex
	_sipWrapper->enableAEC(config.getAudioAEC());
	_sipWrapper->enableHalfDuplex(config.getAudioHalfDuplex());

	// Presence & IM (SIP/SIMPLE)
	_sipWrapper->enablePIM(_sipAccount.isPIMEnabled());

	// Configure call encryption
	setCallsEncryption(config.getCallEncryptionMode());
}

bool PhoneLine::isConnected() const {
	return (_state->getCode() == EnumPhoneLineState::PhoneLineStateOk);
}

void PhoneLine::flipVideoImage(bool flip) {
	_sipWrapper->flipVideoImage(flip);
}

void PhoneLine::setCallsEncryption(bool enable) {
	_sipWrapper->setCallsEncryption(enable);
}

bool PhoneLine::isCallEncrypted(int callId) {
	return _sipWrapper->isCallEncrypted(callId);
}

bool PhoneLine::hasPendingCalls() const {
	return (_activePhoneCall != NULL);
}

void PhoneLine::closePendingCalls() {
	if (_activePhoneCall) {
		_activePhoneCall->close();
	}
}

/************************************************************************/
//启动会议，发送获取会议列表信息
void PhoneLine::startConference(const std::string & confURI,const std::string & szContent)
{
	_sipWrapper->cvMessageReceivedEvent +=boost::bind(&PhoneLine::cvMessageReceivedEventHandler,this,_2,_3);
	_sipWrapper->startConference(_lineId,confURI,szContent);
}

void PhoneLine::cvMessageReceivedEventHandler(const std::string & message,const std::string & from)
{
	cvMessageReceivedEvent(*this,message,from);
}

//发送媒体流
int PhoneLine::media_send_start(char * AudioIP,int AudioPort,char * VideoIP,int VideoPort,HWND previewHwnd,int videoWidth,int videoHeight)
{
	return _sipWrapper->media_send_start(AudioIP,AudioPort,VideoIP,VideoPort,previewHwnd,videoWidth,videoHeight);
}

//停止发送媒体流
void PhoneLine::media_send_stop(char * AudioIP,int AudioPort,char * VideoIP,int VideoPort)
{
	_sipWrapper->media_send_stop(AudioIP,AudioPort,VideoIP,VideoPort);
}

/** zhenHua.sun 2010-08-03
//接收媒体流
int PhoneLine::media_recv_start(std::string userId,char * AudioIP,int AudioPort,char * VideoIP,int VideoPort,char *codecMIME,HWND remoteHwnd,int videoWidth,int videoHeight)
{
	return _sipWrapper->media_recv_start(userId,AudioIP,AudioPort,VideoIP,VideoPort,codecMIME,remoteHwnd,videoWidth,videoHeight);
}
*/

//重载接收媒体流...zhenhua.sun 2010-08-03
int PhoneLine::media_recv_start(std::string userId,char * AudioIP,int AudioPort,const char* audioCodecMIME , const int audioSampleRate , const int audioBps , const int audioChannels,char * VideoIP,int VideoPort,char *codecMIME,HWND remoteHwnd,int videoWidth,int videoHeight)
{
	//return _sipWrapper->media_recv_start(userId,AudioIP,AudioPort,audioCodecMIME, audioSampleRate , audioBps , audioChannels,VideoIP,VideoPort,codecMIME,remoteHwnd,videoWidth,videoHeight);
	//保留创建的媒体接收会话 zhenHua.sun 2010-09-01
	int recvGraphId = _sipWrapper->media_recv_start(userId,AudioIP,AudioPort,audioCodecMIME, audioSampleRate , audioBps , audioChannels,VideoIP,VideoPort,codecMIME,remoteHwnd,videoWidth,videoHeight);
	RunningProfile::getInstance().getSessionManager().addRecvSession( userId , recvGraphId , AudioIP , AudioPort,
		audioCodecMIME , audioSampleRate , audioBps , audioChannels , VideoIP , VideoPort , 
		codecMIME, remoteHwnd, videoWidth , videoHeight );
	return recvGraphId;
}

//停止接收媒体流
void PhoneLine::media_recv_stop(std::string userId)
{
	_sipWrapper->media_recv_stop(userId);

	//删除创建的媒体接收会话  zhenHua.sun 2010-09-01
	RunningProfile::getInstance().getSessionManager().removeRecvSession(userId);
}

//屏幕共享
int PhoneLine::screen_share_send_start(char * ScreenIP,int ScreenPort,int x,int y,int width,int height)
{
	return _sipWrapper->screen_share_send_start(ScreenIP,ScreenPort,x,y,width,height);
}
//取消桌面共享
void PhoneLine::screen_share_send_stop(char * ScreenIP,int ScreenPort)
{
	_sipWrapper->screen_share_send_stop(ScreenIP,ScreenPort);
}
//接收桌面共享
int PhoneLine::screen_share_recv_start(char * ScreenIP,int ScreenPort,HWND remoteHwnd,int videoWidth,int videoHeight)
{
	return _sipWrapper->screen_share_recv_start(ScreenIP,ScreenPort,remoteHwnd,videoWidth,videoHeight);
}
//取消接收桌面共享
void PhoneLine::screen_share_recv_stop()
{
	_sipWrapper->screen_share_recv_stop();
}

void PhoneLine::exit_conference(const std::string & contactId)
{
	_sipWrapper->exit_conference(contactId);

	//清除所有会话 zhenHua.sun 2010-09-01
	RunningProfile::getInstance().getSessionManager().clearRecvSessions();
}

//video窗口resize
void PhoneLine::resizeVideoWindow(const std::string &userId,const HWND &remoteHwnd,int fullscreenflag)
{
	_sipWrapper->resizeVideoWindow(userId,remoteHwnd,fullscreenflag);
}

void PhoneLine::rtpStatRecvEventHandler( const RtpStatItem &rtpstat )
{
	rtpStatRecvEvent(*this, rtpstat);
}

//禁止发言
void PhoneLine::forbidSpeak(char * AudioIP,int AudioPort)
{
	_sipWrapper->forbidSpeak(AudioIP,AudioPort);
}

void PhoneLine::forbidSpeak(char * userId)
{
	_sipWrapper->forbidSpeak(userId);
}
//允许发言
void PhoneLine::permitSpeak(char * AudioIP,int AudioPort)
{
	_sipWrapper->permitSpeak(AudioIP,AudioPort);
}

void PhoneLine::makeInfoCall(int callId,char * szContent) {
	checkCallId(callId);
	_sipWrapper->makeInfoCall(callId,szContent);
	LOG_DEBUG("info call callId=" + String::fromNumber(callId));
}



void PhoneLine::sendQosPara( const std::string & focusURI,const std::string & szContent )
{
	SipAddress sipAddress = SipAddress::fromString(	focusURI, _sipAccount.getRealm());
	_sipWrapper->sendQosPara(_lineId,sipAddress.getRawSipAddress(),szContent);
}


//end
/************************************************************************/

//控制音视频的起停...zhenHua.sun 2010-08-06
/**
* @brief 控制音频的发送
* @param state 如果是true，则启动音频的发送，否则停止音频的发送
*/
void PhoneLine::controlAudioSend( bool state )
{
	_sipWrapper->controlAudioSend( state );
}
/**
* @brief 控制视频的发送
* @param state 如果是true，则启动视频的发送，否则停止视频的发送
*/
void PhoneLine::controlVideoSend( bool state )
{
	_sipWrapper->controlVideoSend( state );
}

void PhoneLine::setDscp( const int dscp )
{
	_sipWrapper->setDscp(dscp);
}

void PhoneLine::tranStatRecvEventHandler( const TranStat & transtat )
{
	tranStatRecvEvent(transtat);
}

int PhoneLine::sendSipMessage(string remoteURI, string msgContent )
{		
	return	_sipWrapper->sendSipMessage(_lineId,remoteURI,msgContent);
}