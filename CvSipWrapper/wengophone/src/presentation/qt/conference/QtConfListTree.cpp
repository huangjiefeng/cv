#include "stdafx.h"
#include ".\qtconflisttree.h"

#include ".\QtConfListTree.h"



QtConfListTree::QtConfListTree(const QString &data, QObject *parent)
: QAbstractItemModel(parent)
{
	QList<QVariant> rootData;
	rootData << "Title" << "Summary";
	rootItem = new QtTreeItem(rootData);

}

QtConfListTree::QtConfListTree()
{
	QList<QVariant> rootData;
	rootData << tr("\344\274\232\350\256\256\345\220\215\347\247\260") << tr("\345\274\200\345\247\213\346\227\266\351\227\264");
	rootItem = new QtTreeItem(rootData);
//	setupModelData(array, rootItem);
}

QtConfListTree::~QtConfListTree()
{
	delete rootItem;
}

int QtConfListTree::columnCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return static_cast<QtTreeItem*>(parent.internalPointer())->columnCount();
	else
		return rootItem->columnCount();
}

QVariant QtConfListTree::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (role != Qt::DisplayRole)
		return QVariant();

	QtTreeItem *item = static_cast<QtTreeItem*>(index.internalPointer());

	return item->data(index.column());
}

Qt::ItemFlags QtConfListTree::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return 0;

	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant QtConfListTree::headerData(int section, Qt::Orientation orientation,
							   int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return rootItem->data(section);

	return QVariant();
}

QModelIndex QtConfListTree::index(int row, int column, const QModelIndex &parent)
const
{
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	QtTreeItem *parentItem;

	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<QtTreeItem*>(parent.internalPointer());

	QtTreeItem *childItem = parentItem->child(row);
	if (childItem)
		return createIndex(row, column, childItem);
	else
		return QModelIndex();
}

QModelIndex QtConfListTree::parent(const QModelIndex &index) const
{
	if (!index.isValid())
		return QModelIndex();

	QtTreeItem *childItem = static_cast<QtTreeItem*>(index.internalPointer());
	QtTreeItem *parentItem = childItem->parent();

	if (parentItem == rootItem)
		return QModelIndex();

	return createIndex(parentItem->row(), 0, parentItem);
}

int QtConfListTree::rowCount(const QModelIndex &parent) const
{
	QtTreeItem *parentItem;
	if (parent.column() > 0)
		return 0;

	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<QtTreeItem*>(parent.internalPointer());

	return parentItem->childCount();
}


void QtConfListTree::setupModelData(vector<ConfInfo *> &array, QtTreeItem *parent)
{
	QList<QtTreeItem*> parents;
	parents.clear();
	parents << parent;
	

	QtTreeItem *pParent = parents.last();
	pParent->deleteAllChildItems();
	/*
	for (int i=0;i<parent->childCount();i++)
	{
		QtTreeItem *pChild = pParent->child(i);

	}*/
	int size = array.size();
	for (int i=0;i<size;i++)
	{
		QList<QVariant> columnData;
		columnData<<tr(array[i]->Title.c_str())<<QString(array[i]->StartTime.c_str());
		parents.last()->appendChild(new QtTreeItem(columnData, parents.last()));
	}
	reset();
}

void QtConfListTree::setupModelData(vector<ConfInfo *> &array)
{
	//rootItem->deleteAllChildItems();

	//int size = array.size();
	//for (int i=0;i<size;i++)
	//{
	//	QList<QVariant> columnData;
	//	columnData<<QString(array[i]->Title.c_str())<<QString(array[i]->StartTime.c_str());
	//	rootItem->appendChild(new QtTreeItem(columnData, rootItem));
	//}
	//reset();
	setupModelData(array,rootItem);
}
