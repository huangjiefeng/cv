#ifndef KINECTINFOWIDGET_H
#define KINECTINFOWIDGET_H

#include <QWidget>
#include "ui_kinectinfowidget.h"

class KinectInfoWidget : public QWidget
{
	Q_OBJECT

public:
	KinectInfoWidget(QWidget *parent = 0);
	~KinectInfoWidget();

	void SetInfoText( const QString& message );

private:
	Ui::KinectInfoWidgetClass ui;
};

#endif // KINECTINFOWIDGET_H
