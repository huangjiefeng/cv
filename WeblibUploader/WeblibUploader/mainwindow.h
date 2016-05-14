#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include "ui_MainWindow.h"
#include "weblibupload.h"
#include "uploadsetting.h"
#include "uploadhistory.h"

class MainWindow : public QMainWindow, public Ui::MainWindow
{
	Q_OBJECT
public:
	MainWindow(QWidget *parent = 0);
	virtual ~MainWindow();

	void uiInitialize();
private slots:
	void showUploadInfo();
	void showSetting();
	void showHistory();
	void showError(const QString &info);
	void emptyErrorInfo();

private:
	QGridLayout *m_glUploader;
	WeblibUpload *m_weblibuploader;
	QGridLayout *m_glSetting;
	UploadSetting *m_uploadsetting;
	QGridLayout *m_glHistory;
	UploadHistory *m_history;
	QLabel *m_status;
};

#endif