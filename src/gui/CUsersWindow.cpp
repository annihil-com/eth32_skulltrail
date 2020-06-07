// ETH32 - an Enemy Territory cheat for windows
// Copyright (c) 2007 eth32 team
// www.cheatersutopia.com & www.nixcoders.org

#include "../eth32.h"

// helper funcs

void funcJoinPortalUser(void)
{
	for (int i=0; i<Portal.numUsers ; i++)
	{
		if (!strcmp(eth32.cg.selectedPortalUser, Portal.users[i].username)) {
			char buf[42];
			sprintf_s(buf,42,"connect %s", Portal.users[i].server);
			Gui.BufferConsoleCommand(buf);
			Gui.SetView(GUI_MAINVIEW);
			//Syscall.CG_SendConsoleCommand(va("connect %s", Portal.users[i].server));
			return;
		}
	}
}

void funcAddPortalFriend(void)
{
	Portal.AddFriend(eth32.cg.selectedPortalUser);
}

void funcRemPortalFriend(void)
{
	Portal.RemoveFriend(eth32.cg.selectedPortalUser);
}

/*********************
* CUsersWindow
*********************/

#define USER_HEIGHT		(GUI_FONTHEIGHT + GUI_FONTHEIGHT + 15)
#define USER_SPACING	((int)(4 * eth32.cg.screenYScale))

CUsersWindow::CUsersWindow(char *wlabel, int wx, int wy, int ww, int wh)
: CWindow(wlabel, wx, wy, ww, wh)
{
	numDisplay = (h - GUI_TITLEBAR_SIZE - (GUI_SPACING * 4)) / (USER_HEIGHT + USER_SPACING);

	ctrlScrollbar = new CScrollBar("scrollbar", ww-5-GUI_SCROLLBAR_WIDTH, 5, wh - GUI_TITLEBAR_SIZE - 10, numDisplay, 0);
	ctrlScrollbar->Resize(ctrlScrollbar->Width(), (numDisplay * (USER_HEIGHT + USER_SPACING)) - USER_SPACING);
	AddControl((CControl*)ctrlScrollbar);

	ctrlJoin = new CButton("Join", ww - 105, wh - 17, 100, 12, funcJoinPortalUser);
	ctrlJoin->SetOrigin(x, y);
	ctrlAddFriend = new CButton("Add Friend", ww - 210, wh - 17, 100, 12, funcAddPortalFriend);
	ctrlAddFriend->SetOrigin(x, y);
	ctrlRemFriend = new CButton("Remove Friend", ww - 210, wh - 17, 100, 12, funcRemPortalFriend);
	ctrlRemFriend->SetOrigin(x, y);

	strcpy_s(username, 64, Portal.GetUsername());
}

CUsersWindow::~CUsersWindow(void)
{
	delete ctrlJoin;
	delete ctrlAddFriend;
	delete ctrlRemFriend;
}

void CUsersWindow::DrawUser(int drawX, int drawY, int width, int height, eth32user_t *usr, bool selected)
{
	int markerWidth = height >> 1;

	if (!usr->anon && !strcmp(eth32.server, usr->server)) {
		Syscall.R_SetColor(colorGreen);
		Draw.RawPic(drawX, drawY, markerWidth, height, eth32.cg.media.portalMarker);
		if (usr->frnd) {
			Syscall.R_SetColor(colorYellow);
			Draw.RawPic(drawX+4, drawY+4, markerWidth-8, height-8, eth32.cg.media.portalMarker);
		}
		Syscall.R_SetColor(NULL);
	} else if (usr->frnd) {
		Syscall.R_SetColor(colorYellow);
		Draw.RawPic(drawX, drawY, markerWidth, height, eth32.cg.media.portalMarker);
		Syscall.R_SetColor(NULL);
	} else {
		Syscall.R_SetColor(colorBlue);
		Draw.RawPic(drawX, drawY, markerWidth, height, eth32.cg.media.portalMarker);
		Syscall.R_SetColor(NULL);
	}

	

	drawX += markerWidth + GUI_SPACING;
	width -= markerWidth + GUI_SPACING;

	qhandle_t shader = (selected) ? eth32.guiAssets.btnselLeft : eth32.guiAssets.btnLeft;

	Draw.RawPicST(drawX, drawY, 12, 6, 0, 0, 1, 0.5, shader);
	Draw.RawPicST(drawX, drawY+6, 12, height-12, 0, .4, 1, 0.6, shader);
	Draw.RawPicST(drawX, drawY+height-6, 12, 6, 0, 0.5, 1, 1, shader);

	shader = (selected) ? eth32.guiAssets.btnselCenter : eth32.guiAssets.btnCenter;

	Draw.RawPicST(drawX + 12, drawY, width - 24, 6, 0, 0, 1, 0.5, shader);
	Draw.RawPicST(drawX + 12, drawY+6, width - 24, height-12, 0, 0.4, 1, 0.6, shader);
	Draw.RawPicST(drawX + 12, drawY+height-6, width - 24, 6, 0, 0.5, 1, 1, shader);

	shader = (selected) ? eth32.guiAssets.btnselRight : eth32.guiAssets.btnRight;

	Draw.RawPicST(drawX + width - 12, drawY, 12, 6, 0, 0, 1, 0.5, shader);
	Draw.RawPicST(drawX + width - 12, drawY+6, 12, height-12, 0, 0.4, 1, 0.6, shader);
	Draw.RawPicST(drawX + width - 12, drawY+height-6, 12, 6, 0, 0.5, 1, 1, shader);

	int textX = drawX + 5;
	int textY = drawY + 5;
	width -= 10;

	Draw.Text(textX, textY, GUI_FONTSCALE, "User:", GUI_FONTCOLOR2, qtrue, qfalse, GUI_FONT, true);
	Draw.Text(textX + TEXTWIDTH("User: "), textY, GUI_FONTSCALE, usr->username, GUI_FONTCOLOR1, qtrue, qfalse, GUI_FONT, true);

	textX += width*0.35f;
	Draw.Text(textX, textY, GUI_FONTSCALE, "Server:", GUI_FONTCOLOR2, qtrue, qfalse, GUI_FONT, true);
	Draw.Text(textX + TEXTWIDTH("Server: "), textY, GUI_FONTSCALE, usr->serverName, GUI_FONTCOLOR1, qtrue, qfalse, GUI_FONT, true);
	
	textX += width*0.45f;
	float *compatColor = ((eth32.et->ver == usr->etVersion) || (eth32.et->ver >= ET_260 && usr->etVersion >= ET_260)) ? colorGreen : colorRed;
	Draw.Text(textX, textY, GUI_FONTSCALE, "ET Ver:", GUI_FONTCOLOR2, qtrue, qfalse, GUI_FONT, true);
	Draw.Text(textX + TEXTWIDTH("ET Ver: "), textY, GUI_FONTSCALE, Tools.GetEtVersionString(usr->etVersion), compatColor, qtrue, qfalse, GUI_FONT, true);

	textX = drawX + 5;
	textY += GUI_FONTHEIGHT + 5;

	if (usr->player && *usr->player->infoValid) {
		Draw.Text(textX, textY, GUI_FONTSCALE, "Player:", GUI_FONTCOLOR2, qtrue, qfalse, GUI_FONT, true);
		Draw.Text(textX + TEXTWIDTH("Player: "), textY, GUI_FONTSCALE, usr->player->name, GUI_FONTCOLOR1, qfalse, qfalse, GUI_FONT, true);
	}

	textX += width*0.35f;
	Draw.Text(textX, textY, GUI_FONTSCALE, "Map:", GUI_FONTCOLOR2, qtrue, qfalse, GUI_FONT, true);
	Draw.Text(textX + TEXTWIDTH("Map: "), textY, GUI_FONTSCALE, usr->serverMapName, GUI_FONTCOLOR1, qtrue, qfalse, GUI_FONT, true);

	textX += width*0.45f;
	Draw.Text(textX, textY, GUI_FONTSCALE, "Mod:", GUI_FONTCOLOR2, qtrue, qfalse, GUI_FONT, true);
	Draw.Text(textX + TEXTWIDTH("Mod: "), textY, GUI_FONTSCALE, Tools.GetModVersionString(usr->modVersion), GUI_FONTCOLOR1, qtrue, qfalse, GUI_FONT, true);
}

void CUsersWindow::Display(void)
{
	ctrlScrollbar->SetNumCurrentItems(Portal.numUsers);
	CWindow::Display();

	if (!Portal.numUsers) {
		Draw.TextCenter(x + (w >> 1), y + (h >> 1), GUI_FONTSCALE, "No other users available.", GUI_FONTCOLOR2, qtrue, qfalse, GUI_FONT, true);
		return;
	}
	int drawX = x + GUI_SPACING;
	int drawY = y + GUI_TITLEBAR_SIZE + GUI_SPACING;
	int width = (ctrlScrollbar->IsActive()) ? w - (GUI_SPACING * 3) - ctrlScrollbar->Width() : w - (GUI_SPACING * 2);

	eth32user_t *usr = Portal.users;

	int startnum = (Portal.numUsers > numDisplay) ? ctrlScrollbar->GetCurrentItemNum() : 0;
	int endnum = (Portal.numUsers > numDisplay) ? startnum + numDisplay : Portal.numUsers;

	for (int i=startnum ; i<endnum ; i++) {
		bool selected = (eth32.cg.selectedPortalUser[0] && !strcmp(eth32.cg.selectedPortalUser, usr[i].username));
		DrawUser(drawX, drawY, width, USER_HEIGHT, &usr[i], selected);
		drawY += USER_HEIGHT + USER_SPACING;
	}

	char buf[64];
	sprintf_s(buf,64,"^7Total users online: ^3%i", Portal.numUsers);
	Draw.Text(x + GUI_SPACING, y2 - GUI_FONTHEIGHT - 5, GUI_FONTSCALE, buf, colorWhite, qfalse, qfalse, GUI_FONT, true);

	if (CanJoinSelected())
		ctrlJoin->Display();

	if (eth32.cg.selectedPortalUser[0] && strcmp(eth32.cg.selectedPortalUser, username)) {
		if (Portal.IsFriend(eth32.cg.selectedPortalUser))
			ctrlRemFriend->Display();
		else
			ctrlAddFriend->Display();
	}
}

int CUsersWindow::ProcessMouse(int mx, int my, UINT mevent, CControl **mhook, CControl **khook)
{
	if (INBOUNDS(mx,my)) {
		if (!ctrlScrollbar->ProcessMouse(mx, my, mevent, mhook, khook))
			return 0;

		if (eth32.cg.selectedPortalUser[0] && strcmp(eth32.cg.selectedPortalUser, username)) {
			bool ret;
			if (Portal.IsFriend(eth32.cg.selectedPortalUser))
				ret = !ctrlRemFriend->ProcessMouse(mx, my, mevent, mhook, khook);
			else
				ret = !ctrlAddFriend->ProcessMouse(mx, my, mevent, mhook, khook);

			if (ret)
				return 0;
		}

		if (CanJoinSelected() && !ctrlJoin->ProcessMouse(mx, my, mevent, mhook, khook))
			return 0;

		int width = (ctrlScrollbar->IsActive()) ? w - (GUI_SPACING * 3) - ctrlScrollbar->Width() : w - (GUI_SPACING * 2);
		int height = ((USER_HEIGHT + USER_SPACING) * numDisplay) - USER_SPACING;
		int userX = x + GUI_SPACING;
		int userY = y + GUI_TITLEBAR_SIZE + GUI_SPACING;

		if (INBOUNDS2(mx,my,userX,userY,(userX+width),(userY+height)) && mevent == WM_LBUTTONDOWN) {
			int num = (my - userY) / (USER_HEIGHT + USER_SPACING);

			if (ctrlScrollbar->IsActive())
				num += ctrlScrollbar->GetCurrentItemNum();
			else if (num >= Portal.numUsers) { // prevents clicking on blank space when only a few users on screen
				eth32.cg.selectedPortalUser[0] = 0;
				return 0;
			}

			if (num < 0 || num >= MAX_PORTAL_USERS) // sanity check
				return 0;

			if (((my - userY) % (USER_HEIGHT + USER_SPACING)) <= USER_HEIGHT) {
				// set selected username here
				strcpy_s(eth32.cg.selectedPortalUser, 64, Portal.users[num].username);
#ifdef ETH32_DEBUG
				Debug.Log("Portal user select: [%i] %s", num, Portal.users[num].username);
#endif
			}
			else
				eth32.cg.selectedPortalUser[0] = 0;
		}		

		return 0;
	}

	return 1;
}

int CUsersWindow::MouseWheel(int mx, int my, int wheel)
{
	if (INBOUNDS(mx,my)) {
		if (!Portal.numUsers)
			return 0;

		ctrlScrollbar->AdjCurrentItemNum(-wheel);
		return 0;
	}
	return 1;
}

bool CUsersWindow::CanJoinSelected(void)
{
	// no user selected
	if (!eth32.cg.selectedPortalUser[0])
		return false;

	// don't allow join self :f
	if (!strcmp(eth32.cg.selectedPortalUser, Portal.GetUsername()))
		return false;

	eth32user_t *usr;
	for (int i=0 ; i<Portal.numUsers ; i++)
	{
		usr = &Portal.users[i];
		if (!strcmp(eth32.cg.selectedPortalUser, usr->username)) {
			if (usr->anon || !strcmp(eth32.server, usr->server)) {
				return false; // user is alread on same server or anonymous
			}
			if (usr->etVersion == eth32.et->ver || (usr->etVersion >= ET_260 && eth32.et->ver >= ET_260)) {
				return true; // version is the same... good to go
			}
			else {
				return false; // version conflict
			}
		}
	}
	return false; // user not found
}