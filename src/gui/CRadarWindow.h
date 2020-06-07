// ETH32 - an Enemy Territory cheat for windows
// Copyright (c) 2007 eth32 team
// www.cheatersutopia.com & www.nixcoders.org

#pragma once

#include "CWindow.h"

class CRadarWindow : public CWindow
{
public:
	CRadarWindow(const char *wlabel, int wx, int wy, int ww, int wh);
	void Display(void);
	int MouseWheel(int mx, int my, int wheel);

	void SetOrigin(int ox, int oy);
	void Reposition(int ox, int oy);

	void AddPlayer(player_t *player);
private:
	player_t *points[MAX_CLIENTS];
	int numPoints;
	int tx;
	int ty;
	int ttx;
	int tty;
	int cx, cy;		// center of window
};

