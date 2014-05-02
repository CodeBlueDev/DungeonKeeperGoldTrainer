#pragma once
#include <Windows.h>

DWORD* GoldBaseAddress = (DWORD*)0x0074B6B0;
DWORD GoldDisplayOffset = 0x499082;
DWORD GoldValueOffset = 0x499086;
WORD GoldAdded = 25000;
bool isAttached = true;
bool isRunning = true;;

DWORD __stdcall GoldIncrease()
{
	DWORD* goldDisplayAddress = (DWORD*)(*GoldBaseAddress+GoldDisplayOffset);	
	DWORD* goldValueAddress = (DWORD*)(*GoldBaseAddress+GoldValueOffset);	
	bool keyPressed;
	while(isAttached)
	{
		DWORD oldProtection;
		while(GetAsyncKeyState(VK_NUMPAD0) && !keyPressed)
		{
			keyPressed = true;
			VirtualProtect(goldDisplayAddress, 8, PAGE_EXECUTE_READWRITE, &oldProtection);
			*goldDisplayAddress += GoldAdded;
			*goldValueAddress += GoldAdded;
			VirtualProtect(goldDisplayAddress, 8, oldProtection, &oldProtection);
		}
		keyPressed = false;
		Sleep(100);
	}
	isRunning = false;
	return 1;
}

bool WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpvReserved)
{
	switch(dwReason)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)GoldIncrease, NULL, NULL, NULL);
		break;
	case DLL_PROCESS_DETACH:
		isAttached = false;
		while(isRunning)
		{
			Sleep(100);
		}
		break;
	}
	return true;
}