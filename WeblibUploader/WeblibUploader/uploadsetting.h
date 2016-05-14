#ifndef _UPLOADSETTING_H_
#define _UPLOADSETTING_H_

#include "ui_uploadsetting.h"
#include "workerservice.h"

class UploadSetting : public QWidget
{
	Q_OBJECT
public:
	UploadSetting(QWidget *parent = 0);
	~UploadSetting();
private:
	void uiInitialize();
	void refreshSetting();

private slots:
	void confirmSlot();
	void cancelSlot();
	void selectDirSlot();
	void selectWeblibDirSlot();
	void selectGroup(const QString &name, const int &id);

private:
	Ui::SettingForm ui;
	int m_groupid;
	WorkerService *m_service;
};

#endif