#include "stdafx.h"
#include ".\cvmsgparser.h"


CVMsgParser::CVMsgParser(void)
{
    //����TiXmlDocument����
    _doc = new TiXmlDocument(); 
    _coolviewElement = NULL;
}

CVMsgParser::~CVMsgParser(void)
{
    if(_doc)
    {
        delete _doc;
        _doc = NULL;
    }
}

const char* CVMsgParser::GetCommandType()
{
    TiXmlAttribute* pAttrib = _coolviewElement->FirstAttribute();
    if(pAttrib != NULL)
    {
        return pAttrib->Value();
    }
    else
        return "";
}

void CVMsgParser::InitParser(const char* message)
{
    _doc->Parse(message);
    _coolviewElement = _doc->FirstChildElement("coolview");
    TiXmlAttribute* pAttrib = _coolviewElement->FirstAttribute();
}


vector<ConfInfo*> CVMsgParser::ParseConfListMsg(vector<ConfInfo*>& conf_list)
{
    TiXmlElement* pConfElement = _coolviewElement->FirstChildElement();
    if(pConfElement == NULL)
        return conf_list;
    TiXmlElement* pChildElement = pConfElement->FirstChildElement("FocusURI");
    for(pConfElement; pConfElement != NULL; pConfElement = pConfElement->NextSiblingElement())
    {
        ConfInfo* conference = new ConfInfo;
        TiXmlElement* element = NULL;
        conference->URI = GetFirstChildElementValue(pConfElement, "uri");
        conference->CID = GetFirstChildElementValue(pConfElement, "cid");
        conference->Title = GetFirstChildElementValue(pConfElement, "title");
        conference->Description = GetFirstChildElementValue(pConfElement, "description");
        conference->HostURI = GetFirstChildElementValue(pConfElement, "hostUri");   
        conference->ControlMode = GetFirstChildElementValue(pConfElement, "controlMode");
        conference->JoinMode = GetFirstChildElementValue(pConfElement, "joinMode");
        conference->JoinPassword = GetFirstChildElementValue(pConfElement, "joinPassword");
        conference->StartTime = GetFirstChildElementValue(pConfElement, "startTime");
        conference->Duration = GetFirstChildElementValue(pConfElement, "duration");
		conference->multicastAddress = GetFirstChildElementValue(pConfElement, "multicastAddress");
        conf_list.push_back(conference);
    }
    return conf_list;
}


vector<MemberInfo*> CVMsgParser::ParseMemberListMsg(vector<MemberInfo*>& member_list, const string& hosturi,const string& confmode)
{
    TiXmlElement* pMemberElement = _coolviewElement->FirstChildElement("members")->FirstChildElement("member");
    for(pMemberElement; pMemberElement != NULL; pMemberElement = pMemberElement->NextSiblingElement())
    {
        MemberInfo* member = new MemberInfo;
		ParseSingleMemberElement(*pMemberElement,*member,hosturi,confmode);
        member_list.push_back(member);
    }
    return member_list;
}

vector<MemberInfo*> CVMsgParser::ParseMemberListMsg(int& audio_port, int& video_port, string& address)
{
	return	ParseMemberListMsg(audio_port,video_port,address,"","");
}

vector<MemberInfo*> CVMsgParser::ParseMemberListMsg(int& audio_port, int& video_port, string& address,const string& hosturi,const string& confmode)
{
    vector<MemberInfo*> member_list;
    getAddrPort(audio_port, video_port, address);
    TiXmlElement* pMemberElement = _coolviewElement->FirstChildElement("members")->FirstChildElement("member");
    for(pMemberElement; pMemberElement != NULL; pMemberElement = pMemberElement->NextSiblingElement())
    {
        MemberInfo* member = new MemberInfo;
		ParseSingleMemberElement(*pMemberElement,*member,hosturi,confmode);        
        member_list.push_back(member);
    }
    return member_list;
}

vector<MemberInfo*> CVMsgParser::ParseMemberListByNotifyMsg(vector<MemberInfo*>& member_list,const string& hosturi,const string& confmode)
{
    if(strcmp(GetCommandType(), "LoginNotify") == 0 || strcmp(GetCommandType(), "loginnotify") == 0)
    {
        TiXmlElement* pMemberElement = _coolviewElement->FirstChildElement("member");
        string uri = GetFirstChildElementValue(pMemberElement, "uri");      
        vector<MemberInfo*>::iterator iter = member_list.begin();
        for(iter; iter != member_list.end(); iter++)
        {
            MemberInfo* c = (MemberInfo*)(*iter);
            if(c->MemberURI == uri)
            {
                delete c;
                member_list.erase(iter);
                break;
            }
        }
        for(pMemberElement; pMemberElement != NULL; pMemberElement = pMemberElement->NextSiblingElement())
        {
            MemberInfo* member = new MemberInfo;
			ParseSingleMemberElement(*pMemberElement,*member,hosturi,confmode);
            member_list.push_back(member);
        }
    }
    else if(strcmp(GetCommandType(), "LogoutNotify") == 0 || strcmp(GetCommandType(), "logoutnotify") == 0)
    {
        TiXmlElement* pUriElement = _coolviewElement->FirstChildElement("uri");
        string uri = pUriElement->FirstChild()->ToText()->ValueStr();
        vector<MemberInfo*>::iterator iter = member_list.begin();
        for(iter; iter != member_list.end(); iter++)
        {
            MemberInfo* c = (MemberInfo*)(*iter);
            if(c->MemberURI == uri)
            {
                delete c;
                member_list.erase(iter);
                break;
            }
        }
    }

    return member_list;
}

void CVMsgParser::getAddrPort(int& audio_port, int& video_port, string& address)
{
	address = GetFirstChildElementValue(_coolviewElement, "multicastAddress");
	audio_port = atoi(GetFirstChildElementValue(_coolviewElement, "audioPort").c_str());
	video_port = atoi(GetFirstChildElementValue(_coolviewElement, "videoPort").c_str());
}

//解析单条成员信息，即xml内容中的<member></member>元素
int CVMsgParser::ParseSingleMemberElement( TiXmlElement& memberElement,  MemberInfo&  info, const string& hosturi, const string& confmode)
{
	info.MemberURI = GetFirstChildElementValue(&memberElement, "uri");
	info.Sdp = GetFirstChildElementValue(&memberElement, "sdp");
	info.isReceived = 0;
	info.hand = 0;
	info.isMulticast = true;
	if(confmode == "host" && info.MemberURI == hosturi)
	{
		info.permitSpeak = 2;
	}
	else
	{
		string floor = GetFirstChildElementValue(&memberElement, "floor"); 
		if(floor == "true")
			info.permitSpeak = 1;
		else
			info.permitSpeak = 0;
	}
	info.multicastAddress = GetFirstChildElementValue(&memberElement,"multicastAddress");
	info.videoPort = atoi(GetFirstChildElementValue(&memberElement, "videoPort").c_str());
	info.audioPort = atoi(GetFirstChildElementValue(&memberElement, "audioPort").c_str());
	return 0;
}


void CVMsgParser::ReleaseConfList(vector<ConfInfo*>& conf_list)
{
    vector<ConfInfo*>::iterator iter = conf_list.begin();
    for(iter; iter != conf_list.end(); iter++)
    {
        ConfInfo* c = (ConfInfo*)(*iter);
        delete c;
    }
    conf_list.clear();
}

void CVMsgParser::ReleaseMemberList(vector<MemberInfo*>& member_list)
{
    vector<MemberInfo*>::iterator iter = member_list.begin();
    for(iter; iter != member_list.end(); iter++)
    {
        MemberInfo* c = (MemberInfo*)(*iter);
        if(c != NULL)
            delete c;
    }
    member_list.clear();
}

string CVMsgParser::CreateMsgByConfInfo(ConfInfo* conf_info)
{
    string szContent = "<?xml version=\"1.0\"?><coolview command=\"ConferenceInvite\"><conference>";
    szContent += "<uri>" + conf_info->URI + "</uri>";
    szContent += "<title>" + conf_info->Title + "</title>";
    szContent += "<description>" + conf_info->Description + "</description>";
    szContent += "<hostUri>" + conf_info->HostURI + "</hostUri>";
    szContent += "<controlMode>" + conf_info->ControlMode + "</controlMode>";
    szContent += "<joinMode>" + conf_info->JoinMode + "</joinMode>";
    szContent += "<startTime>" + conf_info->JoinMode + "</startTime>";
    szContent += "<duration>" + conf_info->Duration + "</duration>";
    szContent += "</conference></coolview>";
    return szContent;
}

string CVMsgParser::CreateMsgByConfInfo(ConfInfo* conf_info, string command)
{
    string szContent = "<?xml version=\"1.0\"?><coolview command=\"" + command + "\"><conference>";
    szContent += "<uri>" + conf_info->URI + "</uri>";
    szContent += "<title>" + conf_info->Title + "</title>";
    szContent += "<description>" + conf_info->Description + "</description>";
    szContent += "<hostUri>" + conf_info->HostURI + "</hostUri>";
    szContent += "<controlMode>" + conf_info->ControlMode + "</controlMode>";
    szContent += "<joinMode>" + conf_info->JoinMode + "</joinMode>";
    szContent += "<startTime>" + conf_info->JoinMode + "</startTime>";
    szContent += "<duration>" + conf_info->Duration + "</duration>";
    szContent += "</conference></coolview>";
    return szContent;
}

string CVMsgParser::GetFocusURI()
{
    return GetFirstChildElementValue(_coolviewElement, "uri");
}

string CVMsgParser::GetMemberUri()
{
    return GetFirstChildElementValue(_coolviewElement, "uri");
}

string CVMsgParser::GetInviterUri()
{
    return GetFirstChildElementValue(_coolviewElement, "inviter");
}

string CVMsgParser::GetLoginMemberUri()
{
    TiXmlElement* pMemberElement = _coolviewElement->FirstChildElement("member");
    return  GetFirstChildElementValue(pMemberElement, "uri");
}

string CVMsgParser::GetTextMessage()
{
    return GetFirstChildElementValue(_coolviewElement, "message");
}

string CVMsgParser::GetWhiteBoardMessage()
{
    return GetFirstChildElementValue(_coolviewElement, "content");
}

string CVMsgParser::GetReceiverIP()
{
    return GetFirstChildElementValue(_coolviewElement, "ip");
}

/*
Add by heixanfu 
为了处理接收的QOS信息
09-9-4
*/
string CVMsgParser::GetElementValueByName(const string fatherElementName, const string childElementName)
{
    string value = "";   
    if(fatherElementName == "coolview")
    {
        value = GetFirstChildElementValue(_coolviewElement, childElementName.c_str());
    }
    else
    {
        TiXmlElement* element = _coolviewElement->FirstChildElement(fatherElementName);
        if(element != NULL)
            value = GetFirstChildElementValue(element, childElementName.c_str());
    }
    return value;
}

string CVMsgParser::GetQosType()
{
    TiXmlAttribute* pAttrib = _coolviewElement->FirstAttribute();
    if(strcmp( pAttrib->Value(), "qos") == 0)
    {
        return pAttrib->Next()->ValueStr();
    }
    else
        return "";
}

string CVMsgParser::GetQosTestType()
{
	TiXmlAttribute* pAttrib = _coolviewElement->FirstAttribute();
	if(strcmp( pAttrib->Value(), "qostest") == 0)
	{
		return pAttrib->Next()->ValueStr();
	}
	else
		return "";
}


int CVMsgParser::ParseLoginPermissionMsg(MsgLoginPermissionInfo& info)
{
    info.userURI = GetFirstChildElementValue(_coolviewElement, "uri");   
    info.dscp = atoi(GetFirstChildElementValue(_coolviewElement, "dscp").c_str());   
    info.rate = atoi(GetFirstChildElementValue(_coolviewElement, "rate").c_str());   
    return 0;
}


int CVMsgParser::ParseLoginRejectionMsg(MsgLoginRejectionInfo& info)
{
    info.userURI = GetFirstChildElementValue(_coolviewElement, "uri");   
    info.description = GetFirstChildElementValue(_coolviewElement, "description");   
    info.suggestedRate = atoi(GetFirstChildElementValue(_coolviewElement, "suggestedRate").c_str());   
    return 0;
}

int CVMsgParser::ParseQoSTestBeginSendInfo( QoSTestBeginSendInfo& info )
{
	info.sessionId =atoi( GetFirstChildElementValue(_coolviewElement, "testSessionId").c_str()); 
	info.remoteIP = GetFirstChildElementValue(_coolviewElement, "remoteIP");   
	info.remotePort = atoi(GetFirstChildElementValue(_coolviewElement, "remotePort").c_str());   
	info.sendRate = atoi(GetFirstChildElementValue(_coolviewElement, "rate").c_str());   
	info.dscp = atoi(GetFirstChildElementValue(_coolviewElement, "dscp").c_str());   
	info.duration = atoi(GetFirstChildElementValue(_coolviewElement, "duration").c_str());   
	return 0;
}

int CVMsgParser::ParseQoSTestBeginReceiveInfo( QoSTestBeginReceiveInfo& info )
{
	info.sessionId =atoi( GetFirstChildElementValue(_coolviewElement, "testSessionId").c_str()); 
	info.localPort = atoi(GetFirstChildElementValue(_coolviewElement, "localPort").c_str());   
	info.sourceIP = GetFirstChildElementValue(_coolviewElement, "sourceIP");   
	info.duration = atoi(GetFirstChildElementValue(_coolviewElement, "duration").c_str());   
	return 0;	
}