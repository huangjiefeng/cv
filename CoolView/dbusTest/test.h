#ifndef  TEST_H
#define  TEST_H

#include  <QtCore>
#include  <QTimer>

class Test : public QObject
{
	Q_OBJECT
public:
	Test();

	public slots:
		QString testStart();
		void changeTest();

signals:
		void stateChange(QString str);

private:
	QTimer *timer;
};
#endif  /*TEST_H*/
