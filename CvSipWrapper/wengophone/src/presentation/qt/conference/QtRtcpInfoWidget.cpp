#include "stdafx.h"
#include ".\qtrtcpinfowidget.h"
#include <util/SafeDelete.h>
#include <util/Logger.h>
#include <qtutil/WidgetBackgroundImage.h>
#include <qtutil/SafeConnect.h>
#include "QtWengoPhone.h"
#include <control/profile/CUserProfile.h>
#include <control/profile/CUserProfileHandler.h>
#include <control/CWengoPhone.h>
#include <WinSock.h>
#include <QtGui/QtGui>

#include <dbus/conferenceRoom/ConfRoomClient.h>

QtRtcpInfoWidget::QtRtcpInfoWidget(QtWengoPhone& qtWengoPhone) : _qtWengoPhone(qtWengoPhone), _cWengoPhone(_qtWengoPhone.getCWengoPhone())
{
    _ui = new Ui::RtcpInfoWidget;
    _ui->setupUi(this);

	clearStatText();
	tranStat_init(&_latestTranStat);

	//使用slot机制
	SAFE_CONNECT_TYPE(this, SIGNAL(displayTranStatSinal(const TranStat * )), SLOT(displayTranStatSlot(const TranStat * )),
		Qt::QueuedConnection);

    //注册传输统计事件
    _cWengoPhone.getCUserProfileHandler().getCUserProfile()->tranStatRecvEvent += boost::bind(&QtRtcpInfoWidget::tranStatRecvEventHandler, this, _1);

#ifdef DBUS_ENABLE
	connect( _ui->showRtcpMsgButton , SIGNAL(clicked(bool)) , this , SLOT(showRtcpMessage(bool)));
#endif
}

QtRtcpInfoWidget::~QtRtcpInfoWidget(void)
{
    //取消事件绑定
    _cWengoPhone.getCUserProfileHandler().getCUserProfile()->tranStatRecvEvent -= boost::bind(&QtRtcpInfoWidget::tranStatRecvEventHandler, this, _1);
    OWSAFE_DELETE(_ui);
}

void QtRtcpInfoWidget::tranStatRecvEventHandler(const TranStat& transtat)
{
	_latestTranStat = transtat;
    //引发signal，通知界面修改数据
	 displayTranStatSinal(&_latestTranStat);
}


void QtRtcpInfoWidget::displayTranStatSlot(const TranStat * pTranstat )
{
    displayTranStat(*pTranstat);
}

void QtRtcpInfoWidget::clearStatText()
{
    _ui->totalBandwidthLabel->setText("");
    _ui->totalLostLabel->setText("");
    _ui->totalConnectionCountLabel->setText("");
    _ui->totalSendBandwidthLabel->setText("");
    _ui->totalSendLostLabel->setText("");
    _ui->totalSendConnectionCountLabel->setText("");
    _ui->totalReceiveBandwidthLabel->setText("");
    _ui->totalReceiveLostLabel->setText("");
    _ui->totalReceiveConnectionCountLabel->setText("");
    _ui->videoSendBandwidthLabel->setText("");
    _ui->videoSendLostLabel->setText("");
    _ui->videoSendConnectionCountLabel->setText("");
    _ui->videoReceiveBandwidthLabel->setText("");
    _ui->videoReceiveLostLabel->setText("");
    _ui->videoReceiveConnectionCountLabel->setText("");
    _ui->audioSendBandwidthLabel->setText("");
    _ui->audioSendLostLabel->setText("");
    _ui->audioSendConnectionCountLabel->setText("");
    _ui->audioReceiveBandwidthLabel->setText("");
    _ui->audioReceiveLostLabel->setText("");
    _ui->audioReceiveConnectionCountLabel->setText("");
}


void QtRtcpInfoWidget::displayTranStat(const TranStat& transtat)
{
    _ui->totalBandwidthLabel->setText(QString::number(transtat.totalStat.bandwidth));
    _ui->totalLostLabel->setText(QString::number(transtat.totalStat.lost));
    _ui->totalConnectionCountLabel->setText(QString::number(transtat.totalStat.connectionCount));
    _ui->totalSendBandwidthLabel->setText(QString::number(transtat.totalSendStat.bandwidth));
    _ui->totalSendLostLabel->setText(QString::number(transtat.totalSendStat.lost));
    _ui->totalSendConnectionCountLabel->setText(QString::number(transtat.totalSendStat.connectionCount));
    _ui->totalReceiveBandwidthLabel->setText(QString::number(transtat.totalReceiveStat.bandwidth));
    _ui->totalReceiveLostLabel->setText(QString::number(transtat.totalReceiveStat.lost));
    _ui->totalReceiveConnectionCountLabel->setText(QString::number(transtat.totalReceiveStat.connectionCount));
    _ui->videoSendBandwidthLabel->setText(QString::number(transtat.videoSendStat.bandwidth));
    _ui->videoSendLostLabel->setText(QString::number(transtat.videoSendStat.lost));
    _ui->videoSendConnectionCountLabel->setText(QString::number(transtat.videoSendStat.connectionCount));
    _ui->videoReceiveBandwidthLabel->setText(QString::number(transtat.videoReceiveStat.bandwidth));
    _ui->videoReceiveLostLabel->setText(QString::number(transtat.videoReceiveStat.lost));
    _ui->videoReceiveConnectionCountLabel->setText(QString::number(transtat.videoReceiveStat.connectionCount));
    _ui->audioSendBandwidthLabel->setText(QString::number(transtat.audioSendStat.bandwidth));
    _ui->audioSendLostLabel->setText(QString::number(transtat.audioSendStat.lost));
    _ui->audioSendConnectionCountLabel->setText(QString::number(transtat.audioSendStat.connectionCount));
    _ui->audioReceiveBandwidthLabel->setText(QString::number(transtat.audioReceiveStat.bandwidth));
    _ui->audioReceiveLostLabel->setText(QString::number(transtat.audioReceiveStat.lost));
    _ui->audioReceiveConnectionCountLabel->setText(QString::number(transtat.audioReceiveStat.connectionCount));
}

void QtRtcpInfoWidget::showRtcpMessage( bool show )
{
	ConfRoomDBusClient::getInstance()->ShowRtcpMessage( show );
}