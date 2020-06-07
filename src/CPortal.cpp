// ETH32 - an Enemy Territory cheat for windows
// Copyright (c) 2007 eth32 team
// www.cheatersutopia.com & www.nixcoders.org

#include "eth32.h"
#include "codetable.h"

// disable insecure string functions warning C4996
#pragma warning (disable:4996)

CPortal Portal;

uint32_t nextrnd;

__forceinline uint32_t _rand(void) {
	nextrnd = nextrnd * 1103515245 + 12345;
	return((uint32_t)(4294967296.0*(((uint32_t)(nextrnd/65536) % 32768)/(32768+1.0))));
}

// modified web-safe base64 encode ( map 3bytes -> 4 printable chars )
__forceinline int _b64w_encode(char* dest, const char* str, int len)
{
    int i;
    const uint8_t* s = (const uint8_t*) str;
    uint8_t* p = (uint8_t*) dest;

    /* unsigned here is important! */
    /* uint8_t is fastest on G4, amd */
    /* uint32_t is fastest on Intel */
    uint32_t t1, t2, t3;

    for (i = 0; i < len - 2; i += 3) {
        t1 = s[i]; t2 = s[i+1]; t3 = s[i+2];
        *p++ = e0[t1];
        *p++ = e1[((t1 & 0x03) << 4) | ((t2 >> 4) & 0x0F)];
        *p++ = e1[((t2 & 0x0F) << 2) | ((t3 >> 6) & 0x03)];
        *p++ = e2[t3];
    }
	
	// handle padding (apped a '&', php-safe)
    switch (len - i) {
    case 0:
        break;
    case 1:
        t1 = s[i];
        *p++ = e0[t1];
        *p++ = e1[(t1 & 0x03) << 4];
        *p++ = '&';
        *p++ = '&';
        break;
    default: /* case 2 */
        t1 = s[i]; t2 = s[i+1];
        *p++ = e0[t1];
        *p++ = e1[((t1 & 0x03) << 4) | ((t2 >> 4) & 0x0F)];
        *p++ = e2[(t2 & 0x0F) << 2];
        *p++ = '&';
    }

    *p = '\0';
    return p - (uint8_t*)dest;
}

__forceinline int _b64w_decode(char* dest, const char* src, int len)
{
	if (len == 0) return 0;
    /*
	* if padding is used, then the message must be at least
	* 4 chars and be a multiple of 4
	*/
	if (len < 4 || (len % 4 != 0)) return -1; /* error */
	/* there can be at most 2 pad chars at the end */
	if (src[len-1] == '&') {
		len--;
		if (src[len -1] == '&') {
			len--;
		}
	}

	int i;
	int leftover = len % 4;
	int chunks = (leftover == 0) ? len / 4 - 1 : len /4;

	uint8_t* p = (uint8_t*) dest;
	uint32_t x = 0;
	uint32_t* destInt = (uint32_t*) p;
	uint32_t* srcInt = (uint32_t*) src;
	uint32_t y = *srcInt++;
	for (i = 0; i < chunks; ++i) {
		x = d0[y & 0xff] |
				d1[(y >> 8) & 0xff] |
				d2[(y >> 16) & 0xff] |
				d3[(y >> 24) & 0xff];

		if (x >= 0x01FFFFFF) return -1;
		*destInt = x ;
		p += 3;
		destInt = (uint32_t*)p;
		y = *srcInt++;}


		switch (leftover) {
			case 0:
				x = d0[y & 0xff] |
						d1[(y >> 8) & 0xff] |
						d2[(y >> 16) & 0xff] |
						d3[(y >> 24) & 0xff];

				if (x >= 0x01FFFFFF) return -1;
				*p++ =  ((uint8_t*)(&x))[0];
				*p++ =  ((uint8_t*)(&x))[1];
				*p =    ((uint8_t*)(&x))[2];
				return (chunks+1)*3;
				break;
			case 2: // * case 2, 1  output byte */
				x = d0[y & 0xff] | d1[y >> 8 & 0xff];
				*p = *((uint8_t*)(&x)); // i.e. first char
				break;
			default: /* case 3, 2 output bytes */
				x = d0[y & 0xff] |
						d1[y >> 8 & 0xff ] |
						d2[y >> 16 & 0xff];  /* 0x3c */
				*p++ =  ((uint8_t*)(&x))[0];
				*p =  ((uint8_t*)(&x))[1];
				break;
		}

		if (x >= 0x01FFFFFF) return -1;

		return 3*chunks + (6*leftover)/8;
}

// add the key first, and the encoded message after
__forceinline int pencode(uchar *out, uchar *in, int len, int key)
{
	// assume key length 24bit
	// create padding if message is not a multiple of key-length
	int r = 3 - len % 3;

	uchar *in_b = (uchar *)malloc(len+r+1);

	int i=0;
	unsigned o;
	unsigned p;

	for (; i<len; i++ )
		in_b[i] = in[i];
	for (i=len; i<len+r; i++)
		in_b[i] = '&';
	in_b[i] = '\0';

	// add the key
	out[0] = (uchar) key & 0xff;
	out[1] = (uchar) (key >> 8) & 0xff;
	out[2] = (uchar) (key >> 16) & 0xff;
	int ofs = 3;
	
	for( i=0;i<(len+r)/3; i++ ){
		p = (unsigned)in_b[i*3+0] + ((unsigned)in_b[i*3+1] << 8) + ((unsigned)in_b[i*3+2] << 16);
		o = p ^ key;
		out[ofs+i*3+0] = (uchar) o & 0xff;
		out[ofs+i*3+1] = (uchar) (o >> 8) & 0xff;
		out[ofs+i*3+2] = (uchar) (o >> 16) & 0xff;
	}

	free(in_b);

	return len+r+ofs;
}

__forceinline uchar *pdecode(uchar *in, int len)
{
	// assume key length 24bit
	char *out = (char *)malloc(len+3);

	memset(out,0,len+3);

	int i=0;
	unsigned o;
	unsigned key;
	unsigned p;

	// retrieve the key
	key = ((unsigned)in[0] & 0xff) + (((unsigned)in[1] & 0xff) << 8) + (((unsigned)in[2] & 0xff) << 16);
	int ofs = 3;

	for( i=0;i<(len-ofs)/3; i++ ){
		p = ((unsigned)in[ofs+i*3+0] & 0xff) + (((unsigned)in[ofs+i*3+1] & 0xff) << 8) + (((unsigned)in[ofs+i*3+2] & 0xff) << 16);
		o = p ^ key;
		out[i*3+0] = (char) o & 0xff;
		out[i*3+1] = (char) (o >> 8) & 0xff;
		out[i*3+2] = (char) (o >> 16) & 0xff;
	}
	out[i*3] = '\0';

	return (uchar *)out;
}

/********************
* Portal User Sort
********************/

int PortalUsersSort(const void *u1, const void *u2)
{
	const eth32user_t *a = (eth32user_t *) u1;
	const eth32user_t *b = (eth32user_t *) u2;

	if (a->player && b->player)
		return (_stricmp(a->username, b->username));
	else if (a->player)
		return -1;
	else if (b->player)
		return 1;
	else if (a->frnd && b->frnd)
		return (_stricmp(a->username, b->username));
	else if (a->frnd)
		return -1;
	else if (b->frnd)
		return 1;
	else 
		return (_stricmp(a->username, b->username));
}

/********************
* CPortal
********************/

CPortal::CPortal()
{
	memset(users, 0, sizeof(users));
	numUsers = 0;

	memset(servers, 0, sizeof(servers));
	numServers = 0;

	memset(friends, 0, sizeof(friends));
}

CPortal::~CPortal()
{

}

bool CPortal::Auth()
{
return true;
 
 // Lots of random/trash code inside just to make reversing a bit harder
    char smth[10];
	TRASH60
	TRASH61
	char authFile[MAX_PATH];
	sprintf_s(authFile, MAX_PATH, "%s%s", eth32.path, AUTH_FILE);

	TRASH62
	TRASH63
	TRASH64
	GetPrivateProfileString("Auth", "Username", "demoUser", this->username, sizeof(this->username), authFile);
	GetPrivateProfileString("Auth", "Password", "demoPass", this->password, sizeof(this->password), authFile);

	// replace username with us, password with pw, etc...
	char msg2[512];
	char msg[512];
	TRASH74
	sprintf(msg,"us=%s&pw=%s&cl=%s&crc=0x%x", this->username, this->password, ETH32_VERSION, eth32.eth32CRC);

	// instead of using (tracable) calls to time() or rand() generate a seed on the cpu timestamp counter
	_asm 
	{
		push eax
		push edx
		rdtsc
		xor eax, edx
		mov nextrnd, eax
		pop edx
		pop eax
	}

	uint32_t key = _rand() >> 8;
	int mlen = pencode( (uchar *)msg2, (uchar *)msg, strlen(msg), key );
	_b64w_encode(msg, msg2, mlen);
	TRASH65
	
	char authstring[512];
	sprintf(authstring, "/auth.php?q=%s", msg);
	TRASH66

	smth[1] = 'a';
	smth[5] = ':';
	char *response = this->net.Get("eth32.nixcoders.org", authstring);

#ifdef ETH32_DEBUG
	Debug.Log("authpost:  %s", authstring);
#endif

	TRASH67

	if (!response || !*response)
		return false;

	#define AUTHRESPONSE	"Valid: "
	char *tmp = strstr(response, AUTHRESPONSE);
	TRASH68
	smth[6] = ' ';
	smth[4] = 'd';
	TRASH69
	TRASH90
	TRASH100
	smth[2] = 'l';
	TRASH81
	//this->trackerThread = CTHREAD(PortalThread);
	TRASH82
	TRASH70
	if (!tmp) return false;
	TRASH71
	smth[0] = 'V';
	TRASH72

	tmp += strlen(AUTHRESPONSE);
	smth[3] = 'i';
	TRASH73
	smth[7] = '1';
	TRASH74
	if (*tmp == '1') return 1;
	
	TRASH75
	int fake = !strcmp("Authed OK", smth);
	TRASH76
	TRASH77
	return fake;
	TRASH78
	TRASH79
	TRASH80
}

void CPortal::Init()
{
	static bool init = false;

	int i;
	// reset all eth32 flags, in case of server change
	for (i=0 ; i<MAX_CLIENTS ; i++) {
		eth32.cg.players[i].eth32user = false;
		eth32.cg.players[i].eth32friend = false;
	}

	// if we already have portal info, reassign everything in case we had just changed servers
	if (numUsers) {
		for (i=0 ; i<numUsers ; i++)
		{
			if (!strcmp(eth32.server, users[i].server) && users[i].clientNum >= 0 && users[i].clientNum < MAX_CLIENTS) {
				users[i].player = &eth32.cg.players[users[i].clientNum];
				users[i].player->eth32user = true;
				users[i].player->eth32friend = users[i].frnd;
			}
			else
				users[i].player = NULL;

			// update our client num and mod in case of server change
			if (!strcmp(users[i].username, username)) {
				users[i].clientNum = eth32.cg.clientNum;
				users[i].modVersion = eth32.cgMod->type;
			}

			users[i].anon = (strcmp(users[i].server, "255.255.255.255:255") == 0);
		}
		qsort(users, numUsers, sizeof(eth32user_t), PortalUsersSort);
	}

	
	if (!init) {
//#ifdef ETH32_THREADED
		char settingsFile[MAX_PATH];
		sprintf_s(settingsFile, MAX_PATH, "%s\\%s", eth32.path, ETH32_SETTINGS);
		LoadSettings(settingsFile);

		this->trackerThread = CTHREAD(PortalThread);
//#endif
		init = true;
	}
}

void CPortal::UpdateTracker()
{
	// Sanity checks
	// sol: no longer sanity checks, completely necessary now that we are 
	//      running portal thread before connecting to a server
	if (!eth32.cgameLoaded)
		return;

	if (!eth32.cgMod)
		return;

	if (!eth32.et)
		return;

	if (!eth32.cg.time)
		return;

	char updatestr[512];
	char msg[512];

	bool anon = eth32.settings.portalAnon;
	sprintf_s(updatestr, sizeof(updatestr), "/userupdate.php?q=username=%s&password=%s&client=%s&server_ip=%s&server_mod=%i&et_ver=%i&clientnum=%i&server_name=%s&server_mapname=%s", this->username, this->password, ETH32_VERSION, (anon)?"255.255.255.255:255":eth32.server, eth32.cgMod->type, eth32.et->ver, eth32.cg.clientNum, (anon)?"anonymous":eth32.cg.serverName, (anon)?"anonymous":eth32.cg.serverMapName);

	uint32_t key = _rand() >> 8;
	int mlen = pencode( (uchar *)msg, (uchar *)(updatestr+18), strlen(updatestr+18), key );
	_b64w_encode(updatestr+18, msg, mlen);

	eth32.cg.lastTrackerUpdate = eth32.cg.time;

	char *ret = this->net.Get("eth32.nixcoders.org", updatestr);

#ifdef ETH32_DEBUG
	Debug.Log("Updating tracker, time: %i", eth32.cg.time);
#endif
}

void CPortal::UpdateServers(int usernum)
{
	eth32user_t *u = &users[usernum];

	// don't add anonymous users to server list
	if (u->anon)
		return;

	// search for a current rampage that matches
	for (int i=0 ; i<numServers ; i++)
	{
		// found a match
		if (!strcmp(u->server, servers[i].serverAddr)) {
			servers[i].numUsers++;
			servers[i].rampage = servers[i].numUsers >= eth32.settings.portalRampageNum;			

			if (u->frnd)
				servers[i].numFriends++;

			return;
		}
	}

	// if space available, append a new server
	if (numServers < MAX_PORTAL_SERVERS) {
		strcpy_s(servers[numServers].serverAddr, 32, u->server);
		strcpy_s(servers[numServers].serverName, 64, u->serverName);
		strcpy_s(servers[numServers].serverMapName, 64, u->serverMapName);

		servers[numServers].numUsers = 1;		
		servers[numServers].numFriends = (u->frnd) ? 1 : 0;
		//servers[numServers].rampage = false;
		servers[numServers].rampage = servers[numServers].numUsers >= eth32.settings.portalRampageNum;

		numServers++;
		return;
	}
}

bool CPortal::ParseUserInfo(const char *string, int num)
{
	if (!string || !*string) 
		return false;

	StringTokenizer userstring(string, "\\");

	while (userstring.hasMoreTokens()) {
		char datatype = userstring.nextToken().c_str()[0];
		std::string data(userstring.nextToken());

		if (datatype == 'u') {
			// don't add us to list
			if (!strcmp(data.c_str(), this->username))
				return false;	
			strncpy_s(users[num].username, 64, data.c_str(), _TRUNCATE);
		}
		else if (datatype == 'i')
			strncpy_s(users[num].server, 32, data.c_str(), _TRUNCATE);
		else if (datatype == 'e')
			users[num].etVersion = atoi(data.c_str());
		else if (datatype == 'm')
			users[num].modVersion = atoi(data.c_str());
		else if (datatype == 'v')
			users[num].eth32Version = atof(data.c_str());
		else if (datatype == 'c')
			users[num].clientNum = atoi(data.c_str());
		else if (datatype == 'n')
			strncpy_s(users[num].serverName, 50, data.c_str(), _TRUNCATE);
		else if (datatype == 'z')
			strncpy_s(users[num].serverMapName, 25, data.c_str(), _TRUNCATE);
	}

	users[num].anon = (strcmp(users[num].server, "255.255.255.255:255") == 0);
	users[num].frnd = IsFriend(users[num].username);

	if (eth32.cgameLoaded && !strcmp(eth32.server, users[num].server) && users[num].clientNum >= 0 && users[num].clientNum < MAX_CLIENTS) {
		users[num].player = &eth32.cg.players[users[num].clientNum];
		strcpy_s(users[num].player->portalName, sizeof(users[num].player->portalName), users[num].username);
		users[num].player->eth32user = true;
		users[num].player->eth32friend = users[num].frnd;
	}
	else
		users[num].player = NULL;

	UpdateServers(num);	

	return true;
}

void CPortal::RetrieveUsers(void)
{
	char updatestr[] = "/getactiveusers.php";
	char *ret = this->net.Get("eth32.nixcoders.org", updatestr);

	if (!ret)
		return;

	StringTokenizer raw(ret, "\\users\\");
	std::string header(raw.nextToken());
	StringTokenizer userinfo(raw.remainingString(), "\n");

	// reset eth32 user flag on update
	for (int k=0 ; k<MAX_CLIENTS ; k++) {
		eth32.cg.players[k].eth32user = false;
		eth32.cg.players[k].eth32friend = false;
	}
		
	numUsers = 0;
	numServers = 0;

	int num = atoi(userinfo.nextToken().c_str());
	int validNum = 0;

#ifdef ETH32_DEBUG
	Debug.Log("Active players retrieved: %i", num);
#endif

	if (num) {
		if (num > MAX_PORTAL_USERS)
			num = MAX_PORTAL_USERS;

		for (int i=0 ; i<num ; i++)
		{
			std::string user = userinfo.nextToken();
			if (ParseUserInfo(user.c_str(), validNum))
				validNum++;
		}
	}

	numUsers = validNum;

/*#ifdef ETH32_DEBUG
	srand(timeGetTime());
	DebugExtraUsers(rand() % 100);
#endif*/

	if (eth32.cgameLoaded && eth32.settings.portalRampageNotify) {
		for (int i=0 ; i<numServers ; i++)
		{
			if (servers[i].rampage && strcmp(eth32.server, servers[i].serverAddr)) {
				char msg[1024];
				
				sprintf_s(msg, sizeof(msg), "^1RAMPAGE in progress!!!\n^o%s\n^2( ^o%i ^dusers ^2| ^o%i ^dfriends ^2)", 
					servers[i].serverName, servers[i].numUsers, servers[i].numFriends);

				Gui.CenterPrint(msg, 100);
				Sleep(1700);
			}
		}
	}

	qsort(users, numUsers, sizeof(eth32user_t), PortalUsersSort);

	// sol: weird crashes caused by this, probably trying to post while loading
	/*if (eth32.cgameLoaded) 
		Gui.BufferConsoleMessage("^dTracker updated!\n");
	else Syscall.UI_Print("^dTracker updated!\n");*/
}

void CPortal::DebugExtraUsers(int num)
{
	if  ((numUsers + num) > MAX_PORTAL_USERS)
		num = MAX_PORTAL_USERS - numUsers;

	if (num < 1 || !numUsers) 
		return;

	for (int i=numUsers ; i<(numUsers+num) ; i++)
	{
		sprintf_s(users[i].username, sizeof(users[i].username),"tempuser%02i", i);
		sprintf_s(users[i].server, sizeof(users[i].server),"%i.%i.%i.%i:%i", rand()%256, rand()%256, rand()%256, rand()%256, rand()%64000);
		sprintf_s(users[i].serverMapName, sizeof(users[i].serverMapName),"Baserase v%i", i);
		sprintf_s(users[i].serverName, sizeof(users[i].serverName),"Some Server %02i", i);
		users[i].clientNum = rand() % 64;
		users[i].etVersion = rand() % 4;
		users[i].modVersion = rand() % 5;
	}

	numUsers += num;
}

void CPortal::AddFriend(const char *user)
{
	if (IsFriend(user))
		return;

	if ((strlen(user) + strlen(friends) + 2) > FRIENDS_SIZE)
		return;  // friends string too full already

	char buf[128];
	
	if (*friends) {
		sprintf_s(buf,sizeof(buf)," %s", user);
		strcat_s(friends, sizeof(friends), buf);
	}
	else {
		strcpy_s(friends, sizeof(friends), user);
	}

	for (int i=0 ; i<numUsers ; i++)
	{
		if (!strcmp(user, users[i].username)) {
			users[i].frnd = true;

			if (users[i].player)
				users[i].player->eth32friend = true;

			break;
		}
	}
	qsort(users, numUsers, sizeof(eth32user_t), PortalUsersSort);
}

void CPortal::RemoveFriend(const char *user)
{
	if (!user || !*user || !IsFriend(user))
		return;

	char *d = strstr(friends, user);
	char *s = d + strlen(user);

	if (!d || !s)
		return;	// sanity check

	if (*s) 
		s++;

	strcpy_s(d, FRIENDS_SIZE - (d - friends), s);
	
	for (int i=0 ; i<numUsers ; i++)
	{
		if (!strcmp(user, users[i].username)) {
			users[i].frnd = false;

			if (users[i].player)
				users[i].player->eth32friend = false;

			break;
		}
	}
	qsort(users, numUsers, sizeof(eth32user_t), PortalUsersSort);
}

bool CPortal::IsFriend(const char *user)
{
	if (!friends[0] || !user || !*user)
		return false;

	char *p = strstr(friends, user);
	
	if (!p)
		return false;

	if (p == friends || *(p-1) == ' ') {
		int len = strlen(user);
		if (*(p+len) == ' ' || *(p+len) == '\0')
			return true;
		else
			return false;
	}
	else
		return false;
}

void CPortal::LoadSettings(const char *filename)
{
	GetPrivateProfileString("Main Settings", "friends", "\0", friends, sizeof(friends), filename);
}	

void CPortal::SaveSettings(const char *filename)
{
	WritePrivateProfileString("Main Settings", "friends", friends, filename);	
}

void CPortal::ConsoleCommand(void)
{
	if (Syscall.UI_Argc() == 1) {
		ConsoleList();
	} else {
		const char *cmd = Syscall.UI_Argv(1);
		
		if (!strcmpi("join", cmd)) {
			ConsoleJoin();
		}
		else {
			Syscall.UI_Print("\n^1ERROR: ^d%s ^ois not a valid tracker command\n\n", cmd);
		}
	}
}

void CPortal::ConsoleList(void)
{
	bool altColor = false;
	bool rampages = false;

	Syscall.UI_Print("\n^2*** ^dETH32 Traqur ^2****\n\n");

	if (numServers) {
		char sName[33];
		char sMap[23];
		char sAddr[22];

		Syscall.UI_Print("^d%-32s   %-22s   %-21s   %-3s   %-3s\n", "server", "map", "ip", "u", "f");
		Syscall.UI_Print("^2%-32s   %-22s   %-21s   %-3s   %-3s\n", "------", "---", "--", "-", "-");

		for (int i=0 ; i<numServers ; i++) {
			eth32server_t *s = &servers[i];

			strncpy_s(sName, sizeof(sName), s->serverName, sizeof(sName)-1);
			strncpy_s(sMap, sizeof(sMap), s->serverMapName, sizeof(sMap)-1);
			strncpy_s(sAddr, sizeof(sAddr), s->serverAddr, sizeof(sAddr)-1);

			if (s->rampage)
				rampages = true;

			Syscall.UI_Print("^%c%-32s   %-22s   %-21s   %-3i   %-3i %s\n", (altColor)?'n':'o', sName, sMap, sAddr, s->numUsers, s->numFriends, (s->rampage)?"^1*":"");

			altColor = !altColor;
		}
		Syscall.UI_Print("\n^2%i ^d%s found %s\n\n", numServers, (numServers > 1) ? "servers" : "server", (rampages) ? "^9(^1* ^9= ^1rampage^9)" : "");
	}

	Syscall.UI_Print("^d%-3s   %-16s   %-25s   %-10s   %-4s\n", "num", "username", "server", "mod", "et");
	Syscall.UI_Print("^2%-3s   %-16s   %-25s   %-10s   %-4s\n", "---", "--------", "------", "---", "--");

	if (numUsers) {
		char serverName[26];
		const char *serverModVer;
		const char *serverEtVer;
		bool sameEtVer;
		
		for (int i=0 ; i<numUsers ; i++) {
			if (users[i].anon) {
				Syscall.UI_Print("^%c%-3i   %-16s   %s\n", (altColor)?'n':'o', i+1, users[i].username, "  a    n    o    n    y    m    o    u    s");
																									   
			} else {
				strncpy_s(serverName, sizeof(serverName), users[i].serverName, 25);
				serverModVer = Tools.GetModVersionString(users[i].modVersion);
				serverEtVer = Tools.GetEtVersionString(users[i].etVersion);
				sameEtVer = (users[i].etVersion == eth32.et->ver || (users[i].etVersion >= ET_260 && eth32.et->ver >= ET_260));

				Syscall.UI_Print("^%c%-3i   %-16s   %-25s   %-10s   ^%c%-4s\n", (altColor)?'n':'o', i+1, users[i].username, serverName, serverModVer, (sameEtVer)?'2':'1', serverEtVer);
			}

			altColor = !altColor;
		}
		Syscall.UI_Print("\n^2%i ^d%s found\n\n", numUsers, (numUsers > 1) ? "users" : "user");
		Syscall.UI_Print("^dUse: ^o/eth_tracker join ^2[^9username^2|^9num^2]\n\n");
	}
	else {
		Syscall.UI_Print("^oNo users available\n\n");
		return;
	}
}

void CPortal::ConsoleJoin(void)
{
	if (Syscall.UI_Argc() == 2) {
		Syscall.UI_Print("\n^dSyntax:  ^o/eth_tracker ^2join ^9[^2name^9|^2num^9]\n\n");
	}
	else {
		const char *arg = Syscall.UI_Argv(2);

		int num = atoi(arg);

		// user specified a user number
		if (num > 0) {
			num--;

			if (num >= numUsers) {
				Syscall.UI_Print("\n^1ERROR: ^oSelected a user number that is out of range!\n\n");
				return;
			}

			if (users[num].anon) {
				Syscall.UI_Print("\n^1ERROR: ^oUser ^d%s ^ois anonymous, unable to join!\n\n");
				return;
			}
			
			if (eth32.cgameLoaded && !strcmp(users[num].server, eth32.server)) {
				Syscall.UI_Print("\n^1ERROR: ^oHey goofy, you are already on the same server as ^d%s^o!\n\n", users[num].username);
				return;
			}

			if ((eth32.et->ver != users[num].etVersion) && (eth32.et->ver < ET_260 || users[num].etVersion < ET_260)) {
				Syscall.UI_Print("\n^1ERROR: ^oVersion conflict with user ^d%s^o!\n\n", users[num].username);
				return;
			}

			char cmd[1024];
			sprintf_s(cmd, sizeof(cmd), "connect %s\n", users[num].server);
			Syscall.UI_ExecuteText(cmd);
			return;
		} else {
		// user specified a name
			for (int i=0 ; i<numUsers ; i++) 
			{				
				if (!_strnicmp(users[i].username, arg, strlen(arg))) {
					if (users[i].anon) {
						Syscall.UI_Print("\n^1ERROR: ^oUser ^d%s ^ois anonymous, unable to join!\n\n", users[i].username);
						return;
					}

					if (eth32.cgameLoaded && !strcmp(users[i].server, eth32.server)) {
						Syscall.UI_Print("\n^1ERROR: ^oHey goofy, you are already on the same server as ^d%s^o!\n\n", users[i].username);
						return;
					}	

					if ((eth32.et->ver != users[i].etVersion) && (eth32.et->ver < ET_260 || users[i].etVersion < ET_260)) {
						Syscall.UI_Print("\n^1ERROR: ^oVersion conflict with user ^d%s^o!\n\n", users[i].username);
						return;
					}

					char cmd[1024];
					sprintf_s(cmd, sizeof(cmd), "connect %s\n", users[i].server);
					Syscall.UI_ExecuteText(cmd);
					return;
				}
			}
			Syscall.UI_Print("\n^1ERROR: ^oUnable to find user \"^d%s^o\"!\n\n", arg);
		}
	}
}

/******************
*  Portal Thread
******************/

DWORD PortalThread(void)
{
	bool firstRun = true;

	Tools.RegisterThread(GetCurrentThreadId(), "Portal thread");
	while (1)
	{
		if (firstRun) {
			Sleep(INITIAL_WAIT); // avoid portal/host dos protection, auth called just prior to getting here
			Portal.RetrieveUsers();
			firstRun = false;
		}

		Sleep(UPDATE_INTERVAL);
		
		Portal.UpdateTracker();
		Portal.RetrieveUsers();
	}

	return 1;
}
