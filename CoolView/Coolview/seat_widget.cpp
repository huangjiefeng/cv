#include "seat_widget.h"

#include <QMouseEvent>
#include <QDrag>
#include <QMimeData>
#include <QMessageBox>
#include <QDebug>
#include <QMenu>

#include "profile/RunningProfile.h"

#include "mime_text_parser_and_builder.h"
#include "dbus/channel/type/streamedMedia/common/StreamedMediaServiceCommon.h"

SeatWidget::SeatWidget(QWidget *parent, int seat_index)
    : QWidget(parent) {
  ui_.setupUi(this);
  seat_index_ = seat_index;
  menu_ = nullptr;
  is_show_ = false;

  if (seat_index == SHARED_SCREEN_SEAT) {
    ui_.documentLabel->show();
    ui_.headWidget->show();
    image_url_ = ":/image/folder-with-contents.png";
  } else {
    ui_.documentLabel->hide();
    ui_.headWidget->hide();
    image_url_ = ":/image/users_group.png";
  }
  QPixmap scaled_pixmap = 
    QPixmap(image_url_).scaled(ui_.headWidget->size());
  ui_.headLabel->setPixmap(scaled_pixmap);
  ui_.headLabel->setScaledContents(true);

  ui_.nameLabel->hide();

  menu_ = new QMenu(this);
  // TODO: remove?
  ui_.vCheckFrame->hide();
  ui_.aCheckFrame->hide(); 
}

SeatWidget::~SeatWidget() {
}

void SeatWidget::Set(const QString &uri, const QString &name) {
  terminal_uri_ = uri;
  terminal_vuri_.clear();
  ui_.nameLabel->setText(name.isEmpty() ? uri : name);
  ui_.nameLabel->show();
  is_show_ = true;
}

void SeatWidget::Set(
    const QString &vuri, 
    const QString &name, 
    bool use_small_video) {
  terminal_uri_.clear();
  terminal_vuri_ = vuri;
  QString hint_name = name;
  // TODO: 获取名称的方式需要改变，目前是临时方案
  if (name.isEmpty()) {
    if (isLocalPreviewMediaID(vuri)) {
      hint_name = QString::fromLocal8Bit("本地视频(%1)")
        .arg(getVirtualIndexFromLocalPreviewMediaID(vuri) + 1);
    }
  }
  use_small_video_ = use_small_video;
  if (use_small_video) {
    hint_name +=  QString::fromLocal8Bit("(小流)");
  }
  ui_.nameLabel->setText(hint_name);
  ui_.nameLabel->show();
  ui_.headWidget->show();
  is_show_ = true;
}

void SeatWidget::Reset() {
  terminal_uri_.clear();
  terminal_vuri_.clear();
  ui_.nameLabel->setText(QString());
  ui_.nameLabel->hide();
  if (seat_index_ != SHARED_SCREEN_SEAT) {
    ui_.headWidget->hide();
  }
  is_show_ = false;
}

void SeatWidget::mousePressEvent(QMouseEvent *event) {
  if (is_show_ && event->button() == Qt::LeftButton) {
    start_pos_ = event->pos();
  }
  QWidget::mousePressEvent(event);
}

void SeatWidget::mouseMoveEvent( QMouseEvent * event) { 
  if (is_show_ && 
      event->buttons() & Qt::LeftButton) {
    int distance = (event->pos() - start_pos_).manhattanLength();
    if (distance >= QApplication::startDragDistance()) {
      PerformDrag();
    } 
  }
  QWidget::mouseMoveEvent(event);
}

void SeatWidget::PerformDrag() {
  MimeTextBuilder text_builder(terminal_uri_);
  if (seat_index_ == SHARED_SCREEN_SEAT) {
    text_builder.SetMediaStreamType(MediaStreamType::kScreenType);
  } else {
    text_builder.SetMediaStreamType(use_small_video_ ? 
      MediaStreamType::kSmallStreamType : MediaStreamType::kStreamType);
  }
  if (!terminal_vuri_.isEmpty()) {
    text_builder.AddVirtualURI(terminal_vuri_);
  }

  QMimeData *mimeData = new QMimeData();
  mimeData->setText(text_builder.GetText());

  QDrag *drag = new QDrag(this);
  drag->setMimeData(mimeData);
  QPixmap p(image_url_);
  drag->setPixmap(p);
  drag->setHotSpot(QPoint(p.size().width() / 2, p.size().height() / 2));

  // 在windows上exec会阻塞event loop
  drag->exec();

  // 在windows上需要删除drag
  drag->deleteLater();
}

void SeatWidget::dragEnterEvent( QDragEnterEvent *event) {
  if (event->mimeData()->hasText()) {
    MimeTextParser parser(event->mimeData()->text());
    if (parser.is_coolview_mime_text()) {
      event->acceptProposedAction();
    }
  }
}

void SeatWidget::dropEvent( QDropEvent *event ) {
  MimeTextParser parser(event->mimeData()->text());
  QString terminal_uri = parser.GetURI();
  const QList<QString> &terminal_vuris = parser.GetVURIs();
  if (seat_index_ == SHARED_SCREEN_SEAT) { // 如果是共享屏幕位置
    // 根据uri接收共享屏幕
    emit ReceiveDocumentSignal(terminal_uri, seat_index_);

  } else { // 如果该位置是视频位置
    MediaStreamType type = parser.GetMediaStreamType();
    // 不处理接收共享屏幕的请求
    if (type != MediaStreamType::kScreenType) {
      bool use_small_video = true;
      if (type == MediaStreamType::kStreamType) {
        use_small_video = false;
      } else if (type == MediaStreamType::kSmallStreamType) {
        use_small_video = true;
      } else {
        // 默认为大流
        use_small_video = false;
      }
  
      // 计算能够显示在屏幕上的vuri数量，并请求接收视频流
      const int count = 
        std::min(SHARED_SCREEN_SEAT - seat_index_, terminal_vuris.size());
      for (int i = 0; i < count; ++i) {
        emit ReceiveVideoSignal(
          terminal_vuris[i], i + seat_index_, use_small_video);
      }
    }
  }

  event->setDropAction(Qt::MoveAction);
  event->accept();
}

void SeatWidget::mouseReleaseEvent( QMouseEvent *event ) {
  if (is_show_ && event->button() == Qt::RightButton) {
    // 显示菜单
    if (seat_index_ == SHARED_SCREEN_SEAT) {
      // 对于正在接收共享屏幕，提供停止接收的选项
      menu_->clear();
      menu_->addAction(ui_.stopReceiveAction);
      QAction *action = menu_->exec(event->globalPos());
      if (action == ui_.stopReceiveAction) {
        emit StopDocumentSignal(terminal_uri_, seat_index_);
      }
    } else {
      // 对于正在接收的视频，提供切换大小流和停止接收的选项
      menu_->clear();
      AddVideoActionToMenu(use_small_video_);
      menu_->addSeparator();
      menu_->addAction(ui_.stopReceiveAction);
      QAction *action = menu_->exec(event->globalPos());

      if (action == ui_.stopReceiveAction) {
        emit StopVideoSignal(terminal_vuri_, seat_index_);

      } else {
        bool use_small_video = use_small_video_;
        if (action == ui_.highQualityVideoAction) {
          use_small_video = false;
        } else if (action == ui_.lowQualityVideoAction) {
          use_small_video = true;
        }
        if (use_small_video_ != use_small_video) {
          emit ReceiveVideoSignal(terminal_vuri_, seat_index_, use_small_video);
        }
      }
    }
  }
  QWidget::mouseReleaseEvent(event);
}

void SeatWidget::AddVideoActionToMenu(bool use_small_video) {
  menu_->addAction(ui_.highQualityVideoAction);
  menu_->addAction(ui_.lowQualityVideoAction);
  if (use_small_video) {
    ui_.lowQualityVideoAction->setChecked(true);
    ui_.highQualityVideoAction->setChecked(false);
  } else {
    ui_.lowQualityVideoAction->setChecked(false);
    ui_.highQualityVideoAction->setChecked(true);
  }
}

void SeatWidget::resizeEvent( QResizeEvent *event ) {
  AdjustUI(event->size().width(), event->size().height());
}

void SeatWidget::AdjustUI( int width, int height ) {
  // TODO: 调整
  if (width < 11) width = 11;
  if (height < 11) height = 11;
  int head_widget_width = std::min(width - 10, height - 10);
  int head_widget_x = (width - head_widget_width) / 2;
  int head_widget_y = 5;
  ui_.headWidget->setGeometry(
    head_widget_x, head_widget_y, 
    head_widget_width, head_widget_width);

  int check_frame_height = 16;
  int check_frame_width = 16;
  int check_frame_x = width - 20;
  int check_frame_y = 8;
  ui_.vCheckFrame->setGeometry(
    check_frame_x - 18, check_frame_y,
    check_frame_width, check_frame_height);
  ui_.aCheckFrame->setGeometry(
    check_frame_x, check_frame_y,
    check_frame_width, check_frame_height);

  if (width > 60) {
    int name_label_height = 20;
    int name_lable_width = width - 4;
    int name_label_x = 2;
    int name_label_y = height - 22;
    ui_.nameLabel->setGeometry(
      name_label_x, name_label_y,
      name_lable_width, name_label_height);
    QFont font;
    font.setPointSize(7);
    ui_.nameLabel->setFont(font);
    ui_.nameLabel->setAlignment(Qt::AlignCenter);

    if (ui_.nameLabel->isVisible()) {
      ui_.headWidget->show();
      QPixmap temp(image_url_);
      QPixmap fixed_pixmap = temp.scaled(width, height);
      ui_.headLabel->setPixmap(fixed_pixmap);
      ui_.headLabel->show();
    }
  } else {
    int name_label_height = height * 4 / 5;
    int name_lable_width = width - 4;
    int name_label_x = 2;
    int name_label_y = (height - name_label_height) / 2;
    ui_.nameLabel->setGeometry(
      name_label_x, name_label_y,
      name_lable_width, name_label_height);
    QFont font;
    font.setPointSize(7);
    ui_.nameLabel->setFont(font);
    ui_.nameLabel->setAlignment(Qt::AlignCenter);

    ui_.headWidget->hide();
  }
}

void SeatWidget::SetSeatTitle( const QString &title ) {
  ui_.documentLabel->setText(title);
}



