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

#include "QtTreeViewDelegate.h"

#include "QtContactWidget.h"
#include "QtContactListManager.h"
#include "QtContactPixmap.h"
#include "QtContactList.h"

#include <control/CWengoPhone.h>
#include <control/profile/CUserProfile.h>
#include <control/profile/CUserProfileHandler.h>

#include <util/Logger.h>
#include <util/StringList.h>

#include <QtGui/QtGui>

static int GROUP_WIDGET_FRAME_HEIGHT = 22;

QtTreeViewDelegate::QtTreeViewDelegate(CWengoPhone & cWengoPhone,
	QtContactManager * qtContactManager, QObject * parent)
	: QItemDelegate(parent),
	_qtContactManager(qtContactManager),
	_cWengoPhone(cWengoPhone) {
}

void QtTreeViewDelegate::setParent(QWidget * parent) {
	_parent = parent;
}

QWidget * QtTreeViewDelegate::createEditor(QWidget * parent,
	const QStyleOptionViewItem &, const QModelIndex & index) const {

	QtContactListManager * ul = QtContactListManager::getInstance();
	QtContact * qtContact = ul->getContact(index.data().toString());
	QtContactWidget * widget =
		new QtContactWidget(qtContact->getId().toStdString(), _cWengoPhone,
			_qtContactManager, parent);
	return widget;
}

void QtTreeViewDelegate::setEditorData(QWidget * editor, const QModelIndex & index) const {
	QtContactWidget * widget = qobject_cast<QtContactWidget *>(editor);
	if (!widget) {
		return;
	}
	widget->setText(index.model()->data(index).toString());
}

void QtTreeViewDelegate::setModelData(QWidget * editor, QAbstractItemModel * model,
	const QModelIndex & index) const {

	QtContactWidget * widget = qobject_cast<QtContactWidget *>(editor);

	if (!widget) {
		return;
	}
	model->setData(index, widget->text());
}

void QtTreeViewDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const {
	if (!index.parent().isValid()) {
		drawGroup(painter, option, index);
	} else {
		QtContactListManager * ul = QtContactListManager::getInstance();
		ul->paintContact(painter, option, index);
	}
}

QSize QtTreeViewDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const {
	QSize orig = QItemDelegate::sizeHint(option, index);
	QtContactListManager * ul = QtContactListManager::getInstance();
	QtContact * qtContact = ul->getContact(index.data().toString());

	if (qtContact) {
		return QSize(orig.width(), ul->getContact(index.data().toString())->getHeight());
	} else {
		if (!index.parent().isValid()) {
			return (QSize(orig.width(), GROUP_WIDGET_FRAME_HEIGHT));
		}
	}
	return orig;
}

void QtTreeViewDelegate::drawGroup(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const {
	painter->setPen(option.palette.text().color());
	painter->drawPixmap(option.rect.left(),option.rect.top(),getGroupBackGround(option.rect));

	QFont font = option.font;
	font.setBold(true);
	painter->setFont(font);

	QtContactPixmap * spx = QtContactPixmap::getInstance();
	QPixmap px;
	if (option.state & QStyle::State_Open) {
		px = spx->getPixmap(QtContactPixmap::ContactGroupOpen);
	} else {
		px = spx->getPixmap(QtContactPixmap::ContactGroupClose);
	}
	int x = option.rect.left();
	QRect r = option.rect;
	painter->drawPixmap(x, r.top() + 3, px);
	x += px.width() + 3;
	r.setLeft(x);
	int y = ((r.bottom()-r.top())-QFontMetrics(font).height())/2;
	r.setTop(y + r.top());
	r.setLeft(r.left() + 10);
	QString groupName;
	std::string groupId;
	std::string groupNameTmp;
	if (index.data().toString() == QtContactList::DEFAULT_GROUP_NAME) {
		groupName = tr("Contacts list");
		groupNameTmp=std::string(groupName.toUtf8().data());
	} else {
		groupName = index.data().toString();
		groupId = groupName.toStdString();
		groupNameTmp = _cWengoPhone.getCUserProfileHandler().getCUserProfile()->getCContactList().getContactGroupName(groupId);
	}
	groupName = QString::fromUtf8(groupNameTmp.c_str(), groupNameTmp.size());
	QString str = groupName;
	painter->drawText(r, Qt::AlignLeft, str, 0);
}

QPixmap QtTreeViewDelegate::getGroupBackGround(const QRect & rect) const {
	if (_groupBackground.isNull() || _groupBackground.width() != rect.width()) {
		QLinearGradient lg(QPointF(1.0, 0.0), QPointF(1.0, GROUP_WIDGET_FRAME_HEIGHT));
		lg.setColorAt(0.8, QColor(212, 208, 200));
		lg.setColorAt(0.0, QColor(255, 255, 255));
		_groupBackground = QPixmap(rect.width(),GROUP_WIDGET_FRAME_HEIGHT);
		QPainter painter(&_groupBackground);
		painter.fillRect(_groupBackground.rect(),QBrush(lg));
		painter.end();
	}
	return _groupBackground;
}
