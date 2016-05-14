#include "terminal_list_widget.h"

#include <algorithm>

#include <QDragEnterEvent>
#include <QMimeData>
#include <QMessageBox>

#include "util/ini/CVIniConfig.h"
#include "profile/RunningProfile.h"

#include "terminal_widget.h"
#include "terminal_description.h"
#include "seat_widget.h"

#include "conference_controller_interface.h"
#include "display_controller_interface.h"
#include "controller_manager_interface.h"

#include "mime_text_parser_and_builder.h"

TerminalListWidget::TerminalListWidget(QWidget *parent)
    : QWidget(parent) {
  ui_.setupUi(this);

  has_initialized_ = false;
  widget_width_ = -1;
  controller_manager_ = nullptr;
  show_type_ = kTypeOfShowOnline;
  is_chairman_ = false;
}

TerminalListWidget::~TerminalListWidget() {
}

void TerminalListWidget::Initialize( IControllerManager *controller_manager ) {
  assert(controller_manager);
  controller_manager_ = controller_manager;

  IConferenceController *conference_controller =
    controller_manager->GetConferenceController();
  assert(conference_controller);
  
  // 连接处理会议状态变更的信号和槽
  connect(
    conference_controller, 
    SIGNAL(NotifyConferenceStateChangedSignal(
      const QString&, IConferenceController::ConferenceState)),
    this,
    SLOT(HandleConferenceStateChangedNotifySlot(
      const QString&, IConferenceController::ConferenceState)));

  // 连接处理接收到的终端列表信号和槽
  connect(
    conference_controller,
    &IConferenceController::NotifyTerminalReceivedSignal,
    this, 
    &TerminalListWidget::HandleTerminalReceivedNotifySlot);

  // 连接处理接收到的终端总数变更的信号和槽
  connect(
    conference_controller,
    SIGNAL(NotifyTerminalCountChangedSignal(int, const QString&)),
    this,
    SLOT(HandleTerminalCountChangedNotifySlot(int, const QString&)));

  // 连接处理接收到终端信息更新的信号和槽
  connect(
    conference_controller,
    &IConferenceController::NotifyTerminalUpdateSignal,
    this,
    &TerminalListWidget::HandleTerminalUpdateNotifySlot);

  connect(
    this, SIGNAL(LeaveConferenceSignal()),
    conference_controller, SLOT(HandleLeaveConferenceRequestSlot()));
  
  connect(
    ui_.leaveConferenceAction, SIGNAL(triggered()),
    this, SLOT(ConfirmAndLeaveConferenceSlot()));

  IDisplayController *display_controller = 
    controller_manager->GetDisplayController();
  assert(display_controller);

  // 连接处理停止视频的请求信号和槽
  connect(this,
          SIGNAL(StopVideoSignal(const QString&)),
          display_controller, 
          SLOT(HandleStopVideoRequestSlot(const QString&)));

  // 连接处理停止共享屏幕的请求信号和槽
  connect(this, 
          SIGNAL(StopDocumentSignal(const QString&)),
          display_controller,
          SLOT(HandleStopDocumentRequestSlot(const QString&)));

  // 连接处理音频停止的提示信号和槽
  connect(display_controller, 
          SIGNAL(NotifyReceiveAudioSignal(const QString&, bool)),
          this,
          SLOT(HandleReceiveAudioNotifySlot(const QString&, bool)));

  SetupGui();
  ResetTerminalistWidget();

  has_initialized_ = true;
}

void TerminalListWidget::SetupGui() {
  menu_ = new QMenu(this);
  menu_->addAction(ui_.showAllTerminalsAction);
  menu_->addAction(ui_.showOnlineTerminalsAction);
  menu_->addAction(ui_.showOfflineTerminalsAction);

  connect(ui_.showAllTerminalsAction, SIGNAL(triggered()),
          this, SLOT(HandleShowAllTerminalsSlot()));
  connect(ui_.showOnlineTerminalsAction, SIGNAL(triggered()),
          this, SLOT(HandleShowOnlineTerminalsSlot()));
  connect(ui_.showOfflineTerminalsAction, SIGNAL(triggered()),
          this, SLOT(HandleShowOfflineTerminalsSlot()));

  // 默认为显示全部终端
  show_type_ = kTypeOfShowInOrder;
  ui_.showAllTerminalsAction->setChecked(true);
  ui_.showOfflineTerminalsAction->setChecked(false);
  ui_.showOnlineTerminalsAction->setChecked(false);
}

void TerminalListWidget::HandleConferenceStateChangedNotifySlot(
    const QString &conference_uri, IConferenceController::ConferenceState state) {
  // 在进入会议和退出会议时，需要重置终端列表界面
  if (state == IConferenceController::kIsInConference) {
    current_conference_uri_ = conference_uri;
    setVisible(true);
  } else if (state == IConferenceController::kIsNotInConference &&
             current_conference_uri_ == conference_uri) {
    ResetTerminalistWidget();
    setVisible(false);
  }
}

void TerminalListWidget::ResetTerminalistWidget() {
  widgets_per_row_ = -1;
  tx_terminal_count_ = 0;
  hd_terminal_count_ = 0;
  is_chairman_ = false;

  show_type_ = kTypeOfShowOnline;
  ui_.showAllTerminalsAction->setChecked(false);
  ui_.showOfflineTerminalsAction->setChecked(false);
  ui_.showOnlineTerminalsAction->setChecked(true);

  RemoveAllTerminals();
}

void TerminalListWidget::RemoveAllTerminals() {
  // 先将清空layout
  ClearLayout();

  // 再释放所有的widget
  for (auto widget : terminal_widget_dict_) {
    if (widget) {
      widget->deleteLater();
    }
  }
  terminal_widget_dict_.clear();
}

void TerminalListWidget::HandleTerminalCountChangedNotifySlot(
    int total_terminal_count, const QString &conference_uri ) {
  // 由于不显示本地终端，因此需要将hd终端数减1
  hd_terminal_count_ = total_terminal_count - tx_terminal_count_ - 1;
}

void TerminalListWidget::HandleTerminalReceivedNotifySlot(
    ConstTerminalPointer terminal, const QString &conference_uri) {
  QString local_uri = QString::fromStdString(
    RunningProfile::getInstance()->user_uri());
  if (terminal->uri == local_uri) {
    ActivateSpeakControl(terminal->is_chairman_terminal);
  }

  if (TerminalHelper::IsModelTX(terminal->model)) {
    // 统计TX和HD终端数量
    ++tx_terminal_count_;
    --hd_terminal_count_;
    // 不显示TX终端

	//终端列表显示回TX录制服务器gmlan20150727
  } if (terminal->uri != local_uri) {
    // 显示非本地的终端
    auto it = terminal_widget_dict_.find(terminal->uri);
    if (it != terminal_widget_dict_.end()) {
      TerminalWidget *widget = it.value();
      widget->AddVirtualTerminal(terminal);
      // TODO: 发送接收本地回显的信号

    } else {
      TerminalWidget *new_widget = new TerminalWidget(this);
      new_widget->Initialize(controller_manager_);
      new_widget->AddVirtualTerminal(terminal);
      new_widget->ActivateSpeakControl(is_chairman_);
      terminal_widget_dict_.insert(terminal->uri, new_widget);
      
      // TODO: 发送接收本地回显的信号

      // 在layout中显示该终端
      if (widget_width_ == -1 || widgets_per_row_ == -1) {
        widget_width_ = new_widget->width();
        widgets_per_row_ = CaculateWidgetsPerRow(widget_width_, width());
      }
      if (show_type_ == kTypeOfShowInOrder || 
          (show_type_ == kTypeOfShowOnline && terminal->is_available) ||
          (show_type_ == kTypeOfShowOffline && !terminal->is_available)) {
            AddWidgetToLayout(ui_.gridLayout->count(), new_widget);
      }
    }
  }

  // 当所有HD终端到齐后，按show_type_重新显示终端
  if (terminal_widget_dict_.size() == hd_terminal_count_ + tx_terminal_count_ - 1) {
    ShowTerminalWidgetByOnlineStatus(show_type_);
  }
}

void TerminalListWidget::ActivateSpeakControl( bool is_chairman ) {
  // 检查本地终端是否为chairman
  // 并激活或关闭widget的允许发言控制
  if (is_chairman_ != is_chairman) {
      is_chairman_ = is_chairman;
    for (auto widget: terminal_widget_dict_) {
      widget->ActivateSpeakControl(is_chairman_);
    }
  }
}

int TerminalListWidget::CaculateWidgetsPerRow(
    int widget_width, int layout_width) {
  assert(widget_width != 0);
  int row_count = layout_width / widget_width;
  if (layout_width < (widget_width + 1) * row_count - 1) {
    --row_count;
  }
  return row_count <= 0 ? 1 : row_count;
}

void TerminalListWidget::AddWidgetToLayout( int index, TerminalWidget *widget ) {
  int row = index / widgets_per_row_;
  int col = index - row * widgets_per_row_;
  ui_.gridLayout->addWidget(widget, row, col);
}

void TerminalListWidget::HandleTerminalUpdateNotifySlot( 
    ConstTerminalPointer terminal, const QString &conference_uri) {
  QString local_uri = QString::fromStdString(
    RunningProfile::getInstance()->user_uri());
  if (terminal->uri == local_uri) {
    ActivateSpeakControl(terminal->is_chairman_terminal);
  }

  auto it = terminal_widget_dict_.find(terminal->uri);
  if (it != terminal_widget_dict_.end()) {
    bool need_to_refresh_layout = false;
    TerminalWidget *widget = it.value();
    // TODO: 以后改为只要终端有一个虚拟终端在线，则为在线
    // 只有虚拟终端序列号为0的终端决定在线与离线状态
    if (terminal->virtual_index == 0) {
      if (widget->is_online() != terminal->is_available) {
        need_to_refresh_layout = true;
      }
    }
    widget->UpdateVirtualTerminal(terminal);
    if (need_to_refresh_layout) {
      // TODO: 可以优化
      // 一个终端的上下线，会刷新整个layout
      ShowTerminalWidgetByOnlineStatus(show_type_);
    }
  }
}

void TerminalListWidget::ShowTerminalWidgetByOnlineStatus(ShowType show_type) {
  show_type_ = show_type;
  if (show_type == kTypeOfShowInOrder) {
    ShowTerminalWidgetInOrder();
  } else {
    ShowTerminalWidgetByOnlineStatus(show_type == kTypeOfShowOnline);
  }
}

void TerminalListWidget::ShowTerminalWidgetInOrder() {
  ClearLayout();

  WidgetList widgets;
  widgets.swap(terminal_widget_dict_.values());
  SortTerminalWidgetToList(widgets);
  
  int index = 0;
  for (auto widget : widgets) {
    AddWidgetToLayout(index++, widget);
    widget->setVisible(true);
  }
}

void TerminalListWidget::ShowTerminalWidgetByOnlineStatus( bool online ) {
  ClearLayout();
  int index = 0;
  for (auto widget : terminal_widget_dict_) {
    if (widget->is_online() == online) {
      AddWidgetToLayout(index++, widget);
      widget->setVisible(true);
    } else {
      widget->setVisible(false);
    }
  }
}

void TerminalListWidget::ClearLayout() {
  //QLayoutItem *item;
  //while ((item = ui_.gridLayout->takeAt(0)) != 0) {
  //  ui_.gridLayout->removeItem(item);
  //}
}

void TerminalListWidget::SortTerminalWidgetToList( WidgetList &widgets ) {
  // 将在线的终端排在前面
  std::partition(widgets.begin(), widgets.end(), 
    [](TerminalWidget *widget)->bool {
      return widget->is_online();
    }
  );
}

void TerminalListWidget::HandleReceiveAudioNotifySlot(
    const QString &vuri, bool receive) {
  const QString uri = TerminalHelper::GetTerminalURI(vuri);
  auto it = terminal_widget_dict_.find(uri);
  if (it != terminal_widget_dict_.end()) {
    it.value()->SetAudioReceivedMark(receive);
  }
}

void TerminalListWidget::resizeEvent( QResizeEvent *event ) {
  if (widget_width_ != -1) {
    int new_widget_per_row = CaculateWidgetsPerRow(
      widget_width_, event->size().width());
    if (new_widget_per_row != widgets_per_row_) {
      widgets_per_row_ = new_widget_per_row;
      ShowTerminalWidgetByOnlineStatus(show_type_);
    }
  }
}

void TerminalListWidget::dragEnterEvent( QDragEnterEvent *event ) {
  if (event->mimeData()->hasText()) {
    MimeTextParser parser(event->mimeData()->text());
    if (parser.is_coolview_mime_text()) {
      event->acceptProposedAction();
    }
  }
}

void TerminalListWidget::dropEvent( QDropEvent *event ) {
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

void TerminalListWidget::mouseReleaseEvent( QMouseEvent *event ) {
  if (event->button() == Qt::RightButton) {
    if (menu_) {
      menu_->exec(event->globalPos());
    }
  }
  event->accept();
}

void TerminalListWidget::HandleShowAllTerminalsSlot() {
  if (show_type_ == kTypeOfShowInOrder) {
    ui_.showAllTerminalsAction->setChecked(true);
    return;
  }
  show_type_ = kTypeOfShowInOrder;
  ClearMenu();
  ui_.showAllTerminalsAction->setChecked(true);
  ShowTerminalWidgetInOrder();
}

void TerminalListWidget::HandleShowOnlineTerminalsSlot() {
  if (show_type_ == kTypeOfShowOnline) {
    ui_.showOnlineTerminalsAction->setChecked(true);
    return;
  }
  show_type_ = kTypeOfShowOnline;
  ClearMenu();
  ui_.showOnlineTerminalsAction->setChecked(true);
  ShowTerminalWidgetByOnlineStatus(true);
}

void TerminalListWidget::HandleShowOfflineTerminalsSlot() {
  if (show_type_ == kTypeOfShowOffline) {
    ui_.showOfflineTerminalsAction->setChecked(true);
    return;
  }
  show_type_ = kTypeOfShowOffline;
  ClearMenu();
  ui_.showOfflineTerminalsAction->setChecked(true);
  ShowTerminalWidgetByOnlineStatus(false);
}

void TerminalListWidget::ClearMenu() {
  ui_.showAllTerminalsAction->setChecked(false);
  ui_.showOnlineTerminalsAction->setChecked(false);
  ui_.showOfflineTerminalsAction->setChecked(false);
}

void TerminalListWidget::SetConferenceTitleSlot( const QString &title ) {
  ui_.conferenceTitleLabel->setText(title);
}

void TerminalListWidget::ConfirmAndLeaveConferenceSlot()
{
  QString message = QString::fromLocal8Bit("正在进行会议，确认离开会议吗？");
  QMessageBox::StandardButton res = QMessageBox::warning(
    this, tr("CoolView"), message, QMessageBox::Yes | QMessageBox::No);
  if(res == QMessageBox::Yes) {
    emit LeaveConferenceSignal();
  }
}
