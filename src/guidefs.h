
// ETH32 - an Enemy Territory cheat for windows
// Copyright (c) 2007 eth32 team
// www.cheatersutopia.com & www.nixcoders.org

#pragma once

#include "CAimbot.h"
#include "CIrc.h"

#define SETFLOAT(x)			((int)(x * 1000))		// only keeping up to 0.001
#define GETFLOAT(x)			(x / 1000.f)

// Control definition reference: (update this if you add new control types)

// Type					Arg0			Arg1			Arg2			Arg3			Arg4
// CTRL_CHECKBOX		bool *target	N/A				N/A				N/A				N/A
// CTRL_INTDISPLAY		int *target		N/A				N/A				N/A				N/A
// CTRL_INTSLIDER		int	min			int	max			int *target		N/A				N/A
// CTRL_FLOATSLIDER		float min		float max		float *target	N/A				N/A
// CTRL_COLORPICKER		BYTE *color		N/A				N/A				N/A				N/A

// NOTE: cast arg0-arg4 to int if used, use SETFLOAT/GETFLOAT for floats

static const windef_t windows[] = 
{
	// MAINVIEW
	{ 
		"Status",				// title
		WIN_STATUS,				// type
		GUI_MAINVIEW,			// view
		5, 422, 120, 68,		// x, y, w, h
		0,						// num controls
	},
	{
		"Weapon",				// title
		WIN_WEAPON,				// type
		GUI_MAINVIEW,			// view
		95, 422, 120, 60,		// x, y, w, h
		0,						// num controls
	},
	{
		"Spectators",			// title
  		WIN_SPECTATOR,			// type
		GUI_MAINVIEW,			// view
		50, 193, 100, 160,		// x, y, w, h
		0,				// num controls
	},
	/*{ // sol: a window I use for setting weapon rendering parms *** DO NOT REMOVE ***
		"Weap Rendering",		// title
		WIN_STANDARD,			// type
		GUI_MAINVIEW,			// view
		160, 200, 150, 200,		// x, y, w, h
		7,						// num controls
		{
			// Type				Label				X		Y		W		H		Arg0 ... Arg4
			{ CTRL_FLOATSLIDER,	"X",				5,		5,		140,	20,		SETFLOAT(-10), SETFLOAT(10), (int)&eth32.settings.weapOrigin[0] },
			{ CTRL_FLOATSLIDER,	"Y",				5,		30,		140,	20,		SETFLOAT(-10), SETFLOAT(10), (int)&eth32.settings.weapOrigin[1] },
			{ CTRL_FLOATSLIDER,	"Z",				5,		55,		140,	20,		SETFLOAT(-10), SETFLOAT(10), (int)&eth32.settings.weapOrigin[2] },
			{ CTRL_FLOATSLIDER,	"Pitch",			5,		80,		140,	20,		SETFLOAT(0), SETFLOAT(360), (int)&eth32.settings.weapAngle[0] },
			{ CTRL_FLOATSLIDER,	"Yaw",				5,		105,	140,	20,		SETFLOAT(0), SETFLOAT(360), (int)&eth32.settings.weapAngle[1] },
			{ CTRL_FLOATSLIDER,	"Roll",				5,		130,	140,	20,		SETFLOAT(0), SETFLOAT(360), (int)&eth32.settings.weapAngle[2] },
			{ CTRL_FLOATSLIDER,	"Dist",				5,		155,	140,	20,		SETFLOAT(0), SETFLOAT(120), (int)&eth32.settings.weapDist },
		},
	},*/
	{
		"mainchat",				// title
		WIN_CHAT,				// type
		GUI_MAINVIEW,			// view
		289, 379, 350, 100,		// x, y, w, h
		0,						// num controls
	},
	{
		"Respawn",				// title
		WIN_RESPAWN,			// type
		GUI_MAINVIEW,			// view
		280, 5, 38, 20,			// x, y, w, h
		0,						// num controls
	},
	{
		"Radar",				// title
		WIN_RADAR,				// type
		GUI_MAINVIEW,			// view
		395, 302, 120, 120,		// x, y, w, h
		0,						// num controls
	},
	{
		"Cameras",				// title
		WIN_CAMERA,				// type
		GUI_MAINVIEW,			// view
		5, 5, 5, 5,				// x, y, w, h : these don't matter though
		0,						// num controls
	},
	{
		"WA Frontend",			// title
		WIN_WINAMP,				// type
		GUI_MAINVIEW,			// view
		220, 5, 160, 65,		// x, y, w, h
		0,						// num controls
	},
	// MENUVIEW
	{
		"Aimbot",			// title
		WIN_STANDARD,			// type
		GUI_MENUVIEW,			// view
		7, 55, 150, 325,		// x, y, w, h
		13,				// num controls
		{
			// Type			Label			X		Y		W		H		Arg0 ... Arg4
			{ CTRL_DROPBOX,		"Aimbot Mode", 		5, 		5,		140,		23,		0, AIMMODE_MAX-1, (int)&eth32.settings.aimMode, (int)aimModeText },
			{ CTRL_DROPBOX,		"Aim Type",		5,		34,		140,		23,		AIM_OFF, AIM_MAX-1, (int)&eth32.settings.aimType, (int)aimTypeText },
			{ CTRL_CHECKBOX,	"Autofire",		5,		63,		140,		8,		(int)&eth32.settings.autofire },
			{ CTRL_CHECKBOX,	"Validate Attack",	5,		76,		140,		8,		(int)&eth32.settings.atkValidate },
			{ CTRL_CHECKBOX,	"Target Lock",		5,		89,		140,		8,		(int)&eth32.settings.lockTarget },
			{ CTRL_FLOATSLIDER, 	"FOV",			5,		102,		140,		20,		SETFLOAT(0), SETFLOAT(360), (int)&eth32.settings.fov },
			{ CTRL_DROPBOX,		"Target Sort",		5,		127,		140,		23,		SORT_OFF, SORT_MAX-1, (int)&eth32.settings.aimSort, (int)sortTypeText },
			{ CTRL_DROPBOX,		"Aim Priority",		5,		159,		140,		23,		0, AP_MAX-1, (int)&eth32.settings.headbody, (int)priorityTypeText },
			{ CTRL_DROPBOX,		"Head Trace Style",	5,		185,		140,		23,		HEAD_CENTER, HEAD_MAX-1, (int)&eth32.settings.headTraceType, (int)headTraceTypeText },
			{ CTRL_DROPBOX,		"Body Trace Style",	5,		210,		140,		23,		BODY_CENTER, BODY_MAX-1, (int)&eth32.settings.bodyTraceType, (int)bodyTraceTypeText },
			{ CTRL_FLOATSLIDER,	"Dynamic Hitbox",       5,		235, 		140,		20,		SETFLOAT(0), SETFLOAT(3), (int)&eth32.settings.dynamicHitboxScale },
			{ CTRL_FLOATSLIDER, 	"Anim. Correction", 	5,		260,		140,		20,		SETFLOAT(-10), SETFLOAT(10), (int)&eth32.settings.animCorrection },
			{ CTRL_CHECKBOX,	"Auto Crouch",		5,		285,		140,		8,		(int)&eth32.settings.autoCrouch },
		}
	},

	{
		"Predictions",			// title
		WIN_STANDARD,			// type
		GUI_MENUVIEW,			// view
		7, 55, 250, 230,	// x, y, w, h
		9,				// num controls
		{
			// Type			Label				X	Y	W	H		Arg0 ... Arg4
			{ CTRL_CHECKBOX,	"Preshoot",		        5,	5,      240,	8,		(int)&eth32.settings.preShoot },
			{ CTRL_CHECKBOX,	"Preaim",		        5,	18,     240,	8,		(int)&eth32.settings.preAim },
			{ CTRL_FLOATSLIDER,	"Preshoot Time",	        5,	31,	240,	20,		SETFLOAT(0), SETFLOAT(5000), (int)&eth32.settings.preShootTime },
			{ CTRL_FLOATSLIDER,	"Preaim Time",		        5,	56, 	240,	20,		SETFLOAT(0), SETFLOAT(5000), (int)&eth32.settings.preAimTime },
			{ CTRL_DROPBOX,		"Self Predict Type",		5,	81,	240,	23,		SPR_OFF, SPR_MAX-1, (int)&eth32.settings.predSelfType, (int)selfPredictText },
			{ CTRL_FLOATSLIDER,	"Self Predict",			5,	110,	240,	20,		SETFLOAT(-0.1), SETFLOAT(0.1), (int)&eth32.settings.predSelf },
			{ CTRL_CHECKBOX,	"Auto predict BOTs",		5,	135,	240,	8,		(int)&eth32.settings.autoPredictBots },
			{ CTRL_FLOATSLIDER,	"Target Predict",	        5,     	156,	240,	20,		SETFLOAT(-0.1), SETFLOAT(0.1), (int)&eth32.settings.pred },
			{ CTRL_FLOATSLIDER,	"BOT Predict",	                5,     	181,	240,	20,		SETFLOAT(-0.1), SETFLOAT(0.1), (int)&eth32.settings.predbot },
		}
	},
	
	{
		"GUI Style",			// title
		WIN_STANDARD,			// type
		GUI_MENUVIEW,			// view
		319, 47, 150, 132,		// x, y, w, h
		7,						// num controls
		{
			{ CTRL_CHECKBOX,	"Chatbox",				5,		5,		140,	8,		(int)&eth32.settings.guiChat },
			{ CTRL_CHECKBOX,	"Centerprint",			5,		18,		140,	8,		(int)&eth32.settings.guiCenterPrint },
			{ CTRL_CHECKBOX,	"Centerprint animate", 	5, 		31, 	140, 	8, 		(int)&eth32.settings.guiCenterPrintAnim },
			{ CTRL_FLOATSLIDER,	"Centerprint size",		5,		44,		140,	20,		SETFLOAT(0.0), SETFLOAT(3.0), (int)&eth32.settings.guiCenterPrintScale },
			{ CTRL_CHECKBOX,	"Banner",				5,		69,		140,	8,		(int)&eth32.settings.guiBanner },
			{ CTRL_FLOATSLIDER,	"Banner size",			5,		82,		140,	20,		SETFLOAT(0.0), SETFLOAT(3.0), (int)&eth32.settings.BannerScale },
			{ CTRL_CHECKBOX,	"Original hud",			5,		107,	140,	8,		(int)&eth32.settings.guiOriginal },		
		},
	},
	{
		"Chams",			// title
		WIN_STANDARD,			// type
		GUI_MENUVIEW,			// view
		7, 55, 300, 200,		// x, y, w, h
		16,				// num controls
		{
			// Type			Label				X		Y		W		H		Arg0 ... Arg4
			{ CTRL_DROPBOX,		"Team Shader 1",		5,		5,		140,		23,		0, SHADER1_MAX-1, (int)&eth32.settings.teamShader1, (int)Shader1Text },
			{ CTRL_CHECKBOX,	"Wallhack",			5,		30,		140,		8,		(int)&eth32.settings.teamShader1Wallhack },
			{ CTRL_DROPBOX,		"Team Shader 2",		5,		43,		140,		23,		0, SHADER2_MAX-1, (int)&eth32.settings.teamShader2, (int)Shader2Text },
			{ CTRL_CHECKBOX,	"Wallhack",			5,		68,		140,		8,		(int)&eth32.settings.teamShader2Wallhack },
			
			{ CTRL_DROPBOX,		"Enemy Shader 1",		155,		5,		140,		23,		0, SHADER1_MAX-1, (int)&eth32.settings.enemyShader1, (int)Shader1Text },
			{ CTRL_CHECKBOX,	"Wallhack",			155,		30,		140,		8,		(int)&eth32.settings.enemyShader1Wallhack },
			{ CTRL_DROPBOX,		"Enemy Shader 2",		155,		43,		140,		23,		0, SHADER2_MAX-1, (int)&eth32.settings.enemyShader2, (int)Shader2Text },
			{ CTRL_CHECKBOX,	"Wallhack",			155,		68,		140,		8,		(int)&eth32.settings.enemyShader2Wallhack },
			
			{ CTRL_DROPBOX,		"Weapon Shader 1",		5,		93,		140,		23,		0, SHADER3_MAX-1, (int)&eth32.settings.weaponShader1, (int)Shader3Text },
			{ CTRL_CHECKBOX,	"Wallhack",			5,		118,		140,		8,		(int)&eth32.settings.weaponShader1Wallhack },
			{ CTRL_DROPBOX,		"Weapon Shader 2",		5,		131,		140,		23,		0, SHADER4_MAX-1, (int)&eth32.settings.weaponShader2, (int)Shader4Text },
			{ CTRL_CHECKBOX,	"Wallhack",			5,		156,		140,		8,		(int)&eth32.settings.weaponShader2Wallhack },			
			
			{ CTRL_DROPBOX,		"Item Shader 1",		155,		93,		140,		23,		0, SHADER3_MAX-1, (int)&eth32.settings.itemShader1, (int)Shader3Text },
			{ CTRL_CHECKBOX,	"Wallhack",			155,		118,		140,		8,		(int)&eth32.settings.itemShader1Wallhack },
			{ CTRL_DROPBOX,		"Item Shader 2",		155,		131,		140,		23,		0, SHADER4_MAX-1, (int)&eth32.settings.itemShader2, (int)Shader4Text },
			{ CTRL_CHECKBOX,	"Wallhack",			155,		156,		140,		8,		(int)&eth32.settings.itemShader2Wallhack },
		},
	},
	{
		"Custom Models",		// title
		WIN_STANDARD,			// type
		GUI_MENUVIEW,			// view
		7, 55, 150, 71,			// x, y, w, h
		3,				// num controls
		{
			{ CTRL_CHECKBOX,	"Reaper Head",			5,		5,		140,		8,		(int)&eth32.settings.reaperHead },
			{ CTRL_CHECKBOX,	"Hitler Head",			5,		18,		140,		8,		(int)&eth32.settings.hitlerHead },
			{ CTRL_CHECKBOX,	"Hitler Body",			5,		31,		140,		8,		(int)&eth32.settings.hitlerBody },
		},
	},
	{
		"Visuals",				// title
		WIN_STANDARD,			// type
		GUI_MENUVIEW,			// view
		60, 46, 150, 250,		// x, y, w, h
		11,						// num controls
		{
			// Type				Label						X		Y		W		H		Arg0 ... Arg4
			{ CTRL_CHECKBOX,	"Wallhack",					5,		5,		140,	8,		(int)&eth32.settings.wallhack },
			{ CTRL_CHECKBOX,	"Draw Blackout",			5,		20,		140,	8,		(int)&eth32.settings.blackout },
			{ CTRL_CHECKBOX,	"Weapon Zoom",				5,		35,		140,	8,		(int)&eth32.settings.weaponZoom },
			{ CTRL_FLOATSLIDER,	"Scoped Turning",			5,		50,		140,	20,		SETFLOAT(0.1), SETFLOAT(1.0), (int)&eth32.settings.scopedTurnSpeed },
			{ CTRL_INTSLIDER,	"Smoke Visibility",			5,		75,	140,	20,		0, 100, (int)&eth32.settings.smoketrnsp },
			{ CTRL_FLOATSLIDER,	"Radar Range",				5,		100,	140,	20,		SETFLOAT(100.0), SETFLOAT(10000.0), (int)&eth32.settings.radarRange },
			{ CTRL_CHECKBOX,	"Remove Foliage",			5,		125,		140,		8,		(int)&eth32.settings.removeFoliage },
			{ CTRL_CHECKBOX,	"Draw Warning Messages",	5,		140,		140,		8,		(int)&eth32.settings.showReload },
			{ CTRL_CHECKBOX,	"Remove Hit Particles",		5,		165,		140,		8,		(int)&eth32.settings.removeParticles },
			{ CTRL_DROPBOX,		"Damage Indicator Type",	5,		182,		140,		23,		DMG_OFF, DMG_MAX-1, (int)&eth32.settings.dmgIndicator, (int)damageIndicatorText },
			{ CTRL_FLOATSLIDER,	"Dmg Arrow Size",			5,		210,		140,		20,		SETFLOAT(16), SETFLOAT(128), (int)&eth32.settings.dmgArrSize },
		},
	},
	{
		"Hitbox Display",		// title
		WIN_STANDARD,			// type
		GUI_MENUVIEW,			// view
		157, 55, 150, 148,		// x, y, w, h
		8,				// num controls
		{
			// Type			Label			X		Y		W		H		Arg0 ... Arg4
			{ CTRL_CHECKBOX,	"Head",			5,		5,		140,		8,		(int)&eth32.settings.drawHeadHitbox },
			{ CTRL_CHECKBOX,	"Head Axes",		5,		18,		140,		8,		(int)&eth32.settings.drawHeadAxes },
			{ CTRL_CHECKBOX,	"Body",			5,		31,		140,		8,		(int)&eth32.settings.drawBodyHitbox },
			{ CTRL_CHECKBOX,	"Show Aimpoints",	5,		44,		140,		8,		(int)&eth32.settings.debugPoints},
			{ CTRL_CHECKBOX,	"Bullet Rail",		5,		57,		140,		8,		(int)&eth32.settings.drawBulletRail },
			{ CTRL_CHECKBOX,	"Rail Wallhack",	5,		70,		140,		8,		(int)&eth32.settings.railWallhack },
			{ CTRL_INTSLIDER,	"Head Trail Time",	5,		83,		140,		20,		0, 300, (int)&eth32.settings.headRailTime },
			{ CTRL_INTSLIDER,	"Body Trail Time",	5,		108,		140,		20,		0, 300, (int)&eth32.settings.bodyRailTime },
		},
	},
	{
		"ESP",				// title
		WIN_ESPCONFIG,			// type
		GUI_MENUVIEW,			// view
		310, 55, 150, 425,		// x, y, w, h
		15,				// num controls
		{
			{ CTRL_CHECKBOX,	"Missile Blast Light",		5,		130,		140,		8,		(int)&eth32.settings.grenadeDlight },
			{ CTRL_CHECKBOX,	"Mortar Blast Light",		5,		143,		140,		8,		(int)&eth32.settings.mortarDlight },
			{ CTRL_CHECKBOX,	"Disguised ESP",		5,		156,		140,		8,		(int)&eth32.settings.drawDisguised },
			{ CTRL_CHECKBOX,	"Mortar Trace and Esp",		5,		169,		140,		8,		(int)&eth32.settings.mortarTrace },
			{ CTRL_CHECKBOX,	"Arty Markers",			5,		182,		140,		8,		(int)&eth32.settings.artyMarkers },
			{ CTRL_CHECKBOX,	"Old ESP Icons",			5,		195,		140,		8,		(int)&eth32.settings.oldClsIcons },
			{ CTRL_DROPBOX,  	"Class ESP",	  		5,              208,      	140,            23,        	0, CLS_MAX-1, (int)&eth32.settings.classEspType, (int)classEspText },
			{ CTRL_FLOATSLIDER,	"Class ESP Size",		5,		237,		140,		20,		SETFLOAT(0), SETFLOAT(30), (int)&eth32.settings.clsSize },
			{ CTRL_FLOATSLIDER,	"Class ESP Opacity",		5,		262,		140,		20,		SETFLOAT(0), SETFLOAT(1), (int)&eth32.settings.clsOpacity },
			{ CTRL_CHECKBOX,	"Items ESP",			5,		287,		140,		8,		(int)&eth32.settings.itemEsp },
			{ CTRL_FLOATSLIDER,	"Items ESP Size",		5,		300,		140,		20,		SETFLOAT(0), SETFLOAT(30), (int)&eth32.settings.itemEspSize },
			{ CTRL_FLOATSLIDER,	"Items ESP Opacity",		5,		325,		140,		20,		SETFLOAT(0), SETFLOAT(1), (int)&eth32.settings.itemEspOpacity },
			{ CTRL_CHECKBOX,	"Box ESP",			5,		346,		140,		8,		(int)&eth32.settings.boxEsp },
			{ CTRL_INTSLIDER,	"Box ESP Border Size",		5,		359,		140,		20,		0, 5, (int)&eth32.settings.boxEspBorder },
			{ CTRL_FLOATSLIDER,	"Box ESP Opacity",		5,		384,		140,		20,		SETFLOAT(0), SETFLOAT(1), (int)&eth32.settings.boxEspOpacity },
		},
	},
	/** *******************************************************************
					COLOR PICKER
	******************************************************************* **/
	{
		"Colors",			// title
		WIN_STANDARD,			// type
		GUI_MENUVIEW,			// view
		5, 20, 630, 176,		// x, y, w, h
		1,				// num controls
		{
			// Type		    Label			X		Y		W		H		Arg0 ... Arg4
			{ CTRL_COLORPICKER, "Picker",			5,		5,		620,		156 },
		},
	},
	
	{
		"Weapon Config",		// title
		WIN_WEAPCONFIG,				// type
		GUI_MENUVIEW,			// view
		157, 55, 150, 225,		// x, y, w, h
		0,						// num controls
	},
	/******************VECS*****************/
	{
		"Corrections",		// title
		WIN_STANDARD,			// type
		GUI_MENUVIEW,			// view
		7, 55, 250, 189,		// x, y, w, h
		7,				// num controls
		{
			{ CTRL_FLOATSLIDER,	"Head Hitbox Size",			5,	5, 	240,	20,		SETFLOAT(1), SETFLOAT(15), (int)&eth32.settings.headBoxSize },
			{ CTRL_FLOATSLIDER,	"Body Hitbox Size",	    		5,	30, 	240,	20,		SETFLOAT(1), SETFLOAT(40), (int)&eth32.settings.bodybox },
			{ CTRL_CHECKBOX,	"Auto weapon Delay",   			5,	55,	240,	8,		(int)&eth32.settings.autoDelay },
			{ CTRL_INTSLIDER,	"Weapon Delay Close",       		5,	68, 	240,	20,		0,800, (int)&eth32.settings.delayClose },
			{ CTRL_INTSLIDER,	"Weapon Delay Med",       		5,	92, 	240,	20,		0,800, (int)&eth32.settings.delayMed },
			{ CTRL_INTSLIDER,	"Weapon Delay Far",       		5,	118, 	240,	20,		0,800, (int)&eth32.settings.delayFar },
			{ CTRL_INTSLIDER,	"Bullet size",       			5,	143, 	240,	20,		0,5, (int)&eth32.settings.bulletSize },
		}
	},
	{
		"Hitbox",		// title
		WIN_HITBOX,			// type
		GUI_MENUVIEW,			// view
		7, 55, 250, 134,		// x, y, w, h
		0,				// num controls
	},
	{
		"Camera Settings",
		WIN_CAMCONFIG,
		GUI_MENUVIEW,
		218, 48, 150, 150, 
		0,
	},
	
	{
		"Sound",			// title
		WIN_STANDARD,			// type
		GUI_MENUVIEW,			// view
		157, 250, 150, 122,		// x, y, w, h
		6,				// num controls
		{
			{ CTRL_CHECKBOX,	"Spree Sounds",		        5,		5,		140,	8,		(int)&eth32.settings.killSounds },
			{ CTRL_CHECKBOX,	"Random Spree Sounds",		5,		18,		140,	8,		(int)&eth32.settings.randomSpreeSounds },
			{ CTRL_DROPBOX,		"Hitsound Type",		5,		31,		140,	23,		0, HIT_MAX-2, (int)&eth32.settings.hitsounds, (int)hitsoundsText },
			{ CTRL_CHECKBOX,	"Pure Only",		        5,		56,		140,	8,		(int)&eth32.settings.pureSounds },
			{ CTRL_CHECKBOX,	"HQ sounds",		        5,		69,		140,	8,		(int)&eth32.settings.hqSounds },
			{ CTRL_FLOATSLIDER,	"MK Reset Time",		5,		82,		140,	20,		SETFLOAT(0), SETFLOAT(10000), (int)&eth32.settings.mkResetTime },
		},
	},
	{
		"Misc",					// title
		WIN_STANDARD,			// type
		GUI_MENUVIEW,			// view
		7, 55, 150, 300,		// x, y, w, h
		16,						// num controls
		{
			{ CTRL_CHECKBOX,	"Name Click",		5,		5,		140,	8,		(int)&eth32.settings.nameClick },
			{ CTRL_CHECKBOX,	"Spec list", 		5, 		20, 		140, 	8, 		(int)&eth32.settings.getSpeclist },
			{ CTRL_CHECKBOX,	"Kill Spam",		5,		35,		140,	8,		(int)&eth32.settings.killSpam },
			{ CTRL_CHECKBOX,	"Third Person",		5,		50,		140,	8,		(int)&eth32.settings.thirdPerson },
			{ CTRL_CHECKBOX,	"Transparent Console",	5,	65,		140,	8,		(int)&eth32.settings.transparantConsole },
			{ CTRL_CHECKBOX,	"Respawn Timers",	5,		90,		140,	8,		(int)&eth32.settings.respawnTimers },
			{ CTRL_CHECKBOX,	"Auto Tapout",		5,		105,		140,	8,		(int)&eth32.settings.autoTapout },
			{ CTRL_CHECKBOX,	"Auto Vote",		5,		120,	140,	8,		(int)&eth32.settings.autoVote },
			{ CTRL_DROPBOX,		"Auto Push",		5,		135,	140,	23,		0, MAX_AUTOPUSH-1, (int)&eth32.settings.autoPush, (int)autopushText },
			{ CTRL_FLOATSLIDER,	"Auto Push Distance",5,		160,	140,	20,		SETFLOAT(0.0), SETFLOAT(256.0), (int)&eth32.settings.autoPushDistance },
			{ CTRL_CHECKBOX,	"Panzer Suicide",	5,		185,	140,	8,		(int)&eth32.settings.panzerSuicide },
			{ CTRL_FLOATSLIDER,	"Impact Distance",	5,		200,	140,	20,		SETFLOAT(40.0), SETFLOAT(500.0), (int)&eth32.settings.panzerImpactDist },
			{ CTRL_CHECKBOX,	"Anti Teamkill",	5,		225,		140,	8,		(int)&eth32.settings.antiTk },
			{ CTRL_CHECKBOX,	"Spoof OS",		5,		240,		140,	8,		(int)&eth32.settings.etproOs },
			{ CTRL_CHECKBOX,	"Timenudge Hack",	5,		255,		140,	8,		(int)&eth32.settings.nudgeHack },
			{ CTRL_CHECKBOX,	"Auto Complaint",	5,		270,		140,	8,		(int)&eth32.settings.autoComplaint },
		},
	},
	{
		"Winamp",				// title
		WIN_STANDARD,			// type
		GUI_MENUVIEW,			// view
		281, 48, 150, 86,		// x, y, w, h
		4,						// num controls
		{
			{ CTRL_WA_CHECKBOX,	"Enable Control",	5,		5,		140,	8,		(int)&Winamp.enableWinamp, WA_CTRL_ENABLE_CTRL },
			{ CTRL_WA_CHECKBOX,	"Show Song",		5,		18,		140,	8,		(int)&Winamp.showSong, WA_CTRL_SHOW_TITLE },
			{ CTRL_INTSLIDER,	"Scroll Rate",		5,		31,		140,	20,		10, 500, (int)&Winamp.scrollRate },
			{ CTRL_CHECKBOX,	"Auto Spam",		5,		56,		140,	8,		(int)&Winamp.autoSpam },
		},
	},
	// USERSVIEW
	{
		"ETH32 Users",			// title
		WIN_USERS,				// type
		GUI_USERVIEW,			// view
		5, 5, 630, 470,			// x, y, w, h
		0,						// num controls
	},
	// CLIENTVIEW
	{ 
		"Clients",				// title
		WIN_CLIENTS,			// type
		GUI_CLIENTVIEW,			// view
		5, 5, 630, 470,			// x, y, w, h
		0,						// num controls
	},
	// IRCVIEW
	{
		"IRC Frontend",			// title
		WIN_IRC,				// type
		GUI_IRCVIEW,			// view
		5, 5, 630, 470,			// x, y, w, h
		0,						// num controls
	},
	{
		"Namestealer",			// title
		WIN_STANDARD,			// type
		GUI_MENUVIEW,			// view
		325, 58, 150, 115,		// x, y, w, h
		4,						// num controls
		{
			// Type				Label					X		Y		W		H		Arg0 ... Arg4
			{ CTRL_CHECKBOX,	"Name steal",			5,		5,		140,	8,		(int)&eth32.settings.doNamesteal },
			{ CTRL_INTSLIDER,	"Delay"		,			5,		18,		140,	20,		0, 20000, (int)&eth32.settings.NamestealDelay },
			{ CTRL_INTSLIDER,	"Init Grace",			5,		41,		140,	20,		0, 20000, (int)&eth32.settings.NamestealGrace },
			{ CTRL_DROPBOX,		"steal type",			5,		64,		140,	23,		0, NAMESTEAL_MAX-1, (int)&eth32.settings.NamestealMode, (int)namestealText },
		}
	},
	{
		"Spam",					// title
		WIN_STANDARD,			// type
		GUI_MENUVIEW,			// view
		325, 200, 150, 115,		// x, y, w, h
		6,						// num controls
		{
			// Type				Label					X		Y		W		H		Arg0 ... Arg4
			{ CTRL_CHECKBOX,	"Kill Spam",			5,		5,		140,	8,		(int)&eth32.settings.killSpam },
			{ CTRL_CHECKBOX,	"Multikill Spam",		5,		18,		140,	8,		(int)&eth32.settings.multikillSpam },
			{ CTRL_CHECKBOX,	"Death Spam",			5,		31,		140,	8,		(int)&eth32.settings.deathSpam },
			{ CTRL_CHECKBOX,	"Suicide Spam",			5,		44,		140,	8,		(int)&eth32.settings.suicideSpam },
			{ CTRL_CHECKBOX,	"PrivateMsg killspam",	5,		57,		140,	8,		(int)&eth32.settings.pmKillspam },
			{ CTRL_DROPBOX,		"Killspam Type",		5,		70,		140,	23,		0, KILLSPAM_MAX-1, (int)&eth32.settings.KillspamType, (int)killspamText },
		}
	},		
	{
		"Banner",
		WIN_BANNER,				// type
		GUI_MAINVIEW,			// view
		20, 20, 1, 1,			// x, y, w, h
		0,						// num controls	
	},
	// BALLISTICBOT
	{
		"Grenadebot",			// title
		WIN_STANDARD,			// type
		GUI_MENUVIEW,			// view
		320, 55, 150, 300,		// x, y, w, h
		16,						// num controls
		{
			// Type				Label					X		Y		W		H		Arg0 ... Arg4
			{ CTRL_CHECKBOX,	"Grenade Aimbot",		5,		5,		140,		8,		(int)&eth32.settings.grenadeBot },
			{ CTRL_CHECKBOX,	"Riflenade Aimbot",		5,		18,		140,		8,		(int)&eth32.settings.rifleBot },
			{ CTRL_CHECKBOX,	"Block fire",			5,		31,		140,		8,		(int)&eth32.settings.grenadeBlockFire },
			{ CTRL_CHECKBOX,	"Grenade Trajectory",		5,		44,		140,		8,		(int)&eth32.settings.valGrenTrajectory },
			{ CTRL_CHECKBOX,	"Rifle Trajectory",		5,		57,		140,		8,		(int)&eth32.settings.valRifleTrajectory },
			{ CTRL_CHECKBOX,	"Grenade Senslock",		5,		70,		140,		8,		(int)&eth32.settings.grenadeSenslock },
			{ CTRL_FLOATSLIDER,	"Rifle Z correction",		5,		93,		140,		20,		SETFLOAT(-50), SETFLOAT(50), (int)&eth32.settings.riflenadeZ },
			{ CTRL_FLOATSLIDER,	"Grenade Z correction",		5,		118,		140,		20,		SETFLOAT(-50), SETFLOAT(50), (int)&eth32.settings.grenadeZ },
			{ CTRL_INTSLIDER,	"Grenade Fire Delay",		5,		142,		140,		20,		0, 1000, (int)&eth32.settings.grenadeFireDelay },
			{ CTRL_CHECKBOX,	"Grenade Autofire",		5,		165,		140,		8,		(int)&eth32.settings.grenadeAutoFire },
			{ CTRL_CHECKBOX,	"Riflenade Autofire",		5,		178,		140,		8,		(int)&eth32.settings.rifleAutoFire },
			{ CTRL_DROPBOX,		"Target Predict",		5,		191,		140,		23,		0, RF_PREDICT_MAX-1, (int)&eth32.settings.ballisticPredict, (int)riflePredictText},
			{ CTRL_CHECKBOX,	"Check Radius Damage",		5,		220,		140,		8,		(int)&eth32.settings.ballisticRadiusDamage },
			{ CTRL_FLOATSLIDER,	"Blast radius",			5,		233,		140,		20,		SETFLOAT(30), SETFLOAT(500), (int)&eth32.settings.radiusDamage },
			{ CTRL_CHECKBOX,	"Auto Targets",			5,		258,		140,		8,		(int)&eth32.settings.autoGrenTargets },
			{ CTRL_CHECKBOX,	"Multi Bounce",			5,		271,		140,		8,			(int)&eth32.settings.allowMultiBounce },
		}
	},
	{
		"Human Aim",			// title
		WIN_STANDARD,			// type
		GUI_MENUVIEW,			// view
		325, 60, 150, 354,		// x, y, w, h
		14,						// num controls
		{
			// Type			Label				X	Y	W	H		Arg0 ... Arg4
	    		{ CTRL_DROPBOX,		"Human Mode", 		        5,      5,	140,	23,		0, HUMAN_AIM_MAX-1, (int)&eth32.settings.humanMode, (int)humanModeText },
	    		{ CTRL_FLOATSLIDER,	"Simple Logins - Speed",	5,	34,	140,	20,		SETFLOAT(0), SETFLOAT(0.2), (int)&eth32.settings.human1_speed },
	    		{ CTRL_FLOATSLIDER,	"Full Logins - Human Value",	5,	59,	140,	20,		SETFLOAT(0), SETFLOAT(0.2), (int)&eth32.settings.human2_humanValue },
            		{ CTRL_FLOATSLIDER,	"Full Logins - SpeedX",		5,	84,	140,	20,		SETFLOAT(0), SETFLOAT(10), (int)&eth32.settings.human2_aimX },
            		{ CTRL_FLOATSLIDER,	"Full Logins - SpeedY",		5,	109,	140,	20,		SETFLOAT(0), SETFLOAT(10), (int)&eth32.settings.human2_aimY },
            		{ CTRL_FLOATSLIDER,	"Full Logins - DivMin",		5,	134,	140,	20,		SETFLOAT(0), SETFLOAT(10), (int)&eth32.settings.human2_divMin },
            		{ CTRL_FLOATSLIDER,	"Full Logins - DivMax",		5,	159,	140,	20,		SETFLOAT(0), SETFLOAT(10), (int)&eth32.settings.human2_divMax },
            		{ CTRL_DROPBOX,		"Aim Protect", 		      	5, 	184,    140,	23,		0, PROTECT_MAX-1, (int)&eth32.settings.aimprotect, (int)aimprotectText },            		
            		{ CTRL_CHECKBOX,	"Lock mouse while aiming",	5,	213,	140,	8,		(int)&eth32.settings.lockMouse },
			{ CTRL_CHECKBOX,	"Randomized Aim",		5,	226,	140,	8,		(int)&eth32.settings.randomAim },
			{ CTRL_INTSLIDER,	"Shake Frequency ms",		5,	239,	140,	20,		500, 20000, (int)&eth32.settings.shakeFreq },
			{ CTRL_INTSLIDER,	"Random Factor X",		5,	264,	140,	20,		0, 1000, (int)&eth32.settings.randFactX },
			{ CTRL_INTSLIDER,	"Random Factor Y",		5,	289,	140,	20,		0, 1000, (int)&eth32.settings.randFactY },
			{ CTRL_INTSLIDER,	"Random Factor Z",		5,	314,	140,	20,		0, 1000, (int)&eth32.settings.randFactZ },
		}
	},
    {
		"Crosshair",			// title
		WIN_STANDARD,			// type
		GUI_MENUVIEW,			// view
		7, 55, 200, 95,		// x, y, w, h
		3,				// num controls
		{
			{ CTRL_DROPBOX,		"Custom Crosshair",	5,		5,		190,	23,		0, XHAIR_MAX-1, (int)&eth32.settings.customXhair, (int)crosshairText },
			{ CTRL_FLOATSLIDER,	"Size",			5,		30,		190,	20,		SETFLOAT(0), SETFLOAT(500), (int)&eth32.settings.crossSize },
			{ CTRL_FLOATSLIDER,	"Opacity",		5,		55,		190,	20,		SETFLOAT(0), SETFLOAT(1), (int)&eth32.settings.xhairOpacity },
		},
	},
};
static const assetdef_t assetDefs[] =
{
//	  Key				Type					Target
	{ "titlecolor",		ASSET_VEC4,				(void*)eth32.guiAssets.titleColor },
	{ "textcolor1",		ASSET_VEC4,				(void*)eth32.guiAssets.textColor1 },
	{ "textcolor2",		ASSET_VEC4,				(void*)eth32.guiAssets.textColor2 },
	{ "titleleft",		ASSET_SHADERNOMIP,		(void*)&eth32.guiAssets.titleLeft },
	{ "titlecenter",	ASSET_SHADERNOMIP,		(void*)&eth32.guiAssets.titleCenter },
	{ "titleright",		ASSET_SHADERNOMIP,		(void*)&eth32.guiAssets.titleRight },
	{ "winleft",		ASSET_SHADERNOMIP,		(void*)&eth32.guiAssets.winLeft },
	{ "wintop",			ASSET_SHADERNOMIP,		(void*)&eth32.guiAssets.winTop },
	{ "wintopl",		ASSET_SHADERNOMIP,		(void*)&eth32.guiAssets.winTopLeft },
	{ "wincenter",		ASSET_SHADERNOMIP,		(void*)&eth32.guiAssets.winCenter },
	{ "txtinputl",		ASSET_SHADERNOMIP,		(void*)&eth32.guiAssets.txtinputLeft },
	{ "txtinputc",		ASSET_SHADERNOMIP,		(void*)&eth32.guiAssets.txtinputCenter },
	{ "txtinputr",		ASSET_SHADERNOMIP,		(void*)&eth32.guiAssets.txtinputRight },
	{ "btnl",			ASSET_SHADERNOMIP,		(void*)&eth32.guiAssets.btnLeft },	
	{ "btnc",			ASSET_SHADERNOMIP,		(void*)&eth32.guiAssets.btnCenter },
	{ "btnr",			ASSET_SHADERNOMIP,		(void*)&eth32.guiAssets.btnRight },
	{ "btnsell",		ASSET_SHADERNOMIP,		(void*)&eth32.guiAssets.btnselLeft },
	{ "btnselc",		ASSET_SHADERNOMIP,		(void*)&eth32.guiAssets.btnselCenter },
	{ "btnselr",		ASSET_SHADERNOMIP,		(void*)&eth32.guiAssets.btnselRight },
	{ "check",			ASSET_SHADERNOMIP,		(void*)&eth32.guiAssets.check },
	{ "checkbox",		ASSET_SHADERNOMIP,		(void*)&eth32.guiAssets.checkBox },
	{ "mouse",			ASSET_SHADERNOMIP,		(void*)&eth32.guiAssets.mousePtr },
	{ "dropboxarrow",	ASSET_SHADERNOMIP,		(void*)&eth32.guiAssets.dropboxArrow },
	{ "scrollarrow",	ASSET_SHADERNOMIP,		(void*)&eth32.guiAssets.scrollbarArrow },
	{ "scrollbtn",		ASSET_SHADERNOMIP,		(void*)&eth32.guiAssets.scrollbarBtn },
	{ "scrolltrack",	ASSET_SHADERNOMIP,		(void*)&eth32.guiAssets.scrollbarTrack },
	{ "sliderbtn",		ASSET_SHADERNOMIP,		(void*)&eth32.guiAssets.sliderBtn },
	{ "slidertrack",	ASSET_SHADERNOMIP,		(void*)&eth32.guiAssets.sliderTrack },
	{ "camcorner",		ASSET_SHADERNOMIP,		(void*)&eth32.guiAssets.camCorner },
};
