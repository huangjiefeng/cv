/*
 * WengoPhone, a voice over Internet phone
 * Copyright (C) 2004-2006  Wengo
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

#ifndef OWQTDIALPAD_H
#define OWQTDIALPAD_H

#include <util/NonCopyable.h>

#include <QtGui/QWidget>

#include <string>

class QtWengoPhone;
class CDtmfThemeManager;
class QtSVGDialpad;
class QStringList;
class UpQComboBox;

namespace Ui { class DialpadWidget; }

/**
 * Dialpad widget: where the user clicks for DTMF.
 *
 * @author Tanguy Krotoff
 */
class QtDialpad : public QWidget, NonCopyable {
	Q_OBJECT
public:
	enum ThemeMode { ThemeModeAll, ThemeModeDefaultOnly, ThemeModeAudioSmileysOnly };

	QtDialpad(CDtmfThemeManager & cDtmfThemeManager, QtWengoPhone * qtWengoPhone);

	~QtDialpad();

	void setThemeMode(ThemeMode);

private Q_SLOTS:

	void oneButtonClicked();

	void twoButtonClicked();

	void threeButtonClicked();

	void fourButtonClicked();

	void fiveButtonClicked();

	void sixButtonClicked();

	void sevenButtonClicked();

	void eightButtonClicked();

	void nineButtonClicked();

	void zeroButtonClicked();

	void starButtonClicked();

	void poundButtonClicked();

	void setCurrentTheme(const QString & newThemeName);

	void refreshComboBox();

private:

	void fillComboBox();

	void playTone(const std::string & tone);

	Ui::DialpadWidget * _ui;

	QtWengoPhone * _qtWengoPhone;

	CDtmfThemeManager & _cDtmfThemeManager;

	QtSVGDialpad * _qtSVGDialpad;

	UpQComboBox * _audioSmileysComboBox;

	ThemeMode _themeMode;
};

#endif	//OWQTDIALPAD_H
