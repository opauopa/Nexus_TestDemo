
#include "NexusDrvSDK.h"
#include"NexusDrvResouce.h"

#include <cstdio>
#include <fstream>
#include <string>
#include <vector>
// 

#pragma warning(push)
#pragma warning(disable: 4005)
#include "MemoryModule.h"
#pragma warning(pop)


#pragma comment(lib, "ntdll.lib")


static HMEMORYMODULE g_hNexusDrvModule = nullptr;
static const char* g_lastMissingExport = nullptr;
static NTSTATUS g_lastECallStatus = STATUS_SUCCESS;

static FARPROC GetRequiredExport(HMEMORYMODULE module, const char* exportName)
{
    FARPROC proc = MemoryGetProcAddress(module, exportName);
    if (!proc) {
        g_lastMissingExport = exportName;
        std::fprintf(stderr, "[NexusSDK] Missing export: %s\n", exportName);
    }

    return proc;
}

typedef NEXUS_STATUS(WINAPI* PFN_Nexus_InitDrv)(const char* code, bool init_with_pdb);
typedef NTSTATUS(WINAPI* PFN_Nexus_UnloadDrv)();
typedef bool(WINAPI* PFN_Nexus_TestComm)();
typedef NTSTATUS(WINAPI* PFN_Nexus_InitRWProcess)(ULONG pid, BOOLEAN needFuckCr3);
typedef ULONG64(WINAPI* PFN_Nexus_GetProcessCr3)(ULONG pid, BOOLEAN needFuckCr3);
typedef NTSTATUS(WINAPI* PFN_Nexus_ReadProcessPhysicalMemory)(ULONG pid, ULONG64 address, PVOID buffer, SIZE_T size);
typedef NTSTATUS(WINAPI* PFN_Nexus_WriteProcessPhysicalMemory)(ULONG pid, ULONG64 address, PVOID buffer, SIZE_T size);
typedef NTSTATUS(WINAPI* PFN_Nexus_InvlpgReadProcessPhysicalMemory)(ULONG pid, ULONG64 address, PVOID buffer, SIZE_T size);
typedef NTSTATUS(WINAPI* PFN_Nexus_InvlpgWriteProcessPhysicalMemory)(ULONG pid, ULONG64 address, PVOID buffer, SIZE_T size);
typedef NTSTATUS(WINAPI* PFN_Nexus_GetProcessModuleInfo_Attach)(ULONG pid, PULONG64 retModuleAddress);
typedef NTSTATUS(WINAPI* PFN_Nexus_GetProcessModuleInfo_NoAttach)(ULONG pid, PWCHAR moduleName, PULONG64 retModuleAddress, SIZE_T* retModuleSize);
typedef NTSTATUS(WINAPI* PFN_Nexus_GetKernelModuleInfo)(PCHAR moduleName, PULONG64 retModuleAddress, SIZE_T* retModuleSize);
typedef NTSTATUS(WINAPI* PFN_Nexus_PatternSearch)(ULONG pid, PWCHAR moduleName, const char* pattern, const char* mask, const char* secName, PULONG64 addr);
typedef NTSTATUS(WINAPI* PFN_Nexus_AllocateMemory)(ULONG pid, SIZE_T allocSize, ULONG protect, AllocateType type, PULONG64 allocAddress);
typedef NTSTATUS(WINAPI* PFN_Nexus_FreeMemory)(ULONG pid, ULONG64 free_address, SIZE_T memory_size);
typedef NTSTATUS(WINAPI* PFN_Nexus_InjectWithApcAttach)(ULONG pid, PCHAR dll_buffer, SIZE_T dll_size, AllocateType allocate_type, PULONG64 allocAddr);
typedef NTSTATUS(WINAPI* PFN_Nexus_InjectWithApcNoAttach)(ULONG pid, PCHAR dll_buffer, SIZE_T dll_size, AllocateType allocate_type, PULONG64 allocAddr);
typedef NTSTATUS(WINAPI* PFN_Nexus_InjectWithHijackThreadAttach)(ULONG pid, PCHAR dll_buffer, SIZE_T dll_size, AllocateType allocate_type, PULONG64 allocAddr);
typedef NTSTATUS(WINAPI* PFN_Nexus_InjectWithHijackThreadNoAttach)(ULONG pid, PCHAR dll_buffer, SIZE_T dll_size, AllocateType allocate_type, PULONG64 allocAddr);
typedef NTSTATUS(WINAPI* PFN_Nexus_InjectWithRemoteThread)(ULONG pid, PCHAR dll_buffer, SIZE_T dll_size, PULONG64 allocAddr);

typedef VOID(WINAPI* PFN_Nexus_MouseSendInput)(long LastX, long LastY, unsigned short button_flags, USHORT Flags);
typedef VOID(WINAPI* PFN_Nexus_MouseRelativeMovement)(long LastX, long LastY);
typedef VOID(WINAPI* PFN_Nexus_MouseAbsoluteMovement)(long x, long y);
typedef VOID(WINAPI* PFN_Nexus_MouseDown)();
typedef VOID(WINAPI* PFN_Nexus_MouseUp)();

typedef NTSTATUS(WINAPI* PFN_Nexus_MouseSendInputEx)(long LastX, long LastY, unsigned short button_flags, USHORT Flags);
typedef VOID(WINAPI* PFN_Nexus_MouseRelativeMovementEx)(long LastX, long LastY);
typedef VOID(WINAPI* PFN_Nexus_MouseAbsoluteMovementEx)(long x, long y);
typedef VOID(WINAPI* PFN_Nexus_MouseDownEx)();
typedef VOID(WINAPI* PFN_Nexus_MouseUpEx)();

typedef VOID(WINAPI* PFN_Nexus_KeyPressed)(DWORD KeyCode);
typedef VOID(WINAPI* PFN_Nexus_KeyRelease)(DWORD KeyCode);
typedef VOID(WINAPI* PFN_Nexus_ProtectProcessPPL)(ULONG pid);
typedef VOID(WINAPI* PFN_Nexus_AntiScreenShot)(ULONG pid);
typedef bool(WINAPI* PFN_Nexus_CheckAntiVm)();
typedef bool(WINAPI* PFN_Nexus_CheckAntiDebug)();
// 鍐呴儴鍑芥暟鎸囬拡
static PFN_Nexus_InitDrv pfn_Nexus_InitDrv = nullptr;
static PFN_Nexus_UnloadDrv pfn_Nexus_UnloadDrv = nullptr;
static PFN_Nexus_TestComm pfn_Nexus_TestComm = nullptr;
static PFN_Nexus_InitRWProcess pfn_Nexus_InitRWProcess = nullptr;
static PFN_Nexus_GetProcessCr3 pfn_Nexus_GetProcessCr3 = nullptr;
static PFN_Nexus_ReadProcessPhysicalMemory pfn_Nexus_ReadProcessPhysicalMemory = nullptr;
static PFN_Nexus_WriteProcessPhysicalMemory pfn_Nexus_WriteProcessPhysicalMemory = nullptr;
static PFN_Nexus_InvlpgReadProcessPhysicalMemory pfn_Nexus_InvlpgReadProcessPhysicalMemory = nullptr;
static PFN_Nexus_InvlpgWriteProcessPhysicalMemory pfn_Nexus_InvlpgWriteProcessPhysicalMemory = nullptr;
static PFN_Nexus_GetProcessModuleInfo_Attach pfn_Nexus_GetProcessModuleInfo_Attach = nullptr;
static PFN_Nexus_GetProcessModuleInfo_NoAttach pfn_Nexus_GetProcessModuleInfo_NoAttach = nullptr;
static PFN_Nexus_GetKernelModuleInfo pfn_Nexus_GetKernelModuleInfo = nullptr;
static PFN_Nexus_PatternSearch pfn_Nexus_PatternSearch = nullptr;
static PFN_Nexus_AllocateMemory pfn_Nexus_AllocateMemory = nullptr;
static PFN_Nexus_FreeMemory pfn_Nexus_FreeMemory = nullptr;
static PFN_Nexus_InjectWithApcAttach pfn_Nexus_InjectWithApcAttach = nullptr;
static PFN_Nexus_InjectWithApcNoAttach pfn_Nexus_InjectWithApcNoAttach = nullptr;
static PFN_Nexus_InjectWithHijackThreadAttach pfn_Nexus_InjectWithHijackThreadAttach = nullptr;
static PFN_Nexus_InjectWithHijackThreadNoAttach pfn_Nexus_InjectWithHijackThreadNoAttach = nullptr;
static PFN_Nexus_InjectWithRemoteThread pfn_Nexus_InjectWithRemoteThread = nullptr;
static PFN_Nexus_MouseRelativeMovement pfn_Nexus_MouseRelativeMovement = nullptr;
static PFN_Nexus_MouseAbsoluteMovement pfn_Nexus_MouseAbsoluteMovement = nullptr;
static PFN_Nexus_MouseDown pfn_Nexus_MouseDown = nullptr;
static PFN_Nexus_MouseUp pfn_Nexus_MouseUp = nullptr;
static PFN_Nexus_MouseSendInput pfn_Nexus_MouseSendInput = nullptr;

static PFN_Nexus_MouseSendInputEx pfn_Nexus_MouseSendInputEx = nullptr;
static PFN_Nexus_MouseRelativeMovementEx pfn_Nexus_MouseRelativeMovementEx = nullptr;
static PFN_Nexus_MouseAbsoluteMovementEx pfn_Nexus_MouseAbsoluteMovementEx = nullptr;
static PFN_Nexus_MouseDownEx pfn_Nexus_MouseDownEx = nullptr;
static PFN_Nexus_MouseUpEx pfn_Nexus_MouseUpEx = nullptr;

static PFN_Nexus_KeyPressed pfn_Nexus_KeyPressed = nullptr;
static PFN_Nexus_KeyRelease pfn_Nexus_KeyRelease = nullptr;
static PFN_Nexus_ProtectProcessPPL pfn_Nexus_ProtectProcessPPL = nullptr;
static PFN_Nexus_AntiScreenShot pfn_Nexus_AntiScreenShot = nullptr;
static PFN_Nexus_CheckAntiVm pfn_Nexus_CheckAntiVm = nullptr;
static PFN_Nexus_CheckAntiDebug pfn_Nexus_CheckAntiDebug = nullptr;
template <typename T>
static bool LoadRequiredExport(HMEMORYMODULE module, const char* exportName, T& target)
{
    target = reinterpret_cast<T>(GetRequiredExport(module, exportName));
    return target != nullptr;
}

static void ResetLoadedExports()
{
    g_lastECallStatus = STATUS_SUCCESS;
    pfn_Nexus_InitDrv = nullptr;
    pfn_Nexus_UnloadDrv = nullptr;
    pfn_Nexus_TestComm = nullptr;
    pfn_Nexus_InitRWProcess = nullptr;
    pfn_Nexus_GetProcessCr3 = nullptr;
    pfn_Nexus_ReadProcessPhysicalMemory = nullptr;
    pfn_Nexus_WriteProcessPhysicalMemory = nullptr;
    pfn_Nexus_InvlpgReadProcessPhysicalMemory = nullptr;
    pfn_Nexus_InvlpgWriteProcessPhysicalMemory = nullptr;
    pfn_Nexus_GetProcessModuleInfo_Attach = nullptr;
    pfn_Nexus_GetProcessModuleInfo_NoAttach = nullptr;
    pfn_Nexus_GetKernelModuleInfo = nullptr;
    pfn_Nexus_PatternSearch = nullptr;
    pfn_Nexus_AllocateMemory = nullptr;
    pfn_Nexus_FreeMemory = nullptr;
    pfn_Nexus_InjectWithApcAttach = nullptr;
    pfn_Nexus_InjectWithApcNoAttach = nullptr;
    pfn_Nexus_InjectWithHijackThreadAttach = nullptr;
    pfn_Nexus_InjectWithHijackThreadNoAttach = nullptr;
    pfn_Nexus_InjectWithRemoteThread = nullptr;
    pfn_Nexus_MouseRelativeMovement = nullptr;
    pfn_Nexus_MouseAbsoluteMovement = nullptr;
    pfn_Nexus_MouseDown = nullptr;
    pfn_Nexus_MouseUp = nullptr;
    pfn_Nexus_MouseSendInput = nullptr;
    pfn_Nexus_MouseSendInputEx = nullptr;
    pfn_Nexus_MouseRelativeMovementEx = nullptr;
    pfn_Nexus_MouseAbsoluteMovementEx = nullptr;
    pfn_Nexus_MouseDownEx = nullptr;
    pfn_Nexus_MouseUpEx = nullptr;
    pfn_Nexus_KeyPressed = nullptr;
    pfn_Nexus_KeyRelease = nullptr;
    pfn_Nexus_ProtectProcessPPL = nullptr;
    pfn_Nexus_AntiScreenShot = nullptr;
    pfn_Nexus_CheckAntiVm = nullptr;
    pfn_Nexus_CheckAntiDebug = nullptr;
}

static bool LoadModuleExports(HMEMORYMODULE module)
{
    bool success = true;

    success &= LoadRequiredExport(module, "Nexus_InitDrv", pfn_Nexus_InitDrv);
    success &= LoadRequiredExport(module, "Nexus_UnloadDrv", pfn_Nexus_UnloadDrv);
    success &= LoadRequiredExport(module, "Nexus_TestComm", pfn_Nexus_TestComm);
    success &= LoadRequiredExport(module, "Nexus_InitRWProcess", pfn_Nexus_InitRWProcess);
    success &= LoadRequiredExport(module, "Nexus_GetProcessCr3", pfn_Nexus_GetProcessCr3);
    success &= LoadRequiredExport(module, "Nexus_ReadProcessPhysicalMemory", pfn_Nexus_ReadProcessPhysicalMemory);
    success &= LoadRequiredExport(module, "Nexus_WriteProcessPhysicalMemory", pfn_Nexus_WriteProcessPhysicalMemory);
    success &= LoadRequiredExport(module, "Nexus_InvlpgReadProcessPhysicalMemory", pfn_Nexus_InvlpgReadProcessPhysicalMemory);
    success &= LoadRequiredExport(module, "Nexus_InvlpgWriteProcessPhysicalMemory", pfn_Nexus_InvlpgWriteProcessPhysicalMemory);
    success &= LoadRequiredExport(module, "Nexus_GetProcessModuleInfo_Attach", pfn_Nexus_GetProcessModuleInfo_Attach);
    success &= LoadRequiredExport(module, "Nexus_GetProcessModuleInfo_NoAttach", pfn_Nexus_GetProcessModuleInfo_NoAttach);
    success &= LoadRequiredExport(module, "Nexus_GetKernelModuleInfo", pfn_Nexus_GetKernelModuleInfo);
    success &= LoadRequiredExport(module, "Nexus_PatternSearch", pfn_Nexus_PatternSearch);
    success &= LoadRequiredExport(module, "Nexus_AllocateMemory", pfn_Nexus_AllocateMemory);
    success &= LoadRequiredExport(module, "Nexus_FreeMemory", pfn_Nexus_FreeMemory);
    success &= LoadRequiredExport(module, "Nexus_InjectWithApcAttach", pfn_Nexus_InjectWithApcAttach);
    success &= LoadRequiredExport(module, "Nexus_InjectWithApcNoAttach", pfn_Nexus_InjectWithApcNoAttach);
    success &= LoadRequiredExport(module, "Nexus_InjectWithHijackThreadAttach", pfn_Nexus_InjectWithHijackThreadAttach);
    success &= LoadRequiredExport(module, "Nexus_InjectWithHijackThreadNoAttach", pfn_Nexus_InjectWithHijackThreadNoAttach);
    success &= LoadRequiredExport(module, "Nexus_InjectWithRemoteThread", pfn_Nexus_InjectWithRemoteThread);
    success &= LoadRequiredExport(module, "Nexus_MouseRelativeMovement", pfn_Nexus_MouseRelativeMovement);
    success &= LoadRequiredExport(module, "Nexus_MouseAbsoluteMovement", pfn_Nexus_MouseAbsoluteMovement);
    success &= LoadRequiredExport(module, "Nexus_MouseDown", pfn_Nexus_MouseDown);
    success &= LoadRequiredExport(module, "Nexus_MouseUp", pfn_Nexus_MouseUp);
    success &= LoadRequiredExport(module, "Nexus_MouseSendInput", pfn_Nexus_MouseSendInput);
    success &= LoadRequiredExport(module, "Nexus_MouseSendInputEx", pfn_Nexus_MouseSendInputEx);
    success &= LoadRequiredExport(module, "Nexus_MouseRelativeMovementEx", pfn_Nexus_MouseRelativeMovementEx);
    success &= LoadRequiredExport(module, "Nexus_MouseAbsoluteMovementEx", pfn_Nexus_MouseAbsoluteMovementEx);
    success &= LoadRequiredExport(module, "Nexus_MouseDownEx", pfn_Nexus_MouseDownEx);
    success &= LoadRequiredExport(module, "Nexus_MouseUpEx", pfn_Nexus_MouseUpEx);
    success &= LoadRequiredExport(module, "Nexus_KeyPressed", pfn_Nexus_KeyPressed);
    success &= LoadRequiredExport(module, "Nexus_KeyRelease", pfn_Nexus_KeyRelease);
    success &= LoadRequiredExport(module, "Nexus_CheckAntiVm", pfn_Nexus_CheckAntiVm);
    success &= LoadRequiredExport(module, "Nexus_CheckAntiDebug", pfn_Nexus_CheckAntiDebug);

    return success;
}


// 鍒濆鍖栨ā鍧?
NEXUS_SDK_API bool Nexus_InitializeModule(const char* dllPath)
{
    if (g_hNexusDrvModule) {
        return true;
    }

    g_lastMissingExport = nullptr;
    ResetLoadedExports();

    
    // Read DLL file into memory
    std::ifstream file(dllPath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        return false;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<BYTE> buffer(size);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        return false;
    }
    file.close();

    g_hNexusDrvModule = MemoryLoadLibrary(buffer.data(), buffer.size());

    if (!g_hNexusDrvModule) {
        return false;
    }
    if (!LoadModuleExports(g_hNexusDrvModule)) {
        if (g_lastMissingExport) {
            std::fprintf(stderr, "[NexusSDK] Initialization aborted because export '%s' was not found.\n", g_lastMissingExport);
        }
        MemoryFreeLibrary(g_hNexusDrvModule);
        g_hNexusDrvModule = nullptr;
        ResetLoadedExports();
        return false;
    }
    return true;
}

NEXUS_SDK_API bool  Nexus_InitializeModuleWithMemory()
{
    if (g_hNexusDrvModule) {
        return true;
    }

    g_lastMissingExport = nullptr;
    ResetLoadedExports();


    // Use MemoryModule to load DLL from memory
    g_hNexusDrvModule = MemoryLoadLibrary(driver_data, sizeof(driver_data));

    if (!g_hNexusDrvModule) {
        std::fprintf(stderr, "[NexusSDK] MemoryLoadLibrary failed.\n");
        return false;
    }
    if (!LoadModuleExports(g_hNexusDrvModule)) {
        if (g_lastMissingExport) {
            std::fprintf(stderr, "[NexusSDK] Initialization aborted because export '%s' was not found.\n", g_lastMissingExport);
        }
        MemoryFreeLibrary(g_hNexusDrvModule);
        g_hNexusDrvModule = nullptr;
        ResetLoadedExports();
        return false;
    }

    // Clear PE headers and unlink module for stealth.
    PMEMORYMODULE module = (PMEMORYMODULE)g_hNexusDrvModule;
    ClearPEHeadersEx(reinterpret_cast<unsigned char*>(module->codeBase));
#ifdef _WIN64
    UnlinkModule(reinterpret_cast<unsigned char*>(module->codeBase));
#endif
    memset(driver_data, 0, sizeof(driver_data));
    return true;

}





// SDK鍑芥暟瀹炵幇
NEXUS_SDK_API NEXUS_STATUS WINAPI Nexus_InitDrv(const char* code, bool init_with_pdb)
{
   
    return pfn_Nexus_InitDrv(code, init_with_pdb);
}

NEXUS_SDK_API NTSTATUS WINAPI Nexus_UnloadDrv()
{
    return pfn_Nexus_UnloadDrv();
}

NEXUS_SDK_API bool WINAPI Nexus_TestComm()
{
    return pfn_Nexus_TestComm();
}

NEXUS_SDK_API NTSTATUS WINAPI Nexus_InitRWProcess(ULONG pid, BOOLEAN needFuckCr3)
{
    return pfn_Nexus_InitRWProcess(pid, needFuckCr3);
}

NEXUS_SDK_API ULONG64 WINAPI Nexus_GetProcessCr3(ULONG pid, BOOLEAN needFuckCr3 )
{
	return pfn_Nexus_GetProcessCr3(pid, needFuckCr3);
}
NEXUS_SDK_API NTSTATUS WINAPI Nexus_ReadProcessPhysicalMemory(ULONG pid, ULONG64 address, PVOID buffer, SIZE_T size)
{
    return pfn_Nexus_ReadProcessPhysicalMemory(pid, address, buffer, size);
}

NEXUS_SDK_API NTSTATUS WINAPI Nexus_WriteProcessPhysicalMemory(ULONG pid, ULONG64 address, PVOID buffer, SIZE_T size)
{
    return pfn_Nexus_WriteProcessPhysicalMemory(pid, address, buffer, size);
}

NEXUS_SDK_API NTSTATUS WINAPI Nexus_InvlpgReadProcessPhysicalMemory(ULONG pid, ULONG64 address, PVOID buffer, SIZE_T size)
{
    return pfn_Nexus_InvlpgReadProcessPhysicalMemory(pid, address, buffer, size);
}

NEXUS_SDK_API NTSTATUS WINAPI Nexus_InvlpgWriteProcessPhysicalMemory(ULONG pid, ULONG64 address, PVOID buffer, SIZE_T size)
{
    return pfn_Nexus_InvlpgWriteProcessPhysicalMemory(pid, address, buffer, size);
}

NEXUS_SDK_API NTSTATUS WINAPI Nexus_GetProcessModuleInfo_Attach(ULONG pid, PULONG64 retModuleAddress)
{
    return pfn_Nexus_GetProcessModuleInfo_Attach(pid, retModuleAddress);
}

NEXUS_SDK_API NTSTATUS WINAPI Nexus_GetProcessModuleInfo_NoAttach(ULONG pid, PWCHAR moduleName, PULONG64 retModuleAddress, SIZE_T* retModuleSize)
{
    return pfn_Nexus_GetProcessModuleInfo_NoAttach(pid, moduleName, retModuleAddress, retModuleSize);
}

NEXUS_SDK_API NTSTATUS WINAPI Nexus_GetKernelModuleInfo(PCHAR moduleName, PULONG64 retModuleAddress, SIZE_T* retModuleSize)
{
    return pfn_Nexus_GetKernelModuleInfo(moduleName, retModuleAddress, retModuleSize);
}

NEXUS_SDK_API NTSTATUS WINAPI Nexus_PatternSearch(ULONG pid, PWCHAR moduleName, const char* pattern, const char* mask, const char* secName, PULONG64 addr)
{
    return pfn_Nexus_PatternSearch(pid, moduleName, pattern, mask, secName, addr);
}

NEXUS_SDK_API NTSTATUS WINAPI Nexus_AllocateMemory(ULONG pid, SIZE_T allocSize, ULONG protect, AllocateType type, PULONG64 allocAddress)
{
    return pfn_Nexus_AllocateMemory(pid, allocSize, protect, type, allocAddress);
}

NEXUS_SDK_API NTSTATUS WINAPI Nexus_FreeMemory(ULONG pid, ULONG64 free_address, SIZE_T memory_size)
{
    return pfn_Nexus_FreeMemory(pid, free_address, memory_size);
}

NEXUS_SDK_API NTSTATUS WINAPI Nexus_InjectWithApcAttach(ULONG pid, PCHAR dll_buffer, SIZE_T dll_size, AllocateType allocate_type, PULONG64 allocAddr)
{
    return pfn_Nexus_InjectWithApcAttach(pid, dll_buffer, dll_size, allocate_type, allocAddr);
}

NEXUS_SDK_API NTSTATUS WINAPI Nexus_InjectWithApcNoAttach(ULONG pid, PCHAR dll_buffer, SIZE_T dll_size, AllocateType allocate_type, PULONG64 allocAddr)
{
    return pfn_Nexus_InjectWithApcNoAttach(pid, dll_buffer, dll_size, allocate_type, allocAddr);
}

NEXUS_SDK_API NTSTATUS WINAPI Nexus_InjectWithHijackThreadAttach(ULONG pid, PCHAR dll_buffer, SIZE_T dll_size, AllocateType allocate_type, PULONG64 allocAddr)
{
    return pfn_Nexus_InjectWithHijackThreadAttach(pid, dll_buffer, dll_size, allocate_type, allocAddr);
}

NEXUS_SDK_API NTSTATUS WINAPI Nexus_InjectWithHijackThreadNoAttach(ULONG pid, PCHAR dll_buffer, SIZE_T dll_size, AllocateType allocate_type, PULONG64 allocAddr)
{
    return pfn_Nexus_InjectWithHijackThreadNoAttach(pid, dll_buffer, dll_size, allocate_type, allocAddr);
}

NEXUS_SDK_API NTSTATUS WINAPI Nexus_InjectWithRemoteThread(ULONG pid, PCHAR dll_buffer, SIZE_T dll_size, PULONG64 allocAddr)
{
    return pfn_Nexus_InjectWithRemoteThread(pid, dll_buffer, dll_size, allocAddr);
}

NEXUS_SDK_API VOID WINAPI Nexus_MouseRelativeMovement(long LastX, long LastY)
{
    pfn_Nexus_MouseRelativeMovement(LastX, LastY);
}

NEXUS_SDK_API VOID WINAPI Nexus_MouseAbsoluteMovement(long x, long y)
{
    pfn_Nexus_MouseAbsoluteMovement(x, y);
}

NEXUS_SDK_API VOID WINAPI Nexus_MouseDown()
{
    pfn_Nexus_MouseDown();
}

NEXUS_SDK_API VOID WINAPI Nexus_MouseUp()
{
    pfn_Nexus_MouseUp();
}

NEXUS_SDK_API VOID WINAPI Nexus_MouseSendInput(long LastX, long LastY, unsigned short button_flags, USHORT Flags)
{
    pfn_Nexus_MouseSendInput(LastX, LastY, button_flags, Flags);
}


NEXUS_SDK_API VOID WINAPI Nexus_MouseRelativeMovementEx(long LastX, long LastY)
{
    pfn_Nexus_MouseRelativeMovementEx(LastX, LastY);
}

NEXUS_SDK_API VOID WINAPI Nexus_MouseAbsoluteMovementEx(long x, long y)
{
    pfn_Nexus_MouseAbsoluteMovementEx(x, y);
}

NEXUS_SDK_API VOID WINAPI Nexus_MouseDownEx()
{
    pfn_Nexus_MouseDownEx();
}

NEXUS_SDK_API VOID WINAPI Nexus_MouseUpEx()
{
    pfn_Nexus_MouseUpEx();
}

NEXUS_SDK_API NTSTATUS WINAPI Nexus_MouseSendInputEx(long LastX, long LastY, unsigned short button_flags, USHORT Flags)
{
    return pfn_Nexus_MouseSendInputEx(LastX, LastY, button_flags, Flags);
}

NEXUS_SDK_API VOID WINAPI Nexus_KeyPressed(DWORD KeyCode)
{
    pfn_Nexus_KeyPressed(KeyCode);
}

NEXUS_SDK_API VOID WINAPI Nexus_KeyRelease(DWORD KeyCode)
{
    pfn_Nexus_KeyRelease(KeyCode);
}
NEXUS_SDK_API bool WINAPI Nexus_CheckAntiVm()
{
    return pfn_Nexus_CheckAntiVm();

}

NEXUS_SDK_API bool WINAPI Nexus_CheckAntiDebug()
{

    return pfn_Nexus_CheckAntiDebug();
}


