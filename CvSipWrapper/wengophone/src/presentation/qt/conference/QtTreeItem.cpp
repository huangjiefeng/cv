#include "stdafx.h"
#include ".\qttreeitem.h"


QtTreeItem::QtTreeItem(const QList<QVariant> &data, QtTreeItem *parent)
{
	parentItem = parent;
	itemData = data;
}

QtTreeItem::~QtTreeItem()
{
//	qDeleteAll(childItems);
	deleteAllChildItems();
}

void QtTreeItem::appendChild(QtTreeItem *item)
{
	childItems.append(item);
}

void QtTreeItem::removeChild(int index)
{
	childItems.removeAt(index);
}

QtTreeItem *QtTreeItem::child(int row)
{
	return childItems.value(row);
}

int QtTreeItem::childCount() const
{
	return childItems.count();
}

int QtTreeItem::columnCount() const
{
	return itemData.count();
}

QVariant QtTreeItem::data(int column) const
{
	return itemData.value(column);
}

QtTreeItem *QtTreeItem::parent()
{
	return parentItem;
}

int QtTreeItem::row() const
{
	if (parentItem)
		return parentItem->childItems.indexOf(const_cast<QtTreeItem*>(this));

	return 0;
}

void QtTreeItem::deleteAllChildItems()
{
	for (int i=0;i<childItems.size();i++)
	{
		QtTreeItem *item = childItems.at(i);
		delete item;
	}
	childItems.clear();
}
