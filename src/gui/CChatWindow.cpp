// ETH32 - an Enemy Territory cheat for windows
// Copyright (c) 2007 eth32 team
// www.cheatersutopia.com & www.nixcoders.org

#include "../eth32.h"

CChatWindow::CChatWindow(char *wlabel, int wx, int wy, int ww, int wh)
: CWindow(wlabel, wx, wy, ww, wh)
{
	showTitle = false;
	chatTarget = CHAT_NONE;

	inputOffset = (GUI_FONTHEIGHT + 8);

	ctrlBuffer = new CTextBuffer("chatbuffer", 5, 5, ww - 10, wh - 10, 512);
	ctrlInput = new CTextInput("chatinput", 5, wh + (5 + GUI_FONTHEIGHT) / eth32.cg.screenYScale, ww - 10, inputOffset / eth32.cg.screenYScale, MAX_SAY_TEXT+1, funcRouteChat);	// funcRouteChat() : see below

	inputOffset += (5 * eth32.cg.screenYScale) + 5 + GUI_FONTHEIGHT;

	lxOffset = (ww >> 1) * eth32.cg.screenXScale;
	lyOffset = wh * eth32.cg.screenYScale;
	lx = x + lxOffset;
	ly = y + lxOffset;

	ctrlInput->Show(false);

	AddControl((CControl*)ctrlBuffer);
	AddControl((CControl*)ctrlInput);
}

void CChatWindow::Display(void)
{
	if (!eth32.settings.guiChat || eth32.settings.guiOriginal)
		return;

	char buf[MAX_STRING_CHARS];
	CWindow::Display();
	
	if (selected) {		// need to print our msg type
		if (chatTarget == CHAT_SAY)
			Draw.TextCenter(lx, ly, GUI_FONTSCALE, "say", GUI_FONTCOLOR1, qtrue, qfalse, GUI_FONT, true);
		else if (chatTarget == CHAT_TEAM)
			Draw.TextCenter(lx, ly, GUI_FONTSCALE, "team", GUI_FONTCOLOR1, qtrue, qfalse, GUI_FONT, true);
		else if (chatTarget == CHAT_FTEAM)
			Draw.TextCenter(lx, ly, GUI_FONTSCALE, "squad", GUI_FONTCOLOR1, qtrue, qfalse, GUI_FONT, true);
		else if (chatTarget == CHAT_PM){
			sprintf_s(buf, sizeof(buf),"pm ^3-> ^*%s", privmsgTarget);
			Draw.TextCenter(lx, ly, GUI_FONTSCALE, buf, GUI_FONTCOLOR1, qfalse, qfalse, GUI_FONT, true);
		} else if (chatTarget == CHAT_IRC) {
			CIrcWindow *ircwin = (CIrcWindow*)Gui.GetWinByTitle("IRC Frontend");
			if (ircwin) {
				char activeChannel[64];
				ircwin->GetActiveChannel(activeChannel,sizeof(activeChannel));
				sprintf_s(buf, sizeof(buf),"irc ^3-> ^*%s", activeChannel);
				Draw.TextCenter(lx, ly, GUI_FONTSCALE, buf, GUI_FONTCOLOR1, qfalse, qfalse, GUI_FONT, true);
			}
		}
	}
}

int CChatWindow::ProcessMouse(int mx, int my, UINT mevent, CControl **mhook, CControl **khook)
{
	if (INBOUNDS(mx, my) && mevent == WM_LBUTTONDOWN && eth32.settings.nameClick && CheckNameClick(mx, my, khook))
		return 0;

	return CWindow::ProcessMouse(mx, my, mevent, mhook, khook);
}

int CChatWindow::ProcessKeybd(unsigned char key, CControl **khook)
{
	if (ctrlInput)
		ctrlInput->ProcessKeybd(key, khook);

	if (key == 0x1B || key == 0x0D)	{	// user pressed esc or enter, so rearrange window
		HideInputFocus();
		*khook = NULL;
	}
	return 0;
}

void CChatWindow::AddMessage(const char *msg)
{
	if (ctrlBuffer)
		ctrlBuffer->AddText(msg);
}

void CChatWindow::ShowInputFocus(void)
{
	if (!selected) {
		inputOffset = (GUI_FONTHEIGHT << 1) + (5 * eth32.cg.screenYScale) + 13;

		Resize(w, h + inputOffset);
		Reposition(x, y - inputOffset);

		ctrlInput->Show(true);
		ctrlInput->SetFocus();
		selected = true;
	}
}

void CChatWindow::HideInputFocus(void)
{
	if (selected) {
		Resize(w, h - inputOffset);
		Reposition(x, y + inputOffset);

		ctrlInput->Show(false);
		selected = false;
	}
}

void CChatWindow::SetTarget(chattype_t type, CControl **khook)
{
	chatTarget = type;
	
	if (chatTarget >= CHAT_SAY && chatTarget <= CHAT_PM) {
		ShowInputFocus();
		*khook = (CControl*)this;
	}
}

void CChatWindow::SetPrivMsgTarget(const char *target)
{
	if (target && *target) {
		strcpy_s(privmsgTarget, 128, target);
	}
}

bool CChatWindow::CheckNameClick(int mx, int my, CControl **khook)
{
	if (ctrlBuffer) {
		const char *p = ctrlBuffer->GetTextClick(mx, my);
		char *end;

		if (!p)
			return false;
		
		int wordLength = (strchr(p, ' ')) ? strchr(p, ' ') - p : strlen(p);
		char word[MAX_SAY_TEXT];

		strncpy_s(word, MAX_SAY_TEXT, p, wordLength);
		end = &word[wordLength-1];
		
		if (*end == ':') {
			*end = 0;
			end--;
		}

		if (Q_IsColorString(end - 1)) {
			*(end-1) = 0;
		}

		for (int i=0 ; i<MAX_CLIENTS ; i++)
		{
			player_t *plyr = &eth32.cg.players[i];
			if (*plyr->infoValid && (strstr(word, plyr->name) || strstr(plyr->name, word))) {
				strcpy_s(privmsgTarget, 128, plyr->name);
				SetTarget(CHAT_PM, khook);
				return true;
			}
		}
	}
	return false;
}

// Utility text function
void funcRouteChat(const char *msg)
{
	char buf[MAX_STRING_CHARS];
	chattype_t type = Gui.GetChatTarget();

	switch (type)
	{
	case CHAT_SAY:
		sprintf_s(buf, sizeof(buf), "say \"%s\"\n", msg);
		Gui.BufferConsoleCommand(buf);
		break;
	case CHAT_TEAM:
		sprintf_s(buf, sizeof(buf), "say_team \"%s\"\n", msg);
		Gui.BufferConsoleCommand(buf);
		break;
	case CHAT_FTEAM:
		sprintf_s(buf, sizeof(buf), "say_buddy \"%s\"\n", msg);
		Gui.BufferConsoleCommand(buf);
		break;
	case CHAT_IRC:
		Irc.ParseUserInput(msg);
		break;
	case CHAT_PM:
		Gui.PrivateMessage(msg);
		break;
	default:
#ifdef ETH32_DEBUG
		Debug.Log("funcRouteChat() - Chat type %i not valid.", type);
#endif
		break;
	}
}


