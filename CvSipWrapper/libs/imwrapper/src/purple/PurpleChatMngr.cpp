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

#include "PurpleChatMngr.h"

#include "PurpleAccountMngr.h"
#include "PurpleEnumIMProtocol.h"
#include "PurpleIMFactory.h"

#include <imwrapper/IMChatSession.h>
#include <imwrapper/IMContact.h>

#include <util/Logger.h>

extern "C" {
#include <libpurple/util.h>
}

#ifdef OS_WINDOWS
	//#include <Windows.h>
	//#include <Winbase.h>
	#define snprintf _snprintf
	//#define sleep(i) Sleep(i * 1000)
//#else
	//#include <unistd.h>
#endif


/* **************** UTIL ****************** */
int GetPurpleConversationId(const char *name)
{
	int id = 0;
	char *str_id = (char *) name;

	if (name == NULL)
		return id;

	while (str_id && *str_id && (*str_id > '9' || *str_id < '0'))
		str_id++;

	if (*str_id == '\0')
		return id;

	id = strtol(str_id, (char **) NULL, 10);
	return id;
}


/* ***************** PURPLE CALLBACK ***************** */
void C_CreateIncomingConversationCbk(PurpleConversation *conv)
{
	PurpleChatMngr::CreateConversationCbk(conv, false);
}

void C_CreateConversationCbk(PurpleConversation *conv)
{
	PurpleChatMngr::CreateConversationCbk(conv, true);
}

static void C_DestroyConversationCbk(PurpleConversation *conv)
{
	PurpleChatMngr::DestroyConversationCbk(conv);
}

static void C_WriteChatCbk(PurpleConversation *conv, const char *who,
						const char *message, PurpleMessageFlags flags,
						time_t mtime)
{
	PurpleChatMngr::WriteChatCbk(conv, who, message, flags, mtime);
}

static void C_WriteIMCbk(PurpleConversation *conv, const char *who,
						const char *message, PurpleMessageFlags flags, time_t mtime)
{
	PurpleChatMngr::WriteIMCbk(conv, who, message, flags, mtime);
}

static void C_WriteConvCbk(PurpleConversation *conv, const char *name, const char *alias,
						const char *message, PurpleMessageFlags flags,
						time_t mtime)
{
	PurpleChatMngr::WriteConvCbk(conv, name, alias, message, flags, mtime);
}

static void C_ChatAddUsersCbk(PurpleConversation *conv, GList *users, gboolean new_arrivals)
{
	PurpleChatMngr::ChatAddUsersCbk(conv, users, new_arrivals);
}

static void C_ChatRenameUserCbk(PurpleConversation *conv, const char *old_name,
							const char *new_name, const char *new_alias)
{
	PurpleChatMngr::ChatRenameUserCbk(conv, old_name, new_name, new_alias);
}

static void C_ChatRemoveUsersCbk(PurpleConversation *conv, GList *users)
{
	PurpleChatMngr::ChatRemoveUsersCbk(conv, users);
}

static void C_ChatUpdateUserCbk(PurpleConversation *conv, const char *user)
{
	PurpleChatMngr::ChatUpdateUserCbk(conv, user);
}

void C_PresentConvCbk(PurpleConversation *conv)
{
    PurpleChatMngr::PresentConvCbk(conv);
}

gboolean C_HasFocusCbk(PurpleConversation *conv)
{
	return PurpleChatMngr::HasFocusCbk(conv);
}

static gboolean C_CustomSmileyAddCbk(PurpleConversation *conv, const char *smile, gboolean remote)
{
	return PurpleChatMngr::CustomSmileyAddCbk(conv, smile, remote);
}

static void C_CustomSmileyWriteCbk(PurpleConversation *conv, const char *smile,
								const guchar *data, gsize size)
{
	PurpleChatMngr::CustomSmileyWriteCbk(conv, smile, data, size);
}

static void C_CustomSmileyCloseCbk(PurpleConversation *conv, const char *smile)
{
	PurpleChatMngr::CustomSmileyCloseCbk(conv, smile);
}

static void C_SendConfirm(PurpleConversation *conv, const char *message)
{
	PurpleChatMngr::SendConfirm(conv, message);
}

PurpleConversationUiOps chat_wg_ops = {
	C_CreateConversationCbk,
	C_DestroyConversationCbk,	/* destroy_conversation */
	C_WriteChatCbk,				/* write_chat */
	C_WriteIMCbk,				/* write_im */
	C_WriteConvCbk,				/* write_conv */
	C_ChatAddUsersCbk,			/* chat_add_users */
	C_ChatRenameUserCbk,		/* chat_rename_user */
	C_ChatRemoveUsersCbk,		/* chat_remove_users */
	C_ChatUpdateUserCbk,		/* chat_update_user */
	C_PresentConvCbk,			/* present */
	C_HasFocusCbk,				/* has_focus */
	C_CustomSmileyAddCbk,		/* custom_smiley_add */
	C_CustomSmileyWriteCbk,		/* custom_smiley_write */
	C_CustomSmileyCloseCbk,		/* custom_smiley_close */
	C_SendConfirm,				/* send_confirm */
};


/* **************** MISCELLEANOUS CALLBACK ****************** */

void chat_joined_cb(PurpleConversation *conv)
{
	PurpleChatMngr::ChatJoinedCbk(conv);
}

int chat_invite_request_cb(PurpleAccount *account, const char *who,
						const char *message, void *data)
{
	return 1;
}

void update_buddy_typing_cb(PurpleAccount *account, const char *who)
{
	PurpleConversation *gConv = NULL;

	gConv = purple_find_conversation_with_account(PURPLE_CONV_TYPE_IM, who, account);
	if (!gConv)
		return;

	PurpleChatMngr::UpdateBuddyTyping(gConv, purple_conv_im_get_typing_state(PURPLE_CONV_IM(gConv)));
}

void received_im_msg_cb(PurpleAccount *account, char *sender, char *message,
						PurpleConversation *conv, int flags)
{
	PurpleConversationUiOps *ops = &chat_wg_ops;

	if (conv != NULL)
		return;

	ops->create_conversation = C_CreateIncomingConversationCbk;
	purple_conversation_new(PURPLE_CONV_TYPE_IM, account, sender);
	ops->create_conversation = C_CreateConversationCbk;
}

void init_chat_event()
{
	void *handle = purple_wg_get_handle();

	purple_signal_connect(purple_conversations_get_handle(), "buddy-typing",
		handle, PURPLE_CALLBACK(update_buddy_typing_cb), NULL);
	
	purple_signal_connect(purple_conversations_get_handle(), "buddy-typing-stopped",
		handle, PURPLE_CALLBACK(update_buddy_typing_cb), NULL);

	purple_signal_connect(purple_conversations_get_handle(), "chat-invited",
		handle, PURPLE_CALLBACK(chat_invite_request_cb), NULL);

	purple_signal_connect(purple_conversations_get_handle(), "chat-joined",
		handle, PURPLE_CALLBACK(chat_joined_cb), NULL);

	purple_signal_connect(purple_conversations_get_handle(), "chat-left",
		handle, PURPLE_CALLBACK(chat_joined_cb), NULL);

	purple_signal_connect(purple_conversations_get_handle(), "received-im-msg",
		handle, PURPLE_CALLBACK(received_im_msg_cb), NULL);
}

/* ************************************************** */

PurpleChatMngr * PurpleChatMngr::_staticInstance = NULL;
PurpleAccountMngr *PurpleChatMngr::_accountMngr = NULL;
std::list<PurpleIMChat *> PurpleChatMngr::_purpleIMChatList;
RecursiveMutex PurpleChatMngr::_mutex;

PurpleChatMngr::PurpleChatMngr()
{
}

void PurpleChatMngr::Init()
{
	_accountMngr = PurpleAccountMngr::getInstance();
	init_chat_event();
	purple_conversations_set_ui_ops(&chat_wg_ops);
}

PurpleChatMngr *PurpleChatMngr::getInstance()
{
	if (!_staticInstance)
	{
		_staticInstance = new PurpleChatMngr();
	}
	return _staticInstance;
}

void PurpleChatMngr::ChatJoinedCbk(PurpleConversation *conv)
{
	mConvInfo_t *mConv = (mConvInfo_t *) conv->ui_data;

	if (mConv->pending_invit)
	{
		for (GList *l = mConv->pending_invit; l != NULL; l = l->next)
		{
			serv_chat_invite(purple_conversation_get_gc(conv),
				purple_conv_chat_get_id(PURPLE_CONV_CHAT(conv)),
				"Join my conference...", (char *)l->data);
		}
	}
}

std::string PurpleChatMngr::cleanContactId(std::string contactId, EnumIMProtocol::IMProtocol prtcl)
{
	if (prtcl == EnumIMProtocol::IMProtocolJabber) {
		std::string::size_type index = contactId.find('/');

		if (index != std::string::npos)
			return contactId.substr(0, index);
	}
	
	return contactId;
}

void PurpleChatMngr::CreateConversationCbk(PurpleConversation *conv, bool userCreated)
{
	PurpleAccount *gAccount = purple_conversation_get_account(conv);
	PurpleConversationType chatType = purple_conversation_get_type(conv);
	const char *gPrclId = purple_account_get_protocol_id(gAccount);
	IMAccount *account = _accountMngr->FindIMAccount(purple_account_get_username(gAccount),
		PurpleIMPrcl::GetEnumIMProtocol(gPrclId));
	PurpleIMChat *mChat = FindIMChat(*account);
	mConvInfo_t *mConv;

	if (chatType == PURPLE_CONV_TYPE_IM)
	{
		IMChatSession *chatSession = NULL;

		// Check if it's a jabber contact, and remove his ressource
		std::string contactId = cleanContactId(std::string(purple_conversation_get_name(conv)),
			PurpleIMPrcl::GetEnumIMProtocol(gPrclId));

		IMContact imContact(*account, contactId);

		mConv = mChat->CreateChatSession(userCreated, *mChat);
		mConv->purple_conv_session = conv;
 		conv->ui_data = mConv;

		chatSession = (IMChatSession *) mConv->conv_session;

		((IMContactSet &) chatSession->getIMContactSet()).insert(imContact);
		mChat->newIMChatSessionCreatedEvent(*mChat, *chatSession);

		mChat->contactAddedEvent(*mChat, *chatSession, imContact);
	}
	else if (chatType == PURPLE_CONV_TYPE_CHAT)
	{
		int id = GetPurpleConversationId(purple_conversation_get_name(conv));

		if ((mConv = mChat->FindChatStructById(id)) == NULL)
		{
			mConv = mChat->CreateChatSession(userCreated, *mChat);
		}

		if (mConv->purple_conv_session)
			purple_conversation_destroy((PurpleConversation *)mConv->purple_conv_session);

		mConv->purple_conv_session = conv;
		conv->ui_data = mConv;
	}
}

void PurpleChatMngr::DestroyConversationCbk(PurpleConversation *conv)
{
	// TODO: free (mConvInfo_t *) conv->ui_data;
	LOG_DEBUG("wgconv : purple_wgconv_destroy()\n");

}

void PurpleChatMngr::WriteChatCbk(PurpleConversation *conv, const char *who,
							const char *message, PurpleMessageFlags flags,
							time_t mtime)
{
	purple_conversation_write(conv, who, message, flags, mtime);
}

void PurpleChatMngr::WriteIMCbk(PurpleConversation *conv, const char *who,
							const char *message, PurpleMessageFlags flags,
							time_t mtime)
{
	purple_conversation_write(conv, who, message, flags, mtime);
}

void PurpleChatMngr::WriteConvCbk(PurpleConversation *conv, const char *name, const char *alias,
							const char *message, PurpleMessageFlags flags,
							time_t mtime)
{
	if ((flags & PURPLE_MESSAGE_RECV))
	{
		mConvInfo_t *mConv = (mConvInfo_t *)conv->ui_data;
		PurpleIMChat *mIMChat = FindIMChatByPurpleConv(conv);

		// Prevent from double message during a chat session
		if (strcmp(purple_account_get_username(conv->account), name) == 0)
			return;

		// Check if it's a jabber contact, and remove his ressource
		PurpleAccount *gAccount = purple_conversation_get_account(conv);
		const char *gPrclId = purple_account_get_protocol_id(gAccount);
		std::string contactId = cleanContactId(name,
			PurpleIMPrcl::GetEnumIMProtocol(gPrclId));

		mIMChat->messageReceivedEvent(*mIMChat, 
			*((IMChatSession *)(mConv->conv_session)),
			contactId, std::string(message));
	}
}

void PurpleChatMngr::ChatAddUsersCbk(PurpleConversation *conv, GList *users, gboolean new_arrivals)
{
	GList *l;
	PurpleAccount *gAccount = purple_conversation_get_account(conv);
	const char *gPrclId = purple_account_get_protocol_id(gAccount);
	IMAccount *account = _accountMngr->FindIMAccount(purple_account_get_username(gAccount),
								PurpleIMPrcl::GetEnumIMProtocol(gPrclId));
	mConvInfo_t *mConv = NULL;
	IMChatSession *chatSession = NULL;
	PurpleIMChat *mChat = FindIMChat(*account);

	if (!mChat)
		LOG_FATAL("Can't find IMChat !");

	mConv = (mConvInfo_t *) conv->ui_data;
	chatSession = (IMChatSession *) mConv->conv_session;

	for (l = users; l != NULL; l = l->next)
	{
		PurpleConvChatBuddy *gCCBuddy = (PurpleConvChatBuddy *) l->data;

		if (gCCBuddy && strcmp(purple_account_get_username(gAccount), (char *) gCCBuddy->name))
		{
			std::string buddy((char *) gCCBuddy->name);
			IMContact imContact(*account, buddy);

			if (chatSession->getIMContactSet().find(imContact) != chatSession->getIMContactSet().end())
			{
				LOG_DEBUG("IMContact for " + imContact.getContactId() + " already in IMContactSet");
			}
			else
			{
				((IMContactSet &) chatSession->getIMContactSet()).insert(imContact);

				if (chatSession->getIMContactSet().size() == 1)
					mChat->newIMChatSessionCreatedEvent(*mChat, *chatSession);

				LOG_DEBUG("IMContact " + imContact.getContactId() + " added to IMContactSet");
			}
			mChat->contactAddedEvent(*mChat, *chatSession, imContact);
		}
	}
}

void PurpleChatMngr::ChatRenameUserCbk(PurpleConversation *conv, const char *old_name,
									const char *new_name, const char *new_alias)
{
}

void PurpleChatMngr::ChatRemoveUsersCbk(PurpleConversation *conv, GList *users)
{
	GList *l;
	PurpleAccount *gAccount = purple_conversation_get_account(conv);
	const char *gPrclId = purple_account_get_protocol_id(gAccount);
	IMAccount *account = _accountMngr->FindIMAccount(purple_account_get_username(gAccount),
								PurpleIMPrcl::GetEnumIMProtocol(gPrclId));
	mConvInfo_t *mConv = NULL;
	IMChatSession *chatSession = NULL;
	PurpleIMChat *mChat = FindIMChat(*account);

	if (!mChat)
		LOG_FATAL("Can't find IMChat !");

	mConv = (mConvInfo_t *) conv->ui_data;
	chatSession = (IMChatSession *) mConv->conv_session;

	for (l = users; l != NULL; l = l->next)
	{
		if (strcmp(purple_account_get_username(gAccount), (char *) l->data))
		{
			std::string buddy((char *) l->data);
			IMContact imContact(*account, buddy);

			if (chatSession->getIMContactSet().find(imContact) == chatSession->getIMContactSet().end())
			{
				LOG_ERROR("IMContact for " + imContact.getContactId() + " not in IMContactSet");
			}
			else
			{
				LOG_DEBUG("IMContact " + imContact.getContactId() + " removed from IMContactSet");
				mChat->contactRemovedEvent(*mChat, *chatSession, buddy);
			}
		}
	}
}

void PurpleChatMngr::ChatUpdateUserCbk(PurpleConversation *conv, const char *user)
{
}

void PurpleChatMngr::PresentConvCbk(PurpleConversation *conv)
{
}

gboolean PurpleChatMngr::HasFocusCbk(PurpleConversation *conv)
{
	return TRUE;
}

gboolean PurpleChatMngr::CustomSmileyAddCbk(PurpleConversation *conv, const char *smile,
									   gboolean remote)
{
	return TRUE;
}

void PurpleChatMngr::CustomSmileyWriteCbk(PurpleConversation *conv, const char *smile,
									const guchar *data, gsize size)
{
}

void PurpleChatMngr::CustomSmileyCloseCbk(PurpleConversation *conv, const char *smile)
{
}

void PurpleChatMngr::SendConfirm(PurpleConversation *conv, const char *message)
{
}


void PurpleChatMngr::UpdateBuddyTyping(PurpleConversation *conv, PurpleTypingState state)
{
	IMChat::TypingState mState;

	mConvInfo_t *mConv = (mConvInfo_t *)conv->ui_data;
	PurpleIMChat *mIMChat = FindIMChatByPurpleConv(conv);

	if (!mIMChat)
		return;

	switch (state)
	{
		case PURPLE_TYPING:
			mState = IMChat::TypingStateTyping;
			break;

		case PURPLE_TYPED:
			mState = IMChat::TypingStateStopTyping;
			break;

		default:
			mState = IMChat::TypingStateNotTyping;
			break;
	}

	// Get contact id. If it's a jabber contact, remove its ressource.
	const char *name = purple_conversation_get_name(conv);
	PurpleAccount *gAccount = purple_conversation_get_account(conv);
	const char *gPrclId = purple_account_get_protocol_id(gAccount);
	std::string contactId = cleanContactId(name,
		PurpleIMPrcl::GetEnumIMProtocol(gPrclId));

	mIMChat->typingStateChangedEvent(*mIMChat, *((IMChatSession *)(mConv->conv_session)),
									contactId, mState);

}

/* **************** IMWRAPPER/PURPLE INTERFACE ****************** */
PurpleIMChat *PurpleChatMngr::FindIMChatByPurpleConv(void *gConv)
{
	PurpleAccount *gAccount = purple_conversation_get_account((PurpleConversation *) gConv);
	const char *gPrclId = purple_account_get_protocol_id(gAccount);
	IMAccount *account = _accountMngr->FindIMAccount(purple_account_get_username(gAccount),
													PurpleIMPrcl::GetEnumIMProtocol(gPrclId));
	PurpleIMChat *mChat = FindIMChat(*account);

	return mChat;
}

/* **************** MANAGE CHAT_LIST ****************** */
PurpleIMChat *PurpleChatMngr::FindIMChat(IMAccount &account)
{
	RecursiveMutex::ScopedLock lock(_mutex);

	PurpleIMChatIterator i;
	for (i = _purpleIMChatList.begin(); i != _purpleIMChatList.end(); i++)
	{
		if (PurpleIMFactory::equals((*i)->getIMAccount(), account.getLogin(), account.getProtocol()))
		{
			return (*i);
		}
	}

	return NULL;
}

PurpleIMChat *PurpleChatMngr::AddIMChat(IMAccount &account)
{
	RecursiveMutex::ScopedLock lock(_mutex);

	PurpleIMChat *mIMChat = FindIMChat(account);

	if (mIMChat == NULL)
	{
		mIMChat = new PurpleIMChat(account);

		_purpleIMChatList.push_back(mIMChat);
	}

	return mIMChat;
}

void PurpleChatMngr::RemoveIMChat(IMAccount &account)
{
	RecursiveMutex::ScopedLock lock(_mutex);

	PurpleIMChatIterator i;
	for (i = _purpleIMChatList.begin(); i != _purpleIMChatList.end(); i++)
	{
		if (PurpleIMFactory::equals((*i)->getIMAccount(), account.getLogin(), account.getProtocol()))
		{
			_purpleIMChatList.erase(i);
			break;
		}
	}
}

void PurpleChatMngr::UpdateIMChat(IMAccount &account)
{
	RecursiveMutex::ScopedLock lock(_mutex);

	PurpleIMChatIterator i;
	for (i = _purpleIMChatList.begin(); i != _purpleIMChatList.end(); i++)
	{
		if (PurpleIMFactory::equals((*i)->getIMAccount(), account.getLogin(), account.getProtocol()))
		{
			(*i)->setIMAccount(account);
			break;
		}
	}
}

void PurpleChatMngr::reset() {
	RecursiveMutex::ScopedLock lock(_mutex);

	_purpleIMChatList.clear();
}

/* ****************************************************** */
