#include "stdafx.h"
/*
 * WengoPhone, a voice over Internet phone
 * Copyright (C) 2004-2006  Wengo
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

#include "WsSoftUpdate.h"

#include <model/config/ConfigManager.h>
#include <model/config/Config.h>

#include <WengoPhoneBuildId.h>

#include <cutil/global.h>
#include <util/Logger.h>

#include <tinyxml.h>

#include <sstream>

WsSoftUpdate::WsSoftUpdate(WengoAccount * wengoAccount)
	: WengoWebService(wengoAccount) {

	Config & config = ConfigManager::getInstance().getCurrentConfig();

	//Setup the web service
	setHostname(config.getWengoServerHostname());
	setGet(true);
	setHttps(true);
	setServicePath(config.getWengoSoftUpdatePath());
	setPort(443);
	setWengoAuthentication(false);
}

void WsSoftUpdate::checkForUpdate() {
	std::string operatingSystem;

#ifdef OS_WINDOWS
	operatingSystem = "windows";
#elif defined (OS_MACOSX)
	#if defined (__BIG_ENDIAN__)
		operatingSystem = "osx_ppc";
	#else
		operatingSystem = "osx_x86";
	#endif
#elif defined (OS_LINUX)
	operatingSystem = "linux";
#endif

	setParameters(std::string("aro=wengophoneng") +
			std::string("&installer_type=") + operatingSystem);

	//Calls the web service
	call(this);
}

void WsSoftUpdate::answerReceived(const std::string & answer, int requestId) {
	if (answer.empty()) {
		return;
	}

	unsigned long long buildId = 0;
	std::string downloadUrl;
	std::string version;
	unsigned fileSize = 0;

	TiXmlDocument doc;
	doc.Parse(answer.c_str());

	TiXmlHandle docHandle(&doc);

	TiXmlHandle root = docHandle.FirstChild("softphone").FirstChild("version");

	TiXmlText * text = root.FirstChild("build").FirstChild().Text();
	if (text) {
		std::stringstream ss(std::string(text->Value()));
		ss >> buildId;
	}

	text = root.FirstChild("url").FirstChild().Text();
	if (text) {
		downloadUrl = text->Value();
	}

	text = root.FirstChild("version_marketing").FirstChild().Text();
	if (text) {
		version = text->Value();
	}

	text = root.FirstChild("filesize").FirstChild().Text();
	if (text) {
		std::stringstream ss(std::string(text->Value()));
		ss >> fileSize;
	}


	if ((WengoPhoneBuildId::getBuildId() != 0) && (buildId > WengoPhoneBuildId::getBuildId())) {
		//A new version of WengoPhone is available and we don't have a developer version with buildid=0
		LOG_DEBUG("new WengoPhone version available=" + version + " buildid=" + String::fromUnsignedLongLong(buildId));

		//File size in megabytes
		fileSize = fileSize / 1000 / 1000;

		updateWengoPhoneEvent(*this, downloadUrl, buildId, version, fileSize);
	} else {
		LOG_DEBUG("WengoPhone is up-to-date");
	}
}
