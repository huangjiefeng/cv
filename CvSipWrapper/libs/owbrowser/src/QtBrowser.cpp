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

#include <owbrowser/QtBrowser.h>

#include <util/Logger.h>
#include <util/SafeDelete.h>

#include <qtutil/SafeConnect.h>

#include <QtGui/QtGui>

#if (defined OS_WINDOWS) && (QT_EDITION == QT_EDITION_DESKTOP)
	#include <ActiveQt/QAxWidget>

	#include <windows.h>
	#define INITGUID
	#include <initguid.h>
	#include <exdisp.h>
	#include <memory.h>
#endif

#include <iostream>
using namespace std;

QtBrowser::QtBrowser(QWidget * parent, BrowserMode mode)
	: QObject(parent) {

	_browserWidget = new QWidget(parent);

#if (defined OS_WINDOWS) && (QT_EDITION == QT_EDITION_DESKTOP)
	_ieBrowser = NULL;
#endif
	_qtBrowser = NULL;

	_layout = new QVBoxLayout(_browserWidget);
	_layout->setMargin(0);
	_layout->setSpacing(0);

	setMode(mode);
}

QtBrowser::~QtBrowser() {
#if (defined OS_WINDOWS) && (QT_EDITION == QT_EDITION_DESKTOP)
		//Clean ie browser
		if (_ieBrowser) {
			_layout->removeWidget(_ieBrowser);
			OWSAFE_DELETE(_ieBrowser);
		}
#endif
}

void QtBrowser::setUrl(const std::string & url, const std::string & data, bool postMethod) {
	_url = QString::fromStdString(url);
	if (_mode == BrowserModeIE) {
#if (defined OS_WINDOWS) && (QT_EDITION == QT_EDITION_DESKTOP)
		if (!postMethod || data.empty()) {
			if (data.empty()) {
				_ieBrowser->dynamicCall("Navigate(const QString&)", _url);
			} else {
				_ieBrowser->dynamicCall("Navigate(const QString&)", QString::fromStdString(url + "?" + data));
			}
		} else {
			setPost(url, data);
		}
#endif
	} else {
		_qtBrowser->setSource(_url);
	}
}

void QtBrowser::show() {
	_browserWidget->show();
}

void * QtBrowser::getWidget() const {
	return (void *) _browserWidget;
}

std::string QtBrowser::getUrl() const {
	return _url.toStdString();
}

void QtBrowser::backward() {
	LOG_WARN("not yet implemented");
}

void QtBrowser::forward() {
	LOG_WARN("not yet implemented");
}

void QtBrowser::beforeNavigate(const QUrl & link) {
	urlClickedEvent(link.toString().toStdString());
}

void QtBrowser::beforeNavigate(const QString & url, int, const QString &, const QVariant &, const QString &, bool &) {
	urlClickedEvent(url.toStdString());
}

bool QtBrowser::setMode(BrowserMode mode) {
	//BrowserModeIE is only available on Windows & Qt commercial
#if (defined OS_WINDOWS) && (QT_EDITION != QT_EDITION_DESKTOP)
	if (mode == BrowserModeIE) {
		_mode = BrowserModeQt;
		initBrowser();
		return false;
	}
#endif
	_mode = mode;
	initBrowser();
	return true;
}

void QtBrowser::initBrowser() {
#if (defined OS_WINDOWS) && (QT_EDITION != QT_EDITION_DESKTOP)
	if (_mode == BrowserModeIE) {
		LOG_WARN("BrowserModeIE not allowed");
	}
#endif

	if (_mode == BrowserModeQt) {
#if (defined OS_WINDOWS) && (QT_EDITION == QT_EDITION_DESKTOP)
		//Clean ie browser
		if (_ieBrowser) {
			_layout->removeWidget(_ieBrowser);
			OWSAFE_DELETE(_ieBrowser);
		}
#endif
		//Init Qt browser
		_qtBrowser = new QTextBrowser(_browserWidget);
		SAFE_CONNECT(_qtBrowser, SIGNAL(anchorClicked(const QUrl &)),
				SLOT(beforeNavigate(const QUrl &)));
		_layout->addWidget(_qtBrowser);
	} else {
#if (defined OS_WINDOWS) && (QT_EDITION == QT_EDITION_DESKTOP)
		//Clean qt browser
		if (_qtBrowser) {
			_layout->removeWidget(_qtBrowser);
			OWSAFE_DELETE(_qtBrowser);
		}

		//Init IE browser
		_ieBrowser = new QAxWidget(_browserWidget);
		_ieBrowser->setControl(QString::fromUtf8("{8856F961-340A-11D0-A96B-00C04FD705A2}"));
		_ieBrowser->setObjectName(QString::fromUtf8("IEBrowser"));
		_ieBrowser->setFocusPolicy(Qt::StrongFocus);
		connect(_ieBrowser, SIGNAL(BeforeNavigate(const QString &, int, const QString &, const QVariant &, const QString &, bool &)),
			SLOT(beforeNavigate(const QString &, int, const QString &, const QVariant &, const QString &, bool &)));
		_layout->addWidget(_ieBrowser);
#endif
	}
}

#if (defined OS_WINDOWS) && (QT_EDITION == QT_EDITION_DESKTOP)
bool initPostData(LPVARIANT pvPostData, const std::string & postData) {
	LPCTSTR cszPostData = (LPCTSTR) postData.c_str();
	UINT cElems = lstrlen(cszPostData);

	if (!pvPostData) {
		return false;
	}
	VariantInit(pvPostData);
	LPSAFEARRAY psa = SafeArrayCreateVector(VT_UI1, 0, cElems);
	if (!psa) {
		return false;
	}

	LPSTR pPostData;
	HRESULT hr = SafeArrayAccessData(psa, (LPVOID*) &pPostData);
	memcpy(pPostData, cszPostData, cElems);
	hr = SafeArrayUnaccessData(psa);

	V_VT(pvPostData) = VT_ARRAY | VT_UI1;
	V_ARRAY(pvPostData) = psa;
	return true;
}
#endif

void QtBrowser::setPost(const std::string & url, const std::string & postData) {
#if (defined OS_WINDOWS) && (QT_EDITION == QT_EDITION_DESKTOP)
	VARIANT vFlags = {0};
	VARIANT vPostData = {0};
	VARIANT vHeaders = {0};

	//unicode..
	OLECHAR oleUri[1024];
	MultiByteToWideChar(CP_ACP, 0, url.c_str(), -1, oleUri, sizeof(oleUri) - 1);
	BSTR bstrURL = SysAllocString(oleUri);
	if (!bstrURL) {
		goto finalize;
	}

	//unicode..
	BSTR bstrHeaders = SysAllocString(L"Content-Type: application/x-www-form-urlencoded\r\n");
	if (!bstrHeaders) {
		goto finalize;
	}
	V_VT(&vHeaders) = VT_BSTR;
	V_BSTR(&vHeaders) = bstrHeaders;
	if (!initPostData(&vPostData, postData)) {
		goto finalize;
	}
	IWebBrowser * webBrowser = 0;
	_ieBrowser->queryInterface(IID_IWebBrowser, (void**) &webBrowser);
	if (webBrowser) {
		//use POST method
		webBrowser->Navigate(bstrURL, &vFlags, NULL, &vPostData, &vHeaders);
		webBrowser->Release();
	}

finalize:
	if (bstrURL) {
		SysFreeString(bstrURL);
	}
	if (bstrHeaders) {
		SysFreeString(bstrHeaders);
	}
	VariantClear(&vPostData);
#endif
}
