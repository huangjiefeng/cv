#ifndef CONFIG_DESCRIPTION_H
#define CONFIG_DESCRIPTION_H

#include <QHash>
#include <QString>
#include <QVariant>

typedef QHash<QString, QVariant> ConfigDict;

struct ConfigRegister
{
  static void RegisterMetaType() 
  {
    qRegisterMetaType<ConfigDict>("ConfigDict");
  }
};

#endif // !CONFIG_DESCRIPTION_H
