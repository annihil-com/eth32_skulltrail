// ETH32 - an Enemy Territory cheat for windows
// Copyright (c) 2007 eth32 team
// www.cheatersutopia.com & www.nixcoders.org

#pragma once

/* Communication between ETH32 and portal */

#ifndef uint32_t
typedef unsigned int uint32_t;
#endif
#ifndef uchar
typedef unsigned char uchar;
#endif

typedef unsigned char 	uint8_t;

#define MAX_PORTAL_USERS	256
#define MAX_PORTAL_SERVERS	256
#define FRIENDS_SIZE		4096

class CPortal
{
public:
	CPortal();
	~CPortal();
	bool Auth();
	void Init();
	void UpdateTracker();
	bool ParseUserInfo(const char *string, int num);
	void RetrieveUsers();
	const char *GetUsername(void) { return username; }
	void AddFriend(const char *user);
	void RemoveFriend(const char *user);
	bool IsFriend(const char *user);
	void LoadSettings(const char *filename);
	void SaveSettings(const char *filename);

	void ConsoleCommand(void);
	void ConsoleList(void);
	void ConsoleJoin(void);

	eth32user_t users[MAX_PORTAL_USERS];
	int numUsers;

	eth32server_t servers[MAX_PORTAL_SERVERS];
	int numServers;
private:
	void DebugExtraUsers(int num);
	void UpdateServers(int usernum);

	char auth_string[32];
	void *trackerThread;

	char username[64];
	char password[64];

	char friends[FRIENDS_SIZE];

	CNetwork net;
};

extern CPortal Portal;

DWORD PortalThread(void);

#define	AUTH_FILE		"auth.ini"
#define UPDATE_INTERVAL	30000	// 30 seconds tracker update interval
#define INITIAL_WAIT	2000	// 2 second delay before first update request
