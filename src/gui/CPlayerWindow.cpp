// ETH32 - an Enemy Territory cheat for windows
// Copyright (c) 2007 eth32 team
// www.cheatersutopia.com & www.nixcoders.org

#include "../eth32.h"

CPlayerWindow::CPlayerWindow(char *wlabel, int wx, int wy, int ww, int wh)
: CWindow(wlabel, wx, wy, ww, wh)
{
	showTitle = false;
}

void CPlayerWindow::Display(void)
{
	vec4_t teamColor = { eth32.settings.colorTeam[0]/255.f, eth32.settings.colorTeam[1]/255.f, eth32.settings.colorTeam[2]/255.f, 0.6 }; 
	vec4_t enemyColor = { eth32.settings.colorEnemy[0]/255.f, eth32.settings.colorEnemy[1]/255.f, eth32.settings.colorEnemy[2]/255.f, 0.6 }; 
	int team_x = x + 5;
	int team_y = y + 5;
	int enemy_x = x + (w >> 1) + 5;
	int enemy_y = y + 5;

	CWindow::Display();

	for (int i=0 ; i<MAX_CLIENTS ; i++)
	{
		if (IS_INFOVALID(i) && !IS_SPECTATOR(i) && i != eth32.cg.clientNum) {
			if (IS_FRIENDLY(i)) {
				DrawPlayerInfo(i, team_x, team_y, 150, 30, teamColor);
				team_y += 33;
			}
			else {
				DrawPlayerInfo(i, enemy_x, enemy_y, 150, 30, enemyColor);
				enemy_y += 33;
			}
		}
	}
}

void CPlayerWindow::DrawPlayerInfo(int clientNum, int px, int py, int pw, int ph, vec4_t color)
{
	Draw.RawFillRect(px, py, pw, ph, color);

	Draw.Text(px+3, py+3, GUI_FONTSCALE, eth32.cg.players[clientNum].name, colorWhite, qfalse, qfalse, GUI_FONT, true);
}

