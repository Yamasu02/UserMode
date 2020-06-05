#pragma once
#include <windows.h>
#include <iostream>
#include <vector>
#include <TlHelp32.h>

DWORD GetProcID(const wchar_t* procname)
{
	DWORD procid = 0;
	HANDLE hproc = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (hproc != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 procentry;
		procentry.dwSize = sizeof(procentry);
		if (Process32First(hproc, &procentry))
			do
			{
				if (!wcscmp(procentry.szExeFile, procname))
				{
					procid = procentry.th32ProcessID;
					break;
				}
			} while (Process32Next(hproc, &procentry));
	}
	CloseHandle(hproc);
	return procid;
}

uintptr_t  GetModuleBA(DWORD procid, const wchar_t* modname)
{
	uintptr_t modbaseaddress = NULL;
	HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procid);
	if (hsnap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 modentry;
		modentry.dwSize = sizeof(modentry);
		do
		{
			if (!wcscmp(modentry.szModule, modname))
			{
				modbaseaddress = (uintptr_t)modentry.modBaseAddr;
			}
		} while (Module32Next(hsnap, &modentry));
	}
	return modbaseaddress;
}

template<class T>
T Read(HANDLE hProcess, DWORD address) 
{
	T buffer;
	ReadProcessMemory(hProcess, (LPCVOID)address, &buffer, sizeof(T), NULL);
	return buffer;
}



template<class T>
void Write(HANDLE hProcess, DWORD address,T var)
{
	WriteProcessMemory(hProcess, (LPVOID)address, &var, sizeof(T), NULL);
}



template <class T>
T ReadTwice(HANDLE hProc, DWORD addr_with_offset_added, int offset2)
{
	T var;
	ReadProcessMemory(hProc, (BYTE*)addr_with_offset_added, &addr_with_offset_added, sizeof(DWORD), 0);
	addr_with_offset_added += offset2;
	ReadProcessMemory(hProc, (BYTE*)addr_with_offset_added, &var, sizeof(var), 0);
	return var;
}

