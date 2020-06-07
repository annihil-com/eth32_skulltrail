// ETH32 - an Enemy Territory cheat for windows
// Copyright (c) 2007 eth32 team
// www.cheatersutopia.com & www.nixcoders.org

#pragma once

#include <list>

#include "CWindow.h"
#include "CButtons.h"
#include "CTextControls.h"

typedef struct ircuser_s
{
	char *nick;
	char *display;
	unsigned int opStatus;
} ircuser_t;

class CIrcWindow;

class CIrcChannelButton : public CButton
{
public:
	CIrcChannelButton(const char *chanName, int wx, int wy, int ww, int wh, CIrcWindow *ircWindow);
	~CIrcChannelButton(void);
	void Display(void);
	int ProcessMouse(int mx, int my, UINT event, CControl **mhook, CControl **khook);

	void NewMessage(bool newmsg);
	const char *GetChannelName(void) { return channelName; }

	void SetActiveChannel(bool active) { activeChannel = active; }
public:
	CIrcWindow *winIrc;
	char *channelName;
	bool newMessages;
	bool activeChannel;
};

class CIrcUserList : public CControl
{
public:
	CIrcUserList(int cx, int cy, int cw, int ch);
	~CIrcUserList();
	void Display(void);
	int ProcessMouse(int mx, int my, UINT mevent, CControl **mhook, CControl **khook);
	void Resize(int cw, int ch);
	void Reposition(int cx, int cy);
	void SetOrigin(int ox, int oy);
	bool AddUser(const char *nick);			// return true on success
	bool RemoveUser(const char *nick);		// return true on success

	void FixDisplayNick(ircuser_t *user);
	bool ChangeUserMode(const char *nick, const char *modeStr);

	bool NickChange(const char *oldNick, const char *newNick);
private:
	ircuser_t* CreateIrcUser(const char *nick);
	ircuser_t* GetIrcUser(const char *nick);
	void DestroyIrcUser(ircuser_t *user);
	void InsertUser(ircuser_t *user);
	void RemoveUser(ircuser_t *user);
	bool UserHigher(ircuser_t *thisUser, ircuser_t *otherUser);
	
	int userCount;
	CScrollBar *scrollbar;
	list<ircuser_t*> userlist;
};

class CIrcChannelWindow : public CWindow
{
public:
	CIrcChannelWindow(const char *channel, int cx, int cy, int cw, int ch, bool allowUsers);
	~CIrcChannelWindow(void);
	//void Display(void);
	void Resize(int cw, int ch);

	bool AddUser(const char *nick, bool announce);
	bool RemoveUser(const char *nick, const char *msg);
	bool KickUser(const char *nick, const char *msg, const char *src);
	bool UserQuit(const char *nick, const char *msg);
	void NickChange(const char *oldNick, const char *newNick);
	void ChangeUserMode(const char *opNick, const char *nick, const char *mode);

	void AddMessage(const char *msg);

	void SetChannelButton(CIrcChannelButton *button);
	CIrcChannelButton* GetChannelButton(void) { return ctrlChannelButton; }
	CIrcUserList* GetUserList(void) { return ctrlUserList; }
private:
	// these are created and deleted by the channel window
	CIrcUserList *ctrlUserList;
	CTextBuffer *ctrlBuffer;
	// this is set by main IRC window, and should not be destroyed here
	CIrcChannelButton *ctrlChannelButton;
};

class CIrcWindow : public CWindow
{
public:
	CIrcWindow(char *wlabel, int wx, int wy, int ww, int wh);
	~CIrcWindow(void);
	void Display(void);
	int ProcessMouse(int mx, int my, UINT mevent, CControl **mhook, CControl **khook);
	int ProcessKeybd(unsigned char key, CControl **khook);
	void SetKeybdFocus(bool set);
	bool CaptureEscapeCharacter(void) { return false; }

	void SetProps(const char *props, float xScale, float yScale) {   }

	CIrcChannelWindow* GetChannelWindowByName(const char *channelName);
	
	bool AddChannel(const char *channelName, bool allowUsers = true);
	bool RemoveChannel(const char *channelName);
	bool RemoveAllChannels(void);

	bool AddUser(const char *userName, const char *channelName, bool announce = true);
	bool RemoveUser(const char *userName, const char *channelName, const char *msg = NULL);
	void UserQuit(const char *userName, const char *msg = NULL);

	void NickChange(const char *oldNick, const char *newNick);

	bool ChannelMessage(const char *channelName, const char *nick, const char *msg);

	bool SetActiveChannel(const char *channelName);
	//bool SetActiveChannel(CIrcChannelWindow *chanWindow);
	char* GetActiveChannel(char *s, size_t s_s);
private:
	void Reorganize(void);
	bool InsertButton(CIrcChannelButton *btn);
	bool RemoveButton(CIrcChannelButton *btn);

	CTextInput *ctrlInput;
	
	list<CIrcChannelWindow*> channelList;
	list<CIrcChannelButton*> channelButtons;
	CIrcChannelWindow *activeChannel;
};

// Helper IRC function
void funcIrcUserCommand(const char *cmd);