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

#include "PurpleIMPresence.h"

#include "PurpleEnumPresenceState.h"
#include "PurplePresenceMngr.h"

extern "C" {
#include "libpurple/account.h"
#include "libpurple/buddyicon.h"
#include "libpurple/core.h"
#include "libpurple/privacy.h"
}

#include <util/File.h>
#include <util/Logger.h>
#include <util/OWPicture.h>

struct AuthRequest_s
{
	PurpleAccountRequestAuthorizationCb accept_cb;
	PurpleAccountRequestAuthorizationCb deny_cb;
	void *user_data;
};

typedef struct misc_s
{
	PurpleIMPresence * instance;
	void * data_ptr;
	int data_int;
	int cbk_id;
}	misc_t;


Mutex PurpleIMPresence::_mutex;

PurpleIMPresence::PurpleIMPresence(IMAccount account)
	: IMPresence(account)
{
}

PurpleIMPresence::~PurpleIMPresence() {
	PurplePresenceMngr::getInstance()->RemoveIMPresence(_imAccount);
}

const char *PurpleIMPresence::getPurplePresenceId(void *purplePresence)
{
	PurplePresence *gPresence = (PurplePresence *) purplePresence;
	PurpleStatus *gStatus;
	PurpleStatusType *gStatusType;
	PurpleStatusPrimitive gStatusPrim;

	gStatus = purple_presence_get_active_status(gPresence);
	gStatusType = purple_status_get_type(gStatus);
	gStatusPrim = purple_status_type_get_primitive(gStatusType);

	return purple_primitive_get_id_from_type(gStatusPrim);
}

bool PurpleIMPresence::changeMyPresenceCbk(void * data)
{
	Mutex::ScopedLock lock(PurpleIMPresence::_mutex);

	PurpleIMPresence * imPresence = ((misc_t *)data)->instance;
	EnumPresenceState::PresenceState state = 
		(EnumPresenceState::PresenceState)((misc_t *)data)->data_int;
	const char * note = (const char *)((misc_t *)data)->data_ptr; 
	PurpleAccount *gAccount = purple_accounts_find(imPresence->getIMAccount().getLogin().c_str(), 
		PurpleIMPrcl::GetPrclId(imPresence->getIMAccount().getProtocol()));

	if (gAccount && !purple_account_is_connecting(gAccount))
	{
		if (!purple_account_is_connected(gAccount)) {
			purple_account_set_enabled(gAccount, purple_core_get_ui(), TRUE);
		}

		if (strlen(note) == 0) {
			purple_account_set_status(gAccount, PurplePreState::GetStatusId(state, 
				imPresence->getIMAccount().getProtocol()), TRUE, NULL);
		} else {
			purple_account_set_status(gAccount, PurplePreState::GetStatusId(state, 
				imPresence->getIMAccount().getProtocol()), TRUE, "message", note, NULL);
		}
	}

	purple_timeout_remove(((misc_t *)data)->cbk_id);

	free(((misc_t *)data)->data_ptr);
	free((misc_t *)data);
	
	return TRUE;
}

void PurpleIMPresence::changeMyPresence(EnumPresenceState::PresenceState state, 
									  const std::string & note)
{
	misc_t * data = (misc_t *) malloc(sizeof(misc_t));
	memset(data, 0, sizeof(misc_t));

	data->instance = this;
	data->data_int = state;
	if (note.empty()) {
		data->data_ptr = strdup("");
	} else {
		data->data_ptr = strdup(note.c_str());
	}

	Mutex::ScopedLock lock(PurpleIMPresence::_mutex);
	data->cbk_id = purple_timeout_add(0, (GSourceFunc) PurpleIMPresence::changeMyPresenceCbk, data);
}

bool PurpleIMPresence::changeMyAliasCbk(void * data)
{
	Mutex::ScopedLock lock(PurpleIMPresence::_mutex);

	PurpleIMPresence * imPresence = ((misc_t *)data)->instance;
	const char * nickname = (const char *)((misc_t *)data)->data_ptr;
	PurpleAccount *gAccount = purple_accounts_find(imPresence->getIMAccount().getLogin().c_str(), 
		PurpleIMPrcl::GetPrclId(imPresence->getIMAccount().getProtocol()));

	if (gAccount && purple_account_is_connected(gAccount)) {
		purple_account_set_alias(gAccount,nickname);
	}

	purple_timeout_remove(((misc_t *)data)->cbk_id);

	free(((misc_t *)data)->data_ptr);
	free((misc_t *)data);
	
	return TRUE;
}

void PurpleIMPresence::changeMyAlias(const std::string & nickname)
{
	misc_t * data = (misc_t *) malloc(sizeof(misc_t));
	memset(data, 0, sizeof(misc_t));

	data->instance = this;
	if (nickname.empty()) {
		data->data_ptr = strdup("");
	} else {
		data->data_ptr = strdup(nickname.c_str());
	}

	Mutex::ScopedLock lock(PurpleIMPresence::_mutex);
	data->cbk_id = purple_timeout_add(0, (GSourceFunc) PurpleIMPresence::changeMyAliasCbk, data);
}

bool PurpleIMPresence::changeMyIconCbk(void * data)
{
	Mutex::ScopedLock lock(PurpleIMPresence::_mutex);

	PurpleIMPresence * imPresence = ((misc_t *)data)->instance;
	const char * iconPath = (const char *)((misc_t *)data)->data_ptr;
	PurpleAccount *gAccount = purple_accounts_find(imPresence->getIMAccount().getLogin().c_str(), 
		PurpleIMPrcl::GetPrclId(imPresence->getIMAccount().getProtocol()));

	purple_account_set_buddy_icon_path(gAccount, iconPath);

	purple_timeout_remove(((misc_t *)data)->cbk_id);

	free(((misc_t *)data)->data_ptr);
	free((misc_t *)data);

	return TRUE;
}

void PurpleIMPresence::changeMyIcon(const OWPicture & picture)
{
	FileWriter file(File::createTemporaryFile());
	file.write(picture.getData());
	file.close();

	misc_t * data = (misc_t *) malloc(sizeof(misc_t));
	memset(data, 0, sizeof(misc_t));

	data->instance = this;
	data->data_ptr = strdup(file.getFullPath().c_str());

	Mutex::ScopedLock lock(PurpleIMPresence::_mutex);
	data->cbk_id = purple_timeout_add(0, (GSourceFunc) PurpleIMPresence::changeMyIconCbk, data);
}

bool PurpleIMPresence::subscribeToPresenceOfCbk(void * data) 
{
	Mutex::ScopedLock lock(PurpleIMPresence::_mutex);

	PurpleIMPresence * imPresence = ((misc_t *)data)->instance;
	const char * contactId = (const char *)((misc_t *)data)->data_ptr;
	PurpleAccount *gAccount = purple_accounts_find(imPresence->getIMAccount().getLogin().c_str(), 
		PurpleIMPrcl::GetPrclId(imPresence->getIMAccount().getProtocol()));
	PurpleBuddy *gBuddy = NULL;

	if (gAccount && (gBuddy = purple_find_buddy(gAccount, contactId))) {
		PurplePresence *gPresence = purple_buddy_get_presence(gBuddy);
		const char *gPresenceId = getPurplePresenceId(gPresence);
		const char *buddy_alias =
			gBuddy->server_alias && *gBuddy->server_alias ? gBuddy->server_alias : gBuddy->alias;

		imPresence->presenceStateChangedEvent(*imPresence,
			PurplePreState::GetPresenceState(gPresenceId),
			!buddy_alias ? String::null : buddy_alias,
			purple_buddy_get_name(gBuddy));
	}

	purple_timeout_remove(((misc_t *)data)->cbk_id);

	free(((misc_t *)data)->data_ptr);
	free((misc_t *)data);
	
	return TRUE;
}

void PurpleIMPresence::subscribeToPresenceOf(const std::string & contactId)
{
	misc_t * data = (misc_t *) malloc(sizeof(misc_t));
	memset(data, 0, sizeof(misc_t));

	data->instance = this;
	data->data_ptr = strdup(contactId.c_str());

	Mutex::ScopedLock lock(PurpleIMPresence::_mutex);
	data->cbk_id = purple_timeout_add(0, 
		(GSourceFunc) PurpleIMPresence::subscribeToPresenceOfCbk, data);
}

void PurpleIMPresence::unsubscribeToPresenceOf(const std::string & contactId)
{
}

bool PurpleIMPresence::blockContactCbk(void * data)
{
	Mutex::ScopedLock lock(PurpleIMPresence::_mutex);

	PurpleIMPresence * imPresence = ((misc_t *)data)->instance;
	const char * contactId = (const char *)((misc_t *)data)->data_ptr;
	PurpleAccount *gAccount = purple_accounts_find(imPresence->getIMAccount().getLogin().c_str(),
		PurpleIMPrcl::GetPrclId(imPresence->getIMAccount().getProtocol()));

	if (gAccount)
	{
		purple_privacy_deny_add(gAccount, contactId, FALSE);
	}

	purple_timeout_remove(((misc_t *)data)->cbk_id);

	free(((misc_t *)data)->data_ptr);
	free((misc_t *)data);
	
	return TRUE;
}

void PurpleIMPresence::blockContact(const std::string & contactId)
{
	misc_t * data = (misc_t *) malloc(sizeof(misc_t));
	memset(data, 0, sizeof(misc_t));

	data->instance = this;
	data->data_ptr = strdup(contactId.c_str());

	Mutex::ScopedLock lock(PurpleIMPresence::_mutex);
	data->cbk_id = purple_timeout_add(0, 
		(GSourceFunc) PurpleIMPresence::blockContactCbk, data);
}

bool PurpleIMPresence::unblockContactCbk(void * data)
{
	Mutex::ScopedLock lock(PurpleIMPresence::_mutex);

	PurpleIMPresence * imPresence = ((misc_t *)data)->instance;
	const char * contactId = (const char *)((misc_t *)data)->data_ptr;
	PurpleAccount *gAccount = purple_accounts_find(imPresence->getIMAccount().getLogin().c_str(),
		PurpleIMPrcl::GetPrclId(imPresence->getIMAccount().getProtocol()));

	if (gAccount)
	{
		purple_privacy_permit_add(gAccount, contactId, FALSE);
	}

	purple_timeout_remove(((misc_t *)data)->cbk_id);

	free(((misc_t *)data)->data_ptr);
	free((misc_t *)data);

	return TRUE;
}

void PurpleIMPresence::unblockContact(const std::string & contactId)
{
	misc_t * data = (misc_t *) malloc(sizeof(misc_t));
	memset(data, 0, sizeof(misc_t));

	data->instance = this;
	data->data_ptr = strdup(contactId.c_str());

	Mutex::ScopedLock lock(PurpleIMPresence::_mutex);
	data->cbk_id = purple_timeout_add(0, 
		(GSourceFunc) PurpleIMPresence::unblockContactCbk, data);
}

bool PurpleIMPresence::authorizeContactCbk(void * data)
{
	Mutex::ScopedLock lock(PurpleIMPresence::_mutex);

	PurpleIMPresence * imPresence = ((misc_t *)data)->instance;
	const char * contactId = (const char *)((misc_t *)data)->data_ptr;
	int authorized = ((misc_t *)data)->data_int;
	PurpleAccount *gAccount = purple_accounts_find(imPresence->getIMAccount().getLogin().c_str(),
		PurpleIMPrcl::GetPrclId(imPresence->getIMAccount().getProtocol()));

	if (gAccount && imPresence->authRequestMap.find(contactId) != imPresence->authRequestMap.end()){
		if (authorized) {
			imPresence->authRequestMap[contactId]->accept_cb(
				imPresence->authRequestMap[contactId]->user_data);
		} else {
			imPresence->authRequestMap[contactId]->deny_cb(
				imPresence->authRequestMap[contactId]->user_data);
		}
	}

	purple_timeout_remove(((misc_t *)data)->cbk_id);

	free(((misc_t *)data)->data_ptr);
	free((misc_t *)data);

	return TRUE;
}

void PurpleIMPresence::authorizeContact(const std::string & contactId, 
									  bool authorized, const std::string message)
{
	misc_t * data = (misc_t *) malloc(sizeof(misc_t));
	memset(data, 0, sizeof(misc_t));

	data->instance = this;
	data->data_ptr = strdup(contactId.c_str());
	data->data_int = authorized;

	Mutex::ScopedLock lock(PurpleIMPresence::_mutex);
	data->cbk_id = purple_timeout_add(0, (GSourceFunc) PurpleIMPresence::authorizeContactCbk, data);
}

void PurpleIMPresence::addAuthRequestInList(const std::string &contactId, void *acceptCbk, 
										  void *denyCbk, void *userData)
{
	AuthRequest *authReq = new AuthRequest();

	authReq->accept_cb = (PurpleAccountRequestAuthorizationCb) acceptCbk;
	authReq->deny_cb = (PurpleAccountRequestAuthorizationCb) denyCbk;
	authReq->user_data = userData;

	authRequestMap[contactId] = authReq;
}
