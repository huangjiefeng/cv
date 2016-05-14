#include "FileIconDelegate.h"
#include <QFileInfo>
#include <QFileIconProvider>
#include <QApplication>
#include <QIcon>
#include <QDebug>
#include <QPainter>

FileIconDelegate::FileIconDelegate(QWidget *parent /* = 0 */) :  QStyledItemDelegate(parent), m_iconsize(20, 20)
{
}

void FileIconDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	if (!index.isValid())
		return;

	if(!index.data().canConvert<QString>())
		return;

	QString filename = qvariant_cast<QString>(index.data());
	QFileInfo fileinfo(filename);
	if(!fileinfo.exists())	
		return;

	QFileIconProvider provider;
	QIcon icon = provider.icon(fileinfo);

	painter->save();
	QSize iconSize = icon.actualSize(m_iconsize);
	painter->translate(option.rect.left(), option.rect.top());
	QRect iconRect = QRect(4, (option.rect.height() - iconSize.height()) / 2, iconSize.width(), iconSize.height());
	QRect clip(30, (option.rect.height()) / 2 - 6, option.rect.width() - iconRect.right() - 10, option.rect.height() / 2 + 6);
	icon.paint(painter, iconRect);
	painter->drawText(clip, fileinfo.fileName());
	painter->restore();
}

QSize FileIconDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	return QStyledItemDelegate::sizeHint(option, index);
}

QWidget *FileIconDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	return QStyledItemDelegate::createEditor(parent, option, index);
}

void FileIconDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	QStyledItemDelegate:;setEditorData(editor, index);
}

void FileIconDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	QStyledItemDelegate:;setModelData(editor, model, index);
}