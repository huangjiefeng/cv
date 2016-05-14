/*
 * WengoPhone, a voice over Internet phone
 * Copyright (C) 2004-2007  Wengo
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef OWQTBROWSER_H
#define OWQTBROWSER_H

#include <owbrowser/owbrowserdll.h>
#include <owbrowser/OWBrowser.h>

#include <util/Event.h>
#include <cutil/global.h>

#include <QtCore/QObject>

class QWidget;
class QTextBrowser;
class QVBoxLayout;
class QUrl;

#if (defined OS_WINDOWS) && (QT_EDITION == QT_EDITION_DESKTOP)
	class QAxWidget;
#endif

/**
 * Qt implementation of OWBrowser.
 *
 * Implemented with:
 *  - QTextBrowser: local url
 *  - Microsoft Web Browser: local & remote url
 *
 * @author Mathieu Stute
 */
class OWBROWSER_API QtBrowser : public QObject, public OWBrowser {
	Q_OBJECT
public:

	/**
	 * Browser mode.
	 */
	enum BrowserMode {

		/** Qt browser mode. */
		BrowserModeQt,

		/** Internet Explorer (via ActiveX) browser mode. */
		BrowserModeIE
	};

	/**
	 * Default constructor.
	 *
	 * @param parent the parent widget
	 * @param mode browser mode to use (IE or Qt)
	 */
	QtBrowser(QWidget * parent, BrowserMode mode = BrowserModeIE);

	~QtBrowser();

	void setUrl(const std::string & url, const std::string & data = "", bool postMethod = true);

	bool setMode(BrowserMode mode);

	void show();

	void * getWidget() const;

	std::string getUrl() const;

	void backward();

	void forward();

private Q_SLOTS:

	/**
	 * Slot called before loading a page in a Microsoft Web Browser.
	 */
	void beforeNavigate(const QString &, int, const QString &, const QVariant &, const QString &, bool &);

	/**
	 * Slot called before loading a page in a QTextBrowser.
	 */
	void beforeNavigate(const QUrl & link);

private:

	/**
	 * Current url.
	 */
	QString _url;

	/**
	 * Inits the browser (IE or Qt).
	 */
	void initBrowser();

	void setPost(const std::string & url, const std::string & postData);

	/**
	 * BrowserMode: Qt or IE.
	 */
	BrowserMode _mode;

	/**
	 * Main widget.
	 */
	QWidget * _browserWidget;

#if (defined OS_WINDOWS) && (QT_EDITION == QT_EDITION_DESKTOP)
	/**
	 * Microsoft Web Browser ActiveX.
	 */
	QAxWidget * _ieBrowser;
#endif

	/**
	 * Qt html viewer.
	 */
	QTextBrowser * _qtBrowser;

	/**
	 * Widget layout.
	 */
	QVBoxLayout * _layout;
};

#endif	//OWQTBROWSER_H
