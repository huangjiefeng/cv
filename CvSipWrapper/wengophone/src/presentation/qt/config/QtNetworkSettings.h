#pragma once
#include <QtGui/QtGui>

namespace Ui { class NetWorkSetting; }

class QtNetworkSettings:public QDialog
{
	Q_OBJECT
public:
	QtNetworkSettings(void);
	~QtNetworkSettings(void);
	void readConfig();

private Q_SLOTS:
	void saveConfig();
	void setIPList(int index);

private:

	Ui::NetWorkSetting* _ui;
};
