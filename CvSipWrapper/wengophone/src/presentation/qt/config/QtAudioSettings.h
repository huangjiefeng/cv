/************************************************************************/
/*Coolview 3.0 界面
 *zhenHua.sun 2010-08-14
 */
/************************************************************************/
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

#ifndef QTAUDIOSETTINGS_H
#define QTAUDIOSETTINGS_H

#include "QtISettings.h"

#include <QtGui/QWidget>
class QtToolWidget;
class CWengoPhone;
class QSlider;
//namespace Ui { class AudioSettings; }
#include "ui_AudioSettings.h"

/**
 * Audio configuration panel.
 *
 * @author Tanguy Krotoff
 * @author Xavier Desjardins
 */
class QtAudioSettings : public QWidget, public QtISettings {
	friend class QtToolWidget;
	Q_OBJECT
public:

	QtAudioSettings(CWengoPhone & cWengoPhone, QWidget * parent);

	virtual ~QtAudioSettings();

	virtual QString getName() const;

	virtual QString getTitle() const;

	virtual void saveConfig();

	virtual QString getIconName() const;

	virtual QWidget *getWidget() const {
		return (QWidget*)this;
	}

	void initVolumeSlider(QSlider* slider);

private Q_SLOTS:

	void makeTestCallClicked();

	void testOutputDevice();

	/* zhenHua.sun 2010-08-01
	void testRingingDevice();

	void AdvPressed();

	void UpPressed();

	void DownPressed();

	void ResetPressed();
	*/

	void saveButtonClicked();

	void setVolume(int value);

	/**
	 * @brief 点击工具栏里的配置按钮
	 */
	void configButtonClicked();


	void TeleSetVolumeCommand(int Value,char Type);//Add By LZY 2010-10-05 响应SetVolume动作的Signal

private:

	virtual void readConfig();
	void fillAudioCodecList();

	Ui::AudioSettings * _ui;

	CWengoPhone & _cWengoPhone;
};

#endif	//QTAUDIOSETTINGS_H
