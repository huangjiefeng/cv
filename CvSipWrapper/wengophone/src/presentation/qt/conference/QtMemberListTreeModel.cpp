#include "stdafx.h"
#include ".\qtmemberlisttreemodel.h"


QtMemberListTreeModel::QtMemberListTreeModel(const QString &data, QObject *parent)
: QAbstractItemModel(parent)
{
	QList<QVariant> rootData;
	rootData << tr("\345\220\215\347\247\260") << tr("\351\237\263")<< tr("\350\247\206")<< tr("\344\270\276\346\211\213");
	rootItem = new QtTreeItem(rootData);

}

QtMemberListTreeModel::QtMemberListTreeModel()
{
	QList<QVariant> rootData;
	rootData << tr("\345\220\215\347\247\260") << tr("\351\237\263")<< tr("\350\247\206")<< tr("\344\270\276\346\211\213");
	rootItem = new QtTreeItem(rootData);
	//	setupModelData(array, rootItem);
}

QtMemberListTreeModel::~QtMemberListTreeModel()
{
	delete rootItem;
}

int QtMemberListTreeModel::columnCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return static_cast<QtTreeItem*>(parent.internalPointer())->columnCount();
	else
		return rootItem->columnCount();
}

QVariant QtMemberListTreeModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

//	if (role != Qt::DisplayRole)
//		return QVariant();

	QtTreeItem *item = static_cast<QtTreeItem*>(index.internalPointer());

//	return item->data(index.column());
	switch (role) 
	{
		case Qt::DisplayRole:
			if (index.column() == 0)
				return item->data(index.column());
			break;
		case Qt::DecorationRole:
			if (index.column() == 1) {
				int tips = item->data(index.column()).toInt();
				if(tips==0)
					return QVariant(QIcon(tr(":/pics/statusbar/input_volume_error.png")));
				else if(tips==1)
					return QVariant(QIcon(tr(":/pics/conference/audio.png")));
				else
					return QVariant(QIcon(tr(":/pics/iconbar/add_contact.png")));
			}
			else if (index.column() == 2)
			{
				QString tips = item->data(index.column()).toString();
				if(tips.contains("m=video"))
					return QVariant(QIcon(tr(":/pics/iconbar/webcam.png")));
				return QVariant();
			}
			else if (index.column() == 3)
			{
				int tips = item->data(index.column()).toInt();
				if(tips==1)
					return QVariant(QIcon(tr(":/pics/conference/raisehand.bmp")));
				return QVariant();
			}
			break;

	}
	return QVariant();
}

Qt::ItemFlags QtMemberListTreeModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return 0;

	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant QtMemberListTreeModel::headerData(int section, Qt::Orientation orientation,
									int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return rootItem->data(section);

	return QVariant();
}

QModelIndex QtMemberListTreeModel::index(int row, int column, const QModelIndex &parent)
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

QModelIndex QtMemberListTreeModel::parent(const QModelIndex &index) const
{
	if (!index.isValid())
		return QModelIndex();

	QtTreeItem *childItem = static_cast<QtTreeItem*>(index.internalPointer());
	QtTreeItem *parentItem = childItem->parent();

	if (parentItem == rootItem)
		return QModelIndex();

	return createIndex(parentItem->row(), 0, parentItem);
}

int QtMemberListTreeModel::rowCount(const QModelIndex &parent) const
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


void QtMemberListTreeModel::setupModelData(vector<MemberInfo *> &array, QtTreeItem *parent)
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
		int index = array[i]->MemberURI.find('@',0);
		string displayName = array[i]->MemberURI.substr(0,index);
		columnData<<QString(displayName.c_str())<<QString(" ");
		parents.last()->appendChild(new QtTreeItem(columnData, parents.last()));
	}
	reset();
}

void QtMemberListTreeModel::setupModelData(vector<MemberInfo *> &array)
{
	rootItem->deleteAllChildItems();

	int size = array.size();
	for (int i=0;i<size;i++)
	{
		QList<QVariant> columnData;
		int index = array[i]->MemberURI.find('@',0);
		string displayName = array[i]->MemberURI.substr(0,index);
		columnData<<QString(displayName.c_str())<<array[i]->permitSpeak<<QString(array[i]->Sdp.c_str())<<array[i]->hand;
		rootItem->appendChild(new QtTreeItem(columnData, rootItem));
	}
	reset();
//	setupModelData(array,rootItem);
}

QVariant QtMemberListTreeModel::getData(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (role != Qt::DisplayRole)
		return QVariant();
	int row = index.row();
	int col = index.column();
	QtTreeItem *item = rootItem->child(row);
	return item->data(0);
}
