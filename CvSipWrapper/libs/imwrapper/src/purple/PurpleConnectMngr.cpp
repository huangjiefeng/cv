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

extern "C" {
#include "glib.h"
#include "libpurple/core.h"
}

#include "PurpleConnectMngr.h"
#include "PurpleIMFactory.h"

#include "PurpleEnumIMProtocol.h"

#include <util/Logger.h>

/* ***************** PURPLE CALLBACK ***************** */
static void C_ConnProgressCbk(PurpleConnection *gc, const char *text,
							  size_t step, size_t step_count)
{
	PurpleConnectMngr::ConnProgressCbk(gc, text, step, step_count);
}

static void C_ConnConnectedCbk(PurpleConnection *gc)
{
	PurpleConnectMngr::ConnConnectedCbk(gc);
}

static void C_ConnDisconnectedCbk(PurpleConnection *gc)
{
	PurpleConnectMngr::ConnDisconnectedCbk(gc);
}

static void C_ConnNoticeCbk(PurpleConnection *gc, const char *text)
{
	PurpleConnectMngr::ConnNoticeCbk(gc, text);
}

static void C_ConnReportDisconnectCbk(PurpleConnection *gc, const char *text)
{
	PurpleConnectMngr::ConnReportDisconnectCbk(gc, text);
}

static void C_NetworkConnected()
{
	PurpleConnectMngr::NetworkConnected();
}

static void C_NetworkDisconnected()
{
	PurpleConnectMngr::NetworkDisconnected();
}

PurpleConnectionUiOps conn_wg_ops =
{
	C_ConnProgressCbk,
	C_ConnConnectedCbk,
	C_ConnDisconnectedCbk,
	C_ConnNoticeCbk,
	C_ConnReportDisconnectCbk,
	C_NetworkConnected,
	C_NetworkDisconnected,
};

PurpleConnectionUiOps null_conn_wg_ops =
{
	NULL, NULL, NULL, NULL, NULL, NULL, NULL
};

/* ************************************************** */

PurpleConnectMngr *PurpleConnectMngr::_staticInstance = NULL;
std::list<PurpleIMConnect *> PurpleConnectMngr::_purpleIMConnectList;
RecursiveMutex PurpleConnectMngr::_mutex;

PurpleConnectMngr::PurpleConnectMngr()
{
}

void PurpleConnectMngr::Init()
{
	_accountMngr = PurpleAccountMngr::getInstance();
}

PurpleConnectMngr *PurpleConnectMngr::getInstance()
{
	if (!_staticInstance)
	{
		_staticInstance = new PurpleConnectMngr();
	}
	return _staticInstance;
}

PurpleIMConnect *FindIMConnnectByPurpleConnection(PurpleConnection *gc)
{
	PurpleAccount *gAccount = NULL;
	IMAccount	*mAccount = NULL;
	EnumIMProtocol::IMProtocol protocol = EnumIMProtocol::IMProtocolAll;

	gAccount = purple_connection_get_account(gc);
	if (gAccount == NULL)
		return NULL;

	protocol = PurpleIMPrcl::GetEnumIMProtocol(gAccount->protocol_id);
	mAccount = PurpleAccountMngr::FindIMAccount(gAccount->username, protocol);
	if (mAccount == NULL)
		return NULL;

	return PurpleConnectMngr::FindIMConnect(*mAccount);
}

void PurpleConnectMngr::ConnProgressCbk(PurpleConnection *gc, const char *text,
									  size_t step, size_t step_count)
{
	RecursiveMutex::ScopedLock lock(_mutex);

	PurpleIMConnect *mIMConnect = FindIMConnnectByPurpleConnection(gc);

	if (mIMConnect)
	{
		mIMConnect->connectionProgressEvent(*mIMConnect, step, step_count, text == NULL ? String::null : text);
	}
}

void PurpleConnectMngr::ConnConnectedCbk(PurpleConnection *gc)
{
	RecursiveMutex::ScopedLock lock(_mutex);

	PurpleIMConnect *mIMConnect = FindIMConnnectByPurpleConnection(gc);

	if (mIMConnect)
	{
		mIMConnect->setConnected(true);
		mIMConnect->connectedEvent(*mIMConnect);
	}
}

void PurpleConnectMngr::ConnDisconnectedCbk(PurpleConnection *gc)
{
	RecursiveMutex::ScopedLock lock(_mutex);

	PurpleIMConnect *mIMConnect = FindIMConnnectByPurpleConnection(gc);

	if (mIMConnect && mIMConnect->isConnected())
	{
		mIMConnect->setConnected(false);
		mIMConnect->disconnectedEvent(*mIMConnect, false, String::null);

		if (gc->wants_to_die)
		{
			purple_account_set_enabled(gc->account, "Wengo PURPLE", FALSE);
		}
	}
}

void PurpleConnectMngr::ConnNoticeCbk(PurpleConnection *gc, const char *text)
{
}

void PurpleConnectMngr::ConnReportDisconnectCbk(PurpleConnection *gc, const char *text)
{
	RecursiveMutex::ScopedLock lock(_mutex);

	PurpleIMConnect *mIMConnect = FindIMConnnectByPurpleConnection(gc);

	if (mIMConnect)
	{
		mIMConnect->setConnected(false);
		mIMConnect->disconnectedEvent(*mIMConnect, true, text == NULL ? String::null : text);

		if (gc->wants_to_die)
		{
			purple_account_set_enabled(gc->account, "Wengo PURPLE", FALSE);
		}
	}
}

void PurpleConnectMngr::NetworkConnected()
{
}

void PurpleConnectMngr::NetworkDisconnected()
{
}

/* **************** MANAGE CONNECT_LIST ****************** */
PurpleIMConnect *PurpleConnectMngr::FindIMConnect(IMAccount &account)
{
	RecursiveMutex::ScopedLock lock(_mutex);

	PurpleIMConnectIterator i;

	if (account.getLogin().empty())
		return NULL;

	for (i = _purpleIMConnectList.begin(); i != _purpleIMConnectList.end(); i++)
	{
		if (PurpleIMFactory::equals((*i)->getIMAccount(), account.getLogin(), account.getProtocol()))
		{
			return (*i);
		}
	}

	return NULL;
}

PurpleIMConnect *PurpleConnectMngr::AddIMConnect(IMAccount &account)
{
	RecursiveMutex::ScopedLock lock(_mutex);

	PurpleIMConnect *mIMConnect = FindIMConnect(account);

	if (!mIMConnect)
	{
		mIMConnect = new PurpleIMConnect(account);
		_purpleIMConnectList.push_back(mIMConnect);

		_accountMngr->AddIMAccount(account);
	}

	return mIMConnect;
}

void PurpleConnectMngr::RemoveIMConnect(IMAccount &account)
{
	RecursiveMutex::ScopedLock lock(_mutex);

	PurpleIMConnectIterator i;

	if (account.getLogin().empty())
		return;

	for (i = _purpleIMConnectList.begin(); i != _purpleIMConnectList.end(); i++)
	{
		if (PurpleIMFactory::equals((*i)->getIMAccount(), account.getLogin(), account.getProtocol()))
		{
			_purpleIMConnectList.erase(i);
			break;
		}
	}
}

void PurpleConnectMngr::UpdateIMConnect(IMAccount &account)
{
	RecursiveMutex::ScopedLock lock(_mutex);

	PurpleIMConnectIterator i;

	if (account.getLogin().empty())
		return;

	for (i = _purpleIMConnectList.begin(); i != _purpleIMConnectList.end(); i++)
	{
		if (PurpleIMFactory::equals((*i)->getIMAccount(), account.getLogin(), account.getProtocol()))
		{
			(*i)->setIMAccount(account);
			break;
		}
	}
}

void PurpleConnectMngr::reset() {
	RecursiveMutex::ScopedLock lock(_mutex);

	_purpleIMConnectList.clear();
}

/* ******************************************************** */

