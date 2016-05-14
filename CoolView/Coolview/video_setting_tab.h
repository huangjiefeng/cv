#ifndef VIDEO_SETTING_TAB_H
#define VIDEO_SETTING_TAB_H

#include <QWidget>
#include "ui_video_setting_tab.h"

class VideoSettingTab : public QWidget {
  Q_OBJECT

public:
  VideoSettingTab(QWidget *parent = 0);
  ~VideoSettingTab();

public slots:
  void ReadConfig();
  void SaveConfig();

  void ChangeCameraIndex(const int index);

private slots:
  //Video
  void cameraIndexChangedSlot( const QString& indexStr );
  void webcamChangedSlot( const QString& webcamName );
  void crossbarChangedSlot( const QString& crossbar );

  //Advance
  void ckbRecvAutoResync_StateChanged(int state);
  void ckbAdaptiveCodeRate_StateChanged(int state);

private:
  Ui::VideoSettingTab ui;  
};

#endif // VIDEO_SETTING_TAB_H
