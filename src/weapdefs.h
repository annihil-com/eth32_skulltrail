// ETH32 - an Enemy Territory cheat for windows
// Copyright (c) 2007 eth32 team
// www.cheatersutopia.com & www.nixcoders.org

#pragma once

// Weapon definitions to control aimbot/autofire/etc.
//
//	Going to try to share settings for mods that can, TCE is most definitely going to be a seperate list
//  NoQuarter adds a bunch of weapons, but may be able to add on to original list.. will try to capture weapon names/nums
//  as I can... right now, only etpub/etmain/shrub/etpro get assigned a list until I or someone gets the others defined
//  Also, eth32.cg.currentWeapon is assigned upon respawn and during weapon change.

// etmain, etpub, shrub, etpro, jaymod
static weapdef_t normalWeapons[] =
{
	// Name				Origin		Angle		Dist	Type			Attributes
	{ "None",			{0,0,0},	{0,0,0},	0,		WT_NONE,		WA_NONE											},	// 0
	{ "Knife",			{1.5,0,-2},	{300,0,0},	35,		WT_KNIFE,		WA_USER_DEFINED	| WA_NO_AMMO					},	// 1
	{ "Luger",			{-2.5,0,-2},{320,0,0},	30,		WT_PISTOL,		WA_USER_DEFINED | WA_BLOCK_FIRE					},	// 2
	{ "MP-40",			{-7.5,0,-5},{335,0,0},	50,		WT_SMG,			WA_USER_DEFINED | WA_BLOCK_FIRE					},	// 3
	{ "Grenade",		{0,0,-1},	{305,0,0},	30,		WT_EXPLOSIVE,	WA_ONLY_CLIP | WA_BALLISTIC	| WA_GRENADE		},	// 4 axis
	{ "Panzerfaust",	{-2.5,0,-8},{205,0,0},	85,		WT_HEAVY,		WA_PANZER | WA_ONLY_CLIP						},	// 5
	{ "Flamethrower",	{-5,0,-9},	{295,0,0},	45,		WT_HEAVY,		WA_ONLY_CLIP									},	// 6
	{ "Colt",			{-2.5,0,-2},{320,0,0},	30,		WT_PISTOL,		WA_USER_DEFINED									},	// 7
	{ "Thompson",		{0,0,-1},	{320,0,0},	65,		WT_SMG,			WA_USER_DEFINED | WA_BLOCK_FIRE					},	// 8
	{ "Grenade",		{0,0,0},	{335,0,0},	15,		WT_EXPLOSIVE,	WA_ONLY_CLIP | WA_BALLISTIC	| WA_GRENADE		},	// 9 allies
	{ "Sten",			{-4,0,0},	{335,0,0},	70,		WT_SMG,			WA_USER_DEFINED | WA_BLOCK_FIRE | WA_OVERHEAT	},	// 10
	{ "Syringe",		{0,0,-3.5},	{230,0,0},	20,		WT_NONE,		WA_ONLY_CLIP									},	// 11
	{ "Ammo",			{0,-4,0},	{0,0,0},	40,		WT_NONE,		WA_NO_AMMO										},	// 12
	{ "Arty ???",		{0,0,0},	{0,0,0},	0,		WT_NONE,		WA_NONE											},	// 13
	{ "Silenced Luger",	{-4,0,-3.5},{335,0,0},	45,		WT_PISTOL,		WA_USER_DEFINED | WA_BLOCK_FIRE					},	// 14
	{ "Dynamite",		{0,-3,0},	{320,0,0},	35,		WT_EXPLOSIVE,	WA_NO_AMMO										},	// 15
	{ "Smoketrail ???",	{0,0,0},	{0,0,0},	0,		WT_HEAVY,		WA_NONE											},	// 16
	{ "MapMortar",		{0,0,0},	{0,0,0},	0,		WT_HEAVY,		WA_NONE											},	// 17
	{ "BigExplosion",	{0,0,0},	{0,0,0},	0,		WT_HEAVY,		WA_NONE											},	// 18
	{ "Medkit",			{0,-4,-1.5},{0,0,0},	45,		WT_NONE,		WA_NO_AMMO										},	// 19
	{ "Binoculars",		{-3.5,-3,2},{0,0,0},	25,		WT_NONE,		WA_NO_AMMO										},	// 20
	{ "Pliers",			{0,-1,-1.5},{290,0,0},	20,		WT_NONE,		WA_NO_AMMO										},	// 21
	{ "Air Strike",		{0,-1.5,1.5},{0,0,0},	25,		WT_HEAVY,		WA_NO_AMMO										},	// 22
	{ "KAR-98",			{0,0,0},	{0,0,0},	0,		WT_RIFLE,		WA_USER_DEFINED | WA_BLOCK_FIRE					},	// 23
	{ "Carbine",		{-10,0,-10},{320,0,0},	45,		WT_RIFLE,		WA_USER_DEFINED | WA_BLOCK_FIRE					},	// 24
	{ "Garand",			{-5.5,0,-6.5},{320,0,0},55,		WT_SNIPER,		WA_USER_DEFINED | WA_BLOCK_FIRE | WA_UNSCOPED	},	// 25
	{ "Landmine",		{2,0,-6},	{240,0,0},	35,		WT_EXPLOSIVE,	WA_ONLY_CLIP									},	// 26
	{ "Satchel",		{0,-5.5,1.5},{0,0,335},	50,		WT_EXPLOSIVE,	WA_NO_AMMO										},	// 27
	{ "Detonator",		{0,0,-6},	{290,0,0},	30,		WT_NONE,		WA_NO_AMMO | WA_SATCHEL							},	// 28
	{ "Tripmine ??",	{0,0,0},	{0,0,0},	0,		WT_EXPLOSIVE,	WA_NONE											},	// 29
	{ "Smoke Grenade",	{0,-1.5,1.5},{0,0,0},	25,		WT_EXPLOSIVE,	WA_NO_AMMO										},	// 30
	{ "MG-42",			{0,0,-10},	{285,0,320},100,	WT_MG,			WA_USER_DEFINED | WA_BLOCK_FIRE | WA_OVERHEAT	},	// 31
	{ "K-43",			{-5.5,0,-6.5},{320,0,0},55,		WT_SNIPER,		WA_USER_DEFINED | WA_BLOCK_FIRE | WA_UNSCOPED	},	// 32
	{ "FG-42",			{-5.5,0,-6.5},{320,0,0},55,		WT_SNIPER,		WA_USER_DEFINED | WA_BLOCK_FIRE | WA_UNSCOPED	},	// 33
	{ "DummyMG42",		{0,0,0},	{0,0,0},	0,		WT_NONE,		WA_NONE											},	// 34
	{ "Mortar",			{0,5.5,10},	{35,0,0},	65,		WT_HEAVY,		WA_NONE											},	// 35 not deployed
	{ "Lock Pick",		{0,0,0},	{0,0,0},	0,		WT_NONE,		WA_NONE											},	// 36
	{ "Akimbo Colt",	{-2.5,0,-2},{320,0,0},	30,		WT_PISTOL,		WA_USER_DEFINED | WA_BLOCK_FIRE | WA_AKIMBO, 7	},	// 37
	{ "Akimbo Luger",	{-2.5,0,-2},{320,0,0},	30,		WT_PISTOL,		WA_USER_DEFINED | WA_BLOCK_FIRE | WA_AKIMBO, 2	},	// 38
	{ "Rifle Grenade",	{-10,0,-10},{320,0,0},	45,		WT_EXPLOSIVE,	WA_RIFLE_GRENADE | WA_BALLISTIC					},	// 39 axis
	{ "Rifle Grenade",	{-10,0,-10},{320,0,0},	45,		WT_EXPLOSIVE,	WA_RIFLE_GRENADE | WA_BALLISTIC					},	// 40 allies
	{ "Silenced Colt",	{-4,0,-3.5},{335,0,0},	45,		WT_PISTOL,		WA_USER_DEFINED | WA_BLOCK_FIRE					},	// 41
	{ "Garand (Scoped)",{-5.5,0,-6.5},{320,0,0},55,		WT_SNIPER,		WA_USER_DEFINED | WA_BLOCK_FIRE | WA_SCOPED, 25	},	// 42 scoped
	{ "K-43 (Scoped)",	{-5.5,0,-6.5},{320,0,0},55,		WT_SNIPER,		WA_USER_DEFINED | WA_BLOCK_FIRE | WA_SCOPED, 32	},	// 43 scoped
	{ "FG-42 (Scoped)",	{-5.5,0,-6.5},{320,0,0},55,		WT_SNIPER,		WA_USER_DEFINED | WA_BLOCK_FIRE | WA_SCOPED, 33	},	// 44 scoped
	{ "Mortar",			{0,5.5,10},	{35,0,0},	85,		WT_HEAVY,		WA_MORTAR										},	// 45 deployed set
	{ "Adrenaline",		{0,0,-3.5},	{230,0,0},	20,		WT_NONE,		WA_ONLY_CLIP									},	// 46
	{ "Silenced Colts",	{-4,0,-3.5},{335,0,0},	45,		WT_PISTOL,		WA_USER_DEFINED | WA_BLOCK_FIRE | WA_AKIMBO, 7	},	// 47
	{ "Silenced Lugers",{-4,0,-3.5},{335,0,0},	45,		WT_PISTOL,		WA_USER_DEFINED | WA_BLOCK_FIRE | WA_AKIMBO, 2	},	// 48
	{ "Mobile MG-42 ?",	{0,0,-10},	{285,0,320},100,	WT_MG,			WA_USER_DEFINED | WA_BLOCK_FIRE | WA_OVERHEAT	},	// 49
	// JAYMOD Additions
	{ "Poison Needle",	{0,0,0},	{0,0,0},	0,		WT_NONE,		WA_NONE											},	// 50
	{ "Unknown 51",		{0,0,0},	{0,0,0},	0,		WT_NONE,		WA_NONE											},	// 51
	{ "M97",			{0,0,0},	{0,0,0},	0,		WT_SHOTGUN,		WA_USER_DEFINED | WA_BLOCK_FIRE					},	// 52
};

static weapdef_t noquarterWeapons[] =
{
	// Name				Type			Attributes
	{ "None",			{0,0,0},	{0,0,0},	0,		WT_NONE,		WA_NONE											},	// 0
	{ "Knife",			{0,0,0},	{0,0,0},	0,		WT_KNIFE,		WA_USER_DEFINED	| WA_NO_AMMO					},	// 1
	{ "Luger",			{0,0,0},	{0,0,0},	0,		WT_PISTOL,		WA_USER_DEFINED | WA_BLOCK_FIRE					},	// 2
	{ "MP-40",			{0,0,0},	{0,0,0},	0,		WT_SMG,			WA_USER_DEFINED | WA_BLOCK_FIRE					},	// 3
	{ "Grenade",		{0,0,0},	{0,0,0},	0,		WT_EXPLOSIVE,	WA_ONLY_CLIP									},	// 4 axis
	{ "Panzerfaust",	{0,0,0},	{0,0,0},	0,		WT_HEAVY,		WA_PANZER										},	// 5
	{ "Flamethrower",	{0,0,0},	{0,0,0},	0,		WT_HEAVY,		WA_USER_DEFINED									},	// 6
	{ "Colt",			{0,0,0},	{0,0,0},	0,		WT_PISTOL,		WA_USER_DEFINED | WA_BLOCK_FIRE					},	// 7
	{ "Thompson",		{0,0,0},	{0,0,0},	0,		WT_SMG,			WA_USER_DEFINED | WA_BLOCK_FIRE					},	// 8
	{ "Grenade",		{0,0,0},	{0,0,0},	0,		WT_EXPLOSIVE,	WA_ONLY_CLIP									},	// 9 allies
	{ "Sten",			{0,0,0},	{0,0,0},	0,		WT_SMG,			WA_USER_DEFINED | WA_BLOCK_FIRE | WA_OVERHEAT	},	// 10
	{ "Syringe",		{0,0,0},	{0,0,0},	0,		WT_NONE,		WA_ONLY_CLIP									},	// 11
	{ "Ammo",			{0,0,0},	{0,0,0},	0,		WT_NONE,		WA_NO_AMMO										},	// 12
	{ "Arty ???",		{0,0,0},	{0,0,0},	0,		WT_HEAVY,		WA_NONE											},	// 13
	{ "Silenced Luger",	{0,0,0},	{0,0,0},	0,		WT_PISTOL,		WA_USER_DEFINED | WA_BLOCK_FIRE					},	// 14
	{ "Dynamite",		{0,0,0},	{0,0,0},	0,		WT_EXPLOSIVE,	WA_NONE											},	// 15
	{ "Smoketrail ???",	{0,0,0},	{0,0,0},	0,		WT_HEAVY,		WA_NONE											},	// 16
	{ "Unknown 17",		{0,0,0},	{0,0,0},	0,		WT_NONE,		WA_NONE											},	// 17
	{ "Medkit",			{0,0,0},	{0,0,0},	0,		WT_NONE,		WA_NO_AMMO										},	// 18
	{ "Binoculars",		{0,0,0},	{0,0,0},	0,		WT_NONE,		WA_NO_AMMO										},	// 19
	{ "Pliers",			{0,0,0},	{0,0,0},	0,		WT_NONE,		WA_NO_AMMO										},	// 20
	{ "Air Strike",		{0,0,0},	{0,0,0},	0,		WT_HEAVY,		WA_NO_AMMO										},	// 21
	{ "KAR98",			{0,0,0},	{0,0,0},	0,		WT_SMG,			WA_USER_DEFINED | WA_BLOCK_FIRE					},	// 22
	{ "Carbine",		{0,0,0},	{0,0,0},	0,		WT_RIFLE,		WA_USER_DEFINED | WA_BLOCK_FIRE					},	// 23
	{ "Garand",			{0,0,0},	{0,0,0},	0,		WT_RIFLE,		WA_USER_DEFINED | WA_BLOCK_FIRE | WA_UNSCOPED	},	// 24
	{ "Landmine",		{0,0,0},	{0,0,0},	0,		WT_EXPLOSIVE,	WA_ONLY_CLIP									},	// 25
	{ "Satchel",		{0,0,0},	{0,0,0},	0,		WT_EXPLOSIVE,	WA_NO_AMMO										},	// 26
	{ "Detonator",		{0,0,0},	{0,0,0},	0,		WT_NONE,		WA_NO_AMMO | WA_SATCHEL							},	// 27
	{ "Tripmine ??",	{0,0,0},	{0,0,0},	0,		WT_EXPLOSIVE,	WA_NONE											},	// 28
	{ "Smoke Grenade",	{0,0,0},	{0,0,0},	0,		WT_EXPLOSIVE,	WA_NO_AMMO										},	// 29
	{ "MG-42",			{0,0,0},	{0,0,0},	0,		WT_MG,			WA_USER_DEFINED | WA_BLOCK_FIRE | WA_OVERHEAT	},	// 30
	{ "K-43",			{0,0,0},	{0,0,0},	0,		WT_RIFLE,		WA_USER_DEFINED | WA_BLOCK_FIRE | WA_UNSCOPED	},	// 31
	{ "FG-42",			{0,0,0},	{0,0,0},	0,		WT_RIFLE,		WA_USER_DEFINED | WA_BLOCK_FIRE | WA_UNSCOPED	},	// 32
	{ "DummyMG42",		{0,0,0},	{0,0,0},	0,		WT_NONE,		WA_NONE											},	// 33
	{ "Mortar",			{0,0,0},	{0,0,0},	0,		WT_HEAVY,		WA_NONE											},	// 34 not deployed
	{ "Akimbo Colt",	{0,0,0},	{0,0,0},	0,		WT_PISTOL,		WA_USER_DEFINED | WA_BLOCK_FIRE | WA_AKIMBO, 7	},	// 35
	{ "Akimbo Luger",	{0,0,0},	{0,0,0},	0,		WT_PISTOL,		WA_USER_DEFINED | WA_BLOCK_FIRE | WA_AKIMBO, 2	},	// 36
	{ "Rifle Grenade",	{0,0,0},	{0,0,0},	0,		WT_EXPLOSIVE,	WA_RIFLE_GRENADE								},	// 37 axis
	{ "Rifle Grenade",	{0,0,0},	{0,0,0},	0,		WT_EXPLOSIVE,	WA_RIFLE_GRENADE								},	// 38 allies
	{ "Silenced Colt",	{0,0,0},	{0,0,0},	0,		WT_PISTOL,		WA_USER_DEFINED | WA_BLOCK_FIRE					},	// 39
	{ "Garand (Scoped)",{0,0,0},	{0,0,0},	0,		WT_SNIPER,		WA_USER_DEFINED | WA_BLOCK_FIRE | WA_SCOPED		},	// 40 scoped
	{ "K-43 (Scoped)",	{0,0,0},	{0,0,0},	0,		WT_SNIPER,		WA_USER_DEFINED | WA_BLOCK_FIRE | WA_SCOPED		},	// 41 scoped
	{ "FG-42",			{0,0,0},	{0,0,0},	0,		WT_SNIPER,		WA_USER_DEFINED | WA_BLOCK_FIRE | WA_SCOPED		},	// 42 scoped
	{ "Mortar",			{0,0,0},	{0,0,0},	0,		WT_HEAVY,		WA_MORTAR										},	// 43 deployed set
	{ "Adrenaline",		{0,0,0},	{0,0,0},	0,		WT_NONE,		WA_NONE											},	// 44
	{ "Silenced Colts",	{0,0,0},	{0,0,0},	0,		WT_PISTOL,		WA_USER_DEFINED | WA_BLOCK_FIRE | WA_AKIMBO, 7	},	// 45
	{ "Silenced Lugers",{0,0,0},	{0,0,0},	0,		WT_PISTOL,		WA_USER_DEFINED | WA_BLOCK_FIRE | WA_AKIMBO, 2	},	// 46
	{ "Mobile MG-42 ?",	{0,0,0},	{0,0,0},	0,		WT_MG,			WA_USER_DEFINED | WA_BLOCK_FIRE | WA_OVERHEAT	},	// 47
	{ "Shotgun",		{0,0,0},	{0,0,0},	0,		WT_SHOTGUN,		WA_USER_DEFINED | WA_BLOCK_FIRE					},	// 48
	{ "Knife",			{0,0,0},	{0,0,0},	0,		WT_KNIFE,		WA_USER_DEFINED	| WA_NO_AMMO					},	// 49
	{ "Browning",		{0,0,0},	{0,0,0},	0,		WT_MG,			WA_USER_DEFINED | WA_BLOCK_FIRE | WA_OVERHEAT	},	// 50
	{ "Browning Set",	{0,0,0},	{0,0,0},	0,		WT_MG,			WA_USER_DEFINED | WA_BLOCK_FIRE | WA_OVERHEAT	},	// 51
	{ "BAR",			{0,0,0},	{0,0,0},	0,		WT_SMG,			WA_USER_DEFINED | WA_BLOCK_FIRE					},	// 52
	{ "BAR Set",		{0,0,0},	{0,0,0},	0,		WT_SMG,			WA_USER_DEFINED | WA_BLOCK_FIRE					},	// 53
	{ "STG 44",			{0,0,0},	{0,0,0},	0,		WT_SMG,			WA_USER_DEFINED | WA_BLOCK_FIRE					},	// 54
	{ "Sten MKII",		{0,0,0},	{0,0,0},	0,		WT_SMG,			WA_USER_DEFINED | WA_BLOCK_FIRE					},	// 55
	{ "Bazooka",		{0,0,0},	{0,0,0},	0,		WT_HEAVY,		WA_ONLY_CLIP									},	// 56
	{ "MP-34",			{0,0,0},	{0,0,0},	0,		WT_SMG,			WA_USER_DEFINED | WA_BLOCK_FIRE	| WA_OVERHEAT	},	// 57
	{ "Firebolt",		{0,0,0},	{0,0,0},	0,		WT_NONE,		WA_NONE											},	// 58
	{ "Mortar 2",		{0,0,0},	{0,0,0},	0,		WT_HEAVY,		WA_NONE											},	// 59
	{ "Mortar 2 Set",	{0,0,0},	{0,0,0},	0,		WT_HEAVY,		WA_NONE											},	// 60
	{ "Venom",			{0,0,0},	{0,0,0},	0,		WT_MG,			WA_USER_DEFINED | WA_BLOCK_FIRE | WA_OVERHEAT	},	// 61
	{ "Poison Syringe",	{0,0,0},	{0,0,0},	0,		WT_NONE,		WA_NONE											},	// 62
};

// With TCE, don't set sniper weapons to scope... that way we don't modify the refdef.
// Actually, no weapons should have scope set for TCE

//tce 0.48/0.49/0.49b
static weapdef_t tceWeapons[] =
{
	// Name			Type			Attributes
	{ "None",		WT_NONE,		WA_NONE											},	// 0
	{ "Knife",		WT_KNIFE,		WA_USER_DEFINED					},	// 1
	{ "Beretta 92",		WT_PISTOL,		WA_USER_DEFINED | WA_BLOCK_FIRE			},	// 2
	{ "MAC 10 SD",		WT_SMG,			WA_USER_DEFINED | WA_BLOCK_FIRE			},	// 3
	{ "XM84 Stun",		WT_EXPLOSIVE,		WA_NONE						},	// 4
	{ "AKS-74U SD",		WT_SMG,			WA_USER_DEFINED | WA_BLOCK_FIRE			},	// 5
	{ "SPR",		WT_RIFLE,		WA_USER_DEFINED | WA_BLOCK_FIRE			},	// 6
	{ "Unknown 7",		WT_NONE,		WA_NONE						},	// 7
	{ "MP4/40",		WT_SMG,			WA_USER_DEFINED | WA_BLOCK_FIRE			},	// 8
	{ "Mk3A2 Grenade",	WT_EXPLOSIVE,		WA_NONE						},	// 9
	{ "MAC 10",		WT_SMG,			WA_USER_DEFINED | WA_BLOCK_FIRE			},	// 10
	{ "Unknown 11",		WT_NONE,		WA_NONE						},	// 11
	{ "Unknown 12",		WT_NONE,		WA_NONE						},	// 12
	{ "SIG550",		WT_RIFLE,		WA_USER_DEFINED | WA_BLOCK_FIRE			},	// 13
	{ "Unknown 14",		WT_NONE,		WA_NONE						},	// 14
	{ "Unknown 15",		WT_NONE,		WA_NONE						},	// 15
	{ "Unknown 16",		WT_NONE,		WA_NONE						},	// 16
	{ "Unknown 17",		WT_NONE,		WA_NONE						},	// 17
	{ "Unknown 18",		WT_NONE,		WA_NONE						},	// 18
	{ "Unknown 19",		WT_NONE,		WA_NONE						},	// 19
	{ "Unknown 20",		WT_NONE,		WA_NONE						},	// 20
	{ "Unknown 21",		WT_NONE,		WA_NONE						},	// 21
	{ "Unknown 22",		WT_NONE,		WA_NONE						},	// 22
	{ "AK-74M SPETSNAZ",	WT_SMG,			WA_USER_DEFINED | WA_BLOCK_FIRE			},	// 23
	{ "SIG552",		WT_RIFLE,		WA_USER_DEFINED | WA_BLOCK_FIRE			},	// 24
	{ "SR8",		WT_RIFLE,		WA_USER_DEFINED | WA_BLOCK_FIRE			},	// 25
	{ "Unknown 26",		WT_NONE,		WA_NONE						},	// 26
	{ "Unknown 27",		WT_NONE,		WA_NONE						},	// 27
	{ "Unknown 28",		WT_NONE,		WA_NONE						},	// 28
	{ "Unknown 29",		WT_NONE,		WA_NONE						},	// 29
	{ "M83 Smoke",		WT_EXPLOSIVE,		WA_NONE						},	// 30
	{ "Unknown 31",		WT_NONE,		WA_NONE						},	// 31
	{ "Unknown 32",		WT_NONE,		WA_NONE						},	// 32
	{ "AK47",		WT_SMG,			WA_USER_DEFINED | WA_BLOCK_FIRE			},	// 33
	{ "Unknown 34",		WT_NONE,		WA_NONE						},	// 34
	{ "Unknown 35",		WT_NONE,		WA_NONE						},	// 35
	{ "Unknown 36",		WT_NONE,		WA_NONE						},	// 36
	{ "Unknown 37",		WT_NONE,		WA_NONE						},	// 37
	{ "Unknown 38",		WT_NONE,		WA_NONE						},	// 38
	{ "Glock",		WT_PISTOL,		WA_USER_DEFINED | WA_BLOCK_FIRE			},	// 39
	{ "Desert Eagle",	WT_PISTOL,		WA_USER_DEFINED | WA_BLOCK_FIRE			},	// 40
	{ "MP5 SD",		WT_SMG,			WA_USER_DEFINED | WA_BLOCK_FIRE			},	// 41
	{ "UMP45",		WT_SMG,			WA_USER_DEFINED | WA_BLOCK_FIRE			},	// 42
	{ "AKS-74U",		WT_SMG,			WA_USER_DEFINED | WA_BLOCK_FIRE			},	// 43
	{ "M4",			WT_SMG,			WA_USER_DEFINED | WA_BLOCK_FIRE			},	// 44
	{ "M16",		WT_SMG,			WA_USER_DEFINED | WA_BLOCK_FIRE			},	// 45
	{ "R93",		WT_RIFLE,		WA_USER_DEFINED | WA_BLOCK_FIRE			},	// 46
	{ "PSG-1",		WT_RIFLE,		WA_USER_DEFINED | WA_BLOCK_FIRE			},	// 47
	{ "M3S90",		WT_SHOTGUN,		WA_USER_DEFINED | WA_BLOCK_FIRE			},	// 48
	{ "SPAS12",		WT_SHOTGUN,		WA_USER_DEFINED | WA_BLOCK_FIRE			},	// 49
	{ "M4 SOPMOD",		WT_SMG,			WA_USER_DEFINED | WA_BLOCK_FIRE			},	// 50
	{ "M76",		WT_RIFLE,		WA_USER_DEFINED | WA_BLOCK_FIRE			},	// 51
	{ "Unknown 52",		WT_NONE,		WA_NONE						},	// 52
	{ "Unknown 53",		WT_NONE,		WA_NONE						},	// 53
	{ "Unknown 54",		WT_NONE,		WA_NONE						},	// 54
	{ "Unknown 55",		WT_NONE,		WA_NONE						},	// 55
	{ "Unknown 56",		WT_NONE,		WA_NONE						},	// 56
	{ "Unknown 57",		WT_NONE,		WA_NONE						},	// 57
	{ "Unknown 58",		WT_NONE,		WA_NONE						},	// 58
	{ "Unknown 59",		WT_NONE,		WA_NONE						},	// 59
	{ "Unknown 60",		WT_NONE,		WA_NONE						},	// 60
	{ "Unknown 61",		WT_NONE,		WA_NONE						},	// 61
	{ "Unknown 62",		WT_NONE,		WA_NONE						},	// 62
	{ "Unknown 63",		WT_NONE,		WA_NONE						},	// 63
};

// kobject: added just in case so currentWeapon is always valid
extern weapdef_t nullWeapon;
