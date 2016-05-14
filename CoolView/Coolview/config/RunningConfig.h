#ifndef RUNNING_CONFIG_H
#define RUNNING_CONFIG_H

#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtCore/QDataStream>

#include "DeviceManager/DeviceManager.h"

struct VCapSetting
{
	QString friendlyName;//human-readable name for the device
	QString devicePath;	 //unique for each video capture device
	QString crossBarName;
    QString crossBarDisplayName;
	QString crossBarType;
	int index;//每一个设备有个设备号，从1开始递增
	int width;
	int height;
	int fps;
	QString videoCodec;

	VCapSetting() :
		index(0),
		width(0),
		height(0),
		fps(0)
	{}
};
typedef std::vector<VCapSetting> VCapSettingsList;

class RunningConfig
{
public:
	static RunningConfig* Instance()
	{
		static RunningConfig config;
		return &config;
	}
	
    
	int VideoCaptureDeviceCount() const{return _videoCapDeviceCount;}
    void VideoCaptureDeviceCount(int val) { _videoCapDeviceCount = val; }

	VCapSetting VideoCaptureDevice(const int index);

	void VideoCaptureDevice(int index, const VCapSetting &val);

	QString AudioCaptureDevice() const { return _audioCaptureDevice; }
	void AudioCaptureDevice(QString val) 
	{ 
		_audioCaptureDevice = val;
	}

	QString AudioOutputDevice() const { return _audioOutputDevice; }
    void AudioOutputDevice(QString val) { _audioOutputDevice = val; }

    QString AudioInputType() const { return _audioInputType; }
    void AudioInputType(QString val) { _audioInputType = val; }

	QString AudioCodec() const { return _audioCodec; }
	void AudioCodec(QString val) { _audioCodec = val; }

	int AudioSampleRate() const { return _audioSampleRate; }
	void AudioSampleRate(int val);

	int AudioChannel() const { return _audioChannel; }
	void AudioChannel(int val);

	int AudioBitsPerSample() const { return _audioBitsPerSample; }
	void AudioBitsPerSample(int val) { _audioBitsPerSample = val; }

	void saveConfig();
	void loadConfig(); //读取所有设置
    void loadVideoConfig(); //重新读取和校验视频设置

    //由于部分VGA采集设备存在启动延迟，因此需要检查，以便多次尝试加载
    //调用此方法前必须已经调用过loadConfig
    bool IsConfiguredVideoDeviceValid();

    QString GlobalService() const { return _globalService; }
    void GlobalService(QString val) { _globalService = val; }

	const QString& getQosServerURI() const { return _qosServerURI; }
	void setQosServerURI(const QString&  val) { _qosServerURI = val; }

	bool isEnableRecvAutoResync() const { return _enableRecvAutoResync; }
	void EnableRecvAutoResync(bool val) { _enableRecvAutoResync = val; }

	// 已放弃使用
    bool isEnableAdaptiveCodeRate() const { return /*_enableAdaptiveCodeRate*/false; }
    void EnableAdaptiveCodeRate(bool val) { /*_enableAdaptiveCodeRate =*/ val; }

	bool isEnableKinect() const { return _enableKinect; }
	void EnableKinect(bool val) { _enableKinect = val; }

	bool isEnableSmallVideo() const;

	bool isEnableLocalRecord() const { return _enableLocalRecord; }
	void EnableLocalRecord(bool val) { _enableLocalRecord = val; }

	bool isEnableHttp() const { return _enableHttp; }
	void EnableHttp(bool val) { _enableHttp = val; }

    int MainScreen() const { return _mainScreen; }
    void MainScreen(int val) { _mainScreen = val; }

    bool EnableControlPanelBooter() const { return _enableControlPanelBooter; }
    void EnableControlPanelBooter(bool val) { _enableControlPanelBooter = val; }

    float GetCpuTempThreshold() const { return _cpuTempThreshold; }
    void SetCpuTempThreshold(float val) { _cpuTempThreshold = val; }

    // action: 0为noaction，1为beep，2为shutdown，3为restart
    int GetCpuTempAction() const { return _cpuTempAction; }
    void SetCpuTempAction(int val) { _cpuTempAction = val; }

    float GetMainboardTempThreshold() const { return _mainboardTempThreshold; }
    void SetMainboardTempThreshold(float val) { _mainboardTempThreshold = val; }

    // action: 0为noaction，1为beep，2为shutdown，3为restart
    int GetMainboardTempAction() const { return _mainboardTempAction; }
    void SetMainboardTempAction(int val) { _mainboardTempAction = val; }

    QString SerialportName() const { return _serialportName; }
    void SerialportName(const QString &val) { _serialportName = val; }
    
    int Protocol() const { return _protocol; }
    void Protocol(int val) { _protocol = val; }
    
    int StopBits() const { return _stopBits; }
    void StopBits(int val) { _stopBits = val; }
    
    int CameraAddr() const { return _cameraAddr; }
    void CameraAddr(int val) { _cameraAddr = val; }
    
    int BuadBits() const { return _buadBits; }
    void BuadBits(int val) { _buadBits = val; }
    
    int DataBits() const { return _dataBits; }
    void DataBits(int val) { _dataBits = val; }

    int Parity() const { return _parity; }
    void Parity(int val) { _parity = val; }

private:
	RunningConfig();
	~RunningConfig(){};

	// Audio

	QString			_configFilePath;

	QString			_audioCaptureDevice;

	QString			_audioOutputDevice;

    QString         _audioInputType; //存储字符串而不是数值，防止以后内部排序改变导致选项错误

	QString			_audioCodec;

	int             _audioSampleRate;

	int             _audioChannel;

	int             _audioBitsPerSample;

	// Video

	int              _videoCapDeviceCount;

	VCapSettingsList _videoCapSettings;

	// global

	QString			_globalService;

	QString	  _qosServerURI;

	bool _enableRecvAutoResync;

	bool _enableKinect;

	bool _enableSmallVideo;

	bool _enableLocalRecord; // 是否录制本地视频

	bool _enableHttp;


    // UI settings
    int _mainScreen;
    bool _enableControlPanelBooter;

    // Monitor settings
    float _cpuTempThreshold;
    int _cpuTempAction;
    float _mainboardTempThreshold;
    int _mainboardTempAction;

    // ptz settings
    QString _serialportName;
    int _protocol;
    int _cameraAddr;
    int _buadBits;
    int _dataBits;
    int _stopBits;
    int _parity;
};

#endif