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

#ifndef OWQTCHATTABWIDGET_H
#define OWQTCHATTABWIDGET_H

#include <QtGui/QTabWidget>
#include <QtCore/QSet>
#include <QtCore/QHash>

class QKeyEvent;

/**
 * Subclass QTabWidget in order to blink them 
 * and to deal with CTRL+TAB
 * 
 * @ingroup presentation
 * @author Mr K.
 * @author Mathieu Stute.
 */
class QtChatTabWidget : public QTabWidget {
	Q_OBJECT
public:

	QtChatTabWidget(QWidget * parent = 0);

	void setBlinkingTab(int index);

	void stopBlinkingTab(int index);

public Q_SLOTS:

	void goToNextTab();

Q_SIGNALS:

	void ctrlTabPressed();

	void closeButtonClicked();

private:
	struct BlinkingIcon {
		QIcon normal;
		QIcon blinking;
	};
	typedef QHash<int, BlinkingIcon> BlinkingTabIcons;

	virtual void timerEvent(QTimerEvent * event);

	virtual void keyPressEvent(QKeyEvent * event);

	BlinkingTabIcons _blinkingTabIcons;

	QColor _currentColor;

	QColor _normalColor;

	QColor _blinkingColor;

	int _timerId;
};

#endif //OWQTCHATTABWIDGET_H
