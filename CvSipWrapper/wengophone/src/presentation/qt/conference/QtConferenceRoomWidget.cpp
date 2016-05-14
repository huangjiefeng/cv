#include "stdafx.h"
#include ".\qtconferenceroomwidget.h"
#include <QtGui/QtGui>
#include <util/SafeDelete.h>
#include <qtutil/SafeConnect.h>
#include "QtConferenceList.h"
#include "QtMemberList.h"
#include "QtTextChatWidget.h"
#include <qtutil/Widget.h>

//新界面
#include "QtWengoPhone.h"
#include "QtToolWidget.h"
#include "..\config\QtShowSetting.h"
#include "QtToolBar.h"
#include "QtRtcpInfoWidget.h"

#include <RunningProfile.h>
#include <SessionManager.h>

QtConferenceRoomWidget::QtConferenceRoomWidget(QtWengoPhone & qtWengoPhone):_qtWengoPhone(qtWengoPhone)
{
	_ui = new Ui::Form();
	_ui->setupUi(this);

	//CoolView 3.0 界面元素		zhenHua.sun 2010-08-10
    _tools = new QtToolWidget( _qtWengoPhone , this );           //菜单栏

	//设置菜单栏的显示位置
	int toolWidth = _tools->_ui->dockWidget_2->width();
	int toolHeight = _tools->_ui->dockWidget_2->height();
	_tools->_ui->dockWidget_2->setGeometry( 10 ,60 , toolWidth , toolHeight);

    _toolButton = new QPushButton();   //控制菜单栏显示隐藏的按钮

    _toolButton->setMaximumSize(30,30);  //控制按钮大小
    _toolButton->setMinimumSize(30,30);

	QIcon *icon = new QIcon(":/coolview3/setting.bmp"); //按钮添加icon
    QSize *isize = new QSize(25,25);
    _toolButton->setIcon(*icon);
    _toolButton->setIconSize(*isize);

    //这段是布局，要这样布局才能实现菜单隐藏和显示不影响会议视频框布局
    QHBoxLayout *mainLayout = new QHBoxLayout();
    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->addWidget(_toolButton);
	vLayout->addSpacing(this->height()+150);      //add by huixin.du 2010-09-06
	vLayout->setMargin(0);                     //add by huixin.du 2010-09-06
    mainLayout->addLayout(vLayout);
    mainLayout->addWidget(_tools);
	mainLayout->setMargin(0);                 //add by huixin.du 2010-09-06
    _ui->centralwidget->setLayout(mainLayout);
	//_tools->show();
	//_tools->setWindowFlags(Qt::WindowStaysOnTopHint);

    //实现菜单栏隐藏和显示
	connect(_toolButton,SIGNAL(clicked()),_tools,SLOT(hideToolWidget()));    //add by huixin.du 2010-09-06
    
//程序控制
	//注销
	connect(_tools->_ui->logOutButton , SIGNAL(clicked()) , this , SLOT(logOutSlot()));
	
	//退出程序
    connect(_tools->_ui->quitButton,SIGNAL(clicked()),this,SLOT(exitProgramSlot()));

    setAttribute(Qt::WA_DeleteOnClose);

	/* 不知是否拥有，先注释掉   zhenHua.sun 2010-08-10
	confList = new QtConferenceList(_qtWengoPhone,this);
	memberList = new QtMemberList(_qtWengoPhone,this);
	
	rtcpInfoWidget = new QtRtcpInfoWidget(_qtWengoPhone);
	textChatWidget = new QtTextChatWidget(_qtWengoPhone,this);
	whiteBoardWidget = new QtWhiteBoardWidget(_qtWengoPhone,this);
	screenShareWidget = new QtScreenShare(_qtWengoPhone,this);
	
	Widget::createLayout(_ui->tab)->addWidget(confList);
	Widget::createLayout(_ui->tab_2)->addWidget(memberList);
	Widget::createLayout(_ui->tab_confrence)->addWidget(_meetingPlaceWidget);
	Widget::createLayout(_ui->tab_Rtcp)->addWidget(rtcpInfoWidget);
	Widget::createLayout(_ui->TextMsgWidget)->addWidget(textChatWidget);
	Widget::createLayout(_ui->tab_WidgetBoard)->addWidget(whiteBoardWidget);
	Widget::createLayout(_ui->tab_shareScreen)->addWidget(screenShareWidget);

	_ui->tabWidget_2->hide();
	rtcpWidgetisShow = false;
	SAFE_CONNECT(_ui->rtcpWidgetButton , SIGNAL(clicked()), SLOT(rtcpWidgetButtonClicked()));

	textMsgWidgetisShow = true;
	SAFE_CONNECT(_ui->textMsgWidgetButton , SIGNAL(clicked()), SLOT(textMsgWidgetButtonClicked()));
	*/
}

QtConferenceRoomWidget::~QtConferenceRoomWidget(void)
{
	/* 更新界面之后下面的代码无用　　zhenHua.sun 2010-08-10
	OWSAFE_DELETE(memberList);
	OWSAFE_DELETE(screenShareWidget);
	OWSAFE_DELETE(whiteBoardWidget);
	OWSAFE_DELETE(textChatWidget);
	OWSAFE_DELETE(confList);
	OWSAFE_DELETE(_meetingPlaceWidget);
	OWSAFE_DELETE(rtcpInfoWidget);
	*/
	OWSAFE_DELETE(_toolButton);
	OWSAFE_DELETE(_tools);
	OWSAFE_DELETE(_ui);
}

QtToolWidget *QtConferenceRoomWidget::GetToolWidget()
{
	return this->_tools;
}

void QtConferenceRoomWidget::exitProgramSlot()
{
	//弹出确认对话框
	QTextCodec::setCodecForTr( QTextCodec::codecForName("utf-8") );
	if( QMessageBox::question( _tools->_ui->dockWidget_2,          //modified by dhx  warning显示在工具栏中间
		tr("@product@"),
		tr("确认退出程序？"),
		QMessageBox::Ok,
		QMessageBox::Cancel) != QMessageBox::Ok )
	{
		return;
	}
	bool exit = true;
	if( _tools->isInConference() )
	{
		//如果已经进入会议则应该先退出
		exit = _tools->exitConference();
	}

	if( exit )
		_qtWengoPhone.prepareToExitApplication();
}

void QtConferenceRoomWidget::logOutSlot()
{
	//弹出确认对话框
	QTextCodec::setCodecForTr( QTextCodec::codecForName("utf-8") );
	if( QMessageBox::question( _tools->_ui->dockWidget_2,              //modified by dhx  warning显示在工具栏中间
			tr("@product@"),
			tr("确认注销账户？ "),
			QMessageBox::Ok,
			QMessageBox::Cancel) != QMessageBox::Ok )
	{
		return;
	}

	bool exit = true;
	if( _tools->isInConference() )
	{
		//如果已经进入会议则应该先退出
		exit = _tools->exitConference();
	}

	if( exit )
	{
		//销毁会议信息...zhenHua.sun 2010-12-07
		//本来想要放在QtMemberList的exitMeeting上的，但是当断网之后需要在Presentation层执行
		//退出会议的命令，否则界面没有被更新，会导致后面操作的异常，而同时也会销毁掉会议信息
		//这就导致用户回复网络后无法重新进入会议，因此放在这里（或者有更好的解决方法？）
		RunningProfile::getInstance().getSessionManager().removeConfSession();

		_qtWengoPhone.getQtToolBar().logOff();
	}
}
/*更新界面注释掉　  zhenhua.sun 2010-08-11
void QtConferenceRoomWidget::rtcpWidgetButtonClicked()
{
	if (rtcpWidgetisShow)
	{
		_ui->tabWidget_2->hide();
		_ui->rtcpWidgetButton->setIcon(QIcon(":/pics/chat/show_rtcp_widget.png"));
		rtcpWidgetisShow = false;
	}
	else
	{
		_ui->tabWidget_2->show();
		_ui->rtcpWidgetButton->setIcon(QIcon(":/pics/chat/hide_rtcp_widget.png"));
		rtcpWidgetisShow = true;
	}
	
}

void QtConferenceRoomWidget::textMsgWidgetButtonClicked()
{
	if (textMsgWidgetisShow)
	{
		_ui->TextMsgWidget->hide();
		_ui->textMsgWidgetButton->setIcon(QIcon(":/pics/chat/show_rtcp_widget.png"));
		textMsgWidgetisShow = false;
	}
	else
	{
		_ui->TextMsgWidget->show();
		_ui->textMsgWidgetButton->setIcon(QIcon(":/pics/chat/hide_rtcp_widget.png"));
		textMsgWidgetisShow = true;
	}
}

void QtConferenceRoomWidget::clearWhiteBoard()
{
	whiteBoardWidget->clearWhiteBoard();
	//????????????????????????
	screenShareWidget->stopScreenGraph();
	//??????????????
	textChatWidget->clearTextWidget();
}

QtScreenShare *QtConferenceRoomWidget::getScreenShareWidget()
{
	return screenShareWidget;
}
*/
