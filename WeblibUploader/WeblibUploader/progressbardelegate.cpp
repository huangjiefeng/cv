#include "progressbardelegate.h"
#include <QStyleOptionProgressBar>
#include <QApplication>

void ProgressBarDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	if(index.data().canConvert<int>())
	{
		QStyleOptionProgressBar progress;
		progress.rect = option.rect;
		progress.minimum = 0;
		progress.maximum = 100;
		progress.progress = qvariant_cast<int>(index.data());
		progress.text = QString::number(progress.progress) + "%";
		progress.textVisible = true;
		progress.textAlignment = Qt::AlignHCenter;
		QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progress, painter);
	}
	else
		QStyledItemDelegate::paint(painter, option, index);
}

QSize ProgressBarDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	return QStyledItemDelegate::sizeHint(option, index);
}

QWidget *ProgressBarDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	return QStyledItemDelegate::createEditor(parent, option, index);
}

void ProgressBarDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	QStyledItemDelegate:;setEditorData(editor, index);
}

void ProgressBarDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	QStyledItemDelegate:;setModelData(editor, model, index);
}