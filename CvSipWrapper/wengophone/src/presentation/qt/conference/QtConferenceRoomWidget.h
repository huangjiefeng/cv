	#pragma once
#include "ui_ConferenceRoomWidget.h"
//coolview3.0 界面更新

class QtWengoPhone;
class QtToolWidget;
class QScrollArea;
class QPushButton;

class QtConferenceRoomWidget:public QWidget
{
	friend class QtWengoPhone;  //huixin.du  2010-09-16
	Q_OBJECT
public:
	QtConferenceRoomWidget(QtWengoPhone & qtWengoPhone);
	~QtConferenceRoomWidget(void);
    
    QPushButton *_toolButton;     //控制菜单栏显示隐藏的按钮

	//Coolview 3.0 界面方法 zhenHua.sun 2010-08-11
	QtToolWidget *GetToolWidget();

Q_SIGNALS:

private Q_SLOTS:

	/**
	 * @brief 退出程序
	 */
	void exitProgramSlot();
	/**
	 * @brief 用户注销
	 */
	void logOutSlot();

private:
	Ui::Form *_ui;
	QtToolWidget *_tools;             //菜单栏
	QtWengoPhone & _qtWengoPhone;
};
