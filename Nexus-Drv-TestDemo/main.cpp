#include <cstdio>
#include <iostream>
#include <string>
#include <Windows.h>

#include "NexusDrv/NexusDrvSDK.h"

#define Log(...) do { printf(__VA_ARGS__); printf("\r\n"); } while (0)

const char* GetStatusString(NEXUS_STATUS status)
{
    switch (status) {
    case NEXUS_SUCCESS: return "Success";
    case NEXUS_ERROR_NO_ADMIN: return "No admin privilege";
    case NEXUS_ERROR_ENV_ABNORMAL: return "Environment abnormal";
    case NEXUS_ERROR_UNSUPPORTED_SYSTEM: return "Unsupported system";
    case NEXUS_ERROR_DRIVER_INIT_FAILED: return "Driver init failed";
    case NEXUS_ERROR_PDB_LOAD: return "PDB load failed";
    case NEXUS_ERROR_COMM_FAILED: return "Communication failed";
    case NEXUS_ERROR_MAP_FAILED: return "Map failed";
    case NEXUS_ERROR_LOAD_FAILED: return "Load failed";
    case NEXUS_ERROR_UNLOAD_FAILED: return "Unload failed";
    case NEXUS_ERROR_INVALID_PARAMETER: return "Invalid parameter";
    default: return "Unknown status";
    }
}

void PrintHexBuffer(const BYTE* buffer, SIZE_T size)
{
    for (SIZE_T i = 0; i < size; ++i) {
        printf("%02X ", buffer[i]);
    }
    printf("\r\n");
}

void PrintNtStatusResult(const char* action, NTSTATUS status)
{
    std::cout << "    " << action << ": "
              << (NT_SUCCESS(status) ? "Success" : "Failed")
              << " (0x" << std::hex << status << std::dec << ")" << std::endl;
}

bool TestReadWriteByPid(ULONG pid)
{
    NTSTATUS status = STATUS_SUCCESS;
    ULONG64 mainModuleBase = 0;
    ULONG64 ntdllBase = 0;
    SIZE_T ntdllSize = 0;
    BYTE moduleHeader[16] = {};
    ULONG64 readValue = 0;
    ULONG64 invlpgReadValue = 0;

    Log("========================================");
    Log("Target PID: %lu", pid);
    Log("========================================");

    Log("\n[Step 1] Init process RW context...");
    status = Nexus_InitRWProcess(pid, FALSE);
    if (!NT_SUCCESS(status)) {
        Log("  [!] Nexus_InitRWProcess failed: 0x%08X", status);
        return false;
    }
    Log("  [+] Process RW context initialized");

    Log("\n[Step 2] Query process CR3...");
    const ULONG64 cr3 = Nexus_GetProcessCr3(pid, FALSE);
    const ULONG64 cr3Decrypted = Nexus_GetProcessCr3(pid, TRUE);
    Log("  CR3(encrypted): 0x%016llX", cr3);
    Log("  CR3(decrypted): 0x%016llX", cr3Decrypted);

    Log("\n[Step 3] Read main module header...");
    status = Nexus_GetProcessModuleInfo_Attach(pid, &mainModuleBase);
    if (!NT_SUCCESS(status)) {
        Log("  [!] Nexus_GetProcessModuleInfo_Attach failed: 0x%08X", status);
        return false;
    }

    status = Nexus_ReadProcessPhysicalMemory(pid, mainModuleBase, moduleHeader, sizeof(moduleHeader));
    if (!NT_SUCCESS(status)) {
        Log("  [!] Nexus_ReadProcessPhysicalMemory failed: 0x%08X", status);
        return false;
    }

    Log("  Main module base: 0x%016llX", mainModuleBase);
    printf("  Header bytes: ");
    PrintHexBuffer(moduleHeader, sizeof(moduleHeader));

    Log("\n[Step 4] Query ntdll.dll info...");
    status = Nexus_GetProcessModuleInfo_NoAttach(
        pid,
        (PWCHAR)L"ntdll.dll",
        &ntdllBase,
        &ntdllSize
    );
    if (!NT_SUCCESS(status)) {
        Log("  [!] Nexus_GetProcessModuleInfo_NoAttach failed: 0x%08X", status);
        return false;
    }

    Log("  ntdll.dll base: 0x%016llX", ntdllBase);
    Log("  ntdll.dll size: 0x%llX (%llu bytes)", ntdllSize, static_cast<unsigned long long>(ntdllSize));

    Log("\n[Step 5] Benchmark physical read x1,000,000...");
    {
        const int kBenchCount = 1000000;
        ULONGLONG t0 = GetTickCount64();
        for (int i = 0; i < kBenchCount; ++i) {
            Nexus_ReadProcessPhysicalMemory(pid, mainModuleBase, (PVOID)&readValue, sizeof(readValue));
        }
        ULONGLONG t1 = GetTickCount64();
        Log("  [*] Physical read elapsed: %llu ms", t1 - t0);
    }

    Log("\n[Step 6] Benchmark invlpg read x1,000,000...");
    {
        const int kBenchCount = 1000000;
        ULONGLONG t0 = GetTickCount64();
        for (int i = 0; i < kBenchCount; ++i) {
            Nexus_InvlpgReadProcessPhysicalMemory(pid, mainModuleBase, (PVOID)&invlpgReadValue, sizeof(invlpgReadValue));
        }
        ULONGLONG t1 = GetTickCount64();
        Log("  [*] invlpg read elapsed: %llu ms", t1 - t0);
    }

    Log("\n========================================");
    Log("Read/write test completed successfully");
    Log("========================================");
    return true;
}

void Demo_InputSimulation()
{
    std::cout << "\n========================================" << std::endl;
    std::cout << "  Input Simulation Demo" << std::endl;
    std::cout << "========================================" << std::endl;

    std::cout << "[*] This demo will simulate mouse and keyboard input." << std::endl;
    std::cout << "[*] Press Enter to start. You have 3 seconds to switch to the target window...";
    std::cin.get();

    std::cout << "[*] Starting in 3 seconds..." << std::endl;
    Sleep(1000);

    std::cout << "[*] Mouse relative movement test: 30 times, move (50, 50)." << std::endl;
    for (int i = 0; i < 30; ++i)
    {
        Nexus_MouseRelativeMovement(50, 50);
        Sleep(200);
    }

    std::cout << "[*] Move mouse to absolute position (500, 500)." << std::endl;
    Nexus_MouseAbsoluteMovement(500, 500);
    Sleep(500);

    std::cout << "[*] Simulate mouse click." << std::endl;
    Nexus_MouseDown();
    Sleep(50);
    Nexus_MouseUp();
    Sleep(500);

    std::cout << "[*] Simulate keyboard input: Space." << std::endl;
    Nexus_KeyPressed(VK_SPACE);
    Sleep(50);
    Nexus_KeyRelease(VK_SPACE);
    Sleep(500);

    std::cout << "[*] Simulate keyboard input: A." << std::endl;
    Nexus_KeyPressed('A');
    Sleep(50);
    Nexus_KeyRelease('A');
    Sleep(500);

    std::cout << "[+] Input simulation completed" << std::endl;

    std::cout << "[*] Test MouseSendInputEx (move + click)..." << std::endl;
    NTSTATUS st = Nexus_MouseSendInputEx(50, 50, 0, MOUSE_MOVE_RELATIVE);
    PrintNtStatusResult("Move", st);
    Sleep(100);
    st = Nexus_MouseSendInputEx(0, 0, MOUSE_LEFT_BUTTON_DOWN, MOUSE_MOVE_RELATIVE);
    Sleep(50);
    st = Nexus_MouseSendInputEx(0, 0, MOUSE_LEFT_BUTTON_UP, MOUSE_MOVE_RELATIVE);
    PrintNtStatusResult("Click", st);

    std::cout << "[+] MouseSendInputEx test completed" << std::endl;
}

void Demo_MouseExComparison()
{
    std::cout << "\n========================================" << std::endl;
    std::cout << "  MouseEx Comparison Demo" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "[*] Run this only after module init and Nexus_InitDrv succeed." << std::endl;
    std::cout << "[*] Normal mouse helpers do not return NTSTATUS." << std::endl;
    std::cout << "[*] Nexus_MouseSendInputEx is better for test verification." << std::endl;
    std::cout << "[*] Press Enter to start. You have 3 seconds to switch to the target window...";
    std::cin.get();

    Sleep(1000);

  

    std::cout << "[2] Extended relative move test: 30 times, move (50, 50)" << std::endl;
    NTSTATUS status = STATUS_SUCCESS;
    for (int i = 0; i < 30; ++i)
    {
      Nexus_MouseRelativeMovementEx(50, 50);
       
        Sleep(200);
    }

    std::cout << "[3] Normal click: Nexus_MouseDown / Nexus_MouseUp" << std::endl;
    Nexus_MouseDown();
    Sleep(50);
    Nexus_MouseUp();
    Sleep(300);

    std::cout << "[4] Extended click: Nexus_MouseDownEx / Nexus_MouseUpEx" << std::endl;
    Nexus_MouseDownEx();
    Sleep(50);
    Nexus_MouseUpEx();
    Sleep(300);

    std::cout << "[5] Extended explicit status test" << std::endl;
    status = Nexus_MouseSendInputEx(0, 0, MOUSE_LEFT_BUTTON_DOWN, MOUSE_MOVE_RELATIVE);
    PrintNtStatusResult("LeftDown", status);
    Sleep(50);
    status = Nexus_MouseSendInputEx(0, 0, MOUSE_LEFT_BUTTON_UP, MOUSE_MOVE_RELATIVE);
    PrintNtStatusResult("LeftUp", status);

    std::cout << "[+] MouseEx comparison completed" << std::endl;
}

void Demo_CommStressTest()
{
    std::cout << "\n========================================" << std::endl;
    std::cout << "  Communication Stress Test" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "[*] Call Nexus_TestComm() every 500 ms. Stop with Ctrl+C." << std::endl;

    ULONGLONG iteration = 0;
    while (true) {
        ++iteration;
        ULONGLONG t0 = GetTickCount64();
        const bool ok = Nexus_TestComm();
        const ULONGLONG elapsed = GetTickCount64() - t0;
        std::cout << "[#" << iteration << "] Comm test: "
                  << (ok ? "Success" : "Failed")
                  << "  Elapsed: " << elapsed << " ms" << std::endl;
        Sleep(500);
    }
}

void WaitForEnter()
{
    std::cout << "\nPress Enter to continue...";
    std::string line;
    std::getline(std::cin, line);
}

bool ReadUnsignedLong(const char* prompt, ULONG& value)
{
    std::string line;
    std::cout << prompt;
    std::getline(std::cin, line);

    if (line.empty()) {
        std::cout << "[!] Input cannot be empty" << std::endl;
        return false;
    }

    try {
        const unsigned long parsed = std::stoul(line);
        if (parsed == 0) {
            std::cout << "[!] Input must be greater than 0" << std::endl;
            return false;
        }

        value = static_cast<ULONG>(parsed);
        return true;
    }
    catch (...) {
        std::cout << "[!] Invalid number format" << std::endl;
        return false;
    }
}

bool FileExists(const std::string& path)
{
    const DWORD attributes = GetFileAttributesA(path.c_str());
    return attributes != INVALID_FILE_ATTRIBUTES && !(attributes & FILE_ATTRIBUTE_DIRECTORY);
}

std::string GetExecutableDirectory()
{
    char modulePath[MAX_PATH] = {};
    const DWORD length = GetModuleFileNameA(nullptr, modulePath, MAX_PATH);
    if (length == 0 || length >= MAX_PATH) {
        return ".";
    }

    const std::string fullPath(modulePath, length);
    const size_t separator = fullPath.find_last_of("\\/");
    if (separator == std::string::npos) {
        return ".";
    }

    return fullPath.substr(0, separator);
}

bool TryInitializeModuleFromFiles(std::string& loadedPath)
{
    
    std::cout << "[*] Trying module file: " << loadedPath << std::endl;
    if (Nexus_InitializeModule(loadedPath.c_str())) {
     
        return true;
    }

 
    return false;
}

void PrintMenu(bool moduleInitialized, bool driverInitialized)
{
    std::cout << "\n========================================" << std::endl;
    std::cout << "  Nexus Test Menu" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "[State] Module init: " << (moduleInitialized ? "Done" : "Not done") << std::endl;
    std::cout << "[State] Driver init: " << (driverInitialized ? "Done" : "Not done") << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "3. Test communication" << std::endl;
    std::cout << "4. Run RW test by PID" << std::endl;
    std::cout << "5. Mouse/keyboard input demo" << std::endl;
    std::cout << "6. MouseEx comparison demo" << std::endl;
    std::cout << "7. Communication stress test" << std::endl;
    std::cout << "8. Unload driver" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
}

bool EnsureDriverReady(bool moduleInitialized, bool driverInitialized)
{
    if (!moduleInitialized) {
        std::cout << "[!] Please initialize the module first" << std::endl;
        return false;
    }

    if (!driverInitialized) {
        std::cout << "[!] Please initialize the driver first" << std::endl;
        return false;
    }

    return true;
}

int main()
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    std::cout << "========================================" << std::endl;
    std::cout << "  Nexus Driver SDK - Test Demo" << std::endl;
    std::cout << "========================================" << std::endl;

    const std::string defaultCard = "Test1a9a6e27d79ce";
    std::string currentCard = defaultCard;
    bool moduleInitialized = false;
    bool driverInitialized = false;

#ifdef _DEBUG
    std::string loadedModulePath="Nexus-Drv-MapperLib.dll";
    std::cout << "\n[*] Debug build: auto initializing module from file..." << std::endl;
    moduleInitialized = TryInitializeModuleFromFiles(loadedModulePath);
    if (moduleInitialized) {
        std::cout << "[+] Module file init success: " << loadedModulePath << std::endl;
    }
    else {
        std::cout << "[!] Module file init failed. Expected Nexus_Drv_MapperLib.dll in the executable directory." << std::endl;
    }
#else
    std::cout << "\n[*] Release build: auto initializing module from memory..." << std::endl;
    moduleInitialized = Nexus_InitializeModuleWithMemory();
    std::cout << (moduleInitialized ? "[+] Module init success" : "[!] Module init failed") << std::endl;
#endif

    if (moduleInitialized) {
        std::cout << "\n[*] Auto initializing driver..." << std::endl;
        std::cout << "[*] Card code: " << currentCard << std::endl;
        const NEXUS_STATUS status = Nexus_InitDrv(currentCard.c_str());
        std::cout << "[*] Init result: " << GetStatusString(status)
                  << " (0x" << std::hex << status << std::dec << ")" << std::endl;

        driverInitialized = (status == NEXUS_SUCCESS);
        if (!driverInitialized) {
            std::cout << "[!] Driver init failed" << std::endl;
        }
    }

    while (true) {
        PrintMenu(moduleInitialized, driverInitialized);

        std::string choice;
        std::cout << "Select option: ";
        std::getline(std::cin, choice);

       
        if (choice == "3") {
            if (!EnsureDriverReady(moduleInitialized, driverInitialized)) {
                WaitForEnter();
                continue;
            }

            std::cout << "\n[*] Testing communication..." << std::endl;
            std::cout << (Nexus_TestComm() ? "[+] Communication success" : "[!] Communication failed") << std::endl;
            WaitForEnter();
            continue;
        }

        if (choice == "4") {
            if (!EnsureDriverReady(moduleInitialized, driverInitialized)) {
                WaitForEnter();
                continue;
            }

            ULONG targetPid = 0;
            if (ReadUnsignedLong("Input target PID: ", targetPid)) {
                if (!TestReadWriteByPid(targetPid)) {
                    std::cout << "\n[!] Read/write test failed" << std::endl;
                }
            }

            WaitForEnter();
            continue;
        }

        if (choice == "5") {
            if (!EnsureDriverReady(moduleInitialized, driverInitialized)) {
                WaitForEnter();
                continue;
            }

            Demo_InputSimulation();
            WaitForEnter();
            continue;
        }

        if (choice == "6") {
            if (!EnsureDriverReady(moduleInitialized, driverInitialized)) {
                WaitForEnter();
                continue;
            }

            Demo_MouseExComparison();
            WaitForEnter();
            continue;
        }

        if (choice == "7") {
            if (!EnsureDriverReady(moduleInitialized, driverInitialized)) {
                WaitForEnter();
                continue;
            }

            std::cout << "[*] Stress test will not return to the menu. Stop with Ctrl+C." << std::endl;
            WaitForEnter();
            Demo_CommStressTest();
            continue;
        }

        if (choice == "8") {
            if (!driverInitialized) {
                std::cout << "[*] Driver is not initialized" << std::endl;
            }
            else {
                std::cout << "\n[*] Unloading driver..." << std::endl;
                const NTSTATUS status = Nexus_UnloadDrv();
                if (NT_SUCCESS(status)) {
                    driverInitialized = false;
                    std::cout << "[+] Driver unload success" << std::endl;
                }
                else {
                    std::cout << "[!] Driver unload failed: 0x" << std::hex << status << std::dec << std::endl;
                }
            }

            WaitForEnter();
            continue;
        }

        if (choice == "0") {
            break;
        }

        std::cout << "[!] Invalid option" << std::endl;
        WaitForEnter();
    }

    if (driverInitialized) {
        std::cout << "\n[*] Unloading driver before exit..." << std::endl;
        const NTSTATUS status = Nexus_UnloadDrv();
        if (NT_SUCCESS(status)) {
            std::cout << "[+] Driver unload success" << std::endl;
        }
        else {
            std::cout << "[!] Driver unload failed: 0x" << std::hex << status << std::dec << std::endl;
        }
    }

    return 0;
}
