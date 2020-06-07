// ETH32 - an Enemy Territory cheat for windows
// Copyright (c) 2007 eth32 team
// www.cheatersutopia.com & www.nixcoders.org

#include "../eth32.h"

/***********************
* CIrcUserList
***********************/

CIrcUserList::CIrcUserList(int cx, int cy, int cw, int ch)
: CControl("userlist", cx, cy, cw, ch)
{
	userlist.clear();
	scrollbar = new CScrollBar("scrollbar", cw - GUI_SCROLLBAR_WIDTH - GUI_SPACING, GUI_SPACING, ch - (GUI_SPACING * 2), 0, 0, true);
	scrollbar->SetOrigin(x, y);
	scrollbar->SetMaxDisplayItems( (h - 10) / (GUI_FONTHEIGHT + GUI_TEXTSPACING));
	userCount = 0;
}

CIrcUserList::~CIrcUserList(void)
{
	userlist.clear();
	delete scrollbar;
}

void CIrcUserList::Display(void)
{
	int yOff = 0;

	Draw.RawPic(x, y, GUI_ESIZE, GUI_ESIZE, eth32.guiAssets.winTopLeft);
	Draw.RawPic(x + GUI_ESIZE, y, w - (GUI_ESIZE+GUI_ESIZE), GUI_ESIZE, eth32.guiAssets.winTop);
	Draw.RawPicST(x2 - GUI_ESIZE, y, GUI_ESIZE, GUI_ESIZE, 1.f, 0.f, 0.f, 1.f, eth32.guiAssets.winTopLeft);

	yOff += GUI_ESIZE;

	Draw.RawPic(x, y + yOff, GUI_ESIZE, h - GUI_ESIZE - yOff, eth32.guiAssets.winLeft);
	Draw.RawPic(x + GUI_ESIZE, y + yOff, w - (GUI_ESIZE+GUI_ESIZE), h - GUI_ESIZE - yOff, eth32.guiAssets.winCenter);
	Draw.RawPicST(x2 - GUI_ESIZE, y + yOff, GUI_ESIZE, h - GUI_ESIZE - yOff, 1.f, 0.f, 0.f, 1.f, eth32.guiAssets.winLeft);

	Draw.RawPicST(x, y2 - GUI_ESIZE, GUI_ESIZE, GUI_ESIZE, 0.f, 1.f, 1.f, 0.f, eth32.guiAssets.winTopLeft);
	Draw.RawPicST(x + GUI_ESIZE, y2 - GUI_ESIZE, w - (GUI_ESIZE+GUI_ESIZE), GUI_ESIZE, 0.f, 1.f, 1.f, 0.f, eth32.guiAssets.winTop);
	Draw.RawPicST(x2 - GUI_ESIZE, y2 - GUI_ESIZE, GUI_ESIZE, GUI_ESIZE, 1.f, 1.f, 0.f, 0.f, eth32.guiAssets.winTopLeft);

	if (!userlist.empty()) {
		int drawX = x+5;
		int drawY = y+5;

		list<ircuser_t*>::iterator user_iter = userlist.begin();

		char nameBuffer[64];
		char tempColor;
		int charCount;
		int breakWidth = (scrollbar->IsActive()) ? w - 10 - scrollbar->Width() - GUI_SPACING : w - 10;

		if (scrollbar->IsActive()) {
			int startPos = scrollbar->GetCurrentItemNum();
			while (startPos != 0) {
				startPos--;
				user_iter++;
			}
		}
		
		while (user_iter != userlist.end() && (drawY + GUI_FONTHEIGHT + 5) < y2)
		{
			charCount = Draw.BreakStringByWidth((*user_iter)->display, breakWidth, &tempColor, GUI_FONT, false);

			if (charCount < strlen((*user_iter)->display)) {
				strncpy_s(nameBuffer, 64, (*user_iter)->display, charCount);
				Draw.Text(drawX, drawY, GUI_FONTSCALE, nameBuffer, GUI_FONTCOLOR1, qtrue, qfalse, GUI_FONT, true);
			}
			else Draw.Text(drawX, drawY, GUI_FONTSCALE, (*user_iter)->display, GUI_FONTCOLOR1, qtrue, qfalse, GUI_FONT, true);
			
			drawY += GUI_FONTHEIGHT + 5;

			user_iter++;
		}
		scrollbar->Display();
	}
}

int CIrcUserList::ProcessMouse(int mx, int my, UINT mevent, CControl **mhook, CControl **khook)
{
	return scrollbar->ProcessMouse(mx, my, mevent, mhook, khook);
}

void CIrcUserList::Resize(int cw, int ch)
{
	CControl::Resize(cw, ch);

	scrollbar->Reposition(w - scrollbar->Width() - GUI_SPACING, GUI_SPACING);
	scrollbar->Resize(scrollbar->Width(), h - GUI_SPACING - GUI_SPACING);
	scrollbar->SetOrigin(x, y);
	scrollbar->SetMaxDisplayItems( (h - 10) / (GUI_FONTHEIGHT + 5));
}

void CIrcUserList::Reposition(int cx, int cy)
{
	CControl::Reposition(cx, cy);
	
	scrollbar->SetOrigin(x, y);
}

void CIrcUserList::SetOrigin(int ox, int oy)
{
	CControl::SetOrigin(ox, oy);

	scrollbar->SetOrigin(x, y);
}

ircuser_t* CIrcUserList::CreateIrcUser(const char *nick)
{
	ircuser_t *user = new ircuser_t;
	user->opStatus = 0;

	bool adjNick = false;

	if (nick[0] == '~') {
		user->opStatus = IRCUSER_OWNER;
		adjNick = true;
	}
	else if (nick[0] == '&') {
		user->opStatus = IRCUSER_PROTECTED;
		adjNick = true;
	}
	else if (nick[0] == '@') {
		user->opStatus = IRCUSER_OP;
		adjNick = true;
	}
	else if (nick[0] == '%') {
		user->opStatus = IRCUSER_HALFOP;
		adjNick = true;
	}
	else if (nick[0] == '+') {
		user->opStatus = IRCUSER_VOICE;
		adjNick = true;
	}
	else user->opStatus = IRCUSER_NORMAL;
	
	if (adjNick) {
		user->nick = new char[strlen(&nick[1])+1];
		strcpy_s(user->nick, strlen(&nick[1])+1, &nick[1]);
	}
	else {
		user->nick = new char[strlen(nick)+1];
		strcpy_s(user->nick, strlen(nick)+1, nick);
	}
	user->display = new char[strlen(nick)+1];
	strcpy_s(user->display, strlen(nick)+1, nick);

	return user;
}	

ircuser_t* CIrcUserList::GetIrcUser(const char *nick)
{
	if (userlist.empty())
		return NULL;

	for (list<ircuser_t*>::iterator user = userlist.begin() ; user != userlist.end() ; user++)
	{
		if (!strcmp((*user)->nick, nick))
			return *user;
	}
	// not found
	return NULL;
}

void CIrcUserList::DestroyIrcUser(ircuser_t *user)
{
	if (user) {
		delete [] user->nick;
		delete [] user->display;
		delete user;
	}
}

void CIrcUserList::InsertUser(ircuser_t *user)
{
	if (userlist.empty() || UserHigher(user, userlist.front())) {
		userlist.push_front(user);
		return;
	}

	list<ircuser_t*>::iterator user_next;
	for (list<ircuser_t*>::iterator user_iter = userlist.begin() ; user_iter != userlist.end() ; user_iter++)
	{
		user_next = user_iter;
		user_next++;
		if (user_next != userlist.end()) {
			if (UserHigher(user, *user_next)) {
				userlist.insert(user_next, user);
				return;
			}
		}
		else {
			userlist.push_back(user);
			return;
		}
	}
}

void CIrcUserList::RemoveUser(ircuser_t *user)
{
	for (list<ircuser_t*>::iterator user_iter = userlist.begin() ; user_iter != userlist.end() ; user_iter++)
	{
		if (*user_iter == user) {
			userlist.erase(user_iter);
			return;
		}
	}
}

bool CIrcUserList::UserHigher(ircuser_t *thisUser, ircuser_t *otherUser)
{
	if (thisUser->opStatus > otherUser->opStatus)
		return true;

	if (thisUser->opStatus == otherUser->opStatus) 
		return (_strcmpi(thisUser->nick, otherUser->nick) < 0);

	return false;
}

bool CIrcUserList::AddUser(const char *nick)
{
	if (!nick || !*nick) 
		return false;

	ircuser_t *user = CreateIrcUser(nick);
	InsertUser(user);
	userCount++;
	scrollbar->SetNumCurrentItems(userCount);
	return true;
}

bool CIrcUserList::RemoveUser(const char *nick)
{
	if (!userlist.empty() && nick && *nick) {
		for (list<ircuser_t*>::iterator user_iter = userlist.begin() ; user_iter != userlist.end() ; user_iter++)
		{
			if (!strcmp(nick, (*user_iter)->nick)) {
				DestroyIrcUser(*user_iter);
				userlist.erase(user_iter);
				userCount--;
				scrollbar->SetNumCurrentItems(userCount);
				return true;

			}
		}
	}
	return false;
}

bool CIrcUserList::ChangeUserMode(const char *nick, const char *modeStr)
{
	ircuser_t *user = GetIrcUser(nick);

	// user not found
	if (!user)
		return false;

	bool opAdd = (modeStr[0] == '+');

	for (int k=1 ; modeStr[k] ; k++)
	{
		unsigned int mode = 0;

		switch (modeStr[k])
		{
		case 'q':
			mode = IRCUSER_OWNER;
			break;
		case 'a':
			mode = IRCUSER_PROTECTED;
			break;
		case 'o':
			mode = IRCUSER_OP;
			break;
		case 'h':
			mode = IRCUSER_HALFOP;
			break;
		case 'v':
			mode = IRCUSER_VOICE;
			break;
		// other types can be added here, though I don't know them off hand
		}

		if (opAdd) 
			user->opStatus |= mode;
		else 
			user->opStatus &= ~mode;
	}

	if (user->opStatus & ~IRCUSER_NORMAL) // something other than normal user
		user->opStatus &= ~IRCUSER_NORMAL;

	FixDisplayNick(user);
	// remove user and reinsert in proper order
	RemoveUser(user);
	InsertUser(user);

	return true;
}

void CIrcUserList::FixDisplayNick(ircuser_t *user)
{
	delete [] user->display;

	if (user->opStatus & IRCUSER_NORMAL) {
		user->display = new char[strlen(user->nick)+1];
		strcpy_s(user->display, strlen(user->nick)+1, user->nick);
		return;
	}

	char opChar = 0;
	
	if (user->opStatus & IRCUSER_OWNER)
		opChar = '~';
	else if (user->opStatus & IRCUSER_PROTECTED)
		opChar = '&';
	else if (user->opStatus & IRCUSER_OP)
		opChar = '@';
	else if (user->opStatus & IRCUSER_HALFOP)
		opChar = '%';
	else if (user->opStatus & IRCUSER_VOICE)
		opChar = '+';
	
	if (opChar) {
		user->display = new char[strlen(user->nick)+2];
		user->display[0] = opChar;
		strcpy_s(&user->display[1], strlen(user->nick)+1, user->nick);
	}
	else {  // not sure what this is, so assign nick like normal user
		user->display = new char[strlen(user->nick)+1];
		strcpy_s(user->display, strlen(user->nick)+1, user->nick);
	}
}

bool CIrcUserList::NickChange(const char *oldNick, const char *newNick)
{
	for (list<ircuser_t*>::iterator user_iter = userlist.begin() ; user_iter != userlist.end() ; user_iter++)
	{
		if (!strcmp(oldNick, (*user_iter)->nick)) {
			ircuser_t *user = *user_iter;
			char opChar = user->display[0];
			userlist.erase(user_iter);
			delete [] user->nick;
			delete [] user->display;
			user->nick = new char[strlen(newNick)+1];
			strcpy_s(user->nick, strlen(newNick)+1, newNick);
			if (user->opStatus & ~IRCUSER_NORMAL) {
				user->display = new char[strlen(newNick)+2];
				user->display[0] = opChar;
				strcpy_s(&user->display[1], strlen(newNick)+1, newNick);
			}
			else {
				user->display = new char[strlen(newNick)+1];
				strcpy_s(user->display, strlen(newNick)+1, newNick);
			}
			InsertUser(user);
			return true;
		}
	}
	return false;
}

/***********************
* CIrcChannelWindow
***********************/

CIrcChannelWindow::CIrcChannelWindow(const char *channel, int cx, int cy, int cw, int ch, bool allowUsers)
: CWindow(channel, cx, cy, cw, ch)
{
	int bufferWidth;

	if (allowUsers) {
		bufferWidth = (cw-15) * 0.75f;
		ctrlUserList = new CIrcUserList(bufferWidth + 10, 5, cw-15-bufferWidth, ch - GUI_TITLEBAR_SIZE - 10);
		AddControl(ctrlUserList);
	}
	else {
		bufferWidth = cw-10;
		ctrlUserList = NULL;
	}

	ctrlBuffer = new CTextBuffer("chanbuf", 5, 5, bufferWidth, ch - GUI_TITLEBAR_SIZE - 10, IRC_CHAN_BUFFER_SIZE);
	AddControl(ctrlBuffer);

	locked = true;
}

CIrcChannelWindow::~CIrcChannelWindow(void)
{
	//delete ctrlBuffer;
	//delete ctrlUserList;
}

/*void CIrcChannelWindow::Display(void)
{
	ctrlBuffer->Display();
	ctrlUserList->Display();
}*/

void CIrcChannelWindow::Resize(int cw, int ch)
{
	CControl::Resize(cw, ch);

	int bufferWidth;

	if (ctrlUserList) {
		bufferWidth = (w - (GUI_SPACING * 3)) * 0.75f;
		ctrlUserList->Resize(w - bufferWidth - (GUI_SPACING * 3), h - GUI_TITLEBAR_SIZE - (GUI_SPACING * 2));
	}
	else bufferWidth = w - (GUI_SPACING * 2);

	ctrlBuffer->Resize(bufferWidth, h - GUI_TITLEBAR_SIZE - (GUI_SPACING * 2));
}

bool CIrcChannelWindow::AddUser(const char *nick, bool announce)
{
	if (ctrlUserList) {
		bool ret = ctrlUserList->AddUser(nick);
		if (ret && announce) {
			char joinMsg[64];
			sprintf_s(joinMsg, 64, "* %s has joined the channel.", nick);
			AddMessage(joinMsg);
		}
		return true;
	}
	else 
		return false;
}

bool CIrcChannelWindow::RemoveUser(const char *nick, const char *msg)
{
	if (ctrlUserList) {
		bool ret = ctrlUserList->RemoveUser(nick);
		if (ret) {
			char rmvMsg[256];
			if (msg)
				sprintf_s(rmvMsg, 256, "* %s has left the channel. ( %s )", nick, msg);
			else sprintf_s(rmvMsg, 256, "* %s has left the channel.", nick);
			AddMessage(rmvMsg);
		}
		return ret;
	}
	else 
		return false;
}

bool CIrcChannelWindow::KickUser(const char *nick, const char *msg, const char *src)
{
	if (ctrlUserList) {
		bool ret = ctrlUserList->RemoveUser(nick);
		if (ret) {
			char rmvMsg[1024];
			if (msg)
				sprintf_s(rmvMsg, 1024, "* %s was kicked from the channel by %s. ( %s )", nick, src, msg);
			else
				sprintf_s(rmvMsg, 1024, "* %s was kicked from the channel by %s.", nick, src);
			AddMessage(rmvMsg);
		}
		return ret;
	}
	else 
		return false;
}

bool CIrcChannelWindow::UserQuit(const char *nick, const char *msg)
{
	if (ctrlUserList) {
		bool ret = ctrlUserList->RemoveUser(nick);
		if (ret) {
			char rmvMsg[256];
			if (msg)
				sprintf_s(rmvMsg, 256, "* %s has quit IRC. ( %s )", nick, msg);
			else sprintf_s(rmvMsg, 256, "* %s has quit IRC.", nick);
			AddMessage(rmvMsg);
		}
		return ret;
	}
	else 
		return false;
}


void CIrcChannelWindow::NickChange(const char *oldNick, const char *newNick)
{
	if (ctrlUserList) {
		if (ctrlUserList->NickChange(oldNick, newNick)){
			char buf[MAX_STRING_CHARS];
			sprintf_s(buf,sizeof(buf),"* %s is now known as %s", oldNick, newNick);
			AddMessage(buf);
		}
	}
}

void CIrcChannelWindow::ChangeUserMode(const char *opNick, const char *nick, const char *mode)
{
	if (!nick || !*nick || !mode || !*mode || !ctrlUserList)
		return;

	if (!ctrlUserList->ChangeUserMode(nick, mode))
		return;
}

void CIrcChannelWindow::AddMessage(const char *msg)
{
	ctrlBuffer->AddText(msg);
}

void CIrcChannelWindow::SetChannelButton(CIrcChannelButton *button)
{
	ctrlChannelButton = button;
}

/***********************
* CIrcChannelButton
***********************/

CIrcChannelButton::CIrcChannelButton(const char *chanName, int cx, int cy, int cw, int ch, CIrcWindow *ircWindow)
: CButton(chanName, cx, cy, cw, ch, NULL)
{
	channelName = new char[strlen(chanName)+1];
	strcpy_s(channelName, strlen(chanName)+1, chanName);

	winIrc = ircWindow;

	char lastColor;
	int numChar = Draw.BreakStringByWidth(channelName, w-10, &lastColor, GUI_FONT, false);

	if (numChar < strlen(channelName)) {
		char tempLabel[128];
		strncpy_s(tempLabel, 128, channelName, numChar);
		SetLabel(tempLabel);
	}

	newMessages = false;
	activeChannel = false;
}

CIrcChannelButton::~CIrcChannelButton(void)
{
	delete [] channelName;
}	

int CIrcChannelButton::ProcessMouse(int mx, int my, UINT mevent, CControl **mhook, CControl **khook)
{
	if (INBOUNDS(mx,my)) {
		if (mevent == WM_LBUTTONDOWN) {
			selected = true;
			*mhook = this;
		}
		else if (mevent == WM_LBUTTONUP && selected) {
			if (winIrc)
				winIrc->SetActiveChannel(channelName);
			newMessages = false;
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

void CIrcChannelButton::Display(void)
{
	if (selected || activeChannel) {
		Draw.RawPic(x, y, h, h, eth32.guiAssets.btnselLeft);
		Draw.RawPic(x + h, y, w-(h+h), h, eth32.guiAssets.btnselCenter);
		Draw.RawPic(x2 - h, y, h, h, eth32.guiAssets.btnselRight);
	}
	else {
		Draw.RawPic(x, y, h, h, eth32.guiAssets.btnLeft);
		Draw.RawPic(x + h, y, w-(h+h), h, eth32.guiAssets.btnCenter);
		Draw.RawPic(x2 - h, y, h, h, eth32.guiAssets.btnRight);
	}

	if (newMessages) {
		vec4_t flashColor;

		VectorCopy(GUI_FONTCOLOR1, flashColor);
		flashColor[3] = sin((timeGetTime() % IRC_CHAN_FLASH_TIME) * M_PI / IRC_CHAN_FLASH_TIME);
		Draw.TextCenter(x + (w >> 1), y + 2, GUI_FONTSCALE, label, flashColor, qtrue, qfalse, GUI_FONT, true);
	}
	else Draw.TextCenter(x + (w >> 1), y + 2, GUI_FONTSCALE, label, GUI_FONTCOLOR1, qtrue, qfalse, GUI_FONT, true);
}

void CIrcChannelButton::NewMessage(bool newmsg)
{
	newMessages = newmsg;
}

/***********************
* CIrcWindow
***********************/

CIrcWindow::CIrcWindow(char *wlabel, int wx, int wy, int ww, int wh)
: CWindow(wlabel, wx, wy, ww, wh)
{
	showTitle = false; 

	ctrlInput = new CTextInput("ircinput",	5, wh-20, ww-10, 15, IRC_MAX_MSG_LEN, funcIrcUserCommand);

	AddControl((CControl*)ctrlInput);

	// set input behavior to keep keybd focus even after enter is pressed
	ctrlInput->KeepFocus(true);

	// don't allow user to move or minimize this window, no reason to allow it
	locked = true;

	activeChannel = NULL;

	channelList.clear();
	channelButtons.clear();

	AddChannel("Status", false);
}

void CIrcWindow::Display(void)
{
	CWindow::Display();

	if (activeChannel) 
		activeChannel->Display();

	if (!channelButtons.empty()) {
		for (list<CIrcChannelButton*>::iterator but_iter = channelButtons.begin() ; but_iter != channelButtons.end() ; but_iter++)
		{
			(*but_iter)->Display();
		}
	}
}

int CIrcWindow::ProcessMouse(int mx, int my, UINT mevent, CControl **mhook, CControl **khook)
{
	if (INBOUNDS(mx,my)) {
		if (activeChannel) {
			if (activeChannel->ProcessMouse(mx, my, mevent, mhook, khook) == 0) 
				return 0;
		}

		if (!channelButtons.empty()) {
			for (list<CIrcChannelButton*>::iterator but_iter = channelButtons.begin() ; but_iter != channelButtons.end() ; but_iter++) {
				if ((*but_iter)->ProcessMouse(mx, my, mevent, mhook, khook) == 0)
					return 0;
			}
		}

		// mouse was within this window, event handled
		return 0;
	}
	return 1;
}

int CIrcWindow::ProcessKeybd(unsigned char key, CControl **khook)
{
	// tab through available channel windows
	if (key == 0x09) {	// TAB
		if (channelList.size() > 1 && activeChannel) {
			list<CIrcChannelWindow*>::iterator chan_iter = channelList.begin();
			
			while (*chan_iter != activeChannel && chan_iter != channelList.end())
				chan_iter++;

			chan_iter++;

			if (chan_iter == channelList.end())
				chan_iter = channelList.begin();
				
			char buf[64];
			(*chan_iter)->GetLabel(buf,sizeof(buf));
			SetActiveChannel(buf);
			return 0;
		}
		return 0;
	}
	// if not tab, pass the input to the input control
	else if (ctrlInput) {
		ctrlInput->ProcessKeybd(key, khook);
		return 0;
	}

	return 1;
}

void CIrcWindow::SetKeybdFocus(bool set)
{
	CWindow::SetKeybdFocus(set);

	if (ctrlInput)
		ctrlInput->SetKeybdFocus(set);
}

CIrcChannelWindow* CIrcWindow::GetChannelWindowByName(const char *channelName)
{
	if (channelList.empty())
		return NULL;

	char buf[64];
	for (list<CIrcChannelWindow*>::iterator chan_iter = channelList.begin() ; chan_iter != channelList.end() ; chan_iter++)
	{
		(*chan_iter)->GetLabel(buf,sizeof(buf));
		if (!strcmp(channelName, buf))
			return *chan_iter;
	}

	return NULL;
}

bool CIrcWindow::AddChannel(const char *channelName, bool allowUsers)
{
	if (GetChannelWindowByName(channelName) != NULL){ // channel by that name already exists
		return false;
	}

	if (channelList.size() == IRC_MAX_CHANNELS){ // we already have max channels open
		return false;
	}

	int fakeWidth = w / eth32.cg.screenXScale;
	int fakeHeight = h / eth32.cg.screenYScale;

	CIrcChannelWindow *channelWin = new CIrcChannelWindow(channelName, x + 5, y + 5, fakeWidth - 10, fakeHeight - 40, allowUsers);
	CIrcChannelButton *channelBut = new CIrcChannelButton(channelName, x, y, IRC_CHAN_BUTTON_WIDTH, 12, this);

	channelBut->SetOrigin(x, y);

	channelList.push_back(channelWin);
	InsertButton(channelBut);

	channelWin->SetChannelButton(channelBut);

	SetActiveChannel(channelName);

	Reorganize();

	return true;
}

bool CIrcWindow::RemoveChannel(const char *channelName)
{
	if (!strcmp(channelName, "Status"))		// never allowed to remove status window
		return false;

	CIrcChannelWindow *chanWindow = GetChannelWindowByName(channelName);

	if (chanWindow == NULL) // channel doesn't exist
		return false;

	CIrcChannelButton *chanButton = chanWindow->GetChannelButton();

	for (list<CIrcChannelWindow*>::iterator chan_iter = channelList.begin() ; chan_iter != channelList.end() ; chan_iter++)
	{
		if (chanWindow == *chan_iter) {
			delete (chanWindow);
			channelList.erase(chan_iter);
			RemoveButton(chanButton);
			Reorganize();

			if (activeChannel == chanWindow)
				SetActiveChannel("Status");

			return true;
		}
	}
	return false;
}

bool CIrcWindow::RemoveAllChannels(void)
{
	// no channels to remove
	if (channelList.empty())
		return false;

	char buf[64];
	for (list<CIrcChannelWindow*>::iterator chan_iter = channelList.begin() ; chan_iter != channelList.end() ; chan_iter++)
	{
		CIrcChannelWindow *chanWindow = *chan_iter;
		chanWindow->GetLabel(buf,sizeof(buf));
		if (strcmp(buf, "Status"))
		{
			CIrcChannelButton *chanButton = chanWindow->GetChannelButton();
			delete (chanWindow);
			channelList.erase(chan_iter);
			RemoveButton(chanButton);
		}
	}
	SetActiveChannel("Status");
	Reorganize();
	return true;
}


bool CIrcWindow::InsertButton(CIrcChannelButton *btn)
{
	if (channelButtons.empty()) {
		channelButtons.push_front(btn);
		return true;
	}

	list<CIrcChannelButton*>::iterator but_next;

	char btnLbl[64], itrLbl[64], nxtLbl[64];
	for (list<CIrcChannelButton*>::iterator but_iter = channelButtons.begin() ; but_iter != channelButtons.end() ; but_iter++)
	{
		but_next = but_iter;
		but_next++;
		if (but_next != channelButtons.end()) {
			btn->GetLabel(btnLbl,sizeof(btnLbl));
			(*but_iter)->GetLabel(itrLbl, sizeof(itrLbl));
			(*but_next)->GetLabel(nxtLbl,sizeof(nxtLbl));
			if (strcmp(btnLbl, itrLbl) >= 0 && strcmp(btnLbl, nxtLbl) <= 0) {
				channelButtons.insert(but_next, btn);
				return true;
			}
		}
		else {
			channelButtons.push_back(btn);
			return true;
		}
	}
	return false;
}

bool CIrcWindow::RemoveButton(CIrcChannelButton *btn)
{
	if (channelButtons.empty() || btn == NULL)
		return false;

	for (list<CIrcChannelButton*>::iterator but_iter = channelButtons.begin() ; but_iter != channelButtons.end() ; but_iter++)
	{
		if (btn == *but_iter) {
			delete btn;
			channelButtons.erase(but_iter);
			return true;
		}
	}
	return false;
}

bool CIrcWindow::AddUser(const char *nick, const char *channelName, bool announce)
{
	CIrcChannelWindow *chanWindow = GetChannelWindowByName(channelName);

	if (chanWindow == NULL)		// channel not found
		return false;
			
	return chanWindow->AddUser(nick, announce);
}

// used to remove the user from a specific channel
bool CIrcWindow::RemoveUser(const char *nick, const char *channelName, const char *msg)
{
	CIrcChannelWindow *chanWindow = GetChannelWindowByName(channelName);

	if (chanWindow == NULL)		// channel not found
		return false;

	return chanWindow->RemoveUser(nick, msg);
}

// used to remove the user completely (ex: on user QUIT)
void CIrcWindow::UserQuit(const char *nick, const char *msg)
{
	if (!channelList.empty()) {
		for (list<CIrcChannelWindow*>::iterator chan_iter = channelList.begin() ; chan_iter != channelList.end() ; chan_iter++)
		{
			(*chan_iter)->UserQuit(nick, msg);
		}
	}
}

void CIrcWindow::NickChange(const char *oldNick, const char *newNick)
{
	for (list<CIrcChannelWindow*>::iterator chan_iter = channelList.begin() ; chan_iter != channelList.end() ; chan_iter++)
	{
		(*chan_iter)->NickChange(oldNick, newNick);
	}
}

bool CIrcWindow::ChannelMessage(const char *channelName, const char *nick, const char *msg)
{
	CIrcChannelWindow *chanWindow = GetChannelWindowByName(channelName);

	if (chanWindow == NULL)		// channel not found
		return false;

	char ircmsg[1024];
	
	if (nick) {
		sprintf_s(ircmsg, sizeof(ircmsg), "^7%s^9: ^2%s", nick, msg);
		chanWindow->AddMessage(ircmsg);
	}
	else {
		chanWindow->AddMessage(msg);
	}

	if (chanWindow != activeChannel)
		chanWindow->GetChannelButton()->NewMessage(true);

	// no need to send all the status messages to chat window
	if (!strcmp(channelName, "Status") || !nick)
		return true;

	sprintf_s(ircmsg, 1024, "^9[^7%s^9] ^7%s^9: ^2%s\n", channelName, nick, msg);

	switch (eth32.settings.ircChatMsgs)
	{
	case IRC_ROUTING_OFF:
		break;
	case IRC_ROUTING_CURRENT:
		if (chanWindow != activeChannel)
			break;
	case IRC_ROUTING_ALL:
		Gui.ChatMessage(ircmsg);
		break;
	}

	switch (eth32.settings.ircConsoleMsgs)
	{
	case IRC_ROUTING_OFF:
		break;
	case IRC_ROUTING_CURRENT:
		if (chanWindow != activeChannel)
			break;
	case IRC_ROUTING_ALL:
		Gui.BufferConsoleMessage(ircmsg);
		break;
	}

	if (eth32.settings.ircMention && strcmp(nick, Irc.nick) && (strstr(msg, nick) || strstr(msg, Portal.GetUsername()))) {
		char msg[1024]; 
		sprintf_s(msg, sizeof(msg), "^7%s ^zmentioned you in ^7%s", nick, channelName);
		Gui.CenterPrint(msg, 10);
	}

	return true;
}

bool CIrcWindow::SetActiveChannel(const char *channelName)
{
	CIrcChannelWindow *chanWindow = GetChannelWindowByName(channelName);

	if (chanWindow == NULL)		// channel not found
		return false;

	if (chanWindow == activeChannel)	// channel already active
		return false;

	if (activeChannel)
		activeChannel->GetChannelButton()->SetActiveChannel(false);

	activeChannel = chanWindow;
	chanWindow->GetChannelButton()->SetActiveChannel(true);
	chanWindow->GetChannelButton()->NewMessage(false);
	return true;
}

/*bool CIrcWindow::SetActiveChannel(CIrcChannelWindow *chanWindow)
{
	if (chanWindow == NULL)		// passed us a NULL channel
		return false;

	activeChannel = chanWindow;
	return true;
}*/

void CIrcWindow::Reorganize(void)
{
	int space = 5 * eth32.cg.screenXScale;
	int setX = x + space;
	int setY = y + space;

	int realButtonWidth = IRC_CHAN_BUTTON_WIDTH * (eth32.game.glconfig->vidWidth / 640.f);
	int realButtonHeight = 12 * eth32.cg.screenYScale;

	if (!channelButtons.empty()) {
		for (list<CIrcChannelButton*>::iterator but_iter = channelButtons.begin() ; but_iter != channelButtons.end() ; but_iter++)
		{
			if ((setX + realButtonWidth + space) > w) {
				setX = x + space;
				setY += realButtonHeight + space;
			}
			(*but_iter)->Reposition(setX, setY);
			//(*but_iter)->SetOrigin(x, y);
			setX += realButtonWidth + space;
		}
		setY += realButtonHeight + space;
	}
	int height = h - setY - (12 * eth32.cg.screenYScale) - GUI_SPACING; 
	if (!channelList.empty()) {
		for (list<CIrcChannelWindow*>::iterator chan_iter = channelList.begin() ; chan_iter != channelList.end() ; ++chan_iter)
		{
			(*chan_iter)->Resize(w - (space << 1), height);
			(*chan_iter)->Reposition(x + space, setY);
		}
	}
}	

char* CIrcWindow::GetActiveChannel(char *s, size_t s_s)
{
	if (activeChannel){
		char buf[64];
		activeChannel->GetLabel(buf,sizeof(buf));
		strcpy_s(s, s_s, buf);
	} else
		s[0] = 0;

	return s;
}

// Helper Function
void funcIrcUserCommand(const char *cmd)
{
	Irc.ParseUserInput(cmd);
}