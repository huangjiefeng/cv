#pragma once
#include "ui_whiteBoard.h"
#include <QtGui/QtGui>
class QtWengoPhone;
class CWengoPhone;
class QtConferenceRoomWidget;
class CDrawPanel;

class QtWhiteBoardWidget:public QWidget
{
	Q_OBJECT
public:
	QtWhiteBoardWidget(QtWengoPhone & qtWengoPhone,QtConferenceRoomWidget *confRoom);
	~QtWhiteBoardWidget(void);
	void cvWhiteBoardMsgReceivedEventHandler(const std::string &message,const std::string &from);
	void sendWhiteBoardMsg(std::string &content);
	void clearWhiteBoard();

	Ui::WhiteBoard *_ui;

Q_SIGNALS:
	void cvWhiteBoardMsgReceivedEventHandlerSinal(const QString& message,const QString& from);

private Q_SLOTS:
	void SetDrawLine();
	void SetDrawRect();
	void SetDrawEill();
	void SetDrawHand();
	void ClearPanel();
	void RedoDraw();
	void UndoDraw();
	void cvWhiteBoardMsgReceivedEventHandlerSlot(const QString& message,const QString& from);
	void Save();
	void Open();

private:
	CDrawPanel *panel;
	QtWengoPhone & _qtWengoPhone;

	CWengoPhone & _cWengoPhone;

	QtConferenceRoomWidget *_confRoom;

	QPixmap originalPixmap;
};
