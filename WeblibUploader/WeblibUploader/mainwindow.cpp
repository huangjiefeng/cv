#include "mainwindow.h"
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent /* = 0 */)
{
	setupUi(this);
	m_weblibuploader = NULL;
	m_uploadsetting = NULL;
	m_history = NULL;
	uiInitialize();
}

MainWindow::~MainWindow()
{
	if(m_weblibuploader != NULL)
	{
		disconnect(m_weblibuploader, SIGNAL(error(const QString &)), this, SLOT(showError(const QString &)));
		disconnect(this->showUploadAction, SIGNAL(triggered()), this, SLOT(showUploadInfo()));
		delete m_weblibuploader;
		m_weblibuploader = NULL;
		delete m_glUploader;
	}

	if(m_uploadsetting != NULL)
	{
		disconnect(this->uploadSettingAction, SIGNAL(triggered()), this, SLOT(showSetting()));
		delete m_uploadsetting;
		m_uploadsetting = NULL;
		delete m_glSetting;
	}

	if(m_history != NULL)
	{
		disconnect(this->showHistoryAction, SIGNAL(triggered()), this, SLOT(showHistory()));
		delete m_history;
		m_history = NULL;
		delete m_glHistory;
	}

	delete m_status;
}


void MainWindow::uiInitialize()
{
	m_status = new QLabel();
	Ui::MainWindow::statusBar->addWidget(m_status);
	Ui::MainWindow::statusBar->setStyleSheet(QString("QStatusBar::item{border:0px}"));

	m_weblibuploader = new WeblibUpload();
	connect(m_weblibuploader, SIGNAL(error(const QString &)), this, SLOT(showError(const QString &)));
	m_glUploader = new QGridLayout();
	m_glUploader->addWidget(m_weblibuploader);
	m_glUploader->setMargin(0);
	this->uploadpage->setLayout(m_glUploader);

	m_uploadsetting = new UploadSetting();
	m_glSetting = new QGridLayout();
	m_glSetting->addWidget(m_uploadsetting);
	m_glSetting->setMargin(0);
	this->settingpage->setLayout(m_glSetting);

	m_history = new UploadHistory();
	m_glHistory = new QGridLayout();
	m_glHistory->addWidget(m_history);
	m_glHistory->setMargin(0);
	this->historypage->setLayout(m_glHistory);

	connect(this->showUploadAction, SIGNAL(triggered()), this, SLOT(showUploadInfo()));
	connect(this->uploadSettingAction, SIGNAL(triggered()), this, SLOT(showSetting()));
	connect(this->showHistoryAction, SIGNAL(triggered()), this, SLOT(showHistory()));
}

void MainWindow::showUploadInfo()
{
	this->stackedWidget->setCurrentWidget(uploadpage);
}

void MainWindow::showSetting()
{
	this->stackedWidget->setCurrentWidget(settingpage);
}

void MainWindow::showError(const QString &info)
{
	m_status->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss  ") + info);
}

void MainWindow::showHistory()
{
	this->stackedWidget->setCurrentWidget(historypage);
}

void MainWindow::emptyErrorInfo()
{
	m_status->setText(" ");
}