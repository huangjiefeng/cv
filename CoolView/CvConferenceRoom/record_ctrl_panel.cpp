#include "record_ctrl_panel.h"

RecordCtrlPanel::RecordCtrlPanel(QWidget *parent)
    : QWidget(parent)
{
  ui.setupUi(this);
}

RecordCtrlPanel::~RecordCtrlPanel()
{

}

void RecordCtrlPanel::Initialize()
{
  SwitchRecBtnToStart();
  show_edit_info_panel_ = false;

  connect(ui.RecordButton, &QPushButton::clicked, 
    this, &RecordCtrlPanel::HandleRecBtnClickedSlot);
  connect(ui.RecInfoEditButton, &QPushButton::clicked, 
    this, &RecordCtrlPanel::HandleRecInfoEditBtnClickedSlot);
}

QLabel * RecordCtrlPanel::GetStateLabel()
{
  return ui.stateLabel;
}

void RecordCtrlPanel::SwitchRecBtnToStart()
{
  rec_btn_state_start_ = true;
  ui.RecordButton->setIcon(QIcon(":/CvConferenceRoom/Resources/Record-Start.png"));
}

void RecordCtrlPanel::SwitchRecBtnToStop()
{
  rec_btn_state_start_ = false;
  ui.RecordButton->setIcon(QIcon(":/CvConferenceRoom/Resources/Record-Stop.png"));
}

void RecordCtrlPanel::EnableRecBtn( bool enable /*= true*/ )
{
  ui.RecordButton->setEnabled(enable);
}

void RecordCtrlPanel::HandleRecBtnClickedSlot()
{
  if (rec_btn_state_start_) {
    emit StartRecordSignal();
  } else {
    emit StopRecordSignal();
  }
}

void RecordCtrlPanel::HandleRecInfoEditBtnClickedSlot()
{
  show_edit_info_panel_ = !show_edit_info_panel_;
  emit ShowRecInfoPanelSignal(show_edit_info_panel_);
}
