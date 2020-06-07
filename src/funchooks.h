// ETH32 - an Enemy Territory cheat for windows
// Copyright (c) 2007 eth32 team
// www.cheatersutopia.com & www.nixcoders.org

#pragma once

#ifndef CDECL
	#define CDECL
#endif

// LoadLibraryA
typedef HMODULE (__stdcall *LoadLibraryA_t)(LPCSTR lpLibName);
HMODULE __stdcall eth32_LoadLibraryA(LPCSTR lpLibName);
extern LoadLibraryA_t orig_LoadLibraryA;

typedef bool (__stdcall *FreeLibrary_t)(HMODULE hLibModule);
bool __stdcall eth32_FreeLibrary(HMODULE hLibModule);
extern FreeLibrary_t orig_FreeLibrary;

// CreateWindowExA
typedef HWND (__stdcall *CreateWindowExA_t)(DWORD dwExStyle, LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);
HWND __stdcall eth32_CreateWindowExA(DWORD dwExStyle, LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);
extern CreateWindowExA_t orig_CreateWindowExA;

// cvar unlocker
typedef cvar_t *(*Cvar_Get_t)(const char *var_name, const char *var_value, int flags);
cvar_t *__cdecl eth32_Cvar_Get(const char *var_name, const char *var_value, int flags);
extern Cvar_Get_t orig_Cvar_Get;

// syscalls
typedef int (__cdecl *syscall_t)(int cmd, ...);
int __cdecl eth32_CG_syscall(int command, ...);
int __cdecl eth32_UI_syscall(int command, ...);
extern syscall_t orig_syscall;

// vmMains
typedef int (__cdecl *vmMain_t)(int cmd, ...);
int __cdecl eth32_CG_vmMain(int cmd, ...);
extern vmMain_t orig_CG_vmMain;

// Sys_LoadDLL
typedef void* (__cdecl *Sys_LoadDll_t)(const char *name, char *fqpath , int (QDECL **entryPoint)(int, ...), int (QDECL *systemcalls)(int, ...));
void* __cdecl eth32_Sys_LoadDll( const char *name, char *fqpath , int (QDECL **entryPoint)(int, ...), int (QDECL *systemcalls)(int, ...));
extern Sys_LoadDll_t orig_Sys_LoadDll;

// dllEntries
typedef void (__cdecl *dllEntry_t)(int (*syscallptr)(int arg,... ));
void __cdecl eth32_CG_dllEntry(int (*syscallptr)(int arg,... ));
void __cdecl eth32_UI_dllEntry(int (*syscallptr)(int arg,... ));
extern dllEntry_t orig_CG_dllEntry;
extern dllEntry_t orig_UI_dllEntry;

// CG_Player
typedef void (__cdecl *CG_Player_t)(centity_t *cent);
void __cdecl eth32_CG_Player(centity_t *cent);
extern CG_Player_t orig_CG_Player;

// CG_AddPlayerWeapon
typedef void (__cdecl *CG_AddPlayerWeapon_t)(refEntity_t *parent, playerState_t *ps, centity_t *cent);
void __cdecl eth32_CG_AddPlayerWeapon(refEntity_t *parent, playerState_t *ps, centity_t *cent);
extern CG_AddPlayerWeapon_t orig_CG_AddPlayerWeapon;

// CG_PositionRotatedEntityOnTag
typedef void (__cdecl *CG_PositionRotatedEntityOnTag_t)(refEntity_t *entity, const refEntity_t *parent, const char *tagName);
void __cdecl eth32_CG_PositionRotatedEntityOnTag(refEntity_t *entity, const refEntity_t *parent, const char *tagName);
extern CG_PositionRotatedEntityOnTag_t orig_CG_PositionRotatedEntityOnTag;

// CG_SetLerpFrameAnimationRate
typedef void (__cdecl *CG_SetLerpFrameAnimationRate_t)(centity_t *cent, clientInfo_t *ci, lerpFrame_t *lf, int anim);
void __cdecl eth32_CG_SetLerpFrameAnimationRate(centity_t *cent, clientInfo_t *ci, lerpFrame_t *lf, int anim);
extern CG_SetLerpFrameAnimationRate_t orig_CG_SetLerpFrameAnimationRate;

// CG_FinishWeaponChange
typedef void (__cdecl *CG_FinishWeaponChange_t)(int lastweap, int newweap);
extern CG_FinishWeaponChange_t orig_CG_FinishWeaponChange;
void __cdecl eth32_CG_FinishWeaponChange(int lastweap, int newweap);

// CG_PlayVoiceChat
typedef void (__cdecl *CG_PlayVoiceChat_t)(bufferedVoiceChat_t *vchat);
extern CG_PlayVoiceChat_t orig_CG_PlayVoiceChat;
void __cdecl eth32_CG_PlayVoiceChat(bufferedVoiceChat_t *vchat);

// FS_PureServerSetLoadedPaks
typedef void (__cdecl *FS_PureServerSetLoadedPaks_t)(const char *pakSums, const char *pakNames);
extern FS_PureServerSetLoadedPaks_t orig_FS_PureServerSetLoadedPaks;
void __cdecl eth32_FS_PureServerSetLoadedPaks(const char *pakSums, const char *pakNames);

typedef void (__cdecl *FS_AddGameDirectory_t)( const char *path, const char *dir );
extern FS_AddGameDirectory_t orig_FS_AddGameDirectory;

typedef void (__cdecl *FS_AddGameDirectory2_t)( const char *path );
extern FS_AddGameDirectory2_t orig_FS_AddGameDirectory2;

typedef void (__cdecl *FS_Startup_t)( const char *gameName );
extern FS_Startup_t orig_FS_Startup;
void __cdecl eth32_FS_Startup( const char *gameName );

typedef void (__cdecl *FS_Startup2_t)();
extern FS_Startup2_t orig_FS_Startup2;
void __cdecl eth32_FS_Startup2();

// Cmd_AddCommand
typedef void (__cdecl *Cmd_AddCommand_t)(const char *cmd_name, xcommand_t function);
extern Cmd_AddCommand_t orig_Cmd_AddCommand;
void __cdecl eth32_Cmd_AddCommand(const char *cmd_name, xcommand_t function);

// CG_EntityEvent
typedef void (__cdecl *CG_EntityEvent_t)(centity_t *cent, vec3_t position);
void __cdecl eth32_CG_EntityEvent(centity_t *cent, vec3_t position);
extern CG_EntityEvent_t orig_CG_EntityEvent;

// CG_Trace
typedef void (__cdecl *CG_Trace_t)(trace_t *result, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, int skipNumber, int mask);
void __cdecl eth32_CG_Trace(trace_t *result, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, int skipNumber, int mask);
extern CG_Trace_t orig_CG_Trace;

// CG_DamageFeedback
typedef void (__cdecl *CG_DamageFeedback_t)(int yawByte, int pitchByte, int damage);
void __cdecl eth32_CG_DamageFeedback(int yawByte, int pitchByte, int damage);
extern CG_DamageFeedback_t orig_CG_DamageFeedback;

// CG_WeaponFireRecoil
typedef void (__cdecl *CG_WeaponFireRecoil_t)(centity_t *cent);
void __cdecl eth32_CG_WeaponFireRecoil(centity_t *cent);
extern CG_WeaponFireRecoil_t orig_CG_WeaponFireRecoil;

// CG_Respawn
typedef void (__cdecl *CG_Respawn_t)(qboolean revived);
void __cdecl eth32_CG_Respawn(qboolean revived);
extern CG_Respawn_t orig_CG_Respawn;

// CG_AddPMItem
typedef void (__cdecl *CG_AddPMItem_t)(popupMessageType_t type, const char *message, qhandle_t shader);
void __cdecl eth32_CG_AddPMItem(popupMessageType_t type, const char *message, qhandle_t shader);
extern CG_AddPMItem_t orig_CG_AddPMItem;

// CG_AddPMItem2
typedef void (__cdecl *CG_AddPMItem2_t)(popupMessageType_t type, const char *message, qhandle_t shader, int unknown);
void __cdecl eth32_CG_AddPMItem2(popupMessageType_t type, const char *message, qhandle_t shader, int unknown);
extern CG_AddPMItem2_t orig_CG_AddPMItem2;

// CG_DrawPlayerStatusHead
typedef void (__cdecl *CG_DrawPlayerStatusHead_t)(void);
void __cdecl eth32_CG_DrawPlayerStatusHead(void);
extern CG_DrawPlayerStatusHead_t orig_CG_DrawPlayerStatusHead;

// CG_DrawPlayerStats
typedef void (__cdecl *CG_DrawPlayerStats_t)(void);
void __cdecl eth32_CG_DrawPlayerStats(void);
extern CG_DrawPlayerStats_t orig_CG_DrawPlayerStats;

// CG_DrawPlayerStatus
typedef void (__cdecl *CG_DrawPlayerStatus_t)(void);
void __cdecl eth32_CG_DrawPlayerStatus(void);
extern CG_DrawPlayerStatus_t orig_CG_DrawPlayerStatus;

// CG_CenterPrint
typedef void (__cdecl *CG_CenterPrint_t)(const char *str, int y, int charWidth);
void __cdecl eth32_CG_CenterPrint(const char *str, int y, int charWidth);
extern CG_CenterPrint_t orig_CG_CenterPrint;

// CG_PriorityCenterPrint
typedef void (__cdecl *CG_PriorityCenterPrint_t)(const char *str, int y, int charWidth, int priority);
void __cdecl eth32_CG_PriorityCenterPrint(const char *str, int y, int charWidth, int priority);
extern CG_PriorityCenterPrint_t orig_CG_PriorityCenterPrint;

// CG_ChargeTimesChanged
typedef void (__cdecl *CG_ChargeTimesChanged_t)(void);
void __cdecl eth32_CG_ChargeTimesChanged(void);
extern CG_ChargeTimesChanged_t orig_CG_ChargeTimesChanged;

// BG_FindClipForWeapon
typedef int (__cdecl *BG_FindClipForWeapon_t)(int weapon);
extern BG_FindClipForWeapon_t orig_BG_FindClipForWeapon;

// BG_FindAmmoForWeapon
typedef int (__cdecl *BG_FindAmmoForWeapon_t)(int weapon);
extern BG_FindAmmoForWeapon_t orig_BG_FindAmmoForWeapon;

// CG_CalculateReinfTime
typedef int (__cdecl *CG_CalculateReinfTime_t)(qboolean menu);
int __cdecl eth32_CG_CalculateReinfTime(qboolean menu);
extern CG_CalculateReinfTime_t orig_CG_CalculateReinfTime;

// CG_AllocLocalEntity
typedef localEntity_t* (__cdecl *CG_AllocLocalEntity_t)(void);
extern CG_AllocLocalEntity_t orig_CG_AllocLocalEntity;

// etpro anti cheat func
typedef void (__cdecl *etproAntiCheat_t)(void *a, void *b, void *c, int checksum, void *e, char *orig_guid);
extern etproAntiCheat_t orig_etproAntiCheat;
void __cdecl eth32_etproAntiCheat(void *a, void *b, void *c, int checksum, void *e, char *orig_guid);

typedef void *(__cdecl *etproAntiCheat2_t)(DWORD a);
extern etproAntiCheat2_t orig_etproAntiCheat2;
void* __cdecl eth32_etproAntiCheat2(DWORD a);

typedef int (__cdecl *CG_CalcViewValues_t)(void);
extern CG_CalcViewValues_t orig_CG_CalcViewValues;
int __cdecl eth32_CG_CalcViewValues(void);

typedef cvar_t *(__cdecl *Cvar_Set2_t)( const char *var_name, const char *value, qboolean force );
extern Cvar_Set2_t orig_Cvar_Set2;
cvar_t* __cdecl eth32_Cvar_Set2( const char *var_name, const char *value, qboolean force );

typedef void * (*RegisterShader_t)( const char *name, int lightmapIndex, qboolean mipRawImage );
extern RegisterShader_t orig_RegisterShader;

typedef void (__cdecl *LoadTGA_t)( const char *name, byte **pic, int *width, int *height);
extern LoadTGA_t orig_LoadTGA;
void __cdecl eth32_LoadTGA( const char *name, byte **pic, int *width, int *height);

