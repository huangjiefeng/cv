#ifndef _ITEMMODEL_H_
#define _ITEMMODEL_H_

#include <QAbstractTableModel>
#include "commonstruts.h"
#include <QIcon>

class ItemModel : public QAbstractTableModel
{
	Q_OBJECT
public:
	ItemModel();
	~ItemModel();

	//////////////model/////////////////////
	int rowCount(const QModelIndex & parent = QModelIndex()) const;
	int columnCount(const QModelIndex & parent = QModelIndex()) const;
	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
signals:
	void dataChange(QModelIndex, QModelIndex);
	///////////////////////////////////////
public slots:
	void slotItemStateChanged(ItemOperate oper, const UploadItem &item);

private:
	UploadItem *checkFileContained(const QString &filepath);
	QIcon getStatusIcon(UploadStatus status) const;

private:
	//顺序列表，用于model中快速定位上传记录
	QList<UploadItem *> m_filelist;
	//检索列表，用于快速查找特定的上传记录，m_filelist与m_indexlist中的内容相互对应
	QMap<QString, UploadItem *> m_indexlist;
	QMap<QString, QIcon*> m_icons;
};

#endif