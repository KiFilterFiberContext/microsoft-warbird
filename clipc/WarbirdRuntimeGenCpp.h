#pragma once

#include "WarbirdRuntimeSLCCpp.h"

#include <intrin.h>
#include <combaseapi.h>
#include <winstring.h>

namespace WarbirdRuntime
{
	namespace CTermination
	{


		static void TrashStack( )
		{

		}

		static void TrashStack( int ( *Routine )( ) )
		{

		}
	}

	namespace CUtil
	{
		static void* GetCurrentSp( )
		{
			// lea     rax, [rsp]
			// add     rax, 8
			// retn
			return _AddressOfReturnAddress( );
		}

		static void FillRandom( void* Mem )
		{
			BYTE* sp = (BYTE*)( (unsigned __int64) GetCurrentSp( ) & -0x1000 );
			if ( ( uint64_t ) Mem < ( uint64_t ) Mem + 0x10000 )
			{
				
			}
		}
	}

	class AutoEnableDynamicCodeGen
	{
	public:
		DWORD d1;
		DWORD d2;
		DWORD d3;
		DWORD d4;
		bool flag;

		static HRESULT ( *CoCreateInstanceFromAppProc )( _GUID const&, IUnknown*, unsigned long, void*, unsigned long, MULTI_QI* );
		static HRESULT ( *WindowsCreateStringProc )( PCNZWCH, UINT32, HSTRING* );
		static HRESULT ( *WindowsDeleteStringProc )( HSTRING );

		static BOOL	( *GetThreadInformationProc )( HANDLE, THREAD_INFORMATION_CLASS, LPVOID, DWORD );
		static BOOL ( *SetThreadInformationProc )( HANDLE, THREAD_INFORMATION_CLASS, LPVOID, DWORD );
		static BOOL ( *GetProcessMitigationPolicyProc )( HANDLE, PROCESS_MITIGATION_POLICY, PVOID, SIZE_T );

		static bool volatile disableACGCalled;
		static bool volatile processPolicyObtained;

		static PROCESS_MITIGATION_DYNAMIC_CODE_POLICY processPolicy;

		AutoEnableDynamicCodeGen( bool DisableACG )
		{
			DWORD PolicyFlags;
			HMODULE Module;

			d1 = 0x1776DF3;
			d2 = 0x4E50B9AF;
			d3 = 0xE9561C9B;
			d4 = 0x4D71631;

			flag = false;

			if ( DisableACG )
			{
				if ( processPolicyObtained )
				{
					PolicyFlags = processPolicy.Flags;
				}
				else
				{
					Module = GetModuleHandleW( L"api-ms-win-core-processthreads-l1-1-3.dll" );
					if ( Module )
					{
						GetProcessMitigationPolicyProc = (BOOL( * )(HANDLE, PROCESS_MITIGATION_POLICY, PVOID, SIZE_T ) ) GetProcAddress( Module, "GetProcessMitigationPolicy" );
						SetThreadInformationProc = ( BOOL( * )( HANDLE, THREAD_INFORMATION_CLASS, LPVOID, DWORD ) ) GetProcAddress( Module, "SetThreadInformation" );
						GetThreadInformationProc = ( BOOL( * )( HANDLE, THREAD_INFORMATION_CLASS, LPVOID, DWORD ) ) GetProcAddress( Module, "SetThreadInformation" );
					}

					if ( GetProcessMitigationPolicyProc && GetProcessMitigationPolicyProc( Module, ProcessDynamicCodePolicy, &processPolicy, sizeof DWORD ) )
					{
						PolicyFlags = processPolicy.Flags;
					}
					else
					{
						PolicyFlags = processPolicy.Flags & 0xFE;
						processPolicy.Flags &= ~1u;
					}

					processPolicyObtained = true;
				}

				if ( (PolicyFlags & 1) != 0 )
				{

				}
			}
		}
	};

	namespace CMemoryAllocator
	{
		class CBlock
		{
		public:
			static CBlock* Allocate( bool DisableACG )
			{
				DWORD LastError = GetLastError( );

				AutoEnableDynamicCodeGen CodeGen( DisableACG );
				
				void* Block = VirtualAlloc( NULL, 0x10000, MEM_RESERVE | MEM_COMMIT, DisableACG ? 64 : 4 );
				if ( Block )
				{
					CUtil::FillRandom( Block );
					*(( DWORD* ) Block + 16120) = 0;
					*(( DWORD* ) Block + 16121) = 64448;
					*( DWORD* ) (( BYTE* ) Block + 64513) = 0;
					*(( BYTE* ) Block + 64515) = 0;
					*(( BYTE* ) Block + 64512) = ( *(( BYTE* ) Block + 64512) & 0xF9 ) & 6 | 0x19;
					*(( WORD* ) Block + 32260) = 9727;
					*( DWORD* ) (( BYTE* ) Block + 64522) = -54;
					*(( DWORD* ) Block + 16129) = 64520;
					*(( DWORD* ) Block + 16122) = 64512;
					*(( ULONG64* ) Block + 8059) = ( ULONG64 ) printf;
				}

				return NULL;
			}
		};
	}

	typedef struct _ENCRYPTED_SEGMENT_DATA_READ_WRITE
	{

	} ENCRYPTED_SEGMENT_DATA_READ_WRITE, * PENCRYPTED_SEGMENT_DATA_READ_WRITE;

	typedef struct _ENCRYPTED_SEGMENT_DATA_CONST_1
	{
		unsigned __int64 Key;
	} ENCRYPTED_SEGMENT_DATA_CONST, * PENCRYPTED_SEGMENT_DATA_CONST;

	class CRand
	{
	public:
		unsigned __int64 Rand;
	};

	class CRWLock
	{
	public:
		unsigned __int64 Lock;
	};

	struct WarbirdRuntime__HEAP_EXECUTION_CONDITIONAL_BRANCH_ARGUMENTS
	{

	};
	
	struct WarbirdRuntime__HEAP_EXECUTION_CALL_ARGUMENTS
	{
	
	};

	static CMemoryAllocator::CBlock* g_MemoryAllocator;

	static CRWLock g_Lock1;
	static CRWLock g_Lock2;
	static CRWLock g_Lock3;
	static CRWLock g_Lock4;

	static CRand g_Rand;
	static _RTL_CRITICAL_SECTION g_encryptionLock;

	static unsigned long volatile g_PrivateRelocationsTableCount;
	static unsigned long volatile g_PrivateRelocationsTable;
	static unsigned long volatile g_nRvaOriginalEntryMain;

	static unsigned __int64 g_imageBaseForHeapExecution = 0x12345678;

	static ENCRYPTED_SEGMENT_DATA_READ_WRITE g_EncryptedSegmentReadWriteData_1;

	HRESULT RuntimeInit( void )
	{
		g_Lock3.Lock = 0;
		g_Lock1.Lock = 0;
		g_Rand.Rand = __rdtsc( );

		g_MemoryAllocator = CMemoryAllocator::CBlock::Allocate( false );
		if ( g_MemoryAllocator )
		{
			g_Lock2.Lock = 1;
			g_Lock4.Lock = 1;


		}

		return 0;
	}

	int NewEntryMain( void )
	{
		HRESULT Result = RuntimeInit( );
		return 0;
	}
}
