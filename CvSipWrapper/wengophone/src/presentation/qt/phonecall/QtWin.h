#pragma once
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QWidget>
#include "QtVideo.h"
#include "QtVideoQt.h"

class QtVideoQt;

class QtWin:public QWidget
{
public:
	QtWin(QtVideoQt *parent);
	QtWin(void);
	~QtWin(void);

protected:
	virtual bool eventFilter(QObject*, QEvent*);
	virtual void resizeEvent ( QResizeEvent * event );

private:
	QtVideoQt *widget;
};
