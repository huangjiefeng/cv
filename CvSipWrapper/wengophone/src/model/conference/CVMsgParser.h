#pragma once

#ifndef CVMsgParser_h
#define CVMsgParser_h


#include < iostream>
#include < fstream>
#include <tinyxml.h>
#include <map>
#include <vector>
#include <list>
using namespace std;

typedef struct tagConfInfo
{
    string CID;         //会议的id，全局唯一
    string URI;
    string Title;
    string Description;
    string HostURI;		//主持人URI
    string ControlMode;
    string JoinMode;
    string JoinPassword;
    string StartTime;
    string Duration;

	string multicastAddress;	//如果会议是组播会议，则其为组播地址，否则为0.0.0.0  --注:由于当前控制服务器发给终端的会议列表没有该地址信息，所以multicastAddress字段可能为空，使用时需注意。
}ConfInfo;

typedef struct tagMemberInfo
{
    string MemberURI;   //成员uri
    string Sdp;         //sdp
    int isReceived;     //是否已经接收了该成员的媒体流，0未接收，1已接收
    int permitSpeak;    //是否允许发言，0不允许，1允许，2代表主持人
    int hand;           //是否举手，0没举手，1举手
    bool isMulticast;   //是否支持组播
	string multicastAddress;		//组播会议的IP地址，单播会议则是0.0.0.0
	int videoPort;		//视频数据传输的本地端口，默认值为0
	int audioPort ;		//音频数据传输的本地端口，默认值为0
}MemberInfo;

struct ReceiverInfo
{
    string MemberURI;   //成员uri
    string recvIP;			 //成员的IP地址
    int videoPort;		//视频数据传输端口
    int audioPort ;		//音频数据传输端口   
};

//允许加入会议消息的信息内容
struct MsgLoginPermissionInfo
{
    string userURI;
    int dscp;
    int rate;
};

//禁止加入会议消息的信息内容
struct MsgLoginRejectionInfo
{
    string userURI;
    string description;
    int suggestedRate;
};


struct QoSTestBeginSendInfo
{
	int sessionId;
	string remoteIP;
	int remotePort;
	int sendRate;
	int dscp;
	int duration;
};

struct  QoSTestBeginReceiveInfo
{
	int sessionId;
	int localPort;
	string sourceIP;
	int duration;
};

class CVMsgParser
{
public:
    CVMsgParser(void);
    ~CVMsgParser(void);

    //将sip消息内容解析成会议列表
    vector<ConfInfo*> ParseConfListMsg(vector<ConfInfo*>& conf_list);
    //进入会议时获得音视频发送的地址和端口
    void getAddrPort(int& audio_port, int& video_port, string& address);

    ////将sip消息内容解析成员列表，根据需要重载了几个函数
    vector<MemberInfo*> ParseMemberListMsg(vector<MemberInfo*>& member_list,const string& hosturi, const string& confmode);
    vector<MemberInfo*> ParseMemberListMsg(int& audiao_port, int& video_port, string& address);
    vector<MemberInfo*> ParseMemberListMsg(int& audio_port, int& video_port, string& address, const string& hosturi, const string& confmode);

	//根据成员加入/退出会议消息更新成员列表
    vector<MemberInfo*> ParseMemberListByNotifyMsg(vector<MemberInfo*>& member_list, const string& hosturi, const string& confmode);

    //解析消息内容中的命令类型
    const char* GetCommandType();
    void InitParser(const char* message);

    //清空会议列表
	static  void ReleaseConfList(vector<ConfInfo*>& conf_list);
    //清空人员列表
   static	 void ReleaseMemberList(vector<MemberInfo*>& member_list);

	//根据Confinfo构造一条xml内容
    string CreateMsgByConfInfo(ConfInfo* conf_info);
    string CreateMsgByConfInfo(ConfInfo* conf_info, string command);

    //获取FocusURI
    string GetFocusURI();
    string GetMemberUri();
    string GetInviterUri();
    string GetLoginMemberUri();
    string GetTextMessage();
    string GetWhiteBoardMessage();
    //
    string GetReceiverIP();

    /*
      Add by heixanfu 
         为了处理接收的QOS信息
      09-9-4
    */
    string GetElementValueByName(const string fatherElementName, const string childElementName);
	
	//获取qos命令的type类型
    string GetQosType();

  
    //解析LoginPermission sip 消息，返回0表示解析成功
    int ParseLoginPermissionMsg(MsgLoginPermissionInfo& info);

    //解析一条LoginRejection  sip 消息，返回0表示解析成功
    int ParseLoginRejectionMsg(MsgLoginRejectionInfo& info);

	//解析一条QoSTestBeginSend  sip 消息，返回0表示解析成功
	int ParseQoSTestBeginSendInfo(QoSTestBeginSendInfo& info);

	//解析一条QoSTestBeginReceive  sip 消息，返回0表示解析成功
	int ParseQoSTestBeginReceiveInfo(QoSTestBeginReceiveInfo& info);

	string GetQosTestType();

private:
    //获取符合指定名称的第一个xml元素的值（字符串），找不到对应元素则返回空字符串
    static string GetFirstChildElementValue(TiXmlElement* parentElement, const char* childElementName);
	
	//解析单条成员信息，即xml内容中的<member></member>元素
	static	int ParseSingleMemberElement( TiXmlElement& memberElement, MemberInfo& info, const string& hosturi, const string& confmode);
private:
    TiXmlDocument* _doc;
    TiXmlElement* _coolviewElement;
};


inline string CVMsgParser::GetFirstChildElementValue(TiXmlElement* parentElement, const char* childElementName)
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