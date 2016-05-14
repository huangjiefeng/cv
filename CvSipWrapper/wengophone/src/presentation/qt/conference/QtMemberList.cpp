#include "stdafx.h"
#include ".\qtmemberlist.h"
#include <util/SafeDelete.h>
#include <qtutil/SafeConnect.h>
#include "QtWengoPhone.h"
#include <control/profile/CUserProfile.h>
#include <control/profile/CUserProfileHandler.h>
#include <control/CWengoPhone.h>
#include "QtConferenceInvite.h"
#include <QtGui/QtGui>
#include <model/conference/Log.h>
#include "QtToolWidget.h"

#include <dbus/conferenceRoom/ConfRoomClient.h>

//zhenHua.sun 2010-12-07
#include <RunningProfile.h>
#include <SessionManager.h>

QtMemberList::QtMemberList(QtWengoPhone & qtWengoPhone,QtConferenceRoomWidget *confRoom):_qtWengoPhone(qtWengoPhone),_cWengoPhone(_qtWengoPhone.getCWengoPhone()),_confRoom(confRoom)
{
	send_address = "";
	send_video_port = 0;
	send_audio_port = 0;
	_ui = new Ui::MemberList();
	memberListTreeModel =new QtMemberListTreeModel();
	_proxyModel = new QSortFilterProxyModel(this);
	_proxyModel->setSourceModel(memberListTreeModel);
	_ui->setupUi(this);
	_ui->treeView->setModel(_proxyModel);
	_ui->treeView->setRootIsDecorated(false);
	_ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
	//_ui->treeView->header()->setResizeMode(QHeaderView::Stretch);
	_ui->treeView->resizeColumnToContents(1);
	_ui->treeView->resizeColumnToContents(2);
	_ui->treeView->resizeColumnToContents(3);
	_ui->treeView->setColumnWidth(0,85);
	SAFE_CONNECT(_ui->ExitConfButton, SIGNAL(clicked()), SLOT(exitButtonClicked()));
	SAFE_CONNECT(_ui->RecvMediaButton, SIGNAL(clicked()), SLOT(RecvAudioVideo()));
	SAFE_CONNECT(_ui->sendButton, SIGNAL(clicked()), SLOT(SendAudioVideo()));
	SAFE_CONNECT(_ui->handButton, SIGNAL(clicked()), SLOT(handButtonClicked()));
	SAFE_CONNECT(_ui->treeView,SIGNAL(doubleClicked(const QModelIndex&)),SLOT(RecvAudioVideo(const QModelIndex&)));
	SAFE_CONNECT(_ui->treeView,SIGNAL(clicked(const QModelIndex&)),SLOT(setRecvButtonStatus(const QModelIndex&)));

	SAFE_CONNECT(_ui->treeView,SIGNAL(customContextMenuRequested(const QPoint&)),SLOT(showPopupMenuForCurrentIndex()));

	//起停音视频   zhenHua.sun 2010-08-05
	SAFE_CONNECT(_ui->audioControlButton ,SIGNAL(clicked( bool ) ),SLOT(audioControlClicked(bool)) );
	SAFE_CONNECT(_ui->videoControlButton ,SIGNAL(clicked( bool ) ),SLOT(videoControlClicked(bool)) );
	//end


	CUserProfile * cUserProfile = _cWengoPhone.getCUserProfileHandler().getCUserProfile();
	cUserProfile->cvMemberListReceivedEvent +=boost::bind(&QtMemberList::cvMemberListReceivedEventHandler,this,_2,_3);
	cUserProfile->setCallIdEvent +=boost::bind(&QtMemberList::setCallIdEventHandler,this,_2,_3);
	cUserProfile->cvKickedEvent +=boost::bind(&QtMemberList::cvKickedEventHandler,this);
	cUserProfile->cvPermitSpeakEvent +=boost::bind(&QtMemberList::cvHandDownEventHandler,this);

	//起停按钮..zhenHua.sun 2010-08-09
	_ui->audioControlButton->setDisabled( true );
	_ui->videoControlButton->setDisabled( true );
	//end

	_ui->RecvMediaButton->setDisabled(true);
	_ui->handButton->setDisabled(true);
	_ui->sendButton->setDisabled(true);
	_ui->ExitConfButton->setDisabled(true);
	inConfrence = false;

	SAFE_CONNECT_TYPE(this, SIGNAL(setConfTitleSinal(const QString&)),SLOT(setConfTitleSlot(const QString&)), Qt::QueuedConnection);
	SAFE_CONNECT_TYPE(this, SIGNAL(cvKickedEventHandlerSinal()),SLOT(cvKickedEventHandlerSlot()), Qt::QueuedConnection);

	SAFE_CONNECT_TYPE(this, SIGNAL(cvHandDownEventHandlerSinal()),SLOT(cvHandDownEventHandlerSlot()), Qt::QueuedConnection);

	infoTimer = new QTimer(this);
	SAFE_CONNECT_RECEIVER(infoTimer, SIGNAL(timeout()), this, SLOT(makeInfoCallSlot()));

	exitButtonTimer = new QTimer(this);
	SAFE_CONNECT_RECEIVER(exitButtonTimer, SIGNAL(timeout()), this, SLOT(setExitButtonEnable()));

	releaseFloorControlTimer = new QTimer(this);
	SAFE_CONNECT_RECEIVER(releaseFloorControlTimer, SIGNAL(timeout()), this, SLOT(releaseFloorControl()));

	SAFE_CONNECT_RECEIVER(&_qtWengoPhone,SIGNAL(ActionStopMemberSignal(QString)),this,SLOT(teleStopMemberCommand(QString)));//Add By LZY 2010-11-04 绑定响应StopMember动作Signal的函数
	SAFE_CONNECT_RECEIVER(&_qtWengoPhone,SIGNAL(ActionContactMemberSignal(QString)),this,SLOT(teleContactMemberCommand(QString)));//Add By LZY 2010-09-28  绑定响应ContactMember动作Signal的函数
	SAFE_CONNECT_RECEIVER(&_qtWengoPhone,SIGNAL(ActionQuitMeetingSignal()),this,SLOT(exitMeeting()));//Add By LZY 2010-09-28  绑定响应QuitMeeting动作Signal的函数
	SAFE_CONNECT_RECEIVER(&_qtWengoPhone,SIGNAL(ActionRaiseHandSignal()),this,SLOT(teleRaiseHandCommand()));//Add By LZY 2010-09-28  绑定响应RaiseHand动作Signal的函数
	SAFE_CONNECT_RECEIVER(&_qtWengoPhone,SIGNAL(ActionPermitSpeakSignal(QString)),this,SLOT(telePermitSpeakCommand(QString)));//Add By LZY 2010-09-28  绑定响应PermitSpeak动作Signal的函数
	SAFE_CONNECT_RECEIVER(&_qtWengoPhone,SIGNAL(ActionKickMemberSignal(QString)),this,SLOT(teleKickMemberCommand(QString)));//Add By LZY 2010-09-28  绑定响应KickMember动作Signal的函数
	SAFE_CONNECT_RECEIVER(&_qtWengoPhone,SIGNAL(ActionInviteMembersSignal(QString)),this,SLOT(teleInviteMembersCommand(QString)));//Add By LZY 2010-10-05  绑定响应InviteMembers动作Signal的函数
	SAFE_CONNECT_RECEIVER(&_qtWengoPhone,SIGNAL(ActionLocalVideoSettingSignal()),this,SLOT(teleLocalVideoSettingCommand()));//Add By LZY 2010-09-28 绑定响应LocalVideoSetting动作Signal的函数
	SAFE_CONNECT_RECEIVER(&_qtWengoPhone,SIGNAL(ActionLocalAudioSettingSignal()),this,SLOT(teleLocalAudioSettingCommand()));//Add By LZY 2010-09-28 绑定响应LocalAudioSetting动作Signal的函数
}

QtMemberList::~QtMemberList(void)
{
	disconnect(&_qtWengoPhone,SIGNAL(ActionStopMemberSignal(QString)),this,SLOT(teleStopMemberCommand(QString)));//Add By LZY 2010-11-04 解绑响应StopMember动作Signal的函数
	disconnect(&_qtWengoPhone,SIGNAL(ActionContactMemberSignal(QString)),this,SLOT(teleContactMemberCommand(QString)));//Add By LZY 2010-09-28  解绑响应ContactMember动作Signal的函数
	disconnect(&_qtWengoPhone,SIGNAL(ActionQuitMeetingSignal()),this,SLOT(exitMeeting()));//Add By LZY 2010-09-28  解绑响应QuitMeeting动作Signal的函数
	disconnect(&_qtWengoPhone,SIGNAL(ActionRaiseHandSignal()),this,SLOT(teleRaiseHandCommand()));//Add By LZY 2010-09-28  解绑响应RaiseHand动作Signal的函数
	disconnect(&_qtWengoPhone,SIGNAL(ActionPermitSpeakSignal(QString)),this,SLOT(telePermitSpeakCommand(QString)));//Add By LZY 2010-09-28 解绑响应PermitSpeak动作Signal的函数
	disconnect(&_qtWengoPhone,SIGNAL(ActionKickMemberSignal(QString)),this,SLOT(teleKickMemberCommand(QString)));//Add By LZY 2010-09-28 解绑响应KickMember动作Signal的函数
	disconnect(&_qtWengoPhone,SIGNAL(ActionInviteMembersSignal(QString)),this,SLOT(teleInviteMembersCommand(QString)));//Add By LZY 2010-10-05  解绑响应InviteMembers动作Signal的函数
	disconnect(&_qtWengoPhone,SIGNAL(ActionLocalVideoSettingSignal()),this,SLOT(teleLocalVideoSettingCommand()));//Add By LZY 2010-09-28 解绑响应LocalVideoSetting动作Signal的函数
	disconnect(&_qtWengoPhone,SIGNAL(ActionLocalAudioSettingSignal()),this,SLOT(teleLocalAudioSettingCommand()));//Add By LZY 2010-09-28 解绑响应LocalAudioSetting动作Signal的函数
	
	CUserProfile * cUserProfile = _cWengoPhone.getCUserProfileHandler().getCUserProfile();
	cUserProfile->cvMemberListReceivedEvent -=boost::bind(&QtMemberList::cvMemberListReceivedEventHandler,this,_2,_3);
	cUserProfile->setCallIdEvent -=boost::bind(&QtMemberList::setCallIdEventHandler,this,_2,_3);
	cUserProfile->cvKickedEvent -=boost::bind(&QtMemberList::cvKickedEventHandler,this);
	cUserProfile->cvPermitSpeakEvent -=boost::bind(&QtMemberList::cvHandDownEventHandler,this);
	
	if (_callId>0)
	{
		cUserProfile->exitConference(_callId);
		_callId = 0;
	}

	OWSAFE_DELETE(_proxyModel);
	OWSAFE_DELETE(memberListTreeModel);
	OWSAFE_DELETE(_ui);
	//if (MemberList.size()>0)
	//{
	//	vector<MemberInfo *>::iterator iter = MemberList.begin();
	//	for (iter;iter!=MemberList.end();iter++)
	//	{
	//		MemberInfo * c = (MemberInfo *)(*iter);
	//		delete c;
	//	}
	//	MemberList.clear();
	//}
	OWSAFE_DELETE(infoTimer);
	OWSAFE_DELETE(exitButtonTimer);
}

void QtMemberList::ShowMemberList(vector<MemberInfo *> &memberList)
{
	//_proxyModel->setSourceModel(createModel(memberList));
	//_ui->treeView->setRootIsDecorated(false);
	//_ui->treeView->setModel(_proxyModel);
	//_ui->treeView->setSortingEnabled(true);
	//_ui->treeView->sortByColumn(0, Qt::AscendingOrder);
	
//	_ui->sendButton->setDisabled(false);
//	isSend = false;
	memberListTreeModel->setupModelData(memberList);
}

void QtMemberList::cvMemberListReceivedEventHandler(vector<MemberInfo *> &MemberList,std::string ConfTitle) 
{
/*	CVMsgParser cvMessageParser;
	cvMessageParser.InitParser(message.c_str());
	const char *command = cvMessageParser.GetCommandType();
	if (strcmp(command,"LoginResult")==0)
	{
		if(MemberList.size()>0)
			ReleaseMemberList(MemberList);
		MemberList = cvMessageParser.ParserMemberListMsg(send_audio_port,send_video_port,send_address);
		ShowMemberList(MemberList);
		_confRoom->SetTabWidgetIndex(1);
	}
	else if (strcmp(command,"LoginNotify")==0||strcmp(command,"loginnotify")==0)
	{
		cvMessageParser.ParserMemberListMsg(MemberList);
		ShowMemberList(MemberList);
		_confRoom->SetTabWidgetIndex(1);
	}
	else if (strcmp(command,"LogoutNotify")==0||strcmp(command,"logoutnotify")==0)
	{
		cvMessageParser.ParserMemberListMsg(MemberList);
		ShowMemberList(MemberList);
		_confRoom->SetTabWidgetIndex(1);
	}*/
	ShowMemberList(MemberList);
	if(!inConfrence)
	{
		setConfTitleSinal(tr(ConfTitle.c_str()));
		inConfrence = true;
	}

	//新界面不支持该方法  zhenHua.sun 2010-08-11
	//_confRoom->SetTabWidgetIndex(1);

	//-------Add By LZY 2010-09-30 2010-10-05 2010-10-06-------
	//   -   用于通知遥控器界面状态变化
	if (MemberList.size()>0)
	{
		QtMemberList::_qtWengoPhone.SetUIState(UI_S_MEETING);
		QtMemberList::_qtWengoPhone.SetUIState(UI_D_RAISEHAND_SUCCESS);
		QtMemberList::_qtWengoPhone.SetUIState(UI_D_PERMITSPEAK_SUCCESS);
		QtMemberList::_qtWengoPhone.SetUIState(UI_D_KICKMEMBER_SUCCESS);
		QtMemberList::_qtWengoPhone.sendTeleStateChanged(CHANGE_MEMBERLIST);
	}
	//-------End of Add By LZY-----------
}

bool QtMemberList::exitButtonClicked()
{
	//弹出确认对话框
	QTextCodec::setCodecForTr( QTextCodec::codecForName("utf-8") );
	if( QMessageBox::question( this,
		tr("@product@"),
		tr("确认退出会议？ "),
		QMessageBox::Ok,
		QMessageBox::Cancel) != QMessageBox::Ok )
	{
		return  false;
	}

	QtMemberList::_qtWengoPhone.SetUIState(UI_S_EXITMEETING_BEGIN);//Add By LZY 2010-09-30 用于通知遥控器界面状态变化
	//-------Modify By LZY 2010-09-25-------
	QtMemberList::exitMeeting();
	//-------End of Modify By LZY-----------
	return true;
}

void QtMemberList::setRecvButtonStatus(const QModelIndex&proxyIndex)
{
	int row = proxyIndex.row();
	//row = MemberList.size()-1-row;
	//MemberInfo * mlist = (MemberInfo *)MemberList[row];
	//if (mlist->isReceived==0)
	//{
	//	_ui->RecvMediaButton->setDisabled(false);
	//}
	//else
	//	_ui->RecvMediaButton->setDisabled(true);
	CUserProfile * cUserProfile = _cWengoPhone.getCUserProfileHandler().getCUserProfile();
	_ui->RecvMediaButton->setDisabled(cUserProfile->setRecvButtonStatus(row));
}

int QtMemberList::RecvAudioVideo(int &rowIndex,std::string &userId)
{
	CUserProfile * cUserProfile = _cWengoPhone.getCUserProfileHandler().getCUserProfile();
	HWND remoteHwnd = NULL;

	int result = cUserProfile->media_recv_start(rowIndex,remoteHwnd);
	//-------Delete By LZY 2010-11-03-----
	/*if (result==0)
	{
		_ui->RecvMediaButton->setDisabled(true);
	}*/
	//-------End of Delete By LZY-------
	return 0;
}
/***********************************************/
/**
* modified by: hexianfu
* for the teleController to contact a member
* 2009-11-4
*/
void QtMemberList::RecvAudioVideo(const QModelIndex&proxyIndex)
{
	CUserProfile * cUserProfile = _cWengoPhone.getCUserProfileHandler().getCUserProfile();
	int row = proxyIndex.row();
	if(row<0)
		return;
	if (row==0)
	{
		QMessageBox::warning(this, tr("@product@"),
			tr("\344\270\215\350\203\275\346\216\245\346\224\266\350\207\252\345\267\261\347\232\204\351\237\263\350\247\206\351\242\221\357\274\201"), QMessageBox::Ok);
		return;
	}

	HWND remoteHwnd = NULL;
	std::string userId = memberListTreeModel->getData(proxyIndex,Qt::DisplayRole).toString().toStdString();
	RecvAudioVideo(row,userId);
}

void QtMemberList::RecvAudioVideo()
{
	QModelIndex sourceIndex = _ui->treeView->currentIndex();
	RecvAudioVideo(sourceIndex);
}

void QtMemberList::SendAudioVideo()
{
	/*
	if (!isSend)
	{
		char sendAddress[50];
		strcpy(sendAddress,send_address.c_str());
		CUserProfile * cUserProfile = _cWengoPhone.getCUserProfileHandler().getCUserProfile();
		HWND previewWindow = (HWND)_confRoom->GetPreviewVidowWidget()->winId();
		int result = cUserProfile->media_send_start(sendAddress,send_audio_port,sendAddress,send_video_port,previewWindow);
		if (result==0)
		{
			_ui->sendButton->setDisabled(true);
			isSend = true;
		}
	}
	*/
	//CUserProfile * cUserProfile = _cWengoPhone.getCUserProfileHandler().getCUserProfile();
	//HWND previewWindow = (HWND)_confRoom->GetPreviewVidowWidget()->winId();
	//cUserProfile->media_send_start("202.38.254.164",1234,"202.38.254.164",1236,previewWindow);
	QtConferenceInvite conferenceDialog(_qtWengoPhone.getWidget(), _cWengoPhone,_callId);
	conferenceDialog.exec();
}
void QtMemberList::setCallIdEventHandler(int callId,HWND &previewHwnd)
{
	_callId = callId;
//	previewHwnd = (HWND)_confRoom->GetPreviewVidowWidget()->winId();
}

void QtMemberList::getMediaParamFromSdp(int &aport,int &vport,char *mediaType,const char *sdp)
{
	char audioPort[10];
	char videoPort[10];
	char *temp ,*temp2;
	if(!sdp)
		return;
	temp= strstr((char *)sdp,"m=audio");
	if (temp)
	{
		temp+=strlen("m=audio")+1;
		temp2 = temp;
		while (*temp!=' ')
		{
			temp++;
		}
		strncpy(audioPort,temp2,temp-temp2);
		aport = atoi(audioPort);
	}

	temp= strstr((char *)sdp,"m=video");
	if (temp)
	{
		temp+=strlen("m=video")+1;
		temp2 = temp;
		while (*temp!=' ')
		{
			temp++;
		}
		strncpy(videoPort,temp2,temp-temp2);
		vport = atoi(videoPort);
	}

	temp= strstr(temp2,"a=rtpmap");
	if (temp)
	{
		temp+=strlen("a=rtpmap")+1;
		while (*temp!=' ')
		{
			temp++;
		}
		temp++;
		temp2 = temp;
		while (*temp!='/')
		{
			temp++;
		}
		strncpy(mediaType,temp2,temp-temp2);
	} 
}

void QtMemberList::ReleaseMemberList(vector<MemberInfo *> &member_list)
{
	vector<MemberInfo *>::iterator iter = member_list.begin();
	for (iter;iter!=member_list.end();iter++)
	{
		MemberInfo * c = (MemberInfo *)(*iter);
		if(c!=NULL&&c->MemberURI!="")
			delete c;
	}
	member_list.clear();
}

void QtMemberList::setConfTitleSlot(const QString& conf_title)
{
	_ui->confName->setText(conf_title);
	_ui->ExitConfButton->setDisabled(true);

	//启动音视频控制按钮...zhenHua.sun 2010-08-09
	_ui->audioControlButton->setDisabled( false );
	_ui->videoControlButton->setDisabled( false );
	//end

	_ui->handButton->setDisabled(false);
	_ui->sendButton->setDisabled(false);
	infoTimer->start(60000);
	exitButtonTimer->start(2000);
}

void QtMemberList::showPopupMenuForCurrentIndex() {
	QModelIndex index = _ui->treeView->selectionModel()->currentIndex();
	if (!index.isValid()) {
		return;
	}
	QAction* speakAction=NULL;
	QAction* forbidAllAction=NULL;
	QAction* deleteAction=NULL;
	QAction* detailAction=NULL;
	QMenu menu(_ui->treeView);
	CUserProfile * cUserProfile = _cWengoPhone.getCUserProfileHandler().getCUserProfile();
	if (cUserProfile->hostOrNot()&&index.row()>0)
	{
		if(cUserProfile->memberSpeakStatus(index.row())==0)
			speakAction = menu.addAction(QIcon(":/pics/statusbar/input_volume_ok.png"),QApplication::translate("VideoWindow", "\345\205\201\350\256\270\345\217\221\350\250\200", 0, QApplication::UnicodeUTF8));
		if(cUserProfile->memberSpeakStatus(index.row())==1)
			speakAction = menu.addAction(QIcon(":/pics/statusbar/input_volume_error.png"),QApplication::translate("VideoWindow", "\347\246\201\346\255\242\345\217\221\350\250\200", 0, QApplication::UnicodeUTF8));
		forbidAllAction = menu.addAction(QIcon(":/pics/statusbar/input_volume_error.png"),QApplication::translate("VideoWindow", "\347\246\201\346\255\242\346\211\200\346\234\211\345\217\221\350\250\200\344\272\272", 0, QApplication::UnicodeUTF8));
		deleteAction = menu.addAction(QIcon(":/pics/actions/delete.png"),QApplication::translate("VideoWindow", "\345\211\224\351\231\244\350\257\245\344\270\216\344\274\232\344\272\272\345\221\230", 0, QApplication::UnicodeUTF8));
	}
	//detailAction = menu.addAction(QIcon(":/pics/status/unknown-sip.png"),QApplication::translate("VideoWindow", "\350\257\246\347\273\206\350\265\204\346\226\231", 0, QApplication::UnicodeUTF8));

	QAction* action = menu.exec(QCursor::pos());
	if (action!=NULL&&speakAction!=NULL&&action == speakAction) {
		if(cUserProfile->memberSpeakStatus(index.row())==0)
			cUserProfile->setSpeaker(index.row(),true,_callId);
		else
			cUserProfile->setSpeaker(index.row(),false,_callId);
	}
	else if (action!=NULL&&forbidAllAction!=NULL&&action == forbidAllAction) {
		cUserProfile->setSpeaker(-1,false,_callId);
	}
	else if (action!=NULL&&deleteAction!=NULL&&action == deleteAction) {
		cUserProfile->KickOut(index.row(),_callId);
	}
	else if (action!=NULL&&detailAction!=NULL&&action == detailAction) {
		QMessageBox::warning(this, tr("@product@ - Detail"),
			tr("not implemented"), QMessageBox::Ok);
	}
}

void QtMemberList::makeInfoCallSlot()
{
	CUserProfile * cUserProfile = _cWengoPhone.getCUserProfileHandler().getCUserProfile();
	char *szContent = "<?xml version=\"1.0\"?><coolview command=\"Online\"></coolview>";
	cUserProfile->makeInfoCall(_callId,szContent);
}

void QtMemberList::cvKickedEventHandler()
{
	cvKickedEventHandlerSinal();
}

void QtMemberList::cvKickedEventHandlerSlot()
{
	QtMemberList::exitMeeting();//exitButtonClicked();//Modify By LZY 2010-10-06 避开退出确认对话框
	QtMemberList::_qtWengoPhone.sendTeleStateChanged(CHANGE_KICK);//Add By LZY 2010-10-06 通知遥控器用户被踢出会议
}

void QtMemberList::setExitButtonEnable()
{
	_ui->ExitConfButton->setDisabled(false);
	exitButtonTimer->stop();
}

void QtMemberList::handButtonClicked()
{
	CUserProfile * cUserProfile = _cWengoPhone.getCUserProfileHandler().getCUserProfile();	
	if (_ui->handButton->isChecked())
	{
		cUserProfile->handForSpeak(0,1,_callId);
	}
	else
	{
		cUserProfile->handForSpeak(0,0,_callId);
	}
}


void QtMemberList::cvHandDownEventHandler()
{
	cvHandDownEventHandlerSinal();
}

void QtMemberList::cvHandDownEventHandlerSlot()
{
	CUserProfile * cUserProfile = _cWengoPhone.getCUserProfileHandler().getCUserProfile();	
	cUserProfile->handForSpeak(0,0,_callId);
	_ui->handButton->setChecked(false);
}

void QtMemberList::releaseFloorControl()
{
	if (_callId>0)
	{
		CUserProfile * cUserProfile = _cWengoPhone.getCUserProfileHandler().getCUserProfile();
		cUserProfile->setSpeaker(-2,false,_callId);
		cUserProfile->setSpeaker(-2,true,_callId);
	}
}

//-------Modify By LZY 2010-09-27 2010-10-05-------
void QtMemberList::telePermitSpeakCommand(QString userName)
{
	int row = memberListTreeModel->rowCount();
	int column = memberListTreeModel->columnCount();
	int j = 0;
	int i=0 ;
	for( i=0;i<row;i++)
	{

		QModelIndex Index = memberListTreeModel->index(i,j);

		QString itemUserId = memberListTreeModel->getData(Index,Qt::DisplayRole).toString();
		if(QString::compare(userName,itemUserId)==0)
		{
			CUserProfile * cUserProfile = _cWengoPhone.getCUserProfileHandler().getCUserProfile();

			if (cUserProfile->hostOrNot()&&Index.row()>0)
			{
				if(cUserProfile->memberSpeakStatus(Index.row())==0)
					cUserProfile->setSpeaker(Index.row(),true,_callId);
				else
					cUserProfile->setSpeaker(Index.row(),false,_callId);  
			}
			else
			{
				QtMemberList::_qtWengoPhone.SetUIState(UI_D_PERMITSPEAK_ERROR);
			}
			return;
		}
	}
	if(i>=row)
	{
		QtMemberList::_qtWengoPhone.SetUIState(UI_D_PERMITSPEAK_ERROR);
	}
}
//-------End of Modify By LZY------------

//-------Modify By LZY 2010-09-27 2010-10-05-------
void QtMemberList::teleKickMemberCommand(QString userName)
{
	int row = memberListTreeModel->rowCount();
	int column = memberListTreeModel->columnCount();
	int j = 0;
	int i=0;
	for( i=0;i<row;i++)
	{

		QModelIndex Index = memberListTreeModel->index(i,j);

		QString itemUserId = memberListTreeModel->getData(Index,Qt::DisplayRole).toString();
		if(QString::compare(userName,itemUserId)==0)
		{
			CUserProfile * cUserProfile = _cWengoPhone.getCUserProfileHandler().getCUserProfile();

			if (Index.row()>0)
			{
				cUserProfile->KickOut(Index.row(),_callId);
			}
			else
			{
				_qtWengoPhone.SetUIState(UI_D_KICKMEMBER_ERROR);
			}
			return;
		}
	}
	if(i>=row)
	{
		_qtWengoPhone.SetUIState(UI_D_KICKMEMBER_ERROR);
	}
}
//-------End of Modify By LZY----------

//音视频起停...............................zhenHua.sun 2010-08-05

void QtMemberList::audioControlClicked( bool checked )
{
	CUserProfile * cUserProfile = _cWengoPhone.getCUserProfileHandler().getCUserProfile();	
	QTextCodec *codec = QTextCodec::codecForName("utf-8");
	QTextCodec::setCodecForTr(codec);
	if( checked )
	{
		//停止音频发送
		_ui->audioControlButton->setText( QObject::tr("启动音频发送 "));	//加入空格才能编译通过，原因未知
		cUserProfile->controlAudioSend( false );
		QtMemberList::_qtWengoPhone.SetUIState(UI_D_LOCALAUDIO_SEND_STOP);//Add By LZY 2010-10-08
	}else
	{
		//启动音频发送
		_ui->audioControlButton->setText( tr("停止音频发送 ") );
		cUserProfile->controlAudioSend( true );
		QtMemberList::_qtWengoPhone.SetUIState(UI_D_LOCALAUDIO_SEND_START);//Add By LZY 2010-10-08
	}
}

void QtMemberList::videoControlClicked( bool checked )
{
	CUserProfile * cUserProfile = _cWengoPhone.getCUserProfileHandler().getCUserProfile();	
	QTextCodec *codec = QTextCodec::codecForName("utf-8");
	QTextCodec::setCodecForTr(codec);
	if( checked )
	{
		//停止视频发送
		_ui->videoControlButton->setText( QObject::tr("启动视频发送 ") );
		cUserProfile->controlVideoSend( false );
		QtMemberList::_qtWengoPhone.SetUIState(UI_D_LOCALVIDEO_SEND_STOP);//Add By LZY 2010-10-08
	}else
	{
		//启动视频发送
		_ui->videoControlButton->setText( tr("停止视频发送 ") );
		cUserProfile->controlVideoSend( true );
		QtMemberList::_qtWengoPhone.SetUIState(UI_D_LOCALVIDEO_SEND_START);//Add By LZY 2010-10-08
	}
}

//-------Add By LZY 2010-09-21 2010-11-04-------
/*
 * 退出会议的处理函数，界面及遥控器动作都通过调用本函数实现退出会议的操作
 * @return void
 */
void QtMemberList::exitMeeting()
{
	_ui->videoControlButton->setDisabled(true);
	_ui->audioControlButton->setDisabled(true);
	_ui->ExitConfButton->setDisabled(true);
	_ui->handButton->setChecked(false);
	_ui->handButton->setDisabled(true);
	_ui->sendButton->setDisabled(true);
	_ui->RecvMediaButton->setDisabled(true);
	_ui->confName->setText(tr(""));
	vector<MemberInfo *> _memberList;
	memberListTreeModel->setupModelData(_memberList);
	
	//返回会议列表  zhenHua.sun 2010-08-11
	if(  this->_confRoom )
	{
		_confRoom->GetToolWidget()->_ui->conInfoTabWidget->setCurrentWidget(
			_confRoom->GetToolWidget()->_ui->confListTab
			);
		_confRoom->GetToolWidget()->setIsInConference(false);
	}

	CUserProfile * cUserProfile = _cWengoPhone.getCUserProfileHandler().getCUserProfile();
	if (_callId>0)
	{
		cUserProfile->exitConference(_callId);
		_callId = 0;
	}
	infoTimer->stop();
	inConfrence = false;
}

/*
 * ContactMember动作事件处理函数
 * @param QString ContactMemberNameStr,要接收音/视频的用户名（@前的部分）
 * @return void
 */
void QtMemberList::teleContactMemberCommand(QString ContactMemberNameStr)
{
	string ContactMemberName=ContactMemberNameStr.toStdString();
	CUserProfile *cUserProfile = QtMemberList::_cWengoPhone.getCUserProfileHandler().getCUserProfile();
	vector<MemberInfo *> &currentMemberList = cUserProfile->getCurrentMemberInfoList();
	int Size=currentMemberList.size();
	int Row=-1;//定位到的人员对应人员列表行数，-1表示定位失败
	MemberInfo * member;
	int index;
	string nameAbstract;
	for (int LoopVar=0;LoopVar<Size;++LoopVar)
	{
		member=currentMemberList[LoopVar];
		index=member->MemberURI.find('@',0);
		nameAbstract=member->MemberURI;
		if (index!=std::string::npos)
			nameAbstract=nameAbstract.substr(0,index);
		if (ContactMemberName==nameAbstract)
		{
			Row=LoopVar;
			break;
		}
	}
	if (Row==-1)
	{
		QtMemberList::_qtWengoPhone.SetUIState(UI_D_CONTACTMEMBER_ERROR);//Add By LZY 2010-10-05 增加延后响应动作ContactMember的瞬态返回通知
		return;
	}
	QtMemberList::RecvAudioVideo(Row,ContactMemberName);
	QtMemberList::_qtWengoPhone.SetUIState(UI_D_CONTACTMEMBER_SUCCESS);//Add By LZY 2010-10-05 增加延后响应动作ContactMember的瞬态返回通知
}

/*
 * 响应动作RaiseHand的Signal的Slot
 * @return void
 */
void QtMemberList::teleRaiseHandCommand()
{
	if (_ui->handButton->isChecked())
		QtMemberList::_ui->handButton->setChecked(false);
	else
		QtMemberList::_ui->handButton->setChecked(true);
	QtMemberList::handButtonClicked();
}

/*
 * 响应InviteMember动作Signal的Slot，根据字符串向指定的人员发送邀请
 * @param QString InviteMembers,邀请人地址串，用“/”分隔
 * @return void
 */
void QtMemberList::teleInviteMembersCommand(QString InviteMembers)
{
	CUserProfile * cUserProfile = QtMemberList::_cWengoPhone.getCUserProfileHandler().getCUserProfile();
	QStringList inviteList = InviteMembers.split('/');
	QString szContent = "<?xml version=\"1.0\"?><coolview command=\"Invite\">";
	for (int i=0;i<inviteList.count();i++)
		szContent = szContent+"<uri>"+inviteList[i]+"</uri>";
	szContent += "</coolview>";

	char *body = new char[szContent.length()+1];
	strcpy(body,szContent.toStdString().c_str());
	cUserProfile->makeInfoCall(_callId,body);
}

/*
 * 响应LocalVideoSetting动作的Signal
 * @return void
 */
void QtMemberList::teleLocalVideoSettingCommand()
{
	QtMemberList::_ui->videoControlButton->click();
}

/*
 * 响应LocalAudioSetting动作的Signal
 * @return void
 */
void QtMemberList::teleLocalAudioSettingCommand()
{
	QtMemberList::_ui->audioControlButton->click();
}

/*
 * 2010-11-04 响应StopMember动作的Signal
 * @param QString userId,用户名
 * @return void
 */
void QtMemberList::teleStopMemberCommand(QString userId)
{
	_cWengoPhone.getCUserProfileHandler().getCUserProfile()->media_recv_stop(userId.toStdString());
}
//-------End of Add By LZY----------