#pragma once
#include <QObject>
class serviceTest :
	public QObject
{
	  Q_OBJECT
public:
	serviceTest(void);
	~serviceTest(void);

public Q_SLOTS:
	int DbusFunc();
};

