
#include ".\qtconfrencevideowindow.h"
#include <QtWidgets/QDesktopWidget>
#include <QMessageBox>
#include <QLabel>
#include <QGraphicsDropShadowEffect>
#include "ConferenceRoomService.h"
#include "MediaWindowManager.h"

#include <dbus/channel/type/streamedMedia/common/StreamedMediaServiceCommon.h>
#include "dbus/txController/common/TxControllerServiceCommon.h"
#include "dbus/conferenceRoom/common/ConferenceRoomServiceCommon.h"
#include "util/ini/CVIniConfig.h"

#include "record_ctrl_panel.h"
#include "record_advanced_panel.h"
#include "util/ini/TxConfig.h"


const int TEXT_LABEL_MARGING = 10;

QtConfrenceVideoWindow::QtConfrenceVideoWindow()
  : _isLabelTransparent(false)
  , _textLabel(nullptr)
  , _videoStatuslabel(nullptr)
  , _recordCtrlPanel(nullptr)
  , _isRecordCtrlPanelVisible(true)
  , _recordAdvancedPanel(nullptr)
{
	_ui.setupUi(this);
	_userId = "";
	isUsed = false;
	_detached = false;
	_fullScreen = false;
  _showRtcp = false;
  //仅当不使用录制控制时可使用透明字幕
  if (!_isRecordCtrlPanelVisible) {
	  // 由于性能原因，透明字幕改为可选 --by Liaokz，2014-1
    _isLabelTransparent = CVIniConfig::getInstance().useTransparentSubtitle();
  }

	//_stopVideoFrame = new QFrame(this);
	/*stopVideoFrame->setStyleSheet(
    "QFrame{border-image:url(:/CvConferenceRoom/Resources/stopvideo.png);"
    "border-width:0px;border-color:darkGray;border-style:solid;}");*/ // 旧图标
	/*_stopVideoFrame->setStyleSheet(
    "QFrame{border-image:url(:/CvConferenceRoom/Resources/VideoStateStop.png);"
    "border-width:0px;border-color:darkGray;border-style:solid;}");
	_stopVideoFrame->setHidden(true);*/

  _stopAudioFrame = new QFrame(this);
  _stopAudioFrame->setStyleSheet(
    "QFrame{border-image:url(:/CvConferenceRoom/Resources/MicrophoneRed.png);"
    "border-width:0px;border-color:darkGray;border-style:solid;}");
  _stopAudioFrame->setHidden(true);

  _stopRecvAudioFrame = new QFrame(this);
  _stopRecvAudioFrame->setStyleSheet(
    "QFrame{border-image:url(:/CvConferenceRoom/Resources/stopaudio.png);"
    "border-width:0px;border-color:darkGray;border-style:solid;}");
	_stopRecvAudioFrame->setHidden(true);

	_mediaStateFrame = new QFrame(this);
	_mediaStateFrame->setStyleSheet(
    "QFrame{border-image:url(:/CvConferenceRoom/Resources/mediaWaiting.png);"
    "border-width:2px;border-color:darkGray;border-style:solid;"
    "border-radius: 5px; background-color: white;}");
	setMediaState( UiStateTypeVideo, UiMediaState_Initializing, true);

  if (_isRecordCtrlPanelVisible) {
    _recordCtrlPanel = new RecordCtrlPanel();
    _ui.topLayout->addWidget(_recordCtrlPanel);
    _recordCtrlPanel->Initialize();
    if (!_isLabelTransparent) {
      _recordCtrlPanel->setAutoFillBackground(true);
    }

    _recordAdvancedPanel = new RecordAdvancedPanel(this);
    _recordAdvancedPanel->setHidden(true);

    connect(_recordCtrlPanel, &RecordCtrlPanel::StartRecordSignal,
      this, &QtConfrenceVideoWindow::startRecord);
    connect(_recordCtrlPanel, &RecordCtrlPanel::StopRecordSignal,
      this, &QtConfrenceVideoWindow::stopRecord);
    connect(_recordCtrlPanel, &RecordCtrlPanel::ShowRecInfoPanelSignal,
      this, &QtConfrenceVideoWindow::ShowRecordAdvancedPanel);
  } else {
    _ui.topWidget->setVisible(false);
  }

  createLabels();
}

QtConfrenceVideoWindow::~QtConfrenceVideoWindow(void)
{
  SAFE_DELETE( _videoStatuslabel);
  SAFE_DELETE( _textLabel );
	//SAFE_DELETE( _stopVideoFrame );
	SAFE_DELETE( _stopAudioFrame );
	SAFE_DELETE( _mediaStateFrame );
  SAFE_DELETE( _recordCtrlPanel );
  SAFE_DELETE( _recordAdvancedPanel );
}

void QtConfrenceVideoWindow::createLabels()
{
  if (_recordCtrlPanel) {
    //如果有录制控制面板，则使用其内部的文本框
    _textLabel = _recordCtrlPanel->GetStateLabel();
	} else {
    //否则创建一个
    _textLabel = new QLabel(this);
    if (_isLabelTransparent) {
      _textLabel->setWindowFlags(Qt::Window | Qt::FramelessWindowHint); // 实现背景透明必不可少
      _textLabel->setAutoFillBackground(false);
      _textLabel->setAttribute(Qt::WA_TranslucentBackground);
      _textLabel->setWordWrap(true);

      // 投影效果
      QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(_textLabel);
      effect->setBlurRadius(0);
      effect->setColor(QColor("#0"));
      effect->setOffset(1,1);
      _textLabel->setGraphicsEffect(effect);
    } else {
      _textLabel->setAutoFillBackground(true);
    }
  }

  // 无视频信号文本
  _videoStatuslabel = new QLabel(this);
  _videoStatuslabel->setText(QString::fromLocal8Bit("无视频信号"));

  adjustLabelStyle();
}

QFrame *QtConfrenceVideoWindow::getVideoFrame()
{
	return _ui.videoFrame;
}


void QtConfrenceVideoWindow::setUserId(const QString &user)
{
	_userId = user;
	
	_mediaStateFrame->setHidden(false);
	//_stopVideoFrame->setHidden(true);
	_stopAudioFrame->setHidden(true);
	_stopRecvAudioFrame->setHidden(true);
  _videoStatuslabel->hide();

  // 重置RTCP信息
  RtpStatItem emptyRtcp = {0};
  updateRtcpEventHandler(emptyRtcp);

  _recMsg = "";

  if (_recordCtrlPanel) {
    _recordCtrlPanel->SwitchRecBtnToStart();
    CTxConfig &tx_config = CTxConfig::getInstance();
    bool allow_record_ctrl = (tx_config.GetSoapCAllowGlobalCtrl() || 
      (tx_config.GetSoapCAllowSelfCtrl() && isLocalPreviewMediaID(_userId)));
    _recordCtrlPanel->EnableRecBtn(allow_record_ctrl);
  }
}


void QtConfrenceVideoWindow::resizeEvent ( QResizeEvent * event )
{
	//报告窗口大小变更，以便调整视频显示方式
	ConferenceRoomService::getInstance()->resizeMediaWindow( 
    getUserId(), (int)(_ui.videoFrame->winId()) );

	const int width = event->size().width();
	const int height = event->size().height();

  // 必须先设置字体大小再调整位置，字幕栏在最上面，提供下面的frame的Y坐标
  adjustLabelStyle();

	const int frameWidth = 35;   //固定stopAudioFrame和stopVideoFrame的图标大小 add by lzb
  const int frameStartY = 10 + 
    (_isLabelTransparent ? 0 : _textLabel->height());

	//stopVideoFrame->setGeometry(width-frameWidth-10,10+frameWidth,frameWidth,frameWidth);
	_stopAudioFrame->setGeometry(width-frameWidth-10,frameStartY,frameWidth,frameWidth);
	_stopRecvAudioFrame->setGeometry(width-frameWidth-10,frameStartY+2*frameWidth,frameWidth,frameWidth);

	int stateFrameWidth = height/4;
	int left = (width-stateFrameWidth)/2;
	int top = (height-stateFrameWidth)/2;
	_mediaStateFrame->setGeometry( left , top , stateFrameWidth , stateFrameWidth );

	if (_videoStatuslabel) {
		/*if (((float)width) / (height) >= 16.0 /9)
		{
			const int myHeight = height;
			const int myWidth = myHeight / 9 * 16;
			_stopVideoFrame->setGeometry((width - myWidth)/2,0,myWidth,myHeight);
		}
		else
		{
			const int myWidth = width;
			const int myHeight = width / 16 * 9;
			_stopVideoFrame->setGeometry(width,(height-myHeight)/2,width,myHeight);
		}*/
    const int label_width = _videoStatuslabel->width();
    const int label_height = _videoStatuslabel->height();
    QPoint pos((width - label_width)/2, (height - label_height)/2);
    _videoStatuslabel->move(pos);
	}

  // text label
  resizeTextLabel();

  //RecCtrlPanel
  if (_recordAdvancedPanel) {
    _recordAdvancedPanel->setGeometry(
      width - _recordAdvancedPanel->width(), _ui.topWidget->height(), 
      _recordAdvancedPanel->width(), _recordAdvancedPanel->height());
  }
}


void QtConfrenceVideoWindow::updateRtcpEventHandler( const RtpStatItem &rtpstat )
{
  QString msg_format;
  if (_isLabelTransparent) {
    // 背景透明时分行显示
    msg_format = "Bandwidth:%1kb/s\nDelay:%2ms\nLost:%3%";
  } else {
    msg_format = "Bandwidth:%1kb/s  Delay:%2ms  Lost:%3%";
  }

  _rtcpMsg = msg_format.arg(rtpstat.recv_bandwidth, 0 , 'f', 2)
    .arg(rtpstat.delay)
    .arg(rtpstat.lost);

  if (_showRtcp) {
    updateTextLabel(_rtcpMsg);
  }
}

void QtConfrenceVideoWindow::updateRecordEventHandler( const RecStatItem &recstat )
{
  QString rec_msg;

  if (recstat.statType == REC_STAT_FILE_WRITING) {
    QString msg_format;
    if (_isLabelTransparent) {
      // 背景透明时分行显示
      // 虽然在构造函数中已经判断，如果显示录制控制面板则不使用透明
      // 但可能存在本地录制，本地视频窗口要显示本地录制状态
      // Liaokz
      msg_format = QString::fromLocal8Bit("\n录制时长 %1:%2:%3\n文件大小 %4MB");
    } else {
      msg_format = QString::fromLocal8Bit("(已录制%1:%2:%3, %4MB)");
    }

    //时间精度是毫秒
    int hour = recstat.rec.duration / 1000 / 3600;
    int min = recstat.rec.duration / 1000 / 60 % 60;
    int sec = recstat.rec.duration / 1000 % 60;
    rec_msg = msg_format.arg(hour)
        .arg(min, 2, 10, QChar('0'))
        .arg(sec, 2, 10, QChar('0'))
        .arg(recstat.rec.size/1000000.0, 0, 'f', 1);

    updateRecordTextLabel(rec_msg);
    //本地录制暂时不允许控制
    _recordCtrlPanel->SwitchRecBtnToStop();
    _recordCtrlPanel->EnableRecBtn();
  }
  else if (recstat.statType == REC_STAT_MUX_STOP) {
    updateRecordTextLabel(QString::fromLocal8Bit("(未录制)"));
    _recordCtrlPanel->SwitchRecBtnToStart();
    _recordCtrlPanel->EnableRecBtn();
  }
}

void QtConfrenceVideoWindow::showRtcpMessage()
{
  _showRtcp = true;
  updateTextLabel(_rtcpMsg);
}

void QtConfrenceVideoWindow::hideRtcpMessage()
{
  _showRtcp = false;
  updateTextLabel(_displayName);
}

void QtConfrenceVideoWindow::setDisplayName( const QString displayName )
{
	this->_displayName = displayName;
  if (!_showRtcp) {
    updateTextLabel(_displayName);
  }  
}

void QtConfrenceVideoWindow::setMediaState( const QString type, const UiMediaState state,bool isAudioSend)
{
  if (type == UiStateTypeRecord) {
    if (!_recordCtrlPanel) {
      return;
    }
    setRecordState(state);

  } else {

	  switch( state ) {
	  case UiMediaState_Initializing:
		  _mediaStateFrame->setHidden(false);
		  break;
	  case UiMediaState_Ready:
		  _mediaStateFrame->setHidden(true);
		  break;
	  case UiMediaState_Run:
      {
        _mediaStateFrame->setHidden(true);
			  if( type==UiStateTypeAudio ) {
				  if(isAudioSend) {
					  _stopAudioFrame->setHidden(true);
				  } else {
					  _stopRecvAudioFrame->setHidden(true);
				  }
			  } else {
				  //_stopVideoFrame->setHidden(true);
          _videoStatuslabel->hide();
        }
			  break;
		  }
	  case UiMediaState_Stop:
      {
        _mediaStateFrame->setHidden(true);
			  if( type==UiStateTypeAudio ) {
				  if (isAudioSend) {
					  _stopAudioFrame->setHidden(false);
				  } else {
					  _stopRecvAudioFrame->setHidden(false);
				  }
        }
			  else {
				  //_stopVideoFrame->setHidden(false);
          _videoStatuslabel->show();
        }
			  break;
		  }
	  } // switch
  } // else
}

void QtConfrenceVideoWindow::setRecordState(const UiMediaState state)
{
  const int real_state = state & 0xFFFF;
  switch (real_state)
  {
  case UiMediaState_RecordRun:
    //录制中，开启停止按钮
    _recordCtrlPanel->SwitchRecBtnToStop();
    _recordCtrlPanel->EnableRecBtn();
    break;

  case UiMediaState_RecordStop:
    //录制停止，开启录制按钮
    updateRecordTextLabel(QString::fromLocal8Bit("(未录制)"));
    _recordCtrlPanel->SwitchRecBtnToStart();
    _recordCtrlPanel->EnableRecBtn();
    break;

  case UiMediaState_RecordError:
    updateRecordTextLabel(QString::fromLocal8Bit("(错误：未进入会议或未知原因)"));
    _recordCtrlPanel->SwitchRecBtnToStart();
    _recordCtrlPanel->EnableRecBtn();
    break;

  case UiMediaState_RecordSoapError:
    updateRecordTextLabel(QString::fromLocal8Bit("(远程操作错误：TX未启动或地址不正确)"));
    _recordCtrlPanel->SwitchRecBtnToStart();
    _recordCtrlPanel->EnableRecBtn();
    break;

  case UiMediaState_RecordTxError:
    {
      int err_code = state >> 16;
      QString msg;
      if (err_code == TXRecord_Error) msg = QString::fromLocal8Bit("(TX错误)");
      else if (err_code == TXRecord_UriNotExist) msg = QString::fromLocal8Bit("(TX错误：终端不存在或无权录制)");
      else if (err_code == TXRecord_IsRecording) msg = QString::fromLocal8Bit("(TX错误：终端已在录制中)");
      else if (err_code == TXRecord_IsNotRecording) msg = QString::fromLocal8Bit("(TX错误：终端未录制)");
      else if (err_code == TXRecord_NotInFocus) msg = QString::fromLocal8Bit("(TX错误：不在当前会议中)");
      updateRecordTextLabel(msg);
      _recordCtrlPanel->SwitchRecBtnToStart();
      _recordCtrlPanel->EnableRecBtn();
    }
    break;

  default:
    break;
  }
}

bool QtConfrenceVideoWindow::isAudioEnable()
{
	if( _stopAudioFrame==NULL || _stopAudioFrame->isHidden() )
	{
		return true;
	}

	return false;
}

bool QtConfrenceVideoWindow::isVideoEnable()
{
	//if( _stopVideoFrame==NULL||_stopVideoFrame->isHidden() )
  if (_videoStatuslabel == nullptr || _videoStatuslabel->isHidden())
	{
		return true;
	}

	return false;
}

/**
 * @brief 释放窗口资源
 */
void QtConfrenceVideoWindow::release()
{
  //UserId不要赋空，以便快速复用，见MediaWindowManager::FindMediaWindow
	//this->setUserId("" ); 

	_mediaStateFrame->setHidden(true);
	//_stopVideoFrame->setHidden(true);
  _videoStatuslabel->hide();
	_stopAudioFrame->setHidden(true);
	_stopRecvAudioFrame->setHidden(true);
}

void QtConfrenceVideoWindow::hideEvent( QHideEvent * event )
{
  qDebug() << (this) << "hide";
  _textLabel->hide();
}

void QtConfrenceVideoWindow::showEvent( QShowEvent * event )
{
  qDebug() << (this) << "show";
  resizeTextLabel();
  _textLabel->show();
}

void QtConfrenceVideoWindow::moveEvent( QMoveEvent * event )
{
  qDebug() << (this) << "move";
  resizeTextLabel();
}

void QtConfrenceVideoWindow::updateTextLabel( const QString &str )
{
  if (!_textLabel) {
    return;
  }
  _textLabel->setText(str);
  if (_isLabelTransparent) {
    _textLabel->adjustSize();
  }
}

void QtConfrenceVideoWindow::updateRecordTextLabel(const QString &str)
{
  _recMsg = str;
  if (!_showRtcp) {
    updateTextLabel(_displayName + _recMsg);
  }
}

void QtConfrenceVideoWindow::resizeTextLabel()
{
  _textLabel->adjustSize(); // 获得最小高度
  const int width = geometry().width();
  const int height = _textLabel->height();

  if (_isLabelTransparent) {
    QPoint labelPos = mapToGlobal(QPoint(TEXT_LABEL_MARGING, TEXT_LABEL_MARGING));
    _textLabel->move(labelPos);
  } else {
    _textLabel->setGeometry(0, 0, width, height);
  }
}

void QtConfrenceVideoWindow::adjustLabelStyle()
{
  const QString font_name = QString::fromLocal8Bit("微软雅黑");

  const QRect rect = geometry();
  const float unit = rect.width() > rect.height() ? 
    rect.height() / 400.0 : rect.width() / 400.0; // 做参照的基本长度，
  //开发时是在高400像素的窗口中进行的，故除以400是为了方便

  int font_size_in_pt = 0;

  // text label
  font_size_in_pt = 12 * unit;
  if (font_size_in_pt < 12) {
    font_size_in_pt = 12;
  } else if (font_size_in_pt > 24) {
    font_size_in_pt = 24;
  }
  QString font_color;
  if (_isLabelTransparent) {
    font_color = "white";
  } else {
    font_color = "black";
  }
  _textLabel->setStyleSheet(QString::fromLocal8Bit(
    "color: %1; font: bold %2px %3; "
    "qproperty-alignment: AlignLeft;"
		"background-color: rgb(240, 240, 240);")
    .arg(font_color).arg(font_size_in_pt).arg(font_name));
  if (_isLabelTransparent) {
    // 透明时，尽可能减小窗口大小
    _textLabel->adjustSize();
  }

  // video status label
  font_size_in_pt = 48 * unit;
  _videoStatuslabel->setStyleSheet(QString(
    "color: %1; font: %2px %3; qproperty-alignment: AlignCenter;"
    "border: %4px solid darkGrey; border-radius: %5px; "
    "background-color: rgba(230, 230, 230, 30%);")
    .arg("white").arg(font_size_in_pt).arg(font_name)
    .arg(2 * unit).arg(8 * unit));
  _videoStatuslabel->setMargin(8 * unit);
  _videoStatuslabel->adjustSize();
}

void QtConfrenceVideoWindow::startRecord()
{
  StartRecordParam param;
  if (_recordAdvancedPanel) {
    _recordAdvancedPanel->GetRecordInfo(param);
  }

  /*QMessageBox confirm_box(QMessageBox::Question, 
    QString::fromLocal8Bit("确认录制信息"), 
    QString::fromLocal8Bit("录像将使用以下信息：\n 标题：%1\n 与会者：%2\n 关键词：%3\n 描述：%4").
      arg(param.title).arg(param.participants).arg(param.keywords).arg(param.description), 
    QMessageBox::Yes|QMessageBox::No, nullptr);
  if (confirm_box.exec() == QMessageBox::No) {
    return;
  }*/

  updateRecordTextLabel(QString::fromLocal8Bit("(正在启动录制...)"));

  QByteArray output_array;
  QDataStream out(&output_array , QIODevice::WriteOnly );
  out.setVersion(QDataStream::Qt_4_4);
  out << param;

  MediaWindowManager::getInstance()->ControlVideoRecording(_userId, RecCtrl_Start, output_array);
  _recordCtrlPanel->EnableRecBtn(false);
}

void QtConfrenceVideoWindow::stopRecord()
{
  /*if (QMessageBox::question(nullptr, 
        QString::fromLocal8Bit("确认操作"), 
        QString::fromLocal8Bit("要停止录像吗？")) == QMessageBox::No) {
    return;
  }*/
  QByteArray output_array;
  MediaWindowManager::getInstance()->ControlVideoRecording(_userId, RecCtrl_Stop, output_array);
  //已停止，进入stopWait状态
  _recordCtrlPanel->EnableRecBtn(false);
}

void QtConfrenceVideoWindow::ShowRecordAdvancedPanel( bool show )
{
  if (_recordAdvancedPanel) {
    _recordAdvancedPanel->setHidden(!show);
  }
}
