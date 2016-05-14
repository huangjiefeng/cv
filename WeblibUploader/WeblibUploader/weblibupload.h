#ifndef _WELIBUPLOADER_H_
#define _WELIBUPLOADER_H_

#include <QWidget>
#include "ui_weblibupload.h"
#include "workerservice.h"
#include "itemmodel.h"

class WeblibUpload : public QWidget
{
	Q_OBJECT
public:
	WeblibUpload(QWidget *parent = 0);
	~WeblibUpload();

signals:
	void error(const QString &info);

public slots:
	void loginFailed();
private:
	void uiInitialzie();

private:
	Ui::Form ui;
	ItemModel m_model;
	WorkerService *m_service;
};

#endif // TESTER_H
