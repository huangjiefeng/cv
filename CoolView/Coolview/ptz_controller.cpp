#include "ptz_controller.h"

#include <QtSerialPort/QSerialPortInfo>

#include "PROTOCOL.h"
#include "config/RunningConfig.h"

std::mutex PTZController::instance_mutex_;
std::atomic<PTZController*> PTZController::instance_;

namespace {
std::atomic_flag lock_ = ATOMIC_FLAG_INIT;
}

PTZController::PTZController(QObject *parent)
    : QObject(parent), camera_addr_(0), protocol_(nullptr) {
  SetPtzProtocolSlot(RunningConfig::Instance()->Protocol());
  qRegisterMetaType<QSerialPort::SerialPortError>("SerialPortError");
}

PTZController::~PTZController() {
  if (serial_port_.isOpen()) {
    serial_port_.close();
  }
  if (protocol_) {
    delete protocol_;
  }
}

void PTZController::SetPtzProtocolSlot( int protocol_type ) {
  std::lock_guard<std::mutex> lock(mutex_);
  while (::lock_.test_and_set(std::memory_order_acquire)) {};
  if (protocol_ && protocol_type_ != protocol_type) {
    delete protocol_;
    protocol_ = nullptr;
  }
  switch (protocol_type) {
    case PROTOCL_PELCO_D:
      protocol_ = new PELCO_D;
      break;
    case PROTOCL_PELCO_P:
      protocol_ = new PELCO_P;
      break;
    case PROTOCL_VISCA:
      protocol_ = new VISCA;
      break;
    default: // unknown protocol
      assert(false);
      break;
  }
  protocol_->command[1] = camera_addr_;
}

void PTZController::OpenComSlot() {
  bool try_open_com = false;
  bool setting_error = false;
  QSerialPort::SerialPortError error = QSerialPort::NoError;
  {
    std::lock_guard<std::mutex> lock(mutex_);
    if (serial_port_.isOpen()) {
      serial_port_.close(); 
    }
    error = LazyOpenComWithoutLock(try_open_com, setting_error); 
  }
  // note: emit signal at last in case of deadlock
  if (setting_error) {
    emit SerialPortSettingErrorSignal();
  } else {
    emit SerialPortErrorSignal(error);
  }
}

void PTZController::SetCameraAddressSlot( unsigned char addr ) {
  std::lock_guard<std::mutex> lock(mutex_);
  camera_addr_ = addr;
  protocol_->command[1] = addr;
}

QSerialPort::SerialPortError PTZController::LazyOpenComWithoutLock(
    bool &try_open_com, bool &setting_error) {
  if (serial_port_.isOpen()) { 
    try_open_com = false; 
    return QSerialPort::NoError; 
  }
  try_open_com = true;
  RunningConfig *config = RunningConfig::Instance();
  serial_port_.setPortName(config->SerialportName());
  bool no_error = 
    serial_port_.open(QIODevice::WriteOnly) &&
    serial_port_.setBaudRate(config->BuadBits()) && 
    serial_port_.setDataBits(static_cast<QSerialPort::DataBits>(config->DataBits())) &&
    serial_port_.setParity(static_cast<QSerialPort::Parity>(config->Parity())) &&
    serial_port_.setStopBits(static_cast<QSerialPort::StopBits>(config->StopBits())) &&
    serial_port_.setFlowControl(QSerialPort::NoFlowControl);

  setting_error = !no_error && serial_port_.error() == QSerialPort::NoError;
  return serial_port_.error();
}

void PTZController::CloseComSlot() {
  std::lock_guard<std::mutex> lock(mutex_);
  if (serial_port_.isOpen()) { 
    serial_port_.close(); 
  }
}

bool PTZController::IsComOpen() const {
  return serial_port_.isOpen();
}

void PTZController::TurnLeftSlot() {
  mutex_.lock();
  protocol_->left();
  WriteComAndReleaseLock();
}

void PTZController::TurnRightSlot() {
  mutex_.lock();
  protocol_->right();
  WriteComAndReleaseLock();
}

void PTZController::TurnUpSlot() {
  mutex_.lock();
  protocol_->up();
  WriteComAndReleaseLock();
}

void PTZController::TurnDownSlot() {
  mutex_.lock();
  protocol_->down();
  WriteComAndReleaseLock();
}

void PTZController::FocusFarSlot() {
  mutex_.lock();
  protocol_->focusFar();
  WriteComAndReleaseLock();
}

void PTZController::FocusNearSlot() {
  mutex_.lock();
  protocol_->focusNear();
  WriteComAndReleaseLock();
}

void PTZController::ZoomInSlot() {
  mutex_.lock();
  protocol_->zoomIn();
  WriteComAndReleaseLock();
}

void PTZController::ZoomOutSlot() {
  mutex_.lock();
  protocol_->zoomOut();
  WriteComAndReleaseLock();
}

void PTZController::CallPreset1Slot() {
  mutex_.lock();
  protocol_->callPreset1();
  WriteComAndReleaseLock();
}

void PTZController::StopSlot() {
  mutex_.lock();
  protocol_->stop();
  WriteComAndReleaseLock();
}

void PTZController::WriteComAndReleaseLock() {
  bool try_open_com = false;
  bool setting_error = false;
  auto error = LazyOpenComWithoutLock(try_open_com, setting_error);
  if (error == QSerialPort::NoError) {
    serial_port_.write(protocol_->command);
  }
  mutex_.unlock();

  if (try_open_com && error == QSerialPort::NoError) {
    emit OpenComSuccessSignal();
  }
}


