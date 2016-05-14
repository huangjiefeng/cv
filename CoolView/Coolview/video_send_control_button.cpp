#include "video_send_control_button.h"

#include <cassert>
#include <QMouseEvent>
#include <QDrag>
#include <QMimeData>
#include <QApplication>
#include <QDebug>

#include "controller_manager_interface.h"
#include "display_controller_interface.h"

#include "mime_text_parser_and_builder.h"

VideoSendControlButton::VideoSendControlButton(QWidget *parent)
    : QPushButton(parent),
      camera_index_(-1) {
  //ui_.setupUi(this);
  QSize fixed_size(70, 70);
  setMinimumSize(fixed_size);
  setMaximumSize(fixed_size);
  setIconSize(QSize(64, 64));
  setFlat(true);
  send_video_ = true;
  is_setting_mode_ = false;
}

VideoSendControlButton::~VideoSendControlButton() {

}

void VideoSendControlButton::Initilize(IDisplayController *controller) {
  assert(controller);

  // 控制本地视频的发送
  connect(this, &VideoSendControlButton::clicked, 
          this, &VideoSendControlButton::HandleVideoButtonClickedSlot);
  connect(this, &VideoSendControlButton::RequestControlVideoSendSignal,
          controller, &IDisplayController::HandleSendLocalVideoRequestSlot);
  connect(controller, &IDisplayController::NotifySendLocalVideoSignal,
          this, &VideoSendControlButton::HandleControlVideoSendNotifySlot);
}

void VideoSendControlButton::Set( const QString &media_id, const int index ) {
  media_id_ = media_id;
  camera_index_ = index;
  SetSendStatus(true);
  setToolTip(QString::fromLocal8Bit("摄像头%1").arg(index + 1));
}

void VideoSendControlButton::SetSendStatus( bool send ) {
  send_video_ = send;
  image_url_ = send ? ":/image/videoStop.png" : ":/image/videoSend.png";
  setIcon(QIcon(image_url_));
}

void VideoSendControlButton::HandleVideoButtonClickedSlot(bool checked) {
  if (is_setting_mode_) {
    emit ConfigCameraSignal(camera_index_);
  } else {
    emit RequestControlVideoSendSignal(camera_index_, !send_video_);
  }
}

void VideoSendControlButton::HandleControlVideoSendNotifySlot(
    int virtual_index, bool send) {
  if (virtual_index == camera_index_) {
    SetSendStatus(send);
  }
}

void VideoSendControlButton::SetDisplay( bool show ) {
  // TODO:
}

void VideoSendControlButton::mousePressEvent( QMouseEvent *event ) {
  if (event->button() == Qt::LeftButton) {
    start_pos_ = event->globalPos();
  }
  QPushButton::mousePressEvent(event);
}

void VideoSendControlButton::mouseMoveEvent( QMouseEvent *event ) {
  if (event->buttons() & Qt::LeftButton) {
    int distance = (event->pos() - start_pos_).manhattanLength();
    if (distance >= QApplication::startDragDistance()) {
      PerformDrag();
    }
  }
  QPushButton::mouseMoveEvent(event);
}

void VideoSendControlButton::PerformDrag() {
  // note: 暂时将media_id作为uri
  MimeTextBuilder text_builder(media_id_);
  text_builder.AddVirtualURI(media_id_);
  QMimeData *mime_data = new QMimeData();
  mime_data->setText(text_builder.GetText());
  QDrag *drag = new QDrag(this);
  drag->setMimeData(mime_data);

  QPixmap p(":/image/videoLocal.png");
  drag->setPixmap(p);
  drag->setHotSpot(QPoint(p.size().width() / 2, p.size().height() / 2));

  // 在windows上exec会阻塞event loop
  drag->exec();

  // 在windows上需要删除drag
  drag->deleteLater();
}

void VideoSendControlButton::SetSettingMode( bool is_setting_mode ) {
  is_setting_mode_ = is_setting_mode;
  setIcon(QIcon(is_setting_mode ? ":/image/videoSet.png" : image_url_));
}





