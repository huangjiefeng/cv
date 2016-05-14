#include "itemfinishedmodel.h"
#include <QDateTime>

ItemFinishedModel::ItemFinishedModel()
{
	m_querynum = 20;
}

ItemFinishedModel::~ItemFinishedModel()
{
	for(int i=0; i<m_filelist.size(); i++)
		delete m_filelist.at(i);
	m_filelist.clear();
}

int ItemFinishedModel::rowCount(const QModelIndex & parent) const
{
	return m_filelist.size();
}

int ItemFinishedModel::columnCount(const QModelIndex & parent) const
{
	return 4;
}

QVariant ItemFinishedModel::data(const QModelIndex & index, int role) const
{
	if(!index.isValid())
		return QVariant();

	if(index.row() >= rowCount())
		return QVariant();


	if(role == Qt::DisplayRole)
	{
		switch (index.column())
		{
		case 0:
			return index.row() + 1;
		case 1:
			return m_filelist.at(index.row())->filename;
		case 2:
			return SizeConverter(m_filelist.at(index.row())->filesize);
		case 3:
			return m_filelist.at(index.row())->finishedtime;
		default:
			return QVariant();
		}
	}

	return QVariant();
}

QVariant ItemFinishedModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role!=Qt::DisplayRole)
		return QVariant();
	if (orientation!=Qt::Horizontal)
		return QVariant();
	switch (section)
	{
	case 0:
		return QString::fromLocal8Bit("序号");
		break;
	case 1:
		return QString::fromLocal8Bit("文件名");
		break;
	case 2:
		return QString::fromLocal8Bit("大小");
		break;
	case 3:
		return QString::fromLocal8Bit("完成时间");
		break;
	}
	return QVariant();
}

void ItemFinishedModel::slotItemStateChanged(ItemOperate oper, const UploadItem &item)
{
	if(oper != DeleteItem)
		return;

	if(m_filelist.size() > m_querynum)
	{
		beginRemoveRows(QModelIndex(), m_filelist.size(), m_filelist.size());
		delete m_filelist.back();
		m_filelist.pop_back();
		endRemoveRows();
	}

	UploadCompleteItem *finitem = new UploadCompleteItem();
	finitem->filename = this->filenamePicker(item.filename);
	finitem->filesize = item.size;
	finitem->finishedtime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
	beginInsertRows(QModelIndex(), 1, 1);
	m_filelist.push_front(finitem);
	endInsertRows();
}

QString ItemFinishedModel::filenamePicker(const QString &filepath)
{
	int index1 = filepath.lastIndexOf("\\");
	int index2 = filepath.lastIndexOf("/");
	if(index1 < index2)
		index1 = index2;
	return filepath.right(filepath.length() - index1 - 1);
}