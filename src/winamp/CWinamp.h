// ETH32 - an Enemy Territory cheat for windows
// Copyright (c) 2007 eth32 team
// www.cheatersutopia.com & www.nixcoders.org

/*	class for controlling winamp trough IPC
	needs a seperate thread for optimal use */

#pragma once

#define WA_PAUSED	3
#define WA_PLAYING	1
#define WA_STOPPED	0

#define WA_UNKNOWN_TRACK_LENGTH		-1
#define WA_NOT_PLAYING				-1

#define WA_SEEK_EOS		1
#define WA_SEEK_OK		0

#define WA_SONGINFO_FAIL	0
#define WA_SONGINFO_OK		1
#define WA_SONGINFO_TITLE	2

// Controls
#define WA_CTRL_VOLUME		0
#define WA_CTRL_POSITION	1
#define WA_CTRL_PLAY		2
#define WA_CTRL_PAUSE		3
#define WA_CTRL_STOP		4
#define WA_CTRL_NEXT		5
#define WA_CTRL_PREV		6
#define WA_CTRL_ENABLE_CTRL	7
#define WA_CTRL_SHOW_TITLE	8

typedef struct {
	int bandValue[10];			//0-63 (+20db - -20db)
	int preamp;					//0-63 (+20db - -20db)
	bool enabled;
	bool autoload;
} eqData_t;

typedef struct {
	int		samplerate;
	int		bitrate;
	int		channels;
	int		idx;
	int		duration;
	int		position;
	char	artist[128];
	char	songtitle[128];
	char	Title[256];
} songInfo_t;

typedef struct {
	int index;
	int paused;
	int stopped;
	int volume;

	int length;
	int lengthMins, lengthSecs;
	int position;
	int positionMins, positionSecs;

} winampState_t;

class CWinamp
{
public:
	CWinamp(void);
	~CWinamp(void);

	// search for active winamp screens of the proper version
	void FindInstance();

	// commands
	void Play();
	void Stop();
	void Pause();
	void RestartTrack();
	int	SeekTrack(int msec);			// milliseconds from start
	void PlayNext();
	void PlayPrev();
	int PlayPosition(int pos);
	void SetVolume(int volume);

	void UpdatePosition();

	void GetWinampState();

	int CurrentSong();
	int GetStatus();
	int GetTrackLength();				// length in seconds
	int GetTrackPosition();				// position in the song, in milliseconds
	int GetVolume();	
	int GetPlaylistLength();			// number of playlist items
	int GetPlaylistPosition();

	void GetEQData(eqData_t *e);
	void GetEQData();
	int GetEQData(int band);

	void SetEQData(eqData_t *e);
	void SetEQBand(int band, int val);

	void SetSpamFormat(const char *format);
	void MakeSpam(bool console);
	//void SetEQPreamp(int preamp, int val);
	//void SetEQEnable(bool enable);

	bool WinampInstance();
	void StartThread();
	void KillThread();	// should only be accessed by winamp thread itself!
	void ReqKillThread();
	bool WantExit();
	bool WantTitle();
	void SetTitle(bool enabled);
	
	void SetCtrlUpdInterval(int msec);
	int CtrlUpdInterval();
	void UpdateControls();
	int GetTitle();
	void CopyTitle( char *oTitle, size_t _s_t );
	void UpdateSpamFormat( char *newFmt );
	const char* GetFormatString(void) { return this->spamFormatStr; }

	void LoadSettings(char *file);
	void SaveSettings(const char *file);

	songInfo_t currentTrack;	// all possible infos about the current track
	winampState_t WAState;		// winamp state - excluding title

	int ReqWAVolume;			// requested volume
	int ReqWAPosition;			// requested position

	// **** winamp ini settings here **** //
	bool enableWinamp;
	bool autoSpam;
	bool showSong;
	int scrollRate;

private:
	HWND getPlaylistWnd();
	int winampMessage(WPARAM param, LPARAM cmd);
	int winampCommand(WPARAM command);
	int playlistMessage(WPARAM param, LPARAM cmd);

	void formatSpamString(bool infComplete);

	HWND hwndWA;							// winamp
	HWND hwndPE;							// playlist editor
	LRESULT winampVersion;
	
	eqData_t eq;
	

	bool paused;
	char hwndTitle[2048];		// window title - for quick check if song changed
	char spamStr[1024];			// window title - for quick check if song changed
	char spamFormatStr[512];	// ex: "^3Now Playing [t] by [a] - [k]kHz"
	
	int songInfoStatus;			// error occured - no info available

	bool winAmpInstance;		// set true if present, false if not or winamp version unsupported
	bool wantExit;				// let the thread kill itself
	bool wantTitle;				// do we want to retrieve title info

	void *winampThread;
	int ctrlUpdInterval;		// update interval for volume, position, etc
	int	oldTrackDuration;		// used to get signal when new song is playing

	CRITICAL_SECTION *_cs;
};

extern CWinamp Winamp;
DWORD WinampThread(void);