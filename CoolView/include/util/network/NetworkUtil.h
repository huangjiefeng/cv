////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file	network\NetworkUtil.h
///
/// @brief 定义一些网络API
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <string>
#include <vector>
using namespace std;

class NetworkUtil
{
public:
	/**
	 * @brief  根据主机IP获取网关地址，保存到参数pGateway中，pGateway的长度至少为16.
	 * @param pHostIP		主机IP地址
	 * @param pGateway	网关地址输出字符串
	 * @return  返回是否成功获取网关地址
	 */
	static bool getGatewayByHostIP(const char* pHostIP, char * pGateway);

	/**
	 * @brief 获取主机IP地址列表
	 * @param 地址版本 AF_INET|AF_INET6
	 */
	static bool getIPList(int family , vector<string>* ipList );

	/**
	 * @brief 获取主机的所有IP地址
	 */
	static vector<string> getHostIPList();
};
