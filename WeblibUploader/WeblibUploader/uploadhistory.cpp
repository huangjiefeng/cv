#include "uploadhistory.h"

UploadHistory::UploadHistory(QWidget *parent)
{
	ui.setupUi(this);
	uiInitialzie();
}

UploadHistory::~UploadHistory()
{
	disconnect(m_service, SIGNAL(signalItemStateChanged(ItemOperate, const UploadItem &)), &m_model, SLOT(slotItemStateChanged(ItemOperate, const UploadItem &)));
}

void UploadHistory::uiInitialzie()
{
	m_service = &WorkerService::getInstance();

	ui.tv_history->setModel(&m_model);
	ui.tv_history->setSelectionBehavior(QAbstractItemView::SelectRows);

	connect(m_service, SIGNAL(signalItemStateChanged(ItemOperate, const UploadItem &)), &m_model, SLOT(slotItemStateChanged(ItemOperate, const UploadItem &)));
}