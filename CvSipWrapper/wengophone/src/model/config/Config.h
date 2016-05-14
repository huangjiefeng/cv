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

#ifndef OWCONFIG_H
#define OWCONFIG_H

#include <settings/CascadingSettings.h>

#include <model/wenbox/EnumWenboxStatus.h>

#include <util/String.h>
#include <util/StringList.h>

/**
 * Stores the configuration options of WengoPhone.
 *
 * Tries to make it impossible to make a mistake.
 *
 * @see CascadingSettings
 * @ingroup model
 * @author Tanguy Krotoff
 * @author Philippe Bernery
 */
class Config : public CascadingSettings {
public:

	static const std::string CONFIG_VERSION_KEY;
	int getConfigVersion() const;

	Config(const std::string & name);

	~Config();

	/**
	 * Gets the name associated with the Config.
	 *
	 * @return Config name
	 */
	std::string getName() const;

	/** True if SSL connection to SSO is available. */
	static const std::string NETWORK_SSO_SSL_KEY;
	bool getNetworkSSOSSL() const;

	/**
	 * Type of NAT.
	 *
	 * @see EnumNatType::NatType
	 */
	static const std::string NETWORK_NAT_TYPE_KEY;
	std::string getNetworkNatType() const;

	/** Client SIP Port to use. */
	static const std::string NETWORK_SIP_LOCAL_PORT_KEY;
	int getNetworkSipLocalPort() const;

	/** True if tunnel is SSL. */
	static const std::string NETWORK_TUNNEL_SSL_KEY;
	bool getNetworkTunnelSSL() const;

	/** Tunnel server. Empty if no tunnel is needed */
	static const std::string NETWORK_TUNNEL_SERVER_KEY;
	std::string getNetworkTunnelServer() const;

	/** True if HTTP tunnel needed. */
	static const std::string NETWORK_TUNNEL_NEEDED_KEY;
	bool getNetWorkTunnelNeeded() const;

	/** Tunnel server port. */
	static const std::string NETWORK_TUNNEL_PORT_KEY;
	int getNetworkTunnelPort() const;

	/** Is proxy detected ?. */
	static const std::string NETWORK_PROXY_DETECTED_KEY;
	bool getNetworkProxyDetected() const;

	/** Proxy server. Empty if no proxy */
	static const std::string NETWORK_PROXY_SERVER_KEY;
	std::string getNetworkProxyServer() const;

	/** Proxy server port. */
	static const std::string NETWORK_PROXY_PORT_KEY;
	int getNetworkProxyPort() const;

	/** Proxy login. Empty if not needed */
	static const std::string NETWORK_PROXY_LOGIN_KEY;
	std::string getNetworkProxyLogin() const;

	/** Proxy Password. Empty if not needed */
	static const std::string NETWORK_PROXY_PASSWORD_KEY;
	std::string getNetworkProxyPassword() const;

	/** Stun server. */
	static const std::string NETWORK_STUN_SERVER_KEY;
	std::string getNetworkStunServer() const;

	/**
	 * @name PhApi configuration Methods
	 * @{
	 */

	/** Path to codecs. */
	static const std::string CODEC_PLUGIN_PATH_KEY;
	std::string getCodecPluginPath() const;

	/** Path to PhApi plugins (e.g SFP plugin). */
	static const std::string PHAPI_PLUGIN_PATH_KEY;
	std::string getPhApiPluginPath() const;

	/**
	 * @}
	 */
 
	/**
	 * @name Audio Methods
	 * @{
	 */

	/** audio codec list */
	static const std::string AUDIO_CODEC_LIST_KEY;
	StringList getAudioCodecList() const;

	/** Playback audio device name. */
	static const std::string AUDIO_OUTPUT_DEVICEID_KEY;
	StringList getAudioOutputDeviceId() const;

	/** Record audio device name. */
	static const std::string AUDIO_INPUT_DEVICEID_KEY;
	StringList getAudioInputDeviceId() const;

	/** Ringer audio device name. */
	static const std::string AUDIO_RINGER_DEVICEID_KEY;
	StringList getAudioRingerDeviceId() const;

	/** Ringing/incoming call/ringtone audio file name. */
	static const std::string AUDIO_INCOMINGCALL_FILE_KEY;
	std::string getAudioIncomingCallFile() const;

	/** Ringing/double call/ringtone audio file name. */
	static const std::string AUDIO_DOUBLECALL_FILE_KEY;
	std::string getAudioDoubleCallFile() const;

	/** Call closed audio file name. */
	static const std::string AUDIO_CALLCLOSED_FILE_KEY;
	std::string getAudioCallClosedFile() const;

	/** Incoming chat audio file name. */
	static const std::string AUDIO_INCOMINGCHAT_FILE_KEY;
	std::string getAudioIncomingChatFile() const;

	/** IM account connected audio file name. */
	static const std::string AUDIO_IMACCOUNTCONNECTED_FILE_KEY;
	std::string getAudioIMAccountConnectedFile() const;

	/** IM account disconnected audio file name. */
	static const std::string AUDIO_IMACCOUNTDISCONNECTED_FILE_KEY;
	std::string getAudioIMAccountDisconnectedFile() const;

	/** Contact online audio file name. */
	static const std::string AUDIO_CONTACTONLINE_FILE_KEY;
	std::string getAudioContactOnlineFile() const;

	/** Audio smileys directory. */
	static const std::string AUDIO_SMILEYS_DIR_KEY;
	std::string getAudioSmileysDir() const;

	/** AEC (echo canceller) enable or not. */
	static const std::string AUDIO_AEC_KEY;
	bool getAudioAEC() const;

	/** Half duplex mode enable or not. */
	static const std::string AUDIO_HALFDUPLEX_KEY;
	bool getAudioHalfDuplex() const;

	/** Ringing enable or not. */
	static const std::string AUDIO_ENABLE_RINGING_KEY;
	bool getAudioRingingEnable() const;

	/** @} */

	/**
	 * @name Wengo Specific Methods
	 * @{
	 */

	/** Wengo server hostname. */
	static const std::string WENGO_SERVER_HOSTNAME_KEY;
	std::string getWengoServerHostname() const;

	/** Wengo SSO path on the server. */
	static const std::string WENGO_SSO_PATH_KEY;
	std::string getWengoSSOPath() const;

	/** Wengo SMS path on the server. */
	static const std::string WENGO_SMS_PATH_KEY;
	std::string getWengoSMSPath() const;

	/** Wengo info web service path on the server. */
	static const std::string WENGO_INFO_PATH_KEY;
	std::string getWengoInfoPath() const;

	/** Wengo subscription web service path on the server. */
	static const std::string WENGO_SUBSCRIBE_PATH_KEY;
	std::string getWengoSubscribePath() const;

	/** WengoPhone update path on the Wengo server. */
	static const std::string WENGO_SOFTUPDATE_PATH_KEY;
	std::string getWengoSoftUpdatePath() const;

	/**
	 * Wengo cirpack web service path on the server.
	 * FIXME no cirpack information please!
	 */
	static const std::string WENGO_CIRPACK_PATH_KEY;
	std::string getWengoCirpackPath() const;

	/** Wengo directory web service path on the server. */
	static const std::string WENGO_DIRECTORY_PATH_KEY;
	std::string getWengoDirectoryPath() const;

	/** Wengo realm. */
	static const std::string WENGO_REALM_KEY;
	std::string getWengoRealm() const;

	static const std::string WENGO_AUDIOTESTCALL_KEY;
	std::string getWengoAudioTestCall() const;

	static const std::string WENGO_VIDEOTESTCALL_KEY;
	std::string getWengoVideoTestCall() const;

	/** @} */

	/**
	 * @name Sip Methods
	 * @{
	 */
	static const std::string SIP_REGISTER_SERVER;
	std::string getSipRegisterServer() const;

	static const std::string SIP_REGISTER_PORT;
	int getSipRegisterPort() const;

	static const std::string SIP_PROXY_SERVER;
	std::string getSipProxyServer() const;

	static const std::string SIP_PROXY_PORT;
	int getSipProxyPort() const;

	static const std::string SIP_REALM;
	std::string getSipRealm() const;

	static const std::string SIP_SIMPLE_SUPPORT;
	bool getSipSimpleSupport() const;

	static const std::string SIP_AUDIOTESTCALL_KEY;
	std::string getSipAudioTestCall() const;

	static const std::string SIP_VIDEOTESTCALL_KEY;
	std::string getSipVideoTestCall() const;

	/** @} */

	/**
	 * @name Sip Options
	 * @{
	 */
	static const std::string SIP_REGISTER_TIMEOUT;
	int getSipRegisterTimeOut() const;

	static const std::string SIP_PUBLISH_TIMEOUT;
	int getSipPublishTimeOut() const;

	static const std::string SIP_USE_OPTIONS;
	bool getSipUseOptions() const;

	static const std::string SIP_P2P_PRESENCE;
	bool getSipP2pPresence() const;

	static const std::string SIP_CHAT_WITHOUT_PRESENCE;
	bool getSipChatWithoutPresence() const;
	/** @} */

	/**
	 * @name Profile Methods
	 * @{
	 */

	/** Gets the name of the last used UserProfile. */
	static const std::string PROFILE_LAST_USED_NAME_KEY;
	std::string getProfileLastUsedName() const;

	/** Profile all widget enabled. */
	static const std::string PROFILE_ALL_KEY;
	bool getProfileAll() const;

	/** Profile window width. */
	static const std::string PROFILE_WIDTH_KEY;
	int getProfileWidth() const;

	/** Profile window height. */
	static const std::string PROFILE_HEIGHT_KEY;
	int getProfileHeight() const;

	/** Profile window position X. */
	static const std::string PROFILE_POSX_KEY;
	int getProfilePosX() const;

	/** Profile window position Y. */
	static const std::string PROFILE_POSY_KEY;
	int getProfilePoxY() const;

	/** @} */

	/**
	 * @name Call Forwarding Methods
	 * @{
	 */

	/** Call forward mode. */
	static const std::string CALL_FORWARD_MODE_KEY;
	std::string getCallForwardMode() const;

	/** Forward call phone number 1. */
	static const std::string CALL_FORWARD_PHONENUMBER1_KEY;
	std::string getCallForwardPhoneNumber1() const;

	/** Forward call phone number 2. */
	static const std::string CALL_FORWARD_PHONENUMBER2_KEY;
	std::string getCallForwardPhoneNumber2() const;

	/** Forward call phone number 3. */
	static const std::string CALL_FORWARD_PHONENUMBER3_KEY;
	std::string getCallForwardPhoneNumber3() const;

	/** Active voice mail. */
	static const std::string VOICE_MAIL_ACTIVE_KEY;
	bool getVoiceMailActive() const;

	/** @} */

	/**
	 * @name General Settings Methods
	 * @{
	 */

	/** Automatically start WengoPhone when computer starts. */
	static const std::string GENERAL_AUTOSTART_KEY;
	bool getGeneralAutoStart() const;

	/** Start a free call if applicable. */
	static const std::string GENERAL_CLICK_START_FREECALL_KEY;
	bool getGeneralClickStartFreeCall() const;

	/** Start a chat-only when applicable. */
	static const std::string GENERAL_CLICK_START_CHAT_KEY;
	bool getGeneralClickStartChat() const;

	/** Call cell phone or land line if contact not available. */
	static const std::string GENERAL_CLICK_CALL_CELLPHONE_KEY;
	bool getGeneralClickCallCellPhone() const;

	/** Show me as away when I am inactive for. */
	static const std::string GENERAL_AWAY_TIMER_KEY;
	static const int NO_AWAY_TIMER = -1;
	int getGeneralAwayTimer() const;

	/** Show / Hide groups in the contacts list. */
	static const std::string GENERAL_SHOW_GROUPS_KEY;
	bool getShowGroups() const;

	/** Show / Hide offline contacts in the contacts list. */
	static const std::string GENERAL_SHOW_OFFLINE_CONTACTS_KEY;
	bool getShowOfflineContacts() const;

	/** 
	 * The protocol used to create hyperlinks: For example in
	 * 'wengo://someone', 'wengo' is the protocol.
	 */
	static const std::string GENERAL_HYPERLINKPROTOCOL_KEY;
	std::string getHyperlinkProtocol() const;

	/** The toolbar mode. See config.xml for possible values. */
	static const std::string GENERAL_TOOLBARMODE_KEY;
	std::string getToolBarMode() const;

	/** @} */

	/**
	 * @name Appearance Methods
	 * @{
	 */
	static const std::string APPEARANCE_CHATTHEME_KEY;
	std::string getChatTheme() const;
	/** @} */

	/** ISO 639 code of the current language for translating WengoPhone. */
	static const std::string LANGUAGE_KEY;
	static const std::string LANGUAGE_AUTODETECT_KEYVALUE;
	std::string getLanguage() const;

	/**
	 * @name Notification Settings Methods
	 * @{
	 */

	/** Shows toaster for incoming calls. */
	static const std::string NOTIFICATION_SHOW_TOASTER_ON_INCOMING_CALL_KEY;
	bool getNotificationShowToasterOnIncomingCall() const;

	/** Shows toaster for incoming chats. */
	static const std::string NOTIFICATION_SHOW_TOASTER_ON_INCOMING_CHAT_KEY;
	bool getNotificationShowToasterOnIncomingChat() const;

	/** Shows toaster when a contact is turning online. */
	static const std::string NOTIFICATION_SHOW_TOASTER_ON_CONTACT_ONLINE_KEY;
	bool getNotificationShowToasterOnContactOnline() const;

	/** Do not show any toaster or window when in do not disturb mode. */
	static const std::string NOTIFICATION_DONOTDISTURB_NO_WINDOW_KEY;
	bool getNotificationDoNotDisturbNoWindow() const;

	/** Switch off audio notifications when in do not disturb mode mode. */
	static const std::string NOTIFICATION_DONOTDISTURB_NO_AUDIO_KEY;
	bool getNotificationDoNotDisturbNoAudio() const;

	/** Do not show any call toaster when in away mode. */
	static const std::string NOTIFICATION_AWAY_NO_WINDOW_KEY;
	bool getNotificationAwayNoWindow() const;

	/** Switch off audio notifications when in away mode. */
	static const std::string NOTIFICATION_AWAY_NO_AUDIO_KEY;
	bool getNotificationAwayNoAudio() const;

	/** @} */

	/**
	 * @name Privacy Methods
	 * @{
	 */

	/** Allow calls from: anyone. */
	static const std::string PRIVACY_ALLOW_CALL_FROM_ANYONE_KEY;
	bool getPrivacyAllowCallFromAnyone() const;

	/** Allow calls from: only people from my contact list. */
	static const std::string PRIVACY_ALLOW_CALL_ONLY_FROM_CONTACT_LIST_KEY;
	bool getPrivacyAllowCallOnlyFromContactList() const;

	/** Allow chats from: anyone. */
	static const std::string PRIVACY_ALLOW_CHAT_FROM_ANYONE_KEY;
	bool getPrivacyAllowChatsFromAnyone() const;

	/** Allow chats from: only people from my contact list. */
	static const std::string PRIVACY_ALLOW_CHAT_ONLY_FROM_CONTACT_LIST_KEY;
	bool getPrivacyAllowChatOnlyFromContactList() const;

	/** When I sign into Wengo: always sign in as invisible. */
	static const std::string PRIVACY_SIGN_AS_INVISIBLE_KEY;
	bool getPrivacySignAsInvisible() const;

	/** @} */

	/**
	 * @name Video Methods
	 * @{
	 */

	static const std::string VIDEO_ENABLE_KEY;
	bool getVideoEnable() const;

	static const std::string VIDEO_WEBCAM_DEVICE_KEY;
	std::string getVideoWebcamDevice() const;

	static const std::string VIDEO_QUALITY_KEY;
	std::string getVideoQuality() const;

	static const std::string VIDEO_ENABLE_XVIDEO_KEY;
	bool getXVideoEnable() const;

	static const std::string VIDEO_ENABLE_FLIP_KEY;
	bool getVideoFlipEnable() const;

	/************************************************************************////Add by zhuang 08-12-23
	static const std::string VIDEO_FRAME_WIDTH_KEY;
	int getVideoFrameWidth() const;

	static const std::string VIDEO_FRAME_HEIGHT_KEY;
	int getVideoFrameHeight() const;

	static const std::string VIDEO_BITRATE_KEY;
	int getVideoBitRate() const;

	static const std::string VIDEO_CODEC_SERIAL;
	std::string getVideoCodecSerial() const;

	static const std::string SIP_GLOBAL_SERVICE;
	std::string getGlobalService() const;

	static const std::string IP_VERSION;
	int getIPVersion() const;

	static const std::string IP_ADDRESS;
	std::string getIPAddress() const;

	static const std::string IS_MULTICAST;
	bool getIsMulticast() const;

	//是否记住密码
	static const std::string SIP_ACCOUNT_PASSWORDREMEMBER_KEY;
	bool getPasswordRemember() const;

	//是否自动登录
	static const std::string SIP_ACCOUNT_AUTOCONNECT_KEY;
	bool getAutoConnect() const;

	/************************************************************************/

	//************************************************************************add by zhenHua.sun 2010-07-17

	///用于高清SendGraph的crossbar filter
	static const std::string CROSSBAR_NAME;
	std::string getCrossbarName() const;

	///获得Crossbar的输入类型
	static const std::string CROSSBAR_INPUT_TYPE;
	std::string getCrossbarInputType() const;

	//**********************************************************************

	//************************************************************************add by zhenHua.sun 2010-07-30

	///用于音频声道数
	static const std::string AUDIO_CHANNELS;
	int getAudioChannels() const;

	///获得音频采样率
	static const std::string AUDIO_SAMPLE_RATE;
	int getAudioSampleRate() const;

	///获得音频样本的位数
	static const std::string AUDIO_BITS_PER_SAMPLE;
	int getAudioBitsPerSample() const;

	//**********************************************************************

	/** @} */

	/**
	 * @name Path Methods
	 * @{
	 */

	/**
	 * Location of configuration files on the hard disk. 
	 * This is not stored in the config file.
	 */
	static void setConfigDir(const std::string&);
	static std::string getConfigDir();

	/** Last location where a chat history has benn saved */
	static const std::string LAST_CHAT_HISTORY_SAVE_DIR_KEY;
	std::string getLastChatHistorySaveDir() const;

	/** @} */

	/**
	 * @name Wenbox Methods
	 * @{
	 */

	/** Wenbox support enable, disable or not connected (not found). */
	static const std::string WENBOX_ENABLE_KEY;
	std::string getWenboxEnable() const;

	/** Wenbox playback audio device name. */
	static const std::string WENBOX_AUDIO_OUTPUT_DEVICEID_KEY;
	StringList getWenboxAudioOutputDeviceId() const;

	/** Wenbox record audio device name. */
	static const std::string WENBOX_AUDIO_INPUT_DEVICEID_KEY;
	StringList getWenboxAudioInputDeviceId() const;

	/** Wenbox ringer audio device name. */
	static const std::string WENBOX_AUDIO_RINGER_DEVICEID_KEY;
	StringList getWenboxAudioRingerDeviceId() const;

	/** @} */

	/**
	 * @name Other Methods
	 * @{
	 */

	/** Enables Internet Explorer ActiveX support (relevant only under Windows). */
	static const std::string IEACTIVEX_ENABLE_KEY;
	bool getIEActiveXEnable() const;

	/**
	 * Returns the authorized domains that can pass commands to the softphone.
	 * @see CommandServer
	 */
	static const std::string CMDSERVER_AUTHORIZEDDOMAINS_KEY;
	StringList getCmdServerAuthorizedDomains() const;

	/** @} */

	/**
	 * @name File Transfer Options
	 * @{
	 */

	/** Download folder. */
	static const std::string FILETRANSFER_DOWNLOAD_FOLDER_KEY;
	std::string getFileTransferDownloadFolder() const;

	/** Path to the last uploaded file. */
	static const std::string FILETRANSFER_LASTUPLOADEDFILE_FOLDER_KEY;
	std::string getLastUploadedFileFolder() const;

	/** @} */

	/**
	 * @name Linux Options
	 * @{
	 */

	/** Prefered browser. */
	static const std::string LINUX_PREFERED_BROWSER_KEY;
	std::string getLinuxPreferedBrowser() const;

	/** @} */

	/**
	 * @name Security Options
	 * @{
	 */

	/** Call encryption mode. */
	static const std::string CALL_ENCRYPTION_MODE_KEY;
	bool getCallEncryptionMode() const;

	/** @} */


	/**
	 * @name Urls
	 * @{
	 */
	static const std::string URL_ACCOUNTCREATION_KEY;
	std::string getAccountCreationUrl() const;

	static const std::string URL_FORUM_KEY;
	std::string getForumUrl() const;

	static const std::string URL_CALLOUT_KEY;
	std::string getCalloutUrl() const;

	static const std::string URL_SMS_KEY;
	std::string getSmsUrl() const;

	static const std::string URL_VOICEMAIL_KEY;
	std::string getVoiceMailUrl() const;

	static const std::string URL_EXTERNALDIRECTORY_KEY;
	std::string getExternalDirectoryUrl() const;

	static const std::string URL_INTERNALDIRECTORY_KEY;
	std::string getInternalDirectoryUrl() const;

	static const std::string URL_FAQ_KEY;
	std::string getFaqUrl() const;

	static const std::string URL_ACCOUNT_KEY;
	std::string getAccountUrl() const;

	static const std::string URL_BUYWENGOS_KEY;
	std::string getBuyWengosUrl() const;

	static const std::string URL_DOWNLOAD_KEY;
	std::string getDownloadUrl() const;

	static const std::string URL_PHONENUMBER_KEY;
	std::string getPhoneNumberUrl() const;

	static const std::string URL_WIKI_KEY;
	std::string getWikiUrl() const;

	static const std::string URL_LOSTPASSWORD_KEY;
	std::string getLostPasswordUrl() const;

	static const std::string URL_LAUNCHPAD_KEY;
	std::string getLaunchpadUrl() const;

	static const std::string URL_HOME_HTTP_KEY;
	std::string getHttpHomeUrl() const;

	static const std::string URL_HOME_HTTPS_KEY;
	std::string getHttpsHomeUrl() const;

	static const std::string URL_COMPANYWEBSITE_KEY;
	std::string getCompanyWebSiteUrl() const;

	/** @} */

	/**
	 * @name Feature Methods
	 * @{
	 */
	static const std::string FEATURE_SMS_KEY;
	bool getSmsFeatureEnabled() const;

	static const std::string FEATURE_AUDIOCONFERENCE_KEY;
	bool getAudioConferenceFeatureEnabled() const;

	static const std::string SIP_USE_TYPING_STATE;
	bool getSipUseTypingState() const;
	/** @} */

	
	static const  std::string QOS_SERVER_SIP_URI_KEY;
	//获取QoS服务器sip uri
	std::string getQosServerSipUri() const;

	static const  std::string QOS_SIMULATOR_SIP_URI_KEY;
	//获取QoS模拟器sip uri
	std::string getSimulatorServerSipUri() const;

private:

	/** Name of this configuration. */
	std::string _name;

	static std::string _configDir;
};

#endif	//OWCONFIG_H
