#pragma once

#define TXCONTROLLER_SERVICE_NAME			"com.dcampus.coolview.controller.txcontroller"
#define TXCONTROLLER_SERVICE_OBJECT_PATH	"/com/dcampus/coolview/controller/txcontroller"

#include <QtCore/QString>
#include <QtCore/QDataStream>
#include <QtCore/QStringList>
#include <QtCore/QVector>


enum RecordOperation
{
	TXRecordOp_Start = 0,
	TXRecordOp_Stop = 1,
};

//操作错误码 - 用于DBus和WebService两端检测错误
const int TXRecord_Success = 0;
const int TXRecord_Error = 1; //一般错误
const int TXRecord_UriNotExist = 2;
const int TXRecord_IsRecording = 3;
const int TXRecord_IsNotRecording = 4;
const int TXRecord_NotInFocus = 5;

//终端录制状态
#define TxRecordStatus_Offline "offline"
#define TxRecordStatus_Online "online"
#define TxRecordStatus_Recording "recording"

//StartRecordParam
//该参数在普通终端中，用于ConfRoom向CoolView发送录制控制参数；
//在TX中，用于TxMonitor的SOAP组件向CoolView中的TxController发送录制控制指令
class StartRecordParam
{
public:
	QString focus; //要录制的终端所在focus
  QString requester_uri; //录像发起者uri
	QString target_uri; //要录制的终端uri
  QString title; //录像视频集标题（可选）
  QString participants; //参会者（可选）
  QString keywords; //关键词（可选）
  QString description; //描述（可选）

	friend QDataStream &operator>>(QDataStream &in, StartRecordParam &param)
	{
		in >> param.focus >> param.requester_uri >> param.target_uri 
      >> param.title >> param.participants >> param.keywords >> param.description;
		return in;
	}
	friend QDataStream &operator<<(QDataStream &out, StartRecordParam &param)
	{
    out << param.focus << param.requester_uri << param.target_uri 
      << param.title << param.participants << param.keywords << param.description;
		return out;
	}
};

class StopRecordParam
{
public:
  QString focus; //要录制的终端所在focus
  QString target_uri; //要录制的终端uri

  friend QDataStream &operator>>(QDataStream &in, StopRecordParam &param)
  {
    in >> param.focus >> param.target_uri;
    return in;
  }
  friend QDataStream &operator<<(QDataStream &out, StopRecordParam &param)
  {
    out << param.focus << param.target_uri;
    return out;
  }
};

class RecordTerminalStatus
{
public:
  QString vuri;
  QString name;
  QString status;
  int virtual_count; // 该终端总的虚拟终端数
  QString requester_uri;
  unsigned int duration;
  unsigned long long recorded_bytes;

  RecordTerminalStatus() :
    virtual_count(0),
    duration(0),
    recorded_bytes(0) {}

  friend QDataStream &operator>>(QDataStream &in, RecordTerminalStatus &param)
  {
    in >> param.vuri >> param.name >> param.status >> param.virtual_count >> 
      param.requester_uri >> param.duration >> param.recorded_bytes;
    return in;
  }
  friend QDataStream &operator<<(QDataStream &out, RecordTerminalStatus &param)
  {
    out << param.vuri << param.name << param.status << param.virtual_count <<
      param.requester_uri << param.duration << param.recorded_bytes;
    return out;
  }
};

class RecordFocusStatus
{
public:
  QVector<RecordTerminalStatus> terminals_status;

  friend QDataStream &operator>>(QDataStream &in, RecordFocusStatus &param)
  {
    int len;
    in >> len;
    for (int i = 0; i < len; ++i) {
      RecordTerminalStatus status;
      in >> status;
      param.terminals_status.push_back(status);
    }
    return in;
  }
  friend QDataStream &operator<<(QDataStream &out, RecordFocusStatus &param)
  {
    out << param.terminals_status.size();
    for (auto t : param.terminals_status) {
      out << t;
    }
    return out;
  }
};
