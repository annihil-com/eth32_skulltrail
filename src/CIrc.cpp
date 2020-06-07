// ETH32 - an Enemy Territory cheat for windows
// Copyright (c) 2007 eth32 team
// www.cheatersutopia.com & www.nixcoders.org

#include "eth32.h"

// There !!!HAS TO BE!!! a corresponding string for each select type (except IRC_ROUTING_MAX)
const char *ircRoutingText[IRC_ROUTING_MAX] = 
{
	"Off",
	"Current Channel",
	"All Channels",
};

CIrc Irc;

#define CHECKPARAMETERS(cmd_name)	if (!this->parameters) { \
										Gui.IrcStatusMessage("/" cmd_name ": no parameter has been set"); \
										Gui.IrcStatusMessage("-"); \
										return; \
									}

void CIrc::Init()
{
	if (this->init)
		return;

	this->window = (CIrcWindow *) Gui.GetWinByTitle("IRC Frontend");

	this->sock = -1;
	
//#ifdef ETH32_THREADED
	this->ircThread = CTHREAD(IrcThread);
//#else
//	this->ircThread = NULL;
//#endif

	this->init = true;
	nickOld[0] = 0;
}

void CIrc::Kill()
{
	if (this->ircThread) {
		Cmd_Disconnect();
		CLOSETHREAD(this->ircThread)
	}

	this->sock = -1;
	this->init = false;
}

void CIrc::LoadSettings(const char *filename)
{
	if (!filename){
		sprintf_s(this->nick, sizeof(this->nick),"%s%s", IRC_NICK_PREFIX, Portal.GetUsername());
		return;
	}

	char tmp[32];
	char tmp2[128];
	sprintf_s(tmp2, 128, "%s%s", IRC_NICK_PREFIX, Portal.GetUsername());
	GetPrivateProfileString("IRC", "Nick", tmp2, tmp, 32, filename);

	// sol: this is designed to repair a problem with unseeded nick generation
	//      using portal username instead of random number as default
	if (strlen(tmp) && !strncmp(tmp, IRC_NICK_PREFIX, 6)) {
		char *p;
		char tmp3[32];
		
		strcpy_s(tmp3, 32, tmp+strlen(IRC_NICK_PREFIX));
		if (!strlen(tmp3)) { // nick is just PREFIX, fix it
			sprintf_s(this->nick, sizeof(this->nick), "%s%s", IRC_NICK_PREFIX, Portal.GetUsername());
			return;			
		}

		p = &tmp3[strlen(tmp3)-1];
		while (p > tmp3 && *p == '_') { // remove appended underscores
			*p = '\0';
			p--;
		}
		 
		if (*p == '_' || atoi(tmp3)) { // only character left is '_' or # was assigned prior, fix
			sprintf_s(this->nick, sizeof(this->nick), "%s%s", IRC_NICK_PREFIX, Portal.GetUsername());
			return;
		}
	}
		
	strcpy_s(this->nick, sizeof(this->nick), tmp);
}

void CIrc::SaveSettings(const char *filename)
{
	if (!filename)
		return;

	if (!strlen(this->nick) || this->nick[0] == ' ') {
		char tmp[128];
		sprintf_s(tmp,128,"%s%i%i", IRC_NICK_PREFIX, rand() % 20000, rand() % 20000);
		strcpy_s(this->nick, sizeof(this->nick), tmp);
	}

	WritePrivateProfileString("IRC", "Nick", this->nick, filename);
}

void CIrc::SetParameters(const char *parameters)
{
	this->parameters = parameters;
}

void CIrc::ParseUserInput(const char *input)
{
	if (!input || !*input)
		return;

	if (input[0] == '/')
		this->InputCommand(input);
	else
		this->InputMessage(input);
}

void CIrc::InputCommand(const char *input)
{
	if (!input || !*input || strlen(input) < 2)
		return;

	input++;

	char cmd[IRC_MAX_MSG_LEN];
	memset(cmd, 0, sizeof(cmd));

	const char *tmp = strstr(input, " ");
	if (!tmp) {
		strcat_s(cmd, sizeof(cmd), input);
		this->parameters = NULL;
	}
	else {
		int len = tmp - input;
		memcpy(&cmd, input, len);
		this->parameters = ++tmp;
		if (!*this->parameters) this->parameters = NULL;
	}

	if (!_strcmpi(cmd, "disconnect"))
		this->Cmd_Disconnect();
	else if (!_strcmpi(cmd, "nick") || !_strcmpi(cmd, "n"))
		this->Cmd_Nick();
	else if (!_strcmpi(cmd, "join") || !_strcmpi(cmd, "j"))
		this->Cmd_Join();
	else if (!_strcmpi(cmd, "part") || !_strcmpi(cmd, "p"))
		this->Cmd_Part();
	else if (!_strcmpi(cmd, "msg") || !_strcmpi(cmd, "m"))
		this->Cmd_Msg();
	else if (!_strcmpi(cmd, "whois"))
		this->Cmd_Whois();
	else {
		char buf[MAX_STRING_CHARS];
		sprintf_s(buf,sizeof(buf),"* Unknown command: /%s", cmd);
		Gui.IrcStatusMessage(buf);
	}
}

void CIrc::InputMessage(const char *input)
{
	if (!input || !*input)
		return;

	this->parameters = input;
	this->Cmd_Msg();
	this->parameters = NULL;
}

void CIrc::ConnectFailure(void)
{
	closesocket(this->sock);
	this->connected = false;
	this->sock = -1;
	this->connecting = false;

	if (this->ircThread)
		CLOSETHREAD(this->ircThread);
}

void CIrc::ParseRawEvent(std::string rawevent)
{
//	Debug.Log("RAW: %s", rawevent.c_str());
	rawevent = rawevent.substr(0, rawevent.length() - 1);
	StringTokenizer token(rawevent, ":");
	std::string raw = token.nextToken();
	std::string args = token.remainingString();

//	Debug.Log("PARSED, RAW: [%s] ARG: [%s]", raw.c_str(), args.c_str());

	StringTokenizer revent(raw, " ");
	std::string ev1 = revent.nextToken();
	std::string ev2 = revent.nextToken();
	std::string ev3 = revent.nextToken();
	std::string ev3_rs = revent.remainingString();
	std::string ev4 = revent.nextToken();
//	std::string ev44 = revent.remainingString();
	std::string ev5 = revent.nextToken();
	std::string ev6 = revent.nextToken();
	std::string ev7 = revent.nextToken();

	char buf[512];
	#define COMPARE(num, x) (ev##num.compare(0, sizeof(x), x))
	if (!COMPARE(2, IRC_RAW_REGISTERED)) {
		this->connecting = false;
		memcpy(this->nick, ev3.c_str(), sizeof(this->nick));
		Gui.IrcStatusMessage(args.c_str());
		Gui.IrcStatusMessage("-");
		this->parameters = IRC_CHANNEL;
		this->Cmd_Join();
		this->parameters = NULL;
	} else if (!COMPARE(1, IRC_RAW_PING)) {
		GNetwork.SendText(this->sock, "%s :%s", IRC_RAW_PONG, args.c_str());
	} else if (!COMPARE(2, IRC_RAW_SMOTD) || !COMPARE(2, IRC_RAW_MOTD)) {
		Gui.IrcStatusMessage(args.c_str());
	} else if (!COMPARE(2, IRC_RAW_EMOTD)) {
		Gui.IrcStatusMessage(args.c_str());
		Gui.IrcStatusMessage("-");
	} else if (!COMPARE(2, IRC_RAW_JOIN)) {
		std::string joinnick = StringTokenizer(ev1, "!").nextToken();		
		if (!strcmp(this->nick, joinnick.c_str())){
			this->window->AddChannel(args.c_str());
		} else  {
			this->window->AddUser(joinnick.c_str(), args.c_str());
		}
	} else if (!COMPARE(2, IRC_RAW_PART)) {
		std::string partnick = StringTokenizer(ev1, "!").nextToken();
		if (!strcmp(this->nick, partnick.c_str()))
			this->window->RemoveChannel(ev3.c_str());
		else 
			this->window->RemoveUser(partnick.c_str(), ev3.c_str(), (args.size())?args.c_str():NULL);
	} else if (!COMPARE(2, IRC_RAW_QUIT)) {
		std::string quitnick = StringTokenizer(ev1, "!").nextToken();
		if (strcmp(this->nick, quitnick.c_str()))
			this->window->UserQuit(quitnick.c_str(), (args.size())?args.c_str():NULL);
	} else if (!COMPARE(2, IRC_RAW_TOPIC)) {
		sprintf_s(buf,sizeof(buf),"* Topic for this channel is: %s", args.c_str());
		this->window->ChannelMessage(ev4.c_str(), NULL, buf);
	} else if (!COMPARE(2, IRC_RAW_PRIVMSG)) {
		StringTokenizer from(ev1, "!");
		this->window->ChannelMessage(ev3.c_str(), from.nextToken().c_str(), args.c_str());
	} else if (!COMPARE(2, IRC_RAW_NAMES)) {
		StringTokenizer userlist(args, " ");
		while (userlist.hasMoreTokens())
			this->window->AddUser(userlist.nextToken().c_str(), ev5.c_str(), false);
	} else if (!COMPARE(2, IRC_RAW_NICK_INUSE)) {
		sprintf_s(buf,sizeof(buf), "* %s nick already in use", ev4.c_str());
		Gui.IrcStatusMessage(buf);
		Gui.IrcStatusMessage("-");
		
		if (this->connecting) {
			std::string newnick = ev4 + "_";
			this->parameters = newnick.c_str();
			this->Cmd_Nick();
			this->parameters = NULL;
		}
		else  // our new nick was already used, restore old nick
			strcpy_s(this->nick, sizeof(this->nick), this->nickOld);
	} else if (!COMPARE(2, IRC_RAW_NICK)) {
		StringTokenizer from(ev1, "!");
		this->window->NickChange(from.nextToken().c_str(), args.c_str());
	} else if (!COMPARE(2, IRC_RAW_MODE)) {
		if (ev4.length()) {
			StringTokenizer from(ev1, "!");
			CIrcChannelWindow *win = this->window->GetChannelWindowByName(ev3.c_str());
			if (win) {
				std::string op = from.nextToken();
				win->ChangeUserMode(op.c_str(), ev5.c_str(), ev4.c_str());
				char msg[1024];
				sprintf_s(msg, sizeof(msg), "* %s sets mode %s\0", op.c_str(), ev3_rs.c_str());
				win->AddMessage(msg);
			}
		}
	} else if (!COMPARE(2, IRC_RAW_WHOISUSER)) {
		sprintf_s(buf,sizeof(buf),"  User: %s@%s %s %s", ev5.c_str(), ev6.c_str(), ev7.c_str(), args.c_str());
		Gui.IrcStatusMessage(buf);
	} else if (!COMPARE(2, IRC_RAW_WHOISCHANNELS)) {
		sprintf_s(buf,sizeof(buf),"  Channels: %s", args.c_str());
		Gui.IrcStatusMessage(buf);
	} else if (!COMPARE(2, IRC_RAW_WHOISSERVER)) {
		sprintf_s(buf,sizeof(buf),"  Server: %s (%s)", ev5.c_str(), args.c_str());
		Gui.IrcStatusMessage(buf);
	} else if (!COMPARE(2, IRC_RAW_ENDOFWHOIS)) {
		Gui.IrcStatusMessage(args.c_str());
		Gui.IrcStatusMessage("-");
	} else if (!COMPARE(2, IRC_RAW_KICK)) {
		StringTokenizer kicker(ev1, "!");
		std::string src = kicker.nextToken();

		char tmp[1024];
		if (!strcmp(ev4.c_str(), this->nick)) {
			sprintf_s(tmp, sizeof(tmp), "* You were kicked from %s by %s [ %s ]\0", ev3.c_str(), src.c_str(), *args.c_str() ? args.c_str() : "");
			Gui.IrcStatusMessage(tmp);
			Gui.IrcStatusMessage("-");
			this->window->RemoveChannel(ev3.c_str());
			this->window->SetActiveChannel("Status");
		}
		else {
			CIrcChannelWindow *win = this->window->GetChannelWindowByName(ev3.c_str());
			if (win) {
				win->KickUser(ev4.c_str(), args.c_str(), src.c_str());
			}
		}
	}
}

void CIrc::Cmd_Disconnect(void)
{
	GNetwork.SendText(this->sock, "%s :Yah... it was ETH32", IRC_RAW_QUIT);
	closesocket(this->sock);
	this->connected = false;
	this->sock = -1;
	Gui.IrcStatusMessage("* Disconnected");
	Gui.IrcStatusMessage("-");
}

void CIrc::Cmd_Connect(void)
{
	//CHECKPARAMETERS("connect");
	char buf[128];
	sprintf_s(buf,sizeof(buf),"* Connecting to %s", eth32.settings.ircServer);
	Gui.IrcStatusMessage(buf);
	Gui.IrcStatusMessage("-");

#ifdef ETH32_DEBUG
	Debug.Log("IRC: %s", buf);
#endif

	this->sock = GNetwork.ServerConnect(eth32.settings.ircServer, IRC_PORT, SOCK_STREAM);
	this->connected = (this->sock != -1);
}

void CIrc::Cmd_Nick(void)
{
	CHECKPARAMETERS("nick");

	if (this->parameters[0] == ' ') {
		Gui.IrcStatusMessage("* /nick: Invalid nick specified");
		Gui.IrcStatusMessage("-");
		return;
	}
	
	char buf[128];
	
	strcpy_s(this->nickOld, sizeof(this->nickOld), this->nick);
	strcpy_s(this->nick, sizeof(this->nick), this->parameters);
	sprintf_s(buf,sizeof(buf),"New nick: %s", this->nick);
	Gui.IrcStatusMessage(buf);
	Gui.IrcStatusMessage("-");

	if (this->connected)
		GNetwork.SendText(this->sock, "%s %s", IRC_RAW_NICK, this->parameters);
}

void CIrc::Cmd_Join(void)
{
	CHECKPARAMETERS("join");

	GNetwork.SendText(this->sock, "%s %s", IRC_RAW_JOIN, this->parameters);
}

void CIrc::Cmd_Part(void)
{
	char buf[64];
	// no channel specified and "status" is current
	this->window->GetActiveChannel(buf,sizeof(buf));
	if (!this->parameters && !strcmp(buf, "Status")) {
		Gui.IrcStatusMessage("/part: not allowed on Status window");
		Gui.IrcStatusMessage("-");
		return;
	}

	char channel[256];
	if (!this->parameters)
		this->window->GetActiveChannel(channel,sizeof(channel));
	else {
		if (this->window->GetChannelWindowByName(this->parameters) == NULL) {
			// no such channel open
			Gui.IrcStatusMessage("/part: invalid channel specified"); 
			Gui.IrcStatusMessage("-");
			return;
		}
		strcpy_s(channel, sizeof(channel), this->parameters);
	}

	GNetwork.SendText(this->sock, "%s %s", IRC_RAW_PART, channel);
}

void CIrc::Cmd_Msg(void)
{
	CHECKPARAMETERS("msg");

	char channel[64];
	this->window->GetActiveChannel(channel,sizeof(channel));
	if (!strcmp(channel, "Status")) {
		Gui.IrcStatusMessage("/msg: Can't send message to Status window");
		Gui.IrcStatusMessage("-");
		return;
	}

	Gui.IrcMessage(this->nick, this->parameters);
	GNetwork.SendText(this->sock, "%s %s :%s", IRC_RAW_PRIVMSG, channel, this->parameters);
}

void CIrc::Cmd_Whois(void)
{
	CHECKPARAMETERS("whois");

	char buf[128];
	sprintf_s(buf,"WHOIS %s", this->parameters);
	Gui.IrcStatusMessage(buf);
	GNetwork.SendText(this->sock, "%s %s", IRC_RAW_WHOIS, this->parameters);
}

DWORD IrcThread(void)
{
	Tools.RegisterThread(GetCurrentThreadId(), "IRC thread");
	Irc.Cmd_Connect();
	Irc.SetParameters(NULL);

	Irc.connecting = true;
	
	if (!Irc.connected) {
		Gui.IrcStatusMessage("* /connect failed");
		Gui.IrcStatusMessage("-");
		Irc.ConnectFailure();
		return 0;
	}

	bool ok = (GNetwork.SendText(Irc.sock, "%s %s", IRC_RAW_NICK, Irc.nick) && GNetwork.SendText(Irc.sock, "%s %s 0 * :%s", IRC_RAW_USER, Irc.nick, Irc.nick));
	if (!ok) {
		Gui.IrcStatusMessage("* Failed to register to IRC network");
		Gui.IrcStatusMessage("-");
		Irc.ConnectFailure();
		return 0;
	}

	std::string line;
	while (1) {
		char c;

		int result = recv(Irc.sock, &c, 1, 0);
		if (result == 0) {
			Irc.Cmd_Disconnect();
			if (Irc.ircThread)
				CLOSETHREAD(Irc.ircThread);
			return 0;
		}
		else if (result == -1) {
			continue;
			Sleep(20);
		}
		else {
			if (c == '\n') {
				Irc.ParseRawEvent(line);
				line.clear();
				continue;
			}
			line += c;
		}
	}	
	return 1;
}
