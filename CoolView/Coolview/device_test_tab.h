#ifndef DEVICE_TEST_TAB_H
#define DEVICE_TEST_TAB_H

#include <QWidget>
#include "ui_device_test_tab.h"
#include "util/Uploader/Uploader.h"
#include "util/zip/ZipDir.h"

class DeviceTestTab : public QWidget
{
  Q_OBJECT

public:
  DeviceTestTab(QWidget *parent = 0);
  ~DeviceTestTab();

  void ReadConfig();

Q_SIGNALS:
	void serverTestSignal();

private slots:

  //Video
  void preViewWebcamChangedSlot( const QString& webcamName );
  void preViewCrossbarChangedSlot( const QString& crossbar );
  void videoPreViewSlot();

  //Audio
	void startAudioInputTest();
	void startAudioOutputTest();
	void stopAudioInputTest();
  void stopAudioOutputTest();

  //Server
  void serverTestStartSlot();
  void recvServerTestResultSlot(bool isTimeOut,int interval);

	//打包并上传日志文件  add by lzb
	bool compressANDUploadLogFileSlot();
	void OnUploadStateChangedSlot(CUploader::UploadState state, QString msg);
	void OnZipStateChangedSlot(CZipDir::ZipState state, QString msg);

private:
  Ui::DeviceTestTab ui;
  CUploader uploader;
  CZipDir zip;
  QString compressFileName;
  QString username;
  bool isVideoPreview;
};

#endif // DEVICE_TEST_TAB_H
