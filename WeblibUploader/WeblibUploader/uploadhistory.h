#ifndef _UPLOADHISTORY_H_
#define _UPLOADHISTORY_H_

#include <QWidget>
#include "ui_uploadhistory.h"
#include "workerservice.h"
#include "itemfinishedmodel.h"

class UploadHistory : public QWidget
{
	Q_OBJECT
public:
	UploadHistory(QWidget *parent = 0);
	~UploadHistory();
private:
	void uiInitialzie();

private:
	Ui::HistoryForm ui;
	ItemFinishedModel m_model;
	WorkerService *m_service;
};

#endif // TESTER_H
