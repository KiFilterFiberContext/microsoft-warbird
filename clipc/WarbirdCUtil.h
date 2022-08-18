#ifdef __cplusplus  
namespace WarbirdRuntime {
#endif // _cpluplus

//
// Size of the common fields of the runtime argument structures.
//
#define RVA_BIT_COUNT               28
#define FUNCTION_SIZE_BIT_COUNT     28
#define KEY_BIT_COUNT               64
#define CHECKSUM_BIT_COUNT           8
#define HASH_BIT_COUNT              64
#define CONDITION_CODE_BIT_COUNT     4
#define STACK_SIZE_BIT_COUNT        28

#define NUMBER_FEISTEL64_ROUNDS     10

//
// Warbird operation types. Indicates for which operation the argument structure
// is for.
//
typedef enum {
    WbOperationNone,
    WbOperationDecryptEncryptionSegment,
    WbOperationReEncryptEncryptionSegment,
    WbOperationHeapExecuteCall,
    WbOperationHeapExecuteReturn,
    WbOperationHeapExecuteUnconditionalBranch,
    WbOperationHeapExecuteConditionalBranch,
    WbOperationProcessEnd,
    WbOperationProcessStartup,
} WbOperationType;

typedef struct _FEISTEL64_ROUND_DATA
{
    unsigned long FunctionID;
    unsigned long Rand0;
    unsigned long Rand1;
    unsigned long Rand2;
} FEISTEL64_ROUND_DATA, PFEISTEL64_ROUND_DATA;

typedef struct _ENCRYPTION_BLOCK {
    unsigned long bUnitialized:1;
    unsigned long bData:1;
    unsigned long ulChecksum:CHECKSUM_BIT_COUNT;
    unsigned long ulRva:RVA_BIT_COUNT;
    unsigned long ulSize:FUNCTION_SIZE_BIT_COUNT;
} ENCRYPTION_BLOCK, *PENCRYPTION_BLOCK;

typedef struct _ENCRYPTION_SEGMENT {
    unsigned long ulVersion;
    unsigned long ulSegmentID;
    unsigned __int64 ullKey;
    FEISTEL64_ROUND_DATA bRoundData[NUMBER_FEISTEL64_ROUNDS];
    unsigned long cBlocks;
    ENCRYPTION_BLOCK Blocks[1];
} ENCRYPTION_SEGMENT, *PENCRYPTION_SEGMENT;

//
// System call heap execution runtime structures and runtime
//
typedef struct _HEAP_EXECUTE_CALL_ARGUMENT {
    unsigned long ulVersion;
    unsigned long ulCheckStackSize;
    unsigned long ulChecksum:CHECKSUM_BIT_COUNT;
    unsigned long ulWrapperChecksum:CHECKSUM_BIT_COUNT;
    unsigned long ulRva:RVA_BIT_COUNT;
    unsigned long ulSize:FUNCTION_SIZE_BIT_COUNT;
    unsigned long ulWrapperRva:RVA_BIT_COUNT;
    unsigned long ulWrapperSize:FUNCTION_SIZE_BIT_COUNT;
    unsigned __int64 ullKey;
    FEISTEL64_ROUND_DATA RoundData[NUMBER_FEISTEL64_ROUNDS];
} HEAP_EXECUTE_CALL_ARGUMENT, *PHEAP_EXECUTE_CALL_ARGUMENT;

//
// Warbird kernel configuration. The user mode process passes this configuration
// to the kernel when it is first started.
//
typedef struct _PROCESS_STARTUP_ARGUMENT {
    unsigned long ulVersion;
    unsigned long cMaxHeapExecutedCacheEntries;
    void* pPreAllocatedReadExecuteMemory;
    unsigned long cbPreAllocatedReadExecuteMemory;
} PROCESS_STARTUP_ARGUMENT, *PPROCESS_STARTUP_ARGUMENT;

typedef struct _PROCESS_STARTUP_ARGUMENT_LIST {
    unsigned __int64 eType;
    PPROCESS_STARTUP_ARGUMENT pArguments;
} PROCESS_STARTUP_ARGUMENT_LIST, *PPROCESS_STARTUP_ARGUMENT_LIST;

#ifdef __cplusplus  
}; // namespace WarbirdRuntime
#endif // _cpluplus
#if defined(WARBIRD_KERNEL_MODE)
#include <ntos.h>
#include <ntddk.h>
#include <ntstrsafe.h>
#include <ntimage.h>
#include <minwindef.h>
#include <winerror.h>
#include <bcrypt.h>
extern "C" void* _AddressOfReturnAddress();
#else // defined(WARBIRD_KERNEL_MODE)
#include <windows.h>
#include <strsafe.h>
#include <malloc.h>
#include <intrin.h>
#endif // defined(WARBIRD_KERNEL_MODE)
#include <limits.h>
#ifdef _M_E2
#include <ntarch.h>
#undef PAGE_SIZE 
#define PAGE_SIZE NTARCH_PAGE_SIZE
#elif defined(_X86_) || defined(_AMD64_) || defined(_ARM_) || defined(_ARM64_)
#undef PAGE_SIZE 
#define PAGE_SIZE 0x1000
#endif // _M_E2

//
// Warbird runtime's use intrinsics to reduce hookable attack points
//
#pragma intrinsic(memcpy, memcmp, memset)

#define STRINGIZE(x) #x

#if !defined(ALG_TYPE_ANY)
#define ALG_TYPE_ANY                    (0)
#endif
#if !defined(ALG_CLASS_ANY)
#define ALG_CLASS_ANY                   (0)
#endif
#if !defined(ALG_CLASS_HASH)
#define ALG_CLASS_HASH                  (4 << 13)
#endif
#if !defined(ALG_SID_SHA_256)
#define ALG_SID_SHA_256                 12
#endif
#if !defined(ALG_SID_SHA_512)
#define ALG_SID_SHA_512                 14
#endif
#if !defined(CALG_SHA_512)
#define CALG_SHA_512            (ALG_CLASS_HASH | ALG_TYPE_ANY | ALG_SID_SHA_512)
#endif


#if defined(_WIN64)
#define _InterlockedCompareExchangeSizeT    _InterlockedCompareExchange64
#define _InterlockedDecrementSizeT          _InterlockedDecrement64
#define _InterlockedExchangeSizeT           _InterlockedExchange64
#else
#define _InterlockedCompareExchangeSizeT    _InterlockedCompareExchange
#define _InterlockedDecrementSizeT          _InterlockedDecrement
#define _InterlockedExchangeSizeT           _InterlockedExchange
#endif

#if defined(WARBIRD_DEBUG)
#define DebugPrint                          CUtil::DebugPrintFunc
#define WARBIRD_ASSERT                      CUtil::AssertFunc
#else
#define DebugPrint                          __noop
#define WARBIRD_ASSERT                      __noop
#endif

#ifndef HRESULT_FROM_NTSTATUS
#define HRESULT_FROM_NTSTATUS(x)      ((HRESULT) ((x) | FACILITY_NT_BIT))  
#endif // HRESULT_FROM_NTSTATUS

#ifndef WARBIRD_KERNEL_MODE

#define SystemCodeFlowTransition 185

EXTERN_C ULONG WINAPI
NtQuerySystemInformation(
    ULONG SystemInformationClass,
    PVOID SystemInformation,
    ULONG SystemInformationLength,
    PULONG ReturnLength
    );

#endif // !WARBIRD_KERNEL_MODE

#if defined(WARBIRD_KERNEL_MODE)
/*++

Description:

    Define ZwFlushInstructionCache (from zwapi.h)

--*/
EXTERN_C
NTSYSAPI
NTSTATUS
NTAPI
ZwFlushInstructionCache (
    __in HANDLE ProcessHandle,
    __in_opt PVOID BaseAddress,
    __in SIZE_T Length
    );

#ifndef ALG_ID
typedef unsigned int ALG_ID;
#endif

#endif
