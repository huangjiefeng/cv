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

#include "CUserProfile.h"

#include <presentation/PFactory.h>
#include <presentation/PUserProfile.h>

#include <control/CWengoPhone.h>
#include <control/chat/CChatHandler.h>
#include <control/contactlist/CContactList.h>
#include <control/history/CHistory.h>
#include <control/phoneline/CPhoneLine.h>
#include <control/webservices/sms/CSms.h>
#include <control/webservices/softupdate/CSoftUpdate.h>
#include <control/webservices/callforward/CWsCallForward.h>
#include <control/wenbox/CWenboxPlugin.h>

#include <model/WengoPhone.h>
#include <model/account/wengo/WengoAccount.h>
#include <model/config/ConfigManager.h>
#include <model/config/Config.h>
#include <model/contactlist/Contact.h>
#include <model/history/History.h>
#include <model/phoneline/IPhoneLine.h>
#include <model/presence/PresenceHandler.h>
#include <model/profile/UserProfile.h>
#include <model/webservices/sms/WsSms.h>
#include <model/webservices/softupdate/WsSoftUpdate.h>
#include <model/wenbox/WenboxPlugin.h>

#include <sipwrapper/SipWrapper.h>

#include <thread/ThreadEvent.h>
#include <util/Logger.h>
#include <util/SafeDelete.h>


CUserProfile::CUserProfile(UserProfile & userProfile, CWengoPhone & cWengoPhone)
	: _userProfile(userProfile),
	_cWengoPhone(cWengoPhone),
	_cContactList(userProfile.getContactList(), cWengoPhone),
	_cWenboxPlugin(*userProfile.getWenboxPlugin(), cWengoPhone),
	_cChatHandler(userProfile.getChatHandler(), *this) {

	_cHistory = NULL;
	_cPhoneLine = NULL;
	_cSms = NULL;
	_cSoftUpdate = NULL;
	_cWsCallForward = NULL;

	_pUserProfile = PFactory::getFactory().createPresentationUserProfile(*this);

	_userProfile.phoneLineCreatedEvent +=
		boost::bind(&CUserProfile::phoneLineCreatedEventHandler, this, _1, _2);
	_userProfile.wsSmsCreatedEvent +=
		boost::bind(&CUserProfile::wsSmsCreatedEventHandler, this, _1, _2);
	_userProfile.wsSoftUpdateCreatedEvent +=
		boost::bind(&CUserProfile::wsSoftUpdateCreatedEventHandler, this, _1, _2);
	_userProfile.wsCallForwardCreatedEvent +=
		boost::bind(&CUserProfile::wsCallForwardCreatedEventHandler, this, _1, _2);

	_userProfile.loginStateChangedEvent +=
		boost::bind(&CUserProfile::loginStateChangedEventHandler, this, _1, _2);
	_userProfile.networkDiscoveryStateChangedEvent +=
		boost::bind(&CUserProfile::networkDiscoveryStateChangedEventHandler, this, _1, _2);

	_userProfile.getHistory().historyLoadedEvent +=
		boost::bind(&CUserProfile::historyLoadedEventHandler, this, _1);

	_userProfile.getPresenceHandler().authorizationRequestEvent +=
		boost::bind(&CUserProfile::authorizationRequestEventHandler, this, _1, _2, _3);

	_userProfile.getPresenceHandler().incomingSubscribeEvent +=
		boost::bind(&CUserProfile::incomingSubscribeEventHandler, this, _1, _2, _3, _4, _5);

	//绑定事件
	_userProfile.tranStatRecvEvent +=boost::bind(&CUserProfile::tranStatRecvEventHandler,this,_1);
	_userProfile.cvJoinRejectionEvent += boost::bind(&CUserProfile::cvJoinRejectionEventHandler,this,_2,_3);

	//Check if a PhoneLine already exist
	if (_userProfile.getActivePhoneLine()) {
		phoneLineCreatedEventHandler(_userProfile, *_userProfile.getActivePhoneLine());
	}

	historyLoadedEventHandler(_userProfile.getHistory());

	if (_userProfile.getWsSms()) {
		wsSmsCreatedEventHandler(_userProfile, *_userProfile.getWsSms());
	}

	if (_userProfile.getWsSoftUpdate()) {
		wsSoftUpdateCreatedEventHandler(_userProfile, *_userProfile.getWsSoftUpdate());
	}

	if (_userProfile.getWsCallForward()) {
		wsCallForwardCreatedEventHandler(_userProfile, *_userProfile.getWsCallForward());
	}
}

CUserProfile::~CUserProfile() {

		//事件解绑
		_userProfile.tranStatRecvEvent -=boost::bind(&CUserProfile::tranStatRecvEventHandler,this,_1);
		_userProfile.cvJoinRejectionEvent -= boost::bind(&CUserProfile::cvJoinRejectionEventHandler,this,_2,_3);

	OWSAFE_DELETE(_pUserProfile);
	OWSAFE_DELETE(_cHistory);
	OWSAFE_DELETE(_cSms);
	OWSAFE_DELETE(_cSoftUpdate);
	OWSAFE_DELETE(_cPhoneLine);
}

void CUserProfile::loginStateChangedEventHandler(SipAccount & sender,
	EnumSipLoginState::SipLoginState state) {
	_pUserProfile->loginStateChangedEventHandler(sender, state);
}

void CUserProfile::networkDiscoveryStateChangedEventHandler(SipAccount & sender,
	SipAccount::NetworkDiscoveryState state) {
	_pUserProfile->networkDiscoveryStateChangedEventHandler(sender, state);
}

void CUserProfile::authorizationRequestEventHandler(PresenceHandler & sender,
	const IMContact & imContact, const std::string & message) {
	_pUserProfile->authorizationRequestEventHandler(sender, imContact, message);
}

void CUserProfile::incomingSubscribeEventHandler(PresenceHandler & sender,
	const std::string & imaccountId, int sid,const std::string & from,const std::string & evtType) 
{
	_pUserProfile->incomingSubscribeEventHandler(sender,imaccountId, sid, from, evtType);
}

void CUserProfile::phoneLineCreatedEventHandler(UserProfile & sender, IPhoneLine & phoneLine) {
	_cPhoneLine = new CPhoneLine(phoneLine, _cWengoPhone);

	LOG_DEBUG("CPhoneLine created");
}

void CUserProfile::wsSmsCreatedEventHandler(UserProfile & sender, WsSms & sms) {
	if (!_cSms) {
		_cSms = new CSms(sms, _cWengoPhone);
		LOG_DEBUG("CSms created");
	}
}

void CUserProfile::wsCallForwardCreatedEventHandler(UserProfile & sender, WsCallForward & wsCallForward) {
	_cWsCallForward = new CWsCallForward(_cWengoPhone, wsCallForward);
}

void CUserProfile::wsSoftUpdateCreatedEventHandler(UserProfile & sender, WsSoftUpdate & wsSoftUpdate) {
	if (!_cSoftUpdate) {
		_cSoftUpdate = new CSoftUpdate(wsSoftUpdate, _cWengoPhone);
		LOG_DEBUG("CSoftUpdate created");

		//Check for WengoPhone update
		if (_userProfile.getSipAccount()) {
			if (_userProfile.getSipAccount()->getType() == SipAccount::SipAccountTypeWengo) {
				wsSoftUpdate.checkForUpdate();
			}
		}
	}
}

void CUserProfile::historyLoadedEventHandler(History & history) {
	_cHistory = new CHistory(history, _cWengoPhone, *this);
}

void CUserProfile::disconnect() {
	typedef ThreadEvent0<void ()> MyThreadEvent;
	MyThreadEvent * event = new MyThreadEvent(boost::bind(&CUserProfile::disconnectThreadSafe, this));
	WengoPhone::getInstance().postEvent(event);
}

void CUserProfile::disconnectThreadSafe() {
	_userProfile.disconnect();
}

void CUserProfile::makeContactCall(const std::string & contactId) {
	typedef ThreadEvent1<void (std::string contactId), std::string> MyThreadEvent;
	MyThreadEvent * event = new MyThreadEvent(boost::bind(&CUserProfile::makeContactCallThreadSafe, this, _1), contactId);
	WengoPhone::getInstance().postEvent(event);
}

void CUserProfile::makeContactCallThreadSafe(std::string contactId) {
	Contact * contact = _cContactList.getContact(contactId);
	if (contact) {
		EnumMakeCallError::MakeCallError error = _userProfile.makeCall(*contact);
		if (error != EnumMakeCallError::NoError) {
			makeCallErrorEvent(*this, error);
		}
	}
}

void CUserProfile::makeCall(const std::string & phoneNumber) {
	typedef ThreadEvent1<void (std::string phoneNumber), std::string> MyThreadEvent;
	MyThreadEvent * event = new MyThreadEvent(boost::bind(&CUserProfile::makeCallThreadSafe, this, _1), phoneNumber);
	WengoPhone::getInstance().postEvent(event);
}

void CUserProfile::makeCallThreadSafe(std::string phoneNumber) {
	EnumMakeCallError::MakeCallError error = _userProfile.makeCall(phoneNumber);
	if (error != EnumMakeCallError::NoError) {
		makeCallErrorEvent(*this, error);
	}
}

void CUserProfile::startIM(const std::string & contactId) {
	typedef ThreadEvent1<void (std::string contactId), std::string> MyThreadEvent;
	MyThreadEvent * event = new MyThreadEvent(boost::bind(&CUserProfile::startIMThreadSafe, this, _1), contactId);
	WengoPhone::getInstance().postEvent(event);
}

void CUserProfile::startIMThreadSafe(std::string contactId) {
	Contact * contact = _cContactList.getContact(contactId);
	if (contact) {
		_userProfile.startIM(*contact);
	}
}

//void CUserProfile::setWengoAccount(const WengoAccount & wengoAccount) {
//	typedef ThreadEvent1<void (WengoAccount wengoAccount), WengoAccount> MyThreadEvent;
//	MyThreadEvent * event = new MyThreadEvent(boost::bind(&CUserProfile::setWengoAccountThreadSafe, this, _1), wengoAccount);
//	WengoPhone::getInstance().postEvent(event);
//}
//
//void CUserProfile::setWengoAccountThreadSafe(WengoAccount wengoAccount) {
//	_userProfile.setWengoAccount(wengoAccount);
//}

void CUserProfile::setSipAccount(const SipAccount & sipAccount) {
	typedef ThreadEvent1<void (SipAccount sipAccount), SipAccount> MyThreadEvent;
	MyThreadEvent * event = new MyThreadEvent(boost::bind(&CUserProfile::setSipAccountThreadSafe, this, _1), sipAccount);
	WengoPhone::getInstance().postEvent(event);
}

void CUserProfile::setSipAccountThreadSafe(SipAccount sipAccount) {
	_userProfile.setSipAccount(sipAccount);
}

PhoneCall * CUserProfile::getActivePhoneCall() const {
	PhoneCall * result = NULL;

	//FIXME: model must not be used directly by the GUI
	IPhoneLine * phoneLine = _userProfile.getActivePhoneLine();
	if (phoneLine) {
		result = phoneLine->getActivePhoneCall();
	}

	return result;
}

/************************************************************************/
/*                                                                      */
void CUserProfile::startConference(const std::string & confURI,const std::string & szContent)
{
	typedef ThreadEvent2<void (std::string confURI,std::string szContent), std::string, std::string> MyThreadEvent;
	MyThreadEvent * event = new MyThreadEvent(boost::bind(&CUserProfile::startConferenceThreadSafe, this, _1,_2), confURI,szContent);
	WengoPhone::getInstance().postEvent(event);
}

void CUserProfile::startConferenceThreadSafe(std::string & confURI,std::string & szContent)
{
	_userProfile.cvConfListReceivedEvent +=boost::bind(&CUserProfile::cvConfListReceivedEventHandler,this,_2);
	_userProfile.cvConfrenceInviteEvent +=boost::bind(&CUserProfile::cvConfrenceInviteEventHandler,this,_2,_3);
	_userProfile.setCallIdEvent += boost::bind(&CUserProfile::setCallIdEventHandler,this,_2,_3);
	_userProfile.cvMemberListReceivedEvent +=boost::bind(&CUserProfile::cvMemberListReceivedEventHandler,this,_2,_3);
	_userProfile.cvPasswordIncorrectEvent +=boost::bind(&CUserProfile::cvPasswordIncorrectEventHandler,this);
	_userProfile.startConference(confURI,szContent);
}

void CUserProfile::applyConference(const std::string & confURI,const std::string & szContent)
{
	_userProfile.setCallIdEvent += boost::bind(&CUserProfile::setCallIdEventHandler,this,_2,_3);
	_userProfile.cvMemberListReceivedEvent +=boost::bind(&CUserProfile::cvMemberListReceivedEventHandler,this,_2,_3);
	_userProfile.logoutReceivedEvent += boost::bind(&CUserProfile::logoutReceivedEventHandler,this,_2);
	_userProfile.rtpStatRecvEvent += boost::bind(&CUserProfile::rtpStatRecvEventHandler,this,_2);
	_userProfile.cvKickedEvent +=boost::bind(&CUserProfile::cvKickedEventHandler,this);
	_userProfile.cvTextMsgReceivedEvent += boost::bind(&CUserProfile::cvTextMsgReceivedEventHandler,this,_2,_3);
	_userProfile.cvWhiteBoardMsgReceivedEvent += boost::bind(&CUserProfile::cvWhiteBoardMsgReceivedEventHandler,this,_2,_3);
	_userProfile.cvPermitSpeakEvent +=boost::bind(&CUserProfile::cvPermitSpeakEventHandler,this);
	_userProfile.cvHostUnShareScreenEvent +=boost::bind(&CUserProfile::cvHostUnShareScreenEventHandler,this);
	_userProfile.startConference(confURI,szContent);
}
//�˳�����
void CUserProfile::exitConference(int &callId)
{
	typedef ThreadEvent1<void (int cId), int> MyThreadEvent;
	MyThreadEvent * event = new MyThreadEvent(boost::bind(&CUserProfile::exitConferenceThreadSafe, this, _1), callId);
	WengoPhone::getInstance().postEvent(event);
}

// 用户退出一个会议
void CUserProfile::exitConferenceThreadSafe(int &callId)
{
	_userProfile.exitConference(callId);
	_userProfile.setCallIdEvent -= boost::bind(&CUserProfile::setCallIdEventHandler,this,_2,_3);
	_userProfile.cvMemberListReceivedEvent -=boost::bind(&CUserProfile::cvMemberListReceivedEventHandler,this,_2,_3);
	_userProfile.logoutReceivedEvent -= boost::bind(&CUserProfile::logoutReceivedEventHandler,this,_2);
	_userProfile.rtpStatRecvEvent -= boost::bind(&CUserProfile::rtpStatRecvEventHandler,this,_2);
	_userProfile.cvKickedEvent -=boost::bind(&CUserProfile::cvKickedEventHandler,this);
	_userProfile.cvTextMsgReceivedEvent -= boost::bind(&CUserProfile::cvTextMsgReceivedEventHandler,this,_2,_3);
	_userProfile.cvWhiteBoardMsgReceivedEvent -= boost::bind(&CUserProfile::cvWhiteBoardMsgReceivedEventHandler,this,_2,_3);
	_userProfile.cvPermitSpeakEvent -=boost::bind(&CUserProfile::cvPermitSpeakEventHandler,this);
	_userProfile.cvHostUnShareScreenEvent -=boost::bind(&CUserProfile::cvHostUnShareScreenEventHandler,this);
}

// 用户点击进入一个会议
int CUserProfile::makeJoinConfInvite(const int & index)
{
	_userProfile.setCallIdEvent += boost::bind(&CUserProfile::setCallIdEventHandler,this,_2,_3);
	_userProfile.cvMemberListReceivedEvent +=boost::bind(&CUserProfile::cvMemberListReceivedEventHandler,this,_2,_3);
	_userProfile.logoutReceivedEvent += boost::bind(&CUserProfile::logoutReceivedEventHandler,this,_2);
	_userProfile.rtpStatRecvEvent += boost::bind(&CUserProfile::rtpStatRecvEventHandler,this,_2);
	_userProfile.cvKickedEvent +=boost::bind(&CUserProfile::cvKickedEventHandler,this);
	_userProfile.cvTextMsgReceivedEvent += boost::bind(&CUserProfile::cvTextMsgReceivedEventHandler,this,_2,_3);
	_userProfile.cvWhiteBoardMsgReceivedEvent += boost::bind(&CUserProfile::cvWhiteBoardMsgReceivedEventHandler,this,_2,_3);
	_userProfile.cvPermitSpeakEvent +=boost::bind(&CUserProfile::cvPermitSpeakEventHandler,this);
	_userProfile.cvHostUnShareScreenEvent +=boost::bind(&CUserProfile::cvHostUnShareScreenEventHandler,this);
	return _userProfile.makeJoinConfInvite(index);
}

int CUserProfile::makeJoinConfInvite(const std::string & focusURI)
{
	_userProfile.setCallIdEvent += boost::bind(&CUserProfile::setCallIdEventHandler,this,_2,_3);
	_userProfile.cvMemberListReceivedEvent +=boost::bind(&CUserProfile::cvMemberListReceivedEventHandler,this,_2,_3);
	_userProfile.logoutReceivedEvent += boost::bind(&CUserProfile::logoutReceivedEventHandler,this,_2);
	_userProfile.rtpStatRecvEvent += boost::bind(&CUserProfile::rtpStatRecvEventHandler,this,_2);
	_userProfile.cvKickedEvent +=boost::bind(&CUserProfile::cvKickedEventHandler,this);
	_userProfile.cvTextMsgReceivedEvent += boost::bind(&CUserProfile::cvTextMsgReceivedEventHandler,this,_2,_3);
	_userProfile.cvWhiteBoardMsgReceivedEvent += boost::bind(&CUserProfile::cvWhiteBoardMsgReceivedEventHandler,this,_2,_3);
	_userProfile.cvPermitSpeakEvent +=boost::bind(&CUserProfile::cvPermitSpeakEventHandler,this);
	_userProfile.cvHostUnShareScreenEvent +=boost::bind(&CUserProfile::cvHostUnShareScreenEventHandler,this);
	return _userProfile.makeJoinConfInvite(focusURI);
}


void  CUserProfile::cvConfListReceivedEventHandler(vector<ConfInfo *> ConfList)
{
	cvConfListReceivedEvent(*this,ConfList);
}
void  CUserProfile::cvMemberListReceivedEventHandler(vector<MemberInfo *> MemberList,std::string confTitle)
{
	cvMemberListReceivedEvent(*this,MemberList,confTitle);
}

void CUserProfile::cvConfrenceInviteEventHandler( ConfInfo *conference,const std::string & from)
{
	cvConfrenceInviteEvent(*this,conference,from);
}

void CUserProfile::setCallIdEventHandler(int callId,HWND &previewHwnd)
{
	setCallIdEvent(*this,callId,previewHwnd);
}

void CUserProfile::cvPasswordIncorrectEventHandler()
{
	cvPasswordIncorrectEvent(*this);
}

void CUserProfile::cvKickedEventHandler()
{
	cvKickedEvent(*this);
}

//���ý��հ�ť��״̬
bool CUserProfile::setRecvButtonStatus(int index)
{
	return _userProfile.setRecvButtonStatus(index);
}

//����ý����
int CUserProfile::media_send_start(char * AudioIP,int AudioPort,char * VideoIP,int VideoPort,HWND previewHwnd)
{
	return _userProfile.media_send_start(AudioIP,AudioPort,VideoIP,VideoPort,previewHwnd);
	//typedef ThreadEvent5<void (char *,int,char *,int,HWND), char *,int,char *,int,HWND> MyThreadEvent;
	//MyThreadEvent * event = new MyThreadEvent(boost::bind(&CUserProfile::media_send_start_ThreadSafe, this, _1,_2,_3,_4,_5), AudioIP,AudioPort,VideoIP,VideoPort,previewHwnd);
	//WengoPhone::getInstance().postEvent(event);
	//return 0;
}


void CUserProfile::media_send_start_ThreadSafe(char * AudioIP,int AudioPort,char * VideoIP,int VideoPort,HWND previewHwnd) {
	_userProfile.media_send_start(AudioIP,AudioPort,VideoIP,VideoPort,previewHwnd);

}
//ֹͣ����ý����
void CUserProfile::media_send_stop(char * AudioIP,int AudioPort,char * VideoIP,int VideoPort)
{
	_userProfile.media_send_stop(AudioIP,AudioPort,VideoIP,VideoPort);
}

//����ý����
int CUserProfile::media_recv_start(char * AudioIP,int AudioPort,char * VideoIP,int VideoPort,char *codecMIME,HWND remoteHwnd)
{
	return _userProfile.media_recv_start("unknown",AudioIP,AudioPort,VideoIP,VideoPort,codecMIME,remoteHwnd,352,288);
}

int CUserProfile::media_recv_start(int index,HWND remoteHwnd)
{
	return _userProfile.media_recv_start(index,remoteHwnd);
}
//ֹͣ����ý����
void CUserProfile::media_recv_stop(std::string userId)
{
	//_userProfile.media_recv_stop(userId);
	_userProfile.unReceiveMedia(userId);
}

//��Ļ����
int CUserProfile::screen_share_send_start(char * ScreenIP,int ScreenPort,int x,int y,int width,int height)
{
	return _userProfile.screen_share_send_start(ScreenIP,ScreenPort,x,y,width,height);
}
//ȡ�����湲��
void CUserProfile::screen_share_send_stop(char * ScreenIP,int ScreenPort)
{
	_userProfile.screen_share_send_stop(ScreenIP,ScreenPort);
}
//�������湲��
int CUserProfile::screen_share_recv_start(char * ScreenIP,int ScreenPort,HWND remoteHwnd,int videoWidth,int videoHeight)
{
	return _userProfile.screen_share_recv_start(ScreenIP,ScreenPort,remoteHwnd,videoWidth,videoHeight);
}
//ȡ���������湲��
void CUserProfile::screen_share_recv_stop()
{
	_userProfile.screen_share_recv_stop();
}


void CUserProfile::exit_conference()
{
	_userProfile.exit_conference();
}

//���ñ���Ԥ�����ھ��
void CUserProfile::setPreviewWin(HWND  &Hwnd)
{
	_userProfile.setPreviewWin(Hwnd);
}

void CUserProfile::logoutReceivedEventHandler( const std::string & userId)
{
	logoutReceivedEvent(*this,userId);
}

//video����resize
void CUserProfile::resizeVideoWindow(const std::string &userId,const HWND &remoteHwnd,int fullscreenflag)
{
	_userProfile.resizeVideoWindow(userId,remoteHwnd,fullscreenflag);
}

void CUserProfile::rtpStatRecvEventHandler(const RtpStatItem &rtpstat)
{
	rtpStatRecvEvent(*this,rtpstat);
}

//������룬��ȷ����Ϣ��Focus�������򷵻�
int CUserProfile::checkPassword(int &row,std::string &password)
{
	return _userProfile.checkPassword(row,password);
}

//��鵱ǰ�û��Ƿ�������
bool CUserProfile::hostOrNot()
{
	return _userProfile.hostOrNot();
}

//�����Ա����״̬���Ƿ����ڷ���
int CUserProfile::memberSpeakStatus(int row)
{
	return _userProfile.memberSpeakStatus(row);
}

//ָ��������
int CUserProfile::setSpeaker(int row,bool permitOrForbid,int callId)
{
	return _userProfile.setSpeaker(row,permitOrForbid,callId);
}

//ɾ�������
int CUserProfile::KickOut(int row,int callId)
{
	return _userProfile.KickOut(row,callId);
}

void CUserProfile::makeInfoCall(int callId,char * szContent)
{
	_userProfile.makeInfoCall(callId,szContent);
}

void CUserProfile::cvTextMsgReceivedEventHandler(const std::string & Message,const std::string & from)
{
	cvTextMsgReceivedEvent(*this,Message,from);
}

void CUserProfile::cvWhiteBoardMsgReceivedEventHandler(const std::string & Message,const std::string & from)
{
	cvWhiteBoardMsgReceivedEvent(*this,Message,from);
}

//�����ı���Ϣ
void CUserProfile::sendTextMsg(const int &receiverIndex,const std::string &szContent)
{
	_userProfile.sendTextMsg(receiverIndex,szContent);
}

//���ַ���
int CUserProfile::handForSpeak(int row,bool handUpOrDown,int callId)
{
	return _userProfile.handForSpeak(row,handUpOrDown,callId);
}

void CUserProfile::cvPermitSpeakEventHandler()
{
	cvPermitSpeakEvent(*this);
}

void CUserProfile::cvHostUnShareScreenEventHandler()
{
	cvHostUnShareScreenEvent(*this);
}
/**
* Add by: hexianfu
* for the telecontroller
get the latest confinfo and memberinfo
* 2009-10-29
*/
vector<ConfInfo *>& CUserProfile::getCurrentConfInfoList()
{
	return _userProfile.getCurrentConfInfoList();
}
vector<MemberInfo *>& CUserProfile::getCurrentMemberInfoList()
{
	return _userProfile.getCurrentMemberInfoList();
}

/************************************************************************/


//控制音视频的起停...zhenHua.sun 2010-08-06
/**
* @brief 控制音频的发送
* @param state 如果是true，则启动音频的发送，否则停止音频的发送
*/
void CUserProfile::controlAudioSend( bool state )
{
	_userProfile.controlAudioSend( state );
}
/**
* @brief 控制视频的发送
* @param state 如果是true，则启动视频的发送，否则停止视频的发送
*/
void CUserProfile::controlVideoSend( bool state )
{
	_userProfile.controlVideoSend( state );
}

void CUserProfile::tranStatRecvEventHandler( const TranStat & transtat )
{
	tranStatRecvEvent(transtat);
}

void CUserProfile::cvJoinRejectionEventHandler( const string& confFocusURI, const string & reason )
{
	cvJoinRejectionEvent(*this,confFocusURI,reason);
}
//end
