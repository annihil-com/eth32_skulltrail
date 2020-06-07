// ETH32 - an Enemy Territory cheat for windows
// Copyright (c) 2007 eth32 team
// www.cheatersutopia.com & www.nixcoders.org

#pragma once

// CNetwork -- HTTP protocol

#define MAX_RESPONSE_SIZE 102400 // allow for a 100kb page
#define TIMEOUT 16 // seconds (Post/Get will receive data for n seconds)

static const std::string base64_chars = 
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";

class CNetwork
{
public:
	CNetwork();
	~CNetwork();

	char *Get(const char *hostname, const char *path);
	char *Post(const char *hostname, const char *path, const char *params);
	void base64_encode(char *str_to_encode, char *encoded);
	void base64_decode(char *encoded, char *decoded);
	char *GetResponse() { return response; } // Post and Get point to the response too

private:
	bool is_base64(unsigned char c);
	CRITICAL_SECTION *_cs;

private:
	char lastError[256];
	char cmd[2048];
	char response[MAX_RESPONSE_SIZE];
	bool initted;
};

extern CNetwork Network;