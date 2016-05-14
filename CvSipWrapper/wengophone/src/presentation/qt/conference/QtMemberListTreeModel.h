#pragma once

#ifndef QTMEMBERLISTTREEMODEL_H
#define QTMEMBERLISTTREEMODEL_H

#include <vector>
using namespace std;
#include "QtTreeItem.h"
#include <model/conference/CVMsgParser.h>


class QtTreeItem;

class QtMemberListTreeModel : public QAbstractItemModel
{
public:
	QtMemberListTreeModel(const QString &data, QObject *parent = 0);
	QtMemberListTreeModel();
	~QtMemberListTreeModel();

	QVariant data(const QModelIndex &index, int role) const;
	Qt::ItemFlags flags(const QModelIndex &index) const;
	QVariant headerData(int section, Qt::Orientation orientation,
		int role = Qt::DisplayRole) const;
	QModelIndex index(int row, int column,
		const QModelIndex &parent = QModelIndex()) const;
	QModelIndex parent(const QModelIndex &index) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;
	void setupModelData(vector<MemberInfo *> &array);
	QVariant getData(const QModelIndex &index, int role) const;

private:

	void setupModelData(vector<MemberInfo *> &array, QtTreeItem *parent);

	QtTreeItem *rootItem;
};

#endif