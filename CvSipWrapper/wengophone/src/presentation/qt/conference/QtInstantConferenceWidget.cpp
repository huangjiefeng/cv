#include "stdafx.h"

#include ".\qtinstantconferencewidget.h"
#include <model/phoneline/IPhoneLine.h>
#include <model/phonecall/ConferenceCall.h>
#include <model/config/ConfigManager.h>
#include <model/config/Config.h>
#include <util/SafeDelete.h>
#include <util/Logger.h>
#include <qtutil/WidgetBackgroundImage.h>
#include <qtutil/SafeConnect.h>
#include "QtWengoPhone.h"
#include <control/profile/CUserProfile.h>
#include <control/profile/CUserProfileHandler.h>
#include <control/CWengoPhone.h>
#include <QtGui/QtGui>

QtInstantConferenceWidget::QtInstantConferenceWidget(QWidget * parent,CWengoPhone & cWengoPhone): QDialog(parent),  _cWengoPhone(cWengoPhone)
{
	_ui = new Ui::InstantConferenceWidget();
	_ui->setupUi(this);
	WidgetBackgroundImage::setBackgroundImage(_ui->headerLabel, ":/pics/conference/setup_banner.jpg", WidgetBackgroundImage::AdjustSize);
	SAFE_CONNECT(_ui->startButton, SIGNAL(clicked()), SLOT(startClicked()));
	SAFE_CONNECT(_ui->cancelButton, SIGNAL(clicked()), SLOT(reject()));
	SAFE_CONNECT(_ui->comboBox_2, SIGNAL(currentIndexChanged(int)), SLOT(selectJoinMode(int)));
}

QtInstantConferenceWidget::~QtInstantConferenceWidget(void)
{
	OWSAFE_DELETE(_ui);
}

//-------Modify By LZY 2010-09-28-------
void QtInstantConferenceWidget::startClicked()
{
	if ((!_ui->phoneNumber1LineEdit->text().isEmpty())) 
	{
		QtInstantConferenceWidget::_cWengoPhone._qtWengoPhone->CreateMeeting(_ui->phoneNumber1LineEdit->text(),_ui->confDescriptionLineEdit->text(),_ui->comboBox->currentIndex(),_ui->comboBox_2->currentIndex(),_ui->pwLineEdit->text());
		accept();

	} else {
			_ui->phoneNumber1LineEdit->setFocus();
	}
}
//-------End of Modify By LZY---------

void QtInstantConferenceWidget::selectJoinMode(int index)
{
	if (_ui->comboBox_2->currentIndex()==1)
	{
		_ui->pwLineEdit->setDisabled(false);
	}
	else
		_ui->pwLineEdit->setDisabled(true);
}