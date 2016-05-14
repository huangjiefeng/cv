#include "conference_widget.h"

#include <QMessageBox>
#include <QTextStream>
#include <QListWidget>
#include <QListWidgetItem>
#include <QDate>

#include "controller_manager_interface.h"
#include "help-widget.h"

ConferenceWidget::ConferenceWidget(QWidget *parent)
    : QWidget(parent), help_widget_(nullptr) {
  ui_.setupUi(this);
  has_initialized_ = false;
}

ConferenceWidget::~ConferenceWidget() {
  if (help_widget_) {
    help_widget_->close();
    delete help_widget_;
    help_widget_ = nullptr;
  }
}

void ConferenceWidget::Initialize( IControllerManager *controller_manager ) {
  assert(controller_manager != nullptr);

  // 当用户登出时需要情况会议列表
  ISessionController *session_controller =
    controller_manager->GetSessionController();
  assert(session_controller != nullptr);
  connect(
    session_controller, &ISessionController::NotifySessionStateChangedSignal,
    this, &ConferenceWidget::HandleSessionStateChangedNotifySlot);

  IConferenceController *conference_controller = 
    controller_manager->GetConferenceController();
  assert(conference_controller != nullptr); 
  
  // 响应会议状态改变的信号
  connect(
    conference_controller,
    SIGNAL(NotifyConferenceStateChangedSignal(
      const QString&, IConferenceController::ConferenceState)),
    this,
    SLOT(HandleConferenceStateChangedNotifySlot(
      const QString&, IConferenceController::ConferenceState)));

  // 响应加入会议错误的信号
  connect(
    conference_controller,
    SIGNAL(NotifyJoinConferenceErrorSignal(
      const QString&, IConferenceController::JoinConferenceError)),
    this,
    SLOT(HandleJoinConferenceErrorNotifySlot(
      const QString&, IConferenceController::JoinConferenceError)));

  // 响应获取到会议列表的信号
  connect(
    conference_controller,
    &IConferenceController::NotifyConferenceListReceivedSignal,
    this,
    &ConferenceWidget::HandleConferenceListReceivedNotifySlot);

  // 加入会议的信号和槽的建立
  connect(
    this,
    SIGNAL(JoinConferenceSignal(const QString&)),
    conference_controller,
    SLOT(HandleJoinConferenceRequestSlot(const QString&)));

  // 离开会议的信号和槽的建立
  connect(
    this,
    SIGNAL(LeaveConferenceSignal()),
    conference_controller,
    SLOT(HandleLeaveConferenceRequestSlot()));

  connect(
    this, 
    SIGNAL(RequestConferenceListSignal()),
    conference_controller,
    SLOT(HandleConferenceListRequestSlot()));

  SetupGui();

  has_initialized_ = true;
}

void ConferenceWidget::SetupGui() {
  connect(ui_.listWidget, SIGNAL(currentRowChanged(int)),
          this, SLOT(ShowConferenceTipSlot(int)));

  // TODO: 加入双击进入会议功能？
  connect(ui_.enterConferenceAction, SIGNAL(triggered()),
          this, SLOT(TryToJoinConferenceSlot()));

  connect(ui_.updateConferenceAction, SIGNAL(triggered()),
          this, SLOT(UpdateConferenceListSlot()));

  connect(
    ui_.actionHelp, SIGNAL(triggered()),
    this, SLOT(ShowHelpWidgetSlot()));
}

void ConferenceWidget::UpdateConferenceListSlot() {
  conference_dict_.clear();
  ui_.listWidget->clear();
  current_conference_uri_.clear();
  emit RequestConferenceListSignal();
}

void ConferenceWidget::HandleConferenceListReceivedNotifySlot(
    const ConstConferenceList &conferences) {
  for (auto conference : conferences) {
    conference_dict_.insert(conference->uri, conference);
    auto items = 
      ui_.listWidget->findItems(conference->title, Qt::MatchExactly);
    for (auto item : items) {
      if (item->toolTip() == conference->uri) {
        ui_.listWidget->takeItem(ui_.listWidget->row(item));
        delete item;
        break;
      }
    }
    QListWidgetItem *item = 
      new QListWidgetItem(conference->title, ui_.listWidget);
    item->setToolTip(conference->uri);
    int count = ui_.listWidget->count();
    if (count == 1) {
      ui_.listWidget->setCurrentRow(0);
    }
    
    // 选择为current_conference_uri_对应的会议名称
    if (!current_conference_uri_.isEmpty() && 
        conference->uri == current_conference_uri_) {
      ui_.listWidget->setCurrentItem(item);
    }

    if (!attempt_conference_uri_.isEmpty() && 
        conference->uri == attempt_conference_uri_) {
      emit JoinConferenceSignal(attempt_conference_uri_);
    }
  }
}

void ConferenceWidget::TryToJoinConferenceSlot() {
  assert(has_initialized_);
  const int conference_index = ui_.listWidget->currentRow();
  if (conference_index == -1) { // 未选中会议
    QMessageBox::warning(this, tr("CoolView"),
      QString::fromLocal8Bit("请选择您要加入的会议！"));
    return;
  }

  QString new_conference_uri = 
    ui_.listWidget->item(conference_index)->toolTip();

  if (current_conference_uri_ == new_conference_uri) {
    QMessageBox::warning(this, tr("CoolView"),
      QString::fromLocal8Bit("您已经进入了该会议！"));
    return;
  }

  // 尝试进入会议
  emit JoinConferenceSignal(new_conference_uri);
}

void ConferenceWidget::HandleConferenceStateChangedNotifySlot(
    const QString &uri, IConferenceController::ConferenceState state ) {
  switch (state) {
    case IConferenceController::kIsInConference:
      HandleJoinConferenceNotify(uri);
      break;
    case IConferenceController::kIsNotInConference:
      if (current_conference_uri_ == uri) {
        HandleLeaveConferenceNotify();
      }
      break;
  }
}

void ConferenceWidget::HandleJoinConferenceNotify(
    const QString &conference_uri) {
  current_conference_uri_ = conference_uri;
  // 设置窗口标题
  QString title = QString::fromLocal8Bit("正在进行会议: %1")
    .arg(conference_dict_[conference_uri]->title);
  emit SetWindowTitleSignal(title);
  setVisible(false);
}

void ConferenceWidget::HandleLeaveConferenceNotify() {
  conference_dict_.clear();
  ui_.listWidget->clear();
  current_conference_uri_.clear();
  setVisible(true);
}

void ConferenceWidget::HandleJoinConferenceErrorNotifySlot(
    const QString &uri, IConferenceController::JoinConferenceError error ) {
  // 清空记录的要尝试进入的会议
  attempt_conference_uri_.clear();

  const QString error_title = QString::fromLocal8Bit("进入会议出错");
  switch (error) {
    case IConferenceController::kOk:
      // empty
      break;
    case IConferenceController::kErrorOfIsAlreadyInOneConference: {
      if (current_conference_uri_ == uri) {
        return;
      }

      QString info_format = QString::fromLocal8Bit(
        "您已经进入了\"%1\"会议，要离开该会议并进\"%2\"会议吗？");

      QString info = info_format
        .arg(conference_dict_[current_conference_uri_]->title)
        .arg(conference_dict_[uri]->title);

      QMessageBox::StandardButton res = QMessageBox::information(
        this, tr("CoolView"), info, QMessageBox::Ok | QMessageBox::Cancel);

      if (res == QMessageBox::Ok) {
        attempt_conference_uri_ = uri;
        emit LeaveConferenceSignal();
      }
      break; 
    }

    case IConferenceController::kErrorOfConferenceNotExist:
      current_conference_uri_.clear();
      QMessageBox::warning(
        this, error_title, QString::fromLocal8Bit("该会议不存在"));
      break;

    case IConferenceController::kErrorOfIsLeavingConference:
      QMessageBox::warning(
        this, error_title, QString::fromLocal8Bit("正在离开会议"));
      break;

    case IConferenceController::kErrorOfPasswordNeeded:
      current_conference_uri_.clear();
      QMessageBox::warning(
        this, error_title, QString::fromLocal8Bit("该会议需要密码"));
      break;

    case IConferenceController::kErrorOfJoinRejected:
      current_conference_uri_.clear();
      QMessageBox::warning(
        this, error_title, QString::fromLocal8Bit("QoS服务器拒绝了你的加入"));
      break;

    case IConferenceController::kUnknownError:
      // fall through
    default:
      current_conference_uri_.clear();
      QMessageBox::warning(
        this, error_title, QString::fromLocal8Bit("未知错误"));
      break;
  }
}

void ConferenceWidget::ShowConferenceTipSlot( int index ) {
  if (index == -1) return;
  auto item = ui_.listWidget->item(index);
  QString conference_uri = item->toolTip();
  auto it = conference_dict_.find(conference_uri);
  if (it != conference_dict_.end()) {
    SetConferenceTip(it.value());
  }
}

void ConferenceWidget::SetConferenceTip( ConstConferencePointer conference ) {
  ui_.confNameLabel->setText(conference->title);
  ui_.confDateLabel->setText(QDate::currentDate().toString());
  ui_.confDurationLabel->setText(
    ConvertDurationMinToQString(conference->duration.toInt()));
  ui_.confStartTimeLabel->setText(conference->start_time);
  ui_.confLevelLabel->setText(conference->level);
  ui_.multicastAddress->setText(conference->multicast_address);
  ui_.joinModeLabel->setText(conference->join_mode);
  ui_.controlModeLabel->setText(conference->control_mode);
  if (conference->multicast_address.isEmpty() || 
      conference->multicast_address == "0.0.0.0") {
    ui_.isMulticast->setText(QString::fromLocal8Bit("否"));
  } else {
    ui_.isMulticast->setText(QString::fromLocal8Bit("是"));
  }
}

QString ConferenceWidget::ConvertDurationMinToQString( int duration_min ) {
  int days, hours, minutes;
  days = duration_min / (60 * 24);
  duration_min -= days * 60 * 24;
  hours = duration_min / 60;
  duration_min -= hours * 60;
  minutes = duration_min;
  if (days > 0) {
    return QString::fromLocal8Bit("%1天%2时%3分").arg(
      days).arg(hours, 2, 10, QChar('0')).arg(minutes, 2, 10, QChar('0'));
  } else {
    return QString::fromLocal8Bit("%1时%2分").arg(
      hours, 2, 10, QChar('0')).arg(minutes, 2, 10, QChar('0'));
  }
}

void ConferenceWidget::HandleSessionStateChangedNotifySlot(
    ISessionController::SessionState state) {
  if (state == ISessionController::kIsOffline) {
    conference_dict_.clear();
    ui_.listWidget->clear();
    current_conference_uri_.clear();
  }
}

void ConferenceWidget::ShowHelpWidgetSlot() {
  if (help_widget_ == nullptr) {
    help_widget_ = new HelpWidget(nullptr);
  }
  help_widget_->show();
  help_widget_->raise();
  help_widget_->setFocus();
}

