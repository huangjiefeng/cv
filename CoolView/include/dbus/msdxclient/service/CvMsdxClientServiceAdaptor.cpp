/*
 * This file was generated by qdbusxml2cpp version 0.7
 * Command line was: qdbusxml2cpp -c CvMsdxClientServiceAdaptor -a CvMsdxClientServiceAdaptor.h:CvMsdxClientServiceAdaptor.cpp .\com.coolview.cvmsdxclient.xml
 *
 * qdbusxml2cpp is Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */
#include "stdafx.h"
#include "CvMsdxClientServiceAdaptor.h"
#include <QtCore/QMetaObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

/*
 * Implementation of adaptor class CvMsdxClientServiceAdaptor
 */

CvMsdxClientServiceAdaptor::CvMsdxClientServiceAdaptor(QObject *parent)
    : QDBusAbstractAdaptor(parent)
{
    // constructor
    setAutoRelaySignals(true);
}

CvMsdxClientServiceAdaptor::~CvMsdxClientServiceAdaptor()
{
    // destructor
}

int CvMsdxClientServiceAdaptor::client_request(int action_id, const QByteArray &input_garray)
{
    // handle method call com.coolview.cvmsdxclient.client_request
    int result;
    QMetaObject::invokeMethod(parent(), "client_request", Q_RETURN_ARG(int, result), Q_ARG(int, action_id), Q_ARG(QByteArray, input_garray));
    return result;
}
