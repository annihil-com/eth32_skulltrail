// ETH32 - an Enemy Territory cheat for windows
// Copyright (c) 2007 eth32 team
// www.cheatersutopia.com & www.nixcoders.org

#pragma once

#include "CControl.h"
#include "CScrollBar.h"

typedef struct bufline_s
{
	qhandle_t shader;
	char *text;

	DWORD id;

	bufline_s *next, *prev;
} bufline_t;

class CTextBuffer : public CControl
{
public:
	CTextBuffer(char *clabel, int cx, int cy, int cw, int ch, int bufferSize);
	~CTextBuffer(void);
	void Display(void);
	int ProcessMouse(int mx, int my, UINT mevent, CControl **mhook, CControl **khook);
	int MouseWheel(int mx, int my, int wheel);
	void SetOrigin(int ox, int oy);
	void Resize(int cw, int ch);

	void AddText(const char *text);

	void ClearBuffer(void);

	const char *GetTextClick(int mx, int my);
private:
	int maxBuffer;
	int fontHeight;
	int inputLineCount, dispLineCount;
	int startLine;

	int breakWidth;

	DWORD currentId;

	CScrollBar *scrollbar;

	bufline_t *inputHead, *inputTail;
	bufline_t *dispHead, *dispTail, *dispCurrent;
};

class CTextInput : public CControl
{
public:
	CTextInput(char *clabel, int cx, int cy, int cw, int ch, int maxLen, void (*cfunc)(const char*));
	~CTextInput(void);
	void Display(void);
	int ProcessMouse(int mx, int my, UINT mevent, CControl **mhook, CControl **khook);
	int ProcessKeybd(unsigned char key, CControl **khook);
	void SetFocus(void);
	void SetKeybdFocus(bool set);
	void KeepFocus(bool keep);
	bool CaptureEscapeCharacter(void) { return !keepFocus; }
private:
	void ClearInput(void);
	void DrawCursor(void);
	void SetDrawPosition(void);

	void (*func)(const char*);
	char *buffer;
	int maxLength;
	int bufferCount;
	char *drawPos;
	char *inputPos;
	DWORD cursorTime;
	int lastColor;

	bool keepFocus;
};