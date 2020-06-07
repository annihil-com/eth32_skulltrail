// ETH32 - an Enemy Territory cheat for windows
// Copyright (c) 2007 eth32 team
// www.cheatersutopia.com & www.nixcoders.org

#include "../eth32.h"

/***********************
* CWinampButton
***********************/

CWinampButton::CWinampButton(char *clabel, int cx, int cy, int cw, int ch, int watype)
: CControl(clabel, cx, cy, cw, ch)
{
	type = watype;
	
	if (type == WA_CTRL_PLAY)
		shader = eth32.cg.media.waPlay;
	else if (type == WA_CTRL_STOP)
		shader = eth32.cg.media.waStop;
	else if (type == WA_CTRL_PAUSE)
		shader = eth32.cg.media.waPause;
	else if (type == WA_CTRL_NEXT)
		shader = eth32.cg.media.waNext;
	else if (type == WA_CTRL_PREV)
		shader = eth32.cg.media.waPrev;
	else // shouldn't get here
		shader = 0;
}

void CWinampButton::Display(void)
{
	Syscall.R_SetColor(selected ? GUI_FONTCOLOR2 : GUI_FONTCOLOR1);
	Draw.RawPic(x, y, w, w, shader);
	Syscall.R_SetColor(NULL);
}

int CWinampButton::ProcessMouse(int mx, int my, UINT mevent, CControl **mhook, CControl **khook)
{
	if (INBOUNDS(mx,my)) {
		if (mevent == WM_LBUTTONDOWN) {
			selected = true;
			*mhook = this;
		}
		else if (mevent == WM_LBUTTONUP && selected) {
			if (type == WA_CTRL_PLAY)
				Winamp.Play();
			else if (type == WA_CTRL_STOP)
				Winamp.Stop();
			else if (type == WA_CTRL_PAUSE)
				Winamp.Pause();
			else if (type == WA_CTRL_NEXT)
				Winamp.PlayNext();
			else if (type == WA_CTRL_PREV)
				Winamp.PlayPrev();

			selected = false;
			*mhook = NULL;
		}
		else {
			selected = false;
			*mhook = NULL;
		}
		return 0;
	}

	return 1;
}

void CWinampButton::MouseMove(int mx, int my)
{
	selected = INBOUNDS(mx,my);
}

/***********************
* CWinampSlider
***********************/

CWinampSlider::CWinampSlider(char *clabel, int cx, int cy, int cw, int ch, int cmin, int cmax, int ctype, int *ctarget)
: CControl(clabel, cx, cy, cw, ch)
{
	target = ctarget;
	min = cmin;
	max = cmax;
	type = ctype;
	btnHeight = GUI_FONTHEIGHT;
	btnWidth = btnHeight * 1.4;
	
	sliderRange = max - min;
	sliderUnit = sliderRange / (float)(w - btnWidth);

	Refresh();
}

void CWinampSlider::Display(void)
{
	if (!target)
		return;

	Refresh();

	// draw track
	Draw.RawPicST(x + (btnWidth >> 1), y, btnWidth, btnHeight, 0, 0, 0.25, 1, eth32.guiAssets.sliderTrack);
	Draw.RawPicST(x + btnWidth + (btnWidth >> 1), y, w - (btnWidth * 3), btnHeight, 0.25, 0, 0.75, 1, eth32.guiAssets.sliderTrack);
	Draw.RawPicST(x2 - btnWidth - (btnWidth >> 1), y, btnWidth, btnHeight, 0.75, 0, 1, 1, eth32.guiAssets.sliderTrack);

	// draw btn
	Draw.RawPic(sliderPos - (btnWidth >> 1), y, btnWidth, btnHeight, eth32.guiAssets.sliderBtn);
}

int CWinampSlider::ProcessMouse(int mx, int my, UINT event, CControl **mhook, CControl **khook)
{
	if (!target)
		return 1;

	if (!Winamp.WinampInstance())
		return 1;

	if (INBOUNDS(mx,my)) {
		if (event == WM_LBUTTONDOWN && INBOUNDS2(mx,my,x,y,x2,y+btnHeight)) {	// is mouse in slider region
			// give the winamp thread extra priority updating controls
			*mhook = this;
			*target = min + (int)((mx - x - (btnWidth >> 1)) * sliderUnit + 0.5);	// update our target

			Refresh();

			if (type==WA_CTRL_VOLUME)
				Winamp.SetVolume(*target);
			else if(type==WA_CTRL_POSITION)
				Winamp.SeekTrack(*target);

			return 0;
		}
		*mhook = NULL;
		// mouse was in bounds, event handled
		return 0;	
	}
	return 1;
}

void CWinampSlider::MouseMove(int mx, int my)
{
	if (!Winamp.WinampInstance())
		return;

	if (mx < x) mx = x;
	if (mx > x2) mx = x2;
	*target = min + (int)((mx - x - (btnWidth >> 1)) * sliderUnit + 0.5); // fix for snapping target to right value based on position

	Refresh();

	if (type==WA_CTRL_VOLUME)
		Winamp.SetVolume(*target);
	else if(type==WA_CTRL_POSITION)
		Winamp.SeekTrack(*target);

	return;
}

void CWinampSlider::SetOrigin(int ox, int oy)
{
	CControl::SetOrigin(ox,oy);
	Refresh();
}

void CWinampSlider::SetMax(int cmax)
{
	max = cmax;
	if (max < min) max = min;

	sliderRange = max - min;
	sliderUnit = sliderRange / (float)(w - btnWidth);

	Refresh();
}

void CWinampSlider::Refresh(void)
{
	if (target) {
		if (*target < min) *target = min;
		if (*target > max) *target = max;
		sliderPos = x + (btnWidth >> 1) + (*target - min) / sliderUnit;
	}
}

void CWinampSlider::SetTarget(int *ctarget)
{
	target = ctarget;
	Refresh();
}

/***********************
* CWinampWindow
***********************/

CWinampWindow::CWinampWindow(char *wlabel, int wx, int wy, int ww, int wh)
: CWindow(wlabel, wx, wy, ww, wh)
{
	showTitle = false;
	lastShiftTime = eth32.cg.time;
	timeWidth = GUI_SPACING + TEXTWIDTH("00:00") + GUI_SPACING;
	shift = 0;

	int btnSize = 10;
	int spacing = (ww - (btnSize * 5)) / 6;

	int xPos = spacing;
	int yPos = 10 + ((GUI_FONTHEIGHT + 8) / eth32.cg.screenYScale); //wh - btnSize - 5;

	sliderPosition = new CWinampSlider("position", 5, yPos, ww-10, GUI_FONTHEIGHT / eth32.cg.screenYScale, 0, 100, WA_CTRL_POSITION, &Winamp.WAState.position);
	AddControl((CControl*)sliderPosition);
	yPos += (GUI_FONTHEIGHT / eth32.cg.screenYScale) + 5;
	
	CWinampButton *btn;

	btn = new CWinampButton("prev", xPos, yPos, btnSize, btnSize, WA_CTRL_PREV);
	AddControl((CControl*)btn);
	xPos += btnSize + spacing;

	btn = new CWinampButton("play", xPos, yPos, btnSize, btnSize, WA_CTRL_PLAY);
	AddControl((CControl*)btn);
	xPos += btnSize + spacing;

	btn = new CWinampButton("pause", xPos, yPos, btnSize, btnSize, WA_CTRL_PAUSE);
	AddControl((CControl*)btn);
	xPos += btnSize + spacing;

	btn = new CWinampButton("stop", xPos, yPos, btnSize, btnSize, WA_CTRL_STOP);
	AddControl((CControl*)btn);
	xPos += btnSize + spacing;

	btn = new CWinampButton("next", xPos, yPos, btnSize, btnSize, WA_CTRL_NEXT);
	AddControl((CControl*)btn);
	xPos += btnSize + spacing;
	
	yPos += btnSize + 5;
	CWinampSlider *ctrl = new CWinampSlider("volume", ww >> 1, yPos, (ww >> 1) - 5, GUI_FONTHEIGHT / eth32.cg.screenYScale, 0, 255, WA_CTRL_VOLUME, &Winamp.WAState.volume);
	AddControl((CControl*)ctrl);
}

void CWinampWindow::Display(void)
{
	if (!Winamp.WinampInstance() || !Winamp.enableWinamp) // should this be part of eth32.settings ???
		return;

	Winamp.SetTitle(true);

	if (Gui.InputActive()) {
		sliderPosition->SetMax(Winamp.WAState.length);
		Winamp.SetCtrlUpdInterval(150);		// for faster response of volume/etc slider
		CWindow::Display();
		DrawStatus(x + GUI_SPACING, y + GUI_SPACING, w - (GUI_SPACING * 2));
	}
	else {
		if (Winamp.showSong) {
			Winamp.SetCtrlUpdInterval(500);
			DrawStatus(x + GUI_SPACING, y + GUI_SPACING, w - (GUI_SPACING * 2));
		}
		else
			Winamp.SetCtrlUpdInterval(5000);
	}
}

void CWinampWindow::DrawStatus(int tx, int ty, int tw)
{
	int size = GUI_FONTHEIGHT + 8;
	
	Winamp.CopyTitle(songText, sizeof(songText));
	if (*songText) {
		char length[64];
		sprintf_s(length, sizeof(length), " (%.2i:%.2i)", Winamp.WAState.lengthMins, Winamp.WAState.lengthSecs);
		strcat_s(songText, sizeof(songText), length);
	}		

	Draw.RawPic(tx, ty, size, size, eth32.guiAssets.txtinputLeft);
	Draw.RawPic(tx + size, ty, tw - (size << 1), size, eth32.guiAssets.txtinputCenter);
	Draw.RawPic(tx + tw - size, ty, size, size, eth32.guiAssets.txtinputRight);

	Syscall.R_SetColor(GUI_FONTCOLOR1);
	if (Winamp.WAState.paused)
		Draw.RawPic(tx + GUI_SPACING, ty + 4, GUI_FONTHEIGHT, GUI_FONTHEIGHT, eth32.cg.media.waPause);
	else if (Winamp.WAState.stopped)
		Draw.RawPic(tx + GUI_SPACING, ty + 4, GUI_FONTHEIGHT, GUI_FONTHEIGHT, eth32.cg.media.waStop);
	else 
		Draw.RawPic(tx + GUI_SPACING, ty + 4, GUI_FONTHEIGHT, GUI_FONTHEIGHT, eth32.cg.media.waPlay);
	Syscall.R_SetColor(NULL);

	char time[64];
	sprintf_s(time, sizeof(time), "%.2i:%.2i", Winamp.WAState.positionMins, Winamp.WAState.positionSecs);
	Draw.TextCenter(tx + tw - (timeWidth >> 1), ty + 4, GUI_FONTSCALE, time, GUI_FONTCOLOR1, qtrue, qfalse, GUI_FONT, true);

	DrawScrollingText(tx + GUI_FONTHEIGHT + GUI_SPACING + 4, ty, tw - GUI_FONTHEIGHT - GUI_SPACING - 4 - timeWidth, songText);	
}

#define WA_PIXEL_SHIFT_TIME		Winamp.scrollRate
void CWinampWindow::DrawScrollingText(int tx, int ty, int tw, char *txt)
{
	static DWORD remainderTime = 0;
	int xPos = 0;

	if ((eth32.cg.time - lastShiftTime + remainderTime) > WA_PIXEL_SHIFT_TIME) {
		shift--;
		remainderTime = eth32.cg.time - lastShiftTime + remainderTime - WA_PIXEL_SHIFT_TIME;
		remainderTime %= WA_PIXEL_SHIFT_TIME;
		lastShiftTime = eth32.cg.time;
	}

	int pixelLength = Draw.sizeTextWidth(GUI_FONTSCALE, songText, GUI_FONT);

	if (-shift >= pixelLength) {
		shift = tw;
		return;
	}

	if (shift < 0) {
		int breakPoint = Draw.BreakStringByWidth(txt, -shift, NULL, GUI_FONT, false)+1;
		xPos = shift + Draw.sizeTextWidth(GUI_FONTSCALE, txt, GUI_FONT, breakPoint);
		char *p = &txt[breakPoint];
		strncpy_s(drawText, sizeof(drawText), p, Draw.BreakStringByWidth(p, tw - xPos, NULL, GUI_FONT, false));
	}
	else {
		xPos = shift;
		strncpy_s(drawText, sizeof(drawText), txt, Draw.BreakStringByWidth(txt, tw - xPos, NULL, GUI_FONT, false));
	}

	Draw.Text(tx + xPos, ty + 4, GUI_FONTSCALE, drawText, GUI_FONTCOLOR2, qtrue, qfalse, GUI_FONT, true);

}

	