#pragma once
#include <QtGui/QtGui>
#include "../QtWengoPhone.h"//Add By LZY 2010-09-26

namespace Ui { class GlobalServiceDialog; }

class QtGlobalServiceSetting:public QDialog
{
	Q_OBJECT
public:
	QtGlobalServiceSetting(CWengoPhone * cWengoPhone);
	~QtGlobalServiceSetting(void);
	void readConfig();

private Q_SLOTS:
	void saveConfig();

private:

	Ui::GlobalServiceDialog* _ui;
	QtWengoPhone * _qtWengoPhone;//Add By LZY 2010-09-27
};
