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

#include "PurpleIMChat.h"

#include "PurpleEnumIMProtocol.h"

#include <imwrapper/IMAccount.h>
#include <imwrapper/IMChatSession.h>
#include <imwrapper/IMContactSet.h>
#include <imwrapper/IMContact.h>

#include <util/Logger.h>
#include <cutil/global.h>

extern "C" {
#include <libpurple/conversation.h>
#include <libpurple/util.h>
}

#ifdef OS_WINDOWS
	#define snprintf _snprintf
#else
	#include <cstdio>
#endif

typedef struct misc_s
{
	PurpleIMChat * instance;
	void * data_ptr1;
	void * data_ptr2;
	int data_int;
	int cbk_id;
}	misc_t;

std::list<mConvInfo_t *> PurpleIMChat::_PurpleChatSessionList;
Mutex PurpleIMChat::_mutex;

PurpleIMChat::PurpleIMChat(IMAccount account)
	: IMChat(account)
{

}

mConvInfo_t *PurpleIMChat::CreateChatSession(bool userCreated, PurpleIMChat & imChat)
{
	mConvInfo_t *mConv = new mConvInfo_t();
	IMChatSession *chatSession = new IMChatSession(imChat, userCreated);

	mConv->conv_session = chatSession;
	mConv->conv_id = chatSession->getId();
	mConv->pending_invit = NULL;

	AddChatSessionInList(mConv);

	return mConv;
}

bool PurpleIMChat::createSessionCbk(void * data)
{
	Mutex::ScopedLock lock(PurpleIMChat::_mutex);

	PurpleIMChat * imChat = ((misc_t *)data)->instance;
	IMContactSet & imContactSet = *(IMContactSet *)((misc_t *)data)->data_ptr1;
	PurpleAccount *gAccount = purple_accounts_find(imChat->getIMAccount().getLogin().c_str(),
		PurpleIMPrcl::GetPrclId(imChat->getIMAccount().getProtocol()));

	IMContactSet::const_iterator it;

	if (imContactSet.empty())
	{
		LOG_FATAL("imContactSet is empty");
	}
	else if (imContactSet.size() == 1)
	{
		it = imContactSet.begin();
		std::string contactId = (*it).getContactId();

		PurpleConversation *gConv = NULL;
		if ((gConv = purple_find_conversation_with_account(PURPLE_CONV_TYPE_IM,
			contactId.c_str(), gAccount)))
		{
			mConvInfo_t *mConv = (mConvInfo_t *)gConv->ui_data;
			imChat->newIMChatSessionCreatedEvent(*imChat, *((IMChatSession *)(mConv->conv_session)));

			purple_timeout_remove(((misc_t *)data)->cbk_id);
			delete (IMContactSet *)((misc_t *)data)->data_ptr1;
			free((misc_t *)data);

			return FALSE;
		}

		purple_conversation_new(PURPLE_CONV_TYPE_IM, gAccount, contactId.c_str());
	}
	else
	{
		GList *mlist = NULL;
		PurpleConnection *gGC = purple_account_get_connection(gAccount);

		for (it = imContactSet.begin(); it != imContactSet.end(); it++)
		{
			mlist = g_list_append(mlist, (char *) it->getContactId().c_str());
		}

		createPurpleChat(gGC, 0, mlist, *imChat);
	}

	purple_timeout_remove(((misc_t *)data)->cbk_id);
	delete (IMContactSet *)((misc_t *)data)->data_ptr1;
	free((misc_t *)data);

	return TRUE;
}

void PurpleIMChat::createSession(const IMContactSet & imContactSet)
{
	misc_t * data = (misc_t *) malloc(sizeof(misc_t));
	memset(data, 0, sizeof(misc_t));

	data->instance = this;
	data->data_ptr1 = new IMContactSet(imContactSet);

	Mutex::ScopedLock lock(PurpleIMChat::_mutex);
	data->cbk_id = purple_timeout_add(0, (GSourceFunc) PurpleIMChat::createSessionCbk, data);
}

bool PurpleIMChat::closeSessionCbk(void * data)
{
	Mutex::ScopedLock lock(PurpleIMChat::_mutex);

	IMChatSession & chatSession = *(IMChatSession *)((misc_t *)data)->data_ptr1;
	mConvInfo_t *mConv = FindChatStructById(chatSession.getId());
	PurpleConversation *gConv = (PurpleConversation *)mConv->purple_conv_session;

	if (mConv != NULL)
	{
		purple_conversation_destroy(gConv);
		RemoveChatSessionFromList(chatSession.getId());
	}

	purple_timeout_remove(((misc_t *)data)->cbk_id);

	free((misc_t *)data);

	return TRUE;
}

void PurpleIMChat::closeSession(IMChatSession & chatSession)
{
	misc_t * data = (misc_t *) malloc(sizeof(misc_t));
	memset(data, 0, sizeof(misc_t));

	data->data_ptr1 = &chatSession;

	Mutex::ScopedLock lock(PurpleIMChat::_mutex);
	data->cbk_id = purple_timeout_add(0, (GSourceFunc) PurpleIMChat::closeSessionCbk, data);
}


bool PurpleIMChat::sendMessageCbk(void * data)
{
	Mutex::ScopedLock lock(PurpleIMChat::_mutex);

	IMChatSession & chatSession = *(IMChatSession *)((misc_t *)data)->data_ptr1;
	const char * message = (const char *)((misc_t *)data)->data_ptr2;
	char *cleanMess = (char *) message;
	bool cleaned = false;

	mConvInfo_t *mConv = FindChatStructById(chatSession.getId());
	PurpleConversation *gConv = (PurpleConversation *)mConv->purple_conv_session;

	// special case for ICQ
	PurpleAccount *gAccount = purple_conversation_get_account(gConv);
	PurplePlugin *prpl = purple_find_prpl(purple_account_get_protocol_id(gAccount));
	if (prpl)
	{
		PurplePluginProtocolInfo *prpl_info = PURPLE_PLUGIN_PROTOCOL_INFO(prpl);
		if (prpl_info->list_icon != NULL)
		{
			if (!strcmp("icq", prpl_info->list_icon(gAccount, NULL))) {
				cleanMess = (char *) purple_markup_strip_html(message);
				cleaned = true;
			}
		}
	}

	if (purple_conversation_get_type(gConv) == PURPLE_CONV_TYPE_IM)
	{
		purple_conv_im_send_with_flags(PURPLE_CONV_IM(gConv),
			cleanMess, PURPLE_MESSAGE_SEND);
	}
	else if (purple_conversation_get_type(gConv) == PURPLE_CONV_TYPE_CHAT)
	{
		purple_conv_chat_send_with_flags(PURPLE_CONV_CHAT(gConv),
			cleanMess, PURPLE_MESSAGE_SEND);
	}
	
	if (cleaned) {
		g_free(cleanMess);
	}

	purple_timeout_remove(((misc_t *)data)->cbk_id);

	free(((misc_t *)data)->data_ptr2);
	free((misc_t *)data);

	return TRUE;
}

void PurpleIMChat::sendMessage(IMChatSession & chatSession, const std::string & message)
{
	misc_t * data = (misc_t *) malloc(sizeof(misc_t));
	memset(data, 0, sizeof(misc_t));

	data->data_ptr1 = &chatSession;
	data->data_ptr2 = strdup(message.c_str());

	Mutex::ScopedLock lock(PurpleIMChat::_mutex);
	data->cbk_id = purple_timeout_add(0, (GSourceFunc) PurpleIMChat::sendMessageCbk, data);
}

bool PurpleIMChat::changeTypingStateCbk(void * data)
{
	Mutex::ScopedLock lock(PurpleIMChat::_mutex);

	IMChatSession & chatSession = *(IMChatSession *)((misc_t *)data)->data_ptr1;
	IMChat::TypingState state = (IMChat::TypingState) ((misc_t *)data)->data_int;
	PurpleTypingState gState = PURPLE_NOT_TYPING;
	mConvInfo_t *mConv = FindChatStructById(chatSession.getId());
	PurpleConversation *gConv = NULL;

	if (mConv) {
		if ((gConv = (PurpleConversation *)mConv->purple_conv_session)) {

			switch (state)
			{
				case IMChat::TypingStateTyping:
					gState = PURPLE_TYPING;
					break;

				case IMChat::TypingStateStopTyping:
				//gState = PURPLE_TYPED;
					gState = PURPLE_NOT_TYPING;
					break;

				default:
					gState = PURPLE_NOT_TYPING;
					break;
			}

			serv_send_typing(purple_conversation_get_gc(gConv),
				purple_conversation_get_name(gConv), gState);
		}
	}

	purple_timeout_remove(((misc_t *)data)->cbk_id);

	free((misc_t *)data);

	return TRUE;
}

void PurpleIMChat::changeTypingState(IMChatSession & chatSession, 
								   IMChat::TypingState state)
{
	misc_t * data = (misc_t *) malloc(sizeof(misc_t));
	memset(data, 0, sizeof(misc_t));

	data->data_ptr1 = &chatSession;
	data->data_int = state;

	Mutex::ScopedLock lock(PurpleIMChat::_mutex);
	data->cbk_id = purple_timeout_add(0, (GSourceFunc) PurpleIMChat::changeTypingStateCbk, data);
}

bool PurpleIMChat::addContactCbk(void * data)
{
	Mutex::ScopedLock lock(PurpleIMChat::_mutex);

	PurpleIMChat * imChat = ((misc_t *)data)->instance;
	IMChatSession & chatSession = *(IMChatSession *)((misc_t *)data)->data_ptr1;
	const char * contactId = (const char *)((misc_t *)data)->data_ptr2;
	mConvInfo_t *mConv = FindChatStructById(chatSession.getId());
	PurpleAccount *gAccount = purple_accounts_find(imChat->getIMAccount().getLogin().c_str(),
		PurpleIMPrcl::GetPrclId(imChat->getIMAccount().getProtocol()));
	int BuddyNbr = chatSession.getIMContactSet().size();
	PurpleConversation *gConv = NULL;

	if (mConv)
		gConv = (PurpleConversation *)mConv->purple_conv_session;
	else
		LOG_FATAL("ConvInfo not created !!!");

	if (purple_conversation_get_type(gConv) == PURPLE_CONV_TYPE_IM)
	{
		GList *mlist = NULL;
		IMContactSet &chatContact = (IMContactSet &)chatSession.getIMContactSet();
		IMContactSet::const_iterator it = chatContact.begin();
		const std::string & firstContactId = it->getContactId();
		PurpleConnection *gGC;

		gConv = (PurpleConversation *) mConv->purple_conv_session;
		gGC = purple_conversation_get_gc(gConv);
		mlist = g_list_append(mlist, (char *) contactId);
		mlist = g_list_append(mlist, (char *) firstContactId.c_str());

		createPurpleChat(gGC, chatSession.getId(), mlist, *imChat);
	}
	else if (purple_conversation_get_type(gConv) == PURPLE_CONV_TYPE_CHAT)
	{
		serv_chat_invite(purple_conversation_get_gc(gConv), purple_conv_chat_get_id(PURPLE_CONV_CHAT(gConv)),
			NULL, contactId);
	}

	purple_timeout_remove(((misc_t *)data)->cbk_id);

	free(((misc_t *)data)->data_ptr2);
	free((misc_t *)data);

	return TRUE;
}

void PurpleIMChat::addContact(IMChatSession & chatSession, const std::string & contactId)
{
	misc_t * data = (misc_t *) malloc(sizeof(misc_t));
	memset(data, 0, sizeof(misc_t));

	data->instance = this;
	data->data_ptr1 = &chatSession;
	data->data_ptr2 = strdup(contactId.c_str());

	Mutex::ScopedLock lock(PurpleIMChat::_mutex);
	data->cbk_id = purple_timeout_add(0, (GSourceFunc) PurpleIMChat::addContactCbk, data);
}

bool PurpleIMChat::removeContactCbk(void * data)
{
	Mutex::ScopedLock lock(PurpleIMChat::_mutex);

	IMChatSession & chatSession = *(IMChatSession *)((misc_t *)data)->data_ptr1;
	const char * contactId = (const char *)((misc_t *)data)->data_ptr2;
	mConvInfo_t *mConv = FindChatStructById(chatSession.getId());
	PurpleConvChat *conv = PURPLE_CONV_CHAT((PurpleConversation *)(mConv->purple_conv_session));

	purple_conv_chat_remove_user(conv, contactId, NULL);

	purple_timeout_remove(((misc_t *)data)->cbk_id);

	free(((misc_t *)data)->data_ptr2);
	free((misc_t *)data);

	return TRUE;
}

void PurpleIMChat::removeContact(IMChatSession & chatSession, const std::string & contactId)
{
	misc_t * data = (misc_t *) malloc(sizeof(misc_t));
	memset(data, 0, sizeof(misc_t));

	data->data_ptr1 = &chatSession;
	data->data_ptr2 = strdup(contactId.c_str());

	Mutex::ScopedLock lock(PurpleIMChat::_mutex);
	data->cbk_id = purple_timeout_add(0, (GSourceFunc) PurpleIMChat::removeContactCbk, data);
}

void PurpleIMChat::AddChatSessionInList(mConvInfo_t *conv)
{
	if (!IsChatSessionInList(conv->conv_id))
	{
		_PurpleChatSessionList.push_back(conv);
	}
}

void PurpleIMChat::RemoveChatSessionFromList(int convId)
{
	PurpleChatSessionIterator it;
	for (it = _PurpleChatSessionList.begin(); it != _PurpleChatSessionList.end(); it++)
	{
		if ((*it)->conv_id == convId)
		{
			_PurpleChatSessionList.erase(it);
			break;
		}
	}
}

mConvInfo_t *PurpleIMChat::FindChatStructById(int convId)
{
	PurpleChatSessionIterator it;
	for (it = _PurpleChatSessionList.begin(); it != _PurpleChatSessionList.end(); it++)
	{
		if ((*it)->conv_id == convId)
		{
			return (*it);
		}
	}

	return NULL;
}

void PurpleIMChat::createPurpleChat(PurpleConnection *gGC, int id, GList *users, PurpleIMChat & imChat)
{
	char chatName[100];
	mConvInfo_t *mConv = FindChatStructById(id);
	PurpleConversation *gConv;
	EnumIMProtocol::IMProtocol protocol = PurpleIMPrcl::GetEnumIMProtocol(gGC->account->protocol_id);

	if (mConv == NULL)
		mConv = CreateChatSession(true, imChat);

	snprintf(chatName, sizeof(chatName), "Chat%d", mConv->conv_id);

	if (protocol == EnumIMProtocol::IMProtocolMSN)
	{
		//serv_chat_create(gGC, chatName, users);
		gConv = purple_find_conversation_with_account(PURPLE_CONV_TYPE_CHAT, chatName, gGC->account);

		if (!gConv)
			LOG_FATAL("Chat doesn't exist !!");

		mConv->purple_conv_session = gConv;
		gConv->ui_data = mConv;
	}
	else
	{
		GHashTable *components;

		components = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);

		g_hash_table_replace(components, g_strdup("room"), g_strdup(chatName));

		if (protocol == EnumIMProtocol::IMProtocolYahoo)
		{
			g_hash_table_replace(components, g_strdup("topic"), g_strdup("Join my conference..."));
			g_hash_table_replace(components, g_strdup("type"), g_strdup("Conference"));
		}
		else if ((protocol == EnumIMProtocol::IMProtocolAIM)
			|| (protocol == EnumIMProtocol::IMProtocolICQ))
		{
			g_hash_table_replace(components, g_strdup("exchange"), g_strdup("16"));
		}

		mConv->pending_invit = users;

		serv_join_chat(gGC, components);
		g_hash_table_destroy(components);
	}
}

bool PurpleIMChat::IsChatSessionInList(int convId)
{
	return FindChatStructById(convId);
}
