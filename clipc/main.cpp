#include <stdio.h>
#include <stdint.h>
#include <Windows.h>

#include "WarbirdRuntimeSLCCpp.h"

#include "spcallcipherclientimpl.h"
#include "spcallciphertextdata.h"

// d:\os\obj\amd64fre\onecore\ds\security\licensing\clip\client\build\objfre\amd64\WarbirdRuntimeSLCCpp.cpp
// d:\os\obj\amd64fre\onecoreuap\enduser\winstore\licensemanager\tslexchangetask\objfre\amd64\WarbirdRuntimeSLCCpp.cpp

typedef DWORD(*pStoreAppsAreDisabled)();

int main()
{
	HMODULE Lib = LoadLibraryW(L"LicenseManager.dll");

	printf("Base: 0x%08x\n", (ULONGLONG) Lib);
	pStoreAppsAreDisabled StoreAppsAreDisabled = (pStoreAppsAreDisabled)((uint64_t) Lib + 0x37998);

	StoreAppsAreDisabled();

	return 0;
}