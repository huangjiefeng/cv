/** 
 * Coolview3.0 界面元素
 * zhenHua.sun 2010-08-13
 */

#pragma once
#include "ui_ConferenceListWidget.h"
#include <model/conference/CVMsgParser.h>
#include "QtConfListTree.h"
#include <QtGui/QtGui>
#include <qtutil/QObjectThreadSafe.h>
#include <cutil/global.h>
#include <util/Trackable.h>
class QtWengoPhone;
class CWengoPhone;
class QtPasswordInputWidget; //Add by hexianfu 09-12-5
class QtToolWidget;

class QtConferenceList:public QWidget
{
Q_OBJECT
public:
	QtConferenceList(QtWengoPhone & qtWengoPhone,QtToolWidget *tools);
	~QtConferenceList(void);

	void ShowConfList(vector<ConfInfo *> &confList);
	void SendGetConfListMsg();
	void sendQosParam();

	/**
	* Add by: hexianfu
	* reload the JoinConference(const QModelIndex&proxyIndex);
	* 2009-11-3
	*/
	int JoinConference(const int& row,std::string PassWord="");//Modify By LZY 2010-09-20 添加参数PassWord，把返回值从void改为int
    /******************************************/

//	QAbstractItemModel *createModel(vector<ConfInfo *> &confList);
//	void addConference(QAbstractItemModel *model, const QString &title,const QString &startTime);
	void cvConfListReceivedEventHandler(vector<ConfInfo *> ConfList);
	void cvConfrenceInviteEventHandler(ConfInfo * conference,const std::string & from);
	void cvPasswordIncorrectEventHandler();

Q_SIGNALS:
	void cvConfrenceInviteEventHandlerSinal(ConfInfo * conference,const QString& from);
	void cvPasswordIncorrectEventHandlerSinal();

private Q_SLOTS:

	void refreshButtonClicked();
	void instantConfButtonClicked();
	void JoinConference(const QModelIndex&proxyIndex);
	void cvConfrenceInviteEventHandlerSlot(ConfInfo * conference,const QString& from);
	void cvPasswordIncorrectEventHandlerSlot();
	void teleEnterMeetingCommand(QString EnterMeetingStr);//Add By LZY 2010-09-20 处理EnterMeeting动作Slot

private:
	QtWengoPhone & _qtWengoPhone;

	CWengoPhone & _cWengoPhone;

	QStandardItemModel *sourceModel;
	QtConfListTree *confTreeModel;

private:
	Ui::ConferenceList *_ui;
	QSortFilterProxyModel *_proxyModel;
	//QtConferenceRoomWidget *_confRoom;
	QtToolWidget *_tools;

};
