/************************************************************************/
/* CoolView 3.0 ½çÃæÔªËØ
 * zhenHua.sun 2010-08-16
 */
/************************************************************************/
#pragma once
#include "ui_TextChatWidget.h"
#include "QtConferenceRoomWidget.h"
#include <model/conference/CVMsgParser.h>
#include <QtGui/QtGui>
class QtWengoPhone;
class CWengoPhone;
//class QtConferenceRoomWidget;
class QtToolWidget;

class QtTextChatWidget:public QWidget
{
	Q_OBJECT
public:
	//QtTextChatWidget(QtWengoPhone & qtWengoPhone,QtConferenceRoomWidget *confRoom);
	QtTextChatWidget(QtWengoPhone & qtWengoPhone,QtToolWidget *qtTool);
	~QtTextChatWidget(void);

	void cvTextMsgReceivedEventHandler(const std::string &message,const std::string &from);
	void cvMemberListReceivedEventHandler(vector<MemberInfo *> &MemberList,std::string ConfTitle);
	void clearTextWidget();

Q_SIGNALS:
	void cvTextMsgReceivedEventHandlerSinal(const QString& message,const QString& from);

private Q_SLOTS:
	void sendButtonClicked();
	void clearButtonClicked();
	void cvTextMsgReceivedEventHandlerSlot(const QString& message,const QString& from);
	void Save();
protected:
	virtual bool eventFilter(QObject*, QEvent*);

private:
	Ui::TextChatWidget *_ui;
	QtWengoPhone & _qtWengoPhone;

	CWengoPhone & _cWengoPhone;

	//QtConferenceRoomWidget *_confRoom;
	QtToolWidget* _qtTool;
};
