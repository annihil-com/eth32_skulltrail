// ETH32 - an Enemy Territory cheat for windows
// Copyright (c) 2007 eth32 team
// www.cheatersutopia.com & www.nixcoders.org

#include "../eth32.h"

/********************
* CScrollBar        *
********************/

CScrollBar::CScrollBar(char *clabel, int cx, int cy, int ch, int maxDisplay, int numCurrent, bool topDown)
: CControl(clabel, cx, cy, GUI_SCROLLBAR_WIDTH, ch)
{
	SetMaxDisplayItems(maxDisplay);
	SetNumCurrentItems(numCurrent);

	alignTopDown = topDown;
	currentItemNum = 0;

	Reconfigure();
}

void CScrollBar::Display(void)
{
	if (numCurrentItems <= maxDisplayItems)
		return;

	// buttons and track
	Draw.RawPic(x, y, w, w, eth32.guiAssets.scrollbarArrow);
	Draw.RawPicST(x, yBot, w, w, 0, 1, 1, 0, eth32.guiAssets.scrollbarArrow);

	Draw.RawPic(x, yTop, w, butPosition - yTop, eth32.guiAssets.scrollbarTrack);

	int drawPos = butPosition;
	// scroll button
	if (butSize <= w) { 
		Draw.RawPic(x, butPosition, w, butSize, eth32.guiAssets.scrollbarBtn);
		drawPos = butPosition + butSize;
	} 
	else {
		drawPos = butPosition;
		int drawHeight = (w>>1);
		Draw.RawPicST(x, butPosition, w, drawHeight, 0, 0, 1, 0.5f, eth32.guiAssets.scrollbarBtn);

		drawPos += drawHeight;
		drawHeight = butSize - w;
		Draw.RawPicST(x, drawPos, w, drawHeight, 0, 0.5f, 1, 0.6f, eth32.guiAssets.scrollbarBtn);

		drawPos += drawHeight;
		drawHeight = (w>>1);
		Draw.RawPicST(x, drawPos, w, drawHeight, 0, .5f, 1, 1, eth32.guiAssets.scrollbarBtn);

		drawPos += drawHeight;
	}

	if (drawPos < yBot)
		Draw.RawPic(x, drawPos, w, yBot - drawPos, eth32.guiAssets.scrollbarTrack);
}

int CScrollBar::ProcessMouse(int mx, int my, UINT mevent, CControl **mhook, CControl **khook)
{
	if (maxDisplayItems >= numCurrentItems)
		return 1;

	if (mevent == WM_LBUTTONDOWN && INBOUNDS(mx,my))
	{
		if (INBOUNDS2(mx, my, x, y, x2, yTop))
		{
			TopButtonPressed();
			Reconfigure();
			return 0;
		}
		if (INBOUNDS2(mx, my, x, yBot, x2, y2))
		{
			BotButtonPressed();
			Reconfigure();
			return 0;
		}

		butPosition = my - (butSize >> 1);
		if (butPosition < yScrollTop)
			butPosition = yScrollTop;
		if (butPosition > yScrollBot)
			butPosition = yScrollBot;

		currentItemNum = GetItemFromPosition();
		*mhook = this;

		return 0;
	}

	if (*mhook == this)
		*mhook = NULL;
	return 1;
}

void CScrollBar::MouseMove(int mx, int my)
{
	butPosition = my - (butSize >> 1);
	if (butPosition < yScrollTop)
		butPosition = yScrollTop;
	if (butPosition > yScrollBot)
		butPosition = yScrollBot;

	currentItemNum = GetItemFromPosition();
}

void CScrollBar::SetOrigin(int ox, int oy)
{
	CControl::SetOrigin(ox, oy);

	yTop = y + w;
	yBot = y2 - w;
	yScrollTop = yTop;
	yScrollBot = yScrollTop + yScrollRange;

	Reconfigure();
}

void CScrollBar::Resize(int cw, int ch)
{
	CControl::Resize(cw, ch);
	Reconfigure();
}

void CScrollBar::SetMaxDisplayItems(int maxDisplay)
{
	if (maxDisplay != maxDisplayItems)
	{
		maxDisplayItems = maxDisplay;
		Reconfigure();
	}
}

void CScrollBar::SetNumCurrentItems(int numCurrent)
{
	if (numCurrent != numCurrentItems)
	{
		numCurrentItems = numCurrent;

		if (currentItemNum > numCurrentItems)
			currentItemNum = numCurrentItems-1;

		if (numCurrentItems <= maxDisplayItems)
			currentItemNum = 0;

		Reconfigure();
	}
}

void CScrollBar::IncCurrentItems(void)
{
	numCurrentItems++;
	Reconfigure();
}

int CScrollBar::GetItemFromPosition(void)
{
	if (alignTopDown)
		return ((butPosition - yScrollTop) / yScrollScale);
	else return ((yScrollBot - butPosition) / yScrollScale);
}

void CScrollBar::DecCurrentItems(void)
{
	if (numCurrentItems > 0)
		numCurrentItems--;
	Reconfigure();
}

void CScrollBar::SetCurrentItemNum(int currentItem)
{
	if (currentItem > (numCurrentItems - maxDisplayItems))
		currentItem = numCurrentItems - maxDisplayItems;
	if (currentItem < 0)
		currentItem = 0;

	currentItemNum = currentItem;
	Reconfigure();
}

void CScrollBar::AdjCurrentItemNum(int amt)
{
	SetCurrentItemNum(currentItemNum + amt);
}

void CScrollBar::TopButtonPressed(void)
{
	if (alignTopDown)
		currentItemNum--;
	else currentItemNum++;

	if (currentItemNum < 0)
		currentItemNum = 0;
	else if (currentItemNum > (numCurrentItems - maxDisplayItems))
		currentItemNum = numCurrentItems - maxDisplayItems;
}

void CScrollBar::BotButtonPressed(void)
{
	if (alignTopDown)
		currentItemNum++;
	else currentItemNum--;

	if (currentItemNum < 0)
		currentItemNum = 0;
	else if (currentItemNum > (numCurrentItems - maxDisplayItems))
		currentItemNum = numCurrentItems - maxDisplayItems;
}

#define IDEAL_PXL_PER_ITEM		2
void CScrollBar::Reconfigure(void)
{
	yTop = y + w;
	yBot = y2 - w;
	yRange = yBot - yTop;

	if (numCurrentItems > maxDisplayItems) {
		numScrollItems = numCurrentItems - maxDisplayItems;

		// we have more pixels than items, yScrollScale should be greater than 1
		if ((yRange - GUI_SCROLL_BTN_MIN) > numScrollItems)  {
			butSize = yRange - (numScrollItems * IDEAL_PXL_PER_ITEM);
			if (butSize < GUI_SCROLL_BTN_MIN) 
				butSize = GUI_SCROLL_BTN_MIN;
		}
		else butSize = GUI_SCROLL_BTN_MIN;

		yScrollRange = yRange - butSize;
		yScrollScale = yScrollRange / (float)numScrollItems;
		yScrollTop = yTop;
		yScrollBot = yScrollTop + yScrollRange;

		if (alignTopDown)
			butPosition = yTop + (currentItemNum * yScrollScale);
		else 
			butPosition = yBot - butSize - (currentItemNum * yScrollScale);


	}
}
		

		

	