#include "stdafx.h"
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

#include "QtBrowserWidget.h"
#include "QtWengoPhone.h"

#include <control/profile/CUserProfile.h>
#include <control/profile/CUserProfileHandler.h>
#include <control/CWengoPhone.h>

#include <model/account/wengo/WengoAccount.h>
#include <model/profile/UserProfile.h>
#include <model/config/Config.h>
#include <model/config/ConfigManager.h>
#include <model/network/NetworkProxyDiscovery.h>

#include <owbrowser/QtBrowser.h>
#include <qtutil/SafeConnect.h>
#include <util/Logger.h>
#include <util/SafeDelete.h>
#include <WengoPhoneBuildId.h>

#include <QtGui/QtGui>

static const std::string LOCAL_WEB_DIR = "webpages/windows";

QtBrowserWidget::QtBrowserWidget(QtWengoPhone & qtWengoPhone)
	: _qtWengoPhone(qtWengoPhone) {
	_qtBrowser = NULL;

#if (defined OS_WINDOWS) && (QT_EDITION == QT_EDITION_DESKTOP)
	Config & config = ConfigManager::getInstance().getCurrentConfig();
	if (config.getIEActiveXEnable()) {
		//Embedded Browser
		_qtBrowser = new QtBrowser(_qtWengoPhone.getWidget());
	}
#endif

	//browser re-initialization
	SAFE_CONNECT(&_qtWengoPhone, SIGNAL(userProfileDeleted()), SLOT(slotUserProfileDeleted()));

	loadDefaultURL();
}

QtBrowserWidget::~QtBrowserWidget() {
	OWSAFE_DELETE(_qtBrowser);
}

QWidget * QtBrowserWidget::getWidget() const {
	if (!_qtBrowser) {
		return NULL;
	}

	return (QWidget*) _qtBrowser->getWidget();
}

void QtBrowserWidget::slotUserProfileDeleted() {
	if (_qtBrowser) {
		LOG_DEBUG("delete QtBrowser");
		delete _qtBrowser;
		_qtBrowser = NULL;
	}
}

void QtBrowserWidget::loadDefaultURL() {
	if (!_qtBrowser) {
		return;
	}

	std::string defaultURL = QCoreApplication::applicationDirPath().toStdString() + "/" + LOCAL_WEB_DIR + "/loading.html";
	_qtBrowser->setUrl(defaultURL);
}

void QtBrowserWidget::loadAccountURL() {
	if (!_qtBrowser) {
		return;
	}

	Config & config = ConfigManager::getInstance().getCurrentConfig();

	CUserProfile * cUserProfile = _qtWengoPhone.getCWengoPhone().getCUserProfileHandler().getCUserProfile();
	if (cUserProfile) {
		UserProfile & userProfile = cUserProfile->getUserProfile();

		if (userProfile.getActivePhoneLine()) {

			if (userProfile.hasWengoAccount()) {
					WengoAccount wengoAccount = *userProfile.getWengoAccount();
	
				std::string data = "login=" + wengoAccount.getWengoLogin() +
					"&password=" + wengoAccount.getWengoPassword() +
					"&lang=" + config.getLanguage() +
					"&wl=" + std::string(WengoPhoneBuildId::getSoftphoneName()) +
					"&page=softphoneng-web";
	
				NetworkProxy::ProxyAuthType proxyAuthType =
					NetworkProxyDiscovery::getInstance().getNetworkProxy().getProxyAuthType();

				String url;
				if (proxyAuthType == NetworkProxy::ProxyAuthTypeDigest) {
					//HTTPS cannot be used when the HTTP proxy is in digest:
					//ActiveX Internet Explorer crashes!
					url = config.getHttpHomeUrl();
				} else {
					url = config.getHttpsHomeUrl();
				}
				_qtBrowser->setUrl(url, data, false);

			} else if (userProfile.hasSipAccount()) {
				std::string defaultURL = QCoreApplication::applicationDirPath().toStdString() + "/" + LOCAL_WEB_DIR + "/sip.html";
				_qtBrowser->setUrl(defaultURL);
			}
		}
	}
}
