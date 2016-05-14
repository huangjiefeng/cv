#pragma once

#include "ui_PasswordInputWidget.h"
#include <QtGui/QDialog>

class CWengoPhone;
class QtPasswordInputWidget:public QDialog
{
	Q_OBJECT
public:
	QtPasswordInputWidget(QWidget * parent, CWengoPhone & cWengoPhone, int row);
	~QtPasswordInputWidget(void);

private Q_SLOTS:

	/**
	* The user has clicked the start button.
	*/
	void startClicked();

private:
	CWengoPhone & _cWengoPhone;
	Ui::PasswordInputWidget *_ui;

	int _row;
};
