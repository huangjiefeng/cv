#ifndef REMOTE_RECEIVER_H
#define REMOTE_RECEIVER_H

#include <string>
#include <map>
#include <vector>


//记录发送媒体流的目标成员的信息
struct ReceiverInfo
{
	std::string MemberURI;   //成员uri
	std::string recvIP;		 //成员的IP地址
	int videoPort;		//视频数据传输端口
	int audioPort;		//音频数据传输端口
	bool smallVideo;    //是否小流
	int virtualIndex;   //本地发送进程的虚拟终索引

	ReceiverInfo():
		videoPort(0),
		audioPort(0),
		smallVideo(false),
		virtualIndex(0)
	{}
};


class ReceiverInfoManager
{
	typedef std::map<int, ReceiverInfo *> VirtualReceiverMap;
	typedef std::map<std::string, VirtualReceiverMap> RemoteReceiverMap;
	RemoteReceiverMap _map;

public:

	/**
	 * @brief 添加一个新的远程接收端信息
	 * @param virtualIndex 本地发送流虚拟终端索引
	 * @param sipUri 远程终端Uri
	 * @param remoteIPAddr 远程终端IP
	 * @param viedoPort 目标视频端口
	 * @param audioPort 目标音频端口
	 * @param smallVideo 是否为小流
	 */
	void AddRemoteReceiver( const int &virtualIndex, const std::string& sipURI, const std::string&remoteIPAddr, 
		const int&videoPort, const int&audioPort, const bool smallVideo)
	{
		//首先查找是否存在对应的接收端
		ReceiverInfo* receiverInfo = FindRemoteReceiverByURI( virtualIndex, sipURI );
		if( receiverInfo != NULL )
		{
			//如果存在对应的接收端，那么更新它的参数
			receiverInfo->recvIP = remoteIPAddr;
			receiverInfo->videoPort = videoPort;
			receiverInfo->audioPort = audioPort;
			receiverInfo->smallVideo = smallVideo;
		}
		else
		{
			//如果不存在对应的接收端，那么新建一个
			receiverInfo = new ReceiverInfo();
			receiverInfo->MemberURI = sipURI;
			receiverInfo->recvIP = remoteIPAddr;
			receiverInfo->videoPort = videoPort;
			receiverInfo->audioPort = audioPort;
			receiverInfo->smallVideo = smallVideo;
			receiverInfo->virtualIndex = virtualIndex;
			_map[sipURI][virtualIndex] = receiverInfo;
		}
		
	}

	/**
	 * @brief 删除接收本终端的远程终端的信息
	 */
	void RemoveRemoteReceiverByURI( const int virtualIndex, const std::string&sipURI )
	{
		if (_map.find(sipURI) == _map.end())
		{
			return;
		}
		VirtualReceiverMap::iterator it = _map[sipURI].find(virtualIndex);
		if (it != _map[sipURI].end())
		{
			delete it->second;
			_map[sipURI].erase(it);
		}
	}

	/**
	 * @brief 删除接收本终端的远程终端的信息。注意，当存在多个相同IP的接收者信息时，删除行为不确定
	 */
	void RemoveRemoteReceiverByIP( const int virtualIndex, const std::string&ip )
	{
		for(RemoteReceiverMap::iterator it = _map.begin(); it != _map.end(); ++it)
		{
			if (it->second.size() == 0)
			{
				continue;
			}
			VirtualReceiverMap::iterator it2 = it->second.find(virtualIndex);
			if( it2 != it->second.end() && (it2->second)->recvIP == ip)
			{
				delete it2->second;
				it->second.erase(it2);
				return;
			}
		}
	}

	/**
	 * @brief 根据sipURI查找接收当前终端的远程终端的信息
	 * @param receiverList 接收终端列表
	 * @param sipURI 查找的SIP URI
	 * @return 如果找到了则返回相关的信息对象，否则返回空指针
	 */
	ReceiverInfo* FindRemoteReceiverByURI( const int virtualIndex, const std::string&sipURI )
	{
		if (_map.find(sipURI) == _map.end())
		{
			return NULL;
		}
		VirtualReceiverMap::iterator it = _map[sipURI].find(virtualIndex);
		if (it == _map[sipURI].end())
		{
			return NULL;
		}
		return (it->second);
	}


	/**
	 * @brief 根据远程IP查找接收当前终端的远程终端的信息
	 * @param receiverList 接收终端列表
	 * @param ip 查找的远程终端IP地址
	 * @return 如果找到了则返回相关的信息对象，否则返回空指针
	 */
	ReceiverInfo * FindRemoteReceiverByIP( const int virtualIndex, const std::string&ip )
	{
		for(RemoteReceiverMap::iterator it = _map.begin(); it != _map.end(); ++it)
		{
			if (it->second.size() == 0)
			{
				continue;
			}
			VirtualReceiverMap::iterator it2 = it->second.find(virtualIndex);
			if( it2 != it->second.end() && it2->second->recvIP == ip)
			{
				return (it2->second);
			}
		}
		return NULL;
	}

	int ReceiverCount() const
	{
		int count = 0;
		for(RemoteReceiverMap::const_iterator it = _map.cbegin(); it != _map.cend(); ++it)
		{
			if (it->second.size() > 0)
			{
				++count;
			}
		}
		return count;
	}

	typedef std::vector<ReceiverInfo> InfoList;
	InfoList ToInfoList() const
	{
		InfoList list;
		for(RemoteReceiverMap::const_iterator it = _map.cbegin(); it != _map.cend(); ++it)
		{
			for (VirtualReceiverMap::const_iterator it2 = it->second.cbegin();
				 it2 != it->second.cend(); ++it2)
			{
				list.push_back(*(it2->second));
			}
		}
		return list;
	}

};


#endif