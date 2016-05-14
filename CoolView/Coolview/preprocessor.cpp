#include "preprocessor.h"

#include <QThread>
#include <QWidget>
#include <QMessageBox>

#include "ui_pending_widget.h"
#include "util/NetworkUtil.h"
#include "launcher.h"
#include "config/RunningConfig.h"

Preprocessor::Preprocessor(QObject *parent)
	: QObject(parent)
{
	work_thread_ = nullptr;
    pending_widget_ = nullptr;
  ip_valid_ = false;
  video_valid_ = false;
}

Preprocessor::~Preprocessor()
{
	if (work_thread_) {
		work_thread_->deleteLater();
	}
}

void Preprocessor::Initialize(Launcher *launcher)
{
	work_thread_ = new QThread();
	launcher_ = launcher;
	moveToThread(work_thread_); // 通过槽的调用将在子线程中执行
	connect(this, &Preprocessor::StartedSignal,
		this, &Preprocessor::Preprocess);
	connect(this, &Preprocessor::FinishedSignal,
		this, &Preprocessor::Launch);
}

void Preprocessor::Start()
{
	// 在主线程显示GUI
	ShowPendingWidget();
    //RunningConfig要调用UI组件读取屏幕设置，不能在子线程中运行，所以这里调一调，确保其已初始化
    RunningConfig::Instance();
	// 发出信号，在子线程中开始其他工作
	if (work_thread_) {
		work_thread_->start();
		emit StartedSignal();
	}
}

void Preprocessor::Preprocess()
{
	CheckIP();
    CheckVideoDevice();
	//移动回主线程，以便调用Launcher相关方法
	moveToThread(QApplication::instance()->thread());
	work_thread_->quit();
	emit FinishedSignal();
    pending_widget_ = nullptr;//FinishedSignal连接到窗口的销毁槽
}

void Preprocessor::Launch()
{
  //根据与处理结果反馈信息
  //在子线程中不能使用GUI，故在此集中处理
  if (!ip_valid_) {
    QMessageBox::critical(nullptr, 
      QString::fromLocal8Bit("IP错误"),
      QString::fromLocal8Bit("无法获取有效IP地址，请检查网络配置，并重启系统。"));
  }
  if (!video_valid_) {
    QMessageBox::critical(nullptr, 
      QString::fromLocal8Bit("设备错误"),
      QString::fromLocal8Bit("部分视频输入设备无法被侦测到，请检查视频设置，并重启系统。"));
  }

  //启动Laucher
	if (launcher_) {
		launcher_->Initialize();
		launcher_->Start();
	}
}

void Preprocessor::ShowPendingWidget()
{
	if (!work_thread_) {
		return;
	}
	
    pending_widget_ = new QWidget();
	Ui::PendingWidget *pending_widget_ui = new Ui::PendingWidget();
	pending_widget_ui->setupUi(pending_widget_);
	pending_widget_->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
	pending_widget_->show();
	// 当线程停止时自动销毁窗口
	connect(this, &Preprocessor::FinishedSignal,
        pending_widget_, &QObject::deleteLater);
    connect(this, &Preprocessor::ShowMsgSignal,
        pending_widget_ui->msgLabel, &QLabel::setText);
}

/**
 * LoginForm中的CheckIPState()也是检查IP，为何还要在此预处理呢？
 * 因为如果没有有效IP，DBus无法正常工作，DBus启动必须在获得IP之后，
 * 故要等待IP分配后再启动DBus
 */
void Preprocessor::CheckIP()
{
  emit ShowMsgSignal(QString::fromLocal8Bit("正在检查IP地址..."));
	//检查IP地址
  //大致等待max_pending_time/1000秒
  const unsigned int sleep_interval = 100; //ms
  const unsigned int max_pending_time = 15000; //ms
  unsigned int pending_time = 0;

	while (pending_time < max_pending_time) {
		QStringList ip_list = GetHostIPList();
		for (auto ip : ip_list) {
			//TODO:这里只处理了IPv4，未处理IPv6
			if (!ip.isEmpty() && 
				  !ip.startsWith("0.") &&
					!ip.startsWith("169.")) {
						ip_valid_ = true;
						break;
			}
		}
		if (ip_valid_) {
			break;
		}
    pending_time += sleep_interval;
		QThread::msleep(sleep_interval); // 等待100ms重试
	}
}

void Preprocessor::CheckVideoDevice()
{
    emit ShowMsgSignal(QString::fromLocal8Bit("正在检查视频设备..."));
    const unsigned int sleep_interval = 1000; //ms
    const unsigned int max_pending_time = 15000; //ms
    unsigned int pending_time = 0;

    while (pending_time < max_pending_time) {
        RunningConfig::Instance()->loadVideoConfig();//每次检查前读取一次设置
        video_valid_ = RunningConfig::Instance()->IsConfiguredVideoDeviceValid();
        if (video_valid_) {
            break;
        }
        pending_time += sleep_interval;
        QThread::msleep(sleep_interval); // 等待100ms重试
    }
}
