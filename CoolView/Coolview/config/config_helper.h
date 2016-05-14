#pragma once

#include <QStringList>

class ConfigHelper
{
private:
  ConfigHelper(void);
  ~ConfigHelper(void);

public:
  static QStringList GetUIAudioInputTypeOptions();
  static bool GetAudioInputTypeByUIOption(const QString& option, int &config);
};

