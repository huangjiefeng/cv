/**
 * Coolview 3.0 界面更新  
 * zhenHua.sun 2010-08-10
 */

#ifndef QTTOOLWIDGET_H
#define QTTOOLWIDGET_H

/*菜单栏，包含了呼叫（拨号、地址簿及其相关）、会议（会议列表、会议成员）、
设备管理（摄像头、音频、视频显示）、监控（主要是一些基本的网络流量信息）、
短信息、帮助（未实现），共6个部分*/

#include "QtStatusWidget.h"
#include "ui_tool.h"
#include <control/CWengoPhone.h>//Add By LZY 2010-09-25


class QtConferenceRoomWidget;
class QtConferenceList;
class QtMemberList;
class QtWengoPhone;
class QtVideoSettings;
class QtAudioSettings;
class QtShowSetting;
class QtSipCall;
class QtTextChatWidget;
class QtRtcpInfoWidget;
class QtAboutDialog;

class QtToolWidget : public QWidget
{
	friend class QtConferenceRoomWidget;

    Q_OBJECT

    public:
        QtToolWidget(QtWengoPhone& qtWengoPhone, QtConferenceRoomWidget *qtConferenceRoom = 0);
		~QtToolWidget( );
		
		/**
		 * @brief 获取会议中视频的发送状态
		 * @return 如果是1表示正在发送视频，否则为0
		 */
		int getVideoSendState();

		/**
		 * @brief 获取会议中音频的发送状态
		 * @return 如果是1表示正在发送视频，否则为0
		 */
		int getAudioSendState();

		/**
		 * @brief 获取回显窗口句柄
		 */
		QWidget *getPreviewWindow();

		/**
		 * @brief 获取qtWengoPhone
		 */
		QtWengoPhone& getQtWengoPhone(){ return _qtWengoPhone; }

		/**
		 * @brief 获取呼叫面板
		 */
		QtSipCall* getQtSipCall(){	return _qtSipCall; }

		/**
		 * @brief 控制视频发送
		 * @state 0表示停止发送， 1表示启动发送
		 */
		void setVideoSendStateSlot( int state );

		/**
		 * @brief 控制音频发送
		 * @state 0表示停止发送， 1表示启动发送
		 */
		void setAudioSendStateSlot( int state );

		void setIsInConference( bool inConf ){ _isInConference = inConf; }
		bool isInConference(void){ return _isInConference; }

		/**
		 * @brief 退出会议
		 */
		bool exitConference();

		
		Ui::toolbar* _ui;
		QtConferenceList *_confList;
		QtMemberList *_memberList;
		QtAboutDialog *_qtAboutDialog;
		QtWengoPhone& _qtWengoPhone;
         
	public Q_SLOTS:
		void hideToolWidget();  //add by huixin.du 2010-09-06

    private:
        QtStatusWidget *status;
        QAction *delConfAction;  //取消会议的左键点击action
		QtConferenceRoomWidget* _qtConferenceRoom;

		///视频设置窗口
		QtVideoSettings* _qtVideoSetting;

		///音频设置窗口
		QtAudioSettings* _qtAudioSetting;

		///视频窗口格局设置窗口
		QtShowSetting * _qtShowSetting;

		///呼叫窗口
		QtSipCall* _qtSipCall;

		//消息窗口
		QtTextChatWidget* _qtMessage;

		//网络状态窗口
		QtRtcpInfoWidget* _qtNetworkCondition;

		//是否进入了会议
		bool _isInConference;
		
		
};

#endif // QTTOOLWIDGET_H
