// ETH32 - an Enemy Territory cheat for windows
// Copyright (c) 2007 eth32 team
// www.cheatersutopia.com & www.nixcoders.org

#include "../eth32.h"

/*****************
* CCheckBox
*****************/

CCheckBox::CCheckBox(char *clabel, int cx, int cy, int cw, int ch, bool *ctarget)
: CControl(clabel, cx, cy, cw, ch)
{
	target = ctarget;
}

void CCheckBox::Display(void)
{
	Draw.Text(x, y, GUI_FONTSCALE, label, GUI_FONTCOLOR1, qtrue, qfalse, GUI_FONT, true);
	Draw.RawPic(x2 - h, y, h, h, eth32.guiAssets.checkBox);

	if (*target)
		Draw.RawPic(x2 - h, y, h, h, eth32.guiAssets.check);
}

int CCheckBox::ProcessMouse(int mx, int my, UINT event, CControl **mhook, CControl **khook)
{
	if (INBOUNDS(mx,my)) {		// checking whole control area, maybe change to just box?
		if (event == WM_LBUTTONDOWN) 
			*target = !(*target);
		*mhook = *khook = NULL;
		return 0;
	}
	return 1;
}

void CCheckBox::SetTarget(bool *ctarget)
{
	target = ctarget;
}

/*****************
* CWACheckBox
*****************/

CWACheckBox::CWACheckBox(char *clabel, int cx, int cy, int cw, int ch, bool *ctarget, int waType)
: CControl(clabel, cx, cy, cw, ch)
{
	target = ctarget;
	WACtrlType = waType;
	phase = 0;
}

void CWACheckBox::Display(void)
{
	Draw.Text(x, y, GUI_FONTSCALE, label, GUI_FONTCOLOR1, qtrue, qfalse, GUI_FONT, true);
	Draw.RawPic(x2 - h, y, h, h, eth32.guiAssets.checkBox);

	if (*target)
		Draw.RawPic(x2 - h, y, h, h, eth32.guiAssets.check);
}

int CWACheckBox::ProcessMouse(int mx, int my, UINT event, CControl **mhook, CControl **khook)
{
	if (INBOUNDS(mx,my)) {		// checking whole control area, maybe change to just box?
		if (event == WM_LBUTTONDOWN){
			*target = !(*target);
			
			switch(WACtrlType)
			{
				case WA_CTRL_SHOW_TITLE:
					if (*target)
						Winamp.SetTitle(true);
					else {
						Winamp.SetCtrlUpdInterval(5000);
						Winamp.SetTitle(false);
					}
					break;
				case WA_CTRL_ENABLE_CTRL:
					if (*target)
						Winamp.StartThread();
					else
						Winamp.ReqKillThread();		// thread must terminate itself when it finishes tasks
					break;
			}
		}
		*mhook = *khook = NULL;
		return 0;
	}
	return 1;
}

void CWACheckBox::SetTarget(bool *ctarget)
{
	target = ctarget;
}