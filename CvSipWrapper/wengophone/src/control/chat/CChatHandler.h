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

#ifndef OWCCHATHANDLER_H
#define OWCCHATHANDLER_H

#include <control/Control.h>

class Presentation;
class ChatHandler;
class CUserProfile;
class CWengoPhone;
class IMAccount;
class IMContactSet;
class IMChatSession;
class PChatHandler;

/**
 *
 * @ingroup control
 * @author Tanguy Krotoff
 * @author Philippe Bernery
 */
class CChatHandler : public Control {
public:

	CChatHandler(ChatHandler & chatHandler, CUserProfile & cUserProfile);

	~CChatHandler();

	Presentation * getPresentation() const;

	CWengoPhone & getCWengoPhone() const;

	CUserProfile & getCUserProfile() const;

	/**
	 * @see ChatHandler::createSession()
	 */
	void createSession(const std::string & imAccountId, const IMContactSet & imContactSet);

private:

	void initPresentationThreadSafe();

	void newIMChatSessionCreatedEventHandler(ChatHandler & sender, IMChatSession & imChatSession);

	void newIMChatSessionCreatedEventHandlerThreadSafe(IMChatSession & imChatSession);

	ChatHandler & _chatHandler;

	PChatHandler * _pChatHandler;

	CUserProfile & _cUserProfile;
};

#endif	//OWCCHATHANDLER_H
