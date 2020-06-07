// ETH32 - an Enemy Territory cheat for windows
// Copyright (c) 2007 eth32 team
// www.cheatersutopia.com & www.nixcoders.org

#pragma once

// General
#define IRC_PORT		6667
#define IRC_CHANNEL		"#eth32"
#define IRC_NICK_PREFIX	"eth32_"
#define IRC_CLIENT		"eth32 - http://eth32.cheatersutopia.com"

// RAW events
#define IRC_RAW_REGISTERED		"001"
#define IRC_RAW_WHOISUSER		"311"
#define IRC_RAW_WHOISSERVER		"312"
#define IRC_RAW_WHOISOP			"313"
#define IRC_RAW_WHOISIDLE		"317"
#define IRC_RAW_ENDOFWHOIS		"318"
#define IRC_RAW_WHOISCHANNELS	"319"
#define IRC_RAW_TOPIC			"332"
#define IRC_RAW_NAMES			"353"
#define IRC_RAW_MOTD			"372"		// motd line
#define IRC_RAW_SMOTD			"375"		// motd start
#define IRC_RAW_EMOTD			"376"		// motd end
#define IRC_RAW_NICK_INUSE		"433"
#define IRC_RAW_WHOIS			"WHOIS"
#define IRC_RAW_NICK			"NICK"
#define IRC_RAW_USER			"USER"
#define IRC_RAW_MODE			"MODE"
#define IRC_RAW_JOIN			"JOIN"
#define IRC_RAW_PART			"PART"
#define IRC_RAW_QUIT			"QUIT"
#define IRC_RAW_NOTICE			"NOTICE"
#define IRC_RAW_PRIVMSG			"PRIVMSG"
#define IRC_RAW_PING			"PING"
#define IRC_RAW_PONG			"PONG"
#define IRC_RAW_USERHOST		"USERHOST"
#define IRC_RAW_KICK			"KICK"

// GUI
#define IRC_CHAN_BUFFER_SIZE	512
#define IRC_CHAN_FLASH_TIME		1000
#define IRC_MAX_CHANNELS		26
#define IRC_CHAN_BUTTON_WIDTH	80

// USER TYPES
typedef enum {
	IRCUSER_NORMAL = 1,
	IRCUSER_VOICE = 2,
	IRCUSER_HALFOP = 4,
	IRCUSER_OP = 8,
	IRCUSER_PROTECTED = 16,
	IRCUSER_OWNER = 32,
} ircusertype_t;

// ROUTING TYPES
typedef enum {
	IRC_ROUTING_OFF,
	IRC_ROUTING_CURRENT,
	IRC_ROUTING_ALL,
	IRC_ROUTING_MAX,
} ircRouting_t;

extern const char *ircRoutingText[];

class CIrc
{
public:
	void Init();
	void Kill();
	void LoadSettings(const char *filename);
	void SaveSettings(const char *filename);
	void SetParameters(const char *parameters);
	void ParseUserInput(const char *input);
	void InputCommand(const char *input);
	void InputMessage(const char *input);
	void ConnectFailure(void);
	void ParseRawEvent(std::string rawevent);

	void Cmd_Disconnect(void);
	void Cmd_Connect(void);
	void Cmd_Nick(void);
	void Cmd_Join(void);
	void Cmd_Part(void);
	void Cmd_Msg(void);
	void Cmd_Whois(void);

	// vars
	char hostname[64];
	int port;
	int sock;

	void *ircThread;

	bool init;

	bool joinedChannel;
	char nick[32];
	char nickOld[32];
	bool connected;
	bool connecting;

	const char *parameters;

	CIrcWindow *window;
};

extern CIrc Irc;

DWORD IrcThread(void);
