// ETH32 - an Enemy Territory cheat for windows
// Copyright (c) 2007 eth32 team
// www.cheatersutopia.com & www.nixcoders.org

typedef bool H_BOOL_A;
typedef H_BOOL_A A_BOOL_A;
typedef A_BOOL_A BOOLA_BOOLA;
typedef BOOLA_BOOLA BIG_BAD_BOOL;

#pragma once

char *stristr (const char * str1, const char * str2);

typedef struct threadInfo_st {
	char name[128];
	DWORD tID;
} threadInfo_t;

extern const char *namestealText[NAMESTEAL_MAX];
extern const char *killspamText[KILLSPAM_MAX];

class CTools
{
public:
	void Init(HINSTANCE hInst);
	void Shutdown();
	DWORD crc32FromFile(const char *filename);
	unsigned int ClientNumForAddr(DWORD dwCentAddr);
	int ScanCrosshairEntity(void);	// Returns entNum if crosshair is over Ent, else returns -1 (even through walls)
	pack_t *getPack(char *pk3filename);

	// some utilities that may be used by Gui or Settings
	void CharToVec3(const char *txt, vec3_t v);
	void CharToVec4(const char *txt, vec4_t v);
    void SaveSettings(void); 
	void LoadSettings(bool loadGui);
	void DataAsHex(char *out, void *in, size_t size);

	bool SkipCgameCvar(vmCvar_t *cvar, const char *cvarName);

	void SetUserCvars();
	void SaveUserCvars();
	void AddUserCvar(char *cvarName, char *cvarValue);
	void LoadUserCvars();

	// this seems out of place here
	void VectorAngles(const vec3_t forward, vec3_t angles);

	char *CleanString(char *string);
	void UrlString(char *string, int size);
	void RemoveUrlSpaces(char *string);

	const char *GetRandomName(void);

	char *GetEtVersionString(int version);
	char *GetModVersionString(int version);

	// spam functions 
	void NameSteal(int recurse);
	void SetTarget(int n);
	void CommonFormat(char *fmt, char *str);
	void LoadSpam(char *file);
	void SaveSpam(char *file);
	bool KillSpam();
	bool DeathSpam();
	bool MultikillSpam(int spree);
	bool SuicideSpam();
	bool SelfkillSpam();	
	char *GetKillspam(int idx);
	char *GetMultikillspam(int idx);
	char *GetSuicidespam(int idx);
	char *GetSelfkillspam(int idx);
	char *GetDeathspam(int idx);
	int TotalKillspam();
	int TotalMultikillspam();
	int TotalSuicidespam();
	int TotalDeathspam();
	int TotalSelfkillspam();
	void AddSuicidespam(char *fmt);
	void AddKillspam(char *fmt);
	void AddMultikillspam(char *fmt);
	void AddKuicidespam(char *fmt);
	void AddDeathspam(char *fmt);
	void AddSelfkillspam(char *fmt);	
	void ClearKillspam();
	void ClearMultikillspam();
	void ClearSelfkillspam();
	void ClearSuicidespam();
	void ClearDeathspam();	
	// end spam functions

	bool auxSpecList[MAX_CLIENTS];
	
	void RegisterThread(DWORD tID, char *name);
	threadInfo_t *nThreads;
	int sThreads;

	//char *CleanString(char *string);
	void PBNamespoof(char *name);

	int			lastNamestealTime;
	char		(*nameList)[256];
	int			nNames;	

	
	// damn... starting to feel like a real Microsoft Systems Programmer here ;)
	BIG_BAD_BOOL PatchMicrosoftCRTandDOTNETInstallGlobalExceptionFilterOverwritingRoutinesSoTheyDontWorkAgainUntilWeTellThemTo(BIG_BAD_BOOL pszftztsftBigImpressiveBoolPtrArg_DoWeWantToEnbalePatch);

	cvarInfo_t *userCvarList;

	void Print(const char *txtFmt, ...);

	void *FindPattern(void *haystack, int haystacklen, char *pattern, char *mask);

	

private:
	void getOffsetFromINI( char *key, int *offset );
	void getOffsetFromINI( char *key, DWORD *offset );
	bool loadOffsets();
	char offsetFile[MAX_PATH];
	char section[64];

	// spam stuff
	char (*killspamFmt)[512];
	char (*deathspamFmt)[512];
	char (*suicidespamFmt)[512];
	char (*multikillspamFmt)[512];
	char (*selfkillspamFmt)[512];	
	int nKillspam;
	int nDeathspam;
	int nSuicidespam;
	int nMultikillspam;
	int nSelfkillspam;

	char errorMessage[1024];

	bool 		getAmmo();
	int 		ammo;
	int 		clip;
	int			targetID;
	
	pthread_mutex_t *fp_mutex;
	pthread_mutex_t *cc_mutex;
	pthread_mutex_t *print_mutex;
};

extern CTools Tools;

