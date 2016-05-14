#include "stdafx.h"
#include ".\qtpasswordinputwidget.h"
#include <model/phoneline/IPhoneLine.h>
#include <model/phonecall/ConferenceCall.h>

#include <util/SafeDelete.h>
#include <util/Logger.h>
#include <qtutil/WidgetBackgroundImage.h>
#include <qtutil/SafeConnect.h>
#include "QtWengoPhone.h"
#include <control/profile/CUserProfile.h>
#include <control/profile/CUserProfileHandler.h>
#include <control/CWengoPhone.h>
#include <QtGui/QtGui>

QtPasswordInputWidget::QtPasswordInputWidget(QWidget * parent, CWengoPhone & cWengoPhone, int row): QDialog(parent),  _cWengoPhone(cWengoPhone),_row(row)
{
	_ui = new Ui::PasswordInputWidget();
	_ui->setupUi(this);

	SAFE_CONNECT(_ui->startButton, SIGNAL(clicked()), SLOT(startClicked()));
	SAFE_CONNECT(_ui->cancelButton, SIGNAL(clicked()), SLOT(reject()));
}

QtPasswordInputWidget::~QtPasswordInputWidget(void)
{
	OWSAFE_DELETE(_ui);
}

void QtPasswordInputWidget::startClicked()
{
	if ((!_ui->pwLineEdit->text().isEmpty())) 
	{
		std::string password = _ui->pwLineEdit->text().toStdString();
		CUserProfile * cUserProfile = _cWengoPhone.getCUserProfileHandler().getCUserProfile();
//		std::string szContent = "<?xml version=\"1.0\"?><coolview command=\"Login\"><password>"+password+"</password></coolview>";
		int result = cUserProfile->checkPassword(_row,password);
		if(result == 0)
			accept();
		else
		{
			QMessageBox::warning(this, tr("@product@ - Password Error"),
				tr("the password you input is incorrect"), QMessageBox::Ok);
			QtPasswordInputWidget::_cWengoPhone._qtWengoPhone->SetUIState(UI_S_LOGIN);//Add By LZY 2010-09-30 用于通知遥控器界面状态变化
		}

	} else {
		_ui->pwLineEdit->setFocus();
	}
}