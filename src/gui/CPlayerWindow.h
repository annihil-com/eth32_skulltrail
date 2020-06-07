// ETH32 - an Enemy Territory cheat for windows
// Copyright (c) 2007 eth32 team
// www.cheatersutopia.com & www.nixcoders.org

#pragma once

#include "CWindow.h"

class CPlayerWindow : public CWindow
{
public:
	CPlayerWindow(char *wlabel, int wx, int wy, int ww, int wh);
	void Display(void);
private:
	void DrawPlayerInfo(int clientNum, int px, int py, int pw, int ph, vec4_t color);
};
