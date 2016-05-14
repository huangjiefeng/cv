#ifndef SCREEN_WIDGET_H
#define SCREEN_WIDGET_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QFrame>
#include <dbus/conferenceRoom/common/ConferenceRoomServiceCommon.h>

class QtScreenWidget : public QWidget
{
	Q_OBJECT
public:
	QtScreenWidget();
	~QtScreenWidget();

	void startDisplayScreen();
	void stopDisplayScreen();

	void setMediaState( const UiMediaState state );

protected:
	//virtual bool eventFilter(QObject*, QEvent*);
	virtual void paintEvent ( QPaintEvent * event ) ;
	virtual void resizeEvent ( QResizeEvent * event );

private:
	QString		_screenImagePath;

	QTimer*		_repaintTimer;
	QFrame*		mediaStateFrame;
};

#endif