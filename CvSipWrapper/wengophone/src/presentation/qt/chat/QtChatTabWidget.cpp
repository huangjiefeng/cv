#include "stdafx.h"
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
#include "QtChatTabWidget.h"

#include <QtGui/QKeyEvent>
#include <QtGui/QPainter>
#include <QtGui/QTabBar>
#include <QtGui/QToolButton>

#include <cutil/global.h>
#include <util/Logger.h>

static const int BLINK_OVERLAY_ALPHA = 128;

QtChatTabWidget::QtChatTabWidget(QWidget * parent) : QTabWidget (parent) {
	_normalColor = palette().color(QPalette::Foreground);
	_blinkingColor = Qt::red;
	_currentColor = _normalColor;
	_timerId = startTimer(500);

#if !defined(OS_MACOSX)
	// On MacOS X, QtCloseChatTabWidget is used in QtChatWindow
	QToolButton * closeTabButton = new QToolButton(this);
	QIcon closeIcon(QPixmap(":/pics/close_normal.png"));
	closeIcon.addPixmap(QPixmap(":/pics/close_on.png"), QIcon::Normal, QIcon::On);
	closeTabButton->setIcon(closeIcon);
	closeTabButton->setToolTip(tr("Close chat"));
	closeTabButton->setAutoRaise(true);

	setCornerWidget(closeTabButton, Qt::TopRightCorner);
	connect(closeTabButton, SIGNAL(clicked()), SIGNAL(closeButtonClicked()));
#endif
}

void QtChatTabWidget::setBlinkingTab(int index) {
	if (_blinkingTabIcons.contains(index)) {
		return;
	}
	QIcon icon = tabBar()->tabIcon(index);

	_blinkingTabIcons[index].normal = icon;

	// Create blinking icon
	QImage img = icon.pixmap(tabBar()->iconSize()).toImage();

	QPainter painter(&img);
	painter.setCompositionMode(QPainter::CompositionMode_SourceAtop);

	QPixmap overlay(img.size());
	overlay.fill(QColor(255, 255, 255, BLINK_OVERLAY_ALPHA));
	painter.drawPixmap(0, 0, overlay);
	painter.end();

	_blinkingTabIcons[index].blinking = QIcon(QPixmap::fromImage(img));
}

void QtChatTabWidget::stopBlinkingTab(int index) {
	tabBar()->setTabTextColor(index, _normalColor);

	if (_blinkingTabIcons.contains(index)) {
		tabBar()->setTabIcon(index,_blinkingTabIcons[index].normal);
		_blinkingTabIcons.remove(index);
	}
}

void QtChatTabWidget::timerEvent(QTimerEvent * event) {
	if ( _currentColor == _normalColor) {
		_currentColor = _blinkingColor;
	} else {
		_currentColor = _normalColor;
	}

	Q_FOREACH(int index, _blinkingTabIcons.keys()) {
		tabBar()->setTabTextColor(index, _currentColor);

		if (_currentColor == _blinkingColor) {
			tabBar()->setTabIcon(index, _blinkingTabIcons[index].blinking);
		} else {
			tabBar()->setTabIcon(index, _blinkingTabIcons[index].normal);
		}
	}
}

void QtChatTabWidget::keyPressEvent(QKeyEvent* event) {
	if (event->key() == Qt::Key_Tab && (event->modifiers() & Qt::ControlModifier)) {
		goToNextTab();
		event->accept();
	} else {
		event->ignore();
	}
}

void QtChatTabWidget::goToNextTab() {
	//ctrlTabPressed();
	if (count() <= 1) {
		return;
	}

	int i = currentIndex();
	i++;
	if (i ==count()) {
		i = 0;
	}
	setCurrentIndex(i);

}
