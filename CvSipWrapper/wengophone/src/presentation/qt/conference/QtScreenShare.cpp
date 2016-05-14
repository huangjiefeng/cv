#include "stdafx.h"
#include ".\qtscreenshare.h"
#include "cwebpage.h"
#include <util/SafeDelete.h>
#include <qtutil/SafeConnect.h>
#include "QtWengoPhone.h"
#include <control/profile/CUserProfile.h>
#include <control/profile/CUserProfileHandler.h>
#include <control/CWengoPhone.h>
#include "QtConferenceInvite.h"

QtScreenShare::QtScreenShare(QtWengoPhone & qtWengoPhone,QtConferenceRoomWidget *confRoom):_qtWengoPhone(qtWengoPhone),_cWengoPhone(_qtWengoPhone.getCWengoPhone()),_confRoom(confRoom)
{
	_ui = new Ui::ScreenShare;
	_ui->setupUi(this);
	installEventFilter(this);
	connect(_ui->openFileButton, SIGNAL(clicked()), this, SLOT(openFile()));
	connect(_ui->shareButton, SIGNAL(clicked()), this, SLOT(sendScreen()));
	connect(_ui->recvScreenShareButton, SIGNAL(clicked()), this, SLOT(recvScreen()));
	widget = new QWidget;
	//_ui->gridLayout_2->addWidget(widget);
	_ui->scrollArea->setWidget(widget);
	firstOpenFile = false;
	isSendScreen = false;
	isRecvScreen = false;

	CUserProfile * cUserProfile = _cWengoPhone.getCUserProfileHandler().getCUserProfile();
	cUserProfile->cvHostUnShareScreenEvent +=boost::bind(&QtScreenShare::cvHostUnShareScreenEventHandler,this);
	SAFE_CONNECT_TYPE(this, SIGNAL(cvHostUnShareScreenSinal()),SLOT(cvHostUnShareScreenSlot()), Qt::QueuedConnection);
}

QtScreenShare::~QtScreenShare(void)
{
//	stopScreenGraph();
	closeFile();
	delete widget;
	delete _ui;
	CUserProfile * cUserProfile = _cWengoPhone.getCUserProfileHandler().getCUserProfile();
	cUserProfile->cvHostUnShareScreenEvent -=boost::bind(&QtScreenShare::cvHostUnShareScreenEventHandler,this);
}

void QtScreenShare::openFile()
{
	if (!firstOpenFile)
	{
		QFileDialog::Options options;
		QString selectedFilter;
		QString fileName = QFileDialog::getOpenFileName(this,
			tr(""),
			tr(""),
			tr("Files (*.txt *.doc *ppt *pdf)"),
			&selectedFilter,
			options);
		if (fileName.isEmpty())
		{
			return;
		}
		//if(!firstOpenFile)
		//{
		EmbedBrowserObject(widget->winId());
		//	firstOpenFile = true;
		//}
		DisplayHTMLPage(widget->winId(), fileName.toLocal8Bit().data());
		_ui->openFileButton->setText(QApplication::translate("ScreenShare", "\345\205\263\351\227\255\346\226\207\346\241\243", 0, QApplication::UnicodeUTF8));
		firstOpenFile = true;
	}
	else
	{
		closeFile();
		_ui->openFileButton->setText(QApplication::translate("ScreenShare", "\346\211\223\345\274\200\346\226\207\346\241\243", 0, QApplication::UnicodeUTF8));
		firstOpenFile = false;
	}
}

void QtScreenShare::closeFile()
{
	if(firstOpenFile)
		UnEmbedBrowserObject(widget->winId());
}

void QtScreenShare::sendScreen()
{
	//�Ѿ��ڷ�������
	CUserProfile * cUserProfile = _cWengoPhone.getCUserProfileHandler().getCUserProfile();
	//ֻ�������˲��ܷ������湲��
	if (!cUserProfile->hostOrNot())
	{
		QMessageBox::warning(this, tr("@product@"),
			tr("\345\217\252\346\234\211\344\270\273\346\214\201\344\272\272\346\211\215\350\203\275\345\205\261\344\272\253\346\241\214\351\235\242!"), QMessageBox::Ok);
		return;
	}
	if(isSendScreen)
	{
		_ui->shareButton->setText(QApplication::translate("ScreenShare", "\345\205\261\344\272\253\346\226\207\346\241\243", 0, QApplication::UnicodeUTF8));
		cUserProfile->screen_share_send_stop("127.0.0.1",12345);
		isSendScreen = false;
	}
	else
	{
		QPoint point = widget->mapToGlobal(QPoint(0,0));
		
		int result;
		int x = point.x();
		int y = point.y();
		int width = widget->width();
		width = width-width%4;
		int height = widget->height();
		height = height-height%4;
		//msdx_config_set_video_device("USB ��Ƶ�豸");
		//result=msdx_create_send_graph();
		//result=msdx_set_send_screen_codec("Snap_Screen");
		//result=msdx_connect_send_graph();
		//msdx_add_send_address2("202.38.254.176",0,"202.38.254.176",0,"202.38.254.199",6400);
		//result=msdx_run_send_graph();
		//msdx_set_screen_filter_config(x,y,x+832,y+516);
		result = cUserProfile->screen_share_send_start("127.0.0.1",12345,x,y,x+788,y+448);
		if(result==-1)
		{
			QMessageBox::warning(this, tr("@product@"),
				tr("\346\234\252\350\277\233\345\205\245\344\274\232\350\256\256!"), QMessageBox::Ok);
			return;
		}
		_ui->shareButton->setText(QApplication::translate("ScreenShare", "\345\201\234\346\255\242\345\205\261\344\272\253", 0, QApplication::UnicodeUTF8));
		isSendScreen = true;
	}
}

void QtScreenShare::recvScreen()
{
	CUserProfile * cUserProfile = _cWengoPhone.getCUserProfileHandler().getCUserProfile();
	//�����˲��ܽ������湲��
	if (cUserProfile->hostOrNot())
	{
		QMessageBox::warning(this, tr("@product@"),
			tr("\344\270\273\346\214\201\344\272\272\345\217\252\350\203\275\345\205\261\344\272\253\344\270\215\350\203\275\346\216\245\346\224\266!"), QMessageBox::Ok);
		return;
	}
	//�Ѿ�����������湲��
	if (isRecvScreen)
	{
		cUserProfile->screen_share_recv_stop();
		_ui->recvScreenShareButton->setText(QApplication::translate("ScreenShare", "\346\216\245\346\224\266\345\205\261\344\272\253\346\226\207\346\241\243", 0, QApplication::UnicodeUTF8));
//		msdx_stop_recv_graph(graphid);
//		msdx_destroy_recv_graph(graphid);
		isRecvScreen = false;
	}
	else
	{
		int result = cUserProfile->screen_share_recv_start("127.0.0.1",12345,widget->winId(),788,448);
		if(result==-1)
		{
			QMessageBox::warning(this, tr("@product@"),
				tr("\346\234\252\350\277\233\345\205\245\344\274\232\350\256\256!"), QMessageBox::Ok);
			return;
		}
		_ui->recvScreenShareButton->setText(QApplication::translate("ScreenShare", "\345\201\234\346\255\242\346\216\245\346\224\266\345\205\261\344\272\253\346\226\207\346\241\243", 0, QApplication::UnicodeUTF8));
		//int result;
		//graphid=msdx_create_recv_graph();
		//result=msdx_set_recv_screen_codec(graphid,"JPEG",832,516,2,24);
		////result=msdx_set_recv_video_codec(graphid,"XVID",352,288,30,16);
		//msdx_set_recv_screen_address(graphid,"202.38.254.199",6400);
		////result=msdx_set_recv_address(graphid,"202.38.254.199",10600,"202.38.254.199",10702);
		//msdx_set_recv_display_window(graphid,widget->winId(),0,0,widget->width(),widget->height(),0);
		//result=msdx_connect_recv_graph(graphid);
		//result=msdx_run_recv_graph(graphid);
		isRecvScreen = true;
	}
}

void QtScreenShare::stopScreenGraph()
{
	CUserProfile * cUserProfile = _cWengoPhone.getCUserProfileHandler().getCUserProfile();
	if(isSendScreen)
	{
		_ui->shareButton->setText(QApplication::translate("ScreenShare", "\345\205\261\344\272\253\345\261\217\345\271\225", 0, QApplication::UnicodeUTF8));
	//	cUserProfile->screen_share_send_stop("127.0.0.1",12345);
		isSendScreen = false;
	}
	if (isRecvScreen)
	{
	//	cUserProfile->screen_share_recv_stop();
		_ui->recvScreenShareButton->setText(QApplication::translate("ScreenShare", "\346\216\245\346\224\266\345\261\217\345\271\225\345\205\261\344\272\253", 0, QApplication::UnicodeUTF8));
		isRecvScreen = false;
	}
}

bool QtScreenShare::eventFilter(QObject * object, QEvent * event) {
	if (event->type() == QEvent::KeyPress) {
		QKeyEvent * keyEvent = static_cast<QKeyEvent *>(event);

		if (keyEvent->key() == Qt::Key_PageDown/*|Qt::Key_PageUp|Qt::Key_Left|Qt::Key_Right*/){
			return true;
		} 

	}

	return false;
}

void QtScreenShare::resizeEvent ( QResizeEvent * event )
{
	_cWengoPhone.getCUserProfileHandler().getCUserProfile()->resizeVideoWindow("screen_share",widget->winId(),0);
}

void QtScreenShare::cvHostUnShareScreenEventHandler()
{
	cvHostUnShareScreenSinal();
}

void QtScreenShare::cvHostUnShareScreenSlot()
{

	if (isRecvScreen)
	{
		CUserProfile * cUserProfile = _cWengoPhone.getCUserProfileHandler().getCUserProfile();
		QMessageBox::warning(this, tr("@product@"),
			tr("\344\270\273\346\214\201\344\272\272\346\262\241\346\234\211\345\205\261\344\272\253!"), QMessageBox::Ok);
		cUserProfile->screen_share_recv_stop();
		_ui->recvScreenShareButton->setText(QApplication::translate("ScreenShare", "\346\216\245\346\224\266\345\261\217\345\271\225\345\205\261\344\272\253", 0, QApplication::UnicodeUTF8));
		isRecvScreen = false;
	}
}
