#pragma once

#include <Windows.h>

/*
1. Load WARBIRD::g_arModuleInfo and WARBIRD::g_FuncAddress
	A. Decrypt global imports using warbird cipher
	B. perform XOR checksum on decrypted data (= 64)
	C. verify PE header of all modules
	D. dynamically load g_FuncAddress function pointers using GetProcAddress

2. Do Checks
	A. GetProcessWindowStation
	B. GetUserObjectInformationW (WinSta0, Default)
	C. GetThreadDesktop
	D. GetProcessMitigationPolicy (ImageLoadPolicy?)

3. Prepare Input
	A. Allocate 160 byte structure and fill it with the decryption cipher
	B. ...
*/

// same name decoration as __local_stdio_scanf_options
__declspec(noinline) __inline unsigned __int64* __CRTDECL WarbirdUmGetDecryptionCipher(void)
{
	static unsigned __int64 DecryptionCipher[20] = {
		0x57C6892FC4999795,
		0x2F8D98586E894E24,
		0x0DC566911BFDED922,
		0x4CB32EBB44E6F139,
		0x6E56629A235908A9,
		0x5342221EF1D3DA7B,
		0x0,
		0x0,
		0x0,
		0x0,
		0x0,
		0x0,
		0x0,
		0x0,
		0x0,
		0x0,
		0x91C19160C0F0214,
		0x1F1F1F1F1D061503,
		0x1F1F1F1F1F1F1F1F,
		0x1F1F1F1F1F1F1F1F
	};
	return DecryptionCipher;
}

__declspec(noinline) __inline unsigned __int64* __CRTDECL WarbirdUmGetDecryptionKey(void)
{
	static unsigned __int64 nDecryptionKey = 0x7F1137FAB69605E;
	return &nDecryptionKey;
}

#define WARBIRD_DECRYPTION_CIPHER  (WarbirdUmGetDecryptionCipher ())
#define WARBIRD_DECRYPTION_KEY     (*WarbirdUmGetDecryptionKey ())
