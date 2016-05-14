
#include <QtWidgets\QApplication>
#include <QPainter>
#include <QPaintEvent>
#include "QtScreenWidget.h"
#include "uicommon.h"

QtScreenWidget::QtScreenWidget()
{
	_repaintTimer = new QTimer();

	connect( _repaintTimer , SIGNAL(timeout()) , this , SLOT(repaint()) );

	mediaStateFrame = new QFrame(this);
	mediaStateFrame->setStyleSheet("QFrame{border-image:url(:/CvConferenceRoom/Resources/mediaWaiting.png);border-width:2px;border-color:darkGray;border-style:solid;border-radius: 5px;}");
	mediaStateFrame->setHidden(true);

	QString appDir = QApplication::applicationDirPath();
	appDir.replace("/" , "\\" );
	_screenImagePath = appDir +  "\\Resources\\ScreenInitial.jpg";
	//installEventFilter(this);
}

QtScreenWidget::~QtScreenWidget()
{
	SAFE_DELETE(_repaintTimer );
	SAFE_DELETE(mediaStateFrame);
}


void QtScreenWidget::paintEvent( QPaintEvent * event )
{
	QPixmap pixmap;
	pixmap.load(_screenImagePath);
	QPainter painter(this);
	painter.drawPixmap(event->rect(), pixmap);
	painter.end();
}

void QtScreenWidget::startDisplayScreen()
{
	this->repaint(0 ,0 , this->width() , this->height() );

	QString appDir = QApplication::applicationDirPath();
	appDir.replace("/" , "\\" );
	_screenImagePath = appDir +  "\\ScreenCaptrueRecv.jpg";
	_repaintTimer->start(500);
}

void QtScreenWidget::stopDisplayScreen()
{
	QString appDir = QApplication::applicationDirPath();
	appDir.replace("/" , "\\" );
	_screenImagePath = appDir +  "\\Resources\\ScreenInitial.jpg";
	this->repaint(0 ,0 , this->width() , this->height() );
	_repaintTimer->stop();
}

void QtScreenWidget::setMediaState( const UiMediaState state)
{
	switch( state )
	{
	case UiMediaState_Initializing:
		mediaStateFrame->setHidden(false);
		break;
	case UiMediaState_Ready:
	case UiMediaState_Run:
	case UiMediaState_Destroy:
		mediaStateFrame->setHidden(true);
		break;
	}
}

void QtScreenWidget::resizeEvent( QResizeEvent * event )
{
	int width = event->size().width();
	int height = event->size().height();
	int stateFrameWidth = height/4;
	int left = (width-stateFrameWidth)/2;
	int top = (height-stateFrameWidth)/2;
	mediaStateFrame->setGeometry( left , top , stateFrameWidth , stateFrameWidth );
}
//bool QtScreenWidget::eventFilter(QObject * object, QEvent * event) {
	//if (event->type() == QEvent::KeyPress) {
	//	QKeyEvent * keyEvent = static_cast<QKeyEvent *>(event);

	//	if (keyEvent->key() == Qt::Key_PageDown/*|Qt::Key_PageUp|Qt::Key_Left|Qt::Key_Right*/
	//		||keyEvent->key()==Qt::Key_PageUp ){

	//			return true;
	//	} 

	//}else
	//{
	//	if( event->type()==QEvent::MouseButtonRelease )
	//	{
	//		int i=0;
	//	}
	//}

	//return false;
//}

