#include "video_setting_tab.h"

#include <assert.h>

#include <vector>
#include <string>
using std::vector;
using std::string;

#include <QMessageBox>

#include "config/RunningConfig.h"
#include "util/ini/CVIniConfig.h"
#include "msdx/config.h"
#include "util/MediaManager.h"
#include "CoolviewCommon.h"
#include "DeviceManager/DeviceManager.h"

VideoSettingTab::VideoSettingTab(QWidget *parent)
    : QWidget(parent) {
  ui.setupUi(this);

  //隐藏kinect设置
  ui.ckbKinectDevice->hide();

  //隐藏掉视频码率控制和同步偏差纠错的checkbox，貌似工作有问题
  ui.ckbAdaptiveCodeRate->hide();
  ui.ckbRecvAutoResync->hide();

  //Video
  connect(ui.cameraIndexComboBox , SIGNAL(currentIndexChanged ( const QString & )),
    this, SLOT(cameraIndexChangedSlot(const QString& ) ) );
  connect(ui.webcamDeviceComboBox , SIGNAL(currentIndexChanged ( const QString & )),
    this, SLOT(webcamChangedSlot(const QString& ) ) );
  connect(ui.crossbarlistcomboBox, SIGNAL( currentIndexChanged( const QString& )),
    this , SLOT(crossbarChangedSlot( const QString&)) );

  //Advance
  connect(ui.ckbRecvAutoResync, SIGNAL(stateChanged(int)), SLOT( ckbRecvAutoResync_StateChanged(int)) );
  connect(ui.ckbAdaptiveCodeRate, SIGNAL(stateChanged(int)), SLOT( ckbAdaptiveCodeRate_StateChanged(int)) );
  //Save and close
  connect(ui.saveButton , SIGNAL(clicked()) , this , SLOT(SaveConfig()));
}

VideoSettingTab::~VideoSettingTab() {

}

void VideoSettingTab::ReadConfig() {
  RunningConfig* config = RunningConfig::Instance();

  //video
  //cam
  ui.webcamDeviceComboBox->clear();
  {
    VCapDeviceList webcamList;
    DeviceManager::GetInstatnce()->GetVideoCaptureDevices( &webcamList );
    //TransformStringVectorToQStringList( &webcamList , &devices );
    ui.webcamDeviceComboBox->addItem(QString::fromLocal8Bit("无输入"));
    //ui.webcamDeviceComboBox->addItems( devices );
    for (VCapDeviceList::iterator it = webcamList.begin(); it != webcamList.end(); ++it)
    {
		if (it->friendlyName=="Kinect Camera V2")//因为Kinect找不到devicePath,就用displayname替代。代码里Kinect对应devicepath均为displayname--hjf 2015.12.22
		{
			QVariant data(QString::fromStdString(it->displayName));
			ui.webcamDeviceComboBox->addItem(QString::fromStdString(it->friendlyName), data);
		}
		else
		{
		    QVariant data(QString::fromStdString(it->devicePath));
			ui.webcamDeviceComboBox->addItem(QString::fromStdString(it->friendlyName), data);
		}
    }
  }

  //crossbar
  ui.crossbarlistcomboBox->clear();
  vector<string> crossbarList;
  DeviceManager::GetInstatnce()->GetCrossbarDeviceList("", &crossbarList); //第一个参数是废的

  ui.crossbarlistcomboBox->addItem(QString::fromLocal8Bit("不使用"), QVariant(QString("")));
  ui.crossbarlistcomboBox->addItem(QString::fromLocal8Bit("自动选择"), 
      QVariant(QString::fromLocal8Bit(MSDX_CONF_CROSSBAR_AUTO)));
  for (int i = 0; i < crossbarList.size(); i+=2) {
      //显示friendlyName
      QString item_text = QString::fromLocal8Bit(crossbarList[i].c_str());
      //内部保留displayName
      QString item_data = QString::fromLocal8Bit(crossbarList[i+1].c_str());
      ui.crossbarlistcomboBox->addItem(item_text, QVariant(item_data));
  }
  //TransformStringVectorToQStringList( &crossbarList , &itemList ); //旧的只显示friendlyName
  //ui.crossbarlistcomboBox->addItems( itemList );

  //加载完cam和crossbar再设置这个哦，不然无法显示成之前设置的值
  QStringList videoCapIndex;
  for (int i = 1; i <= config->VideoCaptureDeviceCount(); ++i)
  {
    videoCapIndex << QString::number(i);
  }
  ui.cameraIndexComboBox->clear();
  ui.cameraIndexComboBox->addItems(videoCapIndex);
  ui.cameraIndexComboBox->setCurrentIndex(0);

  ui.ckbRecvAutoResync->setChecked(config->isEnableRecvAutoResync());
  ui.ckbAdaptiveCodeRate->setChecked(config->isEnableAdaptiveCodeRate());

  ui.ckbKinectDevice->setChecked( config->isEnableKinect() );

  //通过读取配置文件设置图像质量，这是临时解决方案     add by lzb
  QString appDir = QApplication::applicationDirPath();
  appDir.replace("/" , "\\" );
  QString filePath = appDir +  "\\CvX264.ini";
  QSettings *ConfigIni = new QSettings(filePath,QSettings::IniFormat,0);   
  ui.videoQualitySlider->setSliderPosition( ConfigIni->value("/RC/crf","29").toInt());
  delete ConfigIni;
}

void VideoSettingTab::SaveConfig() {
  RunningConfig* config = RunningConfig::Instance();

  VCapSetting vcap;

  const int vIndex = ui.cameraIndexComboBox->currentIndex();
  if (vIndex < 0)
  {
    QMessageBox::information(
      this, QString::fromLocal8Bit("提示") , 
      QString::fromLocal8Bit("无效的摄像头编号" ), 
      QMessageBox::Ok 
      );
    return;
  }
  vcap.index = vIndex;

  if (0 < ui.webcamDeviceComboBox->currentIndex()) // 0为无设备
  {
    vcap.friendlyName = ui.webcamDeviceComboBox->currentText();
    vcap.devicePath = ui.webcamDeviceComboBox->itemData(ui.webcamDeviceComboBox->currentIndex()).toString();
  }

  //crossbar
  {
      int index = ui.crossbarlistcomboBox->currentIndex();
      if (index > 0) {
          vcap.crossBarName = ui.crossbarlistcomboBox->currentText();
          vcap.crossBarDisplayName = ui.crossbarlistcomboBox->itemData(index).toString();
          vcap.crossBarType = ui.crossbarclasscomboBox->currentText();
      } else {
          //0为不使用，-1为未选择
          vcap.crossBarName = "";
          vcap.crossBarDisplayName = "";
          vcap.crossBarType = "";
      }
  }

  QStringList sizes = ui.videosizecomboBox->currentText().split("X");
  if( sizes.size()>=2 )
  {
    vcap.width = sizes.at(0).toUInt();
    vcap.height = sizes.at(1).toUInt();
    vcap.fps = 30;
    if( sizes.size()==3 )
      vcap.fps = sizes.at(2).toUInt();
  }
  config->VideoCaptureDevice(vIndex, vcap);

  config->EnableKinect( ui.ckbKinectDevice->isChecked() );

  config->saveConfig();

  QMessageBox::information(
    this, QString::fromLocal8Bit("提示") , 
    QString::fromLocal8Bit("保存成功！配置将在下次进入会议时生效。" ), 
    QMessageBox::Ok 
    );

  if( ui.ckbKinectDevice->isChecked() )
  {
    //如果支持kinect，那么启动kinect进程
    startKinectDaemon();
  }else
  {
    //否则关闭kinect进程
    stopKinectDaemon();
  }

  //通过修改配置文件设置图像质量，这是临时解决方案     add by lzb
  QString appDir = QApplication::applicationDirPath();
  appDir.replace("/" , "\\" );
  QString filePath = appDir +  "\\cfg\\CvX264.ini";
  QSettings *ConfigIni = new QSettings(filePath,QSettings::IniFormat,0);   
  ConfigIni->setValue("/RC/crf",(ui.videoQualitySlider->value()));  
  delete ConfigIni;
}

void VideoSettingTab::cameraIndexChangedSlot( const QString& indexStr ) {
  const int vIndex = ui.cameraIndexComboBox->currentIndex();
  assert(vIndex == indexStr.toInt() - 1); // 注意可视编号从1开始

  RunningConfig* config = RunningConfig::Instance();

  if (vIndex >= config->VideoCaptureDeviceCount())
  {
    ui.webcamDeviceComboBox->setCurrentIndex( 0 );
  }

  VCapSetting setting = config->VideoCaptureDevice(vIndex);
  // 读取保存的设置。当设备名过长时会被截断，从而有可能产生尾空格。
  // 由于INI读取API会忽略尾空格，故用Qt::MatchStartsWith只匹配开头
  // 除非两个设备名被截断后的部分完全一样，否则不会产生问题
  int index = ui.webcamDeviceComboBox->findText( setting.friendlyName, Qt::MatchStartsWith );
  ui.webcamDeviceComboBox->setCurrentIndex( index>0 ? index : 0 );

  index = ui.crossbarlistcomboBox->findText( setting.crossBarName, Qt::MatchStartsWith );
  ui.crossbarlistcomboBox->setCurrentIndex( index>0 ? index: 0 );

  index = ui.crossbarclasscomboBox->findText( setting.crossBarType, Qt::MatchStartsWith );
  ui.crossbarclasscomboBox->setCurrentIndex( index>0?index:0);

  QString videoSize = QString::number( setting.width) + "X" + QString::number(setting.height) + "X"+ QString::number(setting.fps);
  index = ui.videosizecomboBox->findText( videoSize );
  ui.videosizecomboBox->setCurrentIndex( index>0?index:0);
}

void VideoSettingTab::webcamChangedSlot( const QString& webcamName ) {
  //多路重复性检查 - Liaokz
  if (ui.webcamDeviceComboBox->currentIndex() > 0)
  {
    const int vIndex = ui.cameraIndexComboBox->currentIndex();
    const QString devicePath = ui.webcamDeviceComboBox->itemData(ui.webcamDeviceComboBox->currentIndex()).toString();
    RunningConfig *config = RunningConfig::Instance();

    int i = 0;
    for (; i < config->VideoCaptureDeviceCount(); ++i)
    {
      VCapSetting setting = config->VideoCaptureDevice(i);

      if (setting.index != vIndex && 
        setting.devicePath == devicePath)
      {
        QString msg = QString::fromLocal8Bit("当前视频设置与第%1路视频设置冲突，是否关闭第%1路视频？").arg(setting.index);
        if (QMessageBox::Yes == QMessageBox::question(this, QString::fromLocal8Bit("设置冲突"), msg))
        {
          VCapSetting vcap;
          vcap.index = setting.index;
          config->VideoCaptureDevice(vcap.index, vcap); //将冲突的视频通道设置为无设备,但暂不保存,直到需要时再保存
        }
        else
        {
          ui.webcamDeviceComboBox->setCurrentIndex(0);
          return;
        }
      }
    }
  }

  //设备属性
  ui.videosizecomboBox->clear();
  vector<string> videoSizeList;
  DeviceManager::GetInstatnce()->GetVideoDeviceMediaType( webcamName.toLocal8Bit().constData() , &videoSizeList );
  QStringList itemList;
  TransformStringVectorToQStringList( &videoSizeList , &itemList );
  ui.videosizecomboBox->addItems( itemList );

  if( webcamName.contains("webcam", Qt::CaseInsensitive) 
    || webcamName.contains("e2eSoft", Qt::CaseInsensitive ) )
  {
    //如果是网络摄像头则清空采集卡配置
    ui.crossbarlistcomboBox->setCurrentIndex(0); //设置为不使用
    ui.crossbarlistcomboBox->setEnabled(false);
  } else {
    ui.crossbarlistcomboBox->setEnabled(true);
  }
}

void VideoSettingTab::crossbarChangedSlot( const QString& crossbar ) {
  ui.crossbarclasscomboBox->clear();
  int index = ui.crossbarlistcomboBox->currentIndex();
  if (index <= 1) {
      //0为不使用，1为自动
      return;
  }
  vector<string> crossbarType;
  QString crossbar_displayname = ui.crossbarlistcomboBox->itemData(index).toString();
  DeviceManager::GetInstatnce()->GetCrossbarInputType( crossbar_displayname.toLocal8Bit().constData() , &crossbarType );

  QStringList itemList;
  TransformStringVectorToQStringList( &crossbarType , &itemList );
  ui.crossbarclasscomboBox->addItems( itemList);
}

void VideoSettingTab::ckbRecvAutoResync_StateChanged( int state ) {
  bool enable =( state == Qt::CheckState::Checked);
  if(RunningConfig::Instance()->isEnableRecvAutoResync() !=enable){
    RunningConfig::Instance()->EnableRecvAutoResync(enable);
    MediaManager::getInstance()->EnableRecvAutoResync(enable);
  }
}

void VideoSettingTab::ckbAdaptiveCodeRate_StateChanged( int state ) {
  bool enable =( state == Qt::CheckState::Checked);
  RunningConfig::Instance()->EnableAdaptiveCodeRate(enable);
  MediaManager::getInstance()->EnableAdaptiveCodeRate(enable);
}

void VideoSettingTab::ChangeCameraIndex( const int index ) {
  ui.cameraIndexComboBox->setCurrentIndex(index);
}
