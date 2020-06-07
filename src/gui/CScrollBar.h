// ETH32 - an Enemy Territory cheat for windows
// Copyright (c) 2007 eth32 team
// www.cheatersutopia.com & www.nixcoders.org

#pragma once

#include "CControl.h"

class CScrollBar : public CControl
{
public:
	CScrollBar(char *label, int cx, int cy, int ch, int maxDisplay, int numCurrent, bool topDown = true);
	void Display(void);
	int ProcessMouse(int mx, int my, UINT mevent, CControl **mhook, CControl **khook);
	void MouseMove(int mx, int my);
	void SetOrigin(int ox, int oy);
	void Resize(int cw, int ch);
	bool IsActive(void) { return numCurrentItems > maxDisplayItems; }

	void SetMaxDisplayItems(int maxDisplay);
	void SetNumCurrentItems(int numCurrent);

	void IncCurrentItems(void);
	void DecCurrentItems(void);

	void SetCurrentItemNum(int currentItem);	// sets currentItemNum to arg
	void AdjCurrentItemNum(int amt);			// adjusts currentItemNum by arg
	int GetCurrentItemNum(void) { return currentItemNum; }
private:
	void Reconfigure(void);

	void TopButtonPressed(void);
	void BotButtonPressed(void);
	int GetItemFromPosition(void);

	int yTop, yBot;					// top & bottom of scrollable region of control
	int yRange;						// range between yTop & yBot

	int yScrollTop, yScrollBot;		// top & bottom of scrollable region from top of scroll button
	float yScrollScale;				// scale of pixels correlating to items per pixel
	int yScrollRange;				// range of pixels between yScrollTop & yScrollBot

	int maxDisplayItems;			// how many items can be displayed at one time
	int numCurrentItems;			// how many items total in list
	int numScrollItems;				// numCurrentItems - maxDisplayItems

	int currentItemNum;				// current start index into item list

	int butSize;					// height of button
	int butPosition;				// current y position of button (from center)

	bool alignTopDown;					// list starts at top or bottom
};