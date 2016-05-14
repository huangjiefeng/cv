#ifndef RECORD_ADVANCED_PANEL_H
#define RECORD_ADVANCED_PANEL_H

#include <QWidget>
#include "ui_record_advanced_panel.h"
#include "dbus/txController/common/TxControllerServiceCommon.h"

class RecordAdvancedPanel : public QFrame
{
    Q_OBJECT

public:
    RecordAdvancedPanel(QWidget *parent = 0);
    ~RecordAdvancedPanel();

    bool eventFilter(QObject *watched, QEvent *event);

public Q_SLOTS:
    void ResetInfo();
    void GetRecordInfo(StartRecordParam &param);

private:
    Ui::RecordAdvancedPanel ui;
};

#endif // RECORD_ADVANCED_PANEL_H
