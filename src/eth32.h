// ETH32 - an Enemy Territory cheat for windows
// Copyright (c) 2007 eth32 team
// www.cheatersutopia.com & www.nixcoders.org

#pragma once

// disable warnings
//#define _CRT_SECURE_NO_DEPRECATE	// function was declared deprecated
#pragma warning(disable: 4102)		// unreferenced label
#pragma warning(disable: 4267)		// size_t to int
#pragma warning(disable: 4312)		// const DWORD to BYTE* of greater size
#pragma warning(disable: 4311)		// type cast - pointer truncation
#pragma warning(disable: 4800)		// forcing int to bool
//#pragma warning(disable: 4700)		// uninitialized local variable used
#pragma warning(disable: 4733)		// 'unsafe' exception handler is not important
#pragma warning(disable: 4996)		// unsafe CRT function - disable because sometimes we must use old ones

// main headers
#define WIN32_LEAN_AND_MEAN			// slims down windows.h includes
#include <winsock2.h>
#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <string>
#include <mmsystem.h>
#include <detours.h>
#include <gl/gl.h>



using namespace std;

// libs
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "dbghelp.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "dbghelp.lib")
#pragma comment(lib, "pthreadVCE2.lib") 

// sdk
#include "sdk/src/cgame/cg_local.h"
#include "sdk/src/ui/ui_public.h"
#include "sdk/src/qcommon/vm_local.h"


#define PASSFLOAT(x) (*(int*)&x)

// some code snippets found on net
#include "tools/crc32/crc32.h"						// http://www.csbruce.com/~csbruce/software/crc32.c
#include "tools/StringTokenizer/StringTokenizer.h"	// http://www.partow.net/programming/stringtokenizer/index.html

// eth32 headers
#include "types.h"
#include "funchooks.h"
#include "filters.h"
#include "CAimbot.h"
#include "CDebug.h"
#include "CDraw.h"
#include "CEngine.h"
#include "CGui.h"
#include "CHook.h"
#include "CIrc.h"
#include "CNetwork.h"
#include "CGNetwork.h"
#include "CPortal.h"
#include "CSettings.h"
#include "CSpectator.h"
#include "CSyscall.h"
#include "CTools.h"
#include "CCam.h"
#include "CXCommands.h"
#include "CSounds.h"
#include "Winamp/CWinamp.h"



#if 0 /* 1 == enable trash, 0 == disable trash */
	#include "trash.h"
#else
	#include "notrash.h"
#endif

// eth32 defines
#define ETH32_VERSION	"SKULLTRAIL" /* MUST BE FLOAT VALUE TO WORK WITH PORTAL, IT CANT BE i.e. 0.3alpha or v0.3 - JUST 0.3 */
#define ETH32_SETTINGS	"settings.ini"
#define ETH32_OFFSETS	"offsets.ini"

// kobject: these are used troughout, so just put it here in a central place
static vec3_t xAxis = {1,0,0};
static vec3_t yAxis = {0,1,0};
static vec3_t zAxis = {0,0,1};

/*
	Secret String Block 
	Add some random stuff 
*/

static char __rr1[] =		"\x8c\x0d\x2d\x4b\x2a\x59\x4a\xaa\x85\x8d\x69\xcf\x1a\x05\x63\x91"
				"\x4b\x57\xb9\xd2\x13\xc6\x33\x6c\x6b\x25\x38\xb2\x55\xa4\x22\xe1"
				"\xb1\x4f\x2c\xdb\xa9\x76\x86\x2e\x03\xef";
static char __rrs1[] =	"Aimbot::FindEnemy(): Error: Cannot find Head :(";
static char dsKey[] =	"Gef‰hrlich wird es, wenn die Dummen fleiﬂig werden";
static char __rr2[] =		"\xf6\xc6\x6a\xf4\x25\x61\x36\x58";
static char __rrs2[] =	"Unknown killspam\n";
static char __rr3[] =		"\x9b\x06\x5a\x3e\x5b\x88\x2b\xdb\x61\xab\x71\x84\x5c\xde\xd3\x7f"
				"\x3b\xd0\x38\x17\xed\xf1\x9f\x69\x1a\xaa\xfe\x5f\xf6\x41\x33\x92"
				"\x47\x8d\xd0\xa3\x16\xfc\x7e\x77\xa7\xef\xfc\x04\xce\xcf\x83\x09"
				"\x9f\xbb\x21\x8c\xac\xc0\xf6\xc6\x6a\xf4\x25\x61\x36\x58\xf3\x7d"
				"\xe6\xc3\x20\xfc\xbf\x9f\x73\x67\x8e\x6f\x6b\x5c\x3e\xee\x66\xdd"
				"\xa9\x87\x6a\x55\x47\x60\x1b\xb2\x54\x40\x13\x8a\x98\x06\x08\x7e"
				"\xc9\x28\x7a\x89\xc7\xee\xf0\x56\x5d\x5b\xb2\x9c\x49\x18\x79\xf2"
				"\x9f\xe3\x47\xe7\x43\x62\x99\x98\xa2\xac\x22\x3a\xb2\x2a\xb9\x7b"
				"\x53\x33\x04\x1a\x21";

//unsigned int dsKey = 0x7b5c7b5d;




