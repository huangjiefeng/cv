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

#ifndef OWWSSOFTUPDATE_H
#define OWWSSOFTUPDATE_H

#include <model/webservices/WengoWebService.h>

/**
 * WengoPhone update web service.
 *
 * Checks for the availability of WengoPhone updates.
 *
 * @author Tanguy Krotoff
 */
class WsSoftUpdate : public WengoWebService {
public:

	/**
	 * Default constructor.
	 *
	 * @param wengoAccount the WengoAccount used for web services
	 */
	WsSoftUpdate(WengoAccount * wengoAccount);

	virtual ~WsSoftUpdate() {}

	/**
	 * Event WengoPhone should be updated.
	 *
	 * @param sender this class
	 * @param downloadUrl WengoPhone update download URL
	 * @param buildId WengoPhone update build ID
	 * @param version WengoPhone update version number
	 * @param fileSize WengoPhone update file size in kil bytes
	 */
	Event<void (WsSoftUpdate & sender,
			const std::string & downloadUrl,
			unsigned long long buildId,
			const std::string & version,
			unsigned fileSize)> updateWengoPhoneEvent;

	/**
	 * Checks if a WengoPhone update is available.
	 */
	void checkForUpdate();

private:

	/**
	 * @see WengoWebService
	 */
	void answerReceived(const std::string & answer, int id);
};

#endif //OWWSSOFTUPDATE_H
