#pragma once

class QStringList;
class NetworkSetting
{
public:
	NetworkSetting(void);
	~NetworkSetting(void);
	QStringList getIPList(int family);
};
