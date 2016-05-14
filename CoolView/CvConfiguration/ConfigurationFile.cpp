#include "ConfigurationFile.h"
#include <QtCore/QCoreApplication>
#include <DeviceManager/DeviceManager.h>
#include <util/network/NetworkUtil.h>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDesktopWidget>


/**
 * @brief 更新parent元素节点下第一个TAG节点的值
 */
void ConfigurationFile::updateElement( QDomElement& parent, const QString& tag, const QString& nodeValue )
{
	QDomDocument doc;
	QDomElement newNode = doc.createElement(tag);
	newNode.appendChild(doc.createTextNode(nodeValue) );

	QDomElement oldNode = parent.firstChildElement(tag);
	if( oldNode.isNull() )
		return;

	parent.replaceChild( newNode , oldNode );	//调用节点的replaceChild方法实现修改功能
}


ConfigurationFile::ConfigurationFile()
{
	QString appDirPath = QCoreApplication::applicationDirPath();
	appDirPath.replace("/" , "\\" );
	QDir appDir(appDirPath);
	if( !appDir.exists("config") )
		appDir.mkdir("config");
	_CONFIG_FILE_NAME = appDirPath + "\\config\\configuration.xml";
	_CURRENT_CONFIG_FILE_NAME = appDirPath + "\\config\\currentconfig.xml" ;

	//重新获取终端的所有网络和硬件参数
	initilizeConfiguration(_CONFIG_FILE_NAME);

}

ConfigurationFile::~ConfigurationFile()
{
}


void ConfigurationFile::initilizeConfiguration( const QString& fileName )
{


	QDomProcessingInstruction instruction = _configDoc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");

	_configDoc.appendChild(instruction);

	_configRoot = _configDoc.createElement("configuration");
	_configDoc.appendChild(_configRoot);

	QDomElement cameraListNode = _configDoc.createElement("cameraList");
	initilizeCameraList(cameraListNode);
	_configRoot.appendChild(cameraListNode);

	QDomElement speakerListNode = _configDoc.createElement("speakerList");
	initilizeSpeakerList(speakerListNode);
	_configRoot.appendChild(speakerListNode);

	QDomElement micListNode = _configDoc.createElement("micList");
	initilizeMicList(micListNode);
	_configRoot.appendChild(micListNode);

	QDomElement ipListNode = _configDoc.createElement("ipList");
	initilizeIPList(ipListNode);
	_configRoot.appendChild(ipListNode);


	saveDOM( _configDoc, _CONFIG_FILE_NAME );
}


void ConfigurationFile::initilizeCameraList( QDomElement& cameraListNode )
{
	QDomDocument doc;

	//读取用户设备参数
	QStringList devices;
	vector<string> stringDevices;
	DeviceManager::getInstatnce()->GetVideoCaptureDevices( &stringDevices );
	for( int i=0 ; i<stringDevices.size(); i++ )
	{
		QString cameraName = QString::fromStdString(stringDevices.at(i));

		QDomElement cameraNode = doc.createElement("camera");
		cameraListNode.appendChild(cameraNode);

		QDomElement nameNode = doc.createElement("name");
		cameraNode.appendChild(nameNode);
		nameNode.appendChild( doc.createTextNode( cameraName ));

		QDomElement sizeListNode = doc.createElement("sizeList");
		cameraNode.appendChild(sizeListNode);

		//设备属性
		vector<string> videoSizeList;
		DeviceManager::getInstatnce()->getVideoDeviceMediaType( cameraName.toStdString(), &videoSizeList );
		for( int j=0 ; j<videoSizeList.size();j++ )
		{
			QDomElement sizeNode = doc.createElement("size");
			sizeListNode.appendChild( sizeNode );
			sizeNode.appendChild(doc.createTextNode(videoSizeList.at(j).c_str()));
		}

		if( !cameraName.contains("webcam", Qt::CaseInsensitive)&&
			!cameraName.contains("VCam" , Qt::CaseInsensitive) )
		{
			//排除网络摄像头和虚拟摄像头，则剩下的可以认为是采集卡
			vector<string> crossbarList;
			DeviceManager::getInstatnce()->getCrossbarDeviceList( cameraName.toLocal8Bit().constData() , &crossbarList );

			QDomElement crossbarListNode = doc.createElement("crossbarList");
			cameraNode.appendChild(crossbarListNode);
			for( int crossbarIndex=0 ; crossbarIndex<crossbarList.size(); crossbarIndex++ )
			{
				QString crossbarName = QString::fromStdString(crossbarList.at(crossbarIndex));
				QDomElement crossbarNode = doc.createElement("crossbar");
				crossbarListNode.appendChild(crossbarNode);

				QDomElement crossbarNameNode = doc.createElement("name");
				crossbarNode.appendChild(crossbarNameNode);
				crossbarNameNode.appendChild(doc.createTextNode(crossbarName) );

				//crossbar的类型列表
				QDomElement crossbarTypeListNode = doc.createElement("typeList");
				crossbarNode.appendChild(crossbarTypeListNode);

				vector<string> crossbarTypeList;
				DeviceManager::getInstatnce()->getCrossbarInputType( crossbarName.toStdString(), &crossbarTypeList );
				for( int typeIndex=0 ; typeIndex<crossbarTypeList.size() ; typeIndex++ )
				{
					QDomElement crossbarTypeNode = doc.createElement("type" );
					crossbarTypeListNode.appendChild( crossbarTypeNode);
					crossbarTypeNode.appendChild(doc.createTextNode(crossbarTypeList.at(typeIndex).c_str()) );
				}
			}
		}
		
	}
}


void ConfigurationFile::initilizeSpeakerList( QDomElement speakerListNode )
{
	QDomDocument doc;
	vector<string> audioOutputList;
	DeviceManager::getInstatnce()->GetAudioOutputDevices( &audioOutputList );
	int volumnIndex = DeviceManager::getInstatnce()->getAudioOutputVolumn();
	for( int i=0; i<audioOutputList.size(); i++ )
	{
		QString speakerName = QString::fromStdString( audioOutputList.at(i) );
		QDomElement speakerNode = doc.createElement("speaker");
		speakerListNode.appendChild(speakerNode);

		QDomElement nameNode = doc.createElement("name");
		speakerNode.appendChild(nameNode);
		nameNode.appendChild(doc.createTextNode(speakerName) );

		QDomElement volumnNode = doc.createElement("volumn");
		speakerNode.appendChild(volumnNode);
		volumnNode.appendChild( doc.createTextNode(QString::number(volumnIndex) ) );
	}
}



void ConfigurationFile::initilizeMicList( QDomElement micListNode )
{
	QDomDocument doc;
	vector<string> audioInputList;
	DeviceManager::getInstatnce()->GetAudioCaptureDevices( &audioInputList );

	for( int i=0; i<audioInputList.size(); i++ )
	{
		QDomElement micNode = doc.createElement("mic");
		micListNode.appendChild(micNode );

		QString micName = QString::fromStdString(audioInputList.at(i));
		QDomElement nameNode = doc.createElement("name");
		micNode.appendChild(nameNode);
		nameNode.appendChild(doc.createTextNode(micName));

		int inputVolumnIndex = DeviceManager::getInstatnce()->getAudioInputVolumn( micName.toStdString() );
		QDomElement volumnNode = doc.createElement("volumn");
		micNode.appendChild(volumnNode);
		volumnNode.appendChild(doc.createTextNode(QString::number(inputVolumnIndex) ) );
	}
}

void ConfigurationFile::initilizeIPList( QDomElement ipListNode )
{
	QDomDocument doc;
	std::vector<std::string> ipList = NetworkUtil::getHostIPList();
	for( int i=0; i<ipList.size(); i++ )
	{
		QDomElement ipNode = doc.createElement("ip");
		ipListNode.appendChild(ipNode);
		ipNode.appendChild( doc.createTextNode(ipList.at(i).c_str() ) );
	}
}

Json::Value ConfigurationFile::queryCameraConfigJSON()
{
	Json::Value root;
	QDomElement cameraNode;
	QDomElement cameraListNode = _configRoot.firstChildElement("cameraList");
	if( cameraListNode.isNull())
		return root;
	else
	{
		cameraNode = cameraListNode.firstChildElement("camera");
	}

	Json::Value jCameraList;
	while( !cameraNode.isNull() )
	{
		
		Json::Value jCamera;
		QDomElement nameNode = cameraNode.firstChildElement("name");
		jCamera["name"] = nameNode.text().toStdString();
		QDomElement sizeListNode = cameraNode.firstChildElement("sizeList");
		if( !sizeListNode.isNull() )
		{
			Json::Value jSizeList;
			QDomElement sizeNode = sizeListNode.firstChildElement("size");
			while( !sizeNode.isNull() )
			{
				jSizeList.append(sizeNode.text().toStdString());
				sizeNode = sizeNode.nextSiblingElement("size");
			}
			jCamera["sizeList"] = jSizeList;
		}
		jCameraList.append(jCamera);
		cameraNode = cameraNode.nextSiblingElement("camera");
	}
	root["cameraList"] = jCameraList;

	//std::string out = root.toStyledString();
	//cout << out <<endl;
	return root;
}

Json::Value ConfigurationFile::querySpeakerConfigJSON()
{
	Json::Value root;
	QDomElement speakerNode;
	QDomElement speakerListNode = _configRoot.firstChildElement("speakerList");
	if( speakerListNode.isNull())
		return root;
	else
	{
		speakerNode = speakerListNode.firstChildElement("speaker");
	}

	Json::Value jSpeakerList;
	while( !speakerNode.isNull() )
	{

		Json::Value jSpeaker;
		QDomElement nameNode = speakerNode.firstChildElement("name");
		jSpeaker["name"] = nameNode.text().toStdString();

		QDomElement volumnNode = speakerNode.firstChildElement("volumn");
		jSpeaker["volumn"] = volumnNode.text().toStdString();
		
		jSpeakerList.append(jSpeaker);
		speakerNode = speakerNode.nextSiblingElement("speaker");
	}
	root["speakerList"] = jSpeakerList;

	return root;
}

Json::Value ConfigurationFile::queryIPConfigJSON()
{
	Json::Value root;
	QDomElement ipNode;
	QDomElement ipListNode = _configRoot.firstChildElement("ipList");
	if( ipListNode.isNull())
		return root;
	else
	{
		ipNode = ipListNode.firstChildElement("ip");
	}

	Json::Value jIPList;
	while( !ipNode.isNull() )
	{
		jIPList.append(ipNode.text().toStdString());
		ipNode = ipNode.nextSiblingElement("ip");
	}
	root["ipList"] = jIPList;

	return root;
}

Json::Value ConfigurationFile::queryMicConfigJSON()
{
	Json::Value root;
	QDomElement micNode;
	QDomElement micListNode = _configRoot.firstChildElement("micList");
	if( micListNode.isNull())
		return root;
	else
	{
		micNode = micListNode.firstChildElement("mic");
	}

	Json::Value jMicList;
	while( !micNode.isNull() )
	{

		Json::Value jMic;
		QDomElement nameNode = micNode.firstChildElement("name");
		jMic["name"] = nameNode.text().toStdString();

		QDomElement volumnNode = micNode.firstChildElement("volumn");
		jMic["volumn"] = volumnNode.text().toStdString();

		jMicList.append(jMic);
		micNode = micNode.nextSiblingElement("mic");
	}
	root["micList"] = jMicList;

	return root;
}

void ConfigurationFile::saveCameraConfig( const QString &camera_name, const QString &crossbar_name, const QString &crossbar_type, int video_width, int video_height )
{
	//获取终端保存的配置信息
	QDomDocument doc;
	loadDOM( &doc, _CURRENT_CONFIG_FILE_NAME);
	QDomElement root = doc.firstChildElement("configuration");

	QDomElement cameraNode = root.firstChildElement("camera");
	QDomElement nameNode = cameraNode.firstChildElement("name");
	if( !nameNode.isNull() )
	{
		updateElement( cameraNode, "name", camera_name);
	}else
	{
		cout<<"Can't find name node for camera"<<endl;
	}

	QDomElement sizeNode = cameraNode.firstChildElement("size");
	if( !sizeNode.isNull() )
	{
		QString size = QString::number(video_width) + "x" + QString::number(video_height);
		updateElement(cameraNode, "size", size );
	}else
	{
		cout<<"Can't find size node for camera"<<endl;
	}

	QDomElement crossbarNameNode = cameraNode.firstChildElement("crossbarName");
	if( !crossbarNameNode.isNull() )
	{
		updateElement(cameraNode, "crossbarName",crossbar_name );
	}else
	{
		cout<<"Can't find crossbar name node for camera"<<endl;
	}

	QDomElement crossbarTypeNode = cameraNode.firstChildElement("crossbarType");
	if( !crossbarTypeNode.isNull() )
	{
		updateElement(cameraNode, "crossbarType", crossbar_type );
	}else
	{
		cout<<"Can't find crossbar type node for camera"<<endl;
	}

	saveDOM( doc, _CURRENT_CONFIG_FILE_NAME);
}

void ConfigurationFile::saveDOM( QDomDocument& doc, const QString& fileName )
{
	QFile file( fileName );

	file.open(QIODevice::WriteOnly);
	QTextStream out(&file);
	out.setCodec("UTF-8");	
	doc.save(out,4,QDomNode::EncodingFromTextStream);
	file.close();
}

void ConfigurationFile::loadDOM( QDomDocument* doc , const QString& fileName )
{
	QFile file(fileName);
	file.open(QIODevice::ReadOnly);
	doc->setContent( &file);
	file.close();
}