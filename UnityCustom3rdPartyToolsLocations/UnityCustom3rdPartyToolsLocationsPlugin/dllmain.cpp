// dllmain.cpp : Definiert den Einstiegspunkt für die DLL-Anwendung.
#include "pch.h"
#include <stdio.h>
#include <shellapi.h>
#include <locale>
#include <codecvt>
#include <string>
#include <fstream>

// Target pointer for the uninstrumented FindExecutable API.
//
static HINSTANCE(WINAPI* TrueFindExecutableW)(_In_ LPCWSTR lpFile, _In_opt_ LPCWSTR lpDirectory, _Out_writes_(MAX_PATH) LPWSTR lpResult) = FindExecutableW;

static bool hasCustomBlender = false;
static std::wstring customBlenderExecutablePath;

// Detour function that replaces the FindExecutable API.
//
HINSTANCE WINAPI HookedFindExecutableW(_In_ LPCWSTR lpFile, _In_opt_ LPCWSTR lpDirectory, _Out_writes_(MAX_PATH) LPWSTR lpResult)
{
	if (hasCustomBlender)
	{
		LPCWSTR extension = wcsrchr(lpFile, L'.');

		// Check if it's a blendfile, and apparently unity may add quotes around the path "just to be sure" I guess
		if (wcscmp(extension, L".blend") == 0 || wcscmp(extension, L".blend\"") == 0)
		{
			wcscpy_s(lpResult, MAX_PATH, customBlenderExecutablePath.c_str());

			// Something > 32
			return (HINSTANCE)33;
		}
	}

	HINSTANCE result = TrueFindExecutableW(lpFile, lpDirectory, lpResult);

	return result;
}

void LoadConfig()
{
	std::ifstream config;
	config.open("blender-config.txt");

	if (config.is_open())
	{
		std::string customBlenderExecutablePathUtf8;

		std::getline(config, customBlenderExecutablePathUtf8);

		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

		customBlenderExecutablePath = converter.from_bytes(customBlenderExecutablePathUtf8);

		config.close();

		hasCustomBlender = true;
	}
	else
	{
		hasCustomBlender = false;
	}
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
	)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		LoadConfig();

		DetourRestoreAfterWith();

		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID&)TrueFindExecutableW, HookedFindExecutableW);
		DetourTransactionCommit();
	}break;
	case DLL_THREAD_ATTACH:break;
	case DLL_THREAD_DETACH:break;
	case DLL_PROCESS_DETACH:
	{
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourDetach(&(PVOID&)TrueFindExecutableW, HookedFindExecutableW);
		DetourTransactionCommit();
	}
	break;
	}

	return TRUE;
}

