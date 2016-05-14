#ifndef CVCONFERENCEROOM_H
#define CVCONFERENCEROOM_H

#include <QtGui/QWidget>
#include "ui_cvconferenceroom.h"

class CvConferenceRoom : public QWidget
{
	Q_OBJECT

public:
	CvConferenceRoom(QWidget *parent = 0, Qt::WFlags flags = 0);
	~CvConferenceRoom();

private:
	Ui::CvConferenceRoomClass ui;
};

#endif // CVCONFERENCEROOM_H
