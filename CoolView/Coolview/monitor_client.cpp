#include "monitor_client.h"

#include <QDomDocument>
#include <QDomElement>
#include <QDebug>

#include "monitor_soap.nsmap"
#include "monitor_soapBasicHttpBinding_USCOREIMonitorServiceProxy.h"

using namespace monitor_soap;

MonitorClient::MonitorClient(QObject *parent)
    : QObject(parent), proxy_(nullptr) {
  static RegisterMetaTypeHelper register_helper;

  proxy_ = new(std::nothrow) BasicHttpBinding_USCOREIMonitorServiceProxy();
  if (proxy_) {
    // 设置编码格式为utf-8
    soap_set_mode(proxy_, SOAP_C_UTFSTRING);
    // 设置soap终端为本地终端
    proxy_->soap_endpoint = "http://127.0.0.1:8010/MonitorService/";
  }
}

MonitorClient::~MonitorClient() {
  if (proxy_) {
    delete proxy_;
    proxy_ = nullptr;
  }
}

void MonitorClient::HandleRequestHardwareInfoSlot() {
  if (nullptr == proxy_) return;
  _ns1__GetHardwareInfo hardware_info;
  _ns1__GetHardwareInfoResponse response;
  if(proxy_->GetHardwareInfo(&hardware_info, &response) == SOAP_OK) {
    QDomDocument doc;
    if (!doc.setContent(QString(response.GetHardwareInfoResult))) {
      qDebug() << "MonitorClient: XML Content Error";
      return;
    }
    HardwareInfo info;
    ParseHardwareInfoFromXML(doc.documentElement(), info);
    emit NotifyHardwareInfoSignal(info);
  } else {
    //qDebug() << "MonitorClient: Soap Error " << proxy_->error;
  }
  // TODO: 还未确认是否需要free掉result的内存
  free(response.GetHardwareInfoResult);
}

void MonitorClient::ParseHardwareInfoFromXML(
    const QDomElement &root, HardwareInfo &info) {
  QString tagname = root.tagName();
  QDomElement main_board = root.firstChildElement("mainboard");
  info.mainboard_temperature = 
    FindMinValue(main_board.firstChildElement("superio"), "temperatures");

  QDomElement cpu = root.firstChildElement("cpu");
  info.cpu_temperature = FindMinValue(cpu, "temperatures");
  info.cpu_load = FindSensorValue(cpu.firstChildElement("load"), "CPU Total");
  
  QDomElement ram = root.firstChildElement("ram");
  info.ram_available_mem = 
    FindSensorValue(ram.firstChildElement("data"), "Available Memory");
  
  QDomNodeList hdds = root.elementsByTagName("hdd");
  const int size = hdds.size();
  for (int i = 0; i < size; ++i) {
    QDomNode node = hdds.at(i);
    if (node.isNull()) {
      continue;
    }
    QDomElement hdd = node.toElement();
    if (!hdd.isNull()) {
      HDDInfo hdd_info;
      hdd_info.temperature = FindSensorValue(
        hdd.firstChildElement("temperatures"), "Temperature");
      hdd_info.used_space = FindSensorValue(
        hdd.firstChildElement("load"), "Used Space");
      info.hdd.push_back(hdd_info);
    }
  }
}

float MonitorClient::FindMinValue(
    const QDomElement &element, const QString &tag_name) {
  if (element.isNull()) {
    return std::numeric_limits<float>::quiet_NaN();
  }

  QDomElement tag_node = element.firstChildElement(tag_name);
  if (tag_node.isNull()) {
    return std::numeric_limits<float>::quiet_NaN();
  }
  
  QDomNodeList sensors = tag_node.elementsByTagName("sensor");
  float min_value = std::numeric_limits<float>::quiet_NaN();
  const int size = sensors.size();
  bool ok = true;
  for (int i = 0; i < size; ++i) {
    QDomElement sensor = sensors.at(i).toElement();
    if (sensor.isNull()) {
      continue;
    }
    QString value_attr = sensor.attribute("value");
    float value = value_attr.left(value_attr.indexOf(' ')).toFloat(&ok);
    if (ok && (min_value != min_value || value < min_value)) {
      min_value = value;
    }
  }
  return min_value;
}

float MonitorClient::FindSensorValue(
    const QDomElement &element, const QString &name) {
  if (element.isNull()) {
    return std::numeric_limits<float>::quiet_NaN();
  }
  QDomNodeList sensors = element.elementsByTagName("sensor");
  const int size = sensors.size();
  bool ok = true;
  for (int i = 0; i < size; ++i) {
    QDomElement sensor = sensors.at(i).toElement();
    if (sensor.isNull()) {
      continue;
    }
    
    QString sensor_name = sensor.attribute("name");
    if (sensor_name == name) {
      QString value_attr = sensor.attribute("value");
      float value = value_attr.left(value_attr.indexOf(' ')).toFloat(&ok);
      if (ok) {
        return value;
      }
    }
  }
  return std::numeric_limits<float>::quiet_NaN();
}

void MonitorClient::HandleRequestBeepForAWhileSlot( int ms ) {
  if (proxy_ == nullptr) return;
  _ns1__BeepForAWhile arg;
  arg.ms = &ms;
  proxy_->BeepForAWhile(&arg);
}

