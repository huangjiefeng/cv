#include "stdafx.h"
#include ".\qttextchatwidget.h"
#include <util/SafeDelete.h>
#include <qtutil/SafeConnect.h>
#include "QtWengoPhone.h"
#include <control/profile/CUserProfile.h>
#include <control/profile/CUserProfileHandler.h>
#include <control/CWengoPhone.h>
#include "QtConferenceInvite.h"
#include <QtGui/QtGui>

#include "QtToolWidget.h"

QtTextChatWidget::QtTextChatWidget(QtWengoPhone & qtWengoPhone,QtToolWidget* qtTool):_qtWengoPhone(qtWengoPhone),_cWengoPhone(_qtWengoPhone.getCWengoPhone()),_qtTool(qtTool)
{
	_ui = new Ui::TextChatWidget();
	_ui->setupUi(this);

	SAFE_CONNECT(_ui->sendButton, SIGNAL(clicked()), SLOT(sendButtonClicked()));
	SAFE_CONNECT(_ui->clearButton, SIGNAL(clicked()), SLOT(clearButtonClicked()));
	SAFE_CONNECT(_ui->saveButton, SIGNAL(clicked()), SLOT(Save()));
	SAFE_CONNECT_TYPE(this, SIGNAL(cvTextMsgReceivedEventHandlerSinal(const QString&,const QString&)),SLOT(cvTextMsgReceivedEventHandlerSlot(const QString&,const QString&)), Qt::QueuedConnection);

	CUserProfile * cUserProfile = _cWengoPhone.getCUserProfileHandler().getCUserProfile();
	cUserProfile->cvTextMsgReceivedEvent +=boost::bind(&QtTextChatWidget::cvTextMsgReceivedEventHandler,this,_2,_3);
	cUserProfile->cvMemberListReceivedEvent +=boost::bind(&QtTextChatWidget::cvMemberListReceivedEventHandler,this,_2,_3);

	installEventFilter(this);
}

QtTextChatWidget::~QtTextChatWidget(void)
{
	CUserProfile * cUserProfile = _cWengoPhone.getCUserProfileHandler().getCUserProfile();
	cUserProfile->cvTextMsgReceivedEvent -=boost::bind(&QtTextChatWidget::cvTextMsgReceivedEventHandler,this,_2,_3);
	cUserProfile->cvMemberListReceivedEvent -=boost::bind(&QtTextChatWidget::cvMemberListReceivedEventHandler,this,_2,_3);
	OWSAFE_DELETE(_ui);
}

void QtTextChatWidget::sendButtonClicked()
{
	if (_ui->lineEdit->text().isEmpty()||_ui->ReceiverComboBox->count()==0)
	{
		return;
	}
	QString sendMsg = QApplication::translate("TextChatWidget", "\346\210\221\350\257\264: ", 0, QApplication::UnicodeUTF8)+_ui->lineEdit->text();
	_ui->plainTextEdit->appendPlainText(sendMsg);
	_ui->plainTextEdit->moveCursor(QTextCursor::End);
	CUserProfile * cUserProfile = _cWengoPhone.getCUserProfileHandler().getCUserProfile();
	QString message = "<?xml version=\"1.0\"?><coolview command=\"Text\"><message>"+_ui->lineEdit->text() +"</message></coolview>";
	cUserProfile->sendTextMsg(_ui->ReceiverComboBox->currentIndex(),message.toUtf8().constData());
	_ui->lineEdit->clear();
}

void QtTextChatWidget::clearButtonClicked()
{
	_ui->plainTextEdit->clear();
}

void QtTextChatWidget::cvTextMsgReceivedEventHandler(const std::string &message,const std::string &from)
{
	QString content = QApplication::translate("TextChatWidget", message.c_str(), 0, QApplication::UnicodeUTF8);
	cvTextMsgReceivedEventHandlerSinal(content,QString(from.c_str()));
}

void QtTextChatWidget::cvTextMsgReceivedEventHandlerSlot(const QString &message,const QString &from)
{
	QString user = from + QApplication::translate("TextChatWidget", "\350\257\264: ", 0, QApplication::UnicodeUTF8);
	_ui->plainTextEdit->appendPlainText(user+message);
	_ui->plainTextEdit->moveCursor(QTextCursor::End);
}

bool QtTextChatWidget::eventFilter(QObject * object, QEvent * event) {
	if (event->type() == QEvent::KeyPress) {
		QKeyEvent * keyEvent = static_cast<QKeyEvent *>(event);

		if (keyEvent->key() == Qt::Key_Return){
			sendButtonClicked();
			return true;
		} 

	}

	return false;
}

void QtTextChatWidget::cvMemberListReceivedEventHandler(vector<MemberInfo *> &MemberList,std::string ConfTitle) 
{
	QStringList members;
	members.append(tr("\346\211\200\346\234\211\344\272\272"));
	for (int i=1;i<MemberList.size();i++)
	{
		int index = MemberList[i]->MemberURI.find('@',0);
		string displayName = MemberList[i]->MemberURI.substr(0,index);
		members.append(displayName.c_str());
	}
	_ui->ReceiverComboBox->clear();
	_ui->ReceiverComboBox->addItems(members);
}

void QtTextChatWidget::clearTextWidget()
{
	_ui->plainTextEdit->clear();
	_ui->ReceiverComboBox->clear();
}

void QtTextChatWidget::Save()
{
	QString format = "txt";
	QString initialPath = QDir::currentPath() + tr("/untitled.") + format;

	QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
		initialPath,
		tr("%1 Files (*.%2);;All Files (*)")
		.arg(format.toUpper())
		.arg(format));
	if (!fileName.isEmpty())
	{
		QFile file(fileName);
		file.open(QIODevice::ReadWrite);
		QTextStream out(&file);
		out<<_ui->plainTextEdit->toPlainText();
		file.close();
	}
}
