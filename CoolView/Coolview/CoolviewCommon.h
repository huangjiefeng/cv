#pragma once

#ifndef COOLVIEW_COMMON_H
#define COOLVIEW_COMMON_H

#include <QtCore/QtCore>
#include <vector>
#include <string>

#define  SAFE_DELETE(X)  { if( (X) ) { delete (X) ;  (X)=NULL ; } }

#define DBUS_DAEMON_PROC_NAME "dbus-daemon.exe"
#define COOLVIEW_PROC_NAME					"CoolView.exe"
#define COOLVIEW_UPGRADER_PROC_NAME			"CoolViewUpgrader.exe"
#define COOLVIEW_TELECONTROLLER_PROC_NAME	"CvTelecontrollerSocket.exe"
#define COOLVIEW_CONFERENCE_ROOM_PROC_NAME	"CvConferenceRoom.exe"
#define COOLVIEW_MSDX_PROC_NAME				"CvChannelDispatcher.exe"
#define COOLVIEW_SIP_WRAPPER_RPOC_NAME		"CvSipWrapper.exe"
#define COOLVIEW_LOCAL_MONITOR_PROC_NAME	"CvLocalMonitorWinForm.exe"
#define COOLVIEW_JABBER_PROC_NAME			"CVMessenger.exe"
#define PROC_NETWORK_MEASURER_NAME		"NetworkMeasurer.exe"
#define COOLVIEW_MONITOR_PROC_NAME		"CvMonitor.exe"
#define COOLVIEW_KINECT_PROC_NAME		"CvKinect.exe"
#define COOLVIEW_TX_MONITOR_NAME		"TxMonitor.exe"
#define WEBLIB_UPLOADER_DIR		        "txuploader"
#define WEBLIB_UPLOADER_NAME		    "WebHDDemo.exe"
#define COOLVIEW_BFCP_CLIENT            "BfcpClient.exe"

void TransformStringVectorToQStringList( const std::vector<std::string>* stringVector , QStringList* qStringList);

void ResizeWidgetInMiddle( QWidget* widget );

QString getProcessPath(const QString &proc_name);


bool openExe( const QString& name , QString dir="" , QString param="" );
int startDBusService();
int startMsdxSvr();
int startSipWrapper();
void stopSipWrapper();
int startTelecontroller( const QString& terminalName="Terminal");
void stopTelecontroller();
int startUpgrader();
int startLocalMonitor();
int startJabberProc();
int startConferenceRoom();

void stopBfcpClient();

int startCvMonitor();
void stopCvMonitor();

// Tx_temp
int startTxMonitor();
void stopTxMonitor();

int startWeblibUploader();
void stopWeblibUploader();

//启动网络测量进程
int startNetworkMeasurerProcess();

/**
 * @brief kinect控制进程
 */
int startKinectDaemon();
void stopKinectDaemon();


/**
 * @brief 开启Coolview所需的相关进程
 */
int startReleativeServices();
/**
 * @brief 关闭Coolview相关的所有进程
 */
void stopReleativeServices();

#endif 