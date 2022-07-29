# Warbird Runtime
> Reverse engineered code originating from Microsoft's Warbird obfuscation project.

Microsoft developed the Warbird to obfuscate different system components, mostly relating to DRM and licensing.  
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

## Disclaimer
The project cannot be used as-is and is merely used for educational purposes to further public understanding.  Warbird functionality cannot be used as the actual runtime dlls are not present but can be specified by passing the `/wbrddll` flag and exporting the routines `GetObfuscator` and `CreateObfuscator2` to the MSVC compiler.
The project could be loosely modified to support calls to `NtQuerySystemInformation` with the `SystemPolicyInformation` class but you would have to reimplement the cipher, get the encryption/decryption keys, etc... :)


## Related 
- [Reversal of Windows' Client Licensing Service (ClipSp.sys)](https://github.com/KiFilterFiberContext/windows-software-policy)
- [Reversal of Warbird integration in the MSVC compiler](https://github.com/KiFilterFiberContext/warbird-obfuscate)
- [Analysis of the Warbird VM](https://github.com/airbus-seclab/warbirdvm)
