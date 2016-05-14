#pragma  once

#ifdef SYSTEMDEVICEMANAGER_EXPORTS
#define SYSTEM_DEVICE_API  __declspec( dllexport )
#else
#define SYSTEM_DEVICE_API  __declspec( dllimport )
#endif

#include <util/StringList.h>
#include <string>
#include <vector>
using namespace std;

class SYSTEM_DEVICE_API SystemDeviceManager
{
public:
	static SystemDeviceManager*  getInstance()
	{
		static SystemDeviceManager deviceManager;
		return &deviceManager;
	}

	void getAudioInputDeviceList( vector<string>* deviceList );

	void getAudioOuputDeviceList( vector<string>* deviceList );

private:
	SystemDeviceManager(){}
	~SystemDeviceManager(){}
};