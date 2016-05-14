#pragma once
#include "qobject.h"
class KinectTest :
	public QObject
{
public:
	KinectTest(void);
	~KinectTest(void);
	template<class Interface>
	inline void SafeRelease( Interface *& pInterfaceToRelease )
	{
		if( pInterfaceToRelease != NULL ){
			pInterfaceToRelease->Release();
			pInterfaceToRelease = NULL;
		}
	}

	int start();
};

