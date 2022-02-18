#include <stdio.h>
#include <stdint.h>

#include "WarbirdRuntimeSLCCpp.h"

#include "spcallcipherclientimpl.h"
#include "spcallciphertextdata.h"

// d:\os\obj\amd64fre\onecore\ds\security\licensing\clip\client\build\objfre\amd64\WarbirdRuntimeSLCCpp.cpp
// d:\os\obj\amd64fre\onecoreuap\enduser\winstore\licensemanager\tslexchangetask\objfre\amd64\WarbirdRuntimeSLCCpp.cpp

typedef DWORD(*pStoreAppsAreDisabled)();

int main()
{
	HMODULE Lib = LoadLibraryW(L".dll");

	// printf("Base: 0x%08x\n", (DWORD) Lib);
	pStoreAppsAreDisabled StoreAppsAreDisabled = (pStoreAppsAreDisabled)((uint64_t) Lib + 0x2fd8);

	StoreAppsAreDisabled();

	WARBIRD::g_WarbirdSecureFunctionsRefCount++;

	int key = WARBIRD_DECRYPTION_KEY;
	unsigned __int64* cipher = WARBIRD_DECRYPTION_CIPHER;

	return 0;
}