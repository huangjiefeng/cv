#pragma once

#include "ui_ConfrenceVideoWindow.h"
#include "qtmeetingplacewidget.h"

#include <QString>
#include <QTimer>

//zhenHua.sun 2010-09-01
#include <util/report/RtpStat.h>
#include <util/report/RecordStat.h>
#include <dbus/conferenceRoom/common/ConferenceRoomServiceCommon.h>


class QtMeetingPlaceWidget;
class QLabel;
class RecordCtrlPanel;
class RecordAdvancedPanel;

class QtConfrenceVideoWindow:public QWidget
{
	friend class QtMeetingPlaceWidget;
	friend class VideoFrame;
Q_OBJECT

public:
	//QtConfrenceVideoWindow(QtMeetingPlaceWidget * parent);
	QtConfrenceVideoWindow();
	~QtConfrenceVideoWindow(void);

	QFrame *getVideoFrame();

  QString getUserId(){return _userId;}
  void setUserId(const QString &user);

	bool getUseStatus(){return isUsed;}
	void setUseStatus(bool usedStatus){isUsed = usedStatus;}

	void updateRtcpEventHandler( const RtpStatItem& item );
    void updateRecordEventHandler(const RecStatItem& item);

	/**
	 * @brief 根据媒体的类型与状态进行图标提示
	 * @param type 媒体类型，audio或video
	 * @param state 媒体状态
	 * @param isAudioSend 如果为true则需要对麦克风图标进行操作，否则应该对喇叭图标进行操作
	 */
	void setMediaState( const QString type , const UiMediaState state,bool isAudioSend);
	void setRecordState(const UiMediaState state);

	bool isAudioEnable();
	bool isVideoEnable();

	/**
	 * @brief 释放窗口资源
	 */
	void release();

	
public Q_SLOTS:
	///@brief 在视频窗口上显示rtcp信息
	void showRtcpMessage();

	///@brief 隐藏视频窗口上的rtcp信息
	void hideRtcpMessage( );

	QString getDisplayName() const { return _displayName; }
  void setDisplayName( const QString displayName );

  //Record control
  void startRecord();
  void stopRecord();
  void ShowRecordAdvancedPanel(bool);

protected:
  virtual void resizeEvent ( QResizeEvent * event );
  virtual void moveEvent(QMoveEvent * event);
  virtual void hideEvent(QHideEvent * event);
  virtual void showEvent(QShowEvent * event);

private:
  void createLabels();
  void updateTextLabel(const QString &str);
  void updateRecordTextLabel(const QString &str);
  void resizeTextLabel();
  void adjustLabelStyle();

private:
	Ui::ConfrenceVideoWindow _ui;
	QString _userId;
	QString _displayName;		//在媒体窗口中显示的名称
  QString _rtcpMsg;
  QString _recMsg;
	bool isUsed;
	bool _detached;
	bool _fullScreen;
  bool _showRtcp;
  bool _isLabelTransparent;

	//rtcp显示条
	QFrame* _stopAudioFrame;
	//QFrame* _stopVideoFrame;
	QFrame* _stopRecvAudioFrame;
	QFrame* _mediaStateFrame;

  QLabel* _textLabel; // 用于显示透明字幕的文本框 - by Liaokz
  QLabel* _videoStatuslabel;

  //录制控制
  RecordCtrlPanel * _recordCtrlPanel;
  bool _isRecordCtrlPanelVisible;
  RecordAdvancedPanel *_recordAdvancedPanel;
};
