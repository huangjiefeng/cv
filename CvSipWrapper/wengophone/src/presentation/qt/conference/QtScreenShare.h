#pragma once
#include "ui_screenShare.h"
#include <QtGui/QtGui>
class QtWengoPhone;
class CWengoPhone;
class QtConferenceRoomWidget;

class QtScreenShare:public QWidget
{
	Q_OBJECT
public:
	QtScreenShare(QtWengoPhone & qtWengoPhone,QtConferenceRoomWidget *confRoom);
	~QtScreenShare(void);
	void stopScreenGraph();
	void cvHostUnShareScreenEventHandler();
Q_SIGNALS:
	void cvHostUnShareScreenSinal();
private Q_SLOTS:
	void openFile();
	void closeFile();
	void sendScreen();
	void recvScreen();
	void cvHostUnShareScreenSlot();

protected:
	virtual bool eventFilter(QObject*, QEvent*);
	virtual void resizeEvent ( QResizeEvent * event );

private:
	Ui::ScreenShare *_ui;
	QWidget *widget;
	bool firstOpenFile;
	bool isSendScreen;
	bool isRecvScreen;

	QtWengoPhone & _qtWengoPhone;

	CWengoPhone & _cWengoPhone;
	QtConferenceRoomWidget *_confRoom;
};
