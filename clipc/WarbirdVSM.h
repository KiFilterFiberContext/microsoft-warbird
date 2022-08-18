#if $(WARBIRD_ENABLE_VSM) && (defined(WARBIRD_KERNEL_MODE) || defined(WARBIRD_VSM_TEST))

namespace WarbirdRuntime
{

#if ($(WARBIRD_VSM_VERSION) == 2)
struct VSM_PAGE_ENTRY
{
    UCHAR HashBuffer[$(WARBIRD_VSM_HASH_LENGTH)]; 
};

__declspec( align($(WARBIRD_VSM_HASH_LENGTH)) ) struct VSM_HASH_TABLE
{
    ULONG  Length;
    ULONG  Version;
    ALG_ID HashAlgorithm;
    ULONG  NumberOfPageHashes;
    ULONG  Flag;
    BYTE   Padding[$(WARBIRD_VSM_HASH_LENGTH) - sizeof(ULONG) - sizeof(ULONG) - sizeof(ALG_ID) - sizeof(ULONG) - sizeof(ULONG)];
    VSM_PAGE_ENTRY Hashes [ANYSIZE_ARRAY];
};

struct VSM_INDEX_ENTRY
{
    ULONG Index;    // Index in Hashes for the first page of the Warbird segment
    ULONG Pages;    // Number of pages in the Warbird segment
    ULONG Rva;      // First page in the Warbird segment
    ULONG SegId;    // Segment Id
    ULONG FileOffset; // Added in version 2 - invalid memory location in early version 1 images
};

// Hash must be at larger then header fields in VSM_HASH_TABLE
C_ASSERT($(WARBIRD_VSM_HASH_LENGTH) >= sizeof(ULONG)*5);
#else  // ($(WARBIRD_VSM_VERSION) == 2)
#error UNDEFINED WARBIRD VSM VERSION
#endif // ($(WARBIRD_VSM_VERSION) == 2)
}  // WarbirdRuntime

//
// VSM global variables 
//

namespace WarbirdRuntime
{

#ifndef STATUS_SUCCESS
#define STATUS_SUCCESS  ((NTSTATUS)0x00000000L)
#endif

#define GetVsmHashTable() (VSM_HASH_TABLE*)((ULONG_PTR)(g_VsmHashTable) + CUtil::GetImageBase())

volatile ULONG g_VsmHashTable = 0x12456908;

#pragma section(".rdata$VsmData", read)

__declspec(allocate(".rdata$VsmData"))
__declspec(selectany)
volatile VSM_INDEX_ENTRY g_VsmSegmentIndex[$(NumSegments)] = {0};

// 
// Coming soon for HVCI
#if !defined(MM_CHANGE_ENABLE_EXECUTE)
#define MM_CHANGE_ENABLE_EXECUTE 1
#endif
#if !defined(MM_CHANGE_DISABLE_EXECUTE)
#define MM_CHANGE_DISABLE_EXECUTE 2
#endif

}  // WarbirdRuntime

#endif // $(WARBIRD_ENABLE_VSM) && (defined(WARBIRD_KERNEL_MODE) || defined(WARBIRD_VSM_TEST))
