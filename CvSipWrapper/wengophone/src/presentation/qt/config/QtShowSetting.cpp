/**
 * Coolview 3.0 界面更新  
 * zhenHua.sun 2010-08-14
 */

#include "stdafx.h"
#include "QtShowSetting.h"
#include "QtWengoPhone.h"
#include "..\conference\QtToolWidget.h"
#include <dbus/conferenceRoom/ConfRoomClient.h>		//zhenHua.sun 2010-10-25

QtShowSetting::QtShowSetting(QtToolWidget* qtToolWidget) : _qtTool( qtToolWidget )
{
	_ui = new Ui::showSettingForm();
	_ui->setupUi(this);

#ifdef DBUS_ENABLE
	//下面的信号和槽是控制分屏的
	connect(_ui->oneRadioButton,SIGNAL(clicked()),this,SLOT(updateConfRoomDisplay1X1()));
	connect(_ui->twoRadioButton,SIGNAL(clicked()),this,SLOT(updateConfRoomDisplay2X2()));
	connect(_ui->threeRadioButton,SIGNAL(clicked()),this,SLOT(updateConfRoomDisplay3X3()));
	connect(_ui->lRadioButton,SIGNAL(clicked()),this,SLOT(updateConfRoomDisplayL3X3()) );
	connect(_ui->allRadioButton,SIGNAL(clicked()),this,SLOT(updateConfRoomDisplayAll()) );
	connect(_ui->l4x4RadioButton,SIGNAL(clicked()),this,SLOT(updateConfRoomDisplayL4X4()) );
	connect(_ui->l5x5RadioButton,SIGNAL(clicked()),this,SLOT(updateConfRoomDisplayL5X5()) );
	SAFE_CONNECT_RECEIVER(&(_qtTool->_qtWengoPhone),SIGNAL(ActionSetLayoutTypeSignal(QString)),this,SLOT(teleSetLayoutTypeCommand(QString)));//Add By LZY 2010-11-03 绑定响应SetLayoutType动作Signal的函数
#endif

}

QtShowSetting::~QtShowSetting()
{
	disconnect(&(_qtTool->_qtWengoPhone),SIGNAL(ActionSetLayoutTypeSignal(QString)),this,SLOT(teleSetLayoutTypeCommand(QString)));//Add By LZY 2010-11-03 解绑响应SetLayoutType动作Signal的函数
	OWSAFE_DELETE(_ui);
}

void QtShowSetting::updateConfRoomDisplay1X1()
{
	ConfRoomDBusClient::getInstance()->SetDisplayModel( CF_DM_1X1 );
}

void QtShowSetting::updateConfRoomDisplay2X2()
{
	ConfRoomDBusClient::getInstance()->SetDisplayModel( CF_DM_2X2 );
}

void QtShowSetting::updateConfRoomDisplay3X3()
{
	ConfRoomDBusClient::getInstance()->SetDisplayModel( CF_DM_3X3 );
}

void QtShowSetting::updateConfRoomDisplayL3X3()
{
	ConfRoomDBusClient::getInstance()->SetDisplayModel( CF_DM_L3X3 );
}

void QtShowSetting::updateConfRoomDisplayAll()
{
	ConfRoomDBusClient::getInstance()->SetDisplayModel( CF_DM_ALL );
}

void QtShowSetting::updateConfRoomDisplayL4X4()
{
	ConfRoomDBusClient::getInstance()->SetDisplayModel( CF_DM_L4X4 );
}

void QtShowSetting::updateConfRoomDisplayL5X5()
{
	ConfRoomDBusClient::getInstance()->SetDisplayModel( CF_DM_L5X5 );
}

/*
 * Add By LZY 2010-11-03
 * 处理遥控器SetLayoutType动作Slot
 * @param QString LayoutType,布局类型字符串
 * @return void
 */
void QtShowSetting::teleSetLayoutTypeCommand(QString LayoutType)
{
	QByteArray LayoutByte = LayoutType.toUtf8();
	const char *layoutStr = LayoutByte.data();
    if(strcmp(layoutStr,"1")==0)
		QtShowSetting::updateConfRoomDisplay1X1();
	else if(strcmp(layoutStr,"2")==0)
		QtShowSetting::updateConfRoomDisplay2X2();
	else if(strcmp(layoutStr,"3")==0)
		QtShowSetting::updateConfRoomDisplay3X3();
	else if(strcmp(layoutStr,"L")==0)
		QtShowSetting::updateConfRoomDisplayL3X3();
	else if(strcmp(layoutStr,"A")==0)
		QtShowSetting::updateConfRoomDisplayAll();
	else if(strcmp(layoutStr,"L4")==0 )
		QtShowSetting::updateConfRoomDisplayL4X4();
	else if(strcmp(layoutStr,"L5")==0)
		QtShowSetting::updateConfRoomDisplayL5X5();
}
