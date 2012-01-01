/**
 * Clever programming language
 * Copyright (c) 2012 Clever Team
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include <fstream>
#include <iostream>
#include <errno.h>
#include <fcntl.h>
#include <sys/poll.h>
#include "compiler/compiler.h"
#include "compiler/cstring.h"
#include "modules/std/net/tcpsocket.h"
#include "types/nativetypes.h"

namespace clever { namespace packages { namespace std { namespace net {

CLEVER_TYPE_METHOD(TcpSocket::constructor) {
	SocketValue* sv = new SocketValue;

	// Socket init.
	// @TODO: check for errors on this.
	sv->socket = socket(AF_INET, SOCK_STREAM, 0);

	// Local socket init.
	sv->local.sin_family = AF_INET;
	sv->local.sin_addr.s_addr = htonl(INADDR_ANY);
	sv->local.sin_port = htons(INADDR_ANY);
	
	// Remote socket init.
	sv->remote.sin_family = AF_INET;

	if (args != NULL) {
		if (args->size() == 1) {
			// Host only.
			sv->remote.sin_addr.s_addr = inet_addr(args->at(0)->getString().c_str());
		} else if (args->size() == 2) {
			// Host and port.
			sv->remote.sin_addr.s_addr = inet_addr(args->at(0)->getString().c_str());
			sv->remote.sin_port = args->at(1)->getInteger();
		}
	}

	retval->setDataValue(sv);
}

CLEVER_TYPE_METHOD(TcpSocket::setHost) {
	SocketValue* sv = CLEVER_GET_VALUE(SocketValue*, value);

	sv->remote.sin_addr.s_addr = inet_addr(args->at(0)->getString().c_str());
}

CLEVER_TYPE_METHOD(TcpSocket::setPort) {
	SocketValue* sv = CLEVER_GET_VALUE(SocketValue*, value);

	sv->remote.sin_port = htons(args->at(0)->getInteger());
}

CLEVER_TYPE_METHOD(TcpSocket::connect) {
	SocketValue* sv = CLEVER_GET_VALUE(SocketValue*, value);
	int socketflags;

	if (::connect(sv->socket, (struct sockaddr*)&sv->remote, sizeof(sv->remote)) != 0) {
		// @TODO: change this.
		::std::cerr << "connect() failed: " << (errno) << " - " << ::std::string(sys_errlist[errno]) << ::std::endl;
	}

	// Change the socket to non-blocking mode.
	socketflags = fcntl(sv->socket, F_GETFL);
	socketflags |= O_NONBLOCK;
	fcntl(sv->socket, F_SETFL, socketflags);
}

CLEVER_TYPE_METHOD(TcpSocket::receive) {
	SocketValue* sv = CLEVER_GET_VALUE(SocketValue*, value);
	char buffer[65535] = {0};
	::std::string data;

	// @TODO: check for errors on this.
	while (::recv(sv->socket, buffer, sizeof(buffer), 0) > 0) {
		data += ::std::string(buffer);
	}

	retval->setString(CSTRING(data));
}

CLEVER_TYPE_METHOD(TcpSocket::send) {
	SocketValue* sv = CLEVER_GET_VALUE(SocketValue*, value);

	// @TODO: check for errors on this.
	::send(sv->socket, args->at(0)->getString().c_str(), args->at(0)->getString().size(), 0);
}

CLEVER_TYPE_METHOD(TcpSocket::poll) {
	SocketValue* sv = CLEVER_GET_VALUE(SocketValue*, value);
	struct pollfd fds[1];

	// Prepare and execute the polling.
	memset(fds, 0, sizeof(fds));
	fds[0].fd = sv->socket;
	fds[0].events = POLLIN;
	::poll(fds, 1, 0);

	if (fds[0].revents == POLLIN) {
		retval->setBoolean(true);
	} else {
		retval->setBoolean(false);
	}
}

CLEVER_TYPE_METHOD(TcpSocket::toString) {
	retval->setString(CSTRING("TcpSocket class"));
}

void TcpSocket::init() {
	addMethod(new Method(CLEVER_CTOR_NAME, (MethodPtr)&TcpSocket::constructor, CLEVER_TYPE("TcpSocket")));

	addMethod(
		(new Method(CLEVER_CTOR_NAME, (MethodPtr)&TcpSocket::constructor, CLEVER_TYPE("TcpSocket")))
			->addArg("host", CLEVER_STR)
	);
		
	addMethod(
		(new Method(CLEVER_CTOR_NAME, (MethodPtr)&TcpSocket::constructor, CLEVER_TYPE("TcpSocket")))
			->addArg("host", CLEVER_STR)
			->addArg("port", CLEVER_INT)
	);

	addMethod(
		(new Method("setHost", (MethodPtr)&TcpSocket::setHost, CLEVER_VOID))
			->addArg("host", CLEVER_STR)
	);

	addMethod(
		(new Method("setPort", (MethodPtr)&TcpSocket::setPort, CLEVER_VOID))
			->addArg("port", CLEVER_INT)
	);

	addMethod(new Method("receive", (MethodPtr)&TcpSocket::receive, CLEVER_STR));

	addMethod(
		(new Method("send", (MethodPtr)&TcpSocket::send, CLEVER_VOID))
			->addArg("data", CLEVER_STR)
	);

	addMethod(new Method("poll", (MethodPtr)&TcpSocket::poll, CLEVER_BOOL));

	addMethod(new Method("connect", (MethodPtr)&TcpSocket::connect, CLEVER_TYPE("TcpSocket")));

	addMethod(new Method("toString", (MethodPtr)&TcpSocket::toString, CLEVER_STR));
}

DataValue* TcpSocket::allocateValue() const {
	return new SocketValue;
}

void TcpSocket::destructor(Value* value) const {
	//SocketValue* sv = CLEVER_GET_VALUE(SocketValue*, value);

	// @TODO: disconnect.
}

}}}} // clever::packages::std::net
