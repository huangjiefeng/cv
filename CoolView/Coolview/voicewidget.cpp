#include "voicewidget.h"
#include "CoolviewCommon.h"
#include "config/RunningConfig.h"
#include <DeviceManager/DeviceManager.h>
#include <vector>
#include <string>
using namespace std;
VoiceWidget::VoiceWidget()
{
    setupUi(this);

  //  this->setWindowFlags(Qt::FramelessWindowHint);
  /*  this->setStyleSheet(
                               "border-width:2px;"
                               "border-color:darkGray;"
                               "border-style:solid;"
                               "border-radius: 5px;"
                               );*/
    connect(this->micSlider,SIGNAL(valueChanged(int)),this,SLOT(micChangeSlot()));
    //connect(this->soundSlider,SIGNAL(sliderReleased()),this,SLOT(soundChangeSlot()));
	connect(this->soundSlider,SIGNAL(valueChanged(int)),this,SLOT(soundChangeSlot()));
}

VoiceWidget::~VoiceWidget()
{
    qDebug()<<"delete voicewidget";
}

void VoiceWidget::micChangeSlot()
{
    int volume = this->micSlider->value();
    emit this->changeVoiceSignal(QString("in"),volume);
}

void VoiceWidget::soundChangeSlot()
{
    int volume = this->soundSlider->value();
    emit this->changeVoiceSignal(QString("out"),volume);
}

void VoiceWidget::loadSystemVolumn()
{
	vector<string> deviceList;
	QStringList devices;

	//音频输出
	int inputVolumnIndex = DeviceManager::GetInstatnce()->GetAudioOutputVolumn();
	if( inputVolumnIndex <0 ) inputVolumnIndex=0;
	if( inputVolumnIndex >=100 ) inputVolumnIndex=99;
	this->soundSlider->setSliderPosition( inputVolumnIndex );


	//音频输入
	DeviceManager::GetInstatnce()->GetAudioCaptureDevices(&deviceList );
	TransformStringVectorToQStringList( &deviceList , &devices );
	if( devices.size()>0 )
	{
		int volumnIndex = DeviceManager::GetInstatnce()->GetAudioInputVolumn( RunningConfig::Instance()->AudioCaptureDevice().toStdString() );
		if( 0<volumnIndex && volumnIndex <= 100 )
		{
			//调整音量条
			this->micSlider->setSliderPosition( volumnIndex );
		}
	} else {
		this->micSlider->setSliderPosition(0);
	}

}