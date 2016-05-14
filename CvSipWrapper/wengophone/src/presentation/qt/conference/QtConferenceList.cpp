/** 
 * Coolview3.0 界面元素
 * zhenHua.sun 2010-08-13
 */

#include "stdafx.h"

#include ".\qtconferencelist.h"
#include <util/SafeDelete.h>
#include <qtutil/SafeConnect.h>
#include "QtWengoPhone.h"
#include <control/profile/CUserProfile.h>
#include <control/profile/CUserProfileHandler.h>
#include <control/CWengoPhone.h>
#include <model/config/ConfigManager.h>
#include <model/config/Config.h>
#include "QtMemberList.h"
#include "QtInstantConferenceWidget.h"
#include "QtPasswordInputWidget.h"
#include <QtGui/QtGui>
#include "ExportUtil.h"
#include "QtToolWidget.h"
#include "control/MessageFrame.h"

QtConferenceList::QtConferenceList(QtWengoPhone & qtWengoPhone,QtToolWidget *tools):_qtWengoPhone(qtWengoPhone),_cWengoPhone(_qtWengoPhone.getCWengoPhone()),_tools(tools)
{
	_ui = new Ui::ConferenceList();
	_proxyModel = new QSortFilterProxyModel(this);
	_ui->setupUi(this);
//	sourceModel = NULL;
	confTreeModel = new QtConfListTree();
	_proxyModel->setSourceModel(confTreeModel);
	_ui->treeView->setModel(_proxyModel);
	_ui->treeView->setRootIsDecorated(false);
	SAFE_CONNECT(_ui->refreshButton, SIGNAL(clicked()), SLOT(refreshButtonClicked()));
	SAFE_CONNECT(_ui->instantConfButton, SIGNAL(clicked()), SLOT(instantConfButtonClicked()));
	SAFE_CONNECT(_ui->treeView,SIGNAL(doubleClicked(const QModelIndex&)),SLOT(JoinConference(const QModelIndex&)));

	SAFE_CONNECT_TYPE(this, SIGNAL(cvConfrenceInviteEventHandlerSinal(ConfInfo *,const QString&)),SLOT(cvConfrenceInviteEventHandlerSlot(ConfInfo *,const QString&)), Qt::QueuedConnection);
	SAFE_CONNECT_TYPE(this, SIGNAL(cvPasswordIncorrectEventHandlerSinal()),SLOT(cvPasswordIncorrectEventHandlerSlot()), Qt::QueuedConnection);

	CUserProfile * cUserProfile = _cWengoPhone.getCUserProfileHandler().getCUserProfile();
	cUserProfile->cvConfListReceivedEvent +=boost::bind(&QtConferenceList::cvConfListReceivedEventHandler,this,_2);
	cUserProfile->cvConfrenceInviteEvent +=boost::bind(&QtConferenceList::cvConfrenceInviteEventHandler,this,_2,_3);
	cUserProfile->cvPasswordIncorrectEvent +=boost::bind(&QtConferenceList::cvPasswordIncorrectEventHandler,this);

	SendGetConfListMsg();
	//发送QoS相关参数
	//sendQosParam();

	SAFE_CONNECT_RECEIVER(&_qtWengoPhone,SIGNAL(ActionRefreshMeetingListSignal()),this,SLOT(refreshButtonClicked()));//Add By LZY 2010-09-28 绑定响应RefreshMeetingList动作Signal的函数
	SAFE_CONNECT_RECEIVER(&_qtWengoPhone,SIGNAL(ActionEnterMeetingSignal(QString)),this,SLOT(teleEnterMeetingCommand(QString)));//Add By LZY 2010-09-28 绑定响应EnterMeeting动作Signal的函数
}

QtConferenceList::~QtConferenceList(void)
{
	disconnect(&_qtWengoPhone,SIGNAL(ActionRefreshMeetingListSignal()),this,SLOT(refreshButtonClicked()));//Add By LZY 2010-09-28 解绑响应RefreshMeetingList动作Signal的函数
	disconnect(&_qtWengoPhone,SIGNAL(ActionEnterMeetingSignal(QString)),this,SLOT(teleEnterMeetingCommand(QString)));//Add By LZY 2010-09-28 解绑响应EnterMeeting动作Signal的函数
	
	CUserProfile * cUserProfile = _cWengoPhone.getCUserProfileHandler().getCUserProfile();
	cUserProfile->cvConfListReceivedEvent -=boost::bind(&QtConferenceList::cvConfListReceivedEventHandler,this,_2);
	cUserProfile->cvConfrenceInviteEvent -=boost::bind(&QtConferenceList::cvConfrenceInviteEventHandler,this,_2,_3);
	OWSAFE_DELETE(_ui);
	OWSAFE_DELETE(_proxyModel);
//	OWSAFE_DELETE(sourceModel);
	OWSAFE_DELETE(confTreeModel);
}

void QtConferenceList::ShowConfList(vector<ConfInfo *> &confList)
{
//	sourceModel = (QStandardItemModel *)createModel(confList);
//	confTreeModel = new QtConfListTree();
	int size = confList.size();
	assert(confTreeModel!=NULL);
	confTreeModel->setupModelData(confList);
}

void QtConferenceList::refreshButtonClicked()
{
	SendGetConfListMsg();
}

void QtConferenceList::SendGetConfListMsg()
{
	CUserProfile * cUserProfile = _cWengoPhone.getCUserProfileHandler().getCUserProfile();
	Config & config = ConfigManager::getInstance().getCurrentConfig();
	//std::string confUri = "sip:service@ser.scut.edu.cn";
	std::string confUri = config.getGlobalService();
	std::string szContent = "<?xml version=\"1.0\"?><coolview command=\"GetConfList\"></coolview>";
//	cUserProfile->cvConfListReceivedEvent +=boost::bind(&QtConferenceList::ShowConfList,this,_2);
	cUserProfile->startConference(confUri,szContent);
}

//发送QoS相关参数
void QtConferenceList::sendQosParam()
{
	CUserProfile * cUserProfile = _cWengoPhone.getCUserProfileHandler().getCUserProfile();
	//std::string confUri = "sip:zhongkan.li@sipx.scut.edu.cn";
	Config & config = ConfigManager::getInstance().getCurrentConfig();
	std::string confUri = config.getGlobalService();
	std::string szContent = "<?xml version=\"1.0\"?><coolview command=\"Command\">Qos参数</coolview>";
	//	cUserProfile->cvConfListReceivedEvent +=boost::bind(&QtConferenceList::ShowConfList,this,_2);
	cUserProfile->startConference(confUri,szContent);
}

//-------Modify By LZY 2010-09-20-------
//   -   添加参数PassWord，把返回值从void改为int
/**
* Add by: hexianfu
* reload the JoinConference(const QModelIndex&proxyIndex);
* 2009-11-3
*/
int QtConferenceList::JoinConference(const int& row,std::string PassWord)
{
#ifndef DBUS_ENABLE
	//要在加入会议前先设置好回显窗口，否则会失败...zhenHua.sun 2010-09-08
    _tools->getMeetingPlaceWidget()->setPreviewWnd();
#endif

	CUserProfile * cUserProfile = _cWengoPhone.getCUserProfileHandler().getCUserProfile();
	int result = cUserProfile->makeJoinConfInvite(row);
	//需要密码
	if (result==-1)
	{
		if (PassWord!="")
		{
			int RowNum=row;
			result = cUserProfile->checkPassword(RowNum,PassWord);
		}
	}

	//进入会议之后显示成员列表  zhenHua.sun 2010-08-13
	if( this->_tools )
	{
		this->_tools->_ui->conInfoTabWidget->setCurrentWidget(
			this->_tools->_ui->memberListTab
			);
		this->_tools->setIsInConference( true );
	}

	return result;
}
//-------End of Modify By LZY------------

//-------Modify By LZY 2010-09-20-------
//   -   增加对JoinConference(const int& row,std::string PassWord)返回值的处理
void QtConferenceList::JoinConference(const QModelIndex&proxyIndex)
{
	QtConferenceList::_qtWengoPhone.SetUIState(UI_S_JOINMEETING_BEGIN);//Add By LZY 2010-09-30 用于通知遥控器界面状态变化
	int row = proxyIndex.row();
	int result = JoinConference(row);
	if (result==-1)//需要密码
	{
		QtPasswordInputWidget passwordInputDialog(_qtWengoPhone.getWidget(), _cWengoPhone,row);
		passwordInputDialog.exec();
	}
	else if (result==-2)//已经在会议里
	{
		QMessageBox::warning(this, tr("@product@"),
		tr("\346\202\250\345\267\262\347\273\217\350\277\233\345\205\245\344\272\206\344\270\200\344\270\252\344\274\232\350\256\256!"), QMessageBox::Ok);
		QtConferenceList::_qtWengoPhone.SetUIState(UI_S_MEETING);//Add By LZY 2010-09-30 用于通知遥控器界面状态变化
	}
	else if (result==-3)
	{
		QMessageBox::warning(this, tr("@product@"),
			QString::fromLocal8Bit("正在退出会议，请稍后再试！"), QMessageBox::Ok);
		QtConferenceList::_qtWengoPhone.SetUIState(UI_S_EXITMEETING_BEGIN);//Add By LZY 2010-09-30 用于通知遥控器界面状态变化
	}
}
//-------End of Modify By LZY-----------

void QtConferenceList::cvConfListReceivedEventHandler(vector<ConfInfo *> ConfList) 
{
	//CVMsgParser cvMessageParser;
	//cvMessageParser.InitParser(message.c_str());
	//const char *command = cvMessageParser.GetCommandType();
	//if (strcmp(command,"ConfList")==0)
	//{
	//	//int size = ConfList.size();
	//	if(ConfList.size()>0)
	//		ReleaseConfList(ConfList);
	//	cvMessageParser.ParserConfListMsg(ConfList);
	//	ShowConfList(ConfList);
	//}
	//typedef PostEvent1<void (vector<ConfInfo *>), vector<ConfInfo *> > MyPostEvent;
	//MyPostEvent * event = new MyPostEvent(boost::bind(&QtConferenceList::ShowConfList, this, _1), ConfList);
	//_qtWengoPhone.postEvent(event);

	ShowConfList(ConfList);
	QtConferenceList::_qtWengoPhone.sendTeleStateChanged(CHANGE_MEETINGLIST);//Add By LZY 2010-10-09 触发会议列表变更通知
}

void QtConferenceList::cvConfrenceInviteEventHandler(ConfInfo * conference,const std::string & from)
{
	cvConfrenceInviteEventHandlerSinal(conference,QString(from.c_str()));
}

/**
* Modifided by: hexianfu
* for the telecontroller to subscribe
* 2010-4-20
*/
void QtConferenceList::cvConfrenceInviteEventHandlerSlot(ConfInfo * conference,const QString& from)
{
	QMessageBox mb(tr("@CoolView@ - Invite"),
		QString::fromLocal8Bit("%1 邀请您参加会议 \"%2\". 是否参加该会议？").arg(from,QString::fromUtf8(conference->Title.c_str())),//Modify By LZY 2010-10-06
		QMessageBox::Question,
		QMessageBox::Yes | QMessageBox::Default,
		QMessageBox::No | QMessageBox::Escape,
		QMessageBox::NoButton, this);
	
	int MsgBoxIdentify=_qtWengoPhone.AddInviteMsg(&mb);//Add By LZY 2010-09-27

	//-------Add By LZY 2010-10-06-------
	CMessageFrame ResponseMsg;
	ResponseMsg.Append(MsgBoxIdentify);
	ResponseMsg.Append(from.toUtf8().data());
	ResponseMsg.Append(conference->Title.c_str());
	QtConferenceList::_qtWengoPhone.sendTeleStateChanged(CHANGE_INVITE,ResponseMsg.getString());//通知遥控器有新入会邀请
	//-------End of Add By LZY-----------

	int MsgBoxReturn = mb.exec();
	if (MsgBoxReturn == QMessageBox::Yes || MsgBoxReturn == QMessageBox::Accepted) {
		_qtWengoPhone.RemoveInviteMsg(MsgBoxIdentify);//Add By LZY 2010-09-27

		CUserProfile * cUserProfile = _cWengoPhone.getCUserProfileHandler().getCUserProfile();
		cUserProfile->makeJoinConfInvite(conference->URI);


		//进入会议之后显示成员列表  zhenHua.sun 2010-10-11
		if( this->_tools )
		{
			this->_tools->_ui->conInfoTabWidget->setCurrentWidget(
				this->_tools->_ui->memberListTab
				);
			this->_tools->setIsInConference( true );
		}
	}
	else
		_qtWengoPhone.RemoveInviteMsg(MsgBoxIdentify);//Add By LZY 2010-09-27
}
/***********************************/

void QtConferenceList::instantConfButtonClicked()
{
	QtInstantConferenceWidget instantConferenceDialog(_qtWengoPhone.getWidget(), _cWengoPhone);

	if( instantConferenceDialog.exec()== QDialog::Accepted )
	{	
		//进入会议之后显示成员列表  zhenHua.sun 2010-08-13
		if( this->_tools )
		{
			this->_tools->_ui->conInfoTabWidget->setCurrentWidget(
				this->_tools->_ui->memberListTab
				);
			this->_tools->setIsInConference( true );
		}
		this->refreshButtonClicked();
	}
}

void QtConferenceList::cvPasswordIncorrectEventHandler()
{
	cvPasswordIncorrectEventHandlerSinal();
}

void QtConferenceList::cvPasswordIncorrectEventHandlerSlot()
{
	QMessageBox::warning(this, tr("@product@ - Password Error"),
		tr("the password you input is incorrect"), QMessageBox::Ok);
}

//-------Add By LZY 2010-09-20-------
/* 处理EnterMeeting动作事件
 * @param QString EnterMeetingStr,传入的参数
 * @return void
 */
void QtConferenceList::teleEnterMeetingCommand(QString EnterMeetingStr)
{
	CMessageFrame EnterMeetingParam(EnterMeetingStr.toStdString().c_str());
	CUserProfile * cUserProfile = _cWengoPhone.getCUserProfileHandler().getCUserProfile();
	vector<ConfInfo *> currentConfList = cUserProfile->getCurrentConfInfoList();
	string MeetingURI=EnterMeetingParam.readLine(true);//要登入的会议识别URI
	int nSize=currentConfList.size();
	int indexRow=-1;//通过会议URI搜索到的会议在列表中的索引号
	for (int LoopVar=0;LoopVar<nSize;++LoopVar)
	{
		if (currentConfList[LoopVar]->URI==MeetingURI)
		{
			indexRow=LoopVar;
			break;
		}
	}
	if (indexRow==-1)
	{
		QtConferenceList::_qtWengoPhone.SetUIState(UI_S_LOGIN);//Add By LZY 2010-09-30 用于通知遥控器界面状态变化，进入会议失败
		return;
	}
	int result=QtConferenceList::JoinConference(indexRow,EnterMeetingParam.readLine(true));
	if (result<0)
		QtConferenceList::_qtWengoPhone.SetUIState(UI_S_LOGIN);//Add By LZY 2010-09-30 用于通知遥控器界面状态变化，进入会议失败
}
//-------End of Add By LZY---------