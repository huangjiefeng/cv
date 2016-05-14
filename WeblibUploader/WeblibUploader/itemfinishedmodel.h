#ifndef _ITEMFINISHEDMODEL_H_
#define _ITEMFINISHEDMODEL_H_

#include <QAbstractTableModel>
#include "commonstruts.h"
#include <QQueue>

struct UploadCompleteItem
{
	QString filename;
	qint64 filesize;
	QString finishedtime;
};

class ItemFinishedModel : public QAbstractTableModel
{
	Q_OBJECT
public:
	ItemFinishedModel();
	~ItemFinishedModel();
	int rowCount(const QModelIndex & parent = QModelIndex()) const;
	int columnCount(const QModelIndex & parent = QModelIndex()) const;
	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
signals:
	void dataChange(QModelIndex, QModelIndex);
public slots:
	void slotItemStateChanged(ItemOperate oper, const UploadItem &item);
private:
	QString filenamePicker(const QString &filepath);
private:
	QQueue<UploadCompleteItem *> m_filelist;
	int m_querynum;
};

#endif