#pragma once


#ifndef QTTREEITEM_H
#define QTTREEITEM_H

#include <QtGui/QtGui>

class QtTreeItem
{
public:
	QtTreeItem(const QList<QVariant> &data, QtTreeItem *parent = 0);
	~QtTreeItem();

	void appendChild(QtTreeItem *child);

	QtTreeItem *child(int row);
	int childCount() const;
	int columnCount() const;
	QVariant data(int column) const;
	int row() const;
	QtTreeItem *parent();

	void deleteAllChildItems();
	void removeChild(int index);

private:
	QList<QtTreeItem*> childItems;
	QList<QVariant> itemData;
	QtTreeItem *parentItem;
};

#endif