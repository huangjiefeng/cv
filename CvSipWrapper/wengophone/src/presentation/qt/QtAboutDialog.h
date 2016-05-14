/************************************************************************/
/* Coolview 3.0 ½çÃæÔªËØ
 * zhenHua.sun 2010-08-17
 */
/************************************************************************/

#pragma once
#include <QtGui/QtGui>

#include <ui_AboutDialog.h>

class QtToolWidget;
class QtAboutDialog:public QDialog
{
	friend class QtToolWidget;
public:
	QtAboutDialog(void);
	~QtAboutDialog(void);

private:

	Ui::AboutDialog * _ui;
};
