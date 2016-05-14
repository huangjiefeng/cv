#ifndef HAND_UP_WIDGET_H
#define HAND_UP_WIDGET_H

#include <QWidget>
#include "ui_hand_up_widget.h"

class IConferenceController;

class HandUpWidget : public QWidget {
  Q_OBJECT

public:
  HandUpWidget(QWidget *parent = 0);
  ~HandUpWidget();

  void Initialize(IConferenceController *controller);
  void Reset();

Q_SIGNALS:
  void HandUpSignal(bool handup);

private Q_SLOTS:
  void HandUpSlot();
  void HandleHandUpNotifySlot(bool handup);

private:
  Ui::HandUpWidget ui_;
  bool is_handup_;
};

#endif // HAND_UP_WIDGET_H
