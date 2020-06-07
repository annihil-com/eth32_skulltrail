// ETH32 - an Enemy Territory cheat for windows
// Copyright (c) 2007 eth32 team
// www.cheatersutopia.com & www.nixcoders.org

#include "../eth32.h"

/********************
* CTextBuffer       *
********************/

CTextBuffer::CTextBuffer(char *clabel, int cx, int cy, int cw, int ch, int bufferSize)
: CControl(clabel, cx, cy, cw, ch)
{
	maxBuffer = bufferSize;
	inputLineCount = dispLineCount = 0;
	startLine = 0;
	inputHead = inputTail = NULL;
	dispHead = dispTail = dispCurrent = NULL;
	fontHeight = Draw.sizeTextHeight(GUI_FONTSCALE, GUI_FONT);
	currentId = 0;
	breakWidth = w - (GUI_SCROLLBAR_WIDTH * eth32.cg.screenXScale) - GUI_SPACING;

	scrollbar = new CScrollBar("bufscroll", cw - GUI_SCROLLBAR_WIDTH, 0, ch, GUI_SCROLLBAR_WIDTH, 0, false);
	scrollbar->SetOrigin(x, y);

	scrollbar->SetMaxDisplayItems(h / (GUI_FONTHEIGHT + GUI_TEXTSPACING));
}

CTextBuffer::~CTextBuffer(void)
{
	delete scrollbar;

	ClearBuffer();
}

void CTextBuffer::ClearBuffer(void)
{
	bufline_t *temp = inputHead;
	while (temp) 
	{
		inputHead = temp->next;
		delete [] temp->text;
		delete temp;
		temp = inputHead;
	}
	
	inputLineCount = dispLineCount = 0;
	inputHead = inputTail = NULL;
	dispHead = dispTail = dispCurrent = NULL;

	scrollbar->SetNumCurrentItems(0);
}


void CTextBuffer::Display(void)
{
	int textY = y2 - (GUI_FONTHEIGHT + GUI_TEXTSPACING);

	bufline_t *bufferLine = dispCurrent;

	int curPos = 0;
	int startPos = startLine = scrollbar->GetCurrentItemNum();
	
	while (curPos < startPos && bufferLine)
	{
		bufferLine = bufferLine->next;
		curPos++;
	}

	while (bufferLine && textY > y) 
	{
		if (bufferLine->text)
			Draw.Text(x, textY, GUI_FONTSCALE, bufferLine->text, colorWhite, qfalse, qfalse, GUI_FONT, true);
#ifdef ETH32_DEBUG
		else 
			Debug.Log("ERROR: CTextBuffer::Display() - encountered NULL buffer line text");
#endif
		bufferLine = bufferLine->next;
		textY -= GUI_TEXTSPACING + GUI_FONTHEIGHT;
	}

	if (Gui.InputActive())
		scrollbar->Display();
}

int CTextBuffer::ProcessMouse(int mx, int my, UINT mevent, CControl **mhook, CControl **khook)
{
	return scrollbar->ProcessMouse(mx, my, mevent, mhook, khook);
}

int CTextBuffer::MouseWheel(int mx, int my, int wheel)
{
	if (INBOUNDS(mx,my)) {
		scrollbar->AdjCurrentItemNum(wheel);
		return 0;
	}
	return 1;
}

void CTextBuffer::SetOrigin(int ox, int oy)
{
	CControl::SetOrigin(ox, oy);
	scrollbar->SetOrigin(x, y);
}

void CTextBuffer::Resize(int cw, int ch)
{
	CControl::Resize(cw, ch);
	scrollbar->Reposition(w - GUI_SPACING - (GUI_SCROLLBAR_WIDTH * eth32.cg.screenXScale), 0);
	scrollbar->Resize(GUI_SCROLLBAR_WIDTH * eth32.cg.screenXScale, h);
	scrollbar->SetMaxDisplayItems(h / (GUI_FONTHEIGHT + GUI_TEXTSPACING));

	breakWidth = w - (GUI_SCROLLBAR_WIDTH * eth32.cg.screenXScale) - GUI_SPACING;
}

void CTextBuffer::AddText(const char *text)
{
	if (!text || !(*text)) return;

	int len = strlen(text);
	char *str = new char[len+1];
	char lastColor = -1;
	char newColor = -1;
	
	strcpy_s(str, len+1, text);

	char *p = &str[len-1]; // get rid of newline char that q3 adds to a lot of msgs
	if (*p == '\n') 
		*p = '\0';

	p = str;
	
	int count = Draw.BreakStringByWidth(p, breakWidth, &lastColor, GUI_FONT, true);
	while (count)
	{
		bufline_t *addLine = new bufline_t;
		 
		if (newColor >= 0) {	// concat string after previous color code
			addLine->text = new char[count+3];
			sprintf_s(addLine->text, count+3, "^%c", newColor);
			strncat_s(addLine->text, count+3, p, count);
			addLine->text[count+2] = '\0';
		}
		else {					// no need for color, cpy it straight in
			addLine->text = new char[count+1];
			strncpy_s(addLine->text, count+1, p, count);
			addLine->text[count] = '\0';
		}

		addLine->prev = NULL;
		addLine->next = dispHead;
		if (dispHead) {
			dispHead->prev = addLine;
			dispHead = addLine;
			dispCurrent = dispCurrent->prev;
		} 
		else {
			dispHead = dispTail = addLine;
			dispCurrent = dispHead;
		}
		dispLineCount++;

		if (startLine)
			startLine++;

		p += count;
		newColor = lastColor;
		count = Draw.BreakStringByWidth(p, breakWidth, &lastColor, GUI_FONT, true);
	}
	// trim down our list to [maxBuffer] lines
	while (dispLineCount > maxBuffer) {
		bufline_t *del = dispTail;
		dispTail = dispTail->prev;
		dispTail->next = NULL;
		delete [] del->text;
		delete del;
		dispLineCount--;
	}
	scrollbar->SetNumCurrentItems(dispLineCount);
	scrollbar->SetCurrentItemNum(startLine);
}

const char* CTextBuffer::GetTextClick(int mx, int my)
{
	if (INBOUNDS(mx,my)) {
		// TODO: will eventually need to make sure this wasn't on buffer slider	
		int linenum = (y2 - my) / (GUI_FONTHEIGHT + GUI_TEXTSPACING);
		float xpos = mx - x;
		
		fontInfo_t *font = GUI_FONT;
		bufline_t *line = dispHead;
		int cnt = 0;
		float charpos = 0.0;

		if (scrollbar->IsActive()) {
			int startLine = scrollbar->GetCurrentItemNum();
			while (startLine > 0) {
				line = line->next;
				startLine--;
			}
		}

		while (line && cnt < linenum) {
			line = line->next;
			cnt++;
		}

		if (line) {
			const char *p = line->text;
			
			while (p && *p && charpos < xpos) {
				if (Q_IsColorString(p)) {
					p += 2;
					continue;
				}
				charpos += font->glyphs[(unsigned char)*p].xSkip * font->glyphScale * GUI_FONTSCALE;
				p++;
			}
		
			if (charpos > xpos)
				p--;

			while (p != line->text && *(p-1) != ' ')
				p--;

			return p;
		}
	}
	return NULL;
}

/********************
* CTextInput        *
********************/

CTextInput::CTextInput(char *clabel, int cx, int cy, int cw, int ch, int maxLen, void (*cfunc)(const char *))
: CControl(clabel, cx, cy, cw, ch)
{
	func = cfunc;
	maxLength = maxLen;
	buffer = new char[maxLength];
	memset(buffer, 0, sizeof(buffer));
	bufferCount = 0;
	inputPos = drawPos = buffer;
	cursorTime = 0;
	lastColor = -1;
	keepFocus = false;
}

CTextInput::~CTextInput(void)
{
	delete [] buffer;
}

void CTextInput::Display(void)
{
	if (!show)
		return;

	float *color = (lastColor > -1) ? g_color_table[ColorIndex(lastColor)] : GUI_FONTCOLOR1;

	Draw.RawPic(x, y, h, h, eth32.guiAssets.txtinputLeft);
	Draw.RawPic(x + h, y, w - (h+h), h, eth32.guiAssets.txtinputCenter);
	Draw.RawPic(x2 - h, y, h, h, eth32.guiAssets.txtinputRight);

	Draw.Text(x+5, y+4, GUI_FONTSCALE, drawPos, color, qfalse, qfalse, GUI_FONT, true);

	if (selected)
		DrawCursor();
}

int CTextInput::ProcessMouse(int mx, int my, UINT mevent, CControl **mhook, CControl **khook)
{
	if (!show)
		return 1;

	if (INBOUNDS(mx,my) && mevent == WM_LBUTTONDOWN) {
		selected = true;
		*khook = this;
		return 0;
	}
	return 1;
}

int CTextInput::ProcessKeybd(unsigned char key, CControl **khook)
{
	switch (key)
	{
	case 0x1B:		// escape
		ClearInput();
		*khook = NULL;
		break;
	case 0x08:		// backspace
		if (bufferCount) {
			bufferCount--;
			buffer[bufferCount] = 0;
			SetDrawPosition();
		}
		break;
	case 0x0D:		// enter
		if (buffer[0] && func)
			func(buffer);
		ClearInput();
		if (!keepFocus)
			*khook = NULL;
		else SetFocus();
		break;
	default:
		if (bufferCount < maxLength - 1) {
			buffer[bufferCount] = key;
			bufferCount++;
			buffer[bufferCount] = 0;
			SetDrawPosition();
		}
	}
	return 0;
}

void CTextInput::SetFocus(void)
{
	selected = true;
}

void CTextInput::SetKeybdFocus(bool set)
{
	if (set && !selected) {
		selected = true;
		return;
	}
	if (!set && selected) {
		ClearInput();
		return;
	}
}

void CTextInput::KeepFocus(bool keep)
{
	keepFocus = keep;
}

void CTextInput::ClearInput(void)
{
	buffer[0] = 0;
	bufferCount = 0;
	inputPos = drawPos = buffer;
	selected = false;
	lastColor = -1;
}

void CTextInput::DrawCursor(void)
{
	static bool show = true;
	
	DWORD time = timeGetTime();

	if (time - cursorTime > GUI_CURFLASHTIME) {
		show = !show;
		cursorTime = time;
	}

	if (show)
		Draw.Text(x + 5 + Draw.sizeTextWidth(GUI_FONTSCALE, drawPos, GUI_FONT), y + 2, GUI_FONTSCALE, "|", GUI_FONTCOLOR1, qtrue, qfalse, GUI_FONT, true);
}

void CTextInput::SetDrawPosition(void)
{
	char *p = buffer;
	int width = Draw.sizeTextWidth(GUI_FONTSCALE, p, GUI_FONT);

	while (width > w - 10)	// 10 for the padding of 5 on each side
	{
		if (Q_IsColorString(p))
			lastColor = *(p+1);
		p++;
		width = Draw.sizeTextWidth(GUI_FONTSCALE, p, GUI_FONT);
	}

	drawPos = p;
}