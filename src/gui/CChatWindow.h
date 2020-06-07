// ETH32 - an Enemy Territory cheat for windows
// Copyright (c) 2007 eth32 team
// www.cheatersutopia.com & www.nixcoders.org

#pragma once

#include "CWindow.h"
#include "CTextControls.h"

typedef enum 
{
	CHAT_NONE,
	CHAT_SAY,
	CHAT_TEAM,
	CHAT_FTEAM,
	CHAT_IRC,
	CHAT_PM,
	CHAT_MAX
} chattype_t;

class CChatWindow : public CWindow
{
public:
	CChatWindow(char *wlabel, int wx, int wy, int ww, int wh);
	void Display(void);
	int ProcessMouse(int mx, int my, UINT mevent, CControl **mhook, CControl **khook);
	int ProcessKeybd(unsigned char key, CControl **khook);
	
	void AddMessage(const char *msg);

	void ShowInputFocus(void);
	void HideInputFocus(void);

	void SetTarget(chattype_t type, CControl **khook);
	chattype_t GetTarget(void) { return chatTarget; }

	void SetPrivMsgTarget(const char *target);
	const char* PrivMsgTarget(void) { return privmsgTarget; }
private:
	bool CheckNameClick(int mx, int my, CControl **khook);

	chattype_t chatTarget;

	CTextBuffer *ctrlBuffer;
	CTextInput *ctrlInput;

	char privmsgTarget[128];

	int inputOffset;
};

// Utility text function
void funcRouteChat(const char *msg);