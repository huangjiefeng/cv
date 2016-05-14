#ifndef QOS_DESCRIPTION_H
#define QOS_DESCRIPTION_H

#include <QString>

// 允许加入会议消息的信息内容
struct LoginPermissionInfo {
  QString user_uri;
  int dscp;
  int rate;
};

// 禁止加入会议消息的信息内容
struct LoginRejectionInfo {
  QString user_uri;
  QString description;
  int suggested_rate;
};

// QoS服务器信息
struct QoSServerInfo {
  QString sip_uri;	
  QString ip;
  int tcp_port;
  int simulate_test_udp_port; // QoS服务器接收模拟测试报告端口
  int operation_udp_port;     // QoS服务器接收报告端口
};

#endif // QOS_DESCRIPTION_H