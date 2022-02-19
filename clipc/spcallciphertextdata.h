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
	A. Allocate 160 byte structure and fill it with the decryption cipher and allocate another block with key
	B. perform checks on string length of "WSLicensingService-AllowDisablingStoreApps" (inlined?)
	C. allocate sufficient block on heap according to size of string (2*len + 200)+8+8+2 (null byte)
	D. fill initial block data (BlockA)
	E. alloc block with size of (sizeof(blockA)+8) + N for 8 byte alignment (312)
	F. alloc block C with size of Block A Container + 8
	G. XOR checksum for plaintext block A Container
	H. Encryption BlockAContainer using nEncryptionKey
	I. Place XorChecksum 
*/

#define DECRYPTED_SIZE 312
#define TOTAL_SIZE DECRYPTED_SIZE + 8

// 48 bytes
struct BlockD
{
	DWORD BlockCSize;
	CHAR Encrypted[TOTAL_SIZE];
};

struct BlockCContainer
{
	CHAR EncryptedData[DECRYPTED_SIZE];
	ULONG64 XorChecksum;
};

#define LICENSE_STRING_SIZE 0x56

struct BlockA
{
	DWORD LicenseClassSize; // 0x04
	DWORD LicenseClass; // 0 for QueryValue

	DWORD DecryptionCipherSize; // 0xA0
	ULONG64 WbDecryptionCipher[20];

	DWORD DecryptionKeySize; // 0x8
	ULONG64 WbDecryptionKey;

	DWORD TimingSize; // 0x8
	ULONG64 Rdtsc; // __rdtsc()

	DWORD LicenseValueSize;
	WCHAR LicenseValue[LICENSE_STRING_SIZE]; // raw length in bytes not wchar

	DWORD MitigationPolicySize; // 0x4
	BOOL MitigationPolicy; // usually 0?

	DWORD LeftOverSize; // 0x4
	DWORD LeftOver; // 0x4
};

#pragma pack(8)
struct BlockAContainer
{
	DWORD Values; // number of values (not sizes) in BlockA
	DWORD DataSize;

	BlockA Data;
};

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
	static unsigned __int64 nDecryptionKey = 0x07F1137FAB69605E;
	return &nDecryptionKey;
}

__declspec(noinline) __inline unsigned __int64* __CRTDECL WarbirdUmGetEncryptionKey(void)
{
	static unsigned __int64 nEncryptionKey = 0x0C81ECB17B1B54A58;
	return &nEncryptionKey;
}

__declspec(noinline) __inline unsigned __int64* __CRTDECL WarbirdUmGetEncryptionCipher(void)
{
	static unsigned __int64 EncryptionCipher[20] = {
		0x2E63A9A31BE598C9, 0x7C7765E053E2E156, 0x0CE87B95F343D263E, 0x14830898F2E7A986, 0x0C33CBD9D91831E85,
		0x0B205784ABE210C22, 0x0B0E2DCE, 0x0, 0x0, 0x0,
		0x0, 0x0, 0x0, 0x0, 0x0,
		0x0, 0x181A011B090F0E1D, 0x1F1F1F02190B051E, 0x1F1F1F1F1F1F1F1F, 0x1F1F1F1F1F1F1F1F
	};
	return EncryptionCipher;
}

#define WARBIRD_DECRYPTION_CIPHER  (WarbirdUmGetDecryptionCipher ())
#define WARBIRD_DECRYPTION_KEY     (*WarbirdUmGetDecryptionKey ())

#define WARBIRD_ENCRYPTION_CIPHER  (WarbirdUmGetEncryptionCipher ())
#define WARBIRD_ENCRYPTION_KEY     (*WarbirdUmGetEncryptionKey ())
