#include "stdafx.h"
#include ".\qtwhiteboardwidget.h"
#include <util/SafeDelete.h>
#include <qtutil/SafeConnect.h>
#include "QtWengoPhone.h"
#include <control/profile/CUserProfile.h>
#include <control/profile/CUserProfileHandler.h>
#include <control/CWengoPhone.h>
#include "QtConferenceInvite.h"
#include "DrawPanel.h"
#include "MessageFrame.h"
#include <QtGui/QtGui>

QtWhiteBoardWidget::QtWhiteBoardWidget(QtWengoPhone & qtWengoPhone,QtConferenceRoomWidget *confRoom):_qtWengoPhone(qtWengoPhone),_cWengoPhone(_qtWengoPhone.getCWengoPhone()),_confRoom(confRoom)
{
	_ui = new Ui::WhiteBoard;
	_ui->setupUi(this);
	
	SAFE_CONNECT(_ui->lineButton, SIGNAL(clicked()), SLOT(SetDrawLine()));
	SAFE_CONNECT(_ui->rectButton, SIGNAL(clicked()), SLOT(SetDrawRect()));
	SAFE_CONNECT(_ui->eillButton, SIGNAL(clicked()), SLOT(SetDrawEill()));
	SAFE_CONNECT(_ui->handButton, SIGNAL(clicked()), SLOT(SetDrawHand()));
	SAFE_CONNECT(_ui->clearButton, SIGNAL(clicked()),SLOT(ClearPanel()));
	SAFE_CONNECT(_ui->redoButton, SIGNAL(clicked()), SLOT(RedoDraw()));
	SAFE_CONNECT(_ui->undoButton, SIGNAL(clicked()), SLOT(UndoDraw()));
	SAFE_CONNECT(_ui->saveButton, SIGNAL(clicked()), SLOT(Save()));
	SAFE_CONNECT(_ui->openButton, SIGNAL(clicked()), SLOT(Open()));
	SAFE_CONNECT_TYPE(this, SIGNAL(cvWhiteBoardMsgReceivedEventHandlerSinal(const QString&,const QString&)),SLOT(cvWhiteBoardMsgReceivedEventHandlerSlot(const QString&,const QString&)), Qt::QueuedConnection);

	panel = new CDrawPanel(this);
	_ui->scrollArea->setWidget(panel);

	CUserProfile * cUserProfile = _cWengoPhone.getCUserProfileHandler().getCUserProfile();
	cUserProfile->cvWhiteBoardMsgReceivedEvent +=boost::bind(&QtWhiteBoardWidget::cvWhiteBoardMsgReceivedEventHandler,this,_2,_3);
}

QtWhiteBoardWidget::~QtWhiteBoardWidget(void)
{
	CUserProfile * cUserProfile = _cWengoPhone.getCUserProfileHandler().getCUserProfile();
	cUserProfile->cvWhiteBoardMsgReceivedEvent -=boost::bind(&QtWhiteBoardWidget::cvWhiteBoardMsgReceivedEventHandler,this,_2,_3);
	OWSAFE_DELETE(panel);
	OWSAFE_DELETE(_ui);
}

void QtWhiteBoardWidget::SetDrawLine()
{
	panel->SetDrawLine();
}

void QtWhiteBoardWidget::SetDrawRect()
{
	panel->SetDrawRect();
}

void QtWhiteBoardWidget::SetDrawEill()
{
	panel->SetDrawEill();
}

void QtWhiteBoardWidget::SetDrawHand()
{
	panel->SetDrawHand();
}

void QtWhiteBoardWidget::ClearPanel()
{
	panel->OnDrawPanelBarMSG(7);
}

void QtWhiteBoardWidget::RedoDraw()
{
	panel->OnDrawPanelBarMSG(6);
}

void QtWhiteBoardWidget::UndoDraw()
{
	panel->OnDrawPanelBarMSG(5);
}

void QtWhiteBoardWidget::cvWhiteBoardMsgReceivedEventHandler(const std::string &message,const std::string &from)
{
	if(message=="")
		return;
	cvWhiteBoardMsgReceivedEventHandlerSinal(QString(message.c_str()),QString(from.c_str()));

}

void QtWhiteBoardWidget::cvWhiteBoardMsgReceivedEventHandlerSlot(const QString &message,const QString &from)
{
	panel->OnNetDrawMsg(message.toLatin1().data());
	
}

void QtWhiteBoardWidget::sendWhiteBoardMsg(string &content)
{
	CUserProfile * cUserProfile = _cWengoPhone.getCUserProfileHandler().getCUserProfile();
	string message = "<?xml version=\"1.0\"?><coolview command=\"WhiteBoard\"><content>"+content +"</content></coolview>";
	cUserProfile->sendTextMsg(0,message);
}

void QtWhiteBoardWidget::clearWhiteBoard()
{
	panel->ClearPanel();
}

void QtWhiteBoardWidget::Save()
{
	originalPixmap = QPixmap(); 
	originalPixmap = QPixmap::grabWindow(panel->winId());

	QString format = "png";
	QString whiteBoardFormat = "cvwb";
	QString initialPath = QDir::currentPath() + tr("/untitled");

	QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
		initialPath,
		tr("%1 Files (*.%2);;CoolView WhiteBoard Files (*.cvwb);;All Files (*)")
		.arg(format.toUpper())
		.arg(format));
	if (!fileName.isEmpty())
	{
		if(fileName.endsWith(whiteBoardFormat))
			panel->Save(fileName);
		else
			originalPixmap.save(fileName, format.toAscii());
	}
}

void QtWhiteBoardWidget::Open()
{
	QString fileName = QFileDialog::getOpenFileName(this,tr("Open File"),QDir::currentPath(),"Files (*.cvwb)");
	panel->Open(fileName);
}
