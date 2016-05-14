#ifndef CONFIGURATION_FILE_H
#define CONFIGURATION_FILE_H

#include <QtCore/QtCore>
#include <QtXml/QtXml>
#include <json/json.h>

/**
 * @brief 关于软件配置的文件
 */
class ConfigurationFile{
public:
	ConfigurationFile();
	~ConfigurationFile();

	/**
	 * @brief 初始化配置表
	 */
	void initilizeConfiguration( const QString& fileName );

	/**
	 * @brief 初始化配置表里的参数
	 */
	void initilizeCameraList( QDomElement& element );
	void initilizeSpeakerList( QDomElement speakerListNode );
	void initilizeMicList( QDomElement micListNode );
	void initilizeIPList( QDomElement ipListNode );

	/**
	 * @brief 以JSON格式获取指定的设备参数
	 */
	Json::Value queryCameraConfigJSON( );
	Json::Value querySpeakerConfigJSON();
	Json::Value queryIPConfigJSON();
	Json::Value queryMicConfigJSON();


	/**
	 * @brief 修改配置信息
	 */
	void saveCameraConfig( const QString &camera_name, const QString &crossbar_name, const QString &crossbar_type, int video_width, int video_height );

private:
	/**
	 * @brief 更新parent元素节点下第一个TAG节点的值
	 */
	void updateElement( QDomElement& parent, const QString& tag, const QString& nodeValue );

	/**
	 * @brief 加载COM文件
	 */
	void loadDOM( QDomDocument* doc , const QString& fileName );
	/**
	 * @brief 保存DOM文件
	 */
	void saveDOM( QDomDocument& doc, const QString& fileName );

public:
	QString _CONFIG_FILE_NAME;
	QString _CURRENT_CONFIG_FILE_NAME;

	//全局配置信息
	QDomDocument _configDoc;
	QDomElement	 _configRoot;

	//终端已选的配置信息
	QDomDocument _currentConfigDoc;
	QDomElement  _currentConfigRoot;
};

#endif