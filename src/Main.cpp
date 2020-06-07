// ETH32 - an Enemy Territory cheat for windows
// Copyright (c) 2007 eth32 team
// www.cheatersutopia.com & www.nixcoders.org

#include "eth32.h"
#include <tlhelp32.h>
#include <dbghelp.h>




eth32_t eth32;

LONG __stdcall Eth32ExceptionFilter( EXCEPTION_POINTERS* pep ) 
{
	struct _CONTEXT *ContextRecord = pep->ContextRecord;
	struct _EXCEPTION_RECORD *ExceptionRecord = pep->ExceptionRecord;
	char path[MAX_STRING_CHARS];
	DWORD thisThread = GetCurrentThreadId();
	FILE *cf;

	sprintf_s(path, sizeof(path), "%s\\crashreport.log", eth32.path);
	if (fopen_s(&cf, path, "w")){
		MessageBox(NULL, "Failed to create crashreport", "Eth32: Fatal Error", MB_OK);
		goto exit;
	}

	fprintf(cf, "Exception %x  flags %x address %p\n\n", ExceptionRecord->ExceptionCode, ExceptionRecord->ExceptionFlags, ExceptionRecord->ExceptionAddress);
	fprintf(cf, "ETH32 version %s\n", ETH32_VERSION);
	fprintf(cf, "ETH32 crc 0x%X\n", eth32.eth32CRC);
	fprintf(cf, "ET version %s\n\n", eth32.et->version);
	fprintf(cf, "cgame mod: %s%s (crc = 0x%X)\n", eth32.cgMod ? eth32.cgMod->name : "none", eth32.cgMod ? eth32.cgMod->version : "", eth32.cgMod ? eth32.cgMod->crc32 : 0);
	fprintf(cf, "automod: %s\ncgame hooked: %s\n", eth32.cg.automod ? "yes" : "no", eth32.cgameLoaded ? "yes" : "no");
	fprintf(cf, "server: %s (%s)\n\n", eth32.server[0] ? eth32.server : "not connected", eth32.cg.serverMapName[0] ? eth32.cg.serverMapName : "");

	DWORD ETProcID = GetCurrentProcessId();

/* list all modules loaded by ET */
	  
	HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
	MODULEENTRY32 me32;

	struct module_t {
		char szName[256];
		DWORD baseAddr;
		DWORD modSize;
	} *modules;

	int nModules = 0;
	
	hModuleSnap = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, ETProcID );
	if( hModuleSnap == INVALID_HANDLE_VALUE ){
		fprintf(cf, "CreateToolhelp32Snapshot TH32CS_SNAPMODULE failed :(\n");
	} else {
		me32.dwSize = sizeof( MODULEENTRY32 );

		if (!Module32First( hModuleSnap, &me32 )){
			fprintf(cf, "Module32First error\n");
			CloseHandle( hModuleSnap );
			return( FALSE );
		} else {
			fprintf(cf, "---------------------------\n---- eth32 module list ----\n---------------------------\n");
			do {
				if (nModules == 0){
					modules = (struct module_t *) malloc(sizeof(struct module_t));
					nModules++;
				} else {
					nModules++;
					modules = (struct module_t *) realloc(modules, sizeof(struct module_t)*nModules);
				}

				strncpy_s(modules[nModules-1].szName, 255, me32.szModule, 255);
				modules[nModules-1].baseAddr = (DWORD) me32.modBaseAddr;
				modules[nModules-1].modSize = me32.modBaseSize;

				fprintf(cf, "%s\n", me32.szModule);
				fprintf(cf, "\texecutable     = %s\n", me32.szExePath );
				fprintf(cf, "\tref count (g)  = 0x%04X\n", me32.GlblcntUsage );
				fprintf(cf, "\tref count (p)  = 0x%04X\n", me32.ProccntUsage );
				fprintf(cf, "\timage base     = 0x%08X\n", (DWORD) me32.modBaseAddr );
				fprintf(cf, "\timage size     = %d\n", me32.modBaseSize );
			} while (Module32Next( hModuleSnap, &me32 ));
		}
	}

/* list all threads loaded by ET */
	HANDLE hThreadSnap = INVALID_HANDLE_VALUE; 
	THREADENTRY32 te32; 
	hThreadSnap = CreateToolhelp32Snapshot( TH32CS_SNAPTHREAD, 0 ); 
	te32.dwSize = sizeof(THREADENTRY32 ); 
	if( hThreadSnap == INVALID_HANDLE_VALUE ){
		fprintf(cf, "failed to query thread information");
	} else {
		if( !Thread32First( hThreadSnap, &te32 ) ){
			fprintf(cf, "Thread32First error");
			CloseHandle( hThreadSnap );	    
		} else {
			HANDLE hThread;
			fprintf(cf, "\n\n---------------------------\n---- eth32 thread list ----\n---------------------------\n");
			do {
				if (te32.th32OwnerProcessID == ETProcID) {
					int q;
					for (q=0; q<Tools.sThreads; q++){
						if (Tools.nThreads[q].tID == te32.th32ThreadID){
							fprintf(cf, "Thread ID = 0x%08X    (%s)\n",te32.th32ThreadID, Tools.nThreads[q].name);
							break;
						}
					}
					if (q==Tools.sThreads)					
						fprintf(cf, "Thread ID = 0x%08X\n", te32.th32ThreadID ); 

					fprintf(cf, "\tbase priority  = %d\n", te32.tpBasePri ); 
					fprintf(cf, "\tdelta priority = %d\n", te32.tpDeltaPri ); 

					if (thisThread != te32.th32ThreadID){
						if (hThread = OpenThread(THREAD_ALL_ACCESS,false, te32.th32ThreadID)){
							if (SuspendThread(hThread)<0)
								fprintf(cf, "\tsuspend failed on <1>\n");
						} else					
							fprintf(cf, "\tsuspend failed on <2>\n");
					}
				}
			} while( Thread32Next(hThreadSnap, &te32) ); 
		}
	}

	int i;
	
	#define WRITE_REGISTER(x,c) \
	for (i=0; i<nModules; i++){ \
		if (c >= modules[i].baseAddr && (c <= (modules[i].baseAddr+modules[i].modSize))){ \
			fprintf(cf, "\t%s    = 0x%08X   (%s+0x%x)\n", x, c, modules[i].szName, c-modules[i].baseAddr); \
			break; \
		} \
	} \
	if (i==nModules) \
		fprintf(cf, "\t%s    = %08i     0x%x\n", x, c, c); 

	fprintf(cf, "\n\n--------------------------------\n---- exception context info ----\n--------------------------------\n");
	fprintf(cf, "Register states\n");

	DWORD fl=ContextRecord->EFlags;
	fprintf(cf, "\tflags: %s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s\n",
		(fl&(1 << 21)) ? "ID " : "",
		(fl&(1 << 20)) ? "VIP " : "",
		(fl&(1 << 19)) ? "VIF " : "",
		(fl&(1 << 18)) ? "AC " : "",
		(fl&(1 << 17)) ? "VM " : "",
		(fl&(1 << 16)) ? "RF " : "",
		(fl&(1 << 14)) ? "NT " : "",
		(fl&(1 << 12) | fl&(1 << 13)) ? "IOPL " : "",
		(fl&(1 << 11)) ? "OF " : "",
		(fl&(1 << 10)) ? "DF " : "",
		(fl&(1 << 9)) ? "IF " : "",
		(fl&(1 << 8)) ? "TF " : "",
		(fl&(1 << 7)) ? "SF " : "",
		(fl&(1 << 6)) ? "ZF " : "",
		(fl&(1 << 4)) ? "AF " : "",
		(fl&(1 << 2)) ? "PF " : "",
		(fl&(1 << 0)) ? "CF " : ""
		);

	WRITE_REGISTER("eax", ContextRecord->Eax);
	WRITE_REGISTER("ebx", ContextRecord->Ebx);
	WRITE_REGISTER("ecx", ContextRecord->Ecx);
	WRITE_REGISTER("edx", ContextRecord->Edx);
	WRITE_REGISTER("esi", ContextRecord->Esi);
	WRITE_REGISTER("edi", ContextRecord->Edi);
	WRITE_REGISTER("ebp", ContextRecord->Ebp);
	WRITE_REGISTER("esp", ContextRecord->Esp);
	WRITE_REGISTER("eip", ContextRecord->Eip);

	fprintf(cf, "\n");

	WRITE_REGISTER("debug 0", ContextRecord->Dr0);
	WRITE_REGISTER("debug 1", ContextRecord->Dr1);
	WRITE_REGISTER("debug 2", ContextRecord->Dr2);
	WRITE_REGISTER("debug 3", ContextRecord->Dr3);
	WRITE_REGISTER("debug 6", ContextRecord->Dr6);
	WRITE_REGISTER("debug 7", ContextRecord->Dr7);

	fprintf(cf, "\n");

	fprintf(cf, "\t%s    = 0x%08x\n", "Cs", ContextRecord->SegCs);
	fprintf(cf, "\t%s    = 0x%08x\n", "Ds", ContextRecord->SegDs);
	fprintf(cf, "\t%s    = 0x%08x\n", "Es", ContextRecord->SegEs);
	fprintf(cf, "\t%s    = 0x%08x\n", "Fs", ContextRecord->SegFs);
	fprintf(cf, "\t%s    = 0x%08x\n", "Gs", ContextRecord->SegGs);
	fprintf(cf, "\t%s    = 0x%08x\n", "Ss", ContextRecord->SegSs);

	/* generate a call stack */
	fprintf(cf, "\n\n--------------------------------\n---- callstack ----\n--------------------------------\n");
	CONTEXT context, ctxTemp;
	memset(&context, 0, sizeof(CONTEXT));
	context.ContextFlags = CONTEXT_FULL;
	HANDLE hThread = GetCurrentThread();
	HANDLE hProcess = GetCurrentProcess();

	if (GetThreadContext(hThread, &context)){
		STACKFRAME callStack;
		memset(&callStack, 0, sizeof(STACKFRAME));
		callStack.AddrPC.Offset    = context.Eip;
		callStack.AddrStack.Offset = context.Esp;
		callStack.AddrFrame.Offset = context.Ebp;
		callStack.AddrPC.Mode      = AddrModeFlat;
		callStack.AddrStack.Mode   = AddrModeFlat;
		callStack.AddrFrame.Mode   = AddrModeFlat;
		
		STACKFRAME ccc;
		
		for(i=0; ;i++ ){
			if (!StackWalk(IMAGE_FILE_MACHINE_I386, hProcess, hThread, &callStack, &ctxTemp, NULL, SymFunctionTableAccess, SymGetModuleBase, NULL))
				break;

			fprintf(cf, "%i: 0x%x 0x%x 0x%x 0x%x\n", i, 
				callStack.AddrPC.Offset,
				callStack.AddrReturn.Offset,
				callStack.AddrFrame.Offset,
				callStack.AddrStack.Offset);
		}
	}

exit:
	if (cf)
		fclose(cf);

	/* create a mini dump */
	sprintf_s(path, sizeof(path), "%s\\eth32crash.dmp", eth32.path);
	HANDLE hFile = CreateFile(path, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL ); 
	if ((hFile != NULL) && (hFile != INVALID_HANDLE_VALUE)){
		MINIDUMP_EXCEPTION_INFORMATION mdei; 
		mdei.ThreadId = thisThread; 
		mdei.ExceptionPointers = pep; 
		mdei.ClientPointers = false; 
		MINIDUMP_TYPE mdt = MiniDumpNormal; 
		bool rv = MiniDumpWriteDump( GetCurrentProcess(), GetCurrentProcessId(), hFile, mdt, (pep != 0) ? &mdei : 0, 0, 0 ); 
		CloseHandle(hFile); 
	}

	char buf[512];
	char loc[128];
	
	loc[0] = '\0';
	for (i=0; i<nModules; i++){
		if (ContextRecord->Eip >= modules[i].baseAddr && (ContextRecord->Eip <= (modules[i].baseAddr+modules[i].modSize))){			
			sprintf_s(loc, 127, "(%s+0x%x)", modules[i].szName, ContextRecord->Eip-modules[i].baseAddr);
			break;
		}
	}
	if (i==nModules)
		sprintf_s(loc, 127, "0x%X (???)", ContextRecord->Eip);

	sprintf_s(buf, sizeof(buf), "Eth32 received exception %X at %s\nA crash report has been saved to crashreport.log", ExceptionRecord->ExceptionCode, loc);

	MessageBox(NULL, buf, "Eth32: Fatal Error :(", MB_OK);
	Tools.PatchMicrosoftCRTandDOTNETInstallGlobalExceptionFilterOverwritingRoutinesSoTheyDontWorkAgainUntilWeTellThemTo(false);

	return EXCEPTION_EXECUTE_HANDLER; 

}

bool __stdcall DllMain(HINSTANCE hInst, DWORD dwReason, LPVOID pvReserved)
{
	TRASH2
	int c = 0;
	for (; c < 45; c++) {
		TRASH4;
	}
	TRASH3
	TRASH15
	
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		INSTALL_EXCEPTION_HANDLER

		TRASH1

		Tools.Init(hInst);
#ifdef ETH32_DEBUG
		Debug.Log("Main Thread ID %x", GetCurrentThreadId());
#endif
		TRASH8
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRASH10
		TRASH11
		TRASH16
		Tools.Shutdown();
		TRASH19
		TRASH20
	}

	TRASH14
	return true;
	TRASH27
	TRASH0
}


