// ETH32 - an Enemy Territory cheat for windows
// Copyright (c) 2007 eth32 team
// www.cheatersutopia.com & www.nixcoders.org

#include "../eth32.h"
#include "CWinamp.h"

/* from winamp 5.32beta SDK */
#include "wa_ipc.h"
#include "ipc_pe.h"
#include "WINAMPCMD.H"

#define BOUND(x,x0,x1) ((x>x0) && (x<x1))
#define ISINT(x) ((x > 0x2f) && (x < 0x3a))

CWinamp Winamp;

CWinamp::CWinamp(void)
{
	hwndWA = hwndPE = NULL;
	winAmpInstance = false;
	_cs = new CRITICAL_SECTION;
	InitializeCriticalSection(_cs);
	this->winampThread = NULL;
	this->wantExit = false;
	this->ctrlUpdInterval = 5000;
	this->wantTitle = false;

	WAState.length = 0;
	this->ReqWAVolume = 0;
	this->ReqWAPosition = 0;
	this->songInfoStatus = WA_SONGINFO_FAIL;

	this->oldTrackDuration = -1;
	spamFormatStr[0] = '\0';
}

CWinamp::~CWinamp(void)
{
	this->KillThread();
	TerminateThread( this->winampThread, 0 );
	DeleteCriticalSection(_cs);
	delete _cs;
}

// returns WA_UNKNOWN_TRACK_LENGTH on error
int  CWinamp::GetTrackLength() { return winampMessage(1, IPC_GETOUTPUTTIME); }
// returns WA_NOT_PLAYING if no track is playing
int  CWinamp::GetTrackPosition() { return winampMessage(0, IPC_GETOUTPUTTIME); }
// returns WA_SEEK_OK, WA_SEEK_EOS (end of song), or WA_NOT_PLAYING
int  CWinamp::SeekTrack(int msec) { return winampMessage(msec,IPC_JUMPTOTIME); }
int  CWinamp::GetStatus(){ return winampMessage(0,IPC_ISPLAYING); }
int  CWinamp::GetEQData(int band) { return winampMessage(band, IPC_GETEQDATA); }
void CWinamp::GetEQData(){ this->GetEQData(&this->eq); }
int  CWinamp::GetPlaylistLength() { return winampMessage(0,IPC_GETLISTLENGTH); }
int  CWinamp::GetPlaylistPosition() { return winampMessage(0,IPC_GETLISTPOS); }
void CWinamp::Stop() { winampCommand(WINAMP_BUTTON4); }
void CWinamp::PlayNext(){ winampCommand(WINAMP_BUTTON5); }
void CWinamp::PlayPrev(){ winampCommand(WINAMP_BUTTON1); }
void CWinamp::SetSpamFormat(const char *format){ strncpy_s(this->spamFormatStr, sizeof(this->spamFormatStr), format, _TRUNCATE); }
int  CWinamp::winampMessage(WPARAM param, LPARAM cmd){ return (int) SendMessageA(hwndWA, WM_WA_IPC, param, cmd); }
int  CWinamp::winampCommand(WPARAM command){ return (int) SendMessageA(hwndWA, WM_COMMAND, command, 0); }
int  CWinamp::playlistMessage(WPARAM param, LPARAM cmd){ return (int) SendMessageA(hwndPE, WM_WA_IPC, param, cmd); }
bool CWinamp::WinampInstance(){ return winAmpInstance; }
void CWinamp::UpdateSpamFormat( char *newFmt ){ strncpy_s(this->spamFormatStr, sizeof(this->spamFormatStr), newFmt, _TRUNCATE); }
int  CWinamp::CtrlUpdInterval(){ return this->ctrlUpdInterval; }
bool CWinamp::WantExit() { return this->wantExit; }
bool CWinamp::WantTitle() { return this->wantTitle; }
void CWinamp::SetTitle(bool enabled){ this->wantTitle = enabled; }
void CWinamp::UpdateControls(){ this->GetWinampState(); }
void CWinamp::CopyTitle( char *oTitle, size_t _s_t ){ strncpy_s( oTitle, _s_t, this->currentTrack.Title, _TRUNCATE ); }
int  CWinamp::GetVolume() { return winampMessage(-666, IPC_SETVOLUME); }
void CWinamp::ReqKillThread() { this->wantExit = true; }

int CWinamp::PlayPosition(int pos)
{
	winampMessage(pos,IPC_SETPLAYLISTPOS);
	SendMessageA(hwndWA,WM_COMMAND,MAKEWPARAM(WINAMP_BUTTON2,0),0);
	return 0;
}

void CWinamp::GetEQData(eqData_t *e)
{
	int i;
	for(i=0;i<10;i++)
		e->bandValue[i] = winampMessage(i, IPC_GETEQDATA);
	e->preamp = winampMessage(10, IPC_GETEQDATA);
	e->enabled = winampMessage(11, IPC_GETEQDATA);
	e->autoload = winampMessage(12, IPC_GETEQDATA);
}

void CWinamp::SetEQData(eqData_t *e)
{
	int i;
	for(i=0;i<10;i++)
		this->SetEQBand(i, e->bandValue[i]);
}

// 2.05+
void CWinamp::SetEQBand(int band, int val)
{
	if (band < 0 || band > 9 || val < 0 || val > 63)
		return;

	if (this->winampVersion <0x2092){
		winampMessage(band, IPC_GETEQDATA);
		winampMessage(val, IPC_SETEQDATA);
	} else {
		int b = (0xDB << 3) + (band << 2) + val;
		winampMessage(b, IPC_SETEQDATA);
	}
}

void CWinamp::SetVolume(int volume)
{
	if (volume < 0 || volume > 255)
		return;
	winampMessage(volume, IPC_SETVOLUME);
}

void CWinamp::FindInstance()
{
	hwndWA = FindWindow("Winamp v1.x",NULL);

	if (hwndWA)
		winampVersion = winampMessage(0, IPC_GETVERSION);
	else {
		this->winAmpInstance = false;
		return;
	}

	if (winampVersion < 0x2005){
		// < 2.05 is not supported
		this->winAmpInstance = false;
		return;
	}

	hwndPE = getPlaylistWnd();
	if (!hwndPE){
		this->winAmpInstance = false;
		return;
	}

	// get initial pause status	
	int len = GetWindowText(hwndWA, hwndTitle, sizeof(hwndTitle));
	if (!len){
		this->winAmpInstance = false;
		return;
	}

	WAState.paused = (this->GetStatus() == WA_PAUSED);
	this->GetWinampState();
	this->ReqWAVolume = WAState.volume;
	this->winAmpInstance = true;
}

// force play (no track restart), resume from pause
void CWinamp::Play()
{
	if (!winampMessage(0,IPC_ISPLAYING))
		winampMessage(0,IPC_STARTPLAY);
	else if( paused ){
		paused = false;
		winampCommand(WINAMP_BUTTON2+1);
	}
}

// force restart
void CWinamp::RestartTrack()
{
	winampCommand(WINAMP_BUTTON4);	//stop
	winampCommand(WINAMP_BUTTON2);	//play
}

// only pauses
void CWinamp::Pause()
{
	if (!paused){
		paused = true;
		winampCommand(WINAMP_BUTTON2+1);	
	}
}

__forceinline int pow10(int l)
{
	if (l==0)
		return 1;
	else
		return 10*pow10(l-1);
}

int CWinamp::GetTitle()
{
	EnterCriticalSection(_cs);
	currentTrack.artist[0] = currentTrack.songtitle[0] = currentTrack.Title[0] = '\0';
	int len = GetWindowText(hwndWA, hwndTitle, sizeof(hwndTitle));
	if (!len){
		this->winAmpInstance = false;
		songInfoStatus = WA_SONGINFO_FAIL;
		LeaveCriticalSection(_cs);
		return WA_SONGINFO_FAIL;
	}

	songInfoStatus = WA_SONGINFO_TITLE;
	char *p;

	p = strstr( hwndTitle, "- Winamp" );
	if (!p){
		p = strstr( hwndTitle, "Winamp" );
		if (!p){
			strncpy_s(currentTrack.Title, sizeof(currentTrack.Title), hwndTitle, _TRUNCATE);
			songInfoStatus = WA_SONGINFO_TITLE;
			LeaveCriticalSection(_cs);
			return WA_SONGINFO_TITLE;
		}
	}
	
	if(BOUND(p-hwndTitle, 0, sizeof(currentTrack.Title)))
		strncpy_s(currentTrack.Title, sizeof(currentTrack.Title), hwndTitle, p-hwndTitle);
	
	currentTrack.idx = SendMessage(hwndPE,WM_WA_IPC,IPC_PE_GETCURINDEX,0);

	int i,j,pidx =0;

	// assume a healthy title has the form: "45. Author - Title - Winamp"
	// used to get title & author seperately
	p = strchr(hwndTitle, '.');
	if (p){
		for(i=p-hwndTitle-1, j=0; i>=0; i--, j++){
			if (!ISINT(hwndTitle[i]))
				break; //error
			pidx += pow10(j)*(hwndTitle[i] - '0');	//check if start of str until '.' is equal to playlist idx
		}
		if ((pidx == currentTrack.idx+1) && i == -1){
			char *q = strchr(p, '-');
			if (q && BOUND(q-p-3, 0, sizeof(currentTrack.artist)))
				strncpy_s(currentTrack.artist, sizeof(currentTrack.artist), p+2, q-p-3);

			p = q+1;
			q = strstr( p, "- Winamp" );
			if (!q){
				q = strstr( p, "Winamp" );
				if (!q){
					strncpy_s(currentTrack.songtitle, sizeof(currentTrack.songtitle), p, _TRUNCATE);
					songInfoStatus = WA_SONGINFO_OK;
					return WA_SONGINFO_OK;
				}
			}
			if (!currentTrack.songtitle[0] && BOUND(q-p-2, 0, sizeof(currentTrack.songtitle)))
				strncpy_s(currentTrack.songtitle, sizeof(currentTrack.songtitle), p+1, q-p-2);

			songInfoStatus = WA_SONGINFO_OK;
			LeaveCriticalSection(_cs);
			return WA_SONGINFO_OK;
		}
	}

	LeaveCriticalSection(_cs);
	return WA_SONGINFO_TITLE;	
}

// needs > 2.05
int CWinamp::CurrentSong()
{
	EnterCriticalSection(_cs);

	if (winampVersion >= 0x5025)
		currentTrack.samplerate = winampMessage(5,IPC_GETINFO);
	else 
		currentTrack.samplerate = winampMessage(0,IPC_GETINFO)*1000;

	currentTrack.bitrate = winampMessage(1,IPC_GETINFO);
	currentTrack.channels = winampMessage(2,IPC_GETINFO);
	currentTrack.duration = this->GetTrackLength();
	currentTrack.position = this->GetTrackPosition();
	
	currentTrack.idx = SendMessage(hwndPE,WM_WA_IPC,IPC_PE_GETCURINDEX,0);

	LeaveCriticalSection(_cs);
	return this->GetTitle();	
}

void CWinamp::LoadSettings(char *file)
{
	if (!file)
		return;

	char val[64];
	GetPrivateProfileString("Winamp Settings", "musicspam", "^oPlaying: ^0[^2[t]^0] ^0[^3[P]^0]", this->spamFormatStr, 512, file);
	GetPrivateProfileString("Winamp Settings", "showsong", "false", val, 64, file);
	if(!strncmp(val,"true", 4)) { this->showSong = true; } else { this->showSong = false; }	
	GetPrivateProfileString("Winamp Settings", "enablewinamp", "false", val, 64, file);
	if(!strncmp(val,"true", 4)) { this->enableWinamp = true; } else { this->enableWinamp = false; }
	GetPrivateProfileString("Winamp Settings", "scrollrate", "20", val, 64, file);
	this->scrollRate = atoi(val);
	GetPrivateProfileString("Winamp Settings", "autospam", "false", val, 64, file);
	if(!strncmp(val,"true", 4)) { this->autoSpam = true; } else { this->autoSpam = false; }

	if (this->enableWinamp)
		StartThread();
}

void CWinamp::SaveSettings(const char *file)
{
	if (!file)
		return;


	char val[64];
	WritePrivateProfileString("Winamp Settings", "musicspam", this->spamFormatStr, file);
	WritePrivateProfileString("Winamp Settings", "enablewinamp", this->enableWinamp ? "true" : "false", file);
	WritePrivateProfileString("Winamp Settings", "showsong", this->enableWinamp ? "true" : "false", file);
	_itoa_s(this->scrollRate, val, 64, 10);
	WritePrivateProfileString("Winamp Settings", "scrollrate", val, file);
	WritePrivateProfileString("Winamp Settings", "autospam", this->autoSpam ? "true" : "false", file);
}

//[t] - title
//[w] - winamp formatted title
//[a] - artist
//[b] - bitrate
//[c] - channels
//[s] - samplerate
//[d] - song duration in seconds
//[D] - song duration in minutes:seconds
//[p] - position in song in seconds
//[P] - position in song in minutes:seconds
void CWinamp::formatSpamString(bool infComplete)
{
	char *s = spamFormatStr;

	if (!(*s)){
		spamStr[0] = '\0';
		return;
	}

	bool isStream = currentTrack.duration ? false : true;
	int i;

	spamStr[0] = '\0';

	for( i=0; i<strlen(s); i++){
		if (s[i]=='[' && s[i+2] == ']'){
			switch (s[i+1]){
				case 'w':
					strcat_s(spamStr, sizeof(spamStr), currentTrack.Title);
					break;
				case 't':
					if (!infComplete)
						strcat_s(spamStr, sizeof(spamStr), currentTrack.Title);
					else
						strcat_s(spamStr, sizeof(spamStr), currentTrack.songtitle);		
					break;
				case 'a':
					if (infComplete)
						strcat_s(spamStr, sizeof(spamStr), currentTrack.artist);
					break;

				case 'b':
					sprintf_s(spamStr+strlen(spamStr), sizeof(spamStr)-strlen(spamStr), "%i", currentTrack.bitrate);
					break;
				case 'c':
					sprintf_s(spamStr+strlen(spamStr), sizeof(spamStr)-strlen(spamStr), "%i", currentTrack.channels);
					break;
				case 's':
					sprintf_s(spamStr+strlen(spamStr), sizeof(spamStr)-strlen(spamStr), "%i", currentTrack.samplerate);
					break;
				case 'l':
					if (!isStream)
						sprintf_s(spamStr+strlen(spamStr), sizeof(spamStr)-strlen(spamStr), "%i", currentTrack.duration);
					break;
				case 'L':
				{
					if (!isStream){
						int min = currentTrack.duration / 60;
						int sec = currentTrack.duration - min*60;
						sprintf_s(spamStr+strlen(spamStr), sizeof(spamStr)-strlen(spamStr), "%02i:%02i", min, sec);
					}
					break;
				}
				case 'p':
					sprintf_s(spamStr+strlen(spamStr), sizeof(spamStr)-strlen(spamStr), "%i", currentTrack.position);
					break;
				case 'P':
				{
					int min = currentTrack.position / 60000;
					int sec = (currentTrack.position - min*60000)/1000;
					sprintf_s(spamStr+strlen(spamStr), sizeof(spamStr)-strlen(spamStr), "%02i:%02i", min, sec);
					break;
				}
				default:
					i-=3;
					break;
			}
			i+=2;
			continue;
		}
		strncat_s(spamStr, 512, s+i, 1);
	}
}

void CWinamp::MakeSpam(bool console)
{
	switch (this->CurrentSong())
	{
		case WA_SONGINFO_OK:
		{
			this->formatSpamString(true);
			break;
		}

		case WA_SONGINFO_TITLE:
		{
			this->formatSpamString(false);
			break;
		}

		default:
			spamStr[0] = '\0';
			break;
	}

	if (spamStr[0]){
		char buf[1050];		
		sprintf_s(buf, sizeof(buf), "say \"%s\"\n", this->spamStr);
		if (console)
			Syscall.UI_ExecuteText(buf);
		else
			Gui.BufferConsoleCommand(buf);
	} else {
		if (console)
			Syscall.UI_Print("^3CWinamp::MakeSpam() ^1Warning: ^2empty spam string\n");
	}

}

HWND CWinamp::getPlaylistWnd()
{
	HWND pl_wnd = 0;

	// get the playlist editor window (either v2.9x method or the older
	// for compatibility incase < 2.9x are used
	if(winampVersion >= 0x2900){
		pl_wnd = (HWND)winampMessage(IPC_GETWND_PE,IPC_GETWND);
	}
	if(!pl_wnd){
		pl_wnd = FindWindowA("Winamp PE",0);
	}
	return pl_wnd;
}

void CWinamp::GetWinampState()
{
	WAState.index = this->GetPlaylistPosition();
	WAState.paused = (this->GetStatus() == WA_PAUSED);
	WAState.stopped = (this->GetTrackPosition() == WA_NOT_PLAYING);
	WAState.volume = this->GetVolume();

	WAState.position = this->GetTrackPosition();
	WAState.positionMins = WAState.position / 60000;
	WAState.positionSecs = (WAState.position - WAState.positionMins * 60000) / 1000;

	WAState.length = this->GetTrackLength() * 1000;
	WAState.lengthMins = WAState.length / 60000;
	WAState.lengthSecs = (WAState.length - WAState.lengthMins * 60000) / 1000;

	// if currentTrack.duration, actual track and not stream
	// don't want autospam on streaming audio
	if (currentTrack.duration && WAState.length && this->autoSpam) {
		if (this->oldTrackDuration < 0 && WAState.length){
			this->oldTrackDuration = WAState.length;
			Winamp.MakeSpam(false);
		}
		
		if (this->oldTrackDuration > 0 && this->oldTrackDuration != WAState.length){
			this->oldTrackDuration = WAState.length;
			Winamp.MakeSpam(false);
		}
	}
}

void CWinamp::SetCtrlUpdInterval(int msec)
{
	EnterCriticalSection(_cs);

#ifdef ETH32_DEBUG
	if (msec != this->ctrlUpdInterval)
		Debug.Log("Winamp: throtling thread interval to %i ms", msec);
#endif
	this->ctrlUpdInterval = msec; 
	LeaveCriticalSection(_cs);
}

void CWinamp::StartThread()
{
	if (!this->winampThread){
#ifdef ETH32_DEBUG
		Debug.Log("Winamp: starting winamp thread");
#endif
		this->winampThread = CTHREAD(WinampThread);
	}
}

void CWinamp::KillThread()
{
#ifdef ETH32_DEBUG
		Debug.Log("Winamp: terminating winamp thread");
#endif
	if (this->winampThread)
		CLOSETHREAD(this->winampThread);
	this->winampThread = NULL;
	this->wantExit = false;
	this->ReqWAVolume = this->ReqWAPosition = 0;
	WAState.length = 0;
	this->hwndPE = this->hwndWA = NULL;
	this->winAmpInstance = false;
}

// main thread
DWORD WinampThread(void)
{
	Tools.RegisterThread(GetCurrentThreadId(), "WA thread");
	int ctrlUpdateTime = 0;
	int songUpdateTime = 0;
	Sleep(3);

	while (1)
	{
		if (!Winamp.WinampInstance()){
			Winamp.FindInstance();
			Sleep(10);
		} else {
			if ((eth32.cg.cgTime - ctrlUpdateTime > Winamp.CtrlUpdInterval())){
				Winamp.UpdateControls();
				ctrlUpdateTime = eth32.cg.cgTime;
			}

			if (Winamp.WantTitle() && (eth32.cg.cgTime - songUpdateTime > 1000)){
				Winamp.GetTitle();
				//char buf[1024];
				//Winamp.CopyTitle( buf, 1024);
				//Debug.Log("Title: %s Artist %s Title %s", Winamp.currentTrack.Title, Winamp.currentTrack.artist, Winamp.currentTrack.songtitle);
				songUpdateTime = eth32.cg.cgTime;
			}

			if (Winamp.WantExit()){
				Winamp.KillThread();
				return 0;
			}

			Sleep(Winamp.CtrlUpdInterval());
		}		
	}

	return 1;
}
