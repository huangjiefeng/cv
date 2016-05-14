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

#ifndef OWIHTTPREQUEST_H
#define OWIHTTPREQUEST_H

#include <util/Event.h>

#include <thread/Thread.h>

#include <string>

/**
 * Sends a HTTP request on a HTTP server then gets and reads the answer from the server.
 *
 * A proxy can be set and an error code is returned.
 *
 * Example **OBSOLETE**:
 * <pre>
 * class MyHttpRequest : public HttpRequest {
 * public:
 *    //Callback called when the server answer to the HTTP request.
 *    virtual void answerReceived(const std::string & answer, Error error) {
 *        if (error == NoError && !answer.empty()) {
 *            std::cout << "HTTP server anwser: " << answer << std::endl;
 *        }
 *    }
 * };
 *
 * HttpRequest::setProxy("proxy.wengo.fr", 8080, "myProxyUsername", "myProxyPassword");
 * MyHttpRequest * http = new MyHttpRequest();
 * http->sendRequest("https://wengo.fr:8080/login.php", "login=mylogin&password=mypassword");
 * </pre>
 *
 * @author Tanguy Krotoff
 */
class IHttpRequest : public Thread {
public:

	enum Error {
		/** No error. */
		NoError,

		/** The hostname lookup failed. */
		HostNotFound,

		/** The server refused the connection. */
		ConnectionRefused,

		/** The server closed the connection unexpectedly. */
		UnexpectedClose,

		/** The server sent an invalid response header. */
		InvalidResponseHeader,

		/**
		 * The client could not read the content correctly because
		 * an error with respect to the content length occurred.
		 */
		WrongContentLength,

		/** The request was aborted. */
		Aborted,

		/** The proxy authentication failed. */
		ProxyAuthenticationError,

		/** The connection with the proxy failed. */
		ProxyConnectionError,

		/** operation timeouted. */
		TimeOut,

		/** Unknow error. */
		UnknownError
	};

	/**
	 * The HTTP answer to the request has been received.
	 *
	 * @param sender this class
	 * @param requestId HTTP request ID
	 * @param answer HTTP answer (std::string is used as a byte array)
	 * @param error Error code
	 */
	Event<void (IHttpRequest * sender, int requestId, const std::string & answer, Error error)> answerReceivedEvent;

	/**
	 * Indicates the current progress of the download.
	 *
	 * @see QHttp::dataReadProgress()
	 * @param requestId HTTP request ID
	 * @param bytesDone specifies how many bytes have been transfered
	 * @param bytesTotal total size of the HTTP transfer, if 0 then the total number of bytes is not known
	 */
	Event<void (int requestId, double bytesDone, double bytesTotal)> dataReadProgressEvent;

	/**
	 * Indicates the current progress of the upload.
	 *
	 * @see QHttp::dataSendProgress()
	 * @param requestId HTTP request ID
	 * @param bytesDone specifies how many bytes have been transfered
	 * @param bytesTotal total size of the HTTP transfer, if 0 then the total number of bytes is not known
	 */
	Event<void (int requestId, double bytesDone, double bytesTotal)> dataSendProgressEvent;

	virtual ~IHttpRequest() { }

	/**
	 * Sends a HTTP request given a HTTP URL.
	 *
	 * A complete HTTP URL is something like:
	 * https://wengo.fr:8080/login.php?login=mylogin&password=mypassword
	 *
	 * @param sslProtocol true if HTTPS protocol (https://), false if HTTP protocol (http://)
	 * @param hostname HTTP server hostname (e.g wengo.fr, yahoo.fr)
	 * @param hostPort HTTP server port number (e.g 80, 8080)
	 * @param path path on the server (e.g login.php)
	 * @param data HTTP data (e.g login=mylogin&password=mypassword)
	 * @param postMethod HTTP POST method if true, HTTP GET method if false
	 * @return HTTP request ID
	 */
	virtual int sendRequest(bool sslProtocol,
			const std::string & hostname,
			unsigned int hostPort,
			const std::string & path,
			const std::string & data,
			bool postMethod = false) = 0;

	/**
	 * Aborts the current request and deletes all scheduled requests.
	 *
	 * Throws answerReceivedEvent with error=Aborted
	 */
	virtual void abort() = 0;

	/**
	 * Thread run method.
	 *
	 * Internal method, this should not be used from outside.
	 */
	virtual void run() = 0;
};

#endif	//OWIHTTPREQUEST_H
