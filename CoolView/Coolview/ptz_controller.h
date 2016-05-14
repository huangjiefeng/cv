#ifndef PTZ_CONTROLLER_H
#define PTZ_CONTROLLER_H

#include <atomic>
#include <mutex>

#include <QObject>
#include <QtSerialPort/QSerialPort>

class PROTOCOL;

class PTZController : public QObject {
  Q_OBJECT
public:
  enum ProtocolType {
    PROTOCL_PELCO_D,
    PROTOCL_PELCO_P,
    PROTOCL_VISCA,
  };

  static PTZController* Instance() {
    PTZController *temp = instance_.load(std::memory_order_acquire);
    if (temp == nullptr) {
      std::lock_guard<std::mutex> guard(instance_mutex_);
      temp = instance_.load(std::memory_order_relaxed);
      if (temp == nullptr) {
        temp = new PTZController(nullptr);
        instance_.store(temp, std::memory_order_release);
      }
    }
    return temp;
  }

  bool IsComOpen() const;

Q_SIGNALS:
  void SerialPortErrorSignal(QSerialPort::SerialPortError error);
  // TODO: enum setting error
  void SerialPortSettingErrorSignal();
  void OpenComSuccessSignal();

public Q_SLOTS:
  void OpenComSlot();
  void CloseComSlot();
  void SetCameraAddressSlot(unsigned char addr);
  void SetPtzProtocolSlot(int protocol_type);
  void TurnLeftSlot();
  void TurnRightSlot();
  void TurnUpSlot();
  void TurnDownSlot();
  void FocusFarSlot();
  void FocusNearSlot();
  void ZoomInSlot();
  void ZoomOutSlot();
  void StopSlot();
  void CallPreset1Slot();

private:
  PTZController(QObject *parent);
  PTZController(const PTZController&);
  ~PTZController();

  PTZController& operator=(const PTZController&);

  QSerialPort::SerialPortError LazyOpenComWithoutLock(bool &try_open_com, bool &setting_error);
  void WriteComAndReleaseLock();

private:
  static std::mutex instance_mutex_;
  static std::atomic<PTZController*> instance_;

  std::mutex mutex_;
  PROTOCOL *protocol_;
  ProtocolType protocol_type_;
  int camera_addr_;
  QSerialPort serial_port_;
};

#endif // PTZ_CONTROLLER_H
