#pragma once

#ifndef QTINSTANTCONFERENCEWIDGET_H
#define QTINSTANTCONFERENCEWIDGET_H
#include "ui_InstantConferenceWidget.h"
#include <QtGui/QDialog>

class CWengoPhone;

class QtInstantConferenceWidget:public QDialog
{
	Q_OBJECT
public:
	QtInstantConferenceWidget(QWidget * parent, CWengoPhone & cWengoPhone);
	~QtInstantConferenceWidget();

private Q_SLOTS:

	/**
	* The user has clicked the start button.
	*/
	void startClicked();
	void selectJoinMode(int index);

private:
	CWengoPhone & _cWengoPhone;
	Ui::InstantConferenceWidget *_ui;
};

#endif
