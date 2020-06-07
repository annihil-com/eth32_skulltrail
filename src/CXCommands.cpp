// ETH32 - an Enemy Territory cheat for windows
// Copyright (c) 2007 eth32 team
// www.cheatersutopia.com & www.nixcoders.org

// kobject: the idea is to put our own console commands in here, and 
// leave hooked engine/cgame commands in CEngine


// Member functions are from Mars, ordinary functions are from Venus.

#include "eth32.h"
#include "settings.h"

CXCommands CCommands;
extern void *etpro_A;
extern void *etpro_B;
extern void *etpro_C;
extern void *etpro_E;
extern DWORD etpro_win32;
extern DWORD etpro_linux;


void xcmd_Tracker()
{
	Portal.ConsoleCommand();
}

void xcmd_Namesteal()
{
	static int old = eth32.settings.NamestealDelay;
	eth32.settings.NamestealDelay = 0;
	Syscall.UI_ExecuteText("eth_set namesteal on; wait 100; eth_set namesteal off\n");
	eth32.settings.NamestealDelay = old;
}

void loadCustomHitbox()
{
	if (Syscall.UI_Argc() == 1){
		Syscall.UI_Print("^dSyntax: ^o/command ^2[^9list^2/^9stand^2] ^1x^9,^1y^9,^1z\n");
		return;
	}

	char *cmd = strdup(Syscall.UI_Argv(1));
	char *newValue;
	vec3_t vec;

	if (!strcmp(cmd,"list")){
		hitbox_t *hbox;
		if( eth32.settings.hitboxType == HITBOX_CUSTOM )
			hbox = &Aimbot.customHitbox;
		else
			hbox = &head_hitboxes[eth32.settings.hitboxType];

		Syscall.UI_Print("^dstand          ^9{^2% .1f, % .1f, % .1f^9}\n",
			hbox->stand_offset[0], hbox->stand_offset[1], hbox->stand_offset[2]);
		Syscall.UI_Print("^drun            ^9{^2% .1f, % .1f, % .1f^9}\n",
			hbox->stand_offset_moving[0], hbox->stand_offset_moving[1], hbox->stand_offset_moving[2]);
		Syscall.UI_Print("^dcrouch         ^9{^2% .1f, % .1f, % .1f^9}\n",
			hbox->crouch_offset[0], hbox->crouch_offset[1], hbox->crouch_offset[2]);
		Syscall.UI_Print("^dcrawl          ^9{^2% .1f, % .1f, % .1f^9}\n",
			hbox->crouch_offset_moving[0], hbox->crouch_offset_moving[1], hbox->crouch_offset_moving[2]);
		Syscall.UI_Print("^dprone          ^9{^2% .1f, % .1f, % .1f^9}\n",
			hbox->prone_offset[0], hbox->prone_offset[1], hbox->prone_offset[2]);
		Syscall.UI_Print("^dsize           ^9{^2% .1f, % .1f, % .1f^9}\n",
			hbox->size[0], hbox->size[1], hbox->size[2]);

		free(cmd);
		return;
	}

	if (Syscall.UI_Argc() != 3 ){
		Syscall.UI_Print("^1Error: ^oinsufficient arguments\n");
		free(cmd);
		return;
	} else
		newValue = strdup(Syscall.UI_Argv(2));

	VectorCopy(vec3_origin, vec);
	sscanf(newValue,"%f,%f,%f", &vec[0], &vec[1], &vec[2]);

	if (!strcmp(cmd, "stand")) {
		VectorCopy(vec,Aimbot.customHitbox.stand_offset);
	} else if (!strcmp(cmd, "run")) {
		VectorCopy(vec,Aimbot.customHitbox.stand_offset_moving);
	} else if (!strcmp(cmd, "crouch")) {
		VectorCopy(vec,Aimbot.customHitbox.crouch_offset);
	} else if (!strcmp(cmd, "crawl")) {
		VectorCopy(vec,Aimbot.customHitbox.crouch_offset_moving);
	} else if (!strcmp(cmd, "prone")) {
		VectorCopy(vec,Aimbot.customHitbox.prone_offset);
	} else if (!strcmp(cmd, "size")) {
		VectorCopy(vec,Aimbot.customHitbox.size);
	} else
		Syscall.UI_Print("^1Error: ^ounknown command\n");

	free(cmd);
	free(newValue);
}

void xcmd_RandName()
{
	if (!Tools.nNames) {
		Syscall.UI_Print("^3No player names loaded\n");
		return;
	}

	int n = (int)(random()*Tools.nNames);
	Syscall.UI_ExecuteText("set name \"%s\"\n", Tools.nameList[n]); 
}

void xcmd_RandGuid()
{
	char buf[1024];
	sprintf(buf, "%08X%08X%08X%08X%08X", 
		(DWORD)(random()*0xFFFFFFFF), (DWORD)(random()*0xFFFFFFFF), (DWORD)(random()*0xFFFFFFFF), (DWORD)(random()*0xFFFFFFFF), (DWORD)(random()*0xFFFFFFFF));
	strncpy(eth32.settings.etproGuid, buf, 1024);
	Syscall.UI_Print("^dNew etpro guid: ^7%s\n^dSetting won't take effect until next map (or do /reconnect)!\n", eth32.settings.etproGuid);
}

void xcmd_RandMac()
{
	char buf[16];
	sprintf(buf, "%02x-%02x-%02x-%02x",  rand() % 256,  rand() % 256,  rand() % 256,  rand() % 256);
	strncpy(eth32.settings.jayMac, buf, 16);
	Syscall.UI_Print("^dNew Jaymac: ^7%s\n^dSetting won't take effect until next map (or do /reconnect)!\n", eth32.settings.jayMac);
}

void xcmd_unbanCommand()
{
	system("rm -rf ~/.etwolf/etmain/etkey");
	Syscall.UI_ExecuteText("pb_cdkeyreg; wait 50\n");
	Syscall.UI_ExecuteText("eth_randguid; eth_randmac; wait 50; eth_randname; reconnect\n");
}


void xcmd_HackSpam()
{	// sol: added simple spam, feel free to modify however you like
	Gui.BufferConsoleCommand("say \"^0[ ^oETH^d32 ^o" ETH32_VERSION " ^d- ^oBy ^dAimbots^o.^dNet ^o& ^dRabbnix ^0]\n\"");
}

void xcmd_MusicSpam()
{
	if (!Winamp.WinampInstance()){
		Syscall.UI_Print("^1Error: ^owinamp not running!\n");
		return;
	}

	Winamp.MakeSpam(true);
}

void xcmd_UpdateMusicSpamFmt()
{
	if (Syscall.UI_Argc() == 1){
		Syscall.UI_Print("^dSyntax:          ^o/eth_musicspamfmt ^2[^9\"format\"^2]\n\n");
		Syscall.UI_Print("^dSwitches:        ^2[^9w^2]  ^owhole unformatted title\n");
		Syscall.UI_Print("                 ^2[^9t^2]  ^osong title\n");
		Syscall.UI_Print("                 ^2[^9a^2]  ^oartist\n");
		Syscall.UI_Print("                 ^2[^9b^2]  ^obitrate\n");
		Syscall.UI_Print("                 ^2[^9c^2]  ^ochannels\n");
		Syscall.UI_Print("                 ^2[^9s^2]  ^osample rate\n");
		Syscall.UI_Print("                 ^2[^9l^2]  ^otrack length (in msec)\n");
		Syscall.UI_Print("                 ^2[^9L^2]  ^oformatted track length (mm:ss)\n");
		Syscall.UI_Print("                 ^2[^9p^2]  ^ocurrent position (in msec)\n");
		Syscall.UI_Print("                 ^2[^9P^2]  ^oformatted current position (mm:ss)\n\n");
		Syscall.UI_Print("^dCurrent Format:  ^7\"%s^7\"\n", Winamp.GetFormatString());
		return;
	}

	Winamp.UpdateSpamFormat((char *)Syscall.UI_Argv(1));
	Syscall.UI_Print("^o*** ^dMusic spam updated.\n");
}

void goHome()
{
	Syscall.UI_ExecuteText("connect 84.244.186.26:27960\n");
}

void xcmd_Crash()
{
	_asm 
	{
		mov eax, 0x123
		mov [eax], 1
	}
}

void formatSetting(char *out, size_t o_s, const settingdef_t *setting)
{
	switch (setting->type) {
		case SETTING_INT:
			sprintf_s(out, o_s, "^9%i", *(int *)setting->target);
			break;
		case SETTING_FLOAT:
			sprintf_s(out, o_s, "^9%.3f", *(float *)setting->target);
			break;
		case SETTING_BOOL:
			(*(bool *)setting->target) ? strcpy_s(out, o_s, "^2true") : strcpy_s(out, o_s, "^1false");
			break;
		case SETTING_VEC3:
		{
			float *v = (float*)setting->target;
			sprintf_s(out, o_s, "^2{^9%.3f^2, ^9%.3f^2, ^9%.3f^2}", v[0], v[1], v[2]);
			break;
		}
		case SETTING_VEC4:
		{
			float *v = (float*)setting->target;
			sprintf_s(out, o_s, "^2{^9%.3f^2, ^9%.3f^2, ^9%.3f^2, ^9%.3f^2}", v[0], v[1], v[2], v[3]);
			break;
		}
		case SETTING_BYTE3:
		{
			BYTE *b = (BYTE*)setting->target;
			sprintf_s(out, o_s, "^9%3i %3i %3i", b[0], b[1], b[2]);
			break;
		}
		case SETTING_STRING:
		{
			sprintf_s(out, o_s, "^o%s", (char *)setting->target);
			break;
		}
		default:
			strcpy_s(out, o_s, "^3unknown type");
			break;
	}
}

// handles main settings only for now
void xcmd_Settings()
{
	Settings.ConsoleCommand();
}

void xcmd_Toggle()
{
	Settings.Toggle();
}

void xcmd_CvarForce()
{
	if (Syscall.UI_Argc() == 1){
		Syscall.UI_Print("^dCvar Force List\n");
		cvarInfo_t *c = Tools.userCvarList;
		int i = 0; 
		while (c) {
			Syscall.UI_Print("^3%03i  ^2%16s   ^9\"^o%s^9\"\n",i, c->name, c->ourValue);
			c = c->next;
			i++;
		}

		return;
	} else if (Syscall.UI_Argc() == 2){
		const char *s = Syscall.UI_Argv(1);
		cvarInfo_t *c = Tools.userCvarList;
		while (c) {
			if (!_stricmp(s, c->name))
				Syscall.UI_Print("^2%16s   ^9\"^o%s^9\"\n", c->name, c->ourValue);
			c = c->next;
		}
		return;
	}

	char *cvarName = _strdup(Syscall.UI_Argv(1));
	char *val = _strdup(Syscall.UI_Argv(2));

	// delete a cvar (( FIX the 0th cvar)
	if (!_stricmp(cvarName, "del")){
		int num = atoi(val);
		if (num > 0){
			if (!IsBadReadPtr(&Tools.userCvarList[num], sizeof(cvarInfo_t))){
				if (Tools.userCvarList[num].next)
					Tools.userCvarList[num-1].next = &Tools.userCvarList[num+1];
				else
					Tools.userCvarList[num-1].next = NULL;

				Syscall.UI_Print("^dDeleted ^2%s\n", Tools.userCvarList[num].name);
			}
		}		
	}

	// see if we should update an already present cvar
	bool found = false;
	cvarInfo_t *c = Tools.userCvarList;
	while (c) {
		if (!_stricmp(cvarName, c->name)){
			strncpy_s(c->ourValue, MAX_STRING_CHARS, val, _TRUNCATE);
			Syscall.UI_Print("^dUpdated:\n^2%16s   ^9\"^o%s^9\"\n", c->name, c->ourValue);
			found = true;
			break;
		}				
		c = c->next;
	}	

	if (!found && _stricmp(cvarName, "del")){
		Syscall.UI_Print("^dAdded new entry:\n^2%16s   ^9\"^o%s^9\"\n", cvarName, val);
		Tools.AddUserCvar(cvarName, val);
	}

	Tools.SaveUserCvars();

	free(val);
	free(cvarName);
}

void xcmd_setEtproGuid()
{
	if (Syscall.UI_Argc() == 1)
	{
		if (!eth32.settings.etproGuid || strlen(eth32.settings.etproGuid) < 2)
			Syscall.UI_Print("^dEtpro Guid: ^2original");
		else
			Syscall.UI_Print("^dEtpro Guid: ^7%s", eth32.settings.etproGuid);
		return;
	}

	strncpy_s(eth32.settings.etproGuid, sizeof(eth32.settings.etproGuid), Syscall.UI_Argv(1), _TRUNCATE);
	Syscall.UI_Print("^dNew etpro guid: ^7%s\n^dSetting won't take effect until next map (or do /reconnect)!", eth32.settings.etproGuid);
}

void xcmd_specstats()
{
	if (!eth32.settings.getSpeclist) {
		Syscall.UI_Print("^dspeclisting not activated\n");
		return;
	}

	if (!eth32.cg.hooked){
		Syscall.UI_Print("^dNot connected\n");
		return;
	}

	int bs, rq;
	char server[64];
	Spectator.GetStats(&bs, &rq, server);
	Syscall.UI_Print("^dServer: ^2%s ^dBytes rec: ^2%i ^dReq: ^2%i\n", bs, rq, server);
}


void xmcd_ircMsg()
{
	Gui.SetChatTarget(4); // irc msg type == 4
}

void xcmd_replyMsg()
{
	if (!eth32.cg.replyPlayer[0]) {
		Gui.CenterPrint("^1Error: ^7No target for reply!", 1);
		return;
	}

	Gui.ReplyMessage();
}

void xcmd_AimKeyDown()
{
	Aimbot.SetAimkeyStatus(true);
}

void xcmd_AimKeyUp()
{
	Aimbot.SetAimkeyStatus(false);
}


void setEtproOS()
{
	if (Syscall.UI_Argc() == 1){
		Syscall.UI_Print("^2%s", eth32.settings.etproOs ? "win" : "linux");
		return;
	}

	char *cmd = _strdup(Syscall.UI_Argv(1));

	if (!strcmp(cmd, "win"))
		eth32.settings.etproOs = true;
	else
		eth32.settings.etproOs = false;

	Syscall.UI_Print("^2new etpro OS set to %s", eth32.settings.etproOs ? "win" : "linux");
	free(cmd);

	if (eth32.cgMod->type == MOD_ETPRO)
		orig_etproAntiCheat(etpro_A, etpro_B, etpro_C, eth32.settings.etproOs ? etpro_win32 : etpro_linux, etpro_E, eth32.settings.etproGuid);
}

void xcmd_savesettings()
{
	if (Syscall.UI_Argc() == 1){
		Syscall.UI_Print("^2must specify a file\n");
		return;
	}
	
	char file[256];
	sprintf(file, "%s/%s", eth32.path, Syscall.UI_Argv(1));
	
	if (Settings.SaveUserSettings((const char*)file))
		Syscall.UI_Print("^2done.\n");
	else
		Syscall.UI_Print("^1error while saving to %s\n", Syscall.UI_Argv(1));
}

void xcmd_killspam()
{	
	if (Syscall.UI_Argc() == 1){
		if (Tools.TotalKillspam() == 0) {
			Syscall.UI_Print("^dno kill spam loaded\n");
			return;
		}
		Syscall.UI_Print("^2%i killspam definitions loaded\n", Tools.TotalKillspam());
		Syscall.UI_Print("^9--------------------------------\n");
		for (int i=0; i<Tools.TotalKillspam(); i++)
			Syscall.UI_Print("^3%0.2i    %s\n", i, Tools.GetKillspam(i));
		
		return;
	}
	
	if (!strcmp(Syscall.UI_Argv(1), "clear")) {
		Tools.ClearKillspam();
		return;
	}
	
	Tools.AddKillspam((char*)Syscall.UI_Argv(1));
	Syscall.UI_Print("^2added %s\n", Syscall.UI_Argv(1));
}

void xcmd_multikillspam()
{	
	if (Syscall.UI_Argc() == 1){
		if (Tools.TotalMultikillspam() == 0) {
			Syscall.UI_Print("^dno multikill spam loaded\n");
			return;
		}
		Syscall.UI_Print("^2%i multikillspam definitions loaded\n", Tools.TotalMultikillspam());
		Syscall.UI_Print("^9--------------------------------\n");
		for (int i=0; i<Tools.TotalMultikillspam(); i++)
			Syscall.UI_Print("^3%0.2i    %s\n", i, Tools.GetMultikillspam(i));
		
		return;
	}

	if (!strcmp(Syscall.UI_Argv(1), "clear")) {
		Tools.ClearMultikillspam();
		return;
	}	
	
	Tools.AddMultikillspam((char*)Syscall.UI_Argv(1));
	Syscall.UI_Print("^2added %s\n", Syscall.UI_Argv(1));
}

void xcmd_selfkillspam()
{	
	if (Syscall.UI_Argc() == 1){
		if (Tools.TotalSelfkillspam() == 0) {
			Syscall.UI_Print("^dno kill spam loaded\n");
			return;
		}
		Syscall.UI_Print("^2%i killspam definitions loaded\n", Tools.TotalSelfkillspam());
		Syscall.UI_Print("^9--------------------------------\n");
		for (int i=0; i<Tools.TotalSelfkillspam(); i++)
			Syscall.UI_Print("^3%0.2i    %s\n", i, Tools.GetSelfkillspam(i));
		
		return;
	}
	
	if (!strcmp(Syscall.UI_Argv(1), "clear")) {
		Tools.ClearSelfkillspam();
		return;
	}	
	
	Tools.AddSelfkillspam((char*)Syscall.UI_Argv(1));
	Syscall.UI_Print("^2added %s\n", Syscall.UI_Argv(1));	
}

void xcmd_suicidespam()
{	
	if (Syscall.UI_Argc() == 1){
		if (Tools.TotalSuicidespam() == 0) {
			Syscall.UI_Print("^dno suicide spam loaded\n");
			return;
		}
		Syscall.UI_Print("^2%i suicide spam definitions loaded\n", Tools.TotalSuicidespam());
		Syscall.UI_Print("^9--------------------------------\n");
		for (int i=0; i<Tools.TotalSuicidespam(); i++)
			Syscall.UI_Print("^3%0.2i    %s\n", i, Tools.GetSuicidespam(i));
		
		return;
	}
	
	if (!strcmp(Syscall.UI_Argv(1), "clear")) {
		Tools.ClearSuicidespam();
		return;
	}	
	
	Tools.AddSuicidespam((char*)Syscall.UI_Argv(1));
	Syscall.UI_Print("^2added %s\n", Syscall.UI_Argv(1));
}

void xcmd_deathspam()
{	
	if (Syscall.UI_Argc() == 1){
		if (Tools.TotalDeathspam() == 0) {
			Syscall.UI_Print("^dno death spam loaded\n");
			return;
		}
		Syscall.UI_Print("^2%i deathspam definitions loaded\n", Tools.TotalDeathspam());
		Syscall.UI_Print("^9--------------------------------\n");
		for (int i=0; i<Tools.TotalDeathspam(); i++)
			Syscall.UI_Print("^3%0.2i    %s\n", i, Tools.GetDeathspam(i));
		
		return;
	}
	
	if (!strcmp(Syscall.UI_Argv(1), "clear")) {
		Tools.ClearDeathspam();
		return;
	}	
	
	Tools.AddDeathspam((char*)Syscall.UI_Argv(1));
	Syscall.UI_Print("^2added %s\n", Syscall.UI_Argv(1));	
}

void xcmd_spamCommand() {
       if (Syscall.UI_Argc() != 1) {
	       orig_syscall(UI_PRINT, "^nSpam with 'eth32 skulltrail'\n");
           orig_syscall(UI_PRINT, "^nUsage: eth_spam\n");
           return;
        }
        
       orig_syscall(UI_CMD_EXECUTETEXT, EXEC_APPEND, "say \"^d _____^9 _   _    ^d _____^9____\"\n");
	   orig_syscall(UI_CMD_EXECUTETEXT, EXEC_APPEND, "say \"^d| ____^9| |_| |__ ^d|___ /^9___ \   ^dS^9kull^dT^9rail by\"\n");
	   orig_syscall(UI_CMD_EXECUTETEXT, EXEC_APPEND, "say \"^d|  _| ^9| __| '_ \^d  |_ \ ^9__) |  ^dn^9ix^dC^9oders^d.^9org\"\n");
       orig_syscall(UI_CMD_EXECUTETEXT, EXEC_APPEND, "say \"^d| |___^9| |_| | | |^d___) /^9 __/     and\"\n");
       orig_syscall(UI_CMD_EXECUTETEXT, EXEC_APPEND, "say \"^d|_____|^9\__|_| |_|^d____/^9_____|  ^Da^9im^db^9ots^d.^9net\"\n");

   } 

CXCommands::CXCommands(void)
{
	cmdsRegistered = false;
}

void CXCommands::RegisterCommands()
{
	if (cmdsRegistered || !orig_Cmd_AddCommand)
		return;
	orig_Cmd_AddCommand("eth_vec", &loadCustomHitbox);
	orig_Cmd_AddCommand("eth_home", &goHome);
	orig_Cmd_AddCommand("eth_musicspam", &xcmd_MusicSpam);
	orig_Cmd_AddCommand("eth_musicspamfmt", &xcmd_UpdateMusicSpamFmt);
	orig_Cmd_AddCommand("eth_spam", &xcmd_spamCommand);
	orig_Cmd_AddCommand("eth_hackspam", &xcmd_HackSpam);
	orig_Cmd_AddCommand("eth_crashme", &xcmd_Crash);
	orig_Cmd_AddCommand("eth_cvar", &xcmd_CvarForce);
	orig_Cmd_AddCommand(va("%s",eth32.settings.cvarPrefix), &xcmd_Settings);
	orig_Cmd_AddCommand("eth_etproguid", &xcmd_setEtproGuid);
	orig_Cmd_AddCommand("eth_ircmsg", &xmcd_ircMsg);
	orig_Cmd_AddCommand("eth_tracker", &xcmd_Tracker);
	orig_Cmd_AddCommand("eth_set", &xcmd_Settings);
	orig_Cmd_AddCommand("eth_toggle", &xcmd_Toggle);
	orig_Cmd_AddCommand("eth_unban", &xcmd_unbanCommand);
	orig_Cmd_AddCommand("eth_randguid", &xcmd_RandGuid);
	orig_Cmd_AddCommand("eth_randmac", &xcmd_RandMac);
	orig_Cmd_AddCommand("eth_reply", &xcmd_replyMsg);

	orig_Cmd_AddCommand("eth_save", &xcmd_savesettings);

	// spamz
	orig_Cmd_AddCommand("eth_killspam", &xcmd_killspam);
	orig_Cmd_AddCommand("eth_selfkillspam", &xcmd_selfkillspam);
	orig_Cmd_AddCommand("eth_namesteal", &xcmd_Namesteal);
	orig_Cmd_AddCommand("eth_randname", &xcmd_RandName);
	orig_Cmd_AddCommand("eth_suicidespam", &xcmd_suicidespam);
	orig_Cmd_AddCommand("eth_deathspam", &xcmd_deathspam);
	orig_Cmd_AddCommand("eth_multikillspam", &xcmd_multikillspam);

	orig_Cmd_AddCommand("eth_specstats", &xcmd_specstats);

	orig_Cmd_AddCommand("+aim", &xcmd_AimKeyDown);
	orig_Cmd_AddCommand("-aim", &xcmd_AimKeyUp);
	orig_Cmd_AddCommand("eth_etproos", &setEtproOS);

	cmdsRegistered = true;
}
