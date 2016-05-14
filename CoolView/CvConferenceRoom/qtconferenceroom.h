#ifndef QTCONFERENCEROOM_H
#define QTCONFERENCEROOM_H

#include <QtCore/QtCore>
#include "ui_qtconferenceroom.h"

class QtMeetingPlaceWidget;
class QScrollArea;
class ConferenceRoomService;
class QtScreenShare;
class QtConferenceRoom : public QWidget
{
	Q_OBJECT

public:
	QtConferenceRoom(int screenIndex , QWidget *parent = 0);
	~QtConferenceRoom();

	QtMeetingPlaceWidget* getMeetingPlaceWidget() { return _meetingPlaceWidget; }
	QtScreenShare*	getScreenShareWidget() { return _screenShareWidget; }

	///显示视频窗口界面
	void showScreenShare();
    void showMaximizedToScreen();
    void showTopHalfToScreen();
	void showMeetringPlace();
    int getScreenIndex() const { return screen_index_; }

protected:
	//void closeEvent( QCloseEvent * event  );

private:
	Ui::QtConferenceRoomClass ui;

	QtMeetingPlaceWidget*	_meetingPlaceWidget;
	QtScreenShare*			_screenShareWidget;
	QScrollArea*			_scrollArea;
  int screen_index_;
};

#endif // QTCONFERENCEROOM_H
