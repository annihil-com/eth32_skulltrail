// ETH32 - an Enemy Territory cheat for windows
// Copyright (c) 2007 eth32 team
// www.cheatersutopia.com & www.nixcoders.org

#pragma once

static et_t etlist[] =
{
	{
		"2.55",                 // version
		0xd88f0f0a,             // crc32
		ET_255,					// ver

		0x150B1EC,              // viewAngleX
		0x150B1E8,              // viewAngleY
		0x850B80,				// inButtons
		0x14E3264,				// glConfig
		0x13CFE24,				// com_frameTime
		0x14E4ECC,				// consoleshader

		0x421D15,				// etroq video
		" ",					// etroq safe

		0x445810,               // syscall
		0x422E80,               // Cvar_Get
		0x423110,				// Cvar_Set2
		0x9FFCC8,               // fs_searchpaths
		0x427700,               // FS_PureServerSetLoadedPaks
		0x427D40,				// FS_AddGameDirectory
		0x0,					// FS_AddGameDirectory2
		0x4279A0,				// FS_Startup
		0x0,					// FS_Startup2
		0x41E8A0,               // Cmd_AddCommand
		0x4459E0,				// VM_Create
		0x4496B0,				// Sys_LoadDll
		0x46EDD0,				// LoadTGA
		0x1455F34,				// server_ip
	},
	{
		"2.56",                 // version
		0x556af704,             // crc32
		ET_256,					// ver

		0x15086EC,              // viewAngleX
		0x15086E8,              // viewAngleY
		0x84DA90,				// inButtons
		0x14E0764,				// glConfig
		0x13CD324,				// com_frameTime
		0x14E23CC,				// consoleshader

		0x421D05,				// etroq video
		" ",					// etroq safe

		0x445DA0,               // syscall
		0x422E70,               // Cvar_Get
		0x424950,				// Cvar_Set2
		0x9FCBD8,               // fs_searchpaths
		0x41E770,               // FS_PureServerSetLoadedPaks
		0x427D30,				// FS_AddGameDirectory
		0x0,					// FS_AddGameDirectory2
		0x427990,				// FS_Startup
		0x0,					// FS_Startup2
		0x41E890,               // Cmd_AddCommand
		0x445F70,				// VM_Create
		0x449CE0,				// Sys_LoadDll
		0x46EE40,				// LoadTGA
		0x1453434,				// server_ip
	},
	{
		"2.60",                 // version
		0x539e3563,             // crc32
		ET_260,					// ver

		0x13EEA8C,              // viewAngleX
		0x13EEA88,              // viewAngleY
		0x835AF8,				// inButtons
		0x15D0924,				// glConfig
		0x13C08E0,				// com_frameTime
		0x15D2588,				// consoleshader

		0x4246F0,				// etroq video
		" ",					// etroq safe

		0x448780,               // syscall
		0x425760,               // Cvar_Get
		0x424950,				// Cvar_Set2
		0x9DAA94,               // fs_searchpaths
		0x429620,               // FS_PureServerSetLoadedPaks
		0x0,					// FS_AddGameDirectory
		0x428720,				// FS_AddGameDirectory2
		0x0,					// FS_Startup
		0x428E40,				// FS_Startup2
		0x41FD20,               // Cmd_AddCommand
		0x448CD0,				// VM_Create
		0x44CDA0,				// Sys_LoadDll
		0x47B2D0,				// LoadTGA
		0x165F7C8,				// server_ip
	},
	{
		"2.60b",                // version
		0x4bc60dd5,             // crc32
		ET_260b,				// ver

		0x013EEA8C,             // viewAngleX
		0x013EEA88,             // ViewAngleY
		0x835AF8,				// inButtons
		0x15D0924,				// glConfig
		0x13C08E0,				// com_frameTime
		0x15D2588,				// consoleshader

		0x4246F0,				// etroq video
		" ",					// etroq safe

		0x4488A0,               // syscall
		0x425760,               // Cvar_Get
		0x424950,				// Cvar_Set2
		0x9DAA94,               // fs_searchpaths
		0x429620,               // FS_PureServerSetLoadedPaks
		0x0,					// FS_AddGameDirectory
		0x428720,				// FS_AddGameDirectory2
		0x0,					// FS_Startup
		0x428E40,				// FS_Startup2
		0x41FD20,               // Cmd_AddCommand
		0x448DF0,				// VM_Create
		0x44CEC0,				// Sys_LoadDll
		0x47B460,				// LoadTGA
		0x165F7C8,				// server_ip
	},
};
