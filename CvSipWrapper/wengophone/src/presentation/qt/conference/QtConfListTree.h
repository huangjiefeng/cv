#pragma once

#ifndef QTCONFLISTTREE_H
#define QTCONFLISTTREE_H

#include <vector>
using namespace std;
#include "QtTreeItem.h"
#include <model/conference/CVMsgParser.h>

struct temp
{
	char *title;
	char *summary;
};


class QtTreeItem;

class QtConfListTree : public QAbstractItemModel
{
public:
	QtConfListTree(const QString &data, QObject *parent = 0);
	QtConfListTree();
	~QtConfListTree();

	QVariant data(const QModelIndex &index, int role) const;
	Qt::ItemFlags flags(const QModelIndex &index) const;
	QVariant headerData(int section, Qt::Orientation orientation,
		int role = Qt::DisplayRole) const;
	QModelIndex index(int row, int column,
		const QModelIndex &parent = QModelIndex()) const;
	QModelIndex parent(const QModelIndex &index) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;
	void setupModelData(vector<ConfInfo *> &array);

private:

	void setupModelData(vector<ConfInfo *> &array, QtTreeItem *parent);

	QtTreeItem *rootItem;
};

#endif