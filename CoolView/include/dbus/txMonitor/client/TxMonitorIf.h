/*
 * This file was generated by qdbusxml2cpp version 0.8
 * Command line was: qdbusxml2cpp -v -c TxMonitorIf -p TxMonitorIf.h:TxMonitorIf.cpp .\com.dcampus.coolview.txMonitor.xml
 *
 * qdbusxml2cpp is Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#ifndef TXMONITORIF_H_1426323560
#define TXMONITORIF_H_1426323560

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>

/*
 * Proxy class for interface com.dcampus.coolview.txMonitor
 */
class TxMonitorIf: public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
    { return "com.dcampus.coolview.txMonitor"; }

public:
    TxMonitorIf(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = 0);

    ~TxMonitorIf();

public Q_SLOTS: // METHODS
    inline QDBusPendingReply<> ReportRecStat(int stat_type, const QByteArray &data_garray)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(stat_type) << QVariant::fromValue(data_garray);
        return asyncCallWithArgumentList(QLatin1String("ReportRecStat"), argumentList);
    }

Q_SIGNALS: // SIGNALS
};

namespace com {
  namespace dcampus {
    namespace coolview {
      typedef ::TxMonitorIf txMonitor;
    }
  }
}
#endif