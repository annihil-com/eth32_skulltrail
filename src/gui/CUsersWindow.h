// ETH32 - an Enemy Territory cheat for windows
// Copyright (c) 2007 eth32 team
// www.cheatersutopia.com & www.nixcoders.org

#pragma once

#include "CWindow.h"
#include "CScrollBar.h"
#include "CButtons.h"

class CUsersWindow : public CWindow
{
public:
	CUsersWindow(char *wlabel, int wx, int wy, int ww, int wh);
	~CUsersWindow(void);

	void Display(void);
	int ProcessMouse(int mx, int my, UINT mevent, CControl **mhook, CControl **khook);
	int MouseWheel(int mx, int my, int wheel);
	void SetProps(const char *props, float xScale, float yScale) {   }
private:
	void DrawUser(int drawX, int drawY, int width, int height, eth32user_t *usr, bool selected);
	bool CanJoinSelected(void);

	char username[64];

	CScrollBar *ctrlScrollbar;
	CButton *ctrlJoin;
	CButton *ctrlAddFriend;
	CButton *ctrlRemFriend;

	int numDisplay;
};

