#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <functional>
#include <QHash>

#include <QString>
#include <QVariant>

#include "config_description.h"

class ConfigManager
{
public:
  static ConfigManager& instance();

  bool Update(const ConfigDict &dict);
  ConfigDict Dump() const;
  void SaveToConfigFiles();

private:
  ConfigManager();
  ~ConfigManager();

  // non-copyable
  ConfigManager(const ConfigManager&);
  ConfigManager& operator=(const ConfigManager&);

private:
  typedef std::function<bool(QVariant)> SettingFunc;
  typedef std::function<QVariant(void)> GettingFunc;
  typedef std::pair<SettingFunc, GettingFunc> FuncPair;

  QHash<QString, FuncPair> func_pairs_;
};



#endif //CONFIG_MANAGER_H