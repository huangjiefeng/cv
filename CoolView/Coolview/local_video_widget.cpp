#include "local_video_widget.h"

#include <cassert>
#include <QMouseEvent>
#include <QDrag>
#include <QMimeData>

#include "controller_manager_interface.h"
#include "display_controller_interface.h"

#include "mime_text_parser_and_builder.h"

LocalVideoWidget::LocalVideoWidget(QWidget *parent)
    : QWidget(parent),
      camera_index_(-1) {
  ui_.setupUi(this);
}

LocalVideoWidget::~LocalVideoWidget() {

}

void LocalVideoWidget::Initilize( IControllerManager *controller_manager ) {
  assert(controller_manager);
  IDisplayController *controller =
    controller_manager->GetDisplayController();
  assert(controller);
  connect(this, &LocalVideoWidget::StopVideoSignal,
          controller, &IDisplayController::HandleStopVideoRequestSlot);
  connect(ui_.videoButton, &QPushButton::clicked,
          this, &LocalVideoWidget::HandleVideoButtonClickedSlot);
}

void LocalVideoWidget::Set( const QString &media_id, const int index ) {
  media_id_ = media_id;
  camera_index_ = index;
  image_url_ = ":/image/camera-web.png";
  ui_.videoButton->setIcon(QIcon(image_url_));
  ui_.videoNameLabel->setText(
    QString::fromLocal8Bit("摄像头%1").arg(index + 1));
}

void LocalVideoWidget::SetDisplay( bool show ) {
  ui_.videoButton->setChecked(show);
}

void LocalVideoWidget::mousePressEvent( QMouseEvent *event ) {
  if (event->button() == Qt::LeftButton) {
    start_pos_ = event->globalPos();
  }
  QWidget::mousePressEvent(event);
}

void LocalVideoWidget::mouseMoveEvent( QMouseEvent *event ) {
  if (event->buttons() & Qt::LeftButton) {
    int distance = (event->pos() - start_pos_).manhattanLength();
    if (distance >= QApplication::startDragDistance()) {
      PerformDrag();
    }
  }
  QWidget::mouseMoveEvent(event);
}

void LocalVideoWidget::PerformDrag() {
  // TODO: 暂时将media_id作为uri
  MimeTextBuilder text_builder(media_id_);
  text_builder.AddVirtualURI(media_id_);
  QMimeData *mime_data = new QMimeData();
  mime_data->setText(text_builder.GetText());
  QDrag *drag = new QDrag(this);
  drag->setMimeData(mime_data);

  QPixmap p(image_url_);
  drag->setPixmap(p);
  drag->setHotSpot(QPoint(p.size().width() / 2, p.size().height() / 2));

  // 在windows上exec会阻塞event loop
  drag->exec();

  // 在windows上需要删除drag
  drag->deleteLater();
}

void LocalVideoWidget::HandleVideoButtonClickedSlot(bool checked)
{
  emit ConfigCameraSignal(camera_index_);
}



