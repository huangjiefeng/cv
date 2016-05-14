#pragma once

#ifndef QTCONFERENCEINVITE_H
#define QTCONFERENCEINVITE_H

#include "ui_ConferenceInvite.h"
#include <QtGui/QDialog>

class IPhoneLine;
class CWengoPhone;
class QMenu;
class QPoint;



class QtConferenceInvite: public QDialog {
Q_OBJECT

public:

	/**
	* Constructor.
	*/
	QtConferenceInvite(QWidget * parent, CWengoPhone & cWengoPhone, int &callId,bool startConference = true);

	/**
	* Destructor.
	*/
	virtual ~QtConferenceInvite();

	void setFirstPeer(std::string peer);

	QString getSecondPeer();

private Q_SLOTS:

	/**
	* The user has clicked the start button.
	*/
	void startClicked();

	void peer1ToolButtonClicked();

	void updateLineEdit(QAction * action);

private:

	bool _startConference;

	void populateMenus();

	void showMenu(QPoint point);

	IPhoneLine * _phoneLine;

	QMenu * _menu;

	QMenu * _wengoMenu;

	QMenu * _sipMenu;

	QMenu * _landLineMenu;

	QMenu * _mobileMenu;

	CWengoPhone & _cWengoPhone;

	int _currentPeer;

	Ui::ConferenceInvite * _ui;

	int _callId;
};
#endif	//QTCONFERENCEINVITE_H