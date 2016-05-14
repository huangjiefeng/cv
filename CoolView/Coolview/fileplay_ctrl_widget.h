#ifndef FILE_PLAY_CTRL_WIDGET_H
#define FILE_PLAY_CTRL_WIDGET_H

#include <QWidget>
#include "ui_fileplay_ctrl_widget.h"

class IFilePlayController;

class FilePlayCtrlWidget : public QWidget
{
    Q_OBJECT

public:
    FilePlayCtrlWidget(QWidget *parent = 0);
    ~FilePlayCtrlWidget();

    void Initialize(IFilePlayController *fileplay_controller);
    void Reset();

    bool eventFilter(QObject *watched, QEvent *event) override;

public Q_SLOTS:
    void HandleNotifyPlayProgressSlot(unsigned long pos, unsigned long duration);
    void HandlePauseButtonClickedSlot();

private:
    void SetPosition(unsigned long pos);

private:
    Ui::FilePlayCtrlWidget ui;

    IFilePlayController *fileplay_controller_;

    unsigned long duration_in_sec_;
    bool paused_;
    bool is_dragging_progressbar_;
    bool is_seeking_;
    int dragging_value_;
};

#endif // FILE_PLAY_CTRL_WIDGET_H
