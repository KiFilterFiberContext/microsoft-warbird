#pragma once

#include <Windows.h>

/*
decrypted module info
WCHAR ModuleName[NAME_SIZE];
DWORD SizeOfImports;
CHAR FunctionNames[1];
*/

namespace WARBIRD
{
	// size= 24 bytes
#pragma pack(8)
	typedef struct _MODULE_INFO
	{
		HMODULE ModuleBase;
		DWORD SizeOfImage;
		IMAGE_DATA_DIRECTORY ExportDirectory;
	} MODULE_INFO, * PMODULE_INFO;

	static PMODULE_INFO g_arModuleInfo; // list of modules
	static void** g_FuncAddress; // function table of WARBIRD_DELAY_LOAD ptrs

	static int g_WarbirdSecureFunctionsRefCount;
	static int g_WarbirdSecureFunctionsLock;
};

struct WARBIRD_DELAY_LOAD
{
	BOOL BitBlt(HDC hdc, int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, DWORD rop)
	{
		SetLastError(ERROR_PROC_NOT_FOUND);
		return NULL;
	}

	HBITMAP CreateCompatibleBitmap(HDC hdc, int cx, int cy)
	{
		SetLastError(ERROR_PROC_NOT_FOUND);
		return NULL;
	}

	HDC CreateCompatibleDC(HDC hdc)
	{
		SetLastError(ERROR_PROC_NOT_FOUND);
		return NULL;
	}

	HBITMAP CreateDIBSection(HDC hdc, const BITMAPINFO* pbmi, UINT iUsage, void* ppvBits, HANDLE hSection, DWORD dwOffset)
	{
		SetLastError(ERROR_PROC_NOT_FOUND);
		return NULL;
	}
};