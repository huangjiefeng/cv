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

#include "PurpleIMContactList.h"

#include "PurpleEnumIMProtocol.h"

#include <util/Logger.h>

extern "C" {
#include <libpurple/blist.h>
}

typedef struct misc_s
{
	PurpleIMContactList * instance;
	void * data_ptr1;
	void * data_ptr2;
	void * data_ptr3;
	int cbk_id;
}	misc_t;

Mutex PurpleIMContactList::_mutex;

PurpleIMContactList::PurpleIMContactList(IMAccount account)
	: IMContactList(account)
{
}

bool PurpleIMContactList::addContactCbk(void * data)
{
	Mutex::ScopedLock lock(PurpleIMContactList::_mutex);

	PurpleIMContactList * imContactList = ((misc_t *)data)->instance;
	const char * groupName =(const char *)((misc_t *)data)->data_ptr1;
	const char * contactId =(const char *)((misc_t *)data)->data_ptr2;
	PurpleAccount *gAccount = purple_accounts_find(imContactList->getIMAccount().getLogin().c_str(),
								PurpleIMPrcl::GetPrclId(imContactList->getIMAccount().getProtocol()));
	PurpleGroup *gGroup = NULL;
	PurpleBuddy *gBuddy = NULL;

	if ((gGroup = purple_find_group(groupName)) == NULL) {
		gGroup = purple_group_new(groupName);
		purple_blist_add_group(gGroup, NULL);
	}

	gBuddy = purple_buddy_new(gAccount, contactId, contactId);
	purple_blist_add_buddy(gBuddy, NULL, gGroup, NULL);
	purple_account_add_buddy(gAccount, gBuddy);

	purple_timeout_remove(((misc_t *)data)->cbk_id);

	free(((misc_t *)data)->data_ptr1);
	free(((misc_t *)data)->data_ptr2);
	free((misc_t *)data);

	return TRUE;
}

void PurpleIMContactList::addContact(const std::string & groupName, 
								   const std::string & contactId)
{
	misc_t * data = (misc_t *) malloc(sizeof(misc_t));
	memset(data, 0, sizeof(misc_t));

	data->instance = this;
	data->data_ptr1 = strdup(groupName.c_str());
	data->data_ptr2 = strdup(contactId.c_str());

	Mutex::ScopedLock lock(PurpleIMContactList::_mutex);
	data->cbk_id = purple_timeout_add(0, (GSourceFunc) PurpleIMContactList::addContactCbk, data);
}

bool PurpleIMContactList::removeContactCbk(void * data)
{
	Mutex::ScopedLock lock(PurpleIMContactList::_mutex);

	PurpleIMContactList * imContactList = ((misc_t *)data)->instance;
	const char * groupName =(const char *)((misc_t *)data)->data_ptr1;
	const char * contactId =(const char *)((misc_t *)data)->data_ptr2;
	PurpleAccount *gAccount = purple_accounts_find(imContactList->getIMAccount().getLogin().c_str(),
								PurpleIMPrcl::GetPrclId(imContactList->getIMAccount().getProtocol()));
	PurpleGroup *gGroup = NULL;
	PurpleBuddy *gBuddy = NULL;

	if (groupName)
	{
		gGroup = purple_find_group(groupName);
		gBuddy = purple_find_buddy_in_group(gAccount, contactId, gGroup);
	}
	else
	{
		gBuddy = purple_find_buddy(gAccount, contactId);
	}

	if (gBuddy) {
		purple_account_remove_buddy(gAccount, gBuddy, gGroup);
		purple_blist_remove_buddy(gBuddy);
	}

	purple_timeout_remove(((misc_t *)data)->cbk_id);

	if (((misc_t *)data)->data_ptr1) {
		free(((misc_t *)data)->data_ptr1);
	}
	free(((misc_t *)data)->data_ptr2);
	free((misc_t *)data);

	return TRUE;
}

void PurpleIMContactList::removeContact(const std::string & groupName, 
									  const std::string & contactId)
{
	misc_t * data = (misc_t *) malloc(sizeof(misc_t));
	memset(data, 0, sizeof(misc_t));

	data->instance = this;
	if (!groupName.empty())
		data->data_ptr1 = strdup(groupName.c_str());
	data->data_ptr2 = strdup(contactId.c_str());

	Mutex::ScopedLock lock(PurpleIMContactList::_mutex);
	data->cbk_id = purple_timeout_add(0, (GSourceFunc) PurpleIMContactList::removeContactCbk, data);
}

bool PurpleIMContactList::addGroupCbk(void * data)
{
	Mutex::ScopedLock lock(PurpleIMContactList::_mutex);

	const char * groupName = (const char *)((misc_t *)data)->data_ptr1;
	PurpleGroup *gGroup = purple_find_group(groupName);

	if (!gGroup) {
		gGroup = purple_group_new(groupName);
		purple_blist_add_group(gGroup, NULL);
	}

	purple_timeout_remove(((misc_t *)data)->cbk_id);

	free(((misc_t *)data)->data_ptr1);
	free((misc_t *)data);

	return TRUE;
}

void PurpleIMContactList::addGroup(const std::string & groupName)
{
	misc_t * data = (misc_t *) malloc(sizeof(misc_t));
	memset(data, 0, sizeof(misc_t));

	data->data_ptr1 = strdup(groupName.c_str());

	Mutex::ScopedLock lock(PurpleIMContactList::_mutex);
	data->cbk_id = purple_timeout_add(0, (GSourceFunc) PurpleIMContactList::addGroupCbk, data);
}

bool PurpleIMContactList::removeGroupCbk(void * data)
{
	Mutex::ScopedLock lock(PurpleIMContactList::_mutex);

	const char * groupName = (const char *)((misc_t *)data)->data_ptr1;
	PurpleGroup *gGroup = purple_find_group(groupName);

	if (gGroup) {
		purple_blist_remove_group(gGroup);
	}

	purple_timeout_remove(((misc_t *)data)->cbk_id);

	free(((misc_t *)data)->data_ptr1);
	free((misc_t *)data);

	return TRUE;
}

void PurpleIMContactList::removeGroup(const std::string & groupName)
{
	misc_t * data = (misc_t *) malloc(sizeof(misc_t));
	memset(data, 0, sizeof(misc_t));

	data->data_ptr1 = strdup(groupName.c_str());

	Mutex::ScopedLock lock(PurpleIMContactList::_mutex);
	data->cbk_id = purple_timeout_add(0, (GSourceFunc) PurpleIMContactList::removeGroupCbk, data);
}

bool PurpleIMContactList::changeGroupNameCbk(void * data)
{
	Mutex::ScopedLock lock(PurpleIMContactList::_mutex);

	const char * newGroupName = (const char *)((misc_t *)data)->data_ptr1;
	const char * oldGroupName = (const char *)((misc_t *)data)->data_ptr2;
	PurpleGroup *gGroup = purple_find_group(oldGroupName);
	
	if (gGroup) {
		purple_blist_rename_group(gGroup, newGroupName);
	}

	purple_timeout_remove(((misc_t *)data)->cbk_id);

	free(((misc_t *)data)->data_ptr1);
	free(((misc_t *)data)->data_ptr2);
	free((misc_t *)data);

	return TRUE;
}

void PurpleIMContactList::changeGroupName(const std::string & oldGroupName, 
										const std::string & newGroupName)
{
	misc_t * data = (misc_t *) malloc(sizeof(misc_t));
	memset(data, 0, sizeof(misc_t));

	data->data_ptr1 = strdup(newGroupName.c_str());
	data->data_ptr2 = strdup(oldGroupName.c_str());

	Mutex::ScopedLock lock(PurpleIMContactList::_mutex);
	data->cbk_id = purple_timeout_add(0, (GSourceFunc) PurpleIMContactList::changeGroupNameCbk, data);
}

bool PurpleIMContactList::moveContactToGroupCbk(void * data)
{
	Mutex::ScopedLock lock(PurpleIMContactList::_mutex);

	PurpleIMContactList * imContactList = ((misc_t *)data)->instance;
	const char * newGroupName = (const char *)((misc_t *)data)->data_ptr1;
	const char * oldGroupName = (const char *)((misc_t *)data)->data_ptr2;
	const char * contactId = (const char *)((misc_t *)data)->data_ptr3;
	PurpleAccount *gAccount = purple_accounts_find(imContactList->getIMAccount().getLogin().c_str(),
		PurpleIMPrcl::GetPrclId(imContactList->getIMAccount().getProtocol()));
	PurpleGroup *gOldGroup = purple_find_group(oldGroupName);

	if (gOldGroup) {
		PurpleBuddy *gBuddy = purple_find_buddy_in_group(gAccount, contactId, gOldGroup);
		if (gBuddy) {
			PurpleGroup *gNewGroup = purple_find_group(newGroupName);
			if (gNewGroup) {
				gNewGroup = purple_group_new(newGroupName);
				purple_blist_add_buddy(gBuddy, NULL, gNewGroup, NULL);
			}
		}
	}

	purple_timeout_remove(((misc_t *)data)->cbk_id);

	free(((misc_t *)data)->data_ptr1);
	free(((misc_t *)data)->data_ptr2);
	free(((misc_t *)data)->data_ptr3);
	free((misc_t *)data);

	return TRUE;
}

void PurpleIMContactList::moveContactToGroup(const std::string & newGroupName,
	const std::string & oldGroupName, const std::string & contactId)
{
	misc_t * data = (misc_t *) malloc(sizeof(misc_t));
	memset(data, 0, sizeof(misc_t));

	data->instance = this;
	data->data_ptr1 = strdup(newGroupName.c_str());
	data->data_ptr2 = strdup(oldGroupName.c_str());
	data->data_ptr3 = strdup(contactId.c_str());

	Mutex::ScopedLock lock(PurpleIMContactList::_mutex);
	data->cbk_id = purple_timeout_add(0, (GSourceFunc) PurpleIMContactList::moveContactToGroupCbk, data);
}
