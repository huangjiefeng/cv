#include "record_advanced_panel.h"

#include <windows.h>
#include <shellapi.h>

#include "util/ProcessManager.h"
#include "log/Log.h"

RecordAdvancedPanel::RecordAdvancedPanel(QWidget *parent)
    : QFrame(parent)
{
  ui.setupUi(this);
  connect(ui.resetButton, &QPushButton::clicked,
    this, &RecordAdvancedPanel::ResetInfo);
  ResetInfo();

  //QT官方文档说，启用WA_InputMethodEnabled，外加设置QApplication.setAutoSipEnabled(true);
  //就可以在点击输入框时调用触摸设备屏幕键盘。别扯了，貌似这个只对移动终端有效，网上都说对Windows无效的
  //ui.titleLineEdit->setAttribute(Qt::WA_InputMethodEnabled, true);

  //现在的解决方案是，先启用QT设备的Touch事件，然后通过事件过滤器监听Touch事件，然后手动调用屏幕键盘程序
  ui.titleLineEdit->setAttribute(Qt::WA_AcceptTouchEvents);
  ui.titleLineEdit->installEventFilter(this);
  ui.keywordsLineEdit->setAttribute(Qt::WA_AcceptTouchEvents);
  ui.keywordsLineEdit->installEventFilter(this);
  ui.participantsLineEdit->setAttribute(Qt::WA_AcceptTouchEvents);
  ui.participantsLineEdit->installEventFilter(this);
  ui.descTextEdit->setAttribute(Qt::WA_AcceptTouchEvents);
  ui.descTextEdit->installEventFilter(this);
}

RecordAdvancedPanel::~RecordAdvancedPanel()
{

}

void RecordAdvancedPanel::ResetInfo()
{
  ui.titleLineEdit->setText("");
  ui.keywordsLineEdit->setText("");
  ui.participantsLineEdit->setText("");
  ui.descTextEdit->setPlainText("");
}

void RecordAdvancedPanel::GetRecordInfo( StartRecordParam &param )
{
  param.title = ui.titleLineEdit->text();
  param.keywords = ui.keywordsLineEdit->text();
  param.participants = ui.participantsLineEdit->text();
  param.description = ui.descTextEdit->toPlainText();
}

bool RecordAdvancedPanel::eventFilter(QObject *watched, QEvent *event)
{
  /*if (QEvent::MouseMove != event->type()) {
  _snprintf(__global_msg , sizeof(__global_msg) , "Event %d", event->type());
  CONFROOM_LOG_INFO(__global_msg);
  }*/
  switch(event->type())
  {
  case QEvent::TouchBegin:
  case QEvent::TouchUpdate:
  case QEvent::TouchEnd:
  case QEvent::RequestSoftwareInputPanel:
    //嵌入式系统上QProcess没用……
    /*QProcess *process = new QProcess(this);
    process->start("osk", QStringList());*/

    //如果已经启动了，不要再启动，不然键盘程序一直抢焦点，没法输入
    if (!ProcessManager::isRunning(TEXT("osk.exe"))) {
      ShellExecute(NULL , TEXT("open"), TEXT("osk.exe"), NULL, NULL,  SW_SHOW);
    }
    //CONFROOM_LOG_INFO("osk started");
    break;
  };
  return QObject::eventFilter(watched, event);
}
