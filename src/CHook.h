// ETH32 - an Enemy Territory cheat for windows
// Copyright (c) 2007 eth32 team
// www.cheatersutopia.com & www.nixcoders.org

#pragma once

#define MAX_ET_VERSIONS		4 /* increased to 6 so if new et version gets released, we can still add support to old eth32 */
#define MAX_CG_MODS			128

class CHook
{
public:
	void hookSystem(bool state);
	int isGameSupported(void);
	void hookGame(bool state);
	int isCgameSupported(LPCSTR lpLibName);
	void hookCgame(bool state);
	void hookEtpro(bool state);
	void hookUi(bool state);
	void UnreferenceBadPk3(void);
	void InitCgameData(void);
	void InitWeaponList(void);
	void ModifyCgameCommands();
	
	

	// Here because weapdefs are static and only visible by CHook
	void SaveWeapons(const char *filename);
	void LoadWeapons(const char *filename);

	cgMod_t cglist[MAX_CG_MODS];

	#ifdef ETH32_DEBUG
		void getOffsets(HMODULE cgame);
	#endif

};

extern CHook Hook;

// for pk3 unlocker - backup good checksums
extern char goodChecksumsPak[BIG_INFO_STRING];
