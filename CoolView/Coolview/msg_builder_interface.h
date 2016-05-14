#ifndef MSG_BUILDER_INTERFACE_H
#define MSG_BUILDER_INTERFACE_H

#include <QString>

class IMsgBuilder {
public:
  IMsgBuilder(const QString &name, const QString &type) {}
  virtual ~IMsgBuilder() {}

  virtual void AddArgument(const QString &name, const QString &value) = 0;
  virtual void AddArgument(const QString &name, bool value) = 0;
  virtual void AddArgument(const QString &name, int value) = 0;
  virtual QString GetMsg() const = 0;
};

#endif // MSG_BUILDER_INTERFACE_H