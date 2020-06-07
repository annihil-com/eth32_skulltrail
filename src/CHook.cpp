// ETH32 - an Enemy Territory cheat for windows
// Copyright (c) 2007 eth32 team
// www.cheatersutopia.com & www.nixcoders.org

#include "eth32.h"
#include "offsets.h"
#include "weapdefs.h"

#define CDECL

void *etpro_A, *etpro_B, *etpro_C, *etpro_E;
DWORD etpro_win32 = 0x30c3e725;
DWORD etpro_linux = 0x395789a0;


CHook Hook;

LoadLibraryA_t						orig_LoadLibraryA = NULL;
FreeLibrary_t						orig_FreeLibrary = NULL;
CreateWindowExA_t					orig_CreateWindowExA = NULL;
Cvar_Get_t							orig_Cvar_Get = NULL;
Cvar_Set2_t							orig_Cvar_Set2 = NULL;
syscall_t							orig_syscall = NULL;
vmMain_t							orig_CG_vmMain = NULL;
CG_Player_t							orig_CG_Player = NULL;
CG_AddPlayerWeapon_t				orig_CG_AddPlayerWeapon = NULL;
CG_PositionRotatedEntityOnTag_t		orig_CG_PositionRotatedEntityOnTag = NULL;
CG_SetLerpFrameAnimationRate_t		orig_CG_SetLerpFrameAnimationRate = NULL;
CG_FinishWeaponChange_t				orig_CG_FinishWeaponChange = NULL;
CG_PlayVoiceChat_t					orig_CG_PlayVoiceChat = NULL;
dllEntry_t							orig_CG_dllEntry = NULL;
dllEntry_t							orig_UI_dllEntry = NULL;
FS_PureServerSetLoadedPaks_t		orig_FS_PureServerSetLoadedPaks = NULL;
FS_AddGameDirectory_t				orig_FS_AddGameDirectory = NULL;
FS_AddGameDirectory2_t				orig_FS_AddGameDirectory2 = NULL;
FS_Startup_t						orig_FS_Startup = NULL;
FS_Startup2_t						orig_FS_Startup2 = NULL;
Cmd_AddCommand_t					orig_Cmd_AddCommand = NULL;
Sys_LoadDll_t						orig_Sys_LoadDll = NULL;
CG_EntityEvent_t					orig_CG_EntityEvent = NULL;
CG_Trace_t							orig_CG_Trace = NULL;
CG_DamageFeedback_t					orig_CG_DamageFeedback = NULL;
CG_WeaponFireRecoil_t				orig_CG_WeaponFireRecoil = NULL;
CG_Respawn_t						orig_CG_Respawn = NULL;
CG_CalculateReinfTime_t				orig_CG_CalculateReinfTime = NULL;

// Standard UI functions we need to remove or mod
CG_AddPMItem_t						orig_CG_AddPMItem = NULL;
CG_AddPMItem2_t						orig_CG_AddPMItem2 = NULL;
CG_DrawPlayerStatusHead_t			orig_CG_DrawPlayerStatusHead = NULL;
CG_DrawPlayerStats_t				orig_CG_DrawPlayerStats = NULL;
CG_DrawPlayerStatus_t				orig_CG_DrawPlayerStatus = NULL;
CG_CenterPrint_t					orig_CG_CenterPrint = NULL;
CG_PriorityCenterPrint_t			orig_CG_PriorityCenterPrint = NULL;
CG_ChargeTimesChanged_t				orig_CG_ChargeTimesChanged = NULL;
BG_FindClipForWeapon_t				orig_BG_FindClipForWeapon = NULL;
BG_FindAmmoForWeapon_t				orig_BG_FindAmmoForWeapon = NULL;
CG_AllocLocalEntity_t				orig_CG_AllocLocalEntity = NULL;
etproAntiCheat_t					orig_etproAntiCheat = NULL;
etproAntiCheat2_t					orig_etproAntiCheat2 = NULL;
CG_CalcViewValues_t					orig_CG_CalcViewValues = NULL;




LoadTGA_t orig_LoadTGA;

extern void etpro_CatchVoiceChat(void);
DWORD savedESP;

char goodChecksumsPak[BIG_INFO_STRING];

// **************************
// System funcs
// **************************

void CHook::hookSystem(bool state)
{
	TRASH50
	TRASH51
	static bool hooked = false;
	static bool first = true;

	Sleep(5000);

	if (first)
	{

		TRASH11
		int et = isGameSupported();

		if (et == -1)
		{
			TRASH58
			#ifdef ETH32_DEBUG
				Debug.Log("This ET version isn't supported!");
			#endif
		}
		else
		{
			TRASH57
			eth32.et = &etlist[et];
			eth32.game.supported = true;
			eth32.game.viewAngleX = (float*)eth32.et->viewAngleX;
			eth32.game.viewAngleY = (float*)eth32.et->viewAngleY;
			eth32.game.glconfig = (glconfig_t *)eth32.et->glConfig;
			eth32.game.com_frameTime = (int *)eth32.et->com_frameTime;

			orig_syscall = (syscall_t) eth32.et->syscall;
			Aimbot.SetAttackButton( (void *)eth32.et->inButtons );
			Aimbot.SetCrouchButton( (void *)((DWORD)eth32.et->inButtons-sizeof(kbutton_t)) );
			//Aimbot.SetCrouchButton( (void *)(eth32.et->inButtons-sizeof(kbutton_t)));

			#ifdef ETH32_DEBUG
				Debug.Log("Found ET version %s", eth32.et->version);
			#endif

			Hook.hookGame(true);
			Tools.LoadSettings(false);
		}

		first = false;
	}
	 
	if (eth32.game.supported && state && !hooked)
	{
		#ifdef ETH32_DEBUG
			Debug.Log("Hooking system functions");
		#endif

		TRASH55
		TRASH58
		DETFUNC(LoadLibraryA);
		DETFUNC(FreeLibrary);
		TRASH59
		DETFUNC(CreateWindowExA);

		hooked = true;
	}
	else if (eth32.game.supported && !state && hooked)
	{
		#ifdef ETH32_DEBUG
			Debug.Log("Un-Hooking system functions");
		#endif
			
		UNDETFUNC(LoadLibraryA);
		UNDETFUNC(FreeLibrary);
		TRASH19
		UNDETFUNC(CreateWindowExA);

		hooked = false;
	}

}

HMODULE __stdcall eth32_LoadLibraryA(LPCSTR lpLibName)
{
	/* with all this trash code, LoadLib hook becomes huge and this func is called a lot,
	   for OPENGL32 i think.. so just return straight away instead of going through much code (perfomance) */
	if (strncmp(lpLibName, eth32.game.path, strlen(eth32.game.path)))
		return orig_LoadLibraryA(lpLibName);

	// don't hook anything if this ET version isn't supported
	if (!eth32.game.supported)
		return orig_LoadLibraryA(lpLibName);

	if (strstr(lpLibName, "cgame_mp_x86.dll"))
	{
		// clearing out cg at new mod load
		TRASH13
		//sol: anyone know why we would need to clear settings?
		//memset(&eth32.settings, 0, sizeof(settings_t));
		memset(&eth32.cg, 0, sizeof(cgame_t));
		eth32.cgMod = NULL;
		TRASH14
		TRASH15

		#ifdef ETH32_DEBUG
			Debug.Log("Loading cgame [crc32: 0x%x]", Tools.crc32FromFile(lpLibName));
		#endif

		// kobject: use this to get offsets on the fly 
		//#ifdef ETH32_DEBUG
		//	Hook.getOffsets(cgameMod);
		//#endif

		eth32.cg.supported = Hook.isCgameSupported(lpLibName);

		TRASH19
		TRASH29
		if (eth32.cg.supported == -1)
		{
			#ifdef ETH32_DEBUG
				Debug.Log("This CG mod isn't supported!");
			#endif

			TRASH69
			if (strstr(lpLibName, "etmain") || strstr(lpLibName, "etpub") || strstr(lpLibName, "shrub"))
			{
				char automoddll[MAX_STRING_CHARS];
				sprintf_s(automoddll, sizeof(automoddll),"%s%s", eth32.path, "dlls\\cgame_mp_x86.dll");
				eth32.cg.supported = Hook.isCgameSupported(automoddll);

				TRASH70
				TRASH71
				TRASH72

				if (eth32.cg.supported != -1)
				{
					eth32.cgMod = &Hook.cglist[eth32.cg.supported];
					
					eth32.cg.module = orig_LoadLibraryA(automoddll);

					#ifdef ETH32_DEBUG
					Debug.Log("Original cgame not supported, loading %s %s [ 0x%x ]", eth32.cgMod->name, eth32.cgMod->version, eth32.cg.module);
					#endif

					Hook.InitCgameData();
					Hook.ModifyCgameCommands();
					TRASH73
					TRASH74
					Hook.hookCgame(true);
					eth32.cg.automod = true;
					return eth32.cg.module;
				}
			}
			// not supported and it isn't safe to redirect to etmain, unhook ET so pb/etpro don't kick us
			//TODO: some sort of alert for the user that mod isn't supported?
			#ifdef ETH32_DEBUG
				Debug.Log("Mod unsupported, not hooking.");
			#endif
			Hook.hookGame(false);
			TRASH108
			TRASH105
			orig_syscall(UI_ERROR, "This mod isn't supported, check portal for latest news and updates regarding ETH32.\n\n\nhttp://eth32.nixcoders.org");
			TRASH99
		}
		else
		{
			eth32.cgMod = &Hook.cglist[eth32.cg.supported];
			eth32.cg.module = orig_LoadLibraryA(lpLibName);
			

			#ifdef ETH32_DEBUG
				Debug.Log("Found mod: %s %s [ 0x%x ]", eth32.cgMod->name, eth32.cgMod->version, (DWORD)eth32.cg.module);
			#endif			

			TRASH91
			Hook.InitCgameData();
			TRASH87
			TRASH88
			Hook.ModifyCgameCommands();
			TRASH92
			Hook.hookCgame(true);
			TRASH45
			TRASH46
			eth32.cg.automod = false;
			return eth32.cg.module;
			TRASH57
		}
	}
	else if (strstr(lpLibName, "ui_mp_x86.dll"))
	{
		eth32.ui.module = orig_LoadLibraryA(lpLibName);
		#ifdef ETH32_DEBUG
			Debug.Log("Loading ui [crc32: 0x%x] at 0x%x", Tools.crc32FromFile(lpLibName), (DWORD) eth32.ui.module);
		#endif

		TRASH88
		Hook.hookUi(true);
		TRASH87
		TRASH86
		return eth32.ui.module;
	}

	return orig_LoadLibraryA(lpLibName);
}

#ifdef ETH32_DEBUG
#define NUM_EXPORTS		24
static char exports[NUM_EXPORTS][256] = 
{
	"CG_Player",
	"CG_AddPlayerWeapon",
	"CG_PositionRotatedEntityOnTag",
	"CG_SetLerpFrameAnimationRate",
	"CG_FinishWeaponChange",
	"CG_PlayVoiceChat",
	"CG_EntityEvent",
	"CG_Trace",
	"CG_DamageFeedback",
	"CG_WeaponFireRecoil",
	"CG_Respawn",
	"CG_CalculateReinfTime",
	"CG_AllocLocalEntity",
	"CG_AddPMItem",
	"CG_AddPMItem2",
	"CG_DrawPlayerStatusHead",
	"CG_DrawPlayerStats",
	"CG_DrawPlayerStatus",
	"CG_CenterPrint",
	"CG_PriorityCenterPrint",
	"CG_ChargeTimesChanged",
	"BG_FindClipForWeapon",
	"BG_FindAmmoForWeapon",
	"cg_entities"
};

void CHook::getOffsets(HMODULE cgame)
{
	BYTE *ret;
	BYTE *o = (BYTE *)cgame;
	int i;
	for( i=0; i<NUM_EXPORTS; i++){
		ret = (BYTE *)GetProcAddress(cgame, exports[i]);
		if (ret == 0x0)
			Debug.Log("%s=0x0", exports[i]);
		else
			Debug.Log("%s=0x%X", exports[i], (unsigned int)(ret - o));
	}	
}

#endif //ETH32_DEBUG

bool __stdcall eth32_FreeLibrary(HMODULE hLibModule)
{
	if (hLibModule && hLibModule == eth32.cg.module) {
#ifdef ETH32_DEBUG
		Debug.Log("FreeLibrary(cgame)");
#endif
		TRASH59
		Hook.hookCgame(false);
		TRASH56
		TRASH58

		eth32.cgameLoaded = false;
		memset(&eth32.cg, 0, sizeof(cgame_t));
		TRASH59
	}
	else if (hLibModule && hLibModule == eth32.ui.module) {
		TRASH62
#ifdef ETH32_DEBUG
		Debug.Log("FreeLibrary(ui)");
#endif
		Hook.hookUi(false);
		TRASH60
		TRASH61
	}

	return orig_FreeLibrary(hLibModule);
}

// credits to bigmac: his source taught me how to do this
HWND __stdcall eth32_CreateWindowExA(DWORD dwExStyle, LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	HWND hwnd = orig_CreateWindowExA(dwExStyle, lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
	if (lpWindowName && strstr(lpWindowName, "Enemy Territory")) {
#ifdef ETH32_DEBUG
		Debug.Log("CreateWindowExA() - Game window created, redirecting wndproc()...");
#endif
		Gui.SetWindowHandle(hwnd);
		TRASH1
		TRASH2
		Gui.RedirectWndProc();
		TRASH9
		TRASH17
		TRASH18
		// remove detour, we have the handle and have redirected wndproc for the remainder of execution
		UNDETFUNC(CreateWindowExA)
		TRASH33
		TRASH35
	}
	return hwnd;
	TRASH55
	TRASH100
	TRASH76
	return (HWND) 1;
}

// **************************
// ET.exe funcs
// **************************

int CHook::isGameSupported()
{
	TRASH78
	eth32.game.module = GetModuleHandleA("ET.exe");

	TRASH77
	TRASH79
	if (eth32.game.module)
	{
		int len = GetModuleFileNameA(eth32.game.module, eth32.game.path, sizeof(eth32.game.path));
		TRASH104
		if (len)
		{
			DWORD crc32 = Tools.crc32FromFile(eth32.game.path);
			#ifdef ETH32_DEBUG
				Debug.Log("ET.exe crc32: 0x%x", crc32);
			#endif

			while (len && eth32.game.path[len] != '\\')
				len--;
			if (len)
				eth32.game.path[len + 1] = '\0';

			int i = 0;
			for (; i < MAX_ET_VERSIONS; i++) {
				TRASH108
				if (crc32 == etlist[i].crc32) {
					TRASH4
					TRASH8
					return i;
					TRASH6
					TRASH9
				}
			}
		}
	}

	TRASH10
	return -1;
	TRASH11
}

void CHook::hookGame(bool state)
{
	static bool hooked = false;

	if (state && !hooked)
	{
		#ifdef ETH32_DEBUG
			Debug.Log("Hooking game functions");
		#endif		
		
		// kobject: disable annoying startup video
		if (eth32.et->etroq_video){
			memcpy((void *)eth32.et->etroq_safe, (LPVOID)eth32.et->etroq_video, 5);
			unprotect((LPVOID)eth32.et->etroq_video,5);
			memset( (BYTE *)eth32.et->etroq_video, 0x90, 5 );
			reprotect((LPVOID)eth32.et->etroq_video,5);
		}

		// Timenudge and snaps Hack
		if (eth32.settings.nudgeHack) {
			//NOP
			unprotect((void *)0x08091841,5);
			memset((void *)0x08091841, 0x90, 5 );
			reprotect((void *)0x08091841,5);
		
			unprotect((void *)0x08091869,5);
			memset((void *)0x08091869, 0x90, 5 );
			reprotect((void *)0x08091869,5);			
		}

		DETGAME(Cvar_Get)
		DETGAME(Cvar_Set2)
		DETGAME(FS_PureServerSetLoadedPaks)
		DETGAME(Cmd_AddCommand)
		//orig_Cmd_AddCommand = (Cmd_AddCommand_t) eth32.et->Cmd_AddCommand;
		DETGAME(Sys_LoadDll)
		DETGAME(LoadTGA)
		

		
		
		
		// causes problems when compiled in debug mode since M$ compiler adds garbage at the start of functions
#ifdef NDEBUG
		DETGAME(FS_Startup);
		DETGAME(FS_Startup2);
#endif

		if (eth32.et->FS_AddGameDirectory)
			orig_FS_AddGameDirectory = (FS_AddGameDirectory_t)eth32.et->FS_AddGameDirectory;
		
		if (eth32.et->FS_AddGameDirectory2)
			orig_FS_AddGameDirectory2 = (FS_AddGameDirectory2_t)eth32.et->FS_AddGameDirectory2;

		hooked = true;
	}
	else if (!state && hooked)
	{
		#ifdef ETH32_DEBUG
			Debug.Log("Un-Hooking game functions");
		#endif
		
		if (eth32.et->etroq_video){
			unprotect((LPVOID)eth32.et->etroq_video,5);
			memcpy((LPVOID)eth32.et->etroq_video, eth32.et->etroq_safe, 5);
			reprotect((LPVOID)eth32.et->etroq_video,5);
		}

		// Timenudge and snaps Hack
		if (eth32.settings.nudgeHack) {
			//NOP
			unprotect((void *)0x08091841,5);
			memset((void *)0x08091841, 0x90, 5 );
			reprotect((void *)0x08091841,5);
		
			unprotect((void *)0x08091869,5);
			memset((void *)0x08091869, 0x90, 5 );
			reprotect((void *)0x08091869,5);			
		}

		UNDETFUNC(Cvar_Get)
		UNDETFUNC(Cvar_Set2)
		UNDETFUNC(FS_PureServerSetLoadedPaks)
		UNDETFUNC(Cmd_AddCommand)
		UNDETFUNC(Sys_LoadDll)
		UNDETFUNC(FS_Startup)
		UNDETFUNC(FS_Startup2)
		UNDETFUNC(LoadTGA)
		hooked = false;
	}
}

cvar_t *__cdecl eth32_Cvar_Get(const char *var_name, const char *var_value, int flags)
{
	flags &= ~(CVAR_CHEAT+CVAR_ROM+CVAR_INIT);

	if (Tools.userCvarList){
		cvarInfo_t *c = Tools.userCvarList;
		while (c) {
			if (!_stricmp(c->name, var_name)){
				var_value = (const char *) c->ourValue;
				break;
			}
			c = c->next;
		}
	}
	cvar_t *ret = orig_Cvar_Get(var_name, var_value, flags);

	if (!strcmp(var_name, "cg_fov")){
#ifdef ETH32_DEBUG
		Debug.Log("found cg_fov @ %p", (void *)ret);
#endif
		eth32.game.cg_fov = ret;
	}

	if (!strcmp(var_name, "sensitivity")){
#ifdef ETH32_DEBUG
		Debug.Log("found sensitivity @ %p", (void *)ret);
#endif
		eth32.game.sensitivity = ret;
	}

	if (!_stricmp(var_name, "cg_railTrailTime")){
#ifdef ETH32_DEBUG
		Debug.Log("found cg_railTrailTime @ %p", (void *)ret);
#endif
		eth32.game.cg_railTrailTime = ret;
	}

	return ret;
}

void eth32_FS_PureServerSetLoadedPaks(const char *pakSums, const char *pakNames)
{
	static char fakePakSums[BIG_INFO_STRING];
	static char fakePakNames[BIG_INFO_STRING];

	strcpy(goodChecksumsPak, pakSums);
	pack_t *eth32Pack = Tools.getPack(eth32.settings.pk3file);

	// If not pure server
	if (!strlen(pakSums) && !strlen(pakNames))
	{
		orig_FS_PureServerSetLoadedPaks(pakSums, pakNames);
	// Add eth32 pk3
	}
	else if (eth32Pack)
	{
		sprintf(fakePakSums, "%s%i ", pakSums, eth32Pack->checksum);
		sprintf(fakePakNames, "%s %s/%s", pakNames, eth32Pack->pakGamename, eth32Pack->pakBasename);
		orig_FS_PureServerSetLoadedPaks(fakePakSums, fakePakNames);
	}
	else
	{
		orig_FS_PureServerSetLoadedPaks(pakSums, pakNames);
	}
}

void __cdecl eth32_FS_Startup( const char *gameName )
{
	char eth32Path[1024];
	strcpy(eth32Path, eth32.path);

	// strip the trailing '\'
	//eth32Path[strlen(eth32Path)-1] = '\0';

#ifdef ETH32_DEBUG
	Debug.Log("FS_Startup: adding %s",eth32Path);
#endif

	orig_FS_AddGameDirectory(eth32Path, "");
	orig_FS_Startup(gameName);
}

// ET 2.60 and 2.60b use optimizations that eliminate the last arg
// but pass them trough the edi register instead
void eth32_FS_Startup2()
{
	DWORD _esi;
	__asm { mov _esi, esi };

	char eth32Path[1024];
	char dir[] = "";
	strcpy(eth32Path, eth32.path);

	// strip the trailing '\'
	//eth32Path[strlen(eth32Path)-1] = '\0';

#ifdef ETH32_DEBUG
	Debug.Log("FS_Startup2: adding %s",eth32Path);
#endif

	// this one likes it in edi
	DWORD ptrr = (DWORD)dir;
	__asm { mov edi, ptrr }
	orig_FS_AddGameDirectory2(eth32Path);

	__asm {
		mov esi, _esi
		call orig_FS_Startup2;
	}
}

void* __cdecl eth32_Sys_LoadDll(const char *name, char *fqpath , int (QDECL **entryPoint)(int, ...), int (QDECL *systemcalls)(int, ...))
{
#ifdef ETH32_DEBUG
	Debug.Log("Sys_LoadDll: %s", name);
#endif

	void *ret = orig_Sys_LoadDll(name, fqpath, entryPoint, systemcalls);

	if (ret && !strcmp(name, "cgame") && eth32.cg.supported != -1) {
		// etpro check
		if (eth32.cgMod->crc32 != 0xC54721AF)
			*entryPoint = eth32_CG_vmMain;
		orig_CG_vmMain = (vmMain_t) GetProcAddress(eth32.cg.module, "vmMain");
#ifdef ETH32_DEBUG
		Debug.Log("orig CG_vmMain=%p vm->entryPoint=%p VM_DllSyscall=%p orig_syscall=%p", orig_CG_vmMain, entryPoint, systemcalls, orig_syscall);
#endif
	}

#ifdef ETH32_DEBUG
	if (ret && !strcmp(name, "ui"))
	{
		Debug.Log("orig UI_vmMain=%p vm->entryPoint=%p VM_DllSyscall=%p orig_syscall=%p", *entryPoint, entryPoint, systemcalls, orig_syscall);
	}
#endif

	return ret;
}

void CHook::UnreferenceBadPk3()
{
	// Not pure server
	if (!strlen(goodChecksumsPak))
		return;

	// Parse all paks
	searchpath_t *browse = *(searchpath_t **)eth32.et->fs_searchpaths;
	for (; browse; browse = browse->next)
	{
		if (!browse->pack)
			continue;

		// Parse all checksums for find this one
		bool purePak = false;
		char buf[16];
		sprintf_s(buf,"%i", browse->pack->checksum);
		if (strstr(buf, goodChecksumsPak))
			purePak = true;

		// If not in server pk3 list and referenced
		if (!purePak && browse->pack->referenced && strstr(browse->pack->pakFilename, eth32.settings.pk3file))
			browse->pack->referenced = 0;
	}
}

// hooking some console commands so we can unlock our pk3 on time
xcommand_t orig_cc_vid_restart;
xcommand_t orig_cc_connect;
xcommand_t orig_cc_start_attack;
xcommand_t orig_cc_end_attack;
xcommand_t orig_cc_StartCrouch;
xcommand_t orig_cc_EndCrouch;

void eth32_cc_StartCrouch(void)
{
	Aimbot.SetUserCrouchStatus(true);
	orig_cc_StartCrouch();
}

void eth32_cc_EndCrouch(void)
{
	Aimbot.SetUserCrouchStatus(false);
	orig_cc_EndCrouch();
}

void eth32_cc_vid_restart (void)
{
	Hook.hookGame(true);
	Tools.SaveSettings();
	//Irc.Kill();
	Gui.DestroyAllWindows();
	DETFUNC(CreateWindowExA);
	orig_cc_vid_restart();
}

void eth32_cc_connect (void)
{
	Hook.hookGame(true);
	orig_cc_connect();
}

void eth32_cc_start_attack (void)
{
	if (!eth32.cg.hasInit || !eth32.cg.snap || !eth32.cg.currentWeapon){
		orig_cc_start_attack();
		return;
	}

	Aimbot.SetUserAttackStatus(true);

	// kobject: block user fire in this mode, suggest we set atkValidate on a per-weapon basis 
	if ((eth32.cg.currentWeapon->attribs & WA_USER_DEFINED) && (eth32.cg.currentWeapon->attribs & WA_BLOCK_FIRE) && eth32.settings.atkValidate )
		return;
	
	// check for heat
	if ((eth32.cg.currentWeapon->attribs & WA_OVERHEAT) && eth32.cg.snap->ps.curWeapHeat > 200)
		return;

	// kobj: this one first
	if ((eth32.cg.currentWeapon->attribs & WA_BALLISTIC) && eth32.settings.grenadeAutoFire && Aimbot.GrenadeTicking){
		// needed to keep this false flag from messing with grenadebot
		Aimbot.SetUserAttackStatus(false);
		return;
	}

	// grenade bot - block fire if no firing solution
	if ((eth32.cg.currentWeapon->attribs & WA_BALLISTIC) && eth32.settings.grenadeBot && eth32.settings.grenadeBlockFire && !Aimbot.GrenadeFireOK)
		return;

	orig_cc_start_attack();
}

void eth32_cc_end_attack (void)
{
	Aimbot.SetUserAttackStatus(false);
	orig_cc_end_attack();
}

void __cdecl eth32_Cmd_AddCommand(const char *cmd_name, xcommand_t function)
{
	if (!strcmp(cmd_name, "vid_restart"))
	{
		orig_cc_vid_restart = function;
		function = eth32_cc_vid_restart;
	}
	else if (!strcmp(cmd_name, "connect"))
	{
		orig_cc_connect = function;
		function = eth32_cc_connect;
	}
	else if (!strcmp(cmd_name, "+attack"))
	{
		orig_cc_start_attack = function;
		function = eth32_cc_start_attack;
	}
	else if (!strcmp(cmd_name, "-attack"))
	{
		orig_cc_end_attack = function;
		function = eth32_cc_end_attack;
	}
	else if (!strcmp(cmd_name, "+movedown"))
	{
		orig_cc_StartCrouch = function;
		function = eth32_cc_StartCrouch;
	}
	else if (!strcmp(cmd_name, "-movedown"))
	{
		orig_cc_EndCrouch = function;
		function = eth32_cc_EndCrouch;
	}
	
	orig_Cmd_AddCommand(cmd_name, function);
}

// **************************
// cgame funcs
// **************************

int CHook::isCgameSupported(LPCSTR lpLibName)
{
	DWORD crc32 = Tools.crc32FromFile(lpLibName);
	if (!crc32) 
		return -1;

	int i = 0;
	for (; i < MAX_CG_MODS; i++)
		if (crc32 == cglist[i].crc32)
			return i;

	return -1;
}

void CHook::hookCgame(bool state)
{
	static bool hooked = false;

	if (state && !hooked)
	{
		#ifdef ETH32_DEBUG
			Debug.Log("Hooking cgame functions...");
		#endif

#ifndef ETH32_PRELIM_OFFSETS // defined only to test skeleton hack on new mods
		DETCGAME(CG_Player)
		// this doesn't work on etpro... get head hmodel directly from cgame
		if (eth32.cgMod->type != MOD_ETPRO)
			DETCGAME(CG_PositionRotatedEntityOnTag)

		DETCGAME(CG_PlayVoiceChat)
		DETCGAME(CG_EntityEvent)
		DETCGAME(CG_DamageFeedback)
		DETCGAME(CG_WeaponFireRecoil)
		DETCGAME(CG_Respawn)
		DETCGAME(CG_FinishWeaponChange)	// using this for more than just scope walk now
		DETCGAME(CG_AddPMItem)
		DETCGAME(CG_AddPMItem2)
		DETCGAME(CG_DrawPlayerStatusHead)
		DETCGAME(CG_DrawPlayerStats)
		DETCGAME(CG_DrawPlayerStatus)
		DETCGAME(CG_CenterPrint)
		DETCGAME(CG_PriorityCenterPrint)
		DETCGAME(CG_ChargeTimesChanged)
		DETCGAME(CG_CalculateReinfTime)
		DETCGAME(CG_CalcViewValues)
		DETCGAME(CG_AddPlayerWeapon)
#endif
		DETCGAME(CG_SetLerpFrameAnimationRate)

		if (eth32.cgMod->type == MOD_ETPRO)
			this->hookEtpro(true);		
		else 
			orig_CG_dllEntry = (dllEntry_t) DetourFunction((BYTE *) GetProcAddress(eth32.cg.module, "dllEntry"), (BYTE *) eth32_CG_dllEntry);

		orig_CG_Trace = (CG_Trace_t)((DWORD)eth32.cg.module + eth32.cgMod->CG_Trace);
		if (eth32.cgMod->CG_Trace && eth32.cgMod->type == MOD_TCE)
			DETCGAME(CG_Trace)

		if(eth32.cgMod->CG_AllocLocalEntity) orig_CG_AllocLocalEntity = (CG_AllocLocalEntity_t) ((DWORD)eth32.cg.module + eth32.cgMod->CG_AllocLocalEntity);
		if(eth32.cgMod->BG_FindClipForWeapon) orig_BG_FindClipForWeapon = (BG_FindClipForWeapon_t) ((DWORD)eth32.cg.module + eth32.cgMod->BG_FindClipForWeapon);
		if(eth32.cgMod->BG_FindAmmoForWeapon) orig_BG_FindAmmoForWeapon = (BG_FindAmmoForWeapon_t) ((DWORD)eth32.cg.module + eth32.cgMod->BG_FindAmmoForWeapon);
		if(eth32.cgMod->predictedPS) eth32.cg.predictedps = (playerState_t *) ((DWORD)eth32.cg.module + eth32.cgMod->predictedPS);
				
		hooked = true;
	}
	else if (!state && hooked)
	{
		#ifdef ETH32_DEBUG
			Debug.Log("Un-Hooking cgame functions");
		#endif
		UNDETFUNC(CG_Player)
		UNDETFUNC(CG_PositionRotatedEntityOnTag)
		UNDETFUNC(CG_FinishWeaponChange)
		UNDETFUNC(CG_PlayVoiceChat)
		UNDETFUNC(CG_EntityEvent)
		UNDETFUNC(CG_DamageFeedback)
		UNDETFUNC(CG_WeaponFireRecoil)
		UNDETFUNC(CG_Respawn)
		UNDETFUNC(CG_FinishWeaponChange)
		UNDETFUNC(CG_AddPMItem)
		UNDETFUNC(CG_AddPMItem2)
		UNDETFUNC(CG_DrawPlayerStatusHead)
		UNDETFUNC(CG_DrawPlayerStats)
		UNDETFUNC(CG_DrawPlayerStatus)
		UNDETFUNC(CG_CenterPrint)
		UNDETFUNC(CG_PriorityCenterPrint)
		UNDETFUNC(CG_ChargeTimesChanged)
		UNDETFUNC(CG_SetLerpFrameAnimationRate)
		UNDETFUNC(CG_CalculateReinfTime)
		UNDETFUNC(CG_CalcViewValues)
		UNDETFUNC(CG_AddPlayerWeapon)
		UNDETFUNC(CG_dllEntry)
		
		if (!eth32.cg.bools.spawntimerReady) UNDETFUNC(CG_CalculateReinfTime)
		
		// this doesn't work on etpro... get head hmodel directly from cgame
		if (eth32.cgMod->type != MOD_ETPRO)
			UNDETFUNC(CG_PositionRotatedEntityOnTag)

		if (eth32.cgMod->type == MOD_TCE)
			UNDETFUNC(CG_Trace)
		else
			orig_CG_Trace = NULL;

		orig_CG_AllocLocalEntity = NULL;
		orig_BG_FindClipForWeapon = NULL;
		orig_BG_FindAmmoForWeapon = NULL;

		hooked = false;
	}
}

void CHook::hookEtpro(bool state)
{
	if (eth32.cgMod->crc32 != 0xC54721AF ){
#ifdef ETH_DEBUG
		Debug.Log("This etpro version is not supported!");
#endif
		return;
	}

	if (state) {
		/*			disable relevant section of CG_DrawActiveFrame security function
		30025181	C705 BCA99B32 >MOV DWORD PTR DS:[329BA9BC],0
					C705 68A20E30 >MOV DWORD PTR DS:[300EA268],0
					E9 6515B32F    JMP 3002527B
					C7 05 BC A9 9B 32 00 00 00 00 C7 05 68 A2 0E 30 00 00 00 00 E9 65 15 B3 2F
		*/
		char aac[32] = "\xC7\x05\xBC\xA9\x9B\x32\x00\x00\x00\x00\xC7\x05"
					   "\x68\xA2\x0E\x30\x00\x00\x00\x00\xE9\xE1\x00\x00"
					   "\x00\x90\x90\x90\x90\x90\x90";

		// adjust for cgame image base (the jmp is relative short so doesn't change)
		DWORD msFlagA = (DWORD) eth32.cg.module + 0x29BA9BC;
		DWORD msFlagB = (DWORD) eth32.cg.module + 0xEA268;
		memcpy(aac+2, &msFlagA, 4);
		memcpy(aac+12, &msFlagB, 4);

		/*	lame hack to prevent crash in 21680 on new mapload
			mov eax, 4488a0
			xor ebx, ebx 
			jmp 300217D6

			B9 A0 88 44 00 33 D2 EB 3D 90 90 90 */				
		char aac2[13] = "\xB9\xA0\x88\x44\x00\x33\xD2\xEB\x3D\x90\x90\x90";

		unprotect((void *)((DWORD)eth32.cg.module+0x24E30),0xFC7);
		memcpy((void *)((DWORD)eth32.cg.module+0x25181), aac, 31);			
		reprotect((void *)((DWORD)eth32.cg.module+0x24E30),0xFC7);

		// lame but necessary to avoid crash on new mapload
		unprotect((void *)((DWORD)eth32.cg.module+0x21680), 0x36B5);
		memcpy((void *)((DWORD)eth32.cg.module+0x21790), aac2, 12);
		memset((void *)((DWORD)eth32.cg.module+0x218AE), 0x90, 18);		// nop out unnecessary security check :P
		reprotect((void *)((DWORD)eth32.cg.module+0x21680), 0x36B5);

		orig_etproAntiCheat = (etproAntiCheat_t) DetourFunction((BYTE *)eth32.cg.module+0x1BA0, (BYTE *)eth32_etproAntiCheat);
		orig_etproAntiCheat2 = (etproAntiCheat2_t) DetourFunction((BYTE *)eth32.cg.module+0x21680, (BYTE *)eth32_etproAntiCheat2);

		// kobject: some creative tinkering to catch voicechat messages
		char tramp[17];
		DWORD targ = (DWORD) etpro_CatchVoiceChat - ((DWORD)eth32.cg.module+0x8A71F+5);
		DWORD ofsEsp = (DWORD) &savedESP;
		memset(tramp, 0x90, 16);			// NOP
		tramp[0] = '\x89';					// 8A719	MOV dword ptr, esp		(save stack ptr)
		tramp[1] = '\x25';					//			MOD dword ptr, esp
		memcpy( tramp+2, &ofsEsp, 4 );		//			MOV dword ptr, esp
		tramp[6] = '\xe9';					// 8A71F	JMP etpro_CatchVoiceChat (detour it)
		memcpy(tramp+7, &targ, 4);

		unprotect((void *)((DWORD)eth32.cg.module+0x8A719), 17);
		memcpy((void *)((DWORD)eth32.cg.module+0x8A719), tramp, 16);
		reprotect((void *)((DWORD)eth32.cg.module+0x8A719), 17);

		// disable compass:
		unprotect((void *)((DWORD)eth32.cg.module+0x3D6C0), 5);
		*(char *) ((DWORD)eth32.cg.module+0x3D6C0) = '\xc3';		// ret
		reprotect((void *)((DWORD)eth32.cg.module+0x3D6C0), 5);
	} else {
		DetourRemove((BYTE *)orig_etproAntiCheat, (BYTE *)eth32_etproAntiCheat);
		DetourRemove((BYTE *)orig_etproAntiCheat2, (BYTE *) eth32_etproAntiCheat2);
		orig_etproAntiCheat2 = NULL;
		orig_etproAntiCheat = NULL;
	}
}

void CHook::InitCgameData(void)
{
	TRASH2
	DWORD dwBase	= (DWORD)eth32.cg.module;
	DWORD dwCent	= dwBase + eth32.cgMod->centBase;
	DWORD dwCi		= dwBase + eth32.cgMod->ciBase;
	TRASH19
	TRASH20
	
	for (int i=0; i<MAX_CLIENTS ; i++) {
		// easier to use player_t pointers this way
		eth32.cg.players[i].clientNum		= i;

		// cent data
		eth32.cg.players[i].cent			= (centity_t*)		(dwCent);
		eth32.cg.players[i].currentState	= (entityState_t*)	(dwCent + eth32.cgMod->centCurrentStateOffset);
		// if we haven't defined lerpOrigin yet, default to currentState->pos.trBase
		if (eth32.cgMod->centLerpOrigin)
			eth32.cg.players[i].lerpOrigin	= (float*)			(dwCent + eth32.cgMod->centLerpOrigin);
		else
			eth32.cg.players[i].lerpOrigin	= eth32.cg.players[i].currentState->pos.trBase;

		// clientinfo data
		eth32.cg.players[i].infoValid		= (bool*)			(dwCi + eth32.cgMod->ciInfoValidOffset);
		eth32.cg.players[i].name			= (char*)			(dwCi + eth32.cgMod->ciNameOffset);
		eth32.cg.players[i].team			= (int*)			(dwCi + eth32.cgMod->ciTeamOffset);
		eth32.cg.players[i].health			= (int*)			(dwCi + eth32.cgMod->ciHealthOffset);
		eth32.cg.players[i].cls				= (int*)			(dwCi + eth32.cgMod->ciClassOffset);
		
		dwCent += eth32.cgMod->centSize;
		dwCi   += eth32.cgMod->ciSize;
	}

	dwBase	= (DWORD)eth32.cg.module;//
	dwCent	= dwBase + eth32.cgMod->centBase; // maybe a better way of doing this so we don't have to set it twice 
	dwCi	= dwBase + eth32.cgMod->ciBase;//

	for (int i=0; i<MAX_GENTITIES ; i++) {
		// easier to use player_t pointers this way
		eth32.cg.gentity[i].clientNum		= i;
		// cent data
		eth32.cg.gentity[i].cent			= (centity_t*)		(dwCent);
		eth32.cg.gentity[i].currentState	= (entityState_t*)	(dwCent + eth32.cgMod->centCurrentStateOffset);

		if (eth32.cgMod->centLerpOrigin)
			eth32.cg.gentity[i].lerpOrigin	= (float*)			(dwCent + eth32.cgMod->centLerpOrigin);
		else
			eth32.cg.gentity[i].lerpOrigin	= eth32.cg.gentity[i].currentState->pos.trBase;

		// clientinfo data
		eth32.cg.gentity[i].infoValid		= (bool*)			(dwCi + eth32.cgMod->ciInfoValidOffset);

		dwCent += eth32.cgMod->centSize;
		dwCi   += eth32.cgMod->ciSize;
	}

	eth32.cg.refdef				= (refdef_t*)	(dwBase + eth32.cgMod->refdef);
	eth32.cg.refdefViewAngles	= (float*)		(dwBase + eth32.cgMod->refdefViewAngles);

	// temp check for valid pmext, until we have info on all mods
	// kobj: note:: etpro's pmext isn't really at the location in offsets.ini
	// i just 'made up' that value so pmext->sprintTime happens to be correct
	if (eth32.cgMod->pmext)
		eth32.cg.pmext			= (pmoveExt_t*)	(dwBase + eth32.cgMod->pmext);
	else eth32.cg.pmext			= NULL;

	InitWeaponList();
	eth32.cg.currentWeapon = &nullWeapon;

#ifdef ETH32_DEBUG
	Debug.Log("Initializing cgame...");
#endif
}

void CHook::InitWeaponList(void)
{
	switch (eth32.cgMod->type)
	{
	case MOD_ETMAIN:
	case MOD_ETPUB:
	case MOD_ETPRO:
		eth32.cg.weaponList = normalWeapons;
		eth32.cg.numWeapons = 49;
		break;
	case MOD_TCE:
		eth32.cg.weaponList = tceWeapons;
		eth32.cg.numWeapons = sizeof(tceWeapons) / sizeof(tceWeapons[0]);
		break;
	case MOD_JAYMOD1:
	case MOD_JAYMOD2:
		eth32.cg.weaponList = normalWeapons;
		eth32.cg.numWeapons = 52;
		break;
	case MOD_NOQUARTER:
		eth32.cg.weaponList = noquarterWeapons;
		eth32.cg.numWeapons = 62;
		break;
	default:
		eth32.cg.weaponList = NULL;
		eth32.cg.numWeapons = 0;
		break;
	}
}

// Modify cgame's command table directly, much faster than doing string compares on every console command
xcommand_t orig_cc_OpenScores;
xcommand_t orig_cc_CloseScores;
xcommand_t orig_cc_OpenTopshots;
xcommand_t orig_cc_CloseTopshots;
xcommand_t orig_cc_OpenStats;
xcommand_t orig_cc_CloseStats;

void eth32_cc_OpenScores(void)
{
	Gui.ScoreboardStatus(true);
	orig_cc_OpenScores();
}

void eth32_cc_CloseScores(void)
{
	Gui.ScoreboardStatus(false);
	orig_cc_CloseScores();
}

void eth32_cc_OpenTopshots(void)
{
	Gui.TopshotsStatus(true);
	orig_cc_OpenTopshots();
}

void eth32_cc_CloseTopshots(void)
{
	Gui.TopshotsStatus(false);
	orig_cc_CloseTopshots();
}

void eth32_cc_OpenStats(void)
{
	Gui.StatspanelStatus(true);
	orig_cc_OpenStats();
}

void eth32_cc_CloseStats(void)
{
	Gui.StatspanelStatus(false);
	orig_cc_CloseStats();
}

// modified a bit for clean unloading
void CHook::ModifyCgameCommands()
{
	if (!eth32.cgMod->commands || !eth32.cgMod->numCommands) {
#ifdef ETH32_DEBUG
		Debug.Log("Unable to modify console commands: cgMod->commands or cgMod->numCommands not defined");
#endif
		return;
	}

	consoleCommand_t *cc = (consoleCommand_t*)((DWORD)eth32.cg.module + eth32.cgMod->commands);
	DWORD oPr1, oPr2;
	VirtualProtect((BYTE *)cc, sizeof(consoleCommand_t)*eth32.cgMod->numCommands, PAGE_READWRITE, &oPr1);
	for (int i=0 ; i<eth32.cgMod->numCommands ; i++) {
		if (!_stricmp(cc[i].cmd, "+scores")) {
			orig_cc_OpenScores = cc[i].function;			
			cc[i].function = (xcommand_t)eth32_cc_OpenScores;			
		}
		else if (!_stricmp(cc[i].cmd, "-scores")) {
			orig_cc_CloseScores = cc[i].function;
			cc[i].function = (xcommand_t)eth32_cc_CloseScores;
		}
		else if (!_stricmp(cc[i].cmd, "+topshots")) {
			orig_cc_OpenTopshots = cc[i].function;
			cc[i].function = (xcommand_t)eth32_cc_OpenTopshots;
		}
		else if (!_stricmp(cc[i].cmd, "-topshots")) {
			orig_cc_CloseTopshots = cc[i].function;
			cc[i].function = (xcommand_t)eth32_cc_CloseTopshots;
		}
		else if (!_stricmp(cc[i].cmd, "+stats")) {
			orig_cc_OpenStats = cc[i].function;
			cc[i].function = (xcommand_t)eth32_cc_OpenStats;
		}
		else if (!_stricmp(cc[i].cmd, "-stats")) {
			orig_cc_CloseStats = cc[i].function;
			cc[i].function = (xcommand_t)eth32_cc_CloseStats;
		}
	}
	VirtualProtect((BYTE *)cc, sizeof(consoleCommand_t)*eth32.cgMod->numCommands, oPr1, &oPr2);
	#ifdef ETH32_DEBUG
		Debug.Log("Modified console command functions.");
	#endif
}

void CHook::SaveWeapons(const char *filename)
{
	char buffer[256];
	weapdef_t *weap;
	char p[256];
	char q[32];

	int num = sizeof(normalWeapons) / sizeof(normalWeapons[0]);
	for (int i=0 ; i<num ; i++)
	{
		weap = &normalWeapons[i];
		if (weap->attribs & WA_USER_DEFINED) {	// only save weapon settings that allowed to be modified
			sprintf_s(p,256, "%i %i %i %i %s", weap->range, weap->delay, weap->headTraces, weap->bodyTraces, (weap->autofire)?"true":"false");
			sprintf_s(q,32,"weap%02i", i);		
			WritePrivateProfileString("Normal Weapons", q, p, filename);
		}
	}
	num = sizeof(tceWeapons) / sizeof(tceWeapons[0]);
	for (int i=0 ; i<num ; i++)
	{
		weap = &tceWeapons[i];
		if (weap->attribs & WA_USER_DEFINED) {	// only save weapon settings that allowed to be modified
			sprintf_s(q,32,"weap%02i", i);	
			sprintf_s(buffer, 256, "%i %i %i %i %s", weap->range, weap->delay, weap->headTraces, weap->bodyTraces, (weap->autofire)?"true":"false");
			WritePrivateProfileString("TCE Weapons", q, buffer, filename);
		}
	}
	num = sizeof(noquarterWeapons) / sizeof(weapdef_t);
	for (int i=0 ; i<num ; i++)
	{
		weap = &noquarterWeapons[i];
		if (weap->attribs & WA_USER_DEFINED) {	// only save weapon settings that allowed to be modified
			sprintf_s(buffer, 256, "%i %i %i %i %s", weap->range, weap->delay, weap->headTraces, weap->bodyTraces, (weap->autofire)?"true":"false");
			sprintf_s(q,32,"weap%02i", i);	
			WritePrivateProfileString("NQ Weapons", q, buffer, filename);
		}
	}
}

void CHook::LoadWeapons(const char *filename)
{
	char buffer[256];
	char autofire[32];
	char q[32];
	weapdef_t *weap;

	int num = sizeof(normalWeapons) / sizeof(normalWeapons[0]);
	for (int i=0 ; i<num ; i++)
	{
		weap = &normalWeapons[i];
		if (weap->attribs & WA_USER_DEFINED) {	// only load weapon settings that allowed to be modified
			sprintf_s(q, 32, "weap%02i", i);
			GetPrivateProfileString("Normal Weapons", q, "10000 0 4 4 false", buffer, 256, filename);
			sscanf_s(buffer, "%i %i %i %i %s", &weap->range, &weap->delay, &weap->headTraces, &weap->bodyTraces, autofire, 32);
			weap->autofire = (strcmp(autofire, "false") != 0);
		}
	}
	num = sizeof(tceWeapons) / sizeof(tceWeapons[0]);
	for (int i=0 ; i<num ; i++)
	{
		weap = &tceWeapons[i];
		if (weap->attribs & WA_USER_DEFINED) {	// only load weapon settings that allowed to be modified
			sprintf_s(q, 32, "weap%02i", i);
			GetPrivateProfileString("TCE Weapons", q, "10000 0 4 4 false", buffer, 256, filename);
			sscanf_s(buffer, "%i %i %i %i %s", &weap->range, &weap->delay, &weap->headTraces, &weap->bodyTraces, autofire, 32);
			weap->autofire = (strcmp(autofire, "false") != 0);
		}
	}
	num = sizeof(noquarterWeapons) / sizeof(weapdef_t);
	for (int i=0 ; i<num ; i++)
	{
		weap = &noquarterWeapons[i];
		if (weap->attribs & WA_USER_DEFINED) {	// only load weapon settings that allowed to be modified
			sprintf_s(q, 32, "weap%02i", i);
			GetPrivateProfileString("NQ Weapons", q, "10000 0 4 4 false", buffer, 256, filename);
			sscanf_s(buffer, "%i %i %i %i %s", &weap->range, &weap->delay, &weap->headTraces, &weap->bodyTraces, autofire, 32);
			weap->autofire = (strcmp(autofire, "false") != 0);
		}
	}
}

void __cdecl eth32_CG_dllEntry(int (*syscallptr)(int arg,... ))
{
	#ifdef ETH32_DEBUG
		Debug.Log("CG_dllEntry(): 0x%x", syscallptr);
	#endif

	orig_CG_dllEntry(&eth32_CG_syscall);
}

static void CG_ParseScore( team_t team ) {
	int		i, j;
	int		offset;
	eth32.ocg.numScores = 0;	// It must be 0, otherwise client numbers will mess up.
	
	if( team == TEAM_AXIS ) {				
		offset = 4;
	} else {		
		offset = 2;
	}

	eth32.cg.numScores = atoi( Syscall.CG_Argv( offset - 1 ) );

	for(j = 0; j < (eth32.cg.numScores); j++) {
		i = eth32.ocg.numScores;

		eth32.ocg.scores[i].client = atoi(			Syscall.CG_Argv( offset + 0 + (j*7)));
		eth32.ocg.scores[i].ping = atoi(			Syscall.CG_Argv( offset + 2 + (j*7)));

		if ( eth32.ocg.scores[i].client < 0 || eth32.ocg.scores[i].client >= MAX_CLIENTS ) {
			eth32.ocg.scores[i].client = 0;
		}

		eth32.ocg.numScores++;
	}
}


// solcrushr - modified flow of syscall
// later on we may not want to call original for things like adding smoke/scope shader/etc
int __cdecl eth32_CG_syscall(int command, ...)
{
	int arg[10];
	va_list arglist;
	va_start(arglist, command);
	int count = 0;
	for (; count < 10; count++)
		arg[count] = va_arg(arglist, int);
	va_end(arglist);

	// we need to block some syscalls (i.e. drawing of sniper blackout)
	bool callOrig = true;
	int retval;

	switch (command)
	{
		case CG_R_ADDPOLYTOSCENE:
		{
			qhandle_t hShader = (qhandle_t) arg[0];
			if (hShader == eth32.cg.media.smokePuff) {
				if (!eth32.settings.smoketrnsp)
					return 0;

				Engine.AddPolyToScene((qhandle_t) arg[0], (polyVert_t *) arg[2]);
			}
		break;
		}

		case CG_R_ADDREFENTITYTOSCENE:
			Engine.AddRefEntity((refEntity_t*)arg[0]);
			break;
		case CG_R_RENDERSCENE:
		{
#ifdef ETH32_DEBUG
			static bool firstRun = true;
			if (firstRun && (DWORD)arg[0] > (DWORD)eth32.cg.module) {
				DWORD refdefAddr = (DWORD)arg[0] - (DWORD)eth32.cg.module;
				Debug.Log("Refdef found... [ 0x%X ] RefdefVA assumed... [ 0x%X ]", refdefAddr, refdefAddr + 0x1AC);
				firstRun = false;
			}
			refdef_t *rd = (refdef_t *) arg[0];
			VectorCopy(rd->viewaxis[0], eth32.cg.tempRenderVector);
#endif

			//kobject: TODO - make mod IS_SCOPED_WEAPON mod-safe
			//sol: think I might of resolved this
			if (eth32.cg.snap && eth32.game.cg_fov && IS_WEAPATTRIB(eth32.cg.snap->ps.weapon, WA_SCOPED) && (eth32.cgMod->type != MOD_TCE) && !eth32.settings.weaponZoom)
			{
				// modifying refdef values in PreFrame didn't do anything, so i had to put it in here
				refdef_t *refdef = (refdef_t *) arg[0];
				refdef->fov_x = eth32.game.cg_fov->value;
				float x = refdef->width / tan( refdef->fov_x / 360 * M_PI );
				refdef->fov_y = atan2( refdef->height, x );
				refdef->fov_y = refdef->fov_y * 360 / M_PI;
			}
			break;
		}
		case CG_GETSNAPSHOT: 
		{
			int oldTime = 0;
			if (eth32.cg.snap)
				oldTime = eth32.cg.snap->serverTime;

			int ret = orig_syscall(command, arg[0], arg[1]);
			eth32.cg.snap = (snapshot_t*)arg[1];
			
			Engine.CorrectServerAngles();

			eth32.cg.ping = (eth32.cg.snap->serverTime - oldTime)*0.001;
			return ret;
		}
		
		case CG_PRINT: {
			char *analise_str = (char *)arg[0];
			
			if( !analise_str )
				break;
				
			if (eth32.settings.autoVote) {
				// Auto vote
				// Do nothing if we called the vote (to avoid canceling it).
				if (strstr(analise_str, va("%s^7 called a vote.  Voting for: ",eth32.cg.players[eth32.cg.clientNum].name)))
					break;
				// If we aren't the caller then vote no.
				else {
					for(int i=0; i<MAX_CLIENTS; i++) {
						if (i != eth32.cg.clientNum) {
							if (strstr(analise_str, va("%s^7 called a vote.  Voting for: ",eth32.cg.players[i].name))) {
								Syscall.CG_SendConsoleCommand("vote no\n");
								break;	// Exit after voting to prevent flood kick.
							}
						}
					}
				}
			}

			// Anti inactivity kick
			if (strstr(analise_str, "^310 seconds until inactivity drop!\n")) {
				Syscall.CG_SendConsoleCommand("+leanright; -leanright\n");
			}
		
			break;
		}
		case CG_TRANSLATE_STRING: 
		{
			char *analise_str = (char *)arg[0];
			
			if( !analise_str )
				break;

			// Auto file complaint
			// Alexplay: The complaint message stays for a few ms even after we voted so we need 
			// to check if it's safe to vote so we don't get kicked for excess of flood
			if (eth32.settings.autoComplaint) {
				if (strstr(analise_str, "File complaint against ")) {
					if (eth32.cg.SafeVote)				
						Syscall.CG_SendConsoleCommand("vote yes\n");
					eth32.cg.SafeVote = false;	// Don't spam the command
				}
			}
			
			break;
		}
		case CG_GETSERVERCOMMAND:
		{
			// solcrushr - fixed: needed to call original before we check args
			int ret = orig_syscall(command, arg[0]);
			if (!arg[0])
				return ret;

			char *cmd = _strdup(Syscall.CG_Argv(0));

			if (!cmd || !*cmd){
				if (cmd)
					free(cmd);
				return ret;
			}
			
#if 0		// to catch etpro security communications and different server cmds
			char *cmm, *a, *bb, *cc, *dd;
			cmm = _strdup(Syscall.CG_Argv(0));
			a = _strdup(Syscall.CG_Argv(1));
			bb = _strdup(Syscall.CG_Argv(2));
			cc = _strdup(Syscall.CG_Argv(3));
			dd = _strdup(Syscall.CG_Argv(4));
			Debug.Log("-----------");
			Debug.Log("CG_GETSERVERCOMMAND: cmd=%s 1=%s 2=%s 3=%s !!!DecodedCmd=%i", cmm, a, bb, cc,dd,(int)decoded[0]);
			free(a);
			free(bb);
			free(cc);
			free(dd);
			free(cmm);
			Debug.Log("->>>");
			Debug.LogData(decoded, decIdx);
			Debug.Log("<<<------E-----");
#endif

			// have to handle things a bit differently for etpr0
			if (eth32.cgMod->type==MOD_ETPRO && (!strcmp(cmd,"c") || !strcmp(cmd,"tc")) ) {
				char text[MAX_SAY_TEXT+1];
				char buf[MAX_STRING_CHARS];
				text[0] = '\0';
				
				int cl = atoi(Syscall.CG_Argv(1));

				if (!strcmp(cmd,"c"))
					sprintf_s(buf, sizeof(buf), "%s^7: ^2%s", eth32.cg.players[cl].name, Syscall.CG_Argv(2));				
				else
					sprintf_s(buf, sizeof(buf), "%s^7: ^5%s", eth32.cg.players[cl].name, Syscall.CG_Argv(2));

				strncpy_s(text, sizeof(text), buf, _TRUNCATE);
				Gui.ChatMessage(text);
			}
            
			// send chat messages to Gui
			if ((!strcmp(cmd, "chat") || !strcmp(cmd, "tchat"))) {
				const char *s;
				char text[MAX_SAY_TEXT+1];
				if (atoi(Syscall.CG_Argv(3))) 
					s = Syscall.CG_LocalizeServerCommand(Syscall.CG_Argv(1));
				else 
					s = Syscall.CG_Argv(1);

				strncpy_s(text, MAX_SAY_TEXT+1, s, MAX_SAY_TEXT );
				Syscall.CG_RemoveChatEscapeChar(text);

				Gui.ChatMessage(text);
			
				if (!strncmp(s, "^oss: ", 6)) {					
					int num;
					sscanf(s, "^oss: %i", &num);
					if (num >= 0 && num < MAX_CLIENTS){
					Tools.auxSpecList[num] = true;
				}
			}
				
				if (!strncmp(s, "^oss_stop:", 10)) {
					int num;
					sscanf(s, "^oss_stop: %i", &num);
					if (num >= 0 && num < MAX_CLIENTS){
					Tools.auxSpecList[num] = false;
				}
			}
		}		

			// handle server-side obits
			if (!eth32.cg.serverObits && !strcmp(cmd, "cpm")) {
				Engine.ParseServerSideObituary((char*)Syscall.CG_Argv(1));
			}
			// prepare for pk3 unlocking
			else if (!strcmp(cmd, "spawnserver"))
				Hook.hookGame(true);
              
			if (eth32.cgMod->type != MOD_ETPRO && eth32.settings.autoPredictBots) {	
				if ( !strcmp( cmd, "sc0" ) ) {
					CG_ParseScore(TEAM_AXIS);
					return ret;
				} else if ( !strcmp( cmd, "sc1" ) ) {
					CG_ParseScore(TEAM_ALLIES);
					return ret;
				}
			}
			if (!strcmp(cmd, "gstats"))
				Engine.ParseWeaponStats();

			free(cmd);

			return ret;
		}
#ifdef  ETH32_PRELIM_OFFSETS
		// searching for lerpOrigin offset
		case CG_CM_BOXTRACE:
		{
			static bool found = false;

			if (!found) {
				DWORD startAddr = (DWORD)arg[1];

				for (int i=0 ; i < MAX_CLIENTS ; i++) {
					DWORD centStart = (DWORD)eth32.cg.players[i].cent;
					DWORD centEnd = centStart + eth32.cgMod->centSize;

					if (startAddr > centStart && startAddr < centEnd) {
						Debug.Log("Found lerpOrigin offset: 0x%X", startAddr - centStart);
						found = true;
						break;
					}
				}
			}
			break;
		}
#endif 
		case CG_R_DRAWSTRETCHPIC:
			callOrig = Draw.CheckPic(*(float *)&arg[0], *(float *)&arg[1], *(float *)&arg[2], *(float *)&arg[3], *(float *)&arg[4], *(float *)&arg[5], *(float *)&arg[6], *(float *)&arg[7], (qhandle_t)arg[8]);
			break;
		case CG_CVAR_REGISTER:
			if (arg[3] & CVAR_CHEAT)
				arg[3] &= ~CVAR_CHEAT;
			if (arg[3] & CVAR_ROM)
				arg[3] &= ~CVAR_ROM;

#ifdef ETH32_DEBUG
			// sol: TCE testing for iron sight issue
			if (!_strcmpi((const char*)arg[1], "cg_freeaim"))
				Debug.Log("Found cvar \"cg_freeaim\": addr = 0x%08X", (DWORD)arg[0] + 0x0C);
#endif

			callOrig = !Tools.SkipCgameCvar((vmCvar_t*)arg[0], (const char*)arg[1]);
			break;
		case CG_CVAR_SET:
			// jay mac sp00f
			if (!strcmp((char *)arg[0], "cl_mac")){
				if (strlen(eth32.settings.jayMac) == 1 && eth32.settings.jayMac[0] == '0')
					sprintf_s(eth32.settings.jayMac, MAX_STRING_CHARS, "%02x-%02x-%02x-%02x",  rand() % 256,  rand() % 256,  rand() % 256,  rand() % 256);
				arg[1] = (DWORD) &eth32.settings.jayMac[0];
				break;
			} else if (Tools.userCvarList) {
				cvarInfo_t *c = Tools.userCvarList;
				while (c) {
					if (!_stricmp(c->name, (char *)arg[0])){
						arg[1] = (DWORD)c->ourValue;
						break;
					}
					c = c->next;
				}
			}
			break;
		case CG_SETUSERCMDVALUE:
			Gui.LimbopanelStatus(arg[1] != 0x0);

			if (Aimbot.SensitivityLocked()){
				arg[2] = 0x0;
				break;
			} else  if (eth32.cg.currentWeapon && (eth32.cg.currentWeapon->attribs & WA_SCOPED) && eth32.settings.weaponZoom) {
				*(float *) &arg[2] = eth32.settings.scopedTurnSpeed;
			} else {
				*(float *) &arg[2] = 1.0f;
			}
			break;
		case CG_INGAME_POPUP:
			// special case: UIMENU_INGAME_MESSAGEMODE == 0x11 in NoQuarter
			if ((eth32.cgMod->type != MOD_NOQUARTER && arg[0] == UIMENU_INGAME_MESSAGEMODE) || (eth32.cgMod->type == MOD_NOQUARTER && arg[0] == 0x11)) {
				char buffer[32];
				Syscall.Cvar_VariableStringBuffer("cg_messageType", buffer, 32);
				callOrig = Gui.SetChatTarget(atoi(buffer));
			}
			break;
		case CG_GETGAMESTATE: 
		{
			if (!eth32.cg.gameState)
				eth32.cg.gameState = (gameState_t *) arg[0];
				
			/*for (int i=0; i<MAX_CLIENTS; i++) {
				const char *info = Engine.ConfigString(i + CS_PLAYERS);
				eth32.cg.players[i].Class = atoi(Info_ValueForKey( info, "c" ));
			}*/
			const char *info;
			for (int i=0; i<MAX_CLIENTS; i++) {
				int offset = eth32.cg.gameState->stringOffsets[i + CS_PLAYERS];
				if (offset != 0) {
					info = Info_ValueForKey((char *)(eth32.cg.gameState->stringData + offset), "c" );
					eth32.cg.players[i].Class = atoi(info);
				}
			}
			break;
		}
		case CG_S_REGISTERSOUND:
			return Sounds.HookSounds((char*)arg[0], arg[1]);
		case CG_S_STARTSTREAMINGSOUND: {
			const char *snd = (const char *) arg[0];
			if( !snd )
				break;

			// Don't play HQ sounds
			if( !eth32.settings.hqSounds && strstr(snd,"hq_") )
				return 0;
			
			break;
		}	
	
		case CG_S_STARTSOUND:
		case CG_S_STARTSOUNDEX:
		case CG_S_ADDREALLOOPINGSOUND:
		case CG_S_ADDLOOPINGSOUND:
		case CG_S_STARTLOCALSOUND:
			callOrig = Sounds.Process(command, arg);
			break;
	}

	if (callOrig) 
		retval = orig_syscall(command, arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7], arg[8], arg[9]);
	else return 0;
	
	switch (command) {
		case CG_R_REGISTERSHADERNOMIP:
		{
			if (!strcmp((char*)arg[0], "ui/cursor/3_cursor3")) {
				//printf("CG_R: %i\n", retval);
				eth32.cg.media.cursor = retval;
			}
			break;
		}

		case CG_R_CLEARSCENE:
			Engine.GentityDecal();
			break;
	}

	return retval;
	
}
// **********************
// CG vmMain
// **********************

int __cdecl eth32_CG_vmMain(int command, ...)
{
	int arg[12];
	int ret;
	va_list arglist;
	va_start(arglist, command);
	int count = 0;
	for (; count < 12; count++)
		arg[count] = va_arg(arglist, int);
	va_end(arglist);

	switch (command)
	{
		case CG_INIT: {
			Engine.CG_Init_Pre(arg[0], arg[1], arg[2]);
			break;
		}
		case CG_SHUTDOWN:
			return Engine.CG_Shutdown();
		case CG_DRAW_ACTIVE_FRAME:{
			// etpro cvar protection disable
			if (eth32.cgMod->type == MOD_ETPRO && eth32.cgMod->crc32 == 0xC54721AF)
				*(DWORD *)((DWORD)eth32.cg.module+0x12D6A0) = 0;
			return Engine.CG_DrawActiveFrame(arg[0], arg[1], arg[2]);
		}
	}

	ret = orig_CG_vmMain(command, arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7], arg[8], arg[9], arg[10], arg[11]);

	// postcall stuff
	switch (command) {
		case CG_INIT: 
			Engine.CG_Init_Post(arg[0], arg[1], arg[2]);
			// time to remove detours PB detects
			Hook.hookGame(false);
			eth32.cg.hasInit = true;
			break;
	}

	return ret;
}

void __cdecl eth32_CG_SetLerpFrameAnimationRate(centity_t *cent, clientInfo_t *ci, lerpFrame_t *lf, int anim)
{
	orig_CG_SetLerpFrameAnimationRate(cent, ci, lf, anim);

	if (cent->currentState.number >= 0 && cent->currentState.number < MAX_CLIENTS)
		eth32.cg.players[cent->currentState.number].lf = lf;


#ifdef ETH32_PRELIM_OFFSETS
	static bool found = false;
	static int iPrevClientNum = -1;
	static DWORD dwPrevCi = 0;

	if (!found) {
		if (!eth32.cgMod->centBase || !eth32.cgMod->centSize) {
			Debug.Log("Unable to calc clientinfo (Cent base & size not defined)");
			found = true;
			return;
		}

		int iNewClientNum = Tools.ClientNumForAddr((DWORD)cent);

		if (iPrevClientNum < 0) {
			iPrevClientNum = iNewClientNum;
			dwPrevCi = (DWORD)ci;
		}
		else if (iNewClientNum != iPrevClientNum) {
			DWORD dwCiSize;
			DWORD dwCiBase;
			DWORD dwNewCi = (DWORD)ci;

			if (iNewClientNum < iPrevClientNum) 
				dwCiSize = (dwPrevCi - dwNewCi) / (iPrevClientNum - iNewClientNum);
			else dwCiSize = (dwNewCi - dwPrevCi) / (iNewClientNum - iPrevClientNum);
				dwCiBase = dwNewCi - (iNewClientNum * dwCiSize) - (DWORD)eth32.cg.module;

			Debug.Log("Found clientinfo... [ Base: 0x%X ] [ Size: 0x%X ]", dwCiBase, dwCiSize);

			found = true;
			return;
		}
	}
#endif // ETH32_PRELIM_OFFSETS
}

void __cdecl eth32_CG_FinishWeaponChange(int lastweap, int newweap)
{
	if ((eth32.cgMod->type != MOD_TCE) && IS_WEAPATTRIB(lastweap, WA_SCOPED) && IS_WEAPATTRIB(newweap, WA_UNSCOPED) && !(Syscall.isKeyActionDown("+reload") || Syscall.isKeyActionDown("weapalt")))
			return;

	// reset autofire/senslock on weapon change
	Aimbot.Autofire(false);	
	Aimbot.LockSensitivity(false);

	if (eth32.cg.weaponList && eth32.cg.snap && eth32.cg.snap->ps.weapon < eth32.cg.numWeapons)
		eth32.cg.currentWeapon = &eth32.cg.weaponList[eth32.cg.snap->ps.weapon];
	else
		eth32.cg.currentWeapon = &nullWeapon;

	//Engine.SetWeaponSpread();

	#ifdef ETH32_DEBUG
		Debug.Log("WeapChange: old weapon = %i new weapon = %i (%s) spread=%f", lastweap, newweap, eth32.cg.currentWeapon->name, eth32.cg.spreadIndex);
	#endif

	orig_CG_FinishWeaponChange(lastweap, newweap);
}

void __cdecl eth32_CG_Respawn(qboolean revived)
{
	orig_CG_Respawn(revived);

	// reset aimbot
	Aimbot.Respawn();

	// sol: odd I know, but a good non-intensive place to make sure cg_teamchatheight is 0
	if (eth32.cg.cg_teamchatheight)
		eth32.cg.cg_teamchatheight->integer = 0;

	//Debug.Log("Respawn: weapon = %i", eth32.cg.snap->ps.weapon);
}

void __cdecl eth32_CG_PlayVoiceChat(bufferedVoiceChat_t *vchat)
{
	// hack need for Jaymod 2.1.x which added an extra member to bufferedVoiceChat_t struct
	Gui.ChatMessage(*vchat->message ? vchat->message : vchat->message+4);
	orig_CG_PlayVoiceChat(vchat);
}

void __cdecl eth32_CG_AddPMItem(popupMessageType_t type, const char *message, qhandle_t shader)
{
	if (message && *message)
		Gui.ChatMessage(message);
	if (eth32.cg.snap->ps.pm_type == PM_INTERMISSION)
		orig_CG_AddPMItem(type, message, shader);
}

void __cdecl eth32_CG_AddPMItem2(popupMessageType_t type, const char *message, qhandle_t shader, int unknown)
{
	if (message && *message)
		Gui.ChatMessage(message);
	if (eth32.cg.snap->ps.pm_type == PM_INTERMISSION)
		orig_CG_AddPMItem2(type, message, shader, unknown);
}

void __cdecl eth32_CG_DrawPlayerStatusHead(void)
{
	if (eth32.settings.guiOriginal)
		orig_CG_DrawPlayerStatusHead();
	return;
}

void __cdecl eth32_CG_DrawPlayerStats(void)
{
	if (eth32.settings.guiOriginal)
		orig_CG_DrawPlayerStats();
	return;
}

void __cdecl eth32_CG_DrawPlayerStatus(void)
{
	if (eth32.settings.guiOriginal)
		orig_CG_DrawPlayerStatus();
	return;
}

void __cdecl eth32_CG_CenterPrint(const char *str, int y, int charWidth)
{
	Gui.CenterPrint(str, 0);
}

void __cdecl eth32_CG_PriorityCenterPrint(const char *str, int y, int charWidth, int priority)
{
	Gui.CenterPrint(str, priority);
}

//kobject: what about TCE?
void __cdecl eth32_CG_ChargeTimesChanged(void)
{
	// NoQuarter changed CS_CHARGETIMES
	int index = (eth32.cgMod->type == MOD_NOQUARTER) ? 0x23 : 0x26;
	const char *info = Engine.ConfigString(index);

	eth32.cg.chargeTimes.soldier[0] = atoi(Info_ValueForKey( info, "axs_sld" ));
	eth32.cg.chargeTimes.soldier[1] = atoi(Info_ValueForKey( info, "ald_sld" ));
	eth32.cg.chargeTimes.medic[0] = atoi(Info_ValueForKey( info, "axs_mdc" ));
	eth32.cg.chargeTimes.medic[1] = atoi(Info_ValueForKey( info, "ald_mdc" ));
	eth32.cg.chargeTimes.engineer[0] = atoi(Info_ValueForKey( info, "axs_eng" ));
	eth32.cg.chargeTimes.engineer[1] = atoi(Info_ValueForKey( info, "ald_eng" ));
	eth32.cg.chargeTimes.lt[0] = atoi(Info_ValueForKey( info, "axs_lnt" ));
	eth32.cg.chargeTimes.lt[1] = atoi(Info_ValueForKey( info, "ald_lnt" ));
	eth32.cg.chargeTimes.covops[0] = atoi(Info_ValueForKey( info, "axs_cvo" ));
	eth32.cg.chargeTimes.covops[1] = atoi(Info_ValueForKey( info, "ald_cvo" ));

	orig_CG_ChargeTimesChanged();
}

// **************************
// ui funcs
// **************************

void CHook::hookUi(bool state)
{
	static bool hooked = false;

	// TODO: WTF?! why UI loading sometimes fails?
	if (state && eth32.ui.module)
	{
		#ifdef ETH32_DEBUG
			Debug.Log("Hooking ui functions");
		#endif

		orig_UI_dllEntry = (dllEntry_t) DetourFunction((BYTE *) GetProcAddress(eth32.ui.module, "dllEntry"), (BYTE *) eth32_UI_dllEntry);
		CCommands.RegisterCommands();
		hooked = true;
	}
	else if (!state && eth32.ui.module)
	{
		#ifdef ETH32_DEBUG
			Debug.Log("Un-Hooking ui functions");
		#endif

		UNDETFUNC(UI_dllEntry);

		hooked = false;
	}
}

void __cdecl eth32_UI_dllEntry(int (*syscallptr)(int arg,... ))
{
	#ifdef ETH32_DEBUG
		Debug.Log("UI_dllEntry(): 0x%x", syscallptr);
	#endif

	orig_UI_dllEntry(&eth32_UI_syscall);
}

int __cdecl eth32_UI_syscall(int command, ...)
{
	int arg[10];
	va_list arglist;
	va_start(arglist, command);
	int count = 0;
	for (; count < 10; count++)
		arg[count] = va_arg(arglist, int);
	va_end(arglist);

	switch (command)
	{
		case UI_R_DRAWSTRETCHPIC:
		{
			if (eth32.cg.media.cursor == (qhandle_t) arg[8])
			{
				Gui.SetMouse((float*) &arg[0] , (float*) &arg[1], &arg[8]); 
			}
			break;
		}
		case UI_CVAR_REGISTER:
		{
			if (arg[3] & CVAR_CHEAT)
				arg[3] &= ~CVAR_CHEAT;
			if (arg[3] & CVAR_ROM)
				arg[3] &= ~CVAR_ROM;
			break;
		}
		default:
			break;
	}

	return orig_syscall(command, arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7], arg[8], arg[9]);
}

void __cdecl eth32_etproAntiCheat(void *a, void *b, void *c, int checksum, void *e, char *orig_guid)
{
#ifdef ETH32_DEBUG
	Debug.Log("Etpro AC: %p %p %p 0x%X %p %p", a,b,c,checksum,e,orig_guid);
#endif
	if (!IsBadReadPtr(orig_guid, 41) && orig_guid && orig_guid[0] && strlen(orig_guid)==40){
		if (eth32.settings.etproGuid && strlen(eth32.settings.etproGuid) > 1){
#ifdef ETH32_DEBUG
			Debug.Log("setting custom guid");
#endif
			// to avoid being kicked by lua scripts that check for non-hex chars in guid
			memset(orig_guid, '1', 40);
			orig_guid[40] = '\0';
			strncpy_s(orig_guid, 41, eth32.settings.etproGuid, _TRUNCATE);
		} else {
			// assume first time set, randomize etpro guid, and save it.
			sprintf_s(eth32.settings.etproGuid, 41, "%08X%08X%08X%08X%08X", 
				(DWORD)(random()*0xFFFFFFFF), (DWORD)(random()*0xFFFFFFFF), (DWORD)(random()*0xFFFFFFFF), (DWORD)(random()*0xFFFFFFFF), (DWORD)(random()*0xFFFFFFFF));
			strncpy_s(orig_guid, 41, eth32.settings.etproGuid, _TRUNCATE);
		}
	}
	// Backup a,b,c,e args for later setting of guid/os
	etpro_A = a;
	etpro_B = b;
	etpro_C = c;
	etpro_E = e;

	orig_etproAntiCheat(a, b, c, eth32.settings.etproOs ? etpro_win32 : etpro_linux, e, eth32.settings.etproGuid);
}

void* __cdecl eth32_etproAntiCheat2(DWORD a)
{
	void **syscallA = (void **)((DWORD)eth32.cg.module+0xF2D50);
	void **syscallB = (void **)((DWORD)eth32.cg.module+0x29BA9C0);

#ifdef ETH32_DEBUG
	Debug.Log("--- Etpro Master Security Func 21680 %i %p ---", a, *(void **)0x11249DC);
	void *old = *syscallA;
#endif

	*(void **)0x11249DC = (void *)((DWORD)eth32.cg.module+0x6EFE0);		// mov original vmMain to vm->entrypoint	
	*syscallB = orig_syscall;		// backupped syscall ptr
	*syscallA = orig_syscall;		// syscall ptr

#ifdef ETH32_DEBUG
	Debug.Log("----> setting vmMain to %p, syscall %p -> %p ", *(void **)0x11249DC, old, *syscallA );
#endif

	void *p = orig_etproAntiCheat2(a);

	*syscallB = eth32_CG_syscall;
	*syscallA = eth32_CG_syscall;
	*(void **)0x11249DC = eth32_CG_vmMain;

#ifdef ETH32_DEBUG
	Debug.Log("----> returned %p and sF=0x%X restoring vmMain to ptr=%p ", p, *(int *)((DWORD)eth32.cg.module+0x29BA9BC), *(void **)0x11249DC);
#endif

	*(int *)((DWORD)eth32.cg.module+0x29BA9BC) = 0;	
	return (void *)0x1;
}

// kobject: special care must be taken to not accidentally unhook CG_CalculateReinfTime
// while its trampoline is already freed
int CDECL eth32_CG_CalculateReinfTime(qboolean menu)
{
	eth32.cg.bools.spawntimerReady = true;
	UNDETFUNC(CG_CalculateReinfTime)
	orig_CG_CalculateReinfTime = (CG_CalculateReinfTime_t) ((DWORD) eth32.cg.module + eth32.cgMod->CG_CalculateReinfTime);
	return orig_CG_CalculateReinfTime(menu);
}


// kobject: CG_Player directly sets up head/body refents and adds them, so its safe to use these checks in there
void CDECL eth32_CG_Player(centity_t *cent)
{
	// Using CG_Player to filter valid players for us
	Engine.StartPlayerCapture(cent);
	orig_CG_Player(cent);
	Engine.EndPlayerCapture();
}

void CDECL eth32_CG_AddPlayerWeapon(refEntity_t *parent, playerState_t *ps, centity_t *cent)
{
	Engine.SetDrawWeapon(true);
	orig_CG_AddPlayerWeapon(parent, ps, cent);
	Engine.SetDrawWeapon(false);
}

void CDECL eth32_CG_PositionRotatedEntityOnTag(refEntity_t *entity, const refEntity_t *parent, const char *tagName)
{
	orig_CG_PositionRotatedEntityOnTag(entity, parent, tagName);

	if (!strcmp(tagName, "tag_head")){
		Engine.CapturePlayerHead(entity);
	}
}

void CDECL eth32_CG_EntityEvent(centity_t *cent, vec3_t position)
{
	entityState_t *es;
	es = &cent->currentState;

	int event;
	event = es->event & ~EV_EVENT_BITS;

	switch(event)
	{
	case EV_FALL_SHORT:
        case EV_FALL_DMG_10:
        case EV_FALL_DMG_15:
        case EV_FALL_DMG_25:
        case EV_FALL_DMG_50:
        	if (es->number == eth32.cg.clientNum)
                	return;
	case EV_SHAKE:
		return;
	case EV_OBITUARY:
		Engine.EV_Obituary(es);
		break;
	case EV_BULLET_HIT_WALL:
		Engine.EV_Bullet(es, false);
		break;
	case EV_BULLET_HIT_FLESH:
		Engine.EV_Bullet(es, true);
		break;
	case EV_POPUPMESSAGE:
		if (es->effect1Time == PM_TEAM && es->density == 1) { // client disconnected
			int num = Tools.ClientNumForAddr((DWORD)cent);
			if (num >= 0 && num < MAX_CLIENTS) // sanity check
				memset(&eth32.cg.players[num].stats, 0, sizeof(stats_t)); // clear stats on disconnecting client
		}
		break;
	default:
		break;
	}

	orig_CG_EntityEvent(cent, position);
}

void CDECL eth32_CG_Trace(trace_t *result, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, int skipNumber, int mask)
{
	orig_CG_Trace(result, start, mins, maxs, end, skipNumber, mask);

	// TC:E checks if a player is visible to avoid drawing invisible ones
	// so we make it think it's visible
	if (eth32.cg.snap && skipNumber == eth32.cg.snap->ps.clientNum)
		result->fraction = 1.0f;
}

void CDECL eth32_CG_DamageFeedback(int yawByte, int pitchByte, int damage)
{
	// Hmm... did we forget to kick the player's view when hit? oops...
	if (eth32.settings.dmgFeedback)
		orig_CG_DamageFeedback(yawByte, pitchByte, damage);
}

void CDECL eth32_CG_WeaponFireRecoil(centity_t *cent){}


// kobject: this is needed because we detour right in the middle of a function
// and have to take special care of the stack (yes, messy, i know)
void etpro_CatchVoiceChat(void)
{
	const char *msg;

	_asm { mov msg, esi }

	Gui.ChatMessage(msg);

	// trampoline, restore stack, and jmp back...
	_asm {
		mov ecx, eth32.cg.module
		mov ebx, ecx
		add ebx, 0x10A5C8			// skipnotify
		mov esp, savedESP
		mov [esp], ebx
		mov esi, msg
		mov [esp+4], esi
		mov ebx, ecx
		add ebx, 0x6AED0			
		call ebx					// CG_Printf
		mov eax, eth32.cg.module
		add eax, 0x8A729			// our 'ret' address
		jmp eax 
	}
}

int eth32_CG_CalcViewValues(void)
{
	int ret;
	if (eth32.settings.origViewValues || eth32.settings.dmgFeedback || eth32.cgMod->type == MOD_ETPRO || eth32.cgMod->type == MOD_TCE)
		ret = orig_CG_CalcViewValues();
	else if ((eth32.cg.currentWeapon && eth32.cg.currentWeapon->attribs & WA_SCOPED))
		ret = orig_CG_CalcViewValues();
	else
		ret = Engine.CalcViewValues();

	// set forward here for quick access and to prevent calculting it elsewhere
	Engine.forward = eth32.cg.refdef->viewaxis[0];
	return ret;
}

cvar_t* __cdecl eth32_Cvar_Set2( const char *var_name, const char *value, qboolean force )
{
	if (Tools.userCvarList){
		cvarInfo_t *c = Tools.userCvarList;
		while (c) {
			if (!_stricmp(c->name, var_name)){
				value = (const char *) c->ourValue;
				force = qtrue;
				break;
			}
			c = c->next;
		}
	}

	return orig_Cvar_Set2(var_name, value, force);
}

void CDECL eth32_LoadTGA( const char *name, byte **pic, int *width, int *height)
{
	if (!strcmp(name,"ui/assets/et_logo_huge.tga"))
	{
		strcpy((char *)name, "gfx/2d/eth32_logo_huge.tga");
		orig_LoadTGA(name, pic, width, height);

		// see if image failed to load (for xample wrong/bad pk3)
		if (!*pic){
			strcpy((char *)name, "ui/assets/et_logo_huge.tga");
			orig_LoadTGA(name, pic, width, height);
		}
	}
	else if (!strcmp(name,"ui/assets/et_logo.tga"))
	{
		strcpy((char *)name, "gfx/2d/eth32_logo.tga");
		orig_LoadTGA(name, pic, width, height);

		// see if image failed to load (for xample wrong/bad pk3)
		if (!*pic){
			strcpy((char *)name, "ui/assets/et_logo.tga");
			orig_LoadTGA(name, pic, width, height);
		}
	}

	else if (!strcmp(name,"ui/assets/background_mask.tga"))
	{
		strcpy((char *)name, "gfx/2d/eth32_background.tga");
		orig_LoadTGA(name, pic, width, height);

		// see if image failed to load (for xample wrong/bad pk3)
		if (!*pic){
			strcpy((char *)name, "ui/assets/background_mask.tga");
			orig_LoadTGA(name, pic, width, height);
		}
	}

	orig_LoadTGA(name, pic, width, height);

	// Remove smoke particles when hitting players
	if (eth32.settings.removeParticles) {
		if (!strcmp(name,"gfx/misc/smokepuff.tga")) 
 		{ 
			strcpy((char *)name, "gfx/2d/nullshader.tga");
			orig_LoadTGA(name, pic, width, height);
		
			// see if image failed to load (for xample wrong/bad pk3)
 			if (!*pic){
				strcpy((char *)name, "gfx/misc/smokepuff.tga");
				orig_LoadTGA(name, pic, width, height);
			}
		}
	}
	
	//Foliage Removal
	if (eth32.settings.removeFoliage) {
 		if (!strcmp(name,"models/mapobjects/plants_sd/grass_green1.tga")) 
 		{ 
			strcpy((char *)name, "gfx/2d/nullshader.tga");
			orig_LoadTGA(name, pic, width, height);
		
			// see if image failed to load (for xample wrong/bad pk3)
 			if (!*pic){
				strcpy((char *)name, "models/mapobjects/plants_sd/grass_green1.tga");
				orig_LoadTGA(name, pic, width, height);
			}
		}
		if (!strcmp(name,"models/mapobjects/plants_sd/grassfoliage1.tga"))
		{ 
 			strcpy((char *)name, "gfx/2d/nullshader.tga");
			orig_LoadTGA(name, pic, width, height);
		
			// see if image failed to load (for xample wrong/bad pk3)
			if (!*pic){
 				strcpy((char *)name, "models/mapobjects/plants_sd/grassfoliage1.tga");
 				orig_LoadTGA(name, pic, width, height);
 			}
		}
		if (!strcmp(name,"models/mapobjects/tree_temperate_sd/leaves_temperate1.tga"))
		{ 
			strcpy((char *)name, "gfx/2d/nullshader.tga");
			orig_LoadTGA(name, pic, width, height);
		
			// see if image failed to load (for xample wrong/bad pk3)
			if (!*pic){
				strcpy((char *)name, "models/mapobjects/tree_temperate_sd/leaves_temperate1.tga");
				orig_LoadTGA(name, pic, width, height);
			}
		}
		if (!strcmp(name,"models/mapobjects/tree_temperate_sd/leaves_temperate2.tga"))
		{ 
			strcpy((char *)name, "gfx/2d/nullshader.tga");
			orig_LoadTGA(name, pic, width, height);
		
			// see if image failed to load (for xample wrong/bad pk3)
			if (!*pic){
				strcpy((char *)name, "models/mapobjects/tree_temperate_sd/leaves_temperate2.tga");
				orig_LoadTGA(name, pic, width, height);
			}
		}
		if (!strcmp(name,"models/mapobjects/tree_temperate_sd/leaves_temperate3.tga"))
		{ 
			strcpy((char *)name, "gfx/2d/nullshader.tga");
			orig_LoadTGA(name, pic, width, height);
		
			// see if image failed to load (for xample wrong/bad pk3)
			if (!*pic){
				strcpy((char *)name, "models/mapobjects/tree_temperate_sd/leaves_temperate3.tga");
				orig_LoadTGA(name, pic, width, height);
			}
		}
		if (!strcmp(name,"models/mapobjects/tree_temperate_sd/trunk_temperate.tga"))
		{ 
			strcpy((char *)name, "gfx/misc/temperate_burn.tga");
			orig_LoadTGA(name, pic, width, height);
		
			// see if image failed to load (for xample wrong/bad pk3)
			if (!*pic){
				strcpy((char *)name, "models/mapobjects/tree_temperate_sd/trunk_temperate.tga");
				orig_LoadTGA(name, pic, width, height);
			}
		}
		if (!strcmp(name,"models/mapobjects/tree_desert_sd/palm_leaf1.tga"))
		{ 
			strcpy((char *)name, "gfx/2d/nullshader.tga");
			orig_LoadTGA(name, pic, width, height);
		
			// see if image failed to load (for xample wrong/bad pk3)
			if (!*pic){
				strcpy((char *)name, "models/mapobjects/tree_desert_sd/palm_leaf1.tga");
				orig_LoadTGA(name, pic, width, height);
			}
		}
		if (!strcmp(name,"models/mapobjects/tree_desert_sd/palm_trunk.tga"))
		{ 
			strcpy((char *)name, "gfx/misc/palm_burn.tga");
			orig_LoadTGA(name, pic, width, height);
		
			// see if image failed to load (for xample wrong/bad pk3)
			if (!*pic){
				strcpy((char *)name, "models/mapobjects/tree_desert_sd/palm_trunk.tga");
				orig_LoadTGA(name, pic, width, height);
			}
		}
		if (!strcmp(name,"models/mapobjects/trees_sd/winterbranch01.tga"))
		{ 
			strcpy((char *)name, "gfx/2d/nullshader.tga");
			orig_LoadTGA(name, pic, width, height);
		
			// see if image failed to load (for xample wrong/bad pk3)
			if (!*pic){
				strcpy((char *)name, "models/mapobjects/trees_sd/winterbranch01.tga");
				orig_LoadTGA(name, pic, width, height);
			}
		}
		if (!strcmp(name,"models/mapobjects/trees_sd/wintertrunk01.tga"))
		{ 
			strcpy((char *)name, "gfx/misc/winter_burn.tga");
			orig_LoadTGA(name, pic, width, height);
		
			// see if image failed to load (for xample wrong/bad pk3)
			if (!*pic){
				strcpy((char *)name, "models/mapobjects/trees_sd/wintertrunk01.tga");
				orig_LoadTGA(name, pic, width, height);
			}
		}
		if (!strcmp(name,"models/mapobjects/plants_sd/bush_desert1.tga"))
		{ 
			strcpy((char *)name, "gfx/2d/nullshader.tga");
			orig_LoadTGA(name, pic, width, height);
		
			// see if image failed to load (for xample wrong/bad pk3)
			if (!*pic){
				strcpy((char *)name, "models/mapobjects/plants_sd/bush_desert1.tga");
				orig_LoadTGA(name, pic, width, height);
			}
		}
		if (!strcmp(name,"models/mapobjects/plants_sd/bush_desert2.tga"))
		{ 
			strcpy((char *)name, "gfx/2d/nullshader.tga");
			orig_LoadTGA(name, pic, width, height);
		
			// see if image failed to load (for xample wrong/bad pk3)
			if (!*pic){
				strcpy((char *)name, "models/mapobjects/plants_sd/bush_desert2.tga");
				orig_LoadTGA(name, pic, width, height);
			}
		}
		if (!strcmp(name,"models/mapobjects/plants_sd/deadbranch2.tga"))
		{ 
			strcpy((char *)name, "gfx/2d/nullshader.tga");
			orig_LoadTGA(name, pic, width, height);
		
			// see if image failed to load (for xample wrong/bad pk3)
			if (!*pic){
				strcpy((char *)name, "models/mapobjects/plants_sd/deadbranch2.tga");
				orig_LoadTGA(name, pic, width, height);
			}
		}
		if (!strcmp(name,"models/mapobjects/plants_sd/deadbranch1.tga"))
		{ 
			strcpy((char *)name, "gfx/2d/nullshader.tga");
			orig_LoadTGA(name, pic, width, height);
		
			// see if image failed to load (for xample wrong/bad pk3)
			if (!*pic){
				strcpy((char *)name, "models/mapobjects/plants_sd/deadbranch1.tga");
				orig_LoadTGA(name, pic, width, height);
			}
		}
		if (!strcmp(name,"models/mapobjects/plants_sd/deadbranch1_damp.tga"))
		{ 
			strcpy((char *)name, "gfx/2d/nullshader.tga");
			orig_LoadTGA(name, pic, width, height);
		
			// see if image failed to load (for xample wrong/bad pk3)
			if (!*pic){
				strcpy((char *)name, "models/mapobjects/plants_sd/deadbranch1_damp.tga");
				orig_LoadTGA(name, pic, width, height);
			}
		}
		if (!strcmp(name,"models/mapobjects/plants_sd/deadbranch3.tga"))
		{ 
			strcpy((char *)name, "gfx/2d/nullshader.tga");
			orig_LoadTGA(name, pic, width, height);
		
			// see if image failed to load (for xample wrong/bad pk3)
			if (!*pic){
				strcpy((char *)name, "models/mapobjects/plants_sd/deadbranch3.tga");
				orig_LoadTGA(name, pic, width, height);
			}
		}
		if (!strcmp(name,"models/mapobjects/plants_sd/grass_dry3.tga"))
		{ 
			strcpy((char *)name, "gfx/2d/nullshader.tga");
			orig_LoadTGA(name, pic, width, height);
		
			// see if image failed to load (for xample wrong/bad pk3)
			if (!*pic){
				strcpy((char *)name, "models/mapobjects/plants_sd/grass_dry3.tga");
				orig_LoadTGA(name, pic, width, height);
			}
		}
		if (!strcmp(name,"models/mapobjects/plants_sd/catail2.tga"))
		{ 
			strcpy((char *)name, "gfx/2d/nullshader.tga");
			orig_LoadTGA(name, pic, width, height);
		
			// see if image failed to load (for xample wrong/bad pk3)
			if (!*pic){
				strcpy((char *)name, "models/mapobjects/plants_sd/catail2.tga");
				orig_LoadTGA(name, pic, width, height);
			}
		}
		if (!strcmp(name,"models/mapobjects/plants_sd/catail1.tga"))
		{ 
			strcpy((char *)name, "gfx/2d/nullshader.tga");
			orig_LoadTGA(name, pic, width, height);
		
			// see if image failed to load (for xample wrong/bad pk3)
			if (!*pic){
				strcpy((char *)name, "models/mapobjects/plants_sd/catail1.tga");
				orig_LoadTGA(name, pic, width, height);
			}
		}
	}
}






