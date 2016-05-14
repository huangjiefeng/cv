#ifndef _GROUPSELECTOR_H_
#define _GROUPSELECTOR_H_

#include "ui_groupselector.h"
#include <QNetworkAccessManager>
#include <QTreeWidgetItem>
#include <QNetworkCookieJar>

struct TreeItemData
{
	QString name;
	QString type;
	int id;
};
Q_DECLARE_METATYPE(TreeItemData);

class GroupSelector : public QDialog
{
	Q_OBJECT
public:
	GroupSelector(const QString &inurl, const QString &account, const QString &password);
	~GroupSelector();

	void uiInitialize();
signals:
	void selectGroup(const QString &groupname, const int &groupid);

protected:
	void accept();

private slots:
	void itemClicked(QTreeWidgetItem *item, int column);
	void itemExpanded(QTreeWidgetItem *item);
	void replycookies();
	void replyselectmember();

private:
	void setWeblibInfo();
	void getTreeItems(int id, QTreeWidgetItem *parent);

private:
	Ui::group ui;
	QString m_url;
	QString m_account;
	QString m_password;
	QNetworkCookieJar *m_cookie;
	QNetworkAccessManager *m_manager;
};

#endif