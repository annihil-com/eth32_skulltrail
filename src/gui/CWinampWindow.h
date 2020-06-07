// ETH32 - an Enemy Territory cheat for windows
// Copyright (c) 2007 eth32 team
// www.cheatersutopia.com & www.nixcoders.org

#pragma once

#include "CWindow.h"

class CWinampButton : public CControl
{
public:
	CWinampButton(char *clabel, int cx, int cy, int cw, int ch, int watype);
	void Display(void);
	int ProcessMouse(int mx, int my, UINT mevent, CControl **mhook, CControl **khook);
	void MouseMove(int mx, int my);
private:
	int type;
	qhandle_t shader;
};	

class CWinampSlider : public CControl
{
private:
	int *target;
	int max, min;
	int sliderPos;
	int sliderRange;
	float sliderUnit;
	int btnHeight;
	int btnWidth;
	int type;
public:
	CWinampSlider(char *clabel, int cx, int cy, int cw, int ch, int cmin, int cmax, int ctype, int *ctarget);
	void Display(void);
	int ProcessMouse(int mx, int my, UINT mevent, CControl **mhook, CControl **khook);
	void MouseMove(int mx, int my);
	void SetOrigin(int ox, int oy);
	void SetMax(int cmax);
	void Refresh(void);
	void SetTarget(int *ctarget);
};

class CWinampWindow : public CWindow
{
public:
	CWinampWindow(char *wlabel, int wx, int wy, int ww, int wh);
	void Display(void);
private:
	void DrawStatus(int tx, int ty, int tw);
	void DrawScrollingText(int tx, int ty, int tw, char *txt);
	char songText[1024];
	char drawText[1024];
	DWORD lastShiftTime;
	int shift;
	int timeWidth;
	CWinampSlider *sliderPosition;
};