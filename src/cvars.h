// ETH32 - an Enemy Territory cheat for windows
// Copyright (c) 2007 eth32 team
// www.cheatersutopia.com & www.nixcoders.org

#pragma once

// list of cvars we want to set and restore on hook / unhook

static cvarInfo_t cvarList[] = {
//	 name						ourValue
	{ "cg_drawcompass",			"0" },
	{ "cg_drawcrosshairnames",	"0" },
	{ "cg_teamchatheight",		"0" },
};
