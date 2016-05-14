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

#include "PhoneCallStateIncoming.h"

#include "PhoneCall.h"

#include <model/config/ConfigManager.h>
#include <model/config/Config.h>
#include <model/contactlist/ContactList.h>
#include <model/wenbox/WenboxPlugin.h>

#include <sound/Sound.h>

void PhoneCallStateIncoming::execute(PhoneCall & phoneCall,bool doublecall) {

	Config & config = ConfigManager::getInstance().getCurrentConfig();
	if (config.getAudioRingingEnable()) {		
		/**   释放音频资源的时候经常出现指针异常，怀疑跟线程同步有关，先屏蔽掉  zhenHua.sun 2010-10-28
		//Ringin tonality
		if(doublecall)
			_soundIncomingCall = new Sound(getSoundDoubleCallFile());
		else
			_soundIncomingCall = new Sound(getSoundIncomingCallFile());

		_soundIncomingCall->setWaveOutDevice(getRingerAudioDevice());
		//Play the sound indefinitely
		_soundIncomingCall->setLoops(-1);
		_soundIncomingCall->play();
		*/
	}

	WenboxPlugin & wenboxPlugin = phoneCall.getWenboxPlugin();
	wenboxPlugin.setState(Wenbox::CallIncoming, phoneCall.getPeerSipAddress().toString());

	//Rejects/accepts the incoming call
	//Shows incoming call popup window
}
