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

#include "QtHistoryWidget.h"

#include "ui_HistoryWidget.h"

#include <model/history/HistoryMemento.h>
#include <model/phonecall/SipAddress.h>
#include <control/history/CHistory.h>
#include <presentation/qt/QtToolBar.h>
#include <presentation/qt/callbar/QtCallBar.h>
#include <presentation/qt/history/QtHistory.h>

#include <qtutil/SafeConnect.h>
#include <util/Logger.h>
#include <util/SafeDelete.h>

#include <QtGui/QtGui>

QtHistoryWidget::QtHistoryWidget(QWidget * parent, QtHistory * qtHistory)
	: QObject(parent), 
	_cHistory(qtHistory->getCHistory()),
	_qtHistory(qtHistory) {

	_qtToolBar = NULL;
	_qtCallBar = NULL;
	_historyWidget = new QWidget(parent);

	_ui = new Ui::HistoryWidget();
	_ui->setupUi(_historyWidget);

	QIcon filterIcon(":/pics/history/filter.png");
	_ui->lineEdit->createLeftButton(filterIcon);

	_proxyModel = new QSortFilterProxyModel(this);
	_proxyModel->setSourceModel(qtHistory);
	_proxyModel->setFilterKeyColumn(1);
	_ui->treeView->setModel(_proxyModel);
	SAFE_CONNECT_RECEIVER(_ui->lineEdit, SIGNAL(textChanged(const QString&)),
		_proxyModel, SLOT(setFilterFixedString(const QString&)) );

#if defined(OS_MACOSX)
	_historyWidget->layout()->setMargin(10);
#endif

	computeTreeViewColumnSizes();

	//treeView
	SAFE_CONNECT(_ui->treeView,
		SIGNAL(doubleClicked(const QModelIndex&)),
		SLOT(emitReplayItemRequested(const QModelIndex&))
		);
	SAFE_CONNECT(_ui->treeView,
		SIGNAL(clicked(const QModelIndex&)),
		SIGNAL(missedCallsSeen()) );

	SAFE_CONNECT(_ui->treeView->selectionModel(),
		SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)),
		SLOT(updateCallBarFromIndex(const QModelIndex&))
		);

	SAFE_CONNECT(_ui->treeView,
		SIGNAL(customContextMenuRequested(const QPoint&)),
		SLOT(showPopupMenuForCurrentIndex())
		);
}

QtHistoryWidget::~QtHistoryWidget() {
	OWSAFE_DELETE(_ui);
	OWSAFE_DELETE(_historyWidget);
}

QWidget * QtHistoryWidget::getWidget() const {
	return _historyWidget;
}

int QtHistoryWidget::idFromIndex(const QModelIndex& proxyIndex) {
	QModelIndex sourceIndex = _proxyModel->mapToSource(proxyIndex);
	return _qtHistory->data(sourceIndex, Qt::UserRole).toInt();
}

void QtHistoryWidget::emitReplayItemRequested(const QModelIndex& index) {
	int id = idFromIndex(index);
	replayItemRequested(id);
}

void QtHistoryWidget::updateCallBarFromIndex(const QModelIndex& index) {
	if (!index.isValid()) {
		return;
	}

	int id = idFromIndex(index);
	HistoryMementoCollection * collection = _cHistory.getHistory().getHistoryMementoCollection();
	HistoryMemento* memento = collection->getMemento(id);
	if (!memento || !_qtCallBar) {
		return;
	}

	if (memento->getState() != HistoryMemento::ChatSession) {
		SipAddress sipAddress = memento->getPeer();
		_qtCallBar->setPhoneComboBoxEditText(sipAddress.toString());
	}
}

void QtHistoryWidget::showPopupMenuForCurrentIndex() {
	QModelIndex index = _ui->treeView->selectionModel()->currentIndex();
	if (!index.isValid()) {
		return;
	}

	int id = idFromIndex(index);
	HistoryMementoCollection * collection = _cHistory.getHistory().getHistoryMementoCollection();
	HistoryMemento* memento = collection->getMemento(id);
	Q_ASSERT(memento);

	QMenu menu(_ui->treeView);
	QAction * replayAction;
	switch (memento->getState()) {
	case HistoryMemento::OutgoingSmsOk:
	case HistoryMemento::OutgoingSmsNok:
		replayAction = menu.addAction(QIcon(":/pics/actions/send-sms-16.png"), tr("Open in SMS window"));
		break;
	case HistoryMemento::OutgoingCall:
	case HistoryMemento::IncomingCall:
	case HistoryMemento::MissedCall:
	case HistoryMemento::RejectedCall:
		replayAction = menu.addAction(QIcon(":/pics/actions/accept-phone.png"), tr("Call this peer"));
		break;
	case HistoryMemento::ChatSession:
		replayAction = menu.addAction(QIcon(":/pics/actions/chat.png"), tr("Show chat log"));
		break;
	default:
		LOG_FATAL("Unhandled memento state");
		break;
	}

	QAction* deleteAction = menu.addAction(QIcon(":/pics/actions/delete.png"), tr("Erase this entry"));

	QAction* action = menu.exec(QCursor::pos());
	if (action == replayAction) {
		replayItemRequested(id);
	} else if (action == deleteAction) {
		removeItemRequested(id);
	}
}

void QtHistoryWidget::retranslateUi() {
	_ui->retranslateUi(_historyWidget);
	_qtHistory->updatePresentation();
}

void QtHistoryWidget::computeTreeViewColumnSizes() {
	// Do *not* use QHeaderView::ResizeToContents because this cause the
	// widget to ask for the data of *all* the rows.
	QHeaderView* header = _ui->treeView->header();

	int margin = _ui->treeView->style()->pixelMetric(QStyle::PM_FocusFrameHMargin) + 1;

	// State
	// lst is the list of state with a text, see QtHistory::textForMementoState
	QList<HistoryMemento::State> lst;
	lst
		<< HistoryMemento::IncomingCall
		<< HistoryMemento::OutgoingCall
		<< HistoryMemento::MissedCall
		<< HistoryMemento::RejectedCall
		<< HistoryMemento::OutgoingSmsOk
		<< HistoryMemento::ChatSession;
	int width = 0;
	QFontMetrics fm = _ui->treeView->fontMetrics();
	Q_FOREACH(HistoryMemento::State state, lst) {
		QString text = _qtHistory->textForMementoState(state);
		width = qMax(width, fm.width(text));
	}
	width += _ui->treeView->style()->pixelMetric(QStyle::PM_ListViewIconSize);
	header->resizeSection(0, width + 4 * margin);

	// Name
	header->resizeSection(1, fm.width("a long long long long username"));

	// Date
	QString text = QtHistory::formatDate(QDateTime::currentDateTime());
	width = fm.width(text);
	header->resizeSection(2, width + 2 * margin);

	// Duration
	text = QtHistory::formatDuration(QTime(20, 0));
	width = fm.width(text);
	header->setResizeMode(3, QHeaderView::Interactive);
	header->resizeSection(3, width + 2 * margin);
}
