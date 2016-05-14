#ifndef JABBER_SERVICE_COMMON_H
#define JABBER_SERVICE_COMMON_H

#define JABBER_SERVICE_NAME "com.dcampus.coolview.im.jabber"
#define JABBER_OBJECT_PATH	"/com/dcampus/coolview/im/jabber"
#define JABBER_SERVER		"im.ccnl.scut.edu.cn"
enum JabberAccountState
{
	StateClosed = 0,
	StateOk = 1,
	StateProgress = 2,
	StateAuthenticationError = 3,
	StateServerError = 4,
	StateTimeout = 5,
	StateUnknown = 6
};

#endif