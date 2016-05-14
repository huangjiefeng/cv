#include "stdafx.h"
/*
 * WengoPhone, a voice over Internet phone
 * Copyright (C) 2007  Wengo
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

#include "WinApp.h"

#include <control/CWengoPhone.h>

#include <util/Logger.h>

#include <windows.h>

#define MY_WM_TERM WM_USER + 684
#define MY_WM_EXIT WM_USER + 685

WinApp::WinApp(int & argc, char ** argv) : QApplication(argc, argv) {
}

bool WinApp::winEventFilter(MSG * msg, long * result) {

	if (msg->message == (MY_WM_TERM)) {
		if (_cWengoPhone) {
			LOG_INFO("Disable serialization");
			_cWengoPhone->enableSerialization(false);
		}
		return true;

	} else if (msg->message == (MY_WM_EXIT)) {
		if (_cWengoPhone) {
			LOG_INFO("Terminating");
			_cWengoPhone->enableSerialization(false);
			// "wait" for pending serialization to be finished
			Sleep(1000);
			exit(1);
		}
	}

	return false;
}

void WinApp::setCWengoPhone(CWengoPhone *cWengoPhone) {
	_cWengoPhone = cWengoPhone;
}
