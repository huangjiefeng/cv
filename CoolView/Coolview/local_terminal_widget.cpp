#include "local_terminal_widget.h"

#include <string>
#include <vector>

#include <QResizeEvent>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QMenu>

#include <dbus/channel/type/streamedMedia/common/StreamedMediaServiceCommon.h>
#include <DeviceManager/DeviceManager.h>

#include "terminal_description.h"
#include "display_controller_interface.h"
#include "controller_manager_interface.h"

#include "profile/RunningProfile.h"
#include "config/RunningConfig.h"
#include "util/ini/CVIniConfig.h"

#include "local_speaker_widget.h"
#include "local_mic_widget.h"

#include "audio_send_control_button.h"
#include "video_send_control_button.h"
#include "hand_up_button.h"
#include "speaker_volume_control_button.h"

#include "mime_text_parser_and_builder.h"

#include "fileplay_ctrl_widget.h"

LocalTerminalWidget::LocalTerminalWidget(QWidget *parent)
    : QWidget(parent) {
  ui_.setupUi(this);
  widget_width_ = -1;
  widgets_per_row_ = -1;
  //mic_ = nullptr;
  //speaker_ = nullptr;
  send_audio_button_ = nullptr;
  hand_up_button_ = nullptr;
  
  //volume_button_ = nullptr;
  speaker_volume_control_button_ = nullptr;
  //device_test_button_ = nullptr;
  monitor_button_ = nullptr;
  exit_button_ = nullptr;
  exit_menu_ = nullptr;
  
  is_setting_mode_ = false;

  play_ctrl_widget_ = nullptr;

  qRegisterMetaType<TerminalConfigType>("TerminalConfigType");
}

LocalTerminalWidget::~LocalTerminalWidget() {
}

void LocalTerminalWidget::Initialize(IControllerManager *controller_manager) {
  assert(controller_manager);
  controller_manager_ = controller_manager;

  IConferenceController *conference_controller =
    controller_manager->GetConferenceController();
  assert(conference_controller);
  connect(
    conference_controller, 
    SIGNAL(NotifyConferenceStateChangedSignal(
      const QString&, IConferenceController::ConferenceState)),
    this,
    SLOT(HandleConferenceStateChangedNotifySlot(
      const QString&, IConferenceController::ConferenceState)));
  
  IDisplayController *display_controller = 
    controller_manager->GetDisplayController();
  assert(display_controller);
  connect(this, SIGNAL(StopVideoSignal(const QString&)),
          display_controller, SLOT(HandleStopVideoRequestSlot(const QString&)));
  connect(this, SIGNAL(StopDocumentSignal(const QString&)),
          display_controller, SLOT(HandleStopDocumentRequestSlot(const QString&)));

  device_setting_button_ = CreateButton(
    ":/image/edit.png", QString::fromLocal8Bit("终端设置"));
  if (CVIniConfig::getInstance().IsModelHD()) {
    device_setting_button_->setCheckable(true);
    connect(device_setting_button_, &QPushButton::clicked,
            this, &LocalTerminalWidget::ChangeSettingModeSlot);
  } else {
    connect(device_setting_button_, &QPushButton::clicked,
      this, &LocalTerminalWidget::DisplayDeviceConfigSlot);
  }
  // 计算每行展示多少个widget
  if (widget_width_ == -1 || widgets_per_row_ == -1) {
    widget_width_ = device_setting_button_->width();
    widgets_per_row_ = CaculateWidgetsPerRow(widget_width_, width());
  }

  // 本地虚拟终端
  const int videos_count = 
    RunningConfig::Instance()->VideoCaptureDeviceCount();
  for (int i = 0; i < videos_count; ++i) {
    VideoSendControlButton *button = new VideoSendControlButton(this);
    button->Initilize(display_controller);
    button->Set(getLocalPreviewMediaID(i), i);
    virtual_terminals_.push_back(button);
    connect(button, &VideoSendControlButton::ConfigCameraSignal,
      this, &LocalTerminalWidget::ConfigCameraSignal);
  }
 
  // 发送音频按钮
  send_audio_button_ = new AudioSendControlButton(this);
  connect(
    send_audio_button_, &AudioSendControlButton::RequestControlAudioSendSignal,
    display_controller, &IDisplayController::HandleSendLocalAudioRequestSlot);
  connect(
    display_controller, &IDisplayController::NotifySendLocalAudioSignal,
    send_audio_button_, &AudioSendControlButton::HandleControlAudioSendNotifySlot);
  connect(
    send_audio_button_, &AudioSendControlButton::DisplayMicConfigSignal,
    this, &LocalTerminalWidget::DisplayMicConfigSlot);

  speaker_volume_control_button_ = new SpeakerVolumeControlButton(this);
  connect(
    speaker_volume_control_button_, &SpeakerVolumeControlButton::RequestControlVoiceMuteSignal,
    display_controller, &IDisplayController::HandleVoiceMuteRequestSlot);
  connect(
    display_controller, &IDisplayController::NotifyMuteChangeSignal,
    speaker_volume_control_button_, &SpeakerVolumeControlButton::HandleMuteChangeNotifySlot);
  connect(speaker_volume_control_button_, &SpeakerVolumeControlButton::DisplaySpeakerConfigSignal,
    this, &LocalTerminalWidget::DisplayMicConfigSlot);

  InitializeExtraSettingButtons();
  
  //// 扬声器音量按钮
  //std::vector<std::string> audio_output_list;
  //DeviceManager::GetInstatnce()->GetAudioOutputDevices(&audio_output_list);
  //if (!audio_output_list.empty()) {
  //  speaker_ = new LocalSpeakerWidget(this);
  //  speaker_->Initialize(display_controller);
  //  AddWidgetToLayout(speaker_);
  //}

  //// mic音量按钮
  //std::vector<std::string> audio_input_list;
  //DeviceManager::GetInstatnce()->GetAudioCaptureDevices(&audio_input_list);
  //if (!audio_input_list.empty()) {
  //  mic_ = new LocalMicWidget(this);
  //  mic_->Initialize(display_controller);
  //  AddWidgetToLayout(mic_);
  //}

  // 举手按钮
  hand_up_button_ = new HandUpButton(this);
  hand_up_button_->Initialize(conference_controller);

  hand_up_button_->hide();

  InitializeExitButton();

  AddButtonsToLayout();
  ChangeSettingModeSlot(is_setting_mode_);
}

void LocalTerminalWidget::InitializeExtraSettingButtons() {
  //volume_button_ = CreateButton(
  //  ":/image/volume1.png", QString::fromLocal8Bit("音频输出设置"));
  //connect(volume_button_, &QPushButton::clicked, 
  //  this, &LocalTerminalWidget::DisplaySpeakerConfiSlot);

  //device_test_button_ = CreateButton(
  //  ":/image/tools.png", QString::fromLocal8Bit("设备测试"));
  //connect(device_test_button_, &QPushButton::clicked, 
  //  this, &LocalTerminalWidget::DisplayDeviceTestConfigSlot);

  //monitor_button_ = CreateButton(
  //  ":/image/monitor.ico", QString::fromLocal8Bit("监控设置"));
  //connect(monitor_button_, &QPushButton::clicked,
  //  this, &LocalTerminalWidget::DisplayMonitorConfigSlot);
}

void LocalTerminalWidget::InitializeExitButton() {
  exit_button_ = CreateButton(
    ":/image/exit.png", QString::fromLocal8Bit("关闭"));
  exit_menu_ = new QMenu(this);

  QAction *logout_action = new QAction(QString::fromLocal8Bit("注销"), this);
  connect(logout_action, &QAction::triggered,
    this, &LocalTerminalWidget::RequestLogoutSignal);
  exit_menu_->addAction(logout_action);

  QAction *restart_action = new QAction(QString::fromLocal8Bit("重启"), this);
  connect(restart_action, &QAction::triggered,
    this, &LocalTerminalWidget::RequestRestartSignal);
  exit_menu_->addAction(restart_action);

#ifdef _DEBUG
  QAction *quit_system_action = new QAction(QString::fromLocal8Bit("退出"), this);
  connect(quit_system_action, &QAction::triggered,
    this, &LocalTerminalWidget::RequestQuitSystemSignal);
 exit_menu_->addAction(quit_system_action);
#else 
  QAction *shutdown_action = new QAction(QString::fromLocal8Bit("关机"), this);
  connect(shutdown_action, &QAction::triggered,
    this, &LocalTerminalWidget::RequestShutdownSignal);
  exit_menu_->addAction(shutdown_action);
#endif // DEBUG

  exit_button_->setMenu(exit_menu_);
}

void LocalTerminalWidget::AddButtonsToLayout() {
  int col = 0;
  if (CVIniConfig::getInstance().IsModelHD()) {
    for (auto button : virtual_terminals_) {
      AddWidgetToLayout(button, 0, col++);
    }
    AddWidgetToLayout(send_audio_button_, 0, col++);
    AddWidgetToLayout(speaker_volume_control_button_, 0, col++);
    AddWidgetToLayout(hand_up_button_, 0, col++);
    //AddWidgetToLayout(volume_button_, 0, col++);
  } else {
    send_audio_button_->hide();
    hand_up_button_->hide();
    //volume_button_->hide();
  }
  //AddWidgetToLayout(device_test_button_);
  //AddWidgetToLayout(monitor_button_);
  AddWidgetToLayout(device_setting_button_, 0, widgets_per_row_ - 2);
  AddWidgetToLayout(exit_button_, 0, widgets_per_row_ - 1);
}

QPushButton* LocalTerminalWidget::CreateButton( 
    const QString &icon, const QString &tooltip) {
  QPushButton *new_button = new QPushButton(this);
  new_button->setMinimumSize(70, 70);
  new_button->setMaximumSize(70, 70);
  new_button->setIconSize(QSize(64, 64));
  new_button->setIcon(QIcon(icon));
  new_button->setToolTip(tooltip);
  new_button->setFlat(true);
  return new_button;
}

void LocalTerminalWidget::HandleConferenceStateChangedNotifySlot(
    const QString &confernece_uri,
    IConferenceController::ConferenceState state) {
  if (!CVIniConfig::getInstance().IsModelHD()) {
    return;
  }
  if (state == IConferenceController::kIsInConference) {
    hand_up_button_->Reset();
    if (ConferenceHelper::IsVodVirtualConference(confernece_uri)) {
      ShowFilePlayCtrlWidget(true);
    } else {
      hand_up_button_->show();
    }
  } else if (state == IConferenceController::kIsNotInConference) {
    hand_up_button_->hide();
    ShowFilePlayCtrlWidget(false);
  }
}

void LocalTerminalWidget::resizeEvent( QResizeEvent *event ) {
  /*
  if (widget_width_ != -1) {
    const int new_widgets_per_row = 
      CaculateWidgetsPerRow(widget_width_, event->size().width());
    if (widgets_per_row_ != new_widgets_per_row) {
      widgets_per_row_ = new_widgets_per_row;
      CleatLayout();
      AddButtonsToLayout();
      //if (speaker_) {
      //  AddWidgetToLayout(speaker_);
      //}
      //if (mic_) {
      //  AddWidgetToLayout(mic_);
      //}
    }
  }*/
  AddButtonsToLayout();
  ChangeSettingModeSlot(is_setting_mode_);
}

int LocalTerminalWidget::CaculateWidgetsPerRow(
  int widget_width, int layout_width) {
    assert(widget_width != 0);
    int row_count = layout_width / widget_width;
    return row_count == 0 ? 1 : row_count;
}

void LocalTerminalWidget::CleatLayout() {
  //QLayoutItem *item;
  //while ((item = ui_.gridLayout->takeAt(0)) != nullptr) {
  //  ui_.verticalLayout->removeItem(item);
  //}
}

void LocalTerminalWidget::AddWidgetToLayout(QWidget *widget, int row, int col) {
  //if (widgets_per_row_ == -1) {
  //  ui_.gridLayout->addWidget(widget, 0, ui_.gridLayout->count());
  //} else {
  //  const int widget_index = ui_.gridLayout->count();
  //  int row = widget_index / widgets_per_row_;
  //  int col = widget_index - row * widgets_per_row_;
  //  ui_.gridLayout->addWidget(widget, row, col);
  //}
  const int offset_y = (ui_.scrollAreaWidgetContents2->height() - widget_width_) / 2;
  const int offset_x = 5, spacing = 5;
  widget->setParent(ui_.scrollAreaWidgetContents2);
  widget->move(
    offset_x + col * (widget_width_ + spacing), 
    offset_y + row * (widget_width_ + spacing));
  //widget->show();
}

void LocalTerminalWidget::dragEnterEvent( QDragEnterEvent *event ) {
  if (event->mimeData()->hasText()) {
    MimeTextParser parser(event->mimeData()->text());
    if (parser.is_coolview_mime_text()) {
      event->acceptProposedAction();
    }
  }
}

void LocalTerminalWidget::dropEvent( QDropEvent *event ) {
  MimeTextParser parser(event->mimeData()->text());
  QString terminal_uri = parser.GetURI();
  const QList<QString> &terminal_vuris = parser.GetVURIs();
  if (terminal_vuris.isEmpty()) {
    // 当vuri列表为空时，则表明是共享屏幕相关
    emit StopDocumentSignal(terminal_uri);
  } else {
    for (const QString &vuri : terminal_vuris) {
      emit StopVideoSignal(vuri);
    }
  }
  event->setDropAction(Qt::MoveAction);
  event->accept();
}

void LocalTerminalWidget::HandleStartVideoNotifySlot( const QString &vuri ) {
  SetLocalTerminalDisplayStatus(vuri, true);
}

void LocalTerminalWidget::HandleStopVideoNotifySlot( const QString &vuri ) {
  SetLocalTerminalDisplayStatus(vuri, false);
}

void LocalTerminalWidget::SetLocalTerminalDisplayStatus(const QString &vuri, bool show ) {
  if (!isLocalPreviewMediaID(vuri)) {
    return;
  }

  for (auto terminal : virtual_terminals_) {
    if (terminal->media_id() == vuri) {
      terminal->SetDisplay(show);
      break;
    }
  }
}

void LocalTerminalWidget::DisplayDeviceConfigSlot() {
  emit DisplayTerminalConfigSignal(kConfigTypeOfVideo);
}

void LocalTerminalWidget::DisplayMicConfigSlot() {
  emit DisplayTerminalConfigSignal(kConfigTypeOfMic);
}

void LocalTerminalWidget::DisplaySpeakerConfiSlot() {
  emit DisplayTerminalConfigSignal(kConfigTypeOfSpeaker);
}

void LocalTerminalWidget::DisplayDeviceTestConfigSlot() {
  emit DisplayTerminalConfigSignal(kConfigTypeOfDeviceTest);
}

void LocalTerminalWidget::DisplayMonitorConfigSlot() {
  emit DisplayTerminalConfigSignal(kConfigTypeOfMonitor);
}

void LocalTerminalWidget::ChangeSettingModeSlot( bool setting ) {
  is_setting_mode_ = setting;
  for (auto button : virtual_terminals_) {
    button->SetSettingMode(setting);
  }
  send_audio_button_->SetSettingMode(setting);
  speaker_volume_control_button_->SetSettingMode(setting);
  //volume_button_->setVisible(setting);
  //device_test_button_->setVisible(setting);
  //monitor_button_->setVisible(setting);
}

void LocalTerminalWidget::ShowFilePlayCtrlWidget( bool show )
{
  if (!play_ctrl_widget_) {
    play_ctrl_widget_ = new FilePlayCtrlWidget;
    play_ctrl_widget_->Initialize(controller_manager_->GetFilePlayController());
    // add and show
    const int offset_y = (ui_.scrollAreaWidgetContents2->height() - play_ctrl_widget_->height()) / 2;
    const int offset_x = 5;
    play_ctrl_widget_->setParent(ui_.scrollAreaWidgetContents2);
    play_ctrl_widget_->move(offset_x, offset_y);
  }
  
  if (show) {
    //显示播放控制按钮并隐藏其他按钮
    play_ctrl_widget_->Reset();
    play_ctrl_widget_->show();
    for (auto cambtn : virtual_terminals_) cambtn->hide();
    send_audio_button_->hide();
    speaker_volume_control_button_->hide();
  } else {
    play_ctrl_widget_->hide();
    for (auto cambtn : virtual_terminals_) cambtn->show();
    send_audio_button_->show();
    speaker_volume_control_button_->show();
  }
}






