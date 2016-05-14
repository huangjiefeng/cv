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

#ifndef QTVIDEOSETTINGS_H
#define QTVIDEOSETTINGS_H

#include "QtISettings.h"

#include <pixertool/pixertool.h>

#include <thread/Mutex.h>
#include <util/Trackable.h>

#include <QtGui/QPixmap>
#include <QtGui/QWidget>

class IWebcamDriver;

class QImage;
class QHideEvent;
class QString;
class CWengoPhone;
#include "ui_VideoSettings.h"

/**
 * Video configuration panel.
 *
 * Inherits from QWidget so we can overwrite QWidget::hideEvent() and
 * QWidget::showEvent() that are protected methods from QWidget.
 *
 * @author Tanguy Krotoff
 */
class QtVideoSettings : public QWidget, public QtISettings, public Trackable {

	friend class QtToolWidget;

	Q_OBJECT
public:

	QtVideoSettings(CWengoPhone & cWengoPhone, QWidget * parent);

	virtual ~QtVideoSettings();

	virtual QString getName() const;

	virtual QString getTitle() const;

	virtual void saveConfig();

	virtual QString getIconName() const;

	virtual QWidget *getWidget() const {
		return (QWidget*)this;
	}

private Q_SLOTS:

	void webcamPreview();

	void newWebcamImageCaptured();

	void startWebcamPreview(const QString & deviceName);

	void makeTestCallClicked();

	void saveButtonClicked();

	/**
	 * @brief 点击工具栏里的配置按钮
	 */
	void configButtonClicked();


	//*****************************************************add by zhenHua.sun 2010-07-15
	/**
	 * @brief 更新crossbar输入类型下拉框的内容
	 */
	void crossbarChanged( const QString& crossbarName );

	//*****************************************************

	/**
	 * @brief 更新视频大小下拉框
	 */
	void webCamChanged( const QString& deviceName );

Q_SIGNALS:

	void newWebcamImage();

private:

	virtual void readConfig();

	void stopWebcamPreview();

	void frameCapturedEventHandler(IWebcamDriver * sender, piximage * image);

	void hideEvent(QHideEvent * event);
	
	Ui::VideoSettings * _ui;

	IWebcamDriver * _webcamDriver;

	/** Contains the converted picture from the Webcam. */
	QImage _rgbImage;

	QPixmap _lastWebcamPixmap;

	Mutex _mutex;

	CWengoPhone & _cWengoPhone;

	bool _previewStarted;

	//获取CrossBar列表。本来想要在DirectXWebcamDriver中实现的。可是要更改较多的
};

#endif	//QTVIDEOSETTINGS_H
