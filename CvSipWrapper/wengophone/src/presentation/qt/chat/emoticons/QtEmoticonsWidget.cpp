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

#include "QtEmoticonsWidget.h"

#include "QtEmoticon.h"
#include "QtEmoticonButton.h"
#include "QtEmoticonsManager.h"

#include <util/Logger.h>
#include <util/SafeDelete.h>
#include <cutil/global.h>

#include <qtutil/SafeConnect.h>

#include <QtGui/QtGui>
#include <QtXml/QtXml>

EmoticonsWidget::EmoticonsWidget(QWidget * parent)
	: QFrame(parent, Qt::Popup) {

	setFrameStyle(QFrame::Box | QFrame::Plain);

	_layout = NULL;
	_state = Popup;
	_buttonX = 0;
	_buttonY = 0;
}

void EmoticonsWidget::buttonClicked(const QtEmoticon & emoticon) {
	if (_state == Popup) {
		close();
	}
	emoticonClicked(emoticon);
}

void EmoticonsWidget::changeState() {
	if (_state == Popup) {
		close();
		setWindowFlags(Qt::Window);
		_state = Window;
		show();
	} else {
		close();
		setWindowFlags(Qt::Popup);
		_state = Popup;
	}
}

void EmoticonsWidget::initButtons(const QString & protocol) {
	OWSAFE_DELETE(_layout);

	_layout = new QGridLayout(this);
	_layout->setMargin(0);
	_layout->setSpacing(0);
	_buttonX = 0;
	_buttonY = 0;
	QtEmoticonsManager * qtEmoticonsManager = QtEmoticonsManager::getInstance();
	QtEmoticonsManager::QtEmoticonList emoticonList = qtEmoticonsManager->getQtEmoticonList(protocol);
	QtEmoticonsManager::QtEmoticonList::iterator it;
	for (it = emoticonList.begin(); it != emoticonList.end(); it++) {
		addButton((*it));
	}
}

void EmoticonsWidget::addButton(const QtEmoticon & emoticon) {
	if (_buttonX == 10) {
		_buttonX = 0;
		_buttonY += 1;
	}
	QtEmoticonButton * button = new QtEmoticonButton(this);
	button->setEmoticon(emoticon);
	QSize buttonSize = emoticon.getButtonPixmap().size();
	button->setIconSize(buttonSize);
	_layout->addWidget(button, _buttonY, _buttonX);
	SAFE_CONNECT(button, SIGNAL(buttonClicked(QtEmoticon)), SLOT(buttonClicked(QtEmoticon)));
	_buttonX++;
}

void EmoticonsWidget::closeEvent(QCloseEvent * event) {
	closed();
	event->accept();
}
