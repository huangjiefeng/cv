#include "uploadsetting.h"
#include <QApplication>
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include "groupselector.h"

UploadSetting::UploadSetting(QWidget *parent /* = 0 */)
{
	ui.setupUi(this);
	uiInitialize();
	refreshSetting();
}

UploadSetting::~UploadSetting()
{
	disconnect(ui.txbtn_save, SIGNAL(clicked()), this, SLOT(confirmSlot()));
	disconnect(ui.txbtn_reset, SIGNAL(clicked()), this, SLOT(cancelSlot()));
	disconnect(ui.txbtn_dirselect, SIGNAL(clicked()), this, SLOT(selectDirSlot()));
	disconnect(ui.txbtn_group, SIGNAL(clicked()), this, SLOT(selectWeblibDirSlot()));

}

void UploadSetting::uiInitialize()
{
	m_service = &WorkerService::getInstance();

	connect(ui.txbtn_save, SIGNAL(clicked()), this, SLOT(confirmSlot()));
	connect(ui.txbtn_reset, SIGNAL(clicked()), this, SLOT(cancelSlot()));
	connect(ui.txbtn_dirselect, SIGNAL(clicked()), this, SLOT(selectDirSlot()));
	connect(ui.txbtn_group, SIGNAL(clicked()), this, SLOT(selectWeblibDirSlot()));
	ui.txtb_password->setEchoMode (QLineEdit::Password);
}

void UploadSetting::refreshSetting()
{
	WeblibSettingInfo info = m_service->getWeblibSetting();

	ui.txtb_address->setText(info.url);
	ui.txtb_account->setText(info.account);
	ui.txtb_password->setText(info.password);
	ui.txtb_dirpath->setText(info.filepath);
	ui.txtb_dirlevel->setValue(info.scanlevel);
	ui.txtb_groupname->setText(info.groupname);
	ui.txtb_timeout->setText(QString::number(info.connecttimeout / 1000));
	ui.txtb_interval->setText(QString::number(info.scaninterval / 1000));
	ui.txtb_speed->setText(QString::number(info.uploadspeed / 1024));
	ui.txtb_threadnum->setText(QString::number(info.threads));
	ui.chkb_userdir->setChecked(info.iscreateudir);
	m_groupid = info.groupid;
}

void UploadSetting::confirmSlot()
{
	WeblibSettingInfo obj;
	obj.url = ui.txtb_address->text();
	obj.account = ui.txtb_account->text();
	obj.password = ui.txtb_password->text();
	obj.filepath = ui.txtb_dirpath->text();
	obj.scanlevel = ui.txtb_dirlevel->text().toInt();
	obj.groupname = ui.txtb_groupname->text();
	obj.groupid = m_groupid;
	obj.connecttimeout = ui.txtb_timeout->text().toLongLong() * 1000;
	obj.scaninterval = ui.txtb_interval->text().toLongLong() * 1000;
	obj.uploadspeed = ui.txtb_speed->text().toLongLong() * 1024;
	if(obj.uploadspeed < 0)
		obj.uploadspeed = 0;

	obj.threads = ui.txtb_threadnum->text().toInt();
	obj.iscreateudir = ui.chkb_userdir->isChecked();

	m_service->slotSetWeblibSetting(obj);
	if(!m_service->saveSettings())
		QMessageBox::critical(this, QString::fromLocal8Bit("修改失败"), QString::fromLocal8Bit("配置文件写入失败"));
	else
		QMessageBox::information(this, QString::fromLocal8Bit("修改成功"), QString::fromLocal8Bit("上传设置修改成功"));
}

void UploadSetting::cancelSlot()
{
	refreshSetting();
}

void UploadSetting::selectDirSlot()
{
	QFileDialog* openFilePath = new QFileDialog(this, QString::fromLocal8Bit("请选择上传文件夹")); 
	openFilePath->setFileMode(QFileDialog::DirectoryOnly);
	if(openFilePath->exec() == QDialog::Accepted)
	{
		ui.txtb_dirpath->setText(openFilePath->selectedFiles().at(0));
	}
	delete openFilePath;
}

void UploadSetting::selectWeblibDirSlot()
{
	if(ui.txtb_address->text().length() == 0
		|| ui.txtb_account->text().length() == 0
		|| ui.txtb_password->text().length() == 0)
	{
		QMessageBox::information(this, QString::fromLocal8Bit("缺少登录信息"), QString::fromLocal8Bit("请先输入Weblib地址、账号以及密码"));
		return;
	}

	GroupSelector selector(ui.txtb_address->text(), ui.txtb_account->text(), ui.txtb_password->text());
	connect(&selector, SIGNAL(selectGroup(const QString &, const int &)), 
		this, SLOT(selectGroup(const QString &, const int &)));

	selector.setModal(true);
	selector.exec();

	disconnect(&selector, SIGNAL(selectGroup(const QString &, const int &)), 
		this, SLOT(selectGroup(const QString &, const int &)));
}

void UploadSetting::selectGroup(const QString &name, const int &id)
{
	ui.txtb_groupname->setText(name);
	m_groupid = id;
}