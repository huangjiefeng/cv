////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file	util\NetworkUtil.h
///
/// @brief 定义一些网络API
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <string>
#include <vector>
#include <QStringList>

/**
 * @brief  根据主机IP获取网关地址，保存到参数pGateway中，pGateway的长度至少为16.
 * @param pHostIP		主机IP地址
 * @param pGateway	网关地址输出字符串
 * @return  返回是否成功获取网关地址
 */
extern bool GetGatewayByHostIP(const char* pHostIP, char * pGateway);

/**
 * @brief 获取主机IP地址列表
 * @param 地址版本 AF_INET|AF_INET6
 */
extern bool GetIPList(int family , std::vector<std::string>* ipList );

/**
 * @brief 判断地址是否为IPv6地址
 * @param ip ip地址
 */
extern bool IsIpv6Address(const std::string &ip);

/**
 * @brief 判断地址是否为IPv4地址
 * @param ip ip地址
 */
extern bool IsIpv4Address(const std::string &ip);

/**
 * @brief 比较两个IP地址，相同返回true，否则返回false
 * @param address1 ip地址1
 * @param address2 ip地址2
 */
extern bool IsIPAddressEqual(const std::string &address1, const std::string &address2);

/**
 * @brief 获取主机ip地址，包括ipv4和ipv6的地址
 * @return 返回地址列表
 */
extern QStringList GetHostIPList();

/**
 * @brief 获取主机ip地址，包括ipv4和ipv6的地址
 * @param ipv4 ipv4地址
 * @param ipv6 ipv6地址
 */
extern void GetHostIPs(QString &ipv4, QString &ipv6);

/**
 * @brief 获取ip地址所对应的MAC地址
 * @return 返回MAC地址
 */
extern QString GetMacAddress( const QString &ip_address );
