#ifndef HAND_UP_WIDGET_H
#define HAND_UP_WIDGET_H

#include <QPushButton>

class IConferenceController;

class HandUpButton : public QPushButton {
  Q_OBJECT

public:
  HandUpButton(QWidget *parent = 0);
  ~HandUpButton();

  void Initialize(IConferenceController *controller);
  void Reset();

Q_SIGNALS:
  void HandUpSignal(bool handup);

private Q_SLOTS:
  void HandUpSlot();
  void HandleHandUpNotifySlot(bool handup);

private:
  void SetHandUpStatus(bool handup);

private:
  bool is_handup_;
};

#endif // HAND_UP_WIDGET_H
