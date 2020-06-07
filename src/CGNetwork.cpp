// ETH32 - an Enemy Territory cheat for windows
// Copyright (c) 2007 eth32 team
// www.cheatersutopia.com & www.nixcoders.org

#include "eth32.h"

CGNetwork GNetwork;

int CGNetwork::ServerConnect(const char *hostname, int port, int type)
{
	int sock;
	struct sockaddr_in sin;
	struct hostent *he;
	memset(&sin, 0, sizeof(sin));

	if (!(he = gethostbyname(hostname)))
		return -1;

	sock = socket(AF_INET, type, 0);
	sin.sin_addr.S_un.S_addr = ((struct in_addr *)(he->h_addr))->s_addr;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);

	int result = connect(sock, (struct sockaddr *) &sin, sizeof(sin));
	if (result == SOCKET_ERROR)
		return -1;

	return sock;
}

bool CGNetwork::SendRaw(int sock, char *buffer, int size)
{
	return (send(sock, buffer, size, 0) >= 0);
}

bool CGNetwork::SendText(int sock, char *fmt, ...)
{
	char buffer[1024];
	static char *trailing = "\n\0";
	
	va_list arglist;
	va_start(arglist, fmt);
		int size = vsprintf_s(buffer, sizeof(buffer) - strlen(trailing), fmt, arglist);
	va_end(arglist);

	if (size > strlen(buffer))
		return false;

	strncat_s(buffer, sizeof(buffer), trailing, sizeof(buffer) - strlen(trailing));
	
	return this->SendRaw(sock, buffer, strlen(buffer));
}

