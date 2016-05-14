#pragma once

#ifndef CVMsgParser_h
#define CVMsgParser_h

#include <iostream>
//#include <fstream>
//#include <map>
#include <vector>
//#include <list>
#include <QtCore/QtCore>
#include "RemoteReceiver.h"

#include <tinyxml/tinyxml.h>

typedef struct tagConfInfo
{
    std::string CID;         //会议的id，全局唯一
    std::string URI;
    std::string Title;
    std::string Description;
    std::string HostURI;		//主持人URI
    std::string ControlMode;	//会议模式：discuss或report
    std::string JoinMode;
    std::string JoinPassword;
    std::string StartTime;
    std::string Duration;

	std::string multicastAddress;	//如果会议是组播会议，则其为组播地址，否则为0.0.0.0  --注:由于当前控制服务器发给终端的会议列表没有该地址信息，所以multicastAddress字段可能为空，使用时需注意。

	std::string		level;		//会议级别
	std::string  Chairman;		//主席账号
	int		pptport;		//用于屏幕共享
}ConfInfo;

typedef struct tagMemberInfo
{
 //   std::string MemberURI;   //成员uri
 //   std::string Sdp;         //sdp
 //   int isReceived;     //是否已经接收了该成员的媒体流，0未接收，1已接收
 //   int permitSpeak;    //是否允许发言，0不允许，1允许，2代表主持人
 //   int hand;           //是否举手，0没举手，1举手
 //   bool isMulticast;   //是否支持组播
	//std::string multicastAddress;		//组播会议的IP地址，单播会议则是0.0.0.0
	//int videoPort;		//视频数据传输的本地端口，默认值为0
	//int audioPort ;		//音频数据传输的本地端口，默认值为0

	//新版本会议服务器支持的字段
	std::string username;		//sipAccount name
	std::string name;			//成员的真实名称
	std::string MemberURI;				//用户URI
	std::string xmpp;			//IM账户
	std::string company;			//公司名称
	std::string sdp;				
	std::string floor;
	std::string multicastAddress;
	int audioPort;
	int videoPort;
	std::string status;
	std::string location;
	std::string AllowTerminal;	//允许操作的终端，用\t分隔各个uri
	int	   permitSpeak;
}MemberInfo;

typedef struct tagTerminalInfo
{
	std::string	uri;		//终端uri
	std::string  virtualUri; //终端存在多摄像头时的虚拟终端号，格式类似v1#{uri}
	int     virtualIndex; //数字表示的虚拟终端索引
	int     virtualCount; //该终端总的虚拟终端数
	std::string	name;	//终端名称
	std::string	dns;		//域名服务器
	std::string	model;	//工作模式
	std::string	version;	//终端版本
	std::string	sdp;		//sdp
	bool	isMulticast;   //是否支持组播
	std::string	multicastAddress;		//组播会议的IP地址，单播会议则是0.0.0.0
	int		videoPort;		//视频数据传输的本地端口，默认值为0
	int		audioPort ;		//音频数据传输的本地端口，默认值为0
	int		smallVideoPort;	//视频小流的传输端口，默认值为0
	std::string	status;			//终端状态
	std::string  location;		//终端位置
	std::string	isSpeaking;		//是否允许发言
	std::string  isHandUp;       //是否举手
	std::string  isChairmanTerminal;  //是否是主席终端
	std::string  isMainSpeaker;		 //是否是主讲人
	int     totalTermialNum; //会议的终端总数

}TerminalInfo;

typedef std::vector<ConfInfo*>		ConferenceVector;
typedef std::vector<MemberInfo*>	MemberVector;
typedef std::vector<TerminalInfo*> TerminalVector;


//允许加入会议消息的信息内容
struct MsgLoginPermissionInfo
{
    std::string userURI;
    int dscp;
    int rate;
};

//禁止加入会议消息的信息内容
struct MsgLoginRejectionInfo
{
    std::string userURI;
    std::string description;
    int suggestedRate;
};

//QoS服务器信息
struct MsgQoSServerInfo 
{
	std::string sipUri;	
	std::string ip;
	int tcpPort;
	int simulateTestUdpPort; //Qos服务器接收模拟测试报告端口
	int operationUdpPort;		//Qos服务器接收报告端口
};

class CVMsgParser
{
public:
    CVMsgParser(void);
    ~CVMsgParser(void);

	void InitParser(const char* message);

	TiXmlElement* GetCoolViewElement() { return this->_coolviewElement; }

	//获取XML消息的文本内容
	const char * getMessageContent();

    //将sip消息内容解析成会议列表
    std::vector<ConfInfo*> ParseConfListMsg(std::vector<ConfInfo*>& conf_list);

	void ParseConfListMsg( QList<QVariantMap>* confList );
	void ParseMemberListMsg(QList<QVariantMap>* memberList,const std::string& hosturi, const std::string& confmode);

    //进入会议时获得音视频发送的地址和端口
    void getAddrPort(int& audio_port, int& video_port, std::string& address);

    ////将sip消息内容解析成员列表，根据需要重载了几个函数
    std::vector<MemberInfo*> ParseMemberListMsg(std::vector<MemberInfo*>& member_list,const std::string& hosturi, const std::string& confmode);
    std::vector<MemberInfo*> ParseMemberListMsg(int& audiao_port, int& video_port, std::string& address);
    std::vector<MemberInfo*> ParseMemberListMsg(int& audio_port, int& video_port, std::string& address, const std::string& hosturi, const std::string& confmode);

	///将sip消息内容解析为终端列表
	TerminalVector ParseTerminalListMsg( TerminalVector& terminalList , const std::string& hosturi , const std::string& confmode );
//************************************
// Method:    recvScreenCancel2ReceiverMsgHandler
// FullName:  ConferenceManager::recvScreenCancel2ReceiverMsgHandler
// Access:    private 
// Returns:   CvSipWrapperIf*
// Qualifier:
//************************************

	//根据成员加入/退出会议消息更新成员列表
    std::vector<MemberInfo*> ParseMemberListByNotifyMsg(std::vector<MemberInfo*>& member_list, const std::string& hosturi, const std::string& confmode);

	void ParseMemberInOrOutByMsg( std::string* memberURI ,bool* memberIn , std::vector<MemberInfo*>& member_list, const std::string& hosturi, const std::string& confmode);

    //解析消息内容中的命令类型
    const char* GetCommandType();
    //清空会议列表
	static  void ReleaseConfList(std::vector<ConfInfo*>& conf_list);
    //清空人员列表
	static	 void ReleaseMemberList(std::vector<MemberInfo*>& member_list);
	//清空终端列表
	static void ReleaseTerminalList( TerminalVector& terminalList);
 

	//根据Confinfo构造一条xml内容
    std::string CreateMsgByConfInfo(ConfInfo* conf_info);
    std::string CreateMsgByConfInfo(ConfInfo* conf_info, std::string command);

    //获取FocusURI
    std::string GetFocusURI();
	std::string GetMemberUri();
	std::string GetTerminalUri();
	std::string GetVirtualUri();
    std::string GetInviterUri();
    std::string GetLoginMemberUri();
    std::string GetTextMessage();
    std::string GetWhiteBoardMessage();
	std::string GetResult();
	std::string GetTerminalCount();
    //
    std::string GetReceiverIP();

	//终端配置模式
	std::string GetTerminalConfMode();

	//获取允许值
	std::string GetPermission();

	//获取类型
	std::string GetType();

	//获取开关值
	std::string GetEnable();

    /*
      Add by heixanfu 
         为了处理接收的QOS信息
      09-9-4
    */
    std::string GetElementValueByName(const std::string fatherElementName, const std::string childElementName);
	
	//获取qos命令的type类型
    std::string GetQosType();

  
    //解析LoginPermission sip 消息，返回0表示解析成功
    int ParseLoginPermissionMsg(MsgLoginPermissionInfo& info);

    //解析一条LoginRejection  sip 消息，返回0表示解析成功
    int ParseLoginRejectionMsg(MsgLoginRejectionInfo& info);

	//解析QoS服务器信息
	 int ParseQoSServerInfoMsg(MsgQoSServerInfo& info);

	//---------------------------登录通告-------------------------------
	/**
	 * @brief 获取登录通告的类型
	 * @return 如果为0则说明是member，1则是terminal
	 */
	int GetLoginNotifyType( );

	///解析用户登录通告,失败返回0
	MemberInfo* ParseMemberLoginNotifyMsg( MemberVector&memberList , std::string& hostUri , std::string& confMode );

	///解析终端登录通告,失败返回0
	TerminalVector &ParseTerminalLoginNotifyMsg( TerminalVector &terminalList,  std::string& hostUri , std::string& confMode );


private:
    //获取符合指定名称的第一个xml元素的值（字符串），找不到对应元素则返回空字符串
    static std::string GetFirstChildElementValue(TiXmlElement* parentElement, const char* childElementName);
	
	//解析单条成员信息，即xml内容中的<member></member>元素
	static	int ParseSingleMemberElement( TiXmlElement& memberElement, MemberInfo& info, const std::string& hosturi, const std::string& confmode);

	static void  ParseSingleMemberElement( TiXmlElement& memberElement, QVariantMap* member, const std::string& hosturi, const std::string& confmode);

	//解析单条终端信息，即xml内容中的<terminal></terminal>元素
	static int ParseSingleTerminalElement( TiXmlElement& memberElement, TerminalInfo& info, const std::string& hosturi, const std::string& confmode);


private:
    TiXmlDocument* _doc;
    TiXmlElement* _coolviewElement;
	
	std::string _content;		//xml消息的文本内容
};


inline std::string CVMsgParser::GetFirstChildElementValue(TiXmlElement* parentElement, const char* childElementName)
{
    TiXmlElement* element = parentElement->FirstChildElement(childElementName);
    if(element)
    {
        TiXmlNode* node = element->FirstChild();
        if(node)
            return node->ValueStr();    
        //return node->ToText()->ValueStr();    
    }
    return "";
}

#endif