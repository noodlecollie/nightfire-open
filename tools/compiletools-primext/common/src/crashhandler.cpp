/*
crashhandler.cpp - advanced crashhandler
Copyright (C) 2016 Mittorn
Copyright (C) 2022 SNMetamorph

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#include <stdio.h>
#include <stdlib.h>  // rand, adbs
#include <stdarg.h>  // va

#include "CompileTools/crashhandler.h"
#include "CompileTools/conprint.h"
#include "BuildPlatform/PlatformID.h"
#include "BuildPlatform/Arch.h"
#include "PlatformLib/Time.h"
#include "CRTLib/crtlib.h"

#if !XASH_WIN32()
#include <stddef.h>  // size_t
#else
#include <sys/types.h>  // off_t
#endif

#if XASH_WIN32()
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <winnt.h>
#include <dbghelp.h>
#include <psapi.h>
#include <ctime>
#include <string>

typedef ULONG_PTR DWORD_PTR, *PDWORD_PTR;

static bool g_writeMinidump = true;
static LPTOP_LEVEL_EXCEPTION_FILTER g_oldFilter;

static int Sys_ModuleName(HANDLE process, char* name, void* address, int len)
{
	static HMODULE* moduleArray;
	static unsigned int moduleCount;
	LPBYTE moduleArrayBytes;
	DWORD bytesRequired;

	if ( len < 3 )
	{
		return 0;
	}

	if ( !moduleArray && EnumProcessModules(process, NULL, 0, &bytesRequired) )
	{
		if ( bytesRequired )
		{
			moduleArrayBytes = (LPBYTE)LocalAlloc(LPTR, bytesRequired);

			if ( moduleArrayBytes &&
				 EnumProcessModules(process, (HMODULE*)moduleArrayBytes, bytesRequired, &bytesRequired) )
			{
				moduleCount = bytesRequired / sizeof(HMODULE);
				moduleArray = (HMODULE*)moduleArrayBytes;
			}
		}
	}

	for ( size_t i = 0; i < moduleCount; i++ )
	{
		MODULEINFO info;
		GetModuleInformation(process, moduleArray[i], &info, sizeof(MODULEINFO));

		if ( (address > info.lpBaseOfDll) &&
			 ((DWORD64)address < (DWORD64)info.lpBaseOfDll + (DWORD64)info.SizeOfImage) )
			return GetModuleBaseName(process, moduleArray[i], name, len);
	}

	return Q_snprintf(name, len, "???");
}

static void Sys_StackTrace(PEXCEPTION_POINTERS pInfo)
{
	static char message[4096];
	int len = 0;
	HANDLE process = GetCurrentProcess();
	HANDLE thread = GetCurrentThread();
	IMAGEHLP_LINE64 line;
	DWORD dline = 0;
	DWORD options;
	CONTEXT context;
	STACKFRAME64 stackframe;
	DWORD image;

	memcpy(&context, pInfo->ContextRecord, sizeof(CONTEXT));

	options = SymGetOptions();
	options |= SYMOPT_DEBUG;
	options |= SYMOPT_LOAD_LINES;
	SymSetOptions(options);

	SymInitialize(process, NULL, TRUE);

	ZeroMemory(&stackframe, sizeof(STACKFRAME64));

#ifdef _M_IX86
	image = IMAGE_FILE_MACHINE_I386;
	stackframe.AddrPC.Offset = context.Eip;
	stackframe.AddrPC.Mode = AddrModeFlat;
	stackframe.AddrFrame.Offset = context.Ebp;
	stackframe.AddrFrame.Mode = AddrModeFlat;
	stackframe.AddrStack.Offset = context.Esp;
	stackframe.AddrStack.Mode = AddrModeFlat;
#elif _M_X64
	image = IMAGE_FILE_MACHINE_AMD64;
	stackframe.AddrPC.Offset = context.Rip;
	stackframe.AddrPC.Mode = AddrModeFlat;
	stackframe.AddrFrame.Offset = context.Rsp;
	stackframe.AddrFrame.Mode = AddrModeFlat;
	stackframe.AddrStack.Offset = context.Rsp;
	stackframe.AddrStack.Mode = AddrModeFlat;
#elif _M_IA64
	image = IMAGE_FILE_MACHINE_IA64;
	stackframe.AddrPC.Offset = context.StIIP;
	stackframe.AddrPC.Mode = AddrModeFlat;
	stackframe.AddrFrame.Offset = context.IntSp;
	stackframe.AddrFrame.Mode = AddrModeFlat;
	stackframe.AddrBStore.Offset = context.RsBSP;
	stackframe.AddrBStore.Mode = AddrModeFlat;
	stackframe.AddrStack.Offset = context.IntSp;
	stackframe.AddrStack.Mode = AddrModeFlat;
#elif
#error
#endif
	len += Q_snprintf(
		message + len,
		sizeof(message) - len,
		"Sys_Crash: address %p, code %p\n",
		pInfo->ExceptionRecord->ExceptionAddress,
		pInfo->ExceptionRecord->ExceptionCode);

	if ( SymGetLineFromAddr64(process, (DWORD64)pInfo->ExceptionRecord->ExceptionAddress, &dline, &line) )
	{
		len += Q_snprintf(
			message + len,
			sizeof(message) - len,
			"Exception: %s:%d:%d\n",
			(char*)line.FileName,
			(int)line.LineNumber,
			(int)dline);
	}

	if ( SymGetLineFromAddr64(process, stackframe.AddrPC.Offset, &dline, &line) )
	{
		len += Q_snprintf(
			message + len,
			sizeof(message) - len,
			"PC: %s:%d:%d\n",
			(char*)line.FileName,
			(int)line.LineNumber,
			(int)dline);
	}

	if ( SymGetLineFromAddr64(process, stackframe.AddrFrame.Offset, &dline, &line) )
	{
		len += Q_snprintf(
			message + len,
			sizeof(message) - len,
			"Frame: %s:%d:%d\n",
			(char*)line.FileName,
			(int)line.LineNumber,
			(int)dline);
	}

	for ( size_t i = 0; i < 25; i++ )
	{
		char buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)];
		PSYMBOL_INFO symbol = (PSYMBOL_INFO)buffer;
		BOOL result = StackWalk64(
			image,
			process,
			thread,
			&stackframe,
			&context,
			NULL,
			SymFunctionTableAccess64,
			SymGetModuleBase64,
			NULL);
		DWORD64 displacement = 0;

		if ( !result )
			break;

		symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
		symbol->MaxNameLen = MAX_SYM_NAME;

		len += Q_snprintf(message + len, sizeof(message) - len, "% 2d %p", i, (void*)stackframe.AddrPC.Offset);

		if ( SymFromAddr(process, stackframe.AddrPC.Offset, &displacement, symbol) )
		{
			len += Q_snprintf(message + len, sizeof(message) - len, " %s ", symbol->Name);
		}

		if ( SymGetLineFromAddr64(process, stackframe.AddrPC.Offset, &dline, &line) )
		{
			len += Q_snprintf(
				message + len,
				sizeof(message) - len,
				"(%s:%d:%d) ",
				(char*)line.FileName,
				(int)line.LineNumber,
				(int)dline);
		}

		len += Q_snprintf(message + len, sizeof(message) - len, "(");
		len += Sys_ModuleName(process, message + len, (void*)stackframe.AddrPC.Offset, sizeof(message) - len);
		len += Q_snprintf(message + len, sizeof(message) - len, ")\n");
	}

	Sys_Print("\n");
	Sys_Print(message);
	SymCleanup(process);
}

static void Sys_GetProcessFileName(std::string& fileName)
{
	fileName.resize(MAX_PATH);
	GetModuleBaseName(GetCurrentProcess(), NULL, &fileName[0], (DWORD)(fileName.capacity() - 1));
	fileName.assign(fileName.c_str());
	fileName.shrink_to_fit();
	fileName.erase(fileName.find_last_of('.'));
}

static void Sys_GetMinidumpFileName(const std::string& processName, std::string& fileName)
{
	std::time_t currentUtcTime = std::time(nullptr);
	const std::tm* currentLocalTime = PlatformLib_LocalTime(&currentUtcTime);
	size_t bufferSize = 0;
	char* stringBuffer = nullptr;

	for ( size_t i = 0; i < 2; ++i )
	{
		bufferSize = std::snprintf(
			stringBuffer,
			bufferSize,
			"%s_%s_crash_%d%.2d%.2d_%.2d%.2d%.2d.mdmp",
			processName.c_str(),  // current process name
			BuildPlatform_CommitString(),  // last commit hash from VCS
			currentLocalTime->tm_year + 1900,  // year
			currentLocalTime->tm_mon + 1,  // month
			currentLocalTime->tm_mday,  // day of month
			currentLocalTime->tm_hour,  // hour
			currentLocalTime->tm_min,  // minutes
			currentLocalTime->tm_sec  // seconds
		);
		fileName.resize(bufferSize + 1);
		stringBuffer = &fileName[0];
		bufferSize += 1;
	}
	fileName.assign(fileName.c_str());
}

static bool Sys_WriteMinidump(PEXCEPTION_POINTERS exceptionInfo, MINIDUMP_TYPE minidumpType)
{
	HRESULT errorCode;
	std::string processName;
	std::string minidumpFileName;

	// get process name & minidump file name
	Sys_GetProcessFileName(processName);
	Sys_GetMinidumpFileName(processName, minidumpFileName);

	// create minidump file
	SetLastError(NOERROR);
	HANDLE minidumpFile = CreateFile(
		minidumpFileName.c_str(),
		GENERIC_WRITE,
		FILE_SHARE_WRITE,
		nullptr,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		nullptr);

	errorCode = HRESULT_FROM_WIN32(GetLastError());
	if ( !SUCCEEDED(errorCode) )
	{
		CloseHandle(minidumpFile);
		return false;
	}

	// write all the data to minidump file
	MINIDUMP_EXCEPTION_INFORMATION mdmpExceptionInfo;
	mdmpExceptionInfo.ThreadId = GetCurrentThreadId();
	mdmpExceptionInfo.ExceptionPointers = exceptionInfo;
	mdmpExceptionInfo.ClientPointers = FALSE;
	bool minidumpWritten = MiniDumpWriteDump(
		GetCurrentProcess(),
		GetCurrentProcessId(),
		minidumpFile,
		minidumpType,
		&mdmpExceptionInfo,
		nullptr,
		nullptr);

	CloseHandle(minidumpFile);
	return minidumpWritten;
}

static LONG _stdcall Sys_Crash(PEXCEPTION_POINTERS pInfo)
{
	Sys_StackTrace(pInfo);

	if ( g_writeMinidump )
	{
		// first try to write as much as possible information, otherwise
		// create minidump with minimal information set
		auto minidumpType = static_cast<MINIDUMP_TYPE>(
			MiniDumpWithDataSegs | MiniDumpWithCodeSegs | MiniDumpWithHandleData | MiniDumpWithFullMemory |
			MiniDumpWithFullMemoryInfo | MiniDumpWithIndirectlyReferencedMemory | MiniDumpWithThreadInfo |
			MiniDumpWithModuleHeaders);

		if ( !Sys_WriteMinidump(pInfo, minidumpType) )
		{
			Sys_WriteMinidump(pInfo, MiniDumpWithDataSegs);
		}
	}

	if ( GetDeveloperLevel() <= D_WARN )
	{
		// no reason to call debugger in release build - just exit
		exit(1);
	}

	if ( g_oldFilter )
	{
		return g_oldFilter(pInfo);
	}

	return EXCEPTION_CONTINUE_EXECUTION;
}

void CrashHandler::Setup()
{
	SetErrorMode(SEM_FAILCRITICALERRORS);  // no abort/retry/fail errors
	g_oldFilter = SetUnhandledExceptionFilter(Sys_Crash);
}

void CrashHandler::Restore()
{
	if ( g_oldFilter )
	{
		SetUnhandledExceptionFilter(g_oldFilter);
	}
}

#elif XASH_LINUX()
#include <dlfcn.h>
#include <ucontext.h>
#include <signal.h>
#include <sys/mman.h>

#define STACK_BACKTRACE_STR "Stack backtrace:\n"
#define STACK_DUMP_STR "Stack dump:\n"

#define STACK_BACKTRACE_STR_LEN (sizeof(STACK_BACKTRACE_STR) - 1)
#define STACK_DUMP_STR_LEN (sizeof(STACK_DUMP_STR) - 1)
#define ALIGN(x, y) (((uintptr_t)(x) + ((y)-1)) & ~((y)-1))

static struct sigaction g_oldFilter;

static int Sys_PrintFrame(char* buf, int len, int i, void* addr)
{
	Dl_info dlinfo;
	if ( len <= 0 )
		return 0;  // overflow

	if ( dladdr(addr, &dlinfo) )
	{
		if ( dlinfo.dli_sname )
			return Q_snprintf(
				buf,
				len,
				"%2d: %p <%s+%lu> (%s)\n",
				i,
				addr,
				dlinfo.dli_sname,
				(uintptr_t)addr - (uintptr_t)dlinfo.dli_saddr,
				dlinfo.dli_fname);  // print symbol, module and address
		else
			return Q_snprintf(buf, len, "%2d: %p (%s)\n", i, addr, dlinfo.dli_fname);  // print module and address
	}
	else
		return Q_snprintf(buf, len, "%2d: %p\n", i, addr);  // print only address
}

static void Sys_Crash(int signal, siginfo_t* si, void* context)
{
	void *pc = NULL, **bp = NULL, **sp = NULL;  // this must be set for every OS!
	char message[8192];
	int len, i = 0;

	ucontext_t* ucontext = (ucontext_t*)context;

#if XASH_AMD64()
	pc = (void*)ucontext->uc_mcontext.gregs[REG_RIP];
	bp = (void**)ucontext->uc_mcontext.gregs[REG_RBP];
	sp = (void**)ucontext->uc_mcontext.gregs[REG_RSP];
#elif XASH_X86()
	pc = (void*)ucontext->uc_mcontext.gregs[REG_EIP];
	bp = (void**)ucontext->uc_mcontext.gregs[REG_EBP];
	sp = (void**)ucontext->uc_mcontext.gregs[REG_ESP];
#endif

	// safe actions first, stack and memory may be corrupted
	len = Q_snprintf(
		message,
		sizeof(message),
		"PrimeXT (%s, commit %s, architecture %s, platform %s)\n",
		__DATE__,
		BuildPlatform_CommitString(),
		BuildPlatform_ArchitectureString(),
		BuildPlatform_PlatformString());

	len += Q_snprintf(
		message + len,
		sizeof(message) - len,
		"Crash: signal %d, errno %d with code %d at %p %p\n",
		signal,
		si->si_errno,
		si->si_code,
		si->si_addr,
		si->si_ptr);

	write(STDERR_FILENO, message, len);

	// flush buffers before writing directly to descriptors
	fflush(stdout);
	fflush(stderr);

	if ( pc && bp && sp )
	{
		size_t pagesize = sysconf(_SC_PAGESIZE);
		// try to print backtrace
		write(STDERR_FILENO, STACK_BACKTRACE_STR, STACK_BACKTRACE_STR_LEN);
		Q_strncpy(message + len, STACK_BACKTRACE_STR, sizeof(message) - len);
		len += STACK_BACKTRACE_STR_LEN;

		// false on success, true on failure
#define try_allow_read(pointer, pagesize) \
	((mprotect((char*)ALIGN((pointer), (pagesize)), (pagesize), PROT_READ | PROT_WRITE | PROT_EXEC) == -1) && \
	 (mprotect((char*)ALIGN((pointer), (pagesize)), (pagesize), PROT_READ | PROT_EXEC) == -1) && \
	 (mprotect((char*)ALIGN((pointer), (pagesize)), (pagesize), PROT_READ | PROT_WRITE) == -1) && \
	 (mprotect((char*)ALIGN((pointer), (pagesize)), (pagesize), PROT_READ) == -1))

		do
		{
			int line = Sys_PrintFrame(message + len, sizeof(message) - len, ++i, pc);
			write(STDERR_FILENO, message + len, line);
			len += line;
			// if( !dladdr(bp,0) ) break; // only when bp is in module
			if ( try_allow_read(bp, pagesize) )
				break;
			if ( try_allow_read(bp[0], pagesize) )
				break;
			pc = bp[1];
			bp = (void**)bp[0];
		}
		while ( bp && i < 128 );

		// try to print stack
		write(STDERR_FILENO, STACK_DUMP_STR, STACK_DUMP_STR_LEN);
		Q_strncpy(message + len, STACK_DUMP_STR, sizeof(message) - len);
		len += STACK_DUMP_STR_LEN;

		if ( !try_allow_read(sp, pagesize) )
		{
			for ( i = 0; i < 32; i++ )
			{
				int line = Sys_PrintFrame(message + len, sizeof(message) - len, i, sp[i]);
				write(STDERR_FILENO, message + len, line);
				len += line;
			}
		}

#undef try_allow_read
	}

	Sys_Print("\n");
	Sys_Print(message);
	exit(1);
}

void CrashHandler::Setup()
{
	struct sigaction act = {};
	act.sa_sigaction = Sys_Crash;
	act.sa_flags = SA_SIGINFO | SA_ONSTACK;
	sigaction(SIGSEGV, &act, &g_oldFilter);
	sigaction(SIGABRT, &act, &g_oldFilter);
	sigaction(SIGBUS, &act, &g_oldFilter);
	sigaction(SIGILL, &act, &g_oldFilter);
}

void CrashHandler::Restore()
{
	sigaction(SIGSEGV, &g_oldFilter, NULL);
	sigaction(SIGABRT, &g_oldFilter, NULL);
	sigaction(SIGBUS, &g_oldFilter, NULL);
	sigaction(SIGILL, &g_oldFilter, NULL);
}

#else
#warning "Crash handler not implemented for this platform."
void CrashHandler::Setup()
{
	// stub
}

void CrashHandler::Restore()
{
	// stub
}
#endif

/*
================
CrashHandler::WaitForDebugger

Useful for debugging things that shutdowns too fast
================
*/
void CrashHandler::WaitForDebugger()
{
#if XASH_WIN32()
	Sys_Print("Waiting for debugger...\n");
	while ( !IsDebuggerPresent() )
	{
		Sleep(250);
	}
#else
	// TODO implement
#endif
}

/*
================
CrashHandler::ToggleMinidumpWrite

Enables/disables writing minidumps when application crashes.
This feature is Windows-specific.
================
*/
void CrashHandler::ToggleMinidumpWrite(bool status)
{
#if XASH_WIN32()
	g_writeMinidump = status;
#else
	(void)status;
#endif
}
