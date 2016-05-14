#include "stdafx.h"
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

#include "UserProfile.h"

#include <coipmanager/CoIpManager.h>
#include <model/WengoPhone.h>
#include <model/account/wengo/WengoAccount.h>
#include <model/chat/ChatHandler.h>
#include <model/config/ConfigManager.h>
#include <model/config/Config.h>
#include <model/connect/ConnectHandler.h>
#include <model/contactlist/Contact.h>
#include <model/contactlist/ContactGroup.h>
#include <model/contactlist/ContactList.h>
#include <model/contactlist/IMContactListHandler.h>
#include <model/history/History.h>
#include <model/phonecall/PhoneCall.h>
#include <model/phoneline/PhoneLine.h>
#include <model/phoneline/PhoneLineState.h>
#include <model/phoneline/IPhoneLine.h>
#include <model/presence/PresenceHandler.h>
#include <model/profile/AvatarList.h>
#include <model/history/History.h>
#include <model/webservices/sms/WsSms.h>
#include <model/webservices/softupdate/WsSoftUpdate.h>
#include <model/webservices/info/WsInfo.h>
#include <model/wenbox/WenboxPlugin.h>
#include <sipwrapper/SipWrapper.h>
#include <thread/Thread.h>
#include <thread/ThreadEvent.h>
#include <util/File.h>
#include <util/Logger.h>
#include <util/SafeDelete.h>
#include <exception>
#include <iostream>

//zhenHua.sun 2010-07-28
#include <fstream>
#include <tchar.h>
#include <string>
using namespace std;

#include <model/conference/Log.h>
#include <model/network/NetworkUtil.h>
#include "qosdefine.h"  //qos相关定义文件

//zhenHua.sun 2010-09-01
#include <RunningProfile.h>			//运行参数
#include <SessionManager.h>
#include "RtpwareProxy.h"

#define DEFAULT_IP_ADDRESS  "0.0.0.0"        //默认的一个地址，用来表示单播会议的IP地址或启动graph用，实际不发送数据
#define DEFAULT_AUDIO_PORT 10000     //默认的音频端口
#define DEFAULT_VIDEO_PORT  10002        //默认的视频端口
#define DEFAULT_SCREEN_SHARE_PORT  12345        //默认的屏幕共享端口

#define  DEFAULT_SIMULATE_REPORT_SERVER_IP	"211.66.86.135"
#define  DEFAULT_SIMULATE_REPORT_SERVER_PORT	9981


/************************************************************************/
/*          一些工具函数                                                              */
/************************************************************************/

//根据cid在成员列表查找指定的会议信息，返回其指针，找不到则返回NULL
static inline ConfInfo* conf_find_by_cid(const vector<ConfInfo*>& confList, const string& cid)
{
    for(vector<ConfInfo*>::const_iterator iter = confList.begin(); iter != confList.end(); ++iter)
    {
        if(cid == (*iter)->CID)
            return (*iter);
    }
    return  NULL;
}

//根据uri在成员列表查找指定的会议信息，返回其指针，找不到则返回NULL
static inline ConfInfo* conf_find_by_uri(const vector<ConfInfo*>& confList, const string& uri)
{
    for(vector<ConfInfo*>::const_iterator iter = confList.begin(); iter != confList.end(); ++iter)
    {
        if(uri == (*iter)->URI)
            return (*iter);
    }
    return  NULL;
}

//根据title在成员列表查找指定的会议信息，返回其指针，找不到则返回NULL
static inline ConfInfo* conf_find_by_title(const vector<ConfInfo*>& confList, const string& title)
{
    for(vector<ConfInfo*>::const_iterator iter = confList.begin(); iter != confList.end(); ++iter)
    {
        if(title == (*iter)->Title)
            return (*iter);
    }
    return  NULL;
}

//根据uri在成员列表查找指定的会议信息，返回其指针，找不到则返回NULL
static inline MemberInfo* member_find_by_uri(const vector<MemberInfo*>& memberList, const string& uri)
{
    for(vector<MemberInfo*>::const_iterator iter = memberList.begin(); iter != memberList.end(); ++iter)
    {
        if(uri == (*iter)->MemberURI)
            return (*iter);
    }
    return  NULL;
}

static inline ReceiverInfo* receiver_find_by_uri( vector<ReceiverInfo>& receiverList, const string& uri)
{
    for(vector<ReceiverInfo>::iterator iter = receiverList.begin(); iter != receiverList.end(); ++iter)
    {
        if(uri == (*iter).MemberURI)
            return &(*iter);
    }
    return  NULL;
}

static inline ReceiverInfo* receiver_find_by_ip( vector<ReceiverInfo>& receiverList, const string& ip)
{
    for(vector<ReceiverInfo>::iterator iter = receiverList.begin(); iter != receiverList.end(); ++iter)
    {
        if(ip == (*iter).recvIP)
            return &(*iter);
    }
    return  NULL;
}

//删除列表中包括指定uri的元素
static inline void receiver_remove_by_uri( vector<ReceiverInfo>& receiverList, const string& uri)
{
    for(vector<ReceiverInfo>::iterator iter = receiverList.begin(); iter != receiverList.end(); )
    {
        if(uri == (*iter).MemberURI)
        {
            //删除元素，指向下一个迭代器
            iter = receiverList.erase(iter);
        }
        else
            ++iter;
    }
}

//删除列表中包括指定ip的元素
static inline void receiver_remove_by_ip( vector<ReceiverInfo>& receiverList, const string& ip)
{
    for(vector<ReceiverInfo>::iterator iter = receiverList.begin(); iter != receiverList.end(); )
    {
        if(ip == (*iter).recvIP)
        {
            //删除元素，指向下一个迭代器
            iter = receiverList.erase(iter);
        }
        else
            ++iter;
    }
}

//验证一个地址是否支持组播方法，若为空或0.0.0.0则不支持组播，其他地址则默认看成是支持组播
static inline bool check_support_multicast(const string& ipaddress)
{
    return ipaddress.length() > 0 && ipaddress != "0.0.0.0";
}

UserProfile::UserProfile()
{
	UserProfile::joiningMeeting = "";//Add By LZY 2010-10-09 初始化加入会议名

    _wsSms = NULL;
    _wsInfo = NULL;
    _wsCallForward = NULL;
    _wsSoftUpdate = NULL;
    _activePhoneLine = NULL;
    _activePhoneCall = NULL;
    _sipAccount = NULL;

    _imAccountManager = new IMAccountManager();
    _imContactListHandler = new IMContactListHandler(*this);
    _connectHandler = new ConnectHandler(this);
    _presenceHandler = new PresenceHandler(*this);
    _chatHandler = new ChatHandler(*this);
    _contactList = new ContactList(*this);
    _coIpManager = new CoIpManager(*this);

    _sipAccountMustConnectAfterInit = false;
    _sipAccountReadyToConnect = false;
    _wenboxPlugin = NULL;
    _historyLoaded = false;

    // Settings Avatar to default
    _icon = AvatarList::getInstance().getDefaultAvatarPicture();
    ////

    _history = new History(*this);
    /************************************************************************/
    _focusURI = "";
    _hostURI = "";
    _confMode = "";
    _confTitle = "instant meeting";
    _currentConfInfo = NULL;
    _previewHwnd = 0;
    //isInConference = false;
    _confState = NotInConference;
    _isShareScreen = false;
    /************************************************************************/

		RtpwareProxy::getInstance().setSimluteReportServer(DEFAULT_SIMULATE_REPORT_SERVER_IP,DEFAULT_SIMULATE_REPORT_SERVER_PORT);
}

UserProfile::~UserProfile()
{
    RecursiveMutex::ScopedLock lock(_mutex);

    // Disconnect events to prevent some crashes (call on deleted objects)
    if(_sipAccount)
    {
        _sipAccount->loginStateChangedEvent -= boost::bind(&UserProfile::loginStateChangedEventHandler, this, _1, _2);
        _sipAccount->networkDiscoveryStateChangedEvent -= boost::bind(&UserProfile::networkDiscoveryStateChangedEventHandler, this, _1, _2);
    }

    // Prevent some synchronization crashes by unregistering to these Events.
    _connectHandler->unregisterAllEvents();
    _imContactListHandler->unregisterAllEvents();
    _chatHandler->unregisterAllEvents();
    _presenceHandler->unregisterAllEvents();

    OWSAFE_DELETE(_activePhoneCall);
    OWSAFE_DELETE(_wsSms);
    OWSAFE_DELETE(_wsSoftUpdate);
    OWSAFE_DELETE(_wsInfo);
    OWSAFE_DELETE(_wsCallForward);
    OWSAFE_DELETE(_history);
    OWSAFE_DELETE(_wenboxPlugin);

    OWSAFE_DELETE(_coIpManager);
    OWSAFE_DELETE(_contactList);
    OWSAFE_DELETE(_imContactListHandler);
    OWSAFE_DELETE(_chatHandler);
    OWSAFE_DELETE(_presenceHandler);

    disconnect();

    if(_activePhoneLine)
    {
        //事件解绑
        _activePhoneLine->tranStatRecvEvent -= boost::bind(&UserProfile::tranStatRecvEventHandler, this, _1);
		_activePhoneLine->tranStatRecvEvent -= boost::bind(&UserProfile::tranStatRecvEventHandler, this, _2);



        Thread::sleep(5); // Sleep for PhApi: lets time to it to unregister
        _activePhoneLine->getSipWrapper().terminate();
    }

    _phoneLineList.clear();
    OWSAFE_DELETE(_activePhoneLine);
    OWSAFE_DELETE(_connectHandler);
    OWSAFE_DELETE(_imAccountManager);

    // HACK haaaack if _sipAccount is detroy when user try to
    //  connect with a bad password the softphone freeze....
    if(_sipAccount && (_sipAccount->getLastLoginState() != EnumSipLoginState::SipLoginStatePasswordError))
    {
        OWSAFE_DELETE(_sipAccount);
    }
    ////
}

void UserProfile::init()
{
    _wenboxPlugin = new WenboxPlugin(*this);

    // Emits IMAccountManager::imAccountAddedEvent
    _imAccountManager->emitIMAccountAddedEvent();
    ////

    // Connect sip account
    _sipAccountMustConnectAfterInit = true;
    sipAccountInit();
    ////

    // IMAccounts are now connected later ;
    // when SIPAccount is OK
}

std::string UserProfile::getProfileDirectory() const
{
    Config& config = ConfigManager::getInstance().getCurrentConfig();
    return File::convertPathSeparators(config.getConfigDir() + "profiles/" + _name + "/");
}

void UserProfile::connect()
{
    connectIMAccounts();
}

void UserProfile::connectIMAccounts()
{
    // Connects all IMAccounts
    // This will also connect the Wengo IMAccount. This will actually
    // call UserProfile::connectSipAccounts after being through the Connect class.
    IMAccountList imAccountList = _imAccountManager->getIMAccountListCopy();
    for(IMAccountList::const_iterator it = imAccountList.begin(); it != imAccountList.end(); ++it)
    {
        if((*it).getPresenceState() != EnumPresenceState::PresenceStateOffline)
        {
            _connectHandler->connect((*it).getUUID());
        }
    }
    ////
}

bool UserProfile::connectSipAccounts()
{
    RecursiveMutex::ScopedLock lock(_mutex);

    // This method is called by the Connect class.

    // Connect the SipAccount if not connected
    LOG_DEBUG("SIP_ In connectSipAccounts");
    if(_activePhoneLine && !_sipAccount->isConnected())
    {
        loginStateChangedEvent(*_sipAccount, EnumSipLoginState::SipLoginStateProgress);
        if(!_activePhoneLine->connect())
        {
            LOG_ERROR("Couldn't connect phoneline");
            return false;
        }

        LOG_DEBUG("SIP connection OK");
        return true;
    }
    return false;
    ////
}

void UserProfile::disconnect()
{
    disconnectIMAccounts();
}

void UserProfile::disconnectIMAccounts()
{
    IMAccountList imAccountList = _imAccountManager->getIMAccountListCopy();
    for(IMAccountList::const_iterator it = imAccountList.begin(); it != imAccountList.end(); ++it)
    {
        _connectHandler->disconnect((*it).getUUID());
    }
}

void UserProfile::disconnectSipAccounts(bool force)
{
    RecursiveMutex::ScopedLock lock(_mutex);

    // This method is called by the Connect class.

    if(_activePhoneLine && _sipAccount->isConnected())
    {
		//在断开连接之前先退出会议...zhenHua.sun 2010-12-06
		ConferenceSession * confSession = RunningProfile::getInstance().getSessionManager().getConfSession();
		if( confSession )
			this->exitConference( confSession->_callID );
		
        _activePhoneLine->disconnect(force);
    }
}

EnumMakeCallError::MakeCallError UserProfile::makeCall(Contact& contact)
{
    if(_activePhoneLine)
    {
        return _activePhoneLine->makeCall(contact.getPreferredNumber());
    }
    else
    {
        return EnumMakeCallError::NotConnected;
    }
}

EnumMakeCallError::MakeCallError UserProfile::makeCall(const std::string& phoneNumber)
{
    if(_activePhoneLine)
    {
        return _activePhoneLine->makeCall(phoneNumber);
    }
    else
    {
        return EnumMakeCallError::NotConnected;
    }
}

void UserProfile::startIM(Contact& contact)
{
    IMContactSet imContactSet;
    IMContact* imContact = contact.getPreferredIMContact();

    if(imContact)
    {
        imContactSet.insert(*imContact);
        _chatHandler->createSession(imContact->getIMAccountId(), imContactSet);
    }
    else
    {
        LOG_ERROR("There is no IMContact available");
    }
}

void UserProfile::setSipAccount(const SipAccount& sipAccount, bool needInitialization)
{
    if(_sipAccount)
    {
        IPhoneLine* phoneLine = findWengoPhoneLine();
        if(phoneLine)
        {
            phoneLine->disconnect();
            //TODO remove the PhoneLine from _phoneLines & destroy it
        }

        std::vector<EnumIMProtocol::IMProtocol> protocols;
        protocols.push_back(EnumIMProtocol::IMProtocolWengo);
        protocols.push_back(EnumIMProtocol::IMProtocolSIP);
        protocols.push_back(EnumIMProtocol::IMProtocolSIPSIMPLE);
        IMAccountList imAccountList = _imAccountManager->getIMAccountsOfProtocolVector(protocols);
        if(imAccountList.size() > 0)
        {
            _removeIMAccount(*imAccountList.begin());
        }
        OWSAFE_DELETE(_sipAccount);
    }

    if(sipAccount.getType() == SipAccount::SipAccountTypeWengo)
    {
        const WengoAccount& wengoAccount = dynamic_cast<const WengoAccount&>(sipAccount);
        //Empty login or password
        if(wengoAccount.getWengoLogin().empty() || wengoAccount.getWengoPassword().empty())
        {
            loginStateChangedEvent(*_sipAccount, EnumSipLoginState::SipLoginStatePasswordError);
            return;
        }

        _sipAccount = new WengoAccount(wengoAccount);
    }
    else
    {
        if(sipAccount.getIdentity().empty())
        {
            loginStateChangedEvent(*_sipAccount, EnumSipLoginState::SipLoginStatePasswordError);
            return;
        }

        _sipAccount = new SipAccount(sipAccount);
    }

    computeName();
    _sipAccount->networkDiscoveryStateChangedEvent += boost::bind(&UserProfile::networkDiscoveryStateChangedEventHandler, this, _1, _2);
    _sipAccount->loginStateChangedEvent += boost::bind(&UserProfile::loginStateChangedEventHandler, this, _1, _2);

    if(needInitialization)
    {
        // Discovers network.
        // Sends the HTTP request to the SSO if _sipAccount is a Wengo account.
        _sipAccountMustConnectAfterInit = false;
        _sipAccountReadyToConnect = false;
        _sipAccount->init();
    }
}

void UserProfile::addIMAccount(const IMAccount& imAccount)
{
    /*
    This code is not used anymore because of ConfigImporter update.
    if (imAccount.getProtocol() == EnumIMProtocol::IMProtocolWengo) {
        LOG_FATAL("cannot add directly a Wengo IMAccount. Must use setWengoAccount");
        return;
    }
    */


    _addIMAccount(imAccount);
}

void UserProfile::_addIMAccount(const IMAccount& imAccount)
{
    LOG_DEBUG("adding an IMAccount");

    _imAccountManager->addIMAccount(imAccount);
}

void UserProfile::removeIMAccount(const IMAccount& imAccount)
{
    if(imAccount.getProtocol() == EnumIMProtocol::IMProtocolWengo)
    {
        LOG_FATAL("cannot remove directly a Wengo IMAccount. Must use setWengoAccount");
        return;
    }

    _removeIMAccount(imAccount);
}

void UserProfile::_removeIMAccount(const IMAccount& imAccount)
{
    LOG_DEBUG("removing an IMAccount");

    if(_imAccountManager->contains(imAccount.getUUID()))
    {
        //_connectHandler->disconnect(imAccount.getUUID());
        _imAccountManager->removeIMAccount(imAccount);
    }
    else
    {
        LOG_ERROR("IMAccount not in IMAccountManager");
    }
}

void UserProfile::updateIMAccount(const IMAccount& imAccount)
{
    LOG_DEBUG("updating IMAccount");

    _imAccountManager->updateIMAccount(imAccount);
}

EnumPresenceState::PresenceState UserProfile::getPresenceState() const
{
    IMAccountList imAccountList = _imAccountManager->getIMAccountListCopy();

    IMAccountList::const_iterator it = imAccountList.begin(),
                                  end = imAccountList.end();

    // There is no account
    if(it == end)
    {
        return EnumPresenceState::PresenceStateUnavailable;
    }

    // Check whether all states are the same
    EnumPresenceState::PresenceState state = it->getPresenceState();
    ++it;

    for(; it != end; ++it)
    {
        if((*it).getPresenceState() != state)
        {
            // They are not
            return EnumPresenceState::PresenceStateMulti;
        }
    }

    // All states are the same
    return state;
}

void UserProfile::setPresenceState(EnumPresenceState::PresenceState presenceState, std::string imAccountId)
{
    IMAccount* realIMAccount = _imAccountManager->getIMAccount(imAccountId);
    if(realIMAccount)
    {
        if(!realIMAccount->isConnected())
        {
            _connectHandler->connect(imAccountId);
        }

        _presenceHandler->changeMyPresenceState(presenceState, String::null, realIMAccount);
        OWSAFE_DELETE(realIMAccount);
    }
}

void UserProfile::setAllPresenceState(EnumPresenceState::PresenceState presenceState)
{
    if(!isConnected())
    {
        connect();
    }

    _presenceHandler->changeMyPresenceState(presenceState, String::null, NULL);
}

void UserProfile::setAlias(const string& alias, IMAccount* imAccount)
{
    _alias = alias;
    _presenceHandler->changeMyAlias(_alias, imAccount);
}

bool UserProfile::addPhoneLine(SipAccount& account)
{
    //Creates new a PhoneLine associated with the account just added
    PhoneLine* phoneLine = new PhoneLine(account, *this);
    if(!phoneLine->init())
    {
        LOG_ERROR("PhoneLine initialisation failed");
        return false;
    }

    //Adds the PhoneLine to the list of PhoneLine
    _phoneLineList += phoneLine;

    //Sets the active PhoneLine
    //FIXME should the last added PhoneLine be the active one all the time?
    if(!_activePhoneLine)
    {
        _activePhoneLine = phoneLine;
        //绑定事件
        _activePhoneLine->tranStatRecvEvent += boost::bind(&UserProfile::tranStatRecvEventHandler, this, _1);
		_activePhoneLine->rtpStatRecvEvent += boost::bind(&UserProfile::rtpStatRecvEventHandler, this, _2);

    }


    return true;
}

void UserProfile::sipAccountInit()
{
    if(_sipAccount)
    {
        _sipAccount->networkDiscoveryStateChangedEvent += boost::bind(&UserProfile::networkDiscoveryStateChangedEventHandler, this, _1, _2);
        _sipAccount->loginStateChangedEvent += boost::bind(&UserProfile::loginStateChangedEventHandler, this, _1, _2);

        // Detect network and launch SSO request if needed WengoAccount
        _sipAccountReadyToConnect = false;
        _sipAccount->init();
    }
}

void UserProfile::loginStateChangedEventHandler(SipAccount& sender, EnumSipLoginState::SipLoginState state)
{
    // FIXME: due to a threads synchronization problem, it's better to comment this part of code temporarily

    typedef ThreadEvent2<void (SipAccount sender, EnumSipLoginState::SipLoginState state), SipAccount, EnumSipLoginState::SipLoginState> MyThreadEvent;
    MyThreadEvent* event = new MyThreadEvent(boost::bind(&UserProfile::loginStateChangedEventHandlerThreadSafe, this, _1, _2), dynamic_cast<SipAccount&>(sender), state);

    WengoPhone::getInstance().postEvent(event);

    //loginStateChangedEvent(sender, state);
    //loginStateChangedEventHandlerThreadSafe(sender, state);
}

void UserProfile::loginStateChangedEventHandlerThreadSafe(SipAccount& sender, EnumSipLoginState::SipLoginState state)
{
    RecursiveMutex::ScopedLock lock(_mutex);

    LOG_DEBUG("SIP_ " + _name);

    switch(state)
    {
    case EnumSipLoginState::SipLoginStateReady:
    {
        LOG_DEBUG("SIP_ READY");

        // if no sip account (generic or wengo) is set : create one !!
        IMAccountList imAccountList;
        if(_sipAccount->getType() == SipAccount::SipAccountTypeWengo)
        {
            imAccountList = _imAccountManager->getIMAccountsOfProtocol(EnumIMProtocol::IMProtocolWengo);
            if(imAccountList.size() == 0)
            {
                IMAccount imAccount(_sipAccount->getIdentity(), _sipAccount->getPassword(), EnumIMProtocol::IMProtocolWengo);
                imAccount.setConnected(false);
                _addIMAccount(imAccount);
            }
        }
        else
        {
            std::vector<EnumIMProtocol::IMProtocol> protocols;
            protocols.push_back(EnumIMProtocol::IMProtocolSIP);
            protocols.push_back(EnumIMProtocol::IMProtocolSIPSIMPLE);
            imAccountList = _imAccountManager->getIMAccountsOfProtocolVector(protocols);
            if(imAccountList.size() == 0)
            {
                IMAccount imAccount(_sipAccount->getIdentity(), _sipAccount->getPassword(), EnumIMProtocol::IMProtocolSIP);
                imAccount.setConnected(false);
                _addIMAccount(imAccount);
            }
        }
        ////

        if(_sipAccountMustConnectAfterInit)
        {
            _sipAccountMustConnectAfterInit = false;

            if(!addPhoneLine(*_sipAccount))
            {
                LOG_ERROR("Couldn't add phoneline");
                sipAccountConnectionFailedEvent(EnumSipLoginState::SipLoginStateNetworkError);
                break;
            }

            if(_sipAccount->getType() == SipAccount::SipAccountTypeWengo)
            {
                WengoAccount* wengoAccount = dynamic_cast<WengoAccount*>(_sipAccount);

                //Creates SMS, SMS needs a WengoAccount
                _wsSms = new WsSms(wengoAccount, *this);
                wsSmsCreatedEvent(*this, *_wsSms);
                LOG_DEBUG("SMS created");

                //Creates SoftUpdate, SoftUpdate needs a WengoAccount
                _wsSoftUpdate = new WsSoftUpdate(wengoAccount);
                wsSoftUpdateCreatedEvent(*this, *_wsSoftUpdate);
                LOG_DEBUG("SoftUpdate created");

                _wsInfo = new WsInfo(wengoAccount);
                wsInfoCreatedEvent(*this, *_wsInfo);
                LOG_DEBUG("WsInfo created");

                //callforward
                _wsCallForward = new WsCallForward(wengoAccount);
                wsCallForwardCreatedEvent(*this, *_wsCallForward);
                _wsCallForward->wsCallForwardEvent += boost::bind(&UserProfile::wsCallForwardEventHandler, this, _1, _2, _3);
            }
            phoneLineCreatedEvent(*this, *_activePhoneLine);

            loadHistory(getProfileDirectory());

            //Connect SipAccounts and IMAccounts;
            connect();
        }

        //sipAccountConnectedEvent();       这个事件应该在SipLoginStateConnected的时候调用...zhenHua.sun 2010-09-10

        _sipAccountReadyToConnect = true;

        break;
    }

    case EnumSipLoginState::SipLoginStatePasswordError:
    {
        LOG_DEBUG("SIP_ PASSWORD ERROR");
        //if (!_sipAccountReadyToConnect)
        //{
        //    sipAccountConnectionFailedEvent(EnumSipLoginState::SipLoginStatePasswordError);
        //}如果登录失败的话，直接发起事件，不再判断_sipAccountReadyToConnect的值，因为会导致无法正常工作
        sipAccountConnectionFailedEvent(EnumSipLoginState::SipLoginStatePasswordError);

        break;
    }

    case EnumSipLoginState::SipLoginStateConnected:
    {
        LOG_DEBUG("SIP_ CONNECTED");

        sipAccountConnectedEvent();

        break;
    }

    case EnumSipLoginState::SipLoginStateDisconnected:
    {
        LOG_DEBUG("SIP_ DISCONNECTED");
        break;
    }

    case EnumSipLoginState::SipLoginStateNetworkError:
    {
        LOG_DEBUG("SIP_ NetworkError");
        sipAccountConnectionFailedEvent(EnumSipLoginState::SipLoginStateNetworkError);
        break;
    }

    default:
        ;
    }
}

void UserProfile::networkDiscoveryStateChangedEventHandler(SipAccount& sender, SipAccount::NetworkDiscoveryState state)
{
    networkDiscoveryStateChangedEvent(sender, state);
}

IPhoneLine* UserProfile::findWengoPhoneLine()
{
    for(unsigned i = 0; i != _phoneLineList.size() ; i++)
    {
        const SipAccount& sipAccount = _phoneLineList[i]->getSipAccount();

        if(sipAccount.getType() == SipAccount::SipAccountTypeWengo)
        {
            return _phoneLineList[i];
        }
        else
        {
            LOG_DEBUG("this SipAccount is not a WengoAccount");
        }
    }
    return NULL;
}

void UserProfile::setIcon(const OWPicture& icon, IMAccount* imAccount)
{
    Profile::setIcon(icon);
    _presenceHandler->changeMyIcon(icon, imAccount);
}

bool UserProfile::loadHistory(const std::string& path)
{
    bool toReturn = _history->load(path + "history.xml");
    _historyLoaded = true;
    historyLoadedEvent(*this, *_history);
    return toReturn;
}

bool UserProfile::saveHistory(const std::string& path)
{
    return _history->save(path + "history.xml");
}

bool UserProfile::hasWengoAccount() const
{
    return (hasSipAccount() && (_sipAccount->getType() == SipAccount::SipAccountTypeWengo));
}

bool UserProfile::hasSipAccount() const
{
    return _sipAccount != NULL;
}

void UserProfile::wsCallForwardEventHandler(WsCallForward& sender, int id, WsCallForward::WsCallForwardStatus status)
{
    if(status == WsCallForward::WsCallForwardStatusOk)
    {
        _wsInfo->getWengosCount(false);
        _wsInfo->getSmsCount(false);
        _wsInfo->getActiveMail(false);
        _wsInfo->getUnreadVoiceMail(false);
        _wsInfo->getLandlineNumber(false);
        _wsInfo->getCallForwardInfo(true);
        _wsInfo->execute();
    }
}

void UserProfile::computeName()
{
    _name = _sipAccount->getFullIdentity();
}

bool UserProfile::isConnected() const
{
    //Checks all IMAccounts
    IMAccountList imAccountList = _imAccountManager->getIMAccountListCopy();

    for(IMAccountList::const_iterator it = imAccountList.begin(); it != imAccountList.end(); ++it)
    {
        if((*it).isConnected())
        {
            return true;
        }
    }

    return false;
}

/************************************************************************/
/*Start Conference*/
void UserProfile::startConference(const std::string& confURI, const std::string& szContent)
{
    if(_activePhoneLine)
    {
        _activePhoneLine->cvMessageReceivedEvent += boost::bind(&UserProfile::cvMessageReceivedEventHandler, this, _2, _3);
        if(szContent.find("ConferenceInvite") != string::npos)
        {
            ConfInfo* conf = conf_find_by_uri(_confList, _focusURI);
            if(conf != NULL)
            {
                CVMsgParser cvMessageParser;
                string content = cvMessageParser.CreateMsgByConfInfo(conf);
                return _activePhoneLine->startConference(confURI, content);
            }
        }
        if(szContent.find("ApplyConference") != string::npos)
        {
            //申请加入会议
            if(_confState == IsInConference)
            {
                return;
            }
            CVMsgParser cvMessageParser;
            cvMessageParser.InitParser(szContent.c_str());
            cvMessageParser.ParseConfListMsg(_confList);
            ConfInfo* conf = _confList[_confList.size() - 1];
            _confTitle = conf->Title;
            _confMode = conf->ControlMode;
            _currentConfInfo = NULL;
            _hostURI = getCurrentUserName();
        }
        return _activePhoneLine->startConference(confURI, szContent);
    }
}

//???????
void UserProfile::exitConference(int& callId)
{
    if(_activePhoneLine)
    {
        _confState = ExitingConference;     //标记为正在退出  ...zhenHua.sun 2010-09-08

        _activePhoneLine->closeCall(callId);
        _activePhoneLine->exit_conference(_focusURI);

        //发送消息给qos模拟器
       // sendQosExitConfReport(_focusURI);

        CVMsgParser::ReleaseMemberList(_memberList);

        //SendGraph将会在exit_conference函数中被销毁，这里是多余的...zhenHua.sun 2010-09-08.
        //screen_share_send_stop(sendAddress, DEFAULT_SCREEN_SHARE_PORT);
        //media_send_stop(sendAddress, send_audio_port, sendAddress, send_video_port);

        _focusURI = "";
        _currentConfInfo = NULL;
        _confState = NotInConference;
		UserProfile::ExitMeetingSuccessEvent();//Add By LZY 2010-09-30 发出退出会议完成事件
        _isShareScreen = false;
        _sendIPList.clear();
        _screenShareSendIPList.clear();
        _requestForSpeakList.clear();
        _speakingList.clear();
		UserProfile::joiningMeeting = "";//Add By LZY 2010-10-09 删除原来加入的会议名
        _activePhoneLine->setCallIdEvent -= boost::bind(&UserProfile::setCallIdEventHandler, this, _2);
        //      cvMemberListReceivedEvent(*this,MemberList);
    }
}

int UserProfile::makeJoinConfInvite(const int& index)
{
    if(index < 0 || index >= _confList.size())
        return -1;
    ConfInfo* conf = (ConfInfo*)_confList[index];
    return  makeJoinConfInvite(conf->URI);
}

int UserProfile::makeJoinConfInvite(const std::string& focus_URI)
{
    switch(_confState)
    {
    case IsInConference:
    {
        //用户已经在一个会议中
		return -2;
    }
    break;
    case ExitingConference:
    {
        //用户正在退出会议
        return -3;
    }
    break;
    case NotInConference:
    case WaitingPermission:
        //抛弃原来的准入等待，重新申请
    case JoinRejected:
        //可以申请加入一个新的会议
    {
        //根据uri查找会议信息
        ConfInfo* conf = conf_find_by_uri(_confList, focus_URI);
        if(!conf)
            return -4;
        if("password" == conf->JoinMode)
            return -1;
        //更新当前会议信息
        _focusURI = focus_URI;
        string szContent = createJoinConferenceMsg(focus_URI);
        if(!_activePhoneLine)
            return -10;
        _activePhoneLine->setCallIdEvent += boost::bind(&UserProfile::setCallIdEventHandler, this, _2);
        UserProfile::joiningMeeting = focus_URI;//Add By LZY 2010-10-09 填写加入的会议名
		_activePhoneLine->makeJoinConfInvite(focus_URI, szContent);
        //更新当前会议标题
        _confTitle = conf->Title;
        _currentConfInfo = NULL;

        //改变会议状态为正在验证中
        _confState = WaitingPermission;
        return 0;
    }
    break;
    default:
        break;
    }
    return -10;
}

void UserProfile::setCallIdEventHandler(int callId)
{
    setCallIdEvent(*this, callId, _previewHwnd);
}

//???y?????????
bool UserProfile::setRecvButtonStatus(int index)
{
    //  int row = index;
    MemberInfo* mlist = (MemberInfo*)_memberList[index];
    if(mlist->isReceived == 0)
    {
        return false;
    }
    else
        return true;
}
//????y????
int UserProfile::media_send_start(char* AudioIP, int AudioPort, char* VideoIP, int VideoPort, HWND previewHwnd)
{
    if(AudioPort > 0 || VideoPort > 0)
    {
        Config& config = ConfigManager::getInstance().getCurrentConfig();
        int width = config.getVideoFrameWidth();
        int height = config.getVideoFrameHeight();
        if(AudioIP == NULL && AudioPort == 0)
            return _activePhoneLine->media_send_start(NULL, 0, VideoIP, VideoPort, previewHwnd, width, height);
        else
            return _activePhoneLine->media_send_start(AudioIP, AudioPort, VideoIP, VideoPort, previewHwnd, width, height);
    }
    return -1;
}

//??????y????
void UserProfile::media_send_stop(char* AudioIP, int AudioPort, char* VideoIP, int VideoPort)
{
    if(AudioPort > 0 || VideoPort > 0)
    {
       //for (vector<ReceiverAddr>::iterator it=sendIPList.begin();it!=sendIPList.end();it++)
        //{
        //  char sendAddress[50];
        //  strcpy(sendAddress,((ReceiverAddr)(*it)).recvIP.c_str());
        //  _activePhoneLine->media_send_stop(sendAddress,send_audio_port,sendAddress,send_video_port);
        //}
        _sendIPList.clear();
    }
}

//开始接收媒体流
int UserProfile::media_recv_start(string userId, char* AudioIP, int AudioPort, char* VideoIP, int VideoPort, char* codecMIME, HWND remoteHwnd, int videoWidth, int videoHeight)
{
    return _activePhoneLine->media_recv_start(userId, AudioIP, AudioPort, "SPEEX", 16000, 16, 1, VideoIP, VideoPort, codecMIME, remoteHwnd, videoWidth, videoHeight);
}

//重载media_recv_start函数，增加了四个参数
int UserProfile::media_recv_start(string userId,
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
                                  int videoHeight)
{
    return _activePhoneLine->media_recv_start(userId, AudioIP, AudioPort, audioCodecMIME, audioSampleRate, audioBps, audioChannels, VideoIP, VideoPort, codecMIME, remoteHwnd, videoWidth, videoHeight);
}


int UserProfile::media_recv_start(int index, HWND remoteHwnd)
{
    int result;
    int audioPort = 0, videoPort = 0;
    int width = 352;
    int height = 288;
    char mediaType[10];
    string userId;
    memset(mediaType, 0, sizeof(mediaType));
    int row = index;
    //row = MemberList.size()-1-row;
    MemberInfo* remote_member = (MemberInfo*)_memberList[row];

#ifndef   DBUS_ENABLE
    if(remote_member->isReceived == 1)
    {
        return -1;
    }
#endif

    char audioCodec[30] = "SPEEX";
    int audioSampleRate =16000;
    int audioBps = 16;
    int audioChannels = 1;
	//音频编码参数写死。
    getMediaParamFromSdp(audioPort, videoPort, mediaType, width, height, audioCodec, audioSampleRate, audioBps, audioChannels, remote_member->Sdp.c_str());
    int pos = remote_member->MemberURI.find('@');
    if(pos != std::string::npos)
    {
        userId = remote_member->MemberURI.substr(0, pos);
    }
    else
        userId = remote_member->MemberURI;

    //获取接收地址，监听对方的端口
    string localAddr;
    if(check_support_multicast(remote_member->multicastAddress))       //组播会议
    {
        getCurrentMulticastAddress(localAddr);	
    }
    else
    {
        //单播会议
        Config& config = ConfigManager::getInstance().getCurrentConfig();
        localAddr = config.getIPAddress();
	
    }
	//监听对方的端口
	videoPort = remote_member->videoPort;
	audioPort = remote_member->audioPort;

    //根据指定参数创建接收Graph，与原来的函数相比增加了四个参数
    result = media_recv_start(userId, (char*)localAddr.c_str(), audioPort, audioCodec, audioSampleRate, audioBps, audioChannels, (char*)localAddr.c_str(), videoPort, mediaType, remoteHwnd, width, height);
 
    //发送SIP消息通知对方发送数据
    string szContent = "<?xml version=\"1.0\"?><coolview command='Receiver'><ip>" + localAddr + "</ip></coolview>";
    _activePhoneLine->startConference("sip:" + remote_member->MemberURI, szContent);

#ifndef   DBUS_ENABLE
    if(result == 0)
    {
        remote_member->isReceived = 1;
    }
#endif

    return result;
}

//停止接收媒体流
void UserProfile::media_recv_stop(std::string userId)
{
    for(int i = 0; i < _memberList.size(); i++)
    {
        MemberInfo* member = _memberList[i];
        int pos = member->MemberURI.find('@');
        if(userId != member->MemberURI.substr(0, pos))
            continue;
        //如果接收的时候单播媒体流，则不接收的时候通知发送方
        if(!check_support_multicast(member->multicastAddress))
        {
            Config& config = ConfigManager::getInstance().getCurrentConfig();
            string localAddr = config.getIPAddress();
            //告诉发送方停止发送媒体流到本机地址?
            string szContent = "<?xml version=\"1.0\"?><coolview command='Cancel2Receiver'><ip>" + localAddr + "</ip></coolview>";
            _activePhoneLine->startConference("sip:" + _memberList[i]->MemberURI, szContent);
        }
        _activePhoneLine->media_recv_stop(userId);
    }
}

//???????
int UserProfile::screen_share_send_start(char* ScreenIP, int ScreenPort, int x, int y, int width, int height)
{
    if(_confState != IsInConference)
    {
        return -1;
    }
    if(checkCurrentIsMulticastConference())
    {
        string multicastAddress;
        getCurrentMulticastAddress(multicastAddress);
        _activePhoneLine->screen_share_send_start((char*)multicastAddress.c_str(), DEFAULT_SCREEN_SHARE_PORT, x, y, width, height);
    }
    _shareRect.left = x;
    _shareRect.top = y;
    _shareRect.right = x + width;
    _shareRect.bottom = y + height;
    _isShareScreen = true;
    return 0;
}

//??????湲??
void UserProfile::screen_share_send_stop(char* ScreenIP, int ScreenPort)
{
    for(vector<ReceiverInfo>::iterator it = _screenShareSendIPList.begin(); it != _screenShareSendIPList.end(); it++)
    {
        _activePhoneLine->screen_share_send_stop((char*)(*it).recvIP.c_str(), DEFAULT_SCREEN_SHARE_PORT);
    }
    _screenShareSendIPList.clear();
    _isShareScreen = false;
}

//???????湲??
int UserProfile::screen_share_recv_start(char* ScreenIP, int ScreenPort, HWND remoteHwnd, int videoWidth, int videoHeight)
{
    char recvAddress[50];
    if(_confState != IsInConference)
    {
        return -1;
    }
    if(_hostURI == "")
        return -2;

    //获取接收地址
    string localAddr;
    if(checkCurrentIsMulticastConference())       //组播会议
    {
        getCurrentMulticastAddress(localAddr);
    }
    else
    {
        //单播会议
        Config& config = ConfigManager::getInstance().getCurrentConfig();
        localAddr = config.getIPAddress();
    }
    int result = _activePhoneLine->screen_share_recv_start((char*)localAddr.c_str(), DEFAULT_SCREEN_SHARE_PORT, remoteHwnd, videoWidth, videoHeight);
    if(result != 0)
        return result;
    //SIP消息通知对方发送共享数据
    string szContent = "<?xml version=\"1.0\"?><coolview command='ScreenReceiver'><ip>" + localAddr + "</ip></coolview>";
    _activePhoneLine->startConference("sip:" + _hostURI, szContent);
    return result;
}

//??????????湲??
void UserProfile::screen_share_recv_stop()
{
    if(!checkCurrentIsMulticastConference())
    {
        //单播会议通知对方停止发送数据
        Config& config = ConfigManager::getInstance().getCurrentConfig();
        string localAddr = config.getIPAddress();
        string szContent = "<?xml version=\"1.0\"?><coolview command='ScreenCancel2Receiver'><ip>" + localAddr + "</ip></coolview>";
        _activePhoneLine->startConference("sip:" + _hostURI, szContent);
    }
    _activePhoneLine->screen_share_recv_stop();
}

void UserProfile::exit_conference()
{
    //  _activePhoneLine->exit_conference();
}

//????????????????
void UserProfile::setPreviewWin(HWND& Hwnd)
{
    _previewHwnd = Hwnd;
}

//video????resize
void UserProfile::resizeVideoWindow(const std::string& userId, const HWND& remoteHwnd, int fullscreenflag)
{
    _activePhoneLine->resizeVideoWindow(userId, remoteHwnd, fullscreenflag);
}

//不接收某路音视频
void UserProfile::unReceiveMedia(const std::string& userId)
{
    media_recv_stop(userId);
    for(vector<MemberInfo*>::iterator it = _memberList.begin(); it != _memberList.end(); it++)
    {
        MemberInfo* mInfo = (MemberInfo*)(*it);
        int index = mInfo->MemberURI.find('@');
        if(index != std::string::npos)
        {
            string _userId = mInfo->MemberURI.substr(0, index);
            if(_userId == userId)
            {
                mInfo->isReceived = 0;
            }
        }
    }
}

void UserProfile::getMediaParamFromSdp(int& aport, int& vport, char* videoCodecMIME, int& width, int& height, char* audioCodecMIME, int& audioSampleRate, int& audioBitsPerSample, int& audioChannels, const char* sdp)
{
    char audioPort[10];
    char videoPort[10];
    char width_str[10];
    char height_str[10];

    char* temp, * temp2;
    if(!sdp || strcmp("", sdp) == 0)
        return;
    temp = strstr((char*)sdp, "m=audio");
    if(temp)
    {
        temp += strlen("m=audio") + 1;
        temp2 = temp;
        while(*temp != ' ')
        {
            temp++;
        }
        strncpy(audioPort, temp2, temp - temp2);
        aport = atoi(audioPort);
    }

	////记录音频参数...............zhenHua.sun 2010-08-03
	//char audio_sample_rate_str[10];
	//char audio_bps_str[10];
	//char audio_channels_str[10];
    //if(strstr(temp2, "SPEEX"))
    //{
    //    temp = strstr(temp2, "a=rtpmap");
    //    if(temp)
    //    {
    //        temp += strlen("a=rtpmap") + 1;
    //        while(*temp != ' ')
    //        {
    //            temp++;
    //        }
    //        temp++;
    //        temp2 = temp;
    //        while(*temp != '/')
    //        {
    //            temp++;
    //        }
    //        strncpy(audioCodecMIME, temp2, temp - temp2);       //获取音频编解码类型

    //        temp++;
    //        temp2 = temp;
    //        while(*temp != '/')
    //        {
    //            temp++;
    //        }
    //        strncpy(audio_sample_rate_str, temp2, temp - temp2);    //音频采样频率

    //        temp++;
    //        if(*temp != '1')
    //        {
    //            //之所以会有这个判断是为了兼容没有修改过的终端发过来的sdp<speex/8000/1> ..zhenHua.sun
    //            temp2 = temp;
    //            while(*temp != '/')
    //            {
    //                temp++;
    //            }
    //            strncpy(audio_bps_str, temp2, temp - temp2);    //音频的bps

    //            temp++;
    //            temp2 = temp;
    //            while(*temp != '/')
    //            {
    //                temp++;
    //            }
    //            strncpy(audio_channels_str, temp2, temp - temp2);   //音频的声道数

    //            //为输出参数赋值
    //            audioSampleRate = atoi(audio_sample_rate_str);
    //            audioBitsPerSample = atoi(audio_bps_str);
    //            audioChannels = atoi(audio_channels_str);
    //        }
    //    }
    //}
    //else
    //{
    //    strcpy(audioCodecMIME, "SPEEX");
    //    audioSampleRate = 16000;
    //    audioBitsPerSample = 16;
    //    audioChannels = 1;
    //}
	//固定音频参数
	audioSampleRate = 16000;
	audioBitsPerSample = 16;
	audioChannels = 1;

    temp = strstr((char*)sdp, "m=video");
    if(temp)
    {
        temp += strlen("m=video") + 1;
        temp2 = temp;
        while(*temp != ' ')
        {
            temp++;
        }
        strncpy(videoPort, temp2, temp - temp2);
        vport = atoi(videoPort);
    }
    else
    {
        vport = aport - 2;
        strcpy(videoCodecMIME, "XVID");
        return;
    }

    temp = strstr(temp2, "CUSTOM");
    if(temp)
    {
        while(*temp != '\r' && *temp != '\n' && *temp != '\0' && *temp != '=')
        {
            temp++;
        }
        temp++;
        temp2 = temp;
        while(*temp2 != '\r' && *temp2 != '\n' && *temp2 != '\0' && *temp2 != ',')
        {
            temp2++;
        }
        temp2++;
        strncpy(width_str, temp, temp2 - 1 - temp);

        temp = temp2;
        while(*temp2 != '\r' && *temp2 != '\n' && *temp2 != '\0' && *temp2 != ' ' && *temp2 != ',')
        {
            temp2++;
        }
        strncpy(height_str, temp, temp2 - temp);
        width = atoi(width_str);
        height = atoi(height_str);
    }

    temp = strstr(temp2, "a=rtpmap");
    if(temp)
    {
        temp += strlen("a=rtpmap") + 1;
        while(*temp != ' ')
        {
            temp++;
        }
        temp++;
        temp2 = temp;
        while(*temp != '/')
        {
            temp++;
        }
        strncpy(videoCodecMIME, temp2, temp - temp2);
    }
}



//???????
void UserProfile::forbidSpeak()
{
    for(int i = 0; i < _sendIPList.size(); i++)
    {
        _activePhoneLine->forbidSpeak((char*)((_sendIPList[i]).recvIP.c_str()), _sendIPList[i].audioPort);
    }
}

void UserProfile::forbidSpeak(string userId)
{
    char* _userId = const_cast<char*>(userId.c_str());
    _activePhoneLine->forbidSpeak(_userId);
}
//???????
void UserProfile::permitSpeak()
{
    for(int i = 0; i < _sendIPList.size(); i++)
    {
        _activePhoneLine->permitSpeak((char*)((_sendIPList[i]).recvIP.c_str()), _sendIPList[i].audioPort);
    }
}

//?????????????????Focus??????????
int UserProfile::checkPassword(int& row, std::string& password)
{
    if(row < 0 || row >= _confList.size())
        return -1;
    ConfInfo* conf = (ConfInfo*)_confList[row];
    //????????????????????
    if("password" == conf->JoinMode && password == conf->JoinPassword)
    {
        //更新当前会议信息
        _focusURI = conf->URI;
        string szContent = createJoinConferenceMsg(_focusURI);
        if(!_activePhoneLine)
            return -10;
        _activePhoneLine->setCallIdEvent += boost::bind(&UserProfile::setCallIdEventHandler, this, _2);
        _activePhoneLine->makeJoinConfInvite(_focusURI, szContent);
        //更新当前会议标题
        _confTitle = conf->Title;
        _currentConfInfo = NULL;

        //改变会议状态为正在验证中
        _confState = WaitingPermission;
        return 0;
    }

    return -1;
}

//??鵱?????????????
bool UserProfile::hostOrNot()
{
    string currentUser = getCurrentUserName();
    //?????????????????????????????
    if(_confMode == "host" && currentUser == _hostURI)
    {
        return true;
    }
    return false;
}

//????????????????????????
int UserProfile::memberSpeakStatus(int row)
{
    MemberInfo* mlist = (MemberInfo*)_memberList[row];
    return mlist->permitSpeak;
}

//?????????
int UserProfile::setSpeaker(int row, bool permitOrForbid, int callId)
{
    std::string command;
    std::string memberUri;
    if(permitOrForbid)
        command = "SetSpeaker";
    else
        command = "ForbidSpeaker";
    //??????
    if(row == -1)
    {
        memberUri = "all";
        if(permitOrForbid)
        {
            //????????????????
            for(list<MemberInfo*>::iterator it = _requestForSpeakList.begin(); it != _requestForSpeakList.end(); it++)
            {
                _speakingList.push_back(*it);
            }
            _requestForSpeakList.clear();
        }
        else
        {
            //?????????????
            _speakingList.clear();
        }
    }
    //????????е????????
    else if(row == -2)
    {
        memberUri = getFirstMember(permitOrForbid);
        if(memberUri == "")
            return 0;
    }
    else
    {
        MemberInfo* mlist = (MemberInfo*)_memberList[row];
        memberUri = mlist->MemberURI;
        if(permitOrForbid)
        {
            //????????????????
            list<MemberInfo*>::iterator it = find(_requestForSpeakList.begin(), _requestForSpeakList.end(), mlist);
            if(it != _requestForSpeakList.end())
                _requestForSpeakList.erase(it);
            _speakingList.push_back(mlist);
        }
        else
        {
            //?????????????
            list<MemberInfo*>::iterator it = find(_speakingList.begin(), _speakingList.end(), mlist);
            if(it != _speakingList.end())
                _speakingList.erase(it);
        }
    }
    std::string szContent = "<?xml version=\"1.0\"?><coolview command=\"" + command + "\"><uri>" + memberUri + "</uri></coolview>";
    _activePhoneLine->makeInfoCall(callId, (char*)szContent.c_str());
    return 0;
}

//????????
int UserProfile::KickOut(int row, int callId)
{
    if(row < 0)
        return -1;
    MemberInfo* mlist = (MemberInfo*)_memberList[row];
    std::string szContent = "<?xml version=\"1.0\"?><coolview command=\"KickOut\"><uri>" + mlist->MemberURI + "</uri></coolview>";
    char* body = new char[szContent.length() + 1];
    strcpy(body, szContent.c_str());
    //????Info???
    _activePhoneLine->makeInfoCall(callId, body);
    return 0;
}

void UserProfile::makeInfoCall(int callId, char* szContent)
{
    _activePhoneLine->makeInfoCall(callId, szContent);
}

//??????????
void UserProfile::sendTextMsg(const int& receiverIndex, const std::string& szContent)
{
    if(_confState == IsInConference)
    {
        //???????????
        if(receiverIndex == 0)
        {
            vector<MemberInfo*>::iterator it = _memberList.begin();
            //???????
            it++;
            for(it; it != _memberList.end(); it++)
            {
                MemberInfo* mlist = (MemberInfo*)(*it);
                //??????????????????壬???????????Message???
                _activePhoneLine->startConference("sip:" + mlist->MemberURI, szContent);
            }
        }
        else
            _activePhoneLine->startConference("sip:" + _memberList[receiverIndex]->MemberURI, szContent);
    }
}

//???????
int UserProfile::handForSpeak(int row, bool handUpOrDown, int callId)
{
    std::string command;
    std::string memberUri;
    if(handUpOrDown)
        command = "HandUp";
    else
        command = "HandDown";
    if(row == -1)
        memberUri = "all";
    else
    {
        MemberInfo* mlist = (MemberInfo*)_memberList[row];
        memberUri = mlist->MemberURI;
    }
    std::string szContent = "<?xml version=\"1.0\"?><coolview command=\"" + command + "\"><uri>" + memberUri + "</uri></coolview>";

    _activePhoneLine->makeInfoCall(callId, (char*)szContent.c_str());
    return 0;
}




string UserProfile::getFirstMember(bool permitOrForbid)
{
    MemberInfo* member;
    if(permitOrForbid)
    {
        //??????????????????????????????????????
        if(_requestForSpeakList.size() == 0)
            return "";
        member = _requestForSpeakList.front();
        _speakingList.push_back(member);
        _requestForSpeakList.pop_front();
    }
    else
    {
        //???????????????????????
        if(_speakingList.size() == 0)
            return "";
        member = _speakingList.front();
        _speakingList.pop_front();
    }
    return member->MemberURI;
}

/**
* Add by: hexianfu
* for the telecontroller
get the latest confinfo and memberinfo
* 2009-10-29
*/
vector<ConfInfo*>& UserProfile::getCurrentConfInfoList()
{
    return _confList;
}

vector<MemberInfo*>& UserProfile::getCurrentMemberInfoList()
{
    return  _memberList;
}


//控制音视频的起停...zhenHua.sun 2010-08-06
/**
* @brief 控制音频的发送
* @param state 如果是true，则启动音频的发送，否则停止音频的发送
*/
void UserProfile::controlAudioSend(bool state)
{
    _activePhoneLine->controlAudioSend(state);
}
/**
* @brief 控制视频的发送
* @param state 如果是true，则启动视频的发送，否则停止视频的发送
*/
void UserProfile::controlVideoSend(bool state)
{
    _activePhoneLine->controlVideoSend(state);
}

void UserProfile::rtpStatRecvEventHandler(const RtpStatItem& rtpstat)
{
    sendRTPStatToQoSServer(rtpstat);
    //引发事件，传给控制层或界面层
    rtpStatRecvEvent(*this, rtpstat);
}

/*                     向QOS服务器发送参数 -- add by qhf                         */

void UserProfile::sendRTPStatToQoSServer(const RtpStatItem& rtpstat)
{
    Config& config = ConfigManager::getInstance().getCurrentConfig();

    char buffer[60];
    //memset(buffer, 0, 60);
    int bandwidth = 0;

    string szContent = "<?xml version=\"1.0\"?><coolview command=\"qos\" type=\"clientQosReport\">";
    //string reporterUri = config.getProfileLastUsedName();
    string reporterUri = getCurrentUserSipUri();

    string reporterIP = config.getIPAddress();
    string senderUri = "";
    string senderIP = "";
    string receiverUri = "";
    string receiverIP = "";
    if(rtpstat.rtp_endpoint_type == eET_Sender)  //发送者报告
    {
        senderUri += reporterUri;
        senderIP += reporterIP;
        bandwidth = rtpstat.send_bandwidth;
        receiverIP += rtpstat.rem_addr;
        ReceiverInfo * recvInfo = receiver_find_by_ip(_sendIPList, rtpstat.rem_addr);
        if (recvInfo)
        {
            receiverUri += recvInfo->MemberURI;
        }
        szContent.append("<endpointType>sender</endpointType>");
    }
    else    //接收者报告
    {
        receiverUri += reporterUri;
        receiverIP += reporterIP;
        senderIP += rtpstat.rem_addr;
        //欠缺senderUri信息的获取
        bandwidth = rtpstat.recv_bandwidth;
        szContent.append("<endpointType>receiver</endpointType>");
    }
    //序列号
    itoa(rtpstat.seqnumber, buffer, 10);
    szContent.append("<seq>").append(buffer).append("</seq>");
    szContent.append("<messageSenderUri>").append(reporterUri).append("</messageSenderUri>");
    szContent.append("<FocusURI>").append(_focusURI).append("</FocusURI>");
    szContent.append("<senderIP>").append(senderIP).append("</senderIP>");
    szContent.append("<senderUri>").append(senderUri).append("</senderUri>");
    szContent.append("<receiverIP>").append(receiverIP).append("</receiverIP>");
    szContent.append("<receiverUri>").append(receiverUri).append("</receiverUri>");

    string flowClass;
    if(rtpstat.media_type == eMT_Video)
        flowClass = "video";
    else if(rtpstat.media_type == eMT_Audio)
        flowClass = "audio";
    else
        flowClass = "app";

    szContent.append("<flowClass>").append(flowClass).append("</flowClass>");

    itoa(bandwidth, buffer, 10);
    szContent.append("<bandwidth>").append(buffer).append("</bandwidth>");

    itoa(rtpstat.lost, buffer, 10);
    szContent.append("<loseRate>").append(buffer).append("</loseRate>");

    itoa(rtpstat.delay, buffer, 10);
    szContent.append("<delay>").append(buffer).append("</delay>");

    itoa(rtpstat.jitter, buffer, 10);
    szContent.append("<jitter>").append(buffer).append("</jitter>");

    _i64toa(rtpstat.timestamp, buffer, 10);	   
	szContent.append("<timestamp>").append(buffer).append("</timestamp>");

    itoa(rtpstat.interval, buffer, 10);
    szContent.append("<interval>").append(buffer).append("</interval>");

    szContent += "</coolview>";

    string qosuri = config.getSimulatorServerSipUri();
    if(qosuri == "" || qosuri.length() == 0)
    {
        qosuri = QOS_SERVER_SIP_URI;
        config.set(Config::QOS_SERVER_SIP_URI_KEY, qosuri);
    }
    if(_activePhoneLine)
        _activePhoneLine->sendQosPara(qosuri, szContent);
}


//end

void UserProfile::sendQosJoinConfReport(const string& curFocusURI)
{
    //先设置一个默认产生值
    string minRate = "100";
    string maxRate = "5200";

    Config& config = ConfigManager::getInstance().getCurrentConfig();
    //string reporterUri = config.getProfileLastUsedName();
    string reporterUri = getCurrentUserSipUri();
    string reporterIP = config.getIPAddress();
	char  strGateway[60];
	memset(strGateway,0,60);
	getGatewayByHostIP(reporterIP.c_str(),strGateway);
	string reporterGateway(strGateway);
	
    string szContent = "<?xml version=\"1.0\"?><coolview command=\"qos\" type=\"Login\">";
    szContent += ("<FocusURI>" + curFocusURI + "</FocusURI>");
    szContent += ("<uri>" + reporterUri + "</uri>");
    szContent += ("<minRate>" + minRate + "</minRate>");
    szContent += ("<maxRate>" + maxRate + "</maxRate>");
    szContent += ("<ip>" + reporterIP + "</ip>");
    szContent += ("<gateway>" + reporterGateway + "</gateway>");
    szContent += "</coolview>";

    string qosuri = config.getSimulatorServerSipUri();
    if(qosuri == "" || qosuri.length() == 0)
    {
        qosuri = QOS_SIMULATOR_SIP_URI;
        config.set(Config::QOS_SIMULATOR_SIP_URI_KEY, qosuri);
    }
    if(_activePhoneLine)
        _activePhoneLine->sendQosPara(qosuri, szContent);
}



void UserProfile::sendQosExitConfReport(const string& curFocusURI)
{
    Config& config = ConfigManager::getInstance().getCurrentConfig();
    //string reporterUri = config.getProfileLastUsedName();
    string reporterUri = getCurrentUserSipUri();
    string reporterIP = config.getIPAddress();
	char  strGateway[60];
	memset(strGateway,0,60);
	getGatewayByHostIP(reporterIP.c_str(),strGateway);
	string reporterGateway(strGateway);

    string szContent = "<?xml version=\"1.0\"?><coolview command=\"qos\" type=\"LogoutNotify\">";
    szContent += ("<FocusURI>" + curFocusURI + "</FocusURI>");
    szContent += ("<uri>" + reporterUri + "</uri>");
    szContent += ("<ip>" + reporterIP + "</ip>");
    szContent += ("<gateway>" + reporterGateway + "</gateway>");
    szContent += "</coolview>";

    string qosuri = config.getSimulatorServerSipUri();
    if(qosuri == "" || qosuri.length() == 0)
    {
        qosuri = QOS_SIMULATOR_SIP_URI;
        config.set(Config::QOS_SIMULATOR_SIP_URI_KEY, qosuri);
    }
    if(_activePhoneLine)
        _activePhoneLine->sendQosPara(qosuri, szContent);
}

void UserProfile::setSendDSCP(int dscp)
{
    if(_activePhoneLine)
        _activePhoneLine->setDscp(dscp);
}

string UserProfile::getCurrentUserSipUri()
{
    if(!_sipAccount)
        return "";
    else
        return _sipAccount->getIdentity() + std::string("@") + _sipAccount->getRealm();
}

//获取当前登录用户的用户名（SIP URI），从配置文件中读取，如果配置文件错误，则会读出空用户名
string UserProfile::getCurrentUserName()
{
    return ConfigManager::getInstance().getCurrentConfig().getProfileLastUsedName();
}

//获取当前用户的成员信息
MemberInfo* UserProfile::getCurrentUserMemberInfo()
{
    return member_find_by_uri(_memberList, getCurrentUserSipUri());
}

void UserProfile::tranStatRecvEventHandler(const TranStat& transtat)
{
    //处理

    //再往上层抛
    tranStatRecvEvent(transtat);
}

string UserProfile::createJoinConferenceMsg(const string& confUri)
{
    string cid = "";

    //根据uri查找会议的cid
    ConfInfo* conf = conf_find_by_uri(_confList, confUri);
    if(conf)
        cid = conf->CID;

    Config& config = ConfigManager::getInstance().getCurrentConfig();
    string reporterUri = getCurrentUserSipUri();
    string reporterIP = config.getIPAddress();
	char  strGateway[60];
	memset(strGateway,0,60);
	getGatewayByHostIP(reporterIP.c_str(),strGateway);
	string reporterGateway(strGateway);

    //先设置一个默认的带宽限值
    string minRate = "50";
    string maxRate = "10000";

    string szContent = "<?xml version=\"1.0\"?><coolview command=\"qos\" type=\"Login\">";
    szContent += ("<cid>" + cid + "</cid>");
    szContent += ("<focusuri>" + confUri + "</focusuri>");
    szContent += ("<uri>" + reporterUri + "</uri>");
    szContent += ("<minRate>" + minRate + "</minRate>");
    szContent += ("<maxRate>" + maxRate + "</maxRate>");
    szContent += ("<ip>" + reporterIP + "</ip>");
    szContent += ("<gateway>" + reporterGateway + "</gateway>");
    szContent += "</coolview>";
    return szContent;
}

//发送一条进入会议是否成功的消息给控制服务器
void UserProfile::sendLoginAckMsg(bool isSuccess)
{
    if(!_activePhoneLine)
        return;
    string sdp = "v=0";
    string loginResult = isSuccess ? "success" : "failure";
    string szContent = "<?xml version=\"1.0\"?><coolview command=\"qos\" type=\"LoginAck\">";
    szContent += ("<result>" + loginResult + "</result>");
    szContent += ("<sdp>" + sdp + "</sdp>");
    szContent += "</coolview>";

    Config& config = ConfigManager::getInstance().getCurrentConfig();
    std::string serverUri = config.getGlobalService();
    _activePhoneLine->sendSipMessage(serverUri, szContent);
}

void UserProfile::cvMessageReceivedEventHandler(const std::string& message, const std::string& fromUri)
{
    CVMsgParser cvMessageParser;
    cvMessageParser.InitParser(message.c_str());
    const char* command = cvMessageParser.GetCommandType();
    //获得会议列表
    if(strcmp(command, "ConfList") == 0)
    {
        recvConfListMsgHandler(cvMessageParser, fromUri);
    }
    //登入会议
    else if(strcmp(command, "LoginResult") == 0)
    {
        recvLoginResultCommandMsgHandler(cvMessageParser, fromUri);
    }
    //收到MemberList的Info消息
    else if(strcmp(command, "MemberList") == 0)
    {
        recvMemberListCommandMsgHandler(cvMessageParser, fromUri);
    }
    //有人加入会议
    else if(strcmp(command, "LoginNotify") == 0 || strcmp(command, "loginnotify") == 0)
    {
        recvLoginNotifyMsgHandler(cvMessageParser, fromUri);
    }
    //有人退出会议
    else if(strcmp(command, "LogoutNotify") == 0 || strcmp(command, "logoutnotify") == 0)
    {
        recvLogoutNotifyMsgHandler(cvMessageParser, fromUri);
    }
    //受到邀请
    else if(strcmp(command, "Invite") == 0)
    {
        recvInviteMsgHandler(cvMessageParser, fromUri);
    }
    //申请即时会议成功
    else if(strcmp(command, "ApplyConfResult") == 0)
    {
        recvApplyConfResultMsgHandler(cvMessageParser, fromUri);
    }
    //密码不正确
    else if(strcmp(command, "PasswordIncorrect") == 0)
    {
        recvPasswordIncorrectMsgHandler(cvMessageParser, fromUri);
    }
    //指定发言人
    else if(strcmp(command, "SetSpeaker") == 0)
    {
        recvSetSpeakerMsgHandler(cvMessageParser, fromUri);
    }
    //取消发言人
    else if(strcmp(command, "ForbidSpeaker") == 0)
    {
        recvForbidSpeakerMsgHandler(cvMessageParser, fromUri);
    }
    //被踢出会议
    else if(strcmp(command, "Kicked") == 0)
    {
        recvKickedMsgHandler(cvMessageParser, fromUri);
    }
    //收到文本消息
    else if(strcmp(command, "Text") == 0)
    {
        recvTextMsgHandler(cvMessageParser, fromUri);
    }
    //收到白板消息
    else if(strcmp(command, "WhiteBoard") == 0)
    {
        recvWhiteBoardMsgHandler(cvMessageParser, fromUri);
    }
    //举手发言
    else if(strcmp(command, "HandUp") == 0)
    {
        recvHandUpMsgHandler(cvMessageParser, fromUri);
    }
    //取消举手
    else if(strcmp(command, "HandDown") == 0)
    {
        recvHandDownMsgHandler(cvMessageParser, fromUri);
    }
    //发送一路单播媒体流
    else if(strcmp(command, "Receiver") == 0)
    {
        recvReceiverMsgHandler(cvMessageParser, fromUri);
    }
    //结束发送一路单播媒体流
    else if(strcmp(command, "Cancel2Receiver") == 0)
    {
        recvCancel2ReceiverMsgHandler(cvMessageParser, fromUri);
    }
    //发送一路屏幕共享流
    else if(strcmp(command, "ScreenReceiver") == 0)
    {
        recvScreenReceiverMsgHandler(cvMessageParser, fromUri);
    }
    //结束发送一路屏幕共享流
    else if(strcmp(command, "ScreenCancel2Receiver") == 0)
    {
        recvScreenCancel2ReceiverMsgHandler(cvMessageParser, fromUri);
    }
    else if(strcmp(command, "UnShareScreen") == 0)
    {
        recvUnShareScreenMsgHandler(cvMessageParser, fromUri);
    }
    //qos相关命令
    else if(strcmp(command, "qos") == 0)
    {
        recvQoSMsgHandler(cvMessageParser, fromUri);
    }
	else if(strcmp(command, "qostest") == 0)	//qostest命令
	{
		recvQoSTestMsgHandler(cvMessageParser, fromUri);
	}
}


//收到ConfList命令SIP消息， 获取会议列表
void UserProfile::recvConfListMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri)
{
    if(_confList.size() > 0)
        CVMsgParser::ReleaseConfList(_confList);
    cvMessageParser.ParseConfListMsg(_confList);
    //更新当前会议列表
    _currentConfInfo = conf_find_by_uri(_confList, _focusURI);
    cvConfListReceivedEvent(*this, _confList);
}

//接收到LoginResult命令SIP消息，登入会议
void UserProfile::recvLoginResultCommandMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri)
{
    // joinConference(_focusURI);
}

//接收到MemberList命令SIP消息，更新成员列表
void UserProfile::recvMemberListCommandMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri)
{
    if(!checkIsInConference())
        return;
    cvMessageParser.ParseMemberListMsg(_memberList, _hostURI, _confMode);
    cvMemberListReceivedEvent(*this, _memberList, _confTitle);
}

//接收到LoginNotify命令SIP消息，有人加入会议
void UserProfile::recvLoginNotifyMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri)
{
    if(!checkIsInConference())
        return;
    cvMessageParser.ParseMemberListByNotifyMsg(_memberList, _hostURI, _confMode);
    cvMemberListReceivedEvent(*this, _memberList, _confTitle);
}

//接收到LogoutNotify命令SIP消息，有人退出会议
void UserProfile::recvLogoutNotifyMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri)
{
    if(!checkIsInConference())
        return;

    string logoutUserUri = cvMessageParser.GetMemberUri();

	//单播会议下，如果有人退出检查是否有发向对方的媒体流，有则停止发送
	if(!checkCurrentIsMulticastConference())
	{
		//停止音视频发送
		ReceiverInfo* receiver = receiver_find_by_uri(_sendIPList, logoutUserUri);
		if (receiver)
		{
			_activePhoneLine->media_send_stop((char*)receiver->recvIP.c_str(), receiver->audioPort, (char*)receiver->recvIP.c_str(), receiver->videoPort);
			//删除发送记录
			receiver_remove_by_uri(_sendIPList, logoutUserUri);
		}
		//停止发送屏幕共享数据
		ReceiverInfo* screen_receiver = receiver_find_by_uri(_screenShareSendIPList, logoutUserUri);
		if (screen_receiver)
		{
			_activePhoneLine->screen_share_send_stop((char*)screen_receiver->recvIP.c_str(), DEFAULT_SCREEN_SHARE_PORT);
			//删除发送记录
			receiver_remove_by_uri(_screenShareSendIPList, logoutUserUri);
		}
	}
    //更新成员列表
    cvMessageParser.ParseMemberListByNotifyMsg(_memberList, _hostURI, _confMode);
    cvMemberListReceivedEvent(*this, _memberList, _confTitle);

    string logoutUserId = logoutUserUri;
    int index = logoutUserUri.find('@');
    if(index != std::string::npos)
    {
        logoutUserId = logoutUserUri.substr(0, index);
    }

    //由于刷新了成员列表，导致media_recv_stop并没有真正销毁对应的recvGraph
    //因此应该直接调用phoneline对recvGraph进行销毁
    //zhenHua.sun 2010-09-02
    _activePhoneLine->media_recv_stop(logoutUserId);
    logoutReceivedEvent(*this, logoutUserId);
 
}


//接收到Invite命令SIP消息，受到邀请
void UserProfile::recvInviteMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri)
{
    ConfInfo* conf = conf_find_by_uri(_confList, fromUri);
    if(conf)
    {
        string inviter = cvMessageParser.GetInviterUri();
        cvConfrenceInviteEvent(*this, conf, inviter);
    }
}

//接收到ApplyConfResult命令SIP消息，申请即时会议成功
void UserProfile::recvApplyConfResultMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri)
{
    string uri = cvMessageParser.GetFocusURI();
    if(uri != "")
    {
        makeJoinConfInvite(uri);
    }
}

//接收到PasswordIncorrect命令SIP消息，密码不正确
void UserProfile::recvPasswordIncorrectMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri)
{
    //_focusURI = "";
    cvPasswordIncorrectEvent(*this);
}

//接收到SetSpeaker命令SIP消息，指定发言人
void UserProfile::recvSetSpeakerMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri)
{
    string userUri = cvMessageParser.GetMemberUri();
	   MemberInfo* member =  member_find_by_uri(_memberList,userUri);
	   if (member)
		   member->permitSpeak =1;	
    cvMemberListReceivedEvent(*this, _memberList, _confTitle);
    if(userUri == getCurrentUserName())
    {
        permitSpeak();
        cvPermitSpeakEvent(*this);
    }
}

//接收到ForbidSpeaker命令SIP消息，取消发言人
void UserProfile::recvForbidSpeakerMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri)
{
    string userUri = cvMessageParser.GetMemberUri();
    vector<MemberInfo*>::iterator it = _memberList.begin();
    //取消主持人以外的所有人发言
    if(userUri == "all")
    {
        for(it; it != _memberList.end(); it++)
        {
            MemberInfo* member = (MemberInfo*)(*it);
            if(_hostURI != member->MemberURI)
            {
                member->permitSpeak = 0;               
            }
        }
    }
    //取消指定的发言人
    else
    {
		MemberInfo* member =  member_find_by_uri(_memberList,userUri);
		if (member)
			member->permitSpeak =0;	    
    }
    cvMemberListReceivedEvent(*this, _memberList, _confTitle);
    if(userUri == getCurrentUserName() || (userUri == "all" && !hostOrNot()))
    {
        forbidSpeak();
    }
}


//接收到Text命令SIP消息，收到文本消息
void UserProfile::recvTextMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri)
{
    string userId = fromUri;
    int index = userId.find('@');
    if(index != std::string::npos)
    {
        userId = userId.substr(0, index);
    }
    string msg = cvMessageParser.GetTextMessage();
    cvTextMsgReceivedEvent(*this, msg, userId);
}


//接收到LoginResult命令SIP消息，被踢出会议
void UserProfile::recvKickedMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri)
{
    cvKickedEvent(*this);
}

//接收到WhiteBoard命令SIP消息，收到白板消息
void UserProfile::recvWhiteBoardMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri)
{
    string userId = fromUri;
    int index = userId.find('@');
    if(index != std::string::npos)
    {
        userId = userId.substr(0, index);
    }
    string msg = cvMessageParser.GetWhiteBoardMessage();
    cvWhiteBoardMsgReceivedEvent(*this, msg, userId);
}

//接收到HandUp命令SIP消息，举手发言
void UserProfile::recvHandUpMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri)
{
    string userUri = cvMessageParser.GetMemberUri();
	MemberInfo* member =  member_find_by_uri(_memberList,userUri);
	if (member)
	{
		member->hand = 1;	
		//加入请求发言队列
		_requestForSpeakList.push_back(member);
	}
    cvMemberListReceivedEvent(*this, _memberList, _confTitle);
}

//接收到HandDown命令SIP消息，取消举手
void UserProfile::recvHandDownMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri)
{
    string userUri = cvMessageParser.GetMemberUri();
	MemberInfo* member =  member_find_by_uri(_memberList,userUri);
	if (member)
	{
		member->hand = 0;	
		//从请求发言队列中删除
		list<MemberInfo*>::iterator it = find(_requestForSpeakList.begin(), _requestForSpeakList.end(), member);
		if(it != _requestForSpeakList.end())
			_requestForSpeakList.erase(it);		
	}  
    cvMemberListReceivedEvent(*this, _memberList, _confTitle);
}

/************************************************************************/
/* 重要函数																						 */
/************************************************************************/

//接收到Receiver命令SIP消息，发送媒体流
void UserProfile::recvReceiverMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri)
{
    //获取对方地址，单播/组播IP地址
    string recvIP = cvMessageParser.GetReceiverIP();
    string remoteIPAddr;					//远程地址其实可以直接使用recvIP
    int videoPort = 0, audioPort = 0;

    //判断是否组播会议 , 单播会议目标端口是对方的，组播会议目标端口是自己的
    if(checkCurrentIsMulticastConference())
    {
        // 组播会议使用组播地址、发送到自己的端口
        getCurrentMulticastAddress(remoteIPAddr);		//此时remoteIPAddr==recvIP 		
    }
    else
    {
        // 单播会议使用对方IP地址、发送到自己的端口    
		 remoteIPAddr = recvIP;
		////从成员列表中查找信息	
	 // MemberInfo* remote_member = member_find_by_uri(_memberList, fromUri);
  //      if(!remote_member)
		//	 return;    
  //      videoPort = remote_member->videoPort;
  //      audioPort = remote_member->audioPort;
    }
	//发送到自己的端口    
	MemberInfo * my_member = getCurrentUserMemberInfo();
	if (!my_member)
		return;	
	videoPort = my_member->videoPort;
	audioPort = my_member->audioPort;

    char* remoteIP = (char*)remoteIPAddr.c_str();
    string currentUser = getCurrentUserName();
    if(_confMode == "host" && currentUser != _hostURI && _memberList[0]->permitSpeak == 0)
    {
        media_send_start(NULL, 0, remoteIP, videoPort, _previewHwnd);
    }
    else
        media_send_start(remoteIP, audioPort, remoteIP, videoPort, _previewHwnd);
    //记录对方信息
    ReceiverInfo receiverAddr;
    receiverAddr.MemberURI = fromUri;
    receiverAddr.recvIP = remoteIP;
    receiverAddr.videoPort = videoPort;
    receiverAddr.audioPort = audioPort;
    _sendIPList.push_back(receiverAddr);
}

//接收到ScreenReceiver命令SIP消息，结束媒体流
void UserProfile::recvScreenReceiverMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri)
{
    if(_isShareScreen)
    {
        string recvIP = cvMessageParser.GetReceiverIP();
        _activePhoneLine->screen_share_send_start((char*)recvIP.c_str(), DEFAULT_SCREEN_SHARE_PORT, _shareRect.left, _shareRect.top, _shareRect.right - _shareRect.left, _shareRect.bottom - _shareRect.top);
        ReceiverInfo receiverAddr;
        receiverAddr.MemberURI = fromUri;
        receiverAddr.recvIP = recvIP;
        receiverAddr.videoPort = 0;
        receiverAddr.audioPort = 0;
        _screenShareSendIPList.push_back(receiverAddr);
    }
    else
    {
        string currentUser = getCurrentUserName();
        if(_confMode == "host" && currentUser == _hostURI)
        {
            //告诉发送方发一路到本机地址
            string szContent = "<?xml version=\"1.0\"?><coolview command='UnShareScreen'></coolview>";
            //此函数名字无特殊意义，只是用来发送Message消息
            _activePhoneLine->startConference("sip:" + fromUri, szContent);
        }
    }
}


//接收到LoginResult命令SIP消息，发送一路屏幕共享流
void UserProfile::recvUnShareScreenMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri)
{
    cvHostUnShareScreenEvent(*this);
}

//接收到Cancel2Receiver命令SIP消息，结束发送一路屏幕共享流
void UserProfile::recvCancel2ReceiverMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri)
{
    //组播会议不停止发送
    if(checkCurrentIsMulticastConference())
        return;
    string recvIP = cvMessageParser.GetReceiverIP();
    ReceiverInfo * receiver  = receiver_find_by_ip(_sendIPList, recvIP);
    if (receiver)
    {
		//_activePhoneLine->media_send_stop((char*)recvIP.c_str(), receiver->audioPort, (char*)recvIP.c_str(), receiver->videoPort);
		_activePhoneLine->media_send_stop((char*)receiver->recvIP.c_str(), receiver->audioPort, (char*)receiver->recvIP.c_str(), receiver->videoPort);
	 //删除发送信息
        receiver_remove_by_ip(_sendIPList, recvIP);
    }
}

//接收到ScreenCancel2Receiver命令SIP消息，取消屏幕共享
void UserProfile::recvScreenCancel2ReceiverMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri)
{
	//组播会议不停止发送
	if(checkCurrentIsMulticastConference())
		return;
	string recvIP = cvMessageParser.GetReceiverIP();
	ReceiverInfo * receiver  = receiver_find_by_ip(_screenShareSendIPList, recvIP);
	if (receiver)
	{
		_activePhoneLine->screen_share_send_stop((char*)recvIP.c_str(), DEFAULT_SCREEN_SHARE_PORT);
		//删除发送信息
		receiver_remove_by_ip(_screenShareSendIPList, recvIP);
	}
}


//接收到QoS命令SIP消息，qos相关命令
void UserProfile::recvQoSMsgHandler(CVMsgParser& cvMessageParser, const std::string& fromUri)
{
    Config& config = ConfigManager::getInstance().getCurrentConfig();
	string strType = cvMessageParser.GetQosType();
    const char* type = strType.c_str();
    if(strcmp(type, "LoginResult") == 0)    //申请加入会议结果
    {
        //sucess表示允许，正在返回LoginPermission消息
        //failure表示失败，正在与qos服务器协商
    }
    else if(strcmp(type, "LoginPermission") == 0)   //qos允许进入会议
    {
        MsgLoginPermissionInfo info;
        cvMessageParser.ParseLoginPermissionMsg(info);

        //开始进入当前会议
        joinConference(_focusURI);

		//设置DSCP值
		setSendDSCP(info.dscp);
    }
    else if(strcmp(type, "LoginRejection") == 0)    //qos不允许进入会议
    {
        MsgLoginRejectionInfo info;
        cvMessageParser.ParseLoginRejectionMsg(info);
        //不允许进入当前会议
        rejectToJoinConference(_focusURI, info.description);
    }
    else if(strcmp(type, "clientQosControl") == 0)
    {
        //控制码率
    }
}

//加入一个新会议，进行退出当前会议、清空成员列表、启动媒体传输等操作
int UserProfile::joinConference(const string& curFocusURI)
{
	//控制服务器允许进入会议，开始进入一个新会议

	//检查当前用户是否已经进入了一个会议，则不处理
	if(checkIsInConference() && _memberList.size()>0 && _sendIPList.size()>0)
	{
		return	-1;
	}	
	//根据记录的会议URI找到指定会议信息，并进入
	ConfInfo* conf = conf_find_by_uri(_confList, curFocusURI);
	if(!conf)
		return -1;

	_focusURI = curFocusURI;
	_confTitle = conf->Title;
	_confMode = conf->ControlMode;
	_hostURI = conf->HostURI;
	_currentConfInfo = conf;

	//释放成员信息
	if(_memberList.size() > 0)
		CVMsgParser::ReleaseMemberList(_memberList);
	//引发成员列表更新事件
	cvMemberListReceivedEvent(*this, _memberList, _confTitle);

	string currentUser = getCurrentUserName();
	//启动媒体传输，但不发送数据
	if(_confMode == "host" && currentUser != _hostURI)
		media_send_start(NULL, 0, DEFAULT_IP_ADDRESS, DEFAULT_VIDEO_PORT, _previewHwnd);
	else
		media_send_start(DEFAULT_IP_ADDRESS, DEFAULT_AUDIO_PORT, DEFAULT_IP_ADDRESS, DEFAULT_VIDEO_PORT, _previewHwnd);

	//改变状态
	_confState = IsInConference;

	//For Testing:向Qos服务器报告登陆信息，发送到模拟器
	//sendQosJoinConfReport(curFocusURI);

	return 0;
}

//拒绝加入会议，比如控制服务器不允许
int UserProfile::rejectToJoinConference(const string& curFocusURI, const string& reason)
{
	if (_confState!= WaitingPermission)
	{
		//没有申请加入，不处理拒绝消息
		return -1;	
	}
	//处理

	//通知上层被拒绝加入会议
	cvJoinRejectionEvent(*this,curFocusURI,reason);

	//改变状态
	_confState = JoinRejected;
    return 0;
}

//判断当前会议是否组播会议
bool UserProfile::checkCurrentIsMulticastConference()
{
    if(!checkIsInConference())
        return false;
    ConfInfo* conf = NULL;
    if(_currentConfInfo)
        conf = _currentConfInfo;
    else
        conf = conf_find_by_uri(_confList, _focusURI);
    //从当前会议信息判断
    if(conf && check_support_multicast(conf->multicastAddress))
        return true;
    //从成员列表的信息中判断
    int memberCount = _memberList.size();
    int count = 0;
    for(vector<MemberInfo*>::iterator iter = _memberList.begin(); iter != _memberList.end(); ++iter)
    {
        if(check_support_multicast((*iter)->multicastAddress))
        {
            count ++;
            //超半数地址则判断是组播会议
            if(count > memberCount * 0.5)
                return true;
        }
    }
    return false;
}


//获取当前会议的组播地址，如果会议是组播会议，则输出其IP地址、端口等，返回ture，否则返回false
bool UserProfile::getCurrentMulticastAddress(string& ipaddress)
{
    if(!checkIsInConference())
        return false;
    ConfInfo* conf = NULL;
    if(_currentConfInfo)
        conf = _currentConfInfo;
    else
        conf = conf_find_by_uri(_confList, _focusURI);
    //从当前会议信息判断
    if(conf && check_support_multicast(conf->multicastAddress) )
    {
        ipaddress = conf->multicastAddress;          
        return true;
    }
    //从成员列表的信息中判断
    int memberCount = _memberList.size();
    for(vector<MemberInfo*>::iterator iter = _memberList.begin(); iter != _memberList.end(); ++iter)
    {
        MemberInfo* member = *iter;
        if(check_support_multicast(member->multicastAddress))
        {
            ipaddress = member->multicastAddress;         
            return true;
        }
    }
    return false;
}

void UserProfile::recvQoSTestMsgHandler( CVMsgParser& cvMessageParser, const std::string& fromUri )
{
	string strType = cvMessageParser.GetQosTestType();
	const char* type = strType.c_str();
	if(strcmp(type, "SimulateTestBeginSend") == 0)    //SimulateTestBeginSend
	{
		QoSTestBeginSendInfo info;
		cvMessageParser.ParseQoSTestBeginSendInfo(info);
		RtpwareProxy::getInstance().startNewSimluteSender(info.sessionId, info.remoteIP.c_str(), info.remotePort, info.sendRate, info.dscp,info.duration);

	}
	else if(strcmp(type, "SimulateTestBeginReceive") == 0)   //qos允许进入会议
	{
		QoSTestBeginReceiveInfo info;
		cvMessageParser.ParseQoSTestBeginReceiveInfo(info);
		RtpwareProxy::getInstance().startNewSimluteReceiver(info.sessionId, info.sourceIP.c_str(),info.localPort,info.duration);
	}
}
