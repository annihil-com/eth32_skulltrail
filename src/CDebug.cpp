// ETH32 - an Enemy Territory cheat for windows
// Copyright (c) 2007 eth32 team
// www.cheatersutopia.com & www.nixcoders.org

#include "eth32.h"
#include <stdio.h>

//#define ETH32_DEBUG

#ifdef _DEBUG
#define ETH32_DEBUG
#endif

CDebug Debug;

CDebug::CDebug()
{
	pathset = false;
	dbgFileFirstOpen = true;
	_cs = new CRITICAL_SECTION;
	InitializeCriticalSection(_cs);
}

CDebug::~CDebug()
{
	DeleteCriticalSection(_cs);
	delete _cs;
}

void CDebug::Log(const char *logmsg, ...)
{
	// debug logging is very important, so *force* a lock
	EnterCriticalSection(_cs);
	
	static char strLogMsg[102400];

	va_list arglist;
	va_start(arglist, logmsg );
	vsprintf_s(strLogMsg, sizeof(strLogMsg), logmsg, arglist);
	va_end(arglist);

	if (!pathset) {
		sprintf_s(path, sizeof(path), "%s\\%s", eth32.path, ETH32_DEBUGFILE);
		pathset = true;
	}
	
	if (!dbgFileFirstOpen) {
		fopen_s(&dbgFile, path, "a");
	} else {
		fopen_s(&dbgFile, path, "w");
		dbgFileFirstOpen = false;
	}

	if (dbgFile) {
		fprintf_s(dbgFile, "%s\n", strLogMsg);
		fclose(dbgFile);
	}
	LeaveCriticalSection(_cs);
}

void CDebug::LogData(void *buf, size_t s_buf)
{
	// debug logging is very important, so *force* a lock
	EnterCriticalSection(_cs);

	if (!pathset) {
		sprintf_s(path, sizeof(path), "%s\\%s", eth32.path, ETH32_DEBUGFILE);
		pathset = true;
	}

	if (!dbgFileFirstOpen) {
		fopen_s(&dbgFile, path, "a");
	} else {
		fopen_s(&dbgFile, path, "w");
		dbgFileFirstOpen = false;
	}

	if (dbgFile) {
		fwrite( buf, 1, s_buf, dbgFile );
		fclose(dbgFile);
	}
	LeaveCriticalSection(_cs);
}

/* to handle _CRT assert errors for invalid parameters, for instance,
	when the buffer for sprintf_s is too small (will crash if not handled) */
void __cdecl InvalidParameterHandler(const wchar_t* expression, const wchar_t* function, const wchar_t* file, unsigned int line, uintptr_t pReserved)
{
	wchar_t mbbuf[2048];
	char cbuf[2048];

	wsprintfW(mbbuf, L"Invalid parameter detected in function %s. File: %s Line: %d Expression: %s\n", function ? function : L"NULL", file ? file : L"NULL", line, expression ? expression : L"NULL");
	WideCharToMultiByte(CP_UTF8, 0, mbbuf, -1, cbuf, sizeof(cbuf), 0, 0);
	Debug.Log("ERROR: _CRT_ASSERT_FAILED: %s", cbuf);
}
