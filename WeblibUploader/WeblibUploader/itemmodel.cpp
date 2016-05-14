#include "itemmodel.h"

ItemModel::ItemModel()
{
	m_icons.insert("uploading", new QIcon(":/image/tx/weblib/Uploading.gif"));
	m_icons.insert("pending", new QIcon(":/image/tx/weblib/Pending.gif"));
	m_icons.insert("error", new QIcon(":/image/tx/weblib/Error.gif"));
	m_icons.insert("failed", new QIcon(":/image/tx/weblib/Failed.gif"));
	m_icons.insert("done", new QIcon(":/image/tx/weblib/Done.gif"));
	m_icons.insert("unknown", new QIcon(":/image/tx/weblib/Unknown.gif"));
}

ItemModel::~ItemModel()
{
	foreach(QIcon *icon, m_icons.values())
	{
		delete icon;
	}
}

int ItemModel::rowCount(const QModelIndex &parent) const
{
	return m_filelist.size();
}

int ItemModel::columnCount(const QModelIndex &parent) const
{
	return 6;
}

QVariant ItemModel::data(const QModelIndex &index, int role) const
{
	if(!index.isValid())
		return QVariant();

	if(index.row() >= rowCount())
		return QVariant();

	if(role == Qt::DecorationRole && index.column() == 5)
	{
		return getStatusIcon(m_filelist.at(index.row())->status);
	}

	if(role == Qt::DisplayRole)
	{
		switch (index.column())
		{
		case 0:
			return m_filelist.at(index.row())->filename;
		case 1:
			return SizeConverter(m_filelist.at(index.row())->size);
		case 2:
			return m_filelist.at(index.row())->pecent;
		case 3:
			return m_filelist.at(index.row())->speed;
		case 4:
			return m_filelist.at(index.row())->lefttime;
		default:
			return QVariant();
		}
	}

	return QVariant();
}

QIcon ItemModel::getStatusIcon(UploadStatus status) const
{
	switch(status)
	{
	case Uploading:
		return *m_icons["uploading"];
	case Pending:
		return *m_icons["pending"];
	case Failed:
		return *m_icons["failed"];
	case Error:
		return *m_icons["error"];
	case Stop:
		return *m_icons["pending"];
	case Done:
		return *m_icons["done"];
	default:
		return *m_icons["unknown"];
	}
}

QVariant ItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role!=Qt::DisplayRole)
		return QVariant();
	if (orientation!=Qt::Horizontal)
		return QVariant();
	switch (section)
	{
	case 0:
		return QString::fromLocal8Bit("文件名");
		break;
	case 1:
		return QString::fromLocal8Bit("大小");
		break;
	case 2:
		return QString::fromLocal8Bit("进度");
		break;
	case 3:
		return QString::fromLocal8Bit("速度");
		break;
	case 4:
		return QString::fromLocal8Bit("剩余时间");
		break;
	case 5:
		return QString::fromLocal8Bit("状态");
		break;
	}
	return QVariant();
}

UploadItem *ItemModel::checkFileContained(const QString &filepath)
{
	if(m_indexlist.contains(filepath))
		return m_indexlist[filepath];
	return NULL;
}

void ItemModel::slotItemStateChanged(ItemOperate oper, const UploadItem &item)
{
	switch(oper)
	{
	case AddItem:
		{
			UploadItem *newitem = new UploadItem;
			*newitem = item;
			beginInsertRows(QModelIndex(), m_filelist.size(), m_filelist.size());
			m_filelist.append(newitem);
			m_indexlist.insert(newitem->filename, newitem);
			endInsertRows();
			break;
		}
	case DeleteItem:
		{
			UploadItem *tagitem = checkFileContained(item.filename);
			if(tagitem == NULL)
				break;

			beginRemoveRows(QModelIndex(), m_filelist.indexOf(tagitem) + 1, m_filelist.indexOf(tagitem) + 1);
			m_filelist.removeAll(tagitem);
			m_indexlist.remove(tagitem->filename);
			endRemoveRows();
			
			QModelIndex startindex = createIndex(0, 1);
			QModelIndex endindex = createIndex(0, columnCount());
			emit dataChanged(startindex, endindex);

			delete tagitem;
			break;
		}
	case RefreshItem:
		{
			UploadItem *tagitem = checkFileContained(item.filename);
			if(tagitem == NULL)
				break;
			tagitem->bytesuploaded = item.bytesuploaded;
			tagitem->lefttime = item.lefttime;
			tagitem->pecent = item.pecent;
			tagitem->speed = item.speed;
			tagitem->status = item.status;

			QModelIndex startindex = createIndex(m_filelist.indexOf(tagitem), 1);
			QModelIndex endindex = createIndex(m_filelist.indexOf(tagitem), columnCount());
			emit dataChanged(startindex, endindex);
			break;
		}
	default:
		break;
	}
}