#ifndef XML_CONFIGURE_MANAGER_H
#define XML_CONFIGURE_MANAGER_H

#include "configure_manager_interface.h"

#include <QHash>

class XMLConfigureManager : public IConfigureManager {
 protected:
  XMLConfigureManager();
  ~XMLConfigureManager();

 private:
  typedef QHash<QString, QVariant> Dictionary;
  Dictionary settings_;
};

#endif // XML_CONFIGURE_MANAGER_H
