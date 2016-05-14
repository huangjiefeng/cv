#ifndef CONFIGURE_MANAGER_INTERFACE_H
#define CONFIGURE_MANAGER_INTERFACE_H

#include <QString>
#include <QVariant>

class IConfigureManager {
 public:
  virtual void SetValue(const QString &name, const QString &value) = 0;
  virtual QString GetValue(const QString &name, const QString &default_value) = 0;

  virtual void SetValue(const QString &name, bool value) = 0;
  virtual bool GetValue(const QString &name, bool default_value) = 0;

  virtual void SetValue(const QString &name, float value) = 0;
  virtual float GetValue(const QString &name, float default_value) = 0;

  virtual void SetValue(const QString &name, int value) = 0;
  virtual int GetValue(const QString &name, int default_name) = 0;

  virtual void SetValue(const QString &name, const QVariant &value) = 0;
  virtual QVariant GetValue(const QString &name, const QVariant &default_value) = 0;

 protected:
  IConfigureManager() {}
  virtual ~IConfigureManager() {}
 
 private:
  IConfigureManager(const IConfigureManager&);
  IConfigureManager& operator=(const IConfigureManager&);
};

#endif // !CONFIGURE_MANAGER_INTERFACE_H
