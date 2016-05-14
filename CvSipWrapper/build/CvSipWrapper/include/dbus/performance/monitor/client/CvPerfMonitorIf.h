/*
 * This file was generated by qdbusxml2cpp version 0.7
 * Command line was: qdbusxml2cpp -v -c CvPerfMonitorIf -p CvPerfMonitorIf.h:CvPerfMonitorIf.cpp .\com.dcampus.coolview.performance.monitor.xml
 *
 * qdbusxml2cpp is Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#ifndef CVPERFMONITORIF_H_1297761393
#define CVPERFMONITORIF_H_1297761393

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>

/*
 * Proxy class for interface com.dcampus.coolview.performance.monitor
 */
class CvPerfMonitorIf: public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
    { return "com.dcampus.coolview.performance.monitor"; }

public:
    CvPerfMonitorIf(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = 0);

    ~CvPerfMonitorIf();

public Q_SLOTS: // METHODS
    inline QDBusReply<void> HelloInfo(int processID, const QByteArray &processImage)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(processID) << qVariantFromValue(processImage);
        return callWithArgumentList( QDBus::NoBlock, QLatin1String("HelloInfo"), argumentList);
    }

    inline QDBusReply<void> RegisterProcess(const QByteArray &processInfo)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(processInfo);
        return callWithArgumentList( QDBus::NoBlock, QLatin1String("RegisterProcess"), argumentList);
    }

    inline QDBusReply<void> UnregisterProcess(int processID)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(processID);
        return callWithArgumentList( QDBus::NoBlock, QLatin1String("UnregisterProcess"), argumentList);
    }

Q_SIGNALS: // SIGNALS
};

namespace com {
  namespace dcampus {
    namespace coolview {
      namespace performance {
        typedef ::CvPerfMonitorIf monitor;
      }
    }
  }
}
#endif