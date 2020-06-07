// ETH32 - an Enemy Territory cheat for windows
// Copyright (c) 2007 eth32 team
// www.cheatersutopia.com & www.nixcoders.org

#pragma once

// CGNetwork -- General network functions

class CGNetwork
{
public:
	int ServerConnect(const char *hostname, int port, int type);
	bool SendRaw(int socket, char *buffer, int size);
	bool SendText(int socket, char *format, ...);
private:
};

extern CGNetwork GNetwork;
