#ifndef HARDWARE_INFO_DESCRIPTION_H
#define HARDWARE_INFO_DESCRIPTION_H

#include <QList>

struct HDDInfo {
  float temperature;
  float used_space;
};

struct HardwareInfo {
  float mainboard_temperature;
  float cpu_temperature;
  float cpu_load;
  float ram_available_mem;
  QList<HDDInfo> hdd;
};

#endif // HARDWARE_INFO_DESCRIPTION_H