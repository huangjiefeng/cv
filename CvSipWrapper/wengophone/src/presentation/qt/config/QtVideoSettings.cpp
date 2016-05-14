#include "stdafx.h"
/*
 * WengoPhone, a voice over Internet phone
 * Copyright (C) 2004-2007  Wengo
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "QtVideoSettings.h"

#include "ui_VideoSettings.h"

#include <model/config/ConfigManager.h>
#include <model/config/Config.h>
#include <model/profile/UserProfile.h>
#include <model/phoneline/IPhoneLine.h>

#include <control/CWengoPhone.h>
#include <control/profile/CUserProfile.h>
#include <control/profile/CUserProfileHandler.h>

#include <presentation/qt/QtWengoPhone.h>
#include <presentation/qt/phonecall/QtVideoQt.h>

#include <webcam/WebcamDriver.h>
#include <sipwrapper/EnumVideoQuality.h>

#include <util/Logger.h>
#include <util/SafeDelete.h>

#include <qtutil/StringListConvert.h>
#include <qtutil/SafeConnect.h>

#include <QtGui/QtGui>

static const int VIDEO_QUALITY_COLUMN = 0;
static const char * VIDEO_TEST_CALL = "335";

QtVideoSettings::QtVideoSettings(CWengoPhone & cWengoPhone, QWidget * parent)
	: QWidget(NULL),
	_cWengoPhone(cWengoPhone) {

	_ui = new Ui::VideoSettings();
	_ui->setupUi(this);

	_webcamDriver = WebcamDriver::getInstance();
	_previewStarted = false;

	SAFE_CONNECT_TYPE(this, SIGNAL(newWebcamImage()), SLOT(newWebcamImageCaptured()), Qt::QueuedConnection);
	//SAFE_CONNECT(_ui->webcamDeviceComboBox, SIGNAL(activated(const QString &)), SLOT(startWebcamPreview(const QString &)));
	SAFE_CONNECT(_ui->makeTestVideoCallButton, SIGNAL(clicked()), SLOT(makeTestCallClicked()));
	SAFE_CONNECT(_ui->webcamPreviewButton, SIGNAL(clicked()), SLOT(webcamPreview()));
	SAFE_CONNECT(_ui->saveButton, SIGNAL(clicked()), SLOT(saveButtonClicked()));

	//根据选择的crossbar修改crossbar输入类型列表  add by zhenHua.sun
	SAFE_CONNECT(_ui->crossbarlistcomboBox , SIGNAL(currentIndexChanged(const QString&)) , SLOT( crossbarChanged( const QString&  )) );

	//根据选择的capturedevice来修改支持的视频大小zhenHua.sun 2010-10-20
	SAFE_CONNECT(_ui->webcamDeviceComboBox ,SIGNAL(currentIndexChanged(const QString&)) , SLOT( webCamChanged( const QString&  )) );
	
	Config & config = ConfigManager::getInstance().getCurrentConfig();
	//No webcam driver
	if (_webcamDriver->getDeviceList().empty()) {
	//	config.set(Config::VIDEO_ENABLE_KEY, false);
		_ui->enableVideoCheckBox->setChecked(false);
		_ui->enableVideoCheckBox->setEnabled(false);
	}

	//// set all columns to the same size
	//_ui->videoQualityTreeWidget->header()->setResizeMode(2, QHeaderView::Stretch);
	//_ui->videoQualityTreeWidget->header()->setResizeMode(1, QHeaderView::Stretch);
	//_ui->videoQualityTreeWidget->header()->setResizeMode(0, QHeaderView::Stretch);
	////

	readConfig();
	//_ui->groupBox_3->setVisible(false);
	_ui->makeTestVideoCallButton->setVisible(false);
	_ui->enableVideoCheckBox->setVisible(false);

	//隐藏取消按钮
	_ui->cancelButton->hide();
}

QtVideoSettings::~QtVideoSettings() {
	OWSAFE_DELETE(_ui);
}

QString QtVideoSettings::getName() const {
	return tr("Video");
}

QString QtVideoSettings::getTitle() const {
	return tr("Video Settings");
}

QString QtVideoSettings::getIconName() const {
	return "video";
}

void QtVideoSettings::saveConfig() {
	Config & config = ConfigManager::getInstance().getCurrentConfig();

	//支持crossbar和crossbar输入类型的选择..........................start add by zhenHua.sun 2010-07-16
	string crossbarName = _ui->crossbarlistcomboBox->currentText().toLocal8Bit() ;
	string crossbarInputType = _ui->crossbarclasscomboBox->currentText().toLocal8Bit();

	config.set( Config::CROSSBAR_NAME , crossbarName );
	config.set( Config::CROSSBAR_INPUT_TYPE , crossbarInputType );



	//config.set(Config::VIDEO_ENABLE_KEY, _ui->enableVideoCheckBox->isChecked());
	config.set(Config::VIDEO_ENABLE_KEY, true);
	config.set(Config::VIDEO_WEBCAM_DEVICE_KEY, std::string(_ui->webcamDeviceComboBox->currentText().toLocal8Bit()));
	/************************************************************************////Add by zhuang 08-12-23
	int width = 176;
	int height = 144;
	int bitrate = 1960;
	std::string codecSerial = "XVID/MPEG2";
	//��ȡ��Ƶ֡��width��height��bitrate
	_webcamDriver->getCodecParameter(std::string(_ui->webcamDeviceComboBox->currentText().toLocal8Bit()),width,height,bitrate,codecSerial);
	/************************************************************************////Modify by zhuang 08-12-26
	//����͸���ɹ̶�ֵ��width=352��height=288
	QStringList mediaTypeInfos = _ui->videosizecomboBox->currentText().split("X");
	if( mediaTypeInfos.size()>0 )
	{
		if( mediaTypeInfos.at(0).size()>0 )
		{
			width = atoi( mediaTypeInfos.at( 0).toLocal8Bit().constData() );
			height = atoi( mediaTypeInfos.at(1).toLocal8Bit().constData() );
		}
	}

	config.set(Config::VIDEO_FRAME_WIDTH_KEY,width);
	config.set(Config::VIDEO_FRAME_HEIGHT_KEY,height);
	//config.set(Config::VIDEO_FRAME_WIDTH_KEY,352);
	//config.set(Config::VIDEO_FRAME_HEIGHT_KEY,288);
	/************************************************************************/
	config.set(Config::VIDEO_BITRATE_KEY,bitrate);
	config.set(Config::VIDEO_CODEC_SERIAL,codecSerial);
	/************************************************************************/
	

/*
	QTreeWidgetItem * item = _ui->videoQualityTreeWidget->currentItem();
	if (item) {
		EnumVideoQuality::VideoQuality videoQuality = EnumVideoQuality::VideoQualityNormal;
		QString text = item->text(VIDEO_QUALITY_COLUMN);
		if (tr("Normal") == text) {
			videoQuality = EnumVideoQuality::VideoQualityNormal;
		}
		else if (tr("Good") == text) {
			videoQuality = EnumVideoQuality::VideoQualityGood;
		}
		else if (tr("Very good") == text) {
			videoQuality = EnumVideoQuality::VideoQualityVeryGood;
		}
		else if (tr("Excellent") == text) {
			videoQuality = EnumVideoQuality::VideoQualityExcellent;
		}
		else {
			LOG_FATAL("unknown video quality=" + text.toStdString());
		}
		*/
	int item = _ui->videoQualitySlider->sliderPosition();
	if (item) {
		EnumVideoQuality::VideoQuality videoQuality = EnumVideoQuality::VideoQualityNormal;
		if (1 == item) {
			videoQuality = EnumVideoQuality::VideoQualityNormal;
		}
		else if (2 == item) {
			videoQuality = EnumVideoQuality::VideoQualityGood;
		}
		else if (3 == item) {
			videoQuality = EnumVideoQuality::VideoQualityVeryGood;
		}
		else if (4 == item) {
			videoQuality = EnumVideoQuality::VideoQualityExcellent;
		}
		else {
			LOG_FATAL("unknown video quality=");
		}
		config.set(Config::VIDEO_QUALITY_KEY, EnumVideoQuality::toString(videoQuality));
	}

	//弹出确定按钮   zhenHua.sun 2010-08-20
	QTextCodec::setCodecForTr( QTextCodec::codecForName("utf-8"));
	QMessageBox::information(getWidget(), 
		QObject::tr("确认"),
		QObject::tr("保存成功"), 
		QMessageBox::Ok);
}

/************************************************************************///modify by zhuang 09-01-09
/*�����������
string �� QString��ת�������Ļ������������
fromstdString->fromLocal8Bit   
tostdString->toLocal8Bit 
*/
/************************************************************************/
void QtVideoSettings::readConfig() {
	Config & config = ConfigManager::getInstance().getCurrentConfig();

	_ui->enableVideoCheckBox->setChecked(config.getVideoEnable());

	_ui->webcamDeviceComboBox->clear();
	_ui->webcamDeviceComboBox->addItems(StringListConvert::toQStringList(_webcamDriver->getDeviceList()));
	_ui->webcamDeviceComboBox->setCurrentIndex(
		_ui->webcamDeviceComboBox->findText(QString::fromLocal8Bit(config.getVideoWebcamDevice().c_str()))
		);

	char mediaTypeInfo[56];
	int width = config.getVideoFrameWidth();
	int height = config.getVideoFrameHeight();
	sprintf( mediaTypeInfo , "%dX%d", width , height );
	int confIndex = _ui->videosizecomboBox->findText( QString::fromLocal8Bit(mediaTypeInfo) );
	_ui->videosizecomboBox->setCurrentIndex( confIndex>0 ? confIndex : 0 );


	//添加对crossbar的配置   add by zhenHua.sun 2010-07-15
	_ui->crossbarlistcomboBox->clear();
	_webcamDriver->getCrossbarDeviceList();
	_ui->crossbarlistcomboBox->addItems( 
		StringListConvert::toQStringList( _webcamDriver->getCrossbarDeviceList() )
		);
	
	_ui->crossbarlistcomboBox->setCurrentIndex(
		_ui->crossbarlistcomboBox->findText( QString::fromLocal8Bit( config.getCrossbarName().c_str() ) )
		);
	_ui->crossbarclasscomboBox->setCurrentIndex( 
		_ui->crossbarclasscomboBox->findText( QString::fromLocal8Bit( config.getCrossbarInputType().c_str() ) )
		);



	EnumVideoQuality::VideoQuality videoQuality = EnumVideoQuality::toVideoQuality(config.getVideoQuality());
	QString videoQualityText;

	switch (videoQuality) {
	case EnumVideoQuality::VideoQualityNormal:
		{
			videoQualityText = tr("Normal");
			_ui->videoQualitySlider->setSliderPosition(1);
			break;
		}
		

	case EnumVideoQuality::VideoQualityGood:
		{
			videoQualityText = tr("Good");
			_ui->videoQualitySlider->setSliderPosition(2);
			break;
		}

	case EnumVideoQuality::VideoQualityVeryGood:
		{
			videoQualityText = tr("Very good");
			_ui->videoQualitySlider->setSliderPosition(3);
			break;
		}

	case EnumVideoQuality::VideoQualityExcellent:
		{
			videoQualityText = tr("Excellent");
			_ui->videoQualitySlider->setSliderPosition(4);
			break;
		}

	default:
		LOG_FATAL("unknown video quality=" + String::fromNumber(videoQuality));
	}

	//QList<QTreeWidgetItem *> items = _ui->videoQualityTreeWidget->findItems(videoQualityText, Qt::MatchExactly, VIDEO_QUALITY_COLUMN);
	//if (items.size() > 0) {
	//	_ui->videoQualityTreeWidget->setItemSelected(items[0], true);
	//}
}

void QtVideoSettings::frameCapturedEventHandler(IWebcamDriver * sender, piximage * image) {

	//if (!_ui->webcamSelectionGroupBox->isEnabled()) {
	//	return;
	//}

	QSize qs(_ui->webcamPreviewLabel->width(), _ui->webcamPreviewLabel->height());

	QImage tmpRgbImage(qs,  QImage::Format_RGB32);
	QtVideoQt::convertPixImageToQImage(image, qs, &tmpRgbImage);

	QPixmap tmp = QPixmap::fromImage(tmpRgbImage);
	{
	    Mutex::ScopedLock lock(_mutex);

	    _lastWebcamPixmap =  tmp;
	}

	newWebcamImage();
}

void QtVideoSettings::newWebcamImageCaptured() {
	Mutex::ScopedLock lock(_mutex);
	
	if (_lastWebcamPixmap.isNull())
	  return;

	_ui->webcamPreviewLabel->setPixmap(_lastWebcamPixmap);
}

void QtVideoSettings::startWebcamPreview(const QString & deviceName) {
	if (!_previewStarted) {
		/************************************************************************///Add by zhuang 09-01-09
		/*                                                                      */
		QString device_str = deviceName;
		std::string device = device_str.toLocal8Bit();
		/************************************************************************/		
		_webcamDriver->frameCapturedEvent += boost::bind(&QtVideoSettings::frameCapturedEventHandler, this, _1, _2);
		_webcamDriver->setDevice(device);
		_webcamDriver->setResolution(320, 240);
		_webcamDriver->setPalette(PIX_OSI_YUV420P);
		_webcamDriver->startCapture();

		_previewStarted = true;
	}
}

void QtVideoSettings::stopWebcamPreview() {
	if (_previewStarted) {
		_webcamDriver->frameCapturedEvent -= boost::bind(&QtVideoSettings::frameCapturedEventHandler, this, _1, _2);
		_webcamDriver->stopCapture();

		_previewStarted = false;
	}
}

void QtVideoSettings::webcamPreview() {
	if (!_previewStarted)
	{
		startWebcamPreview(_ui->webcamDeviceComboBox->currentText());
	}
	else
	{
		stopWebcamPreview();
	}
}

void QtVideoSettings::hideEvent(QHideEvent * event) {
	stopWebcamPreview();
}

void QtVideoSettings::makeTestCallClicked() {
	CUserProfile * cUserProfile = _cWengoPhone.getCUserProfileHandler().getCUserProfile();
	if ((cUserProfile) && (cUserProfile->getUserProfile().getActivePhoneLine())) {
		cUserProfile->getUserProfile().getActivePhoneLine()->makeCall(VIDEO_TEST_CALL);
		QtWengoPhone* qWengoPhone = QtVideoSettings::_cWengoPhone._qtWengoPhone;//Modify By LZY 2010-09-28
		qWengoPhone->getWidget()->raise();
	}
}

void QtVideoSettings::saveButtonClicked()
{
	saveConfig();

	//重新读取配置文件..zhenHua.sun 2010-09-30
	readConfig();
}

//*****************************************************add by zhenHua.sun 2010-07-15
/**
 * @brief 更新crossbar输入类型下拉框的内容
 */
void QtVideoSettings::crossbarChanged( const QString& crossbarName )
{

	//添加对crossbarInputType的配置   add by zhenHua.sun 2010-07-15
	_ui->crossbarclasscomboBox->clear();
	_ui->crossbarclasscomboBox->addItems( 
		StringListConvert::toQStringList( _webcamDriver->getCrossbarInputType( crossbarName.toStdString() ))
		);
}

//*****************************************************


void QtVideoSettings::configButtonClicked()
{
	this->readConfig();
}

void QtVideoSettings::webCamChanged( const QString& deviceName )
{
	Config & config = ConfigManager::getInstance().getCurrentConfig();

	StringList deviceMediaTypes = _webcamDriver->getVideoDeviceMediaType(qPrintable(deviceName));

	_ui->videosizecomboBox->clear();
	if( deviceMediaTypes.size()==0 )
	{
		//deviceMediaTypes += "352X288";
		//deviceMediaTypes += "704X576";
		//deviceMediaTypes += "1280X720";
		//deviceMediaTypes += "1920X1080";  不再设置默认值 ...zhenHua.sun 2010-12-13
	}
	_ui->videosizecomboBox->addItems(
		StringListConvert::toQStringList( deviceMediaTypes )
		);
	_ui->videosizecomboBox->setCurrentIndex( 0 );
}