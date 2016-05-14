#ifndef RECORD_CTRL_PANEL_H
#define RECORD_CTRL_PANEL_H

#include <QWidget>
#include "ui_record_ctrl_panel.h"

class RecordCtrlPanel : public QWidget
{
    Q_OBJECT

public:
    RecordCtrlPanel(QWidget *parent = 0);
    ~RecordCtrlPanel();

    void Initialize();

    void SwitchRecBtnToStart();
    void SwitchRecBtnToStop();
    void EnableRecBtn(bool enable = true);
    QLabel * GetStateLabel();

public slots:
    void HandleRecBtnClickedSlot();
    void HandleRecInfoEditBtnClickedSlot();

Q_SIGNALS:
    void StartRecordSignal();
    void StopRecordSignal();
    void ShowRecInfoPanelSignal(bool);

private:
    Ui::RecordCtrlPanel ui;

    bool rec_btn_state_start_;
    bool show_edit_info_panel_;
};

#endif // RECORD_CTRL_PANEL_H
