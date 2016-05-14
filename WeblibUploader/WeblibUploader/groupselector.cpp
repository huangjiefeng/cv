#include "groupselector.h"
#include <QDebug>
#include <QTextCodec>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonArray>
#include <QJsonObject>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QTimer>

GroupSelector::GroupSelector(const QString &inurl, const QString &account, const QString &password)
: m_url(inurl), m_account(account), m_password(password)
{
	ui.setupUi(this);
	m_cookie = new QNetworkCookieJar();
	m_manager = new QNetworkAccessManager();
	m_manager->setCookieJar(m_cookie);

	setWeblibInfo();
}

GroupSelector::~GroupSelector()
{
	disconnect(ui.treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(itemClicked(QTreeWidgetItem *, int)));
	disconnect(ui.treeWidget, SIGNAL(itemExpanded(QTreeWidgetItem *)), this, SLOT(itemExpanded(QTreeWidgetItem *)));
	delete m_cookie;
	delete m_manager;
}

void GroupSelector::setWeblibInfo()
{
	QUrl url(m_url + "/login/authenticate.action");
	QByteArray data;
	data.append("account=" + m_account + "&password=" + m_password);
	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
	m_manager->setCookieJar(m_cookie);
	QNetworkReply *reply = m_manager->post(request, data);
	connect(reply, SIGNAL(finished()), this, SLOT(replycookies()));
}

void GroupSelector::replycookies()
{
	QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
	disconnect(reply, SIGNAL(finished()), this, SLOT(replycookies()));
	if(reply->error())
	{
		qDebug() << reply->readAll();
		QMessageBox::information(this,  QString::fromLocal8Bit("连接Weblib失败"),  QString::fromLocal8Bit("登录Weblib失败，请检查账号密码或Weblib服务是否可用"));
		reply->deleteLater();
		return;
	}

	QString restring = reply->readAll().trimmed();
	reply->deleteLater();
	int startindex = restring.indexOf("\"id\":");
	int endindex = restring.indexOf(",", startindex);
	QUrl url(m_url + "/login/selectMember.action");
	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
	QByteArray data;
	data.append("memberId=" + restring.mid(startindex + 5, endindex - startindex - 5));
	reply = m_manager->post(request, data);
	connect(reply, SIGNAL(finished()), this, SLOT(replyselectmember()));
}

void GroupSelector::replyselectmember()
{
	QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
	disconnect(reply, SIGNAL(finished()), this, SLOT(replyselectmember()));
	if(reply->error())
	{
		qDebug() << reply->readAll();
		QMessageBox::information(this,  QString::fromLocal8Bit("连接Weblib失败"),  QString::fromLocal8Bit("选择用户时出错"));
		reply->deleteLater();
		return;
	}
	reply->deleteLater();

	uiInitialize();
}

void GroupSelector::uiInitialize()
{
	ui.treeWidget->setHeaderLabel(QString::fromLocal8Bit("选择存储位置"));
	connect(ui.treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(itemClicked(QTreeWidgetItem *, int)));
	connect(ui.treeWidget, SIGNAL(itemExpanded(QTreeWidgetItem *)), this, SLOT(itemExpanded(QTreeWidgetItem *)));
	getTreeItems(0, NULL);
}

void GroupSelector::getTreeItems(int id, QTreeWidgetItem *parent)
{
	if(parent != NULL && parent->childCount() > 0)
	{
		QList<QTreeWidgetItem*> children = parent->takeChildren();
		foreach(QTreeWidgetItem *item, children)
			delete item;
	}

	QUrl url(m_url + "/group/trees.action");
	QByteArray param("containPersonGroup=true&containAblumCategory=false");
	if(id != 0)
		param.append(QString("&categoryId=%1").arg(id));

	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
	QNetworkReply *reply = m_manager->post(request, param);
	QEventLoop loop;
	connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	QTimer::singleShot(10000, &loop, SLOT(quit()));
	loop.exec();
	disconnect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	if(reply->error() || !reply->isFinished())
	{
		qDebug() << reply->readAll();
		reply->deleteLater();
		return;
	}
	
	QByteArray test = reply->readAll();
	QJsonParseError ok;
	QJsonDocument result = QJsonDocument::fromJson(test, &ok);
	reply->deleteLater();
	if(ok.error != QJsonParseError::NoError)
		return;
	
	QJsonObject childobj = result.object();
	QJsonArray idarray = childobj["children"].toArray();
	for(int i=0; i<idarray.size(); i++)
	{
		QJsonObject obj = idarray.at(i).toObject();
		QTreeWidgetItem *treeitem = new QTreeWidgetItem();
		TreeItemData data;
		data.name = obj["displayName"].toString();
		data.id = (int)obj["id"].toDouble();
		data.type = obj["type"].toString();
		treeitem->setText(0, data.name);
		treeitem->setData(0, Qt::UserRole, qVariantFromValue(data));
		if(data.type == "group")
			treeitem->setIcon(0, QIcon(":/image/tx/weblib/group.gif"));
		else
			treeitem->setIcon(0, QIcon(":/image/tx/weblib/category.png"));

		if(parent == NULL)
			ui.treeWidget->addTopLevelItem(treeitem);
		else
			parent->addChild(treeitem);

		if(obj["leaf"].toBool() == false)
		{
			QTreeWidgetItem *emptyitem = new QTreeWidgetItem();
			emptyitem->setText(0, QString::fromLocal8Bit("正在加载..."));
			treeitem->addChild(emptyitem);
		}
	}
}

void GroupSelector::itemClicked(QTreeWidgetItem *item, int column)
{
	if(item->text(0) != QString::fromLocal8Bit("正在加载..."))
	{
		TreeItemData data = QVariant((item->data(0, Qt::UserRole))).value<TreeItemData>();
		if(data.type == "group")
		{
			emit selectGroup(data.name, data.id);
			this->close();
		}
	}
}

void GroupSelector::itemExpanded(QTreeWidgetItem *item)
{
	if(item->childCount() != 1)
		return;

	QTreeWidgetItem *child = item->child(0);
	if(child->text(0) == QString::fromLocal8Bit("正在加载..."))
	{
		TreeItemData data = QVariant(item->data(0, Qt::UserRole)).value<TreeItemData>();
		getTreeItems(data.id, item);
	}
}

void GroupSelector::accept()
{
	QTreeWidgetItem *item = ui.treeWidget->currentItem();
	if(item == NULL)
	{
		QMessageBox::information(this, QString::fromLocal8Bit("请选择目标柜子"),  QString::fromLocal8Bit("未选中文件柜"));
		return;
	}

	if(item->text(0) ==  QString::fromLocal8Bit("正在加载..."))
	{
		QMessageBox::information(this,  QString::fromLocal8Bit("请选择目标柜子"),  QString::fromLocal8Bit("请重新展开柜子以刷新内容"));
		return;
	}

	TreeItemData data = QVariant(item->data(0, Qt::UserRole)).value<TreeItemData>();
	if(data.type != "group")
	{
		QMessageBox::information(this,  QString::fromLocal8Bit("请选择目标柜子"),  QString::fromLocal8Bit("目标不是文件柜"));
		return;
	}
	emit selectGroup(data.name, data.id);
	this->close();
}
