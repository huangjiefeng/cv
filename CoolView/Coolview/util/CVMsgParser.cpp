#include ".\cvmsgparser.h"
#include "profile\RunningProfile.h"

using std::vector;
using std::string;

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
    _content = message != NULL ? message : "";
}


vector<ConfInfo*> CVMsgParser::ParseConfListMsg(vector<ConfInfo*>& conf_list)
{
    TiXmlElement* pConfElement = _coolviewElement->FirstChildElement();
    if(pConfElement == NULL)
        return conf_list;
    TiXmlElement* pChildElement = pConfElement->FirstChildElement("FocusURI");
    for(pConfElement; pConfElement != NULL; pConfElement = pConfElement->NextSiblingElement())
    {
        //判断会议节点是否重复
        string uri = GetFirstChildElementValue(pConfElement , "uri" );
        bool alreadyExist = false;
        for( vector<ConfInfo*>::iterator it = conf_list.begin() ; it != conf_list.end(); it++ )
        {
            if( (*it)->URI == uri )
            {
                alreadyExist = true;
                break;
            }
        }
        if( alreadyExist )
            continue;

        ConfInfo* conference = new ConfInfo;
        TiXmlElement* element = NULL;		
        conference->URI = uri;
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
		conference->pptport = atoi(GetFirstChildElementValue( pConfElement , "pptPort" ).c_str());
		conference->Chairman= GetFirstChildElementValue( pConfElement , "chairman" ).c_str();
		conference->level = GetFirstChildElementValue(pConfElement,"level");
        conf_list.push_back(conference);
    }
    return conf_list;
}

void CVMsgParser::ParseConfListMsg( QList<QVariantMap>* confList )
{
    TiXmlElement* pConfElement = _coolviewElement->FirstChildElement();
    if(pConfElement == NULL)
        return ;
    TiXmlElement* pChildElement = pConfElement->FirstChildElement("FocusURI");
    for(pConfElement; pConfElement != NULL; pConfElement = pConfElement->NextSiblingElement())
    {
        TiXmlElement* element = NULL;
        QString URI = GetFirstChildElementValue(pConfElement, "uri").c_str();
        QString CID = GetFirstChildElementValue(pConfElement, "cid").c_str();
        QString Title = GetFirstChildElementValue(pConfElement, "title").c_str();
        QString Description = GetFirstChildElementValue(pConfElement, "description").c_str();
        QString HostURI = GetFirstChildElementValue(pConfElement, "hostUri").c_str();
        QString ControlMode = GetFirstChildElementValue(pConfElement, "controlMode").c_str();
        QString JoinMode = GetFirstChildElementValue(pConfElement, "joinMode").c_str();
        QString JoinPassword = GetFirstChildElementValue(pConfElement, "joinPassword").c_str();
        QString StartTime = GetFirstChildElementValue(pConfElement, "startTime").c_str();
        QString Duration = GetFirstChildElementValue(pConfElement, "duration").c_str();
        QString multicastAddress = GetFirstChildElementValue(pConfElement, "multicastAddress").c_str();
        QString level = GetFirstChildElementValue(pConfElement, "level").c_str();

        QVariantMap conf;
        conf.insert("name" , Title);
        conf.insert("conference-id" , CID );
        conf.insert("start-time" , StartTime);
        conf.insert("control-mode" , ControlMode);
        conf.insert("join-mode" , JoinMode );
        conf.insert("duration" , Duration);
        conf.insert("level" , level);

        confList->push_back( conf );
    }
}

vector<MemberInfo*> CVMsgParser::ParseMemberListMsg(vector<MemberInfo*>& member_list, const string& hosturi, const string& confmode)
{
    TiXmlElement* pMemberElement = _coolviewElement->FirstChildElement("members")->FirstChildElement("member");
    for(pMemberElement; pMemberElement != NULL; pMemberElement = pMemberElement->NextSiblingElement())
    {
        MemberInfo* member = new MemberInfo;
        ParseSingleMemberElement(*pMemberElement, *member, hosturi, confmode);

        //判断成员列表里是否存在相对应的成员
        bool alreadyExist = false;
        for( vector<MemberInfo*>::iterator it = member_list.begin() ; it != member_list.end() ; it++ )
        {
            if( (*it)->MemberURI == member->MemberURI )
            {
                delete member;
                member = NULL;
                alreadyExist = true;
                break;
            }
        }
        if( !alreadyExist )
            member_list.push_back(member);
    }
    return member_list;
}

vector<MemberInfo*> CVMsgParser::ParseMemberListMsg(int& audio_port, int& video_port, string& address)
{
    return	ParseMemberListMsg(audio_port, video_port, address, "", "");
}

vector<MemberInfo*> CVMsgParser::ParseMemberListMsg(int& audio_port, int& video_port, string& address, const string& hosturi, const string& confmode)
{
    vector<MemberInfo*> member_list;
    getAddrPort(audio_port, video_port, address);
    TiXmlElement* pMemberElement = _coolviewElement->FirstChildElement("members")->FirstChildElement("member");
    for(pMemberElement; pMemberElement != NULL; pMemberElement = pMemberElement->NextSiblingElement())
    {
        MemberInfo* member = new MemberInfo;
        ParseSingleMemberElement(*pMemberElement, *member, hosturi, confmode);
        member_list.push_back(member);
    }
    return member_list;
}

void CVMsgParser::ParseMemberListMsg( QList<QVariantMap>* memberList, const string& hosturi, const string& confmode )
{
    TiXmlElement* pMemberElement = _coolviewElement->FirstChildElement("members")->FirstChildElement("member");
    for(pMemberElement; pMemberElement != NULL; pMemberElement = pMemberElement->NextSiblingElement())
    {
        QVariantMap member;
        ParseSingleMemberElement(*pMemberElement, &member, hosturi, confmode);
        memberList->push_back(member);
    }
}


vector<MemberInfo*> CVMsgParser::ParseMemberListByNotifyMsg(vector<MemberInfo*>& member_list, const string& hosturi, const string& confmode)
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
            ParseSingleMemberElement(*pMemberElement, *member, hosturi, confmode);
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

void CVMsgParser::ParseMemberInOrOutByMsg( string* memberURI , bool* memberIn, vector<MemberInfo*>& member_list, const string& hosturi, const string& confmode )
{
    if(strcmp(GetCommandType(), "LoginNotify") == 0 || strcmp(GetCommandType(), "loginnotify") == 0)
    {
        *memberIn = true;
        TiXmlElement* pMemberElement = _coolviewElement->FirstChildElement("member");
        *memberURI = GetFirstChildElementValue(pMemberElement, "uri");
    }
    else if(strcmp(GetCommandType(), "LogoutNotify") == 0 || strcmp(GetCommandType(), "logoutnotify") == 0)
    {
        *memberIn = false;
        TiXmlElement* pUriElement = _coolviewElement->FirstChildElement("uri");
        *memberURI = pUriElement->FirstChild()->ToText()->ValueStr();
    }
    ParseMemberListByNotifyMsg( member_list , hosturi , confmode );
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
    //info.MemberURI = GetFirstChildElementValue(&memberElement, "uri");
    //info.Sdp = GetFirstChildElementValue(&memberElement, "sdp");
    //info.isReceived = 0;
    //info.hand = 0;
    //info.isMulticast = true;
    //if(confmode == "host" && info.MemberURI == hosturi)
    //{
    //	info.permitSpeak = 2;
    //}
    //else
    //{
    //	string floor = GetFirstChildElementValue(&memberElement, "floor");
    //	if(floor == "true")
    //		info.permitSpeak = 1;
    //	else
    //		info.permitSpeak = 0;
    //}
    //info.multicastAddress = GetFirstChildElementValue(&memberElement,"multicastAddress");
    //info.videoPort = atoi(GetFirstChildElementValue(&memberElement, "videoPort").c_str());
    //info.audioPort = atoi(GetFirstChildElementValue(&memberElement, "audioPort").c_str());

    info.MemberURI = GetFirstChildElementValue(&memberElement, "uri");
    info.username =  GetFirstChildElementValue(&memberElement, "username");
    info.name =  GetFirstChildElementValue(&memberElement, "name");
    info.sdp = GetFirstChildElementValue(&memberElement, "sdp");
    info.company = GetFirstChildElementValue(&memberElement, "company");
    info.floor = GetFirstChildElementValue(&memberElement, "floor" );
    info.location = GetFirstChildElementValue(&memberElement, "location");
    info.multicastAddress = GetFirstChildElementValue(&memberElement, "multicastAddress");
    info.videoPort = atoi(GetFirstChildElementValue(&memberElement, "videoPort").c_str());
    info.audioPort = atoi(GetFirstChildElementValue(&memberElement, "audioPort").c_str());
    info.status =  GetFirstChildElementValue(&memberElement, "status");
    info.xmpp =  GetFirstChildElementValue(&memberElement, "xmpp");

    string allowTerminals = "";
	TiXmlElement* terminal = memberElement.FirstChildElement("terminals");
	if(terminal)
	{
		TiXmlElement* pUriElement = terminal->FirstChildElement( "uri" );
		for(pUriElement; pUriElement != NULL; )
		{
		    allowTerminals += pUriElement->FirstChild()->ValueStr();
		    if( (pUriElement = pUriElement->NextSiblingElement()) != NULL )
		    {
		        allowTerminals.append( "\t" );
		    }
		}
	}

    info.AllowTerminal = allowTerminals;

    return 0;
}

void CVMsgParser::ParseSingleMemberElement( TiXmlElement& memberElement, QVariantMap* member, const string& hosturi, const string& confmode )
{

    QString uri = QString::fromStdString(GetFirstChildElementValue(&memberElement, "uri"));
    QString sdp =  QString::fromStdString(GetFirstChildElementValue(&memberElement, "sdp"));
    QString floor = QString::fromStdString(GetFirstChildElementValue(&memberElement, "floor"));
    QString multicastAddress = QString::fromStdString(GetFirstChildElementValue(&memberElement, "multicastAddress"));
    QString videoPort = QString::fromStdString(GetFirstChildElementValue(&memberElement, "videoPort"));
    QString audioPort = QString::fromStdString(GetFirstChildElementValue(&memberElement, "audioPort"));

    member->insert("uri", uri);
    member->insert("sdp", sdp);
    member->insert("floor", floor);
    member->insert("multicastAddress", multicastAddress);
    member->insert("videoPort", videoPort);
    member->insert("audioPort", audioPort);

}

void CVMsgParser::ReleaseConfList(vector<ConfInfo*>& conf_list)
{
    vector<ConfInfo*>::iterator iter = conf_list.begin();
    for(iter; iter != conf_list.end(); iter++)
    {
        ConfInfo* c = (ConfInfo*)(*iter);
        if( c )
        {
            delete c;
            c = NULL;
        }
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

void CVMsgParser::ReleaseTerminalList( TerminalVector& terminalList )
{
    TerminalVector::iterator iter = terminalList.begin();
    for(iter; iter != terminalList.end(); iter++)
    {
        TerminalInfo* terminal = (TerminalInfo*)(*iter);
        if( terminal )
        {
            delete terminal;
            terminal = NULL;
        }
    }
    terminalList.clear();
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

string CVMsgParser::GetTerminalUri()
{
	return GetFirstChildElementValue(_coolviewElement, "terminalUri");
}

string CVMsgParser::GetVirtualUri()
{
	return GetFirstChildElementValue(_coolviewElement, "virtualUri");
}


std::string CVMsgParser::GetPermission()
{
    return GetFirstChildElementValue(_coolviewElement, "permission");
}

std::string CVMsgParser::GetEnable()
{
	return GetFirstChildElementValue(_coolviewElement , "enable" );
}

std::string CVMsgParser::GetType()
{
	return GetFirstChildElementValue(_coolviewElement , "type" );
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

string CVMsgParser:: GetResult()
{
  return GetFirstChildElementValue(_coolviewElement,"result");
}

string CVMsgParser:: GetTerminalCount()
{
	return GetFirstChildElementValue(_coolviewElement, "count");
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

int CVMsgParser::ParseSingleTerminalElement( TiXmlElement& terminalElement, TerminalInfo& info, const string& hosturi, const string& confmode )
{
	//注意：TinyXML大小写敏感
    info.uri = GetFirstChildElementValue(&terminalElement, "uri");

	string::size_type startIndex;
	if( ( startIndex = info.uri.find("@") )!=string::npos ) 
	{
		//更换当前的域名
		const string username = info.uri.substr( 0, startIndex );
		if (username == RunningProfile::getInstance()->username())
		{
			RunningProfile::getInstance()->set_user_uri( info.uri );
		}
	}
	info.virtualUri = GetFirstChildElementValue(&terminalElement , "virtualuri");
	if (info.virtualUri.empty())
	{
		info.virtualUri = "v1#";
		info.virtualUri += info.uri;
	}
	info.virtualCount = atoi(GetFirstChildElementValue(&terminalElement, "virtualTerminalCount").c_str());
	if (0 >= info.virtualCount)
	{
		info.virtualCount = 1;
	}
    info.name = GetFirstChildElementValue(&terminalElement , "name");
    info.dns = GetFirstChildElementValue(&terminalElement, "dns");
    info.model = GetFirstChildElementValue(&terminalElement, "model");
    info.version = GetFirstChildElementValue(&terminalElement , "version");
    info.sdp = GetFirstChildElementValue(&terminalElement, "sdp");
    info.multicastAddress = GetFirstChildElementValue(&terminalElement, "multicastAddress");
    info.videoPort = atoi(GetFirstChildElementValue(&terminalElement, "videoPort").c_str());
    info.audioPort = atoi(GetFirstChildElementValue(&terminalElement, "audioPort").c_str());
	info.smallVideoPort = atoi(GetFirstChildElementValue(&terminalElement,"smallVideoPort").c_str());
    info.status = GetFirstChildElementValue(&terminalElement , "status");
    info.location = GetFirstChildElementValue(&terminalElement , "location");
	info.isSpeaking = GetFirstChildElementValue(&terminalElement, "isSpeaking");
	info.isHandUp = GetFirstChildElementValue(&terminalElement,"isHandUp");
	info.isChairmanTerminal = GetFirstChildElementValue(&terminalElement,"isChairmanTerminal");
	info.isMainSpeaker = GetFirstChildElementValue(&terminalElement,"isSpeakerTerminal");
	info.totalTermialNum = atoi(GetFirstChildElementValue(&terminalElement, "count").c_str());
	
	int nSharpPos = info.virtualUri.find("#");
	if (std::string::npos != nSharpPos)
	{
		std::string indexStr = info.virtualUri.substr(1, nSharpPos - 1);
		info.virtualIndex = atoi(indexStr.c_str()) - 1; // 跳过首个v
		if (0 < info.virtualIndex)
		{
			info.name += "(" + indexStr + ")";
		}
	}
	else
	{
		info.virtualIndex = 0;
	}

    //if(confmode == "host" && info.MemberURI == hosturi)
    //{
    //	info.permitSpeak = 2;
    //}
    //else
    //{
    //	string floor = GetFirstChildElementValue(&memberElement, "floor");
    //	if(floor == "true")
    //		info.permitSpeak = 1;
    //	else
    //		info.permitSpeak = 0;
    //}
    return 0;
}

//#define MAKE_VIRTUAL_TERMINAL

///将sip消息内容解析为终端列表
TerminalVector CVMsgParser::ParseTerminalListMsg( TerminalVector& terminalList , const string& hosturi , const string& confmode )
{
    TiXmlElement* pTerminalElement = _coolviewElement->FirstChildElement("terminals")->FirstChildElement("terminal");
    for(pTerminalElement; pTerminalElement != NULL; pTerminalElement = pTerminalElement->NextSiblingElement())
    {
        TerminalInfo* terminal = new TerminalInfo();
        ParseSingleTerminalElement(*pTerminalElement , *terminal, hosturi , confmode );

#ifdef MAKE_VIRTUAL_TERMINAL
		int numOfStreams = 2;
		//暂时只设两种类型。具体类型明明规则以后商讨 2013.7.22
		/*if(terminal->model == "CoolView HD") numOfStreams = 1;
		else if(terminal->model == "CoolView HD3") numOfStreams = 3;*/

		for (int i = 0; i < numOfStreams; i++)
		{
			TerminalInfo* tempTerminal = new TerminalInfo;
			*tempTerminal = *terminal;
			char s[5];
			itoa(i + 1,s,10);

			tempTerminal->name = (tempTerminal->name) + "(" + s + ")";								//终端名后添加index来区分不同的摄像头
			tempTerminal->virtualUri = std::string("v") + s + "#" + tempTerminal->uri;
			tempTerminal->videoPort = tempTerminal->videoPort + 2000 * i;							//为每个摄像头的视频分配不同的端口号，按2000递增
			tempTerminal->audioPort = tempTerminal->audioPort + 2000 * i;							//为每个摄像头的视频分配不同的端口号，按200递增
			tempTerminal->smallVideoPort = tempTerminal->smallVideoPort + 2000 * i;					//为每个摄像头的视频分配不同的端口号，按200递增
			tempTerminal->virtualIndex = i;

			terminalList.push_back(tempTerminal);
		}

		delete terminal;
		terminal = NULL;
#else	
		//判断成员列表里是否存在相对应的成员
		bool alreadyExist = false;
		for( TerminalVector::iterator it = terminalList.begin() ; it != terminalList.end() ; it++ )
		{
			if( (*it)->virtualUri == terminal->virtualUri )
			{
				//if( (*it)->uri==RunningProfile::getInstance()->user_uri() )
				//{
				//	//如果该终端与当前终端的Sip账号是一样的，那么应该更新该终端信息（重新登录会议导致端口更改）
				//	TerminalInfo* currentTerminal = (*it);
				//	*currentTerminal = *tempTerminal;
				//}
				delete terminal;
				terminal = NULL;
				alreadyExist = true;
				break;
			}
		}
		if( !alreadyExist )
		{
			terminalList.push_back(terminal);
		}
#endif
    }
    return terminalList;
}


int CVMsgParser::GetLoginNotifyType()
{
    TiXmlElement* pMemberElement = _coolviewElement->FirstChildElement("member");
    if( pMemberElement != NULL )
        return 0;

    TiXmlElement* pTerminalElement = _coolviewElement->FirstChildElement("terminal");
    if( pTerminalElement != NULL )
        return 1;

    return -1;
}


MemberInfo* CVMsgParser::ParseMemberLoginNotifyMsg( MemberVector&memberList , std::string& hostUri , std::string& confMode )
{
    TiXmlElement* pMemberElement = _coolviewElement->FirstChildElement("member");
    if( pMemberElement != NULL )
    {
        MemberInfo* member = new MemberInfo();
        ParseSingleMemberElement( *pMemberElement , *member , hostUri , confMode );
        //判断成员列表里是否存在相对应的成员
        for( vector<MemberInfo*>::iterator it = memberList.begin() ; it != memberList.end() ; it++ )
        {
            MemberInfo* memberElement = (*it);
            if( memberElement->MemberURI == member->MemberURI )
            {
                //更新member状态
                *memberElement = *member;
                delete member;
                member = NULL;
                return memberElement;
            }
        }
        memberList.push_back(member);
        return member;
    }
    return NULL;
}

TerminalVector &CVMsgParser::ParseTerminalLoginNotifyMsg( TerminalVector &terminalList, std::string& hostUri , std::string& confMode )
{
    TiXmlElement* pTerminalElement = _coolviewElement->FirstChildElement("terminal");
    for(pTerminalElement; pTerminalElement != NULL; pTerminalElement = pTerminalElement->NextSiblingElement())
    {
        TerminalInfo* terminal = new TerminalInfo();
        ParseSingleTerminalElement( *pTerminalElement , *terminal , hostUri , confMode );

        //判断成员列表里是否存在相对应的成员
        bool alreadyExist = false;
        for( TerminalVector::iterator it = terminalList.begin() ; it != terminalList.end() ; it++ )
        {
            TerminalInfo* terminalElement = (*it);
            if( terminalElement->virtualUri == terminal->virtualUri )
            {
                //更新terminal状态
                *terminalElement = *terminal;
				delete terminal;
				alreadyExist = true;
                terminal = NULL;
            }
        }

        //如果不存在该终端则加入终端列表
		if (!alreadyExist)
			terminalList.push_back(terminal);
    }
    return terminalList;
}

int CVMsgParser::ParseQoSServerInfoMsg( MsgQoSServerInfo& info )
{
    info.sipUri = GetFirstChildElementValue(_coolviewElement, "sipUri");
    info.ip  = GetFirstChildElementValue(_coolviewElement, "ip");
    info.tcpPort = atoi(GetFirstChildElementValue(_coolviewElement, "simulatetestTCPPort").c_str());
    info.simulateTestUdpPort = atoi(GetFirstChildElementValue(_coolviewElement, "simulatetestUDPPort").c_str());
	info.operationUdpPort = atoi(GetFirstChildElementValue(_coolviewElement, "operationUDPPort").c_str());
    return 0;
}


const char * CVMsgParser::getMessageContent()
{
    return _content.c_str();
}

std::string CVMsgParser::GetTerminalConfMode()
{
	return GetFirstChildElementValue(_coolviewElement, "terminalConfMode");
}