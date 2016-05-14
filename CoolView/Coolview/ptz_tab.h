#ifndef ptz_tab_H
#define ptz_tab_H

#include <QMainWindow>
#include <QtSerialPort\QtSerialPort>//串口头文件
#include <QLineEdit>//摄像头地址编辑文本头文件

namespace Ui {
class PtzTab;
}

class PtzTab : public QMainWindow
{
    Q_OBJECT
public:
    explicit PtzTab(QWidget *parent = 0);
    ~PtzTab();

Q_SIGNALS:
    void setCameraAddressLedSignal(unsigned char index);
    void setPtzProtocolSignal(int protocol_type);
    void openComSignal();
    void closeComSignal();

private Q_SLOTS:
	void setNumOfCam(const QString &);
	
	void openCom();//打开串口
	void closeCom();//关闭串口

    void showComIsOpenSlot();
    void showComErrorSlot(QSerialPort::SerialPortError error);
    void showComSettingErrorSlot();

private:
    void setUIStatus(bool com_is_open);
    void loadConfig();

private:
    Ui::PtzTab *ui;
};

#endif // ptz_tab_H
