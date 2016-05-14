#include "ptz_tab.h"
#include "ui_ptz_tab.h"

#include <QMessageBox>//提示对话框头文件

//2014.4.2.gmlan-增加云台控制功能
#include "ptz_controller.h"
#include "config/RunningConfig.h"

PtzTab::PtzTab(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PtzTab) {
  ui->setupUi(this);

  // 自动检测有效串口号并显示出来
  for (const QSerialPortInfo &SerialPortInfo : QSerialPortInfo ::availablePorts()) {
    ui->numberOfCom->addItem(SerialPortInfo.portName());
  }
  ui->protocolBtn->addItem(tr("PELCO_D"));
  ui->protocolBtn->addItem(tr("PELCO_P"));
  ui->protocolBtn->addItem(tr("VISCA"));
  ui->baudRate->addItem("9600");
  ui->baudRate->addItem("4800");
  ui->dataBits->addItem("8");
  ui->dataBits->addItem("7");
  ui->stopBits->addItem("1");
  ui->stopBits->addItem("2");
  ui->checkBits->addItem(QString::fromLocal8Bit("无校验位"));
  ui->checkBits->addItem(QString::fromLocal8Bit("奇校验位"));
  ui->checkBits->addItem(QString::fromLocal8Bit("偶校验位"));
  // 限制摄像机地址值的范围为0-255
  QValidator *validator = new QIntValidator(0, 255, this); // 0-255 only accept number
  ui->camAdressLed->setValidator(validator);

  //云台控制操作信号槽
  PTZController *controller = PTZController::Instance();
  connect(ui->leftBtn, &QPushButton::pressed, controller, &PTZController::TurnLeftSlot);
  connect(ui->leftBtn, &QPushButton::released, controller, &PTZController::StopSlot);
  connect(ui->rightBtn, &QPushButton::pressed, controller, &PTZController::TurnRightSlot);
  connect(ui->rightBtn, &QPushButton::released, controller, &PTZController::StopSlot);
  connect(ui->upBtn, &QPushButton::pressed, controller, &PTZController::TurnUpSlot);
  connect(ui->upBtn, &QPushButton::released, controller, &PTZController::StopSlot);
  connect(ui->downBtn, &QPushButton::pressed, controller, &PTZController::TurnDownSlot);
  connect(ui->downBtn, &QPushButton::released, controller, &PTZController::StopSlot);
  connect(ui->callPreset1Btn, &QPushButton::pressed, controller, &PTZController::CallPreset1Slot);
  connect(ui->callPreset1Btn, &QPushButton::released, controller, &PTZController::StopSlot);
  connect(ui->zoomInBtn, &QPushButton::pressed, controller, &PTZController::ZoomInSlot);
  connect(ui->zoomInBtn, &QPushButton::released, controller, &PTZController::StopSlot);
  connect(ui->zoomOutBtn, &QPushButton::pressed, controller, &PTZController::ZoomOutSlot);
  connect(ui->zoomOutBtn, &QPushButton::released, controller, &PTZController::StopSlot);
  connect(ui->focusNearBtn, &QPushButton::pressed, controller, &PTZController::FocusNearSlot);
  connect(ui->focusNearBtn, &QPushButton::released, controller, &PTZController::StopSlot);
  connect(ui->focusFarBtn, &QPushButton::pressed, controller, &PTZController::FocusFarSlot);
  connect(ui->focusFarBtn, &QPushButton::released, controller, &PTZController::StopSlot);
  connect(controller, &PTZController::OpenComSuccessSignal, this, &PtzTab::showComIsOpenSlot);
  connect(ui->openComBtn,SIGNAL(clicked()),this,SLOT(openCom()));
  connect(this, &PtzTab::openComSignal, controller, &PTZController::OpenComSlot);
  connect(ui->closeComBtn,SIGNAL(clicked()),this,SLOT(closeCom()));
  connect(this, &PtzTab::closeComSignal, controller, &PTZController::CloseComSlot);
  connect(this, &PtzTab::setCameraAddressLedSignal, controller, &PTZController::SetCameraAddressSlot);

  //监测摄像机地址的值变化
  connect(ui->camAdressLed,SIGNAL(textChanged(const QString &)),this,SLOT(setNumOfCam(const QString &)));

  loadConfig();
}

PtzTab::~PtzTab() {
  delete ui;
}

void PtzTab::loadConfig()  {
  RunningConfig *config = RunningConfig::Instance();
  
  ui->camAdressLed->setText(QString::number(config->CameraAddr()));

  if (!config->SerialportName().isEmpty()) {
    ui->numberOfCom->setCurrentText(config->SerialportName());
  }

  // TODO: check protocol is valid
  ui->protocolBtn->setCurrentIndex(config->Protocol());

  // TODO: check baud bits is valid
  ui->baudRate->setCurrentText(QString::number(config->BuadBits()));

  // TODO: check data bits is valid
  ui->dataBits->setCurrentText(QString::number(config->DataBits()));

  // TODO: check stop bits is valid
  ui->stopBits->setCurrentText(QString::number(config->StopBits()));

  switch (config->Parity()) {
    case QSerialPort::NoParity:
      ui->checkBits->setCurrentIndex(0);
      break;
    case QSerialPort::EvenParity:
      ui->checkBits->setCurrentIndex(2);
      break;
    case QSerialPort::OddParity:
      ui->checkBits->setCurrentIndex(1);
      break;
    default:
      // TODO: check parity is valid
      ui->checkBits->setCurrentIndex(0);
      break;
  }

  setUIStatus(PTZController::Instance()->IsComOpen());
}

void PtzTab::showComIsOpenSlot() {
  // loadConfig();
  setUIStatus(true);
}

//设置摄像机地址
void PtzTab::setNumOfCam(const QString &) {
  emit setCameraAddressLedSignal(ui->camAdressLed->text().toInt());
}

//打开串口
void PtzTab::openCom() {
  RunningConfig *config = RunningConfig::Instance();
  //设置串口号
  config->SerialportName(ui->numberOfCom->currentText());
  config->BuadBits(ui->baudRate->currentText().toInt());
  config->DataBits(ui->dataBits->currentText().toInt());
  config->StopBits(ui->stopBits->currentText().toInt());
  switch (ui->checkBits->currentIndex()) {
    case 0:
      config->Parity(QSerialPort::NoParity);
      break;
    case 1:
      config->Parity(QSerialPort::OddParity);
      break;
    case 2:
      config->Parity(QSerialPort::EvenParity);
      break;
    default:
      assert(false);
      break;
  }
  emit openComSignal();
  
  //设置打开串口后，按钮状态变化
  setUIStatus(true);
}

//关闭串口
void PtzTab::closeCom() {
  emit closeComSignal();
  //设置关闭串口后，按钮状态变化
  setUIStatus(false);
}

void PtzTab::setUIStatus( bool com_is_open ) {
  ui->numberOfCom->setEnabled(!com_is_open);
  ui->baudRate->setEnabled(!com_is_open);
  ui->dataBits->setEnabled(!com_is_open);
  ui->checkBits->setEnabled(!com_is_open);
  ui->stopBits->setEnabled(!com_is_open);
  ui->openComBtn->setEnabled(!com_is_open);
  ui->closeComBtn->setEnabled(com_is_open);
  ui->protocolBtn->setEnabled(com_is_open);
  ui->setingBtn->setEnabled(com_is_open);
  ui->callPreset1Btn->setEnabled(com_is_open);
  ui->leftBtn->setEnabled(com_is_open);
  ui->rightBtn->setEnabled(com_is_open);
  ui->upBtn->setEnabled(com_is_open);
  ui->downBtn->setEnabled(com_is_open);
  ui->openIrisBtn->setEnabled(com_is_open);
  ui->closeIrisBtn->setEnabled(com_is_open);
  ui->focusFarBtn->setEnabled(com_is_open);
  ui->focusNearBtn->setEnabled(com_is_open);
  ui->zoomInBtn->setEnabled(com_is_open);
  ui->zoomOutBtn->setEnabled(com_is_open);
}

void PtzTab::showComErrorSlot( QSerialPort::SerialPortError error ) {
  QString error_msg;
  switch (error) {
    case QSerialPort::NoError:
      // empty
      break;
    case QSerialPort::DeviceNotFoundError:
      error_msg = QString::fromLocal8Bit("未找到相关设备!");
      break;
    case QSerialPort::PermissionError:
      error_msg = QString::fromLocal8Bit("该设备已经由其他进程打开或权限不足!");
      break;
    case QSerialPort::OpenError:
      error_msg = QString::fromLocal8Bit("该设备已经打开!");
      break;
    case QSerialPort::ParityError:
      error_msg = QString::fromLocal8Bit("检测到校验错误!");
      break;
    case QSerialPort::FramingError:
      error_msg = QString::fromLocal8Bit("检测到组帧错误!");
      break;
    case QSerialPort::BreakConditionError:
      error_msg = QString::fromLocal8Bit("检测到中断条件!");
      break;
    case QSerialPort::WriteError:
      error_msg = QString::fromLocal8Bit("I/O写过程中检测到错误!");
      break;
    case QSerialPort::ReadError:
      error_msg = QString::fromLocal8Bit("I/O读过程中检测到错误!");
      break;
    case QSerialPort::ResourceError:
      error_msg = QString::fromLocal8Bit("资源不可用!");
      break;
    case QSerialPort::UnsupportedOperationError:
      error_msg = QString::fromLocal8Bit("设备不支持该项操作!");
    case QSerialPort::UnknownError:
      error_msg = QString::fromLocal8Bit("发生未知错误!");
      break;
    //case QSerialPort::TimeoutError: // for qt5.2
    //  break;
    //case QSerialPort::NoOpenError: // for qt5.2
    //  break;
    default:
      break;
  }
  if (!error_msg.isEmpty()) {
    QMessageBox::information(this, QString::fromLocal8Bit("错误"), error_msg);
  }
}

void PtzTab::showComSettingErrorSlot() {
  QMessageBox::information(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("设置参数有误!"));
}





