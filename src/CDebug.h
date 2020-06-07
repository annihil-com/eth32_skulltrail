// ETH32 - an Enemy Territory cheat for windows
// Copyright (c) 2007 eth32 team
// www.cheatersutopia.com & www.nixcoders.org

#pragma once

// allows debug logging
//#define ETH32_DEBUG
//#define ETH32_PRELIM_OFFSETS

#define ETH32_DEBUGFILE	"eth32.log"

class CDebug
{
public:
	CDebug();
	~CDebug();
	void Log(const char *logmsg, ...);
	void LogData(void *buf, size_t s_buf);
private:
	CRITICAL_SECTION *_cs;
	FILE *dbgFile;
	bool pathset;
	char path[256];
	bool dbgFileFirstOpen;
};

extern CDebug Debug;
