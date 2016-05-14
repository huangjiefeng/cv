#include "stdafx.h"
#include ".\qtwin.h"

QtWin::QtWin(void)
{
	installEventFilter(this);
}

QtWin::QtWin(QtVideoQt *parent)
{
	widget = parent;
	installEventFilter(this);
	setWindowTitle(parent->getUserName());
}

QtWin::~QtWin(void)
{
}

bool QtWin::eventFilter(QObject * object, QEvent * event) {
	if (event->type() == QEvent::Close) {
		if (widget!=NULL)
		{
			widget->closeWin();
		}
	}else if (event->type() == QEvent::MouseButtonDblClick) {
		widget->detach_fullscreen();
		return true;
	} 

	return false;
}

void QtWin::resizeEvent ( QResizeEvent * event )
{
	widget->detach_resize();
}
