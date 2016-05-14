#include "uicommon.h"
#include "qtconferenceroom.h"
#include "QtMeetingPlaceWidget.h"
#include <QMessageBox>
#include <QtWidgets\QApplication>
#include <QtWidgets\QDesktopWidget>
#include "ConferenceRoomService.h"
#include "QtScreenShare.h"
#include <Windows.h>
#include "util/ini/CVIniConfig.h"

QtConferenceRoom::QtConferenceRoom(int screenIndex , QWidget *parent)
	: QWidget(parent), screen_index_(screenIndex)
{
	ui.setupUi(this);

	QIcon icon(":/CvConferenceRoom/Resources/application.png");
	this->setWindowIcon( icon);

	//设置界面
  QRect rect ;

  if (CVIniConfig::getInstance().IsModelHD())
  {  
    //检查一下屏幕的分辨率是否大于1280*720 防止在读取屏幕参数时，系统还未正确读取屏幕的参数。 add by lzb
    //仅对CoolView-HD做这个处理，TX中使用的屏幕数与实际屏幕无关，不要做这个处理，不然不能设定初始布局.  add by Liaokz
    int tryCount=0;
    while(tryCount<3)
    {
      rect = QApplication::desktop()->availableGeometry(screenIndex);
      int width = rect.width();
      int height = rect.height();
      if(width<1280 || height<720)
      {
        Sleep(2000);
        tryCount++;
      }
      else
      {
        break;
      }		
    }
  }

	//Main window size and position saved
	setGeometry( rect);

    setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnBottomHint | Qt::Tool); 
	setAttribute(Qt::WA_DeleteOnClose);

	QGridLayout* mainLayout = new QGridLayout();
	_scrollArea = new QScrollArea();
	mainLayout->addWidget(_scrollArea);
	mainLayout->setMargin(0);

	ui.centralwidget->setLayout(mainLayout);

	//会议视频框
	_meetingPlaceWidget = new QtMeetingPlaceWidget(this);
	_scrollArea->setWidget(_meetingPlaceWidget);
	_scrollArea->setWidgetResizable(true);

	//MediaWindowManager::getInstance()->SetMeetingPlace( _meetingPlaceWidget , screenIndex );

	//共享窗口
	_screenShareWidget = new QtScreenShare(this );
	_screenShareWidget->hide();
}

void QtConferenceRoom::showMaximizedToScreen() {
  QRect rect = QApplication::desktop()->availableGeometry(screen_index_);
  if (ScreenHelper::Instance()->IsAvailableScreen(screen_index_))
  {
    rect = ScreenHelper::Instance()->GetAvailableGeometry(screen_index_);
  } else {
    rect = QApplication::desktop()->availableGeometry(screen_index_);
  }
  setGeometry(rect);
  //showMaximized();
}

void QtConferenceRoom::showTopHalfToScreen() {
  QRect rect = QApplication::desktop()->availableGeometry(screen_index_);
  setGeometry(rect.x(), rect.y(), rect.width(), rect.height() / 2);
  showNormal();
}

QtConferenceRoom::~QtConferenceRoom()
{
	SAFE_DELETE( _meetingPlaceWidget );
	SAFE_DELETE( _screenShareWidget );
	SAFE_DELETE( _scrollArea );
}

//void QtConferenceRoom::closeEvent( QCloseEvent * event )
//{
//	int result = QMessageBox::information(
//		this ,
//		QString::fromLocal8Bit("提示"),
//		QString::fromLocal8Bit("不能关闭会议室窗口"),
//		QMessageBox::Ok /*| QMessageBox::Cancel*/ 
//		);
//    event->ignore();
//}

void QtConferenceRoom::showScreenShare( )
{
  if (_meetingPlaceWidget)
	  _meetingPlaceWidget->hide();
	if (_screenShareWidget->isHidden())
    _scrollArea->takeWidget();

	_scrollArea->setWidget(_screenShareWidget);
	_screenShareWidget->show();
}

void QtConferenceRoom::showMeetringPlace()
{
	if( _screenShareWidget )
		_screenShareWidget->hide();
  if (_meetingPlaceWidget->isHidden())
	  _scrollArea->takeWidget();

	_scrollArea->setWidget(_meetingPlaceWidget);
	_meetingPlaceWidget->show();
}

