#include "weblibupload.h"
#include "progressbardelegate.h"
#include "FileIconDelegate.h"
#include <QTableWidgetItem>
#include <QMessageBox>

WeblibUpload::WeblibUpload(QWidget *parent) : QWidget(parent)
{
	ui.setupUi(this);
	uiInitialzie();
}

WeblibUpload::~WeblibUpload()
{
	disconnect(ui.btn_start, SIGNAL(clicked()), m_service, SLOT(slotStartUpload()));
	disconnect(ui.btn_stop, SIGNAL(clicked()), m_service, SLOT(slotStopUpload()));
	disconnect(m_service, SIGNAL(signalShowErrorInfo(const QString&)), this, SIGNAL(error(const QString &)));
	disconnect(m_service, SIGNAL(signalItemStateChanged(ItemOperate, const UploadItem &)), &m_model, SLOT(slotItemStateChanged(ItemOperate, const UploadItem &)));
}

void WeblibUpload::uiInitialzie()
{
	m_service = &WorkerService::getInstance();

	ui.tableView->setModel(&m_model);
	ui.tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui.tableView->setItemDelegateForColumn(0, new FileIconDelegate);
	ui.tableView->setItemDelegateForColumn(2, new ProgressBarDelegate);

	connect(m_service, SIGNAL(signalItemStateChanged(ItemOperate, const UploadItem &)), &m_model, SLOT(slotItemStateChanged(ItemOperate, const UploadItem &)));
	connect(ui.btn_start, SIGNAL(clicked()), m_service, SLOT(slotStartUpload()));
	connect(ui.btn_stop, SIGNAL(clicked()), m_service, SLOT(slotStopUpload()));
	connect(m_service, SIGNAL(signalShowErrorInfo(const QString&)), this, SIGNAL(error(const QString &)));
}

void WeblibUpload::loginFailed()
{
	QMessageBox::critical(this, QString::fromLocal8Bit("登录weblib失败"), QString::fromLocal8Bit("Weblib登录失败，请检查上传参数或网络连接是否异常"));
}