#include "IpConfigDialog.h"
#include "login-widget.h"
#include "main-window.h"
#include <DeviceManager/DeviceManager.h>
#include <windows.h>

#include <QMessageBox>

IPConfigDialog::IPConfigDialog( LoginWidget* loginWidget )
{
	setWindowFlags( Qt::WindowStaysOnTopHint );

	setupUi(this);
	this->_loginWidget = loginWidget;

	connect( this->adaptorListCombo , SIGNAL(currentIndexChanged( const QString & )),
		this , SLOT( adaptorChangedSlot( const QString& ) ) );
	connect( this->cancelButton , SIGNAL(clicked()) , this , SLOT(cancelButtonClickedSlot()) );
	connect( this->saveButton , SIGNAL(clicked()) , this , SLOT(saveButtonClickedSlot() ) );
	connect( this->enableDHCPCheckbox, SIGNAL(stateChanged( int) ) , this , SLOT( dhcpChangedSlot(int) ) );
	readConfig();
}

IPConfigDialog::~IPConfigDialog()
{

}

void IPConfigDialog::readConfig()
{
	//读取适配器
	vector<string> adaptorList;
	DeviceManager::GetInstatnce()->GetNetworkAdaptorList(&adaptorList);
	QStringList adaptors;
	TransformStringVectorToQStringList( &adaptorList , &adaptors );

	this->adaptorListCombo->clear();
	this->adaptorListCombo->addItems( adaptors );

}

void IPConfigDialog::cancelButtonClickedSlot()
{
	this->hide();
}

void IPConfigDialog::saveButtonClickedSlot()
{
	if( !this->enableDHCPCheckbox->isChecked() )
	{
		QString ip = this->ipAddressLineEdit->text().trimmed();
		QString mask = this->maskLineEdit->text().trimmed();
		QString gateway = this->gatewayLineEdit->text().trimmed();
		QStringList dnsServers = this->dnsLineEdit->text().split(" ");

		if( ip.isEmpty() || mask.isEmpty()
			|| gateway.isEmpty() || dnsServers.size()==0 )
		{
			QMessageBox::information( this ,
				QString::fromLocal8Bit("错误"),
				QString::fromLocal8Bit("请正确填写所有参数"),
				QMessageBox::Ok );
			return;
		}
		DeviceManager::GetInstatnce()->SetNetworkAdaptorInfo( this->adaptorListCombo->currentText().toStdString(),
			ip.toStdString(), mask.toStdString() ,gateway.toStdString(), dnsServers[0].toStdString(),false );
	}else
	{
		string ip = "";
		string mask = "";
		string gateway = "";
		string dns = "";
		DeviceManager::GetInstatnce()->SetNetworkAdaptorInfo( this->adaptorListCombo->currentText().toStdString(),
			ip , mask , gateway ,dns , true );
	}

	if( QMessageBox::information( this , QString::fromLocal8Bit("提示"), 
		QString::fromLocal8Bit("IP配置已经修改，需要重启才能生效，是否现在重启？"),
		QMessageBox::Ok|QMessageBox::Cancel ) == QMessageBox::Ok )
	{
		this->hide();
		_loginWidget->hide();
		_loginWidget->_mainWindow->quitSystemSlot();
		system("shutdown -r -t 0");
	}

	this->hide();
	this->_loginWidget->loadConfig();
}

void IPConfigDialog::adaptorChangedSlot( const QString&name )
{
	string ip;
	string mask;
	string dnsServer;
	string gateway;
	bool enableDHCP;
	DeviceManager::GetInstatnce()->GetNetworkAdaptorInfo( name.toStdString() , &ip , &mask , &gateway , &dnsServer,&enableDHCP );
	this->ipAddressLineEdit->setText( ip.c_str() );
	this->maskLineEdit->setText( mask.c_str() );
	this->gatewayLineEdit->setText( gateway.c_str() );
	this->dnsLineEdit->setText( dnsServer.c_str() );
	if( enableDHCP )
	{
		this->ipAddressLineEdit->setDisabled(true );
		this->maskLineEdit->setDisabled(true );
		this->gatewayLineEdit->setDisabled(true);
		this->dnsLineEdit->setDisabled(true);
		this->enableDHCPCheckbox->setCheckState( Qt::Checked );
	}
}

void IPConfigDialog::dhcpChangedSlot( int state )
{
	if( state==Qt::Unchecked )
	{
		this->ipAddressLineEdit->setDisabled( false );
		this->maskLineEdit->setDisabled(  false );
		this->gatewayLineEdit->setDisabled( false);
		this->dnsLineEdit->setDisabled(false);
	}else
	{
		this->ipAddressLineEdit->setDisabled(true );
		this->maskLineEdit->setDisabled(true );
		this->gatewayLineEdit->setDisabled(true);
		this->dnsLineEdit->setDisabled(true);
	}
}