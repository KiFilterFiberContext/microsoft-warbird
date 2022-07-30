# Warbird Runtime
> Reverse engineered code originating from the runtime of Microsoft Warbird.

Microsoft developed Warbird to obfuscate different system components, mostly relating to DRM and licensing.  
Warbird code has even been integrated into the kernel to obfuscate components (`ci`, `peauth`, `clipsp`, etc).

## Overview

Warbird can support different forms of code obfuscation like:
- virtual machine obfuscation
- section encryption/packing (labeled `PAGEwx` sections which are excluded from PatchGuard)
- constant obfuscation
- [runtime code execution](https://www.youtube.com/watch?v=gu_i6LYuePg) on the heap (`SystemControlFlowTransition` info class for `NtQuerySystemInformation` system call)

Some system images that are publicly known to use Warbird are:
- `ci.dll`
- `clipsp.sys`
- `peauth.sys`
- `ntoskrnl.exe`
- `ClipSVC.dll`
- `ClipUp.exe`
- `Windows.Media.Protection.PlayReady.dll`
- `GenValObj.exe`
- `ManageCI.dll`

There are other images that make calls to `NtQuerySystemInformation` passing the `SystemPolicyInformation` info class that rely on the Warbird Feistel cipher to encrypt/decrypt parameters.

The call stack of those calls generally look like:
`nt!SpCall2ServerInternal` -> `nt!g_kernelCallbacks` -> `clipsp.sys!ClipSp*`. 

More information can be found on that [here](https://github.com/KiFilterFiberContext/windows-software-policy).

## TODO: Classes to Implement
- `WarbirdRuntime::CTermination` 
- `WarbirdRuntime::CMemoryAllocator::CBlock`
- `WarbirdRuntime::CEncryption`
- `WarbirdRuntime::CPrivateRelocationsTable`
- `WarbirdRuntime::CHeapExecution`
- `WarbirdRuntime::AutoEnableDynamicCodeGen`
- `WarbirdRuntime::CUtil`
- `WarbirdRuntime::CRWLock`
- `WarbirdRuntime::CMemoryProtectionChangeHelper`
- `WarbirdRuntime::WarbirdRuntime__HEAP_EXECUTION_CALL_ARGUMENTS`
- `WarbirdRuntime::WarbirdRuntime__HEAP_EXECUTION_CONDITIONAL_BRANCH_ARGUMENTS`
- `WarbirdRuntime::ENCRYPTED_SEGMENT_DATA_CONST`
- `WarbirdRuntime::ENCRYPTED_SEGMENT_DATA_READ_WRITE`
- `WarbirdRuntime::RuntimeInit`
- `WarbirdRuntime::NewEntryMain`

## Disclaimer
The project cannot be used as-is and is merely intended to be used for educational purposes to further public understanding.  
Warbird cannot be integrated into personal projects since the actual runtime dlls are not present.  

A custom Warbird obfuscator toolset could be partially integrated by passing the `/wbrddll:` MSVC compiler flag and exporting the `GetObfuscator` and `CreateObfuscator2` routines.

The project could be loosely modified to support calls to `NtQuerySystemInformation` with the `SystemPolicyInformation` class but you would have to reimplement the cipher, get the encryption/decryption keys, etc... :)

## Related 
- [Reversal of Windows' Client Licensing Service (ClipSp.sys)](https://github.com/KiFilterFiberContext/windows-software-policy)
- [Reversal of Warbird integration in the MSVC compiler](https://github.com/KiFilterFiberContext/warbird-obfuscate)
- [Analysis of the Warbird VM](https://github.com/airbus-seclab/warbirdvm)
