#include "qtmeetingplacewidget.h"
#include "QtConfrenceVideoWindow.h"
#include "VideoFrame.h"
//#include <dbus/telecontroller/CvTelecontrollerClient.h>
#include <QtWidgets/QtWidgets>
#include <log/Log.h>



#include "ConferenceRoomService.h"

#define VIDEONUM	16
#define SAFE_DELETE(x)   { if( (x) ) {  delete (x); (x)=NULL; } }

QtMeetingPlaceWidget::QtMeetingPlaceWidget( QtConferenceRoom* confRoom ): _qtConferenceRoom(confRoom)
{
	_ui = new Ui::MeetingPlaceWidget();
	_ui->setupUi(this);

	//设置会议视频框所在的scrollArea大小，
	//限制了最大值才能有滑动功能，并且显示全部视频的时候
	//才不会导致视频框严重变形
	int x = QApplication::desktop()->width();  //获取系统桌面大小
	int y = QApplication::desktop()->height();
	_ui->scrollArea->setMaximumSize(x, y);

	widget = new QWidget;
	mainLayout = new QGridLayout();
	mainLayout->setMargin(0);           //add by huixin.du 2010-09-06
	mainLayout->setSpacing(0);           //add by huixin.du 2010-09-06

	//connect(this, SIGNAL(closeVideoWinSinal(QtConfrenceVideoWindow * )), this ,SLOT(closeVideoWinSlot(QtConfrenceVideoWindow * )) );

	////移动到meetingpalce中进行连接
	//connect( this, SIGNAL(windowResizeSignal(QString ,int)) , _qtConferenceRoom->getConferenceRoomService() , SLOT(windowResizeSlot(QString,int)) );

	for (int i=0;i<VIDEONUM;i++)
	{
		VideoFrame *vFrame = new VideoFrame(this);
		memberFrames.push_back(vFrame);
	}

    _displayModel = CF_DM_UNKNOWN;
	//sizeL3And3();
    //if (confRoom->getScreenIndex() == 0) {
    //  sizeTop1And2();
    //} else {
      size2And2();
    //}

	//zhenHua.sun 2010-09-03
	showRtcpMessageEnbaled = false;

	////zhenHua.sun 2010-12-07
	//_qtTranStateInfo = new QtRtcpInfoWidget( );
	//connect( this , SIGNAL(updateTranStateSignal( const TranStatInfo& )) ,_qtTranStateInfo,
	//	SLOT(displayTranStatSlot(const TranStatInfo&)) );
}

QtMeetingPlaceWidget::~QtMeetingPlaceWidget(void)
{	
	disconnect(this, SIGNAL(closeVideoWinSinal(QtConfrenceVideoWindow * )), this ,SLOT(closeVideoWinSlot(QtConfrenceVideoWindow * )) );
	
	////zhenHua.sun 2010-12-07
	//disconnect( this , SIGNAL(updateTranStateSignal( const TranStatInfo& )) ,_qtTranStateInfo,
	//	SLOT(displayTranStatSlot(const TranStatInfo&)) );
	//SAFE_DELETE( _qtTranStateInfo );

	//for (int i=0;i<memberFrames.size();i++)
	//{
	//	SAFE_DELETE(memberFrames[i]);
	//}
	SAFE_DELETE(widget);
	SAFE_DELETE(_ui);
}
//
//bool QtMeetingPlaceWidget::checkUserExist(std::string &userId)
//{
//	//for (map<QtConfrenceVideoWindow *,QMdiSubWindow *>::iterator it = video_subwindow_map.begin();it!=video_subwindow_map.end();it++)
//	//{
//	//	QtConfrenceVideoWindow *videoWindow = (QtConfrenceVideoWindow *)(it->first);
//	//	if (userId == videoWindow->getUserId())
//	//	{
//	//		return true;
//	//	}
//	//}
//	for (map<QtConfrenceVideoWindow *,VideoFrame *>::iterator it = video_frame_map.begin();it!=video_frame_map.end();it++)
//	{
//		QtConfrenceVideoWindow *videoWindow = (QtConfrenceVideoWindow *)(it->first);
//		if (userId == videoWindow->getUserId())
//		{
//			return true;
//		}
//	}
//	return false;
//}
//
//void QtMeetingPlaceWidget::closeWindow(const std::string &userId)
//{
//	//for (map<QtConfrenceVideoWindow *,QMdiSubWindow *>::iterator it = video_subwindow_map.begin();it!=video_subwindow_map.end();it++)
//	//{
//	//	QtConfrenceVideoWindow *videoWindow = (QtConfrenceVideoWindow *)(it->first);
//	//	if (userId == videoWindow->getUserId())
//	//	{
//	//		closeVideoWinSinal(videoWindow);
//	//		break;
//	//	}
//	//}
//	for (map<QtConfrenceVideoWindow *,VideoFrame *>::iterator it = video_frame_map.begin();it!=video_frame_map.end();it++)
//	{
//		QtConfrenceVideoWindow *videoWindow = (QtConfrenceVideoWindow *)(it->first);
//		if (userId == videoWindow->getUserId())
//		{
//			closeVideoWinSlot(videoWindow);
//			break;
//		}
//	}
//
//	//QtMeetingPlaceWidget::teleChangeLayoutNotify();//Add By LZY 2010-10-09 主动通知遥控器当前视频布局发生了变化
//}
//
//void QtMeetingPlaceWidget::closeAllWindow()
//{
//
//	//注意这种信号与槽的调用，可能会导致容器for循环的失败
//	//待调用的槽中最好不要使用到当前在枚举的容器
//	//for (map<QtConfrenceVideoWindow *,VideoFrame *>::iterator it = video_frame_map.begin();it!=video_frame_map.end();it++)
//	//{
//	//	QtConfrenceVideoWindow *videoWindow = (QtConfrenceVideoWindow *)(it->first);
//	//	closeVideoWinSinal(videoWindow);
//	//}
//	for( int i=0 ; i<memberFrames.size() ; i++ )
//	{
//		VideoFrame* frame = memberFrames[i];
//		if( frame->isUsed )
//		{
//			if( frame->LinkWin )
//			{
//				this->closeVideoWinSlot( frame->LinkWin );
//			}
//		}
//	}
//}
//
//void QtMeetingPlaceWidget::closeVideoWinSlot(QtConfrenceVideoWindow * videoWin)
//{
//
//	VideoFrame *frame = video_frame_map[videoWin];
//	frame->isUsed = false;
//	frame->LinkWin = NULL;//Add By LZY 2010-10-09 删除关联的窗口指针
//	videoWin->close();
//	delete videoWin;
//	video_frame_map.erase(videoWin);
//
//	////重新排列视频窗口并按当前模式显示...zhenHua.sun 2010-09-15
//	//for( int i=0 ; i<memberFrames.size() ; i++ )
//	//{
//	//	if( memberFrames[i]->isUsed )
//	//		continue;
//	//	if( i<memberFrames.size()-1 )
//	//	{
//	//		VideoFrame * tmp = memberFrames[i];
//	//		memberFrames[i] = memberFrames[i+1];
//	//		memberFrames[i+1] = tmp;	//把空的往后推
//	//	}
//	//}
//	//updateDisplay( _displayModel );
//}
//
//void QtMeetingPlaceWidget::unReceiveMedia(QString userId)//Remove Param Const By LZY 2010-09-28
//{
//	closeWindow(userId.toStdString());
//	//取消接收某一路视频，考虑采用HWND作为索引
//	//_cWengoPhone.getCUserProfileHandler().getCUserProfile()->media_recv_stop(userId.toStdString());
//}
//
//QtConfrenceVideoWindow *QtMeetingPlaceWidget::GetUseableWin()
//{
//	QtConfrenceVideoWindow *child = new QtConfrenceVideoWindow(this);	
//	int i=0;
//	for (i=0;i<memberFrames.size();i++)
//	{
//		if(!memberFrames[i]->isUsed)
//		{
//			memberFrames[i]->layout.addWidget(child);
//			memberFrames[i]->LinkWin = child;//Add By LZY 2010-10-09 加入关联的窗口指针
//			memberFrames[i]->isUsed = true;
//			video_frame_map[child] = memberFrames[i];
//			break;
//		}
//	}
//	//如果没有可用的窗口，则新建一个
//	if(i==memberFrames.size())
//	{
//		VideoFrame *vFrame = new VideoFrame();
//		vFrame->layout.addWidget(child);
//		vFrame->LinkWin = child;//Add By LZY 2010-10-09 加入关联的窗口指针
//		vFrame->isUsed = true;
//		video_frame_map[child] = vFrame;
//		memberFrames.push_back(vFrame);
//		sizeAll();
//	}
//	//
//	//QMdiSubWindow *subWindow1 = new QMdiSubWindow;
//	//subWindow1->resize(352,288);
//	//subWindow1->setWidget(child);
//	//subWindow1->setAttribute(Qt::WA_DeleteOnClose);
//	//
////	_ui->mdiArea->addSubWindow(subWindow1,Qt::FramelessWindowHint);
//	child->show();
////	video_subwindow_map[child] = subWindow1;
//	return child;
//}
//
//void QtMeetingPlaceWidget::setSizeSlot(QtConfrenceVideoWindow *videoWin,int width,int height)
//{
////	QMdiSubWindow *subWindow = video_subwindow_map[videoWin];
////	QSize size = subWindow->size();
////	int resizeWidth = size.width()+width;
////	resizeWidth = resizeWidth<176?176:resizeWidth;
////	int resizeHeight = size.height()+height;
////	resizeHeight = resizeHeight<144?144:resizeHeight;
////	if (resizeWidth>=176&&resizeHeight>=144)
////	{
////		subWindow->resize(resizeWidth,resizeHeight);
////	}
////	
//}
//
//void QtMeetingPlaceWidget::mouseMoveSlot(QtConfrenceVideoWindow *videoWin,const QPoint &point)
//{
//	//QMdiSubWindow *subWindow = video_subwindow_map[videoWin];
//	//QPoint startPoint = _ui->mdiArea->mapToGlobal(QPoint(0,0));
//	//subWindow->move(point.x()-startPoint.x(),point.y()-startPoint.y());
////	win->move(point.x()-startPoint.x(),point.y()-startPoint.y());
//}

void QtMeetingPlaceWidget::sizeAll()
{
    if (_displayModel == CF_DM_ALL) return;
     QRect rect = QApplication::desktop()->availableGeometry(this);
     _qtConferenceRoom->showMaximizedToScreen();

	for (int i=0;i<memberFrames.size();i++)
	{
		memberFrames[i]->setVisible(true);
		int r = i/3;
		int c = i%3;
		mainLayout->addWidget(memberFrames[i],r,c);
	}
	/*mainLayout->setSpacing(0);	mainLayout->setMargin(0);*/	
  widget->setLayout(mainLayout);
	_ui->scrollArea->setWidget(widget);

	_displayModel = CF_DM_ALL;

	//QtMeetingPlaceWidget::teleChangeLayoutNotify();//Add By LZY 2010-10-09 主动通知遥控器当前视频布局发生了变化
}


void QtMeetingPlaceWidget::size3And3()
{
    if (_displayModel == CF_DM_3X3) return;
	//removeAllWidget();
    _qtConferenceRoom->showMaximizedToScreen();

    for (int i=0;i<memberFrames.size();i++)
    {
        memberFrames[i]->setHidden(true);
    }

	int i;
	for (i=0;i<9;i++)
	{
		memberFrames[i]->setVisible(true);
		int r = i/3;
		int c = i%3;
		mainLayout->addWidget(memberFrames[i],r,c);
    }
	/*mainLayout->setSpacing(0);	mainLayout->setMargin(0);*/	
  widget->setLayout(mainLayout);
	_ui->scrollArea->setWidget(widget);

	_displayModel = CF_DM_3X3;

	//QtMeetingPlaceWidget::teleChangeLayoutNotify();//Add By LZY 2010-10-09 主动通知遥控器当前视频布局发生了变化
}

void QtMeetingPlaceWidget::size2And2()
{
    if (_displayModel == CF_DM_2X2) return;
    _qtConferenceRoom->showMaximizedToScreen();
	if (QtMeetingPlaceWidget::_displayModel != CF_DM_2X2)
	{
        for (int i=0;i<memberFrames.size();i++)
        {
            memberFrames[i]->hide();
        }
		int i;
		for (i=0;i<4;i++)
		{
			int r = i/2;
			int c = i%2;
			mainLayout->addWidget(memberFrames[i],r,c);
            memberFrames[i]->show();
		}
		/*mainLayout->setSpacing(0);	
          mainLayout->setMargin(0);	*/
        widget->setLayout(mainLayout);
		_ui->scrollArea->setWidget(widget);
	}
	_displayModel = CF_DM_2X2;

	//QtMeetingPlaceWidget::teleChangeLayoutNotify();//Add By LZY 2010-10-09 主动通知遥控器当前视频布局发生了变化
}

void QtMeetingPlaceWidget::size1And1()
{	
    if (_displayModel == CF_DM_1X1) return;
    _qtConferenceRoom->showMaximizedToScreen();

	//memberFrames[0]->setGeometry( 0,0 ,0, 0);			//防止在1*1Real布局改成当前布局的时候frame没有调用resizeEvent事件
	for (int i=0;i<memberFrames.size();i++)
	{
		memberFrames[i]->hide();
	}
    mainLayout->addWidget(memberFrames[0],0,0,1,1);
    memberFrames[0]->show();
	
	/*mainLayout->setSpacing(0);	mainLayout->setMargin(0);*/	
    widget->setLayout(mainLayout);
	_ui->scrollArea->setWidget(widget);

	_displayModel = CF_DM_1X1;

	//QtMeetingPlaceWidget::teleChangeLayoutNotify();//Add By LZY 2010-10-09 主动通知遥控器当前视频布局发生了变化
}

void QtMeetingPlaceWidget::sizeL3And3()
{
    if (_displayModel == CF_DM_L3X3) return;
    _qtConferenceRoom->showMaximizedToScreen();

	int i;

	for (i=0;i<memberFrames.size();i++)
	{
		memberFrames[i]->setHidden(true);
	}
	for (i=0;i<6;i++)
	{
		memberFrames[i]->setVisible(true);
	}
	mainLayout->addWidget(memberFrames[0],0,0,2,2);
	mainLayout->addWidget(memberFrames[1],0,2);
	mainLayout->addWidget(memberFrames[2],1,2);
	mainLayout->addWidget(memberFrames[3],2,0);
	mainLayout->addWidget(memberFrames[4],2,1);
	mainLayout->addWidget(memberFrames[5],2,2);
	/*mainLayout->setSpacing(0);	mainLayout->setMargin(0);*/	
  widget->setLayout(mainLayout);
	_ui->scrollArea->setWidget(widget);

	_displayModel = CF_DM_L3X3;

	//QtMeetingPlaceWidget::teleChangeLayoutNotify();//Add By LZY 2010-10-09 主动通知遥控器当前视频布局发生了变化
}

void QtMeetingPlaceWidget::sizeL4And4()
{
    if (_displayModel == CF_DM_L4X4) return;
    _qtConferenceRoom->showMaximizedToScreen();

	int i;
	for (i=0;i<8;i++)
	{
		memberFrames[i]->setVisible(true);
	}
	for (i=8;i<memberFrames.size();i++)
	{
		memberFrames[i]->setHidden(true);
	}
	mainLayout->addWidget(memberFrames[0],0,0,3,3);
	mainLayout->addWidget(memberFrames[1],0,3);
	mainLayout->addWidget(memberFrames[2],1,3);
	mainLayout->addWidget(memberFrames[3],2,3);
	mainLayout->addWidget(memberFrames[4],3,0);
	mainLayout->addWidget(memberFrames[5],3,1);
	mainLayout->addWidget(memberFrames[6],3,2);
	mainLayout->addWidget(memberFrames[7],3,3);
	/*mainLayout->setSpacing(0);	mainLayout->setMargin(0);*/	
  widget->setLayout(mainLayout);
	_ui->scrollArea->setWidget(widget);

	_displayModel = CF_DM_L4X4;

	//QtMeetingPlaceWidget::teleChangeLayoutNotify();//Add By LZY 2010-10-09 主动通知遥控器当前视频布局发生了变化
}

void QtMeetingPlaceWidget::sizeL5And5()
{
    if (_displayModel == CF_DM_L5X5) return;
    _qtConferenceRoom->showMaximizedToScreen();

    int i;
	for (i=0;i<10;i++)
	{
		if( i<memberFrames.size() )
		{	
			memberFrames[i]->setVisible(true);
		}else
		{
			//如果memberFrame不够，则新建
			VideoFrame *vFrame = new VideoFrame( this);
			memberFrames.push_back(vFrame);
		}
	}
	for (i=10;i<memberFrames.size();i++)
	{
		memberFrames[i]->setHidden(true);
	}
	mainLayout->addWidget(memberFrames[0],0,0,4,4);
	mainLayout->addWidget(memberFrames[1],0,4);
	mainLayout->addWidget(memberFrames[2],1,4);
	mainLayout->addWidget(memberFrames[3],2,4);
	mainLayout->addWidget(memberFrames[4],3,4);
	mainLayout->addWidget(memberFrames[5],4,0);
	mainLayout->addWidget(memberFrames[6],4,1);
	mainLayout->addWidget(memberFrames[7],4,2);
	mainLayout->addWidget(memberFrames[8],4,3);
	mainLayout->addWidget(memberFrames[9],4,4);
	/*mainLayout->setSpacing(0);	mainLayout->setMargin(0);*/	
  widget->setLayout(mainLayout);
	_ui->scrollArea->setWidget(widget);

	_displayModel = CF_DM_L5X5;

	//QtMeetingPlaceWidget::teleChangeLayoutNotify();//Add By LZY 2010-10-09 主动通知遥控器当前视频布局发生了变化
}

void QtMeetingPlaceWidget::sizeTop1And2() {
    if (_displayModel == CF_DM_TOP_1_2) {
      return;
    }
    _qtConferenceRoom->showTopHalfToScreen();
    QRect rect = QApplication::desktop()->availableGeometry(this);
    int i;
    for (i=0; i<memberFrames.size(); i++)
    {
      memberFrames[i]->hide();
    }
    
    mainLayout->addWidget(memberFrames[0], 0, 0);
    mainLayout->addWidget(memberFrames[1], 0, 1);

    for (i=0; i<2; i++)
    {
      memberFrames[i]->show();
    }

    /*mainLayout->setSpacing(0);
    mainLayout->setMargin(0);*/
    widget->setLayout(mainLayout);
    _ui->scrollArea->setWidget(widget);

    _displayModel = CF_DM_TOP_1_2;
}


void QtMeetingPlaceWidget::sizeAUTO()
{
    if (_displayModel == CF_DM_AUTO) return;

    _qtConferenceRoom->showMaximizedToScreen();

	QRect rect = QApplication::desktop()->availableGeometry(this);	
	int width = rect.width();
	int height = rect.height();
	
	if(width>1280 && height>760)
	{
			
		if(width==1920 && height==1080)
		{
			delete mainLayout;
			mainLayout = NULL;
			int window_w = 384;
			//加20是因为视频窗口下面的终端名字栏需要占用了20像素  by liang.zhongbao
			int window_h = 216+20;
			int width_offset = (width-window_w*5)/6;
			int height_offset = 72;

			memberFrames[0]->setVisible(true);
			memberFrames[0]->setGeometry((width-1280)/2,height_offset/2+5,1280,740);

			int i;

			for (i=1;i<6;i++)
			{
				memberFrames[i]->setVisible(true);
				int w = width_offset*i+window_w*(i-1);
				memberFrames[i]->setGeometry(w,760+height_offset,window_w,window_h);


			}
			for(i=6;i<memberFrames.size();i++)
			{

				memberFrames[i]->setHidden(true);
			}


		}
		else
		{

			delete mainLayout;
			mainLayout = NULL;
			int window_w = width/5;
			int window_h = window_w*9/16;
			int reference_w = (height-760)*16/9;
			//选择小窗口的宽和高 使得小窗口符合16:9的同时，又不超过分辨率的限制
			if(window_w > reference_w)
			{
				window_w = reference_w;
				window_h = height-760;
			}
			int width_offset = (width-window_w*5)/6;
			int height_offset = (height-760-window_h)/2;

			memberFrames[0]->setVisible(true);
			memberFrames[0]->setGeometry((width-1280)/2,10,1280,740);

			int i;

			for (i=1;i<6;i++)
			{
				memberFrames[i]->setVisible(true);
				int w = width_offset*i+window_w*(i-1);
				memberFrames[i]->setGeometry(w,760+height_offset,window_w,window_h);


			}
			for(i=6;i<memberFrames.size();i++)
			{

				memberFrames[i]->setHidden(true);
			}


		}

	}
	
	else
	{
		//如果分辨率低于1280×720的话就是用L布局
		sizeL3And3();
	}
	

	_displayModel = CF_DM_AUTO;


}





void QtMeetingPlaceWidget::sizeL1_20()
{
    if (_displayModel == CF_DM_L1_20) return;
    _qtConferenceRoom->showMaximizedToScreen();

	int i;
	for (i=0;i<21;i++)
	{
		if( i<memberFrames.size() )
		{	
			memberFrames[i]->setVisible(true);
		}else
		{
			//如果memberFrame不够，则新建
			VideoFrame *vFrame = new VideoFrame( this);
			memberFrames.push_back(vFrame);
		}
	}
	for (i=21;i<memberFrames.size();i++)
	{
		memberFrames[i]->setHidden(true);
	}
	mainLayout->addWidget(memberFrames[0],0,0,4,4);
	mainLayout->addWidget(memberFrames[1],0,4);
	mainLayout->addWidget(memberFrames[2],0,5);
	mainLayout->addWidget(memberFrames[3],1,4);
	mainLayout->addWidget(memberFrames[4],1,5);
	mainLayout->addWidget(memberFrames[5],2,4);
	mainLayout->addWidget(memberFrames[6],2,5);
	mainLayout->addWidget(memberFrames[7],3,4);
	mainLayout->addWidget(memberFrames[8],3,5);
	mainLayout->addWidget(memberFrames[9],4,0);
	mainLayout->addWidget(memberFrames[10],4,1);
	mainLayout->addWidget(memberFrames[11],4,2);
	mainLayout->addWidget(memberFrames[12],4,3);
	mainLayout->addWidget(memberFrames[13],4,4);
	mainLayout->addWidget(memberFrames[14],4,5);
	mainLayout->addWidget(memberFrames[15],5,0);
	mainLayout->addWidget(memberFrames[16],5,1);
	mainLayout->addWidget(memberFrames[17],5,2);
	mainLayout->addWidget(memberFrames[18],5,3);
	mainLayout->addWidget(memberFrames[19],5,4);
	mainLayout->addWidget(memberFrames[20],5,5);
	mainLayout->setSpacing(0);	mainLayout->setMargin(0);	widget->setLayout(mainLayout);
	_ui->scrollArea->setWidget(widget);

	_displayModel = CF_DM_L1_20;

	//QtMeetingPlaceWidget::teleChangeLayoutNotify();//Add By LZY 2010-10-09 主动通知遥控器当前视频布局发生了变化

}

void QtMeetingPlaceWidget::showVideoWindow( VideoFrame* frame )
{
	if( memberFrames.size() < frame->_seet )
	{
		//如果队列长度小于frame所在的位置，那么应该填充队列
		_snprintf(__global_msg,sizeof(__global_msg), "添加的视频窗口位置%d超出了视频窗口队列的大小%d",
			frame->_seet , memberFrames.size( ) );
		CONFROOM_LOG_ERROR(__global_msg );
		
		return;
	}else
	{
		//如果队列长度够长，需要判断对应的位置是否已被占用
		int index = frame->_seet - 1;
		VideoFrame* oldFrame = memberFrames.at(index);
		if( oldFrame->LinkWin!=NULL )
		{
			if (oldFrame->LinkWin == frame->LinkWin)
			{
				//是同一个窗口
				return;
			}
			//停止原有的媒体流并隐藏窗口
			ConferenceRoomService::getInstance()->CloseMediaWindow( oldFrame->LinkWin->getUserId());
		}

		//移除oldFrame里面所有的widget
		for( int i=0 ; i<oldFrame->layout.count(); i++ )
		{
			if(oldFrame->layout.itemAt(i)->widget() )
				oldFrame->layout.itemAt(i)->widget()->hide();
			oldFrame->layout.removeItem( oldFrame->layout.itemAt(i) );
		}

		oldFrame = memberFrames.at(index);
		oldFrame->_screenIndex = frame->_screenIndex;
		oldFrame->_seet = frame->_seet;
		oldFrame->isUsed = true;
		oldFrame->layout.addWidget(frame->LinkWin);
		oldFrame->LinkWin = frame->LinkWin;
		oldFrame->_videoHeight = frame->_videoHeight;
		oldFrame->_videoWidth = frame->_videoWidth;

		oldFrame->LinkWin->show();
	}
}
//
//void QtMeetingPlaceWidget::addVideoWindow()
//{
//	QtConfrenceVideoWindow *videoWin = GetUseableWin();
//	videoWin->setUserId("test");
//	//QtMeetingPlaceWidget::teleChangeLayoutNotify();//Add By LZY 2010-10-09 主动通知遥控器当前视频布局发生了变化
//}
//
////-------Modify By LZY 2010-09-27 Old Name teleGridLayout-------
///**
//* Add by: hexianfu
//* for the telecontroller to control the gridLayout
//* 2010-4-16
//*/
//
//void QtMeetingPlaceWidget::teleSetLayoutTypeCommand(QString LayoutType)
//{
//	QByteArray layoutByte = LayoutType.toUtf8();
//	const char *layoutStr = layoutByte.data();
//    if(strcmp(layoutStr,"1")==0)
//		size1And1();
//	else if(strcmp(layoutStr,"2")==0)
//		size2And2();
//	else if(strcmp(layoutStr,"3")==0)
//		size3And3();
//	else if(strcmp(layoutStr,"L")==0)
//		sizeL3And3();
//	else if(strcmp(layoutStr,"A")==0)
//		sizeAll();
//}
///***********************************************/
////-------End of Modify By LZY----------
//
///**
//* @brief RTCP信息显示开关
//* @param visiable true则显示，否则不显示
//*/
//void QtMeetingPlaceWidget::showRtcpMessage( bool visiable )
//{
//	if( video_frame_map.size()>0 )
//	{
//		for (map<QtConfrenceVideoWindow *,VideoFrame *>::iterator it = video_frame_map.begin();it!=video_frame_map.end();it++)
//		{
//			QtConfrenceVideoWindow *videoWindow = (QtConfrenceVideoWindow *)(it->first);
//
//			//如果是本地回显窗口则不显示...zhenHua.sun 2010-09-21
//			if( videoWindow->getUserId()==LOCAL_PLAY_UERID )
//				continue;
//
//			if( visiable )
//				videoWindow->showRtcpMessage();
//			else
//				videoWindow->hideRtcpMessage();
//		}
//	}
//	//	_qtTranStateInfo->showTranStateInfoSlot( visiable );
//
//	showRtcpMessageEnbaled = visiable;
//
//
//}
//
//void QtMeetingPlaceWidget::updateRtcpEventHandler( const RtpStatItemInfo &rtpstatInfo )
//{
//	//只显示与视频有关的统计信息...zhenHua.sun 2010-09-14
//	if( rtpstatInfo.media_type != eMT_Video )
//		return;
//
//	for( map<QtConfrenceVideoWindow* , VideoFrame* >::iterator it = video_frame_map.begin();
//			it!= video_frame_map.end(); it++ )
//	{
//		QtConfrenceVideoWindow* videoWindow = (QtConfrenceVideoWindow*)(it->first );
//		QString videoUserId = QString::fromLocal8Bit(videoWindow->getUserId().c_str());
//		if( videoUserId==rtpstatInfo.user_id )
//		{
//			videoWindow->updateRtcpEventHandler( rtpstatInfo );
//		}
//	}
//
//}
//
//HWND QtMeetingPlaceWidget::setPreviewWnd()
//{
//	//首先判断是否存在回显窗口
//	for (map<QtConfrenceVideoWindow *,VideoFrame *>::iterator it = video_frame_map.begin();it!=video_frame_map.end();it++)
//	{
//		QtConfrenceVideoWindow *videoWindow = (QtConfrenceVideoWindow *)(it->first);
//		if( videoWindow->getUserId()==LOCAL_PLAY_UERID )
//		{
//			return videoWindow->getVideoFrame()->winId();
//		}
//	}
//
//	//创建回显窗口
//	QtConfrenceVideoWindow* previewWindow = GetUseableWin();
//	previewWindow->hideRtcpMessage();		//隐藏掉rtcp信息
//	previewWindow->setUserId(LOCAL_PLAY_UERID);
//	//QtMeetingPlaceWidget::teleChangeLayoutNotify();//Add By LZY 2010-10-09 主动通知遥控器当前视频布局发生了变化
//	previewWindow->_ui.closeButton->setVisible(false);
//	HWND previewWnd = previewWindow->getVideoFrame()->winId();
//
//	return previewWnd;
//}
//
//HWND QtMeetingPlaceWidget::getPreviewWnd()
//{
//	for (map<QtConfrenceVideoWindow *,VideoFrame *>::iterator it = video_frame_map.begin();it!=video_frame_map.end();it++)
//	{
//		QtConfrenceVideoWindow *videoWindow = (QtConfrenceVideoWindow *)(it->first);
//		if( videoWindow->getUserId()==LOCAL_PLAY_UERID )
//		{
//			return videoWindow->getVideoFrame()->winId();
//		}
//	}
//	//如果没有的话则创建一个
//	return setPreviewWnd();
//	
//}

void QtMeetingPlaceWidget::updateDisplay( ConfRoomDisplayModel displayModel )
{

	if( mainLayout==NULL )
	{
		mainLayout = new QGridLayout();
		mainLayout->setMargin(0);           
		mainLayout->setSpacing(0);
	}

	memberFrames[0]->_showRealResolution = false;		//关闭根据真实视频分辨率显示视频窗口的开关
	memberFrames[0]->setMinimumSize( QSize(0,0) );

	switch( displayModel )
	{
	case CF_DM_1X1:
		size1And1();break;
	case CF_DM_2X2:
		size2And2();break;
	case CF_DM_3X3:
		size3And3();break;
	case CF_DM_L3X3:
		sizeL3And3();break;
	case CF_DM_L4X4:
		sizeL4And4();break;
	case CF_DM_L5X5:
		sizeL5And5();break;
	case CF_DM_ALL:
		sizeAll();break;
	case CF_DM_L1_5:
		sizeL3And3();break;
	case CF_DM_L1_20:
		sizeL1_20();break;
	case CF_DM_1X1_REAL:
		size1And1Real();break;
	case CF_DM_AUTO:
		sizeAUTO();break;
    case CF_DM_TOP_1_2:
        sizeTop1And2(); break;
    case CF_DM_4X4:
        size4And4(); break;
	default:
		sizeAll();
	}
}

void QtMeetingPlaceWidget::hideVideoWindow( VideoFrame* frame )
{
	int index = frame->_seet -1 ;
	if( index<memberFrames.size() )
	{
		VideoFrame* oldFrame = memberFrames.at(index);
		oldFrame->layout.removeWidget( oldFrame->LinkWin );
		oldFrame->LinkWin = NULL;
		oldFrame->isUsed = false;
		oldFrame->_videoWidth = -1;
		oldFrame->_videoHeight = -1;

		frame->LinkWin->hide();

		//updateDisplay( _displayModel );
	}
}

void QtMeetingPlaceWidget::size1And1Real()
{
    if (_displayModel == CF_DM_1X1_REAL) return;
	memberFrames[0]->_showRealResolution = true;		//根据真实的视频分辨率进行显示
	memberFrames[0]->setGeometry( 0,0 ,0, 0);				//防止在1*1布局改成当前布局的时候frame没有调用resizeEvent事件
	memberFrames[0]->setMinimumSize( QSize(memberFrames[0]->_videoWidth,memberFrames[0]->_videoHeight ) );
	int localPreviewIndex = -1;
	for (int i=1;i<memberFrames.size();i++)
	{
		memberFrames[i]->setHidden(true);
	}
	mainLayout->addWidget(memberFrames[0],0,0);

	mainLayout->setSpacing(0);	mainLayout->setMargin(0);	widget->setLayout(mainLayout);
	_ui->scrollArea->setWidget(widget);

	_displayModel = CF_DM_1X1_REAL;
}

void QtMeetingPlaceWidget::size4And4() {

  if (_displayModel == CF_DM_4X4) return;

  _qtConferenceRoom->showMaximizedToScreen();

  for (int i=0; i<memberFrames.size(); ++i) {
    memberFrames[i]->hide();
  }
  for (int i=0; i<16; i++) {
    int r = i / 4;
    int c = i % 4;
    mainLayout->addWidget(memberFrames[i], r, c);
    memberFrames[i]->show();
  }
  
  /*mainLayout->setSpacing(0);	mainLayout->setMargin(0);*/	
  widget->setLayout(mainLayout);
  _ui->scrollArea->setWidget(widget);

  _displayModel = CF_DM_4X4;
}

//
////-------Add By LZY 2010-10-09-------
///*
// * 主动通知遥控器视频布局变更
// * @return void
// */
//void //QtMeetingPlaceWidget::teleChangeLayoutNotify()
//{
//	CMessageFrame Response;
//	switch (QtMeetingPlaceWidget::_displayModel)
//	{
//	case CF_DM_1X1:
//		Response.Append("1");
//		break;
//	case CF_DM_2X2:
//		Response.Append("2");
//		break;
//	case CF_DM_3X3:
//		Response.Append("3");
//		break;
//	case CF_DM_ALL:
//		Response.Append("A");
//		break;
//	case CF_DM_L3X3:
//		Response.Append("L");
//		break;
//	}
//	for (int LoopVar=0;LoopVar<QtMeetingPlaceWidget::memberFrames.size();++LoopVar)
//	{
//		if (QtMeetingPlaceWidget::memberFrames[LoopVar]->isVisible())
//		{
//			if (QtMeetingPlaceWidget::memberFrames[LoopVar]->LinkWin != NULL)
//				Response.Append(QtMeetingPlaceWidget::memberFrames[LoopVar]->LinkWin->getUserId().c_str());//写入视频关联的用户名
//			else
//				Response.Append("\t");//作为该视频窗口没视频显示的标记
//		}
//	}
//
//	//通知telecontroller更新布局
//	CMessageFrame SendMsg;
//	SendMsg.Append("State");
//	SendMsg.Append(CHANGE_LAYOUT);
//	if (Response.getString())
//		SendMsg.Append(Response.getString());
//	
//	//CvTelecontrollerClient::getInstance()->TeleInfo( SendMsg.getString() );
//}
//
//
////-------End of Add By LZY-----------
//
//void QtMeetingPlaceWidget::FullScreenShow( QString userID )
//{
//	for (map<QtConfrenceVideoWindow *,VideoFrame *>::iterator it = video_frame_map.begin();it!=video_frame_map.end();it++)
//	{
//		QtConfrenceVideoWindow *videoWindow = (QtConfrenceVideoWindow *)(it->first);
//		if ( userID.toStdString() == videoWindow->getUserId())
//		{
//			videoWindow->toggleFillScreen();
//		}
//	}
//}
//
//void QtMeetingPlaceWidget::windowResizeSlot( QString userID , int wnd )
//{
//	windowResizeSignal( userID ,wnd);
//}
//
//int QtMeetingPlaceWidget::findUserSeet( QString userID )
//{
//	int seet = -1;
//	for ( int frameIndex = 0 ; frameIndex < memberFrames.size() ; frameIndex++ )
//	{
//
//		if( !memberFrames[frameIndex]->isUsed )
//			continue;
//
//		QtConfrenceVideoWindow *videoWindow = memberFrames[frameIndex]->LinkWin;
//		if ( userID.toStdString() == videoWindow->getUserId())
//		{
//			seet = frameIndex + 1;
//		}
//	}
//
//	return seet;
//}
//
//QtConfrenceVideoWindow* QtMeetingPlaceWidget::assignUserSeet( const int& seet , const QString userID )
//{
//	if( seet > VIDEONUM )
//	{
//		CV_LOG_ERROR("座位号大于座位总数");
//		return NULL;
//	}
//	int vectorIndex = seet - 1;
//
//	
//	QtConfrenceVideoWindow *child = new QtConfrenceVideoWindow(this);
//	child->setUserId( userID.toStdString() );
//
//	//如果没被是用，那么直接拿来用
//	//如果已经被使用了，原来的窗口关闭掉，换成当前用户
//	if( memberFrames[vectorIndex]->isUsed )
//	{
//		std::string preUserID = memberFrames[vectorIndex]->LinkWin->getUserId();
//		_qtConferenceRoom->getConferenceRoomService()->CloseMediaWindow( QString::fromStdString(preUserID) );
//		closeWindow( preUserID );
//	}
//
//	memberFrames[vectorIndex]->layout.addWidget(child);
//	memberFrames[vectorIndex]->LinkWin = child;//Add By LZY 2010-10-09 加入关联的窗口指针
//	memberFrames[vectorIndex]->isUsed = true;
//	video_frame_map[child] = memberFrames[vectorIndex];
//
//	return child;
//}
//
//void QtMeetingPlaceWidget::updateTranStateEventHandler( const TranStatInfo &tranStatInfo )
//{
//	updateTranStateSignal( tranStatInfo );
//}



