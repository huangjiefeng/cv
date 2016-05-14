#ifndef CONFERENCE_WIDGET_H
#define CONFERENCE_WIDGET_H

#include <QWidget>
#include "ui_conference_list_widget.h"

#include "conference_controller_interface.h"
#include "session_controller_interface.h"

class IControllerManager;

class ConferenceWidget : public QWidget {
  Q_OBJECT
public:
  ConferenceWidget(QWidget *parent = 0);
  ~ConferenceWidget();

  void Initialize(IControllerManager *controller_manager);

Q_SIGNALS:
  // 向conference controller发送的信号
  void JoinConferenceSignal(const QString &conference_uri);
  void LeaveConferenceSignal();
  void RequestConferenceListSignal();

  // 设置主窗口的标题
  void SetWindowTitleSignal(const QString &title);

public Q_SLOTS:
  void HandleSessionStateChangedNotifySlot(
    ISessionController::SessionState state);

  void HandleConferenceListReceivedNotifySlot(
    const ConstConferenceList &conferences);

  void HandleConferenceStateChangedNotifySlot(
    const QString &conference_uri,
    IConferenceController::ConferenceState state);

  void HandleJoinConferenceErrorNotifySlot(
    const QString &conference_uri, 
    IConferenceController::JoinConferenceError error);

private Q_SLOTS:
  void TryToJoinConferenceSlot();
  void ShowConferenceTipSlot(int index);
  void ShowHelpWidgetSlot();
  void UpdateConferenceListSlot();

private:
  void HandleJoinConferenceNotify(const QString &conference_uri);
  void HandleLeaveConferenceNotify();

  void SetupGui();
  void SetConferenceTip(ConstConferencePointer conference);

  // 将分钟转化为xx天xx时xx分xx秒格式的字符串
  static QString ConvertDurationMinToQString(int mins);

private:
  Ui::ConferenceListWidget ui_;

  // 存储会议列表
  // key: uri, value: conference
  ConstConferenceDictionary conference_dict_;
  
  // 记录当前正在进行的会议
  QString current_conference_uri_;
  // 记录离开会议后马上要进入的会议
  QString attempt_conference_uri_;

  QWidget *help_widget_;
  
  bool has_initialized_;
};

#endif // CONFERENCE_WIDGET_H
