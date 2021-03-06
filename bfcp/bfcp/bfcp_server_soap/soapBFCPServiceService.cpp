/* soapBFCPServiceService.cpp
   Generated by gSOAP 2.8.21 from server.h

Copyright(C) 2000-2014, Robert van Engelen, Genivia Inc. All Rights Reserved.
The generated code is released under one of the following licenses:
GPL or Genivia's license for commercial use.
This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
*/

#include "soapBFCPServiceService.h"

BFCPServiceService::BFCPServiceService()
{	BFCPServiceService_init(SOAP_IO_DEFAULT, SOAP_IO_DEFAULT);
}

BFCPServiceService::BFCPServiceService(const struct soap &_soap) : soap(_soap)
{ }

BFCPServiceService::BFCPServiceService(soap_mode iomode)
{	BFCPServiceService_init(iomode, iomode);
}

BFCPServiceService::BFCPServiceService(soap_mode imode, soap_mode omode)
{	BFCPServiceService_init(imode, omode);
}

BFCPServiceService::~BFCPServiceService()
{ }

void BFCPServiceService::BFCPServiceService_init(soap_mode imode, soap_mode omode)
{	soap_imode(this, imode);
	soap_omode(this, omode);
	static const struct Namespace namespaces[] =
{
	{"SOAP-ENV", "http://schemas.xmlsoap.org/soap/envelope/", "http://www.w3.org/*/soap-envelope", NULL},
	{"SOAP-ENC", "http://schemas.xmlsoap.org/soap/encoding/", "http://www.w3.org/*/soap-encoding", NULL},
	{"xsi", "http://www.w3.org/2001/XMLSchema-instance", "http://www.w3.org/*/XMLSchema-instance", NULL},
	{"xsd", "http://www.w3.org/2001/XMLSchema", "http://www.w3.org/*/XMLSchema", NULL},
	{"ns", "urn:BFCPService", NULL, NULL},
	{NULL, NULL, NULL, NULL}
};
	soap_set_namespaces(this, namespaces);
}

void BFCPServiceService::destroy()
{	soap_destroy(this);
	soap_end(this);
}

void BFCPServiceService::reset()
{	destroy();
	soap_done(this);
	soap_initialize(this);
	BFCPServiceService_init(SOAP_IO_DEFAULT, SOAP_IO_DEFAULT);
}

#ifndef WITH_PURE_VIRTUAL
BFCPServiceService *BFCPServiceService::copy()
{	BFCPServiceService *dup = SOAP_NEW_COPY(BFCPServiceService(*(struct soap*)this));
	return dup;
}
#endif

int BFCPServiceService::soap_close_socket()
{	return soap_closesock(this);
}

int BFCPServiceService::soap_force_close_socket()
{	return soap_force_closesock(this);
}

int BFCPServiceService::soap_senderfault(const char *string, const char *detailXML)
{	return ::soap_sender_fault(this, string, detailXML);
}

int BFCPServiceService::soap_senderfault(const char *subcodeQName, const char *string, const char *detailXML)
{	return ::soap_sender_fault_subcode(this, subcodeQName, string, detailXML);
}

int BFCPServiceService::soap_receiverfault(const char *string, const char *detailXML)
{	return ::soap_receiver_fault(this, string, detailXML);
}

int BFCPServiceService::soap_receiverfault(const char *subcodeQName, const char *string, const char *detailXML)
{	return ::soap_receiver_fault_subcode(this, subcodeQName, string, detailXML);
}

void BFCPServiceService::soap_print_fault(FILE *fd)
{	::soap_print_fault(this, fd);
}

#ifndef WITH_LEAN
#ifndef WITH_COMPAT
void BFCPServiceService::soap_stream_fault(std::ostream& os)
{	::soap_stream_fault(this, os);
}
#endif

char *BFCPServiceService::soap_sprint_fault(char *buf, size_t len)
{	return ::soap_sprint_fault(this, buf, len);
}
#endif

void BFCPServiceService::soap_noheader()
{	this->header = NULL;
}

const SOAP_ENV__Header *BFCPServiceService::soap_header()
{	return this->header;
}

int BFCPServiceService::run(int port)
{	if (soap_valid_socket(this->master) || soap_valid_socket(bind(NULL, port, 100)))
	{	for (;;)
		{	if (!soap_valid_socket(accept()) || serve())
				break;
			soap_destroy(this);
			soap_end(this);
		}
	}
	return this->error;
}

SOAP_SOCKET BFCPServiceService::bind(const char *host, int port, int backlog)
{	return soap_bind(this, host, port, backlog);
}

SOAP_SOCKET BFCPServiceService::accept()
{	return soap_accept(this);
}

#if defined(WITH_OPENSSL) || defined(WITH_GNUTLS)
int BFCPServiceService::ssl_accept()
{	return soap_ssl_accept(this);
}
#endif

int BFCPServiceService::serve()
{
#ifndef WITH_FASTCGI
	unsigned int k = this->max_keep_alive;
#endif
	do
	{

#ifndef WITH_FASTCGI
		if (this->max_keep_alive > 0 && !--k)
			this->keep_alive = 0;
#endif

		if (soap_begin_serve(this))
		{	if (this->error >= SOAP_STOP)
				continue;
			return this->error;
		}
		if (dispatch() || (this->fserveloop && this->fserveloop(this)))
		{
#ifdef WITH_FASTCGI
			soap_send_fault(this);
#else
			return soap_send_fault(this);
#endif
		}

#ifdef WITH_FASTCGI
		soap_destroy(this);
		soap_end(this);
	} while (1);
#else
	} while (this->keep_alive);
#endif
	return SOAP_OK;
}

static int serve_ns__start(BFCPServiceService*);
static int serve_ns__stop(BFCPServiceService*);
static int serve_ns__quit(BFCPServiceService*);
static int serve_ns__addConference(BFCPServiceService*);
static int serve_ns__removeConference(BFCPServiceService*);
static int serve_ns__modifyConference(BFCPServiceService*);
static int serve_ns__addFloor(BFCPServiceService*);
static int serve_ns__removeFloor(BFCPServiceService*);
static int serve_ns__modifyFloor(BFCPServiceService*);
static int serve_ns__addUser(BFCPServiceService*);
static int serve_ns__removeUser(BFCPServiceService*);
static int serve_ns__setChair(BFCPServiceService*);
static int serve_ns__removeChair(BFCPServiceService*);
static int serve_ns__getConferenceIDs(BFCPServiceService*);
static int serve_ns__getConferenceInfo(BFCPServiceService*);

int BFCPServiceService::dispatch()
{	soap_peek_element(this);
	if (!soap_match_tag(this, this->tag, "ns:start"))
		return serve_ns__start(this);
	if (!soap_match_tag(this, this->tag, "ns:stop"))
		return serve_ns__stop(this);
	if (!soap_match_tag(this, this->tag, "ns:quit"))
		return serve_ns__quit(this);
	if (!soap_match_tag(this, this->tag, "ns:addConference"))
		return serve_ns__addConference(this);
	if (!soap_match_tag(this, this->tag, "ns:removeConference"))
		return serve_ns__removeConference(this);
	if (!soap_match_tag(this, this->tag, "ns:modifyConference"))
		return serve_ns__modifyConference(this);
	if (!soap_match_tag(this, this->tag, "ns:addFloor"))
		return serve_ns__addFloor(this);
	if (!soap_match_tag(this, this->tag, "ns:removeFloor"))
		return serve_ns__removeFloor(this);
	if (!soap_match_tag(this, this->tag, "ns:modifyFloor"))
		return serve_ns__modifyFloor(this);
	if (!soap_match_tag(this, this->tag, "ns:addUser"))
		return serve_ns__addUser(this);
	if (!soap_match_tag(this, this->tag, "ns:removeUser"))
		return serve_ns__removeUser(this);
	if (!soap_match_tag(this, this->tag, "ns:setChair"))
		return serve_ns__setChair(this);
	if (!soap_match_tag(this, this->tag, "ns:removeChair"))
		return serve_ns__removeChair(this);
	if (!soap_match_tag(this, this->tag, "ns:getConferenceIDs"))
		return serve_ns__getConferenceIDs(this);
	if (!soap_match_tag(this, this->tag, "ns:getConferenceInfo"))
		return serve_ns__getConferenceInfo(this);
	return this->error = SOAP_NO_METHOD;
}

static int serve_ns__start(BFCPServiceService *soap)
{	struct ns__start soap_tmp_ns__start;
	struct ns__startResponse soap_tmp_ns__startResponse;
	enum ns__ErrorCode soap_tmp_ns__ErrorCode;
	soap_default_ns__startResponse(soap, &soap_tmp_ns__startResponse);
	soap_default_ns__ErrorCode(soap, &soap_tmp_ns__ErrorCode);
	soap_tmp_ns__startResponse.errorCode = &soap_tmp_ns__ErrorCode;
	soap_default_ns__start(soap, &soap_tmp_ns__start);
	if (!soap_get_ns__start(soap, &soap_tmp_ns__start, "ns:start", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = soap->start(soap_tmp_ns__start.af, soap_tmp_ns__start.port, soap_tmp_ns__start.enbaleConnectionThread, soap_tmp_ns__start.workThreadNum, soap_tmp_ns__start.userObsoletedTime, soap_tmp_ns__startResponse.errorCode);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = "";
	soap_serializeheader(soap);
	soap_serialize_ns__startResponse(soap, &soap_tmp_ns__startResponse);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns__startResponse(soap, &soap_tmp_ns__startResponse, "ns:startResponse", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns__startResponse(soap, &soap_tmp_ns__startResponse, "ns:startResponse", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

static int serve_ns__stop(BFCPServiceService *soap)
{	struct ns__stop soap_tmp_ns__stop;
	struct ns__stopResponse soap_tmp_ns__stopResponse;
	enum ns__ErrorCode soap_tmp_ns__ErrorCode;
	soap_default_ns__stopResponse(soap, &soap_tmp_ns__stopResponse);
	soap_default_ns__ErrorCode(soap, &soap_tmp_ns__ErrorCode);
	soap_tmp_ns__stopResponse.errorCode = &soap_tmp_ns__ErrorCode;
	soap_default_ns__stop(soap, &soap_tmp_ns__stop);
	if (!soap_get_ns__stop(soap, &soap_tmp_ns__stop, "ns:stop", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = soap->stop(soap_tmp_ns__stopResponse.errorCode);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = "";
	soap_serializeheader(soap);
	soap_serialize_ns__stopResponse(soap, &soap_tmp_ns__stopResponse);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns__stopResponse(soap, &soap_tmp_ns__stopResponse, "ns:stopResponse", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns__stopResponse(soap, &soap_tmp_ns__stopResponse, "ns:stopResponse", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

static int serve_ns__quit(BFCPServiceService *soap)
{	struct ns__quit soap_tmp_ns__quit;
	soap_default_ns__quit(soap, &soap_tmp_ns__quit);
	if (!soap_get_ns__quit(soap, &soap_tmp_ns__quit, "ns:quit", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = soap->quit();
	if (soap->error)
		return soap->error;
	return soap_closesock(soap);
}

static int serve_ns__addConference(BFCPServiceService *soap)
{	struct ns__addConference soap_tmp_ns__addConference;
	struct ns__addConferenceResponse soap_tmp_ns__addConferenceResponse;
	enum ns__ErrorCode soap_tmp_ns__ErrorCode;
	soap_default_ns__addConferenceResponse(soap, &soap_tmp_ns__addConferenceResponse);
	soap_default_ns__ErrorCode(soap, &soap_tmp_ns__ErrorCode);
	soap_tmp_ns__addConferenceResponse.errorCode = &soap_tmp_ns__ErrorCode;
	soap_default_ns__addConference(soap, &soap_tmp_ns__addConference);
	if (!soap_get_ns__addConference(soap, &soap_tmp_ns__addConference, "ns:addConference", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = soap->addConference(soap_tmp_ns__addConference.conferenceID, soap_tmp_ns__addConference.maxFloorRequest, soap_tmp_ns__addConference.policy, soap_tmp_ns__addConference.timeForChairAction, soap_tmp_ns__addConferenceResponse.errorCode);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = "";
	soap_serializeheader(soap);
	soap_serialize_ns__addConferenceResponse(soap, &soap_tmp_ns__addConferenceResponse);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns__addConferenceResponse(soap, &soap_tmp_ns__addConferenceResponse, "ns:addConferenceResponse", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns__addConferenceResponse(soap, &soap_tmp_ns__addConferenceResponse, "ns:addConferenceResponse", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

static int serve_ns__removeConference(BFCPServiceService *soap)
{	struct ns__removeConference soap_tmp_ns__removeConference;
	struct ns__removeConferenceResponse soap_tmp_ns__removeConferenceResponse;
	enum ns__ErrorCode soap_tmp_ns__ErrorCode;
	soap_default_ns__removeConferenceResponse(soap, &soap_tmp_ns__removeConferenceResponse);
	soap_default_ns__ErrorCode(soap, &soap_tmp_ns__ErrorCode);
	soap_tmp_ns__removeConferenceResponse.errorCode = &soap_tmp_ns__ErrorCode;
	soap_default_ns__removeConference(soap, &soap_tmp_ns__removeConference);
	if (!soap_get_ns__removeConference(soap, &soap_tmp_ns__removeConference, "ns:removeConference", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = soap->removeConference(soap_tmp_ns__removeConference.conferenceID, soap_tmp_ns__removeConferenceResponse.errorCode);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = "";
	soap_serializeheader(soap);
	soap_serialize_ns__removeConferenceResponse(soap, &soap_tmp_ns__removeConferenceResponse);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns__removeConferenceResponse(soap, &soap_tmp_ns__removeConferenceResponse, "ns:removeConferenceResponse", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns__removeConferenceResponse(soap, &soap_tmp_ns__removeConferenceResponse, "ns:removeConferenceResponse", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

static int serve_ns__modifyConference(BFCPServiceService *soap)
{	struct ns__modifyConference soap_tmp_ns__modifyConference;
	struct ns__modifyConferenceResponse soap_tmp_ns__modifyConferenceResponse;
	enum ns__ErrorCode soap_tmp_ns__ErrorCode;
	soap_default_ns__modifyConferenceResponse(soap, &soap_tmp_ns__modifyConferenceResponse);
	soap_default_ns__ErrorCode(soap, &soap_tmp_ns__ErrorCode);
	soap_tmp_ns__modifyConferenceResponse.errorCode = &soap_tmp_ns__ErrorCode;
	soap_default_ns__modifyConference(soap, &soap_tmp_ns__modifyConference);
	if (!soap_get_ns__modifyConference(soap, &soap_tmp_ns__modifyConference, "ns:modifyConference", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = soap->modifyConference(soap_tmp_ns__modifyConference.conferenceID, soap_tmp_ns__modifyConference.maxFloorRequest, soap_tmp_ns__modifyConference.policy, soap_tmp_ns__modifyConference.timeForChairAction, soap_tmp_ns__modifyConferenceResponse.errorCode);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = "";
	soap_serializeheader(soap);
	soap_serialize_ns__modifyConferenceResponse(soap, &soap_tmp_ns__modifyConferenceResponse);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns__modifyConferenceResponse(soap, &soap_tmp_ns__modifyConferenceResponse, "ns:modifyConferenceResponse", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns__modifyConferenceResponse(soap, &soap_tmp_ns__modifyConferenceResponse, "ns:modifyConferenceResponse", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

static int serve_ns__addFloor(BFCPServiceService *soap)
{	struct ns__addFloor soap_tmp_ns__addFloor;
	struct ns__addFloorResponse soap_tmp_ns__addFloorResponse;
	enum ns__ErrorCode soap_tmp_ns__ErrorCode;
	soap_default_ns__addFloorResponse(soap, &soap_tmp_ns__addFloorResponse);
	soap_default_ns__ErrorCode(soap, &soap_tmp_ns__ErrorCode);
	soap_tmp_ns__addFloorResponse.errorCode = &soap_tmp_ns__ErrorCode;
	soap_default_ns__addFloor(soap, &soap_tmp_ns__addFloor);
	if (!soap_get_ns__addFloor(soap, &soap_tmp_ns__addFloor, "ns:addFloor", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = soap->addFloor(soap_tmp_ns__addFloor.conferenceID, soap_tmp_ns__addFloor.floorID, soap_tmp_ns__addFloor.maxGrantedNum, soap_tmp_ns__addFloor.maxHoldingTime, soap_tmp_ns__addFloorResponse.errorCode);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = "";
	soap_serializeheader(soap);
	soap_serialize_ns__addFloorResponse(soap, &soap_tmp_ns__addFloorResponse);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns__addFloorResponse(soap, &soap_tmp_ns__addFloorResponse, "ns:addFloorResponse", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns__addFloorResponse(soap, &soap_tmp_ns__addFloorResponse, "ns:addFloorResponse", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

static int serve_ns__removeFloor(BFCPServiceService *soap)
{	struct ns__removeFloor soap_tmp_ns__removeFloor;
	struct ns__removeFloorResponse soap_tmp_ns__removeFloorResponse;
	enum ns__ErrorCode soap_tmp_ns__ErrorCode;
	soap_default_ns__removeFloorResponse(soap, &soap_tmp_ns__removeFloorResponse);
	soap_default_ns__ErrorCode(soap, &soap_tmp_ns__ErrorCode);
	soap_tmp_ns__removeFloorResponse.errorCode = &soap_tmp_ns__ErrorCode;
	soap_default_ns__removeFloor(soap, &soap_tmp_ns__removeFloor);
	if (!soap_get_ns__removeFloor(soap, &soap_tmp_ns__removeFloor, "ns:removeFloor", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = soap->removeFloor(soap_tmp_ns__removeFloor.conferenceID, soap_tmp_ns__removeFloor.floorID, soap_tmp_ns__removeFloorResponse.errorCode);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = "";
	soap_serializeheader(soap);
	soap_serialize_ns__removeFloorResponse(soap, &soap_tmp_ns__removeFloorResponse);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns__removeFloorResponse(soap, &soap_tmp_ns__removeFloorResponse, "ns:removeFloorResponse", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns__removeFloorResponse(soap, &soap_tmp_ns__removeFloorResponse, "ns:removeFloorResponse", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

static int serve_ns__modifyFloor(BFCPServiceService *soap)
{	struct ns__modifyFloor soap_tmp_ns__modifyFloor;
	struct ns__modifyFloorResponse soap_tmp_ns__modifyFloorResponse;
	enum ns__ErrorCode soap_tmp_ns__ErrorCode;
	soap_default_ns__modifyFloorResponse(soap, &soap_tmp_ns__modifyFloorResponse);
	soap_default_ns__ErrorCode(soap, &soap_tmp_ns__ErrorCode);
	soap_tmp_ns__modifyFloorResponse.errorCode = &soap_tmp_ns__ErrorCode;
	soap_default_ns__modifyFloor(soap, &soap_tmp_ns__modifyFloor);
	if (!soap_get_ns__modifyFloor(soap, &soap_tmp_ns__modifyFloor, "ns:modifyFloor", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = soap->modifyFloor(soap_tmp_ns__modifyFloor.conferenceID, soap_tmp_ns__modifyFloor.floorID, soap_tmp_ns__modifyFloor.maxGrantedNum, soap_tmp_ns__modifyFloor.maxHoldingTime, soap_tmp_ns__modifyFloorResponse.errorCode);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = "";
	soap_serializeheader(soap);
	soap_serialize_ns__modifyFloorResponse(soap, &soap_tmp_ns__modifyFloorResponse);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns__modifyFloorResponse(soap, &soap_tmp_ns__modifyFloorResponse, "ns:modifyFloorResponse", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns__modifyFloorResponse(soap, &soap_tmp_ns__modifyFloorResponse, "ns:modifyFloorResponse", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

static int serve_ns__addUser(BFCPServiceService *soap)
{	struct ns__addUser soap_tmp_ns__addUser;
	struct ns__addUserResponse soap_tmp_ns__addUserResponse;
	enum ns__ErrorCode soap_tmp_ns__ErrorCode;
	soap_default_ns__addUserResponse(soap, &soap_tmp_ns__addUserResponse);
	soap_default_ns__ErrorCode(soap, &soap_tmp_ns__ErrorCode);
	soap_tmp_ns__addUserResponse.errorCode = &soap_tmp_ns__ErrorCode;
	soap_default_ns__addUser(soap, &soap_tmp_ns__addUser);
	if (!soap_get_ns__addUser(soap, &soap_tmp_ns__addUser, "ns:addUser", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = soap->addUser(soap_tmp_ns__addUser.conferenceID, soap_tmp_ns__addUser.userID, soap_tmp_ns__addUser.userName, soap_tmp_ns__addUser.userURI, soap_tmp_ns__addUserResponse.errorCode);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = "";
	soap_serializeheader(soap);
	soap_serialize_ns__addUserResponse(soap, &soap_tmp_ns__addUserResponse);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns__addUserResponse(soap, &soap_tmp_ns__addUserResponse, "ns:addUserResponse", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns__addUserResponse(soap, &soap_tmp_ns__addUserResponse, "ns:addUserResponse", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

static int serve_ns__removeUser(BFCPServiceService *soap)
{	struct ns__removeUser soap_tmp_ns__removeUser;
	struct ns__removeUserResponse soap_tmp_ns__removeUserResponse;
	enum ns__ErrorCode soap_tmp_ns__ErrorCode;
	soap_default_ns__removeUserResponse(soap, &soap_tmp_ns__removeUserResponse);
	soap_default_ns__ErrorCode(soap, &soap_tmp_ns__ErrorCode);
	soap_tmp_ns__removeUserResponse.errorCode = &soap_tmp_ns__ErrorCode;
	soap_default_ns__removeUser(soap, &soap_tmp_ns__removeUser);
	if (!soap_get_ns__removeUser(soap, &soap_tmp_ns__removeUser, "ns:removeUser", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = soap->removeUser(soap_tmp_ns__removeUser.conferenceID, soap_tmp_ns__removeUser.userID, soap_tmp_ns__removeUserResponse.errorCode);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = "";
	soap_serializeheader(soap);
	soap_serialize_ns__removeUserResponse(soap, &soap_tmp_ns__removeUserResponse);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns__removeUserResponse(soap, &soap_tmp_ns__removeUserResponse, "ns:removeUserResponse", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns__removeUserResponse(soap, &soap_tmp_ns__removeUserResponse, "ns:removeUserResponse", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

static int serve_ns__setChair(BFCPServiceService *soap)
{	struct ns__setChair soap_tmp_ns__setChair;
	struct ns__setChairResponse soap_tmp_ns__setChairResponse;
	enum ns__ErrorCode soap_tmp_ns__ErrorCode;
	soap_default_ns__setChairResponse(soap, &soap_tmp_ns__setChairResponse);
	soap_default_ns__ErrorCode(soap, &soap_tmp_ns__ErrorCode);
	soap_tmp_ns__setChairResponse.errorCode = &soap_tmp_ns__ErrorCode;
	soap_default_ns__setChair(soap, &soap_tmp_ns__setChair);
	if (!soap_get_ns__setChair(soap, &soap_tmp_ns__setChair, "ns:setChair", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = soap->setChair(soap_tmp_ns__setChair.conferenceID, soap_tmp_ns__setChair.floorID, soap_tmp_ns__setChair.userID, soap_tmp_ns__setChairResponse.errorCode);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = "";
	soap_serializeheader(soap);
	soap_serialize_ns__setChairResponse(soap, &soap_tmp_ns__setChairResponse);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns__setChairResponse(soap, &soap_tmp_ns__setChairResponse, "ns:setChairResponse", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns__setChairResponse(soap, &soap_tmp_ns__setChairResponse, "ns:setChairResponse", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

static int serve_ns__removeChair(BFCPServiceService *soap)
{	struct ns__removeChair soap_tmp_ns__removeChair;
	struct ns__removeChairResponse soap_tmp_ns__removeChairResponse;
	enum ns__ErrorCode soap_tmp_ns__ErrorCode;
	soap_default_ns__removeChairResponse(soap, &soap_tmp_ns__removeChairResponse);
	soap_default_ns__ErrorCode(soap, &soap_tmp_ns__ErrorCode);
	soap_tmp_ns__removeChairResponse.errorCode = &soap_tmp_ns__ErrorCode;
	soap_default_ns__removeChair(soap, &soap_tmp_ns__removeChair);
	if (!soap_get_ns__removeChair(soap, &soap_tmp_ns__removeChair, "ns:removeChair", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = soap->removeChair(soap_tmp_ns__removeChair.conferenceID, soap_tmp_ns__removeChair.floorID, soap_tmp_ns__removeChairResponse.errorCode);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = "";
	soap_serializeheader(soap);
	soap_serialize_ns__removeChairResponse(soap, &soap_tmp_ns__removeChairResponse);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns__removeChairResponse(soap, &soap_tmp_ns__removeChairResponse, "ns:removeChairResponse", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns__removeChairResponse(soap, &soap_tmp_ns__removeChairResponse, "ns:removeChairResponse", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

static int serve_ns__getConferenceIDs(BFCPServiceService *soap)
{	struct ns__getConferenceIDs soap_tmp_ns__getConferenceIDs;
	ns__ConferenceListResult result;
	result.soap_default(soap);
	soap_default_ns__getConferenceIDs(soap, &soap_tmp_ns__getConferenceIDs);
	if (!soap_get_ns__getConferenceIDs(soap, &soap_tmp_ns__getConferenceIDs, "ns:getConferenceIDs", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = soap->getConferenceIDs(&result);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = "";
	soap_serializeheader(soap);
	result.soap_serialize(soap);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || result.soap_put(soap, "ns:ConferenceListResult", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || result.soap_put(soap, "ns:ConferenceListResult", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

static int serve_ns__getConferenceInfo(BFCPServiceService *soap)
{	struct ns__getConferenceInfo soap_tmp_ns__getConferenceInfo;
	ns__ConferenceInfoResult result;
	result.soap_default(soap);
	soap_default_ns__getConferenceInfo(soap, &soap_tmp_ns__getConferenceInfo);
	if (!soap_get_ns__getConferenceInfo(soap, &soap_tmp_ns__getConferenceInfo, "ns:getConferenceInfo", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = soap->getConferenceInfo(soap_tmp_ns__getConferenceInfo.conferenceID, &result);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = "";
	soap_serializeheader(soap);
	result.soap_serialize(soap);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || result.soap_put(soap, "ns:ConferenceInfoResult", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || result.soap_put(soap, "ns:ConferenceInfoResult", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}
/* End of server object code */
