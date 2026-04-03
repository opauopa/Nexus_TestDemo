#include <iostream>
#include <limits>
#include <string>
#include <Windows.h>

#include "NexusDrv/NexusDrvSDK.h"

#define Log(...) do { printf(__VA_ARGS__); printf("\r\n"); } while (0)

const char* GetStatusString(NEXUS_STATUS status)
{
    switch (status) {
    case NEXUS_SUCCESS: return "鎴愬姛";
    case NEXUS_ERROR_NO_ADMIN: return "缂哄皯绠＄悊鍛樻潈闄?;
    case NEXUS_ERROR_ENV_ABNORMAL: return "鐜寮傚父";
    case NEXUS_ERROR_UNSUPPORTED_SYSTEM: return "绯荤粺鐗堟湰涓嶅彈鏀寔";
    case NEXUS_ERROR_DRIVER_INIT_FAILED: return "椹卞姩鍒濆鍖栧け璐?;
    case NEXUS_ERROR_PDB_LOAD: return "PDB 鍔犺浇澶辫触";
    case NEXUS_ERROR_COMM_FAILED: return "閫氫俊澶辫触";
    case NEXUS_ERROR_MAP_FAILED: return "鏄犲皠澶辫触";
    case NEXUS_ERROR_LOAD_FAILED: return "鍔犺浇澶辫触";
    case NEXUS_ERROR_UNLOAD_FAILED: return "鍗歌浇澶辫触";
    case NEXUS_ERROR_INVALID_PARAMETER: return "鍙傛暟鏃犳晥";
    default: return "鏈煡鐘舵€?;
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
    std::cout << "    " << action << "锛?
              << (NT_SUCCESS(status) ? "鎴愬姛" : "澶辫触")
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
    Log("鐩爣杩涚▼ PID锛?lu", pid);
    Log("========================================");

    Log("\n[姝ラ 1] 鍒濆鍖栬鍐欒繘绋?..");
    status = Nexus_InitRWProcess(pid, FALSE);
    if (!NT_SUCCESS(status)) {
        Log("  [!] 鍒濆鍖栬鍐欒繘绋嬪け璐ワ細0x%08X", status);
        return false;
    }
    Log("  [+] 璇诲啓杩涚▼鍒濆鍖栨垚鍔?);

    Log("\n[姝ラ 2] 鑾峰彇杩涚▼ CR3...");
    const ULONG64 cr3 = Nexus_GetProcessCr3(pid, FALSE);
    const ULONG64 cr3Decrypted = Nexus_GetProcessCr3(pid, TRUE);
    Log("  CR3锛堝姞瀵嗭級锛?x%016llX", cr3);
    Log("  CR3锛堣В瀵嗭級锛?x%016llX", cr3Decrypted);

    Log("\n[姝ラ 3] 璇诲彇鐩爣涓绘ā鍧楀ご...");
    status = Nexus_GetProcessModuleInfo_Attach(pid, &mainModuleBase);
    if (!NT_SUCCESS(status)) {
        Log("  [!] 鑾峰彇涓绘ā鍧楀熀鍧€澶辫触锛?x%08X", status);
        return false;
    }

    status = Nexus_ReadProcessPhysicalMemory(pid, mainModuleBase, moduleHeader, sizeof(moduleHeader));
    if (!NT_SUCCESS(status)) {
        Log("  [!] 璇诲彇涓绘ā鍧楀ご澶辫触锛?x%08X", status);
        return false;
    }

    Log("  涓绘ā鍧楀熀鍧€锛?x%016llX", mainModuleBase);
    printf("  妯″潡澶村瓧鑺傦細");
    PrintHexBuffer(moduleHeader, sizeof(moduleHeader));

    Log("\n[姝ラ 4] 鏌ヨ ntdll.dll 淇℃伅...");
    status = Nexus_GetProcessModuleInfo_NoAttach(
        pid,
        (PWCHAR)L"ntdll.dll",
        &ntdllBase,
        &ntdllSize
    );
    if (!NT_SUCCESS(status)) {
        Log("  [!] 鑾峰彇 ntdll.dll 淇℃伅澶辫触锛?x%08X", status);
        return false;
    }

    Log("  ntdll.dll 鍩哄潃锛?x%016llX", ntdllBase);
    Log("  ntdll.dll 澶у皬锛?x%llX锛?llu 瀛楄妭锛?, ntdllSize, static_cast<unsigned long long>(ntdllSize));

    Log("\n[姝ラ 5] 娴嬭瘯鐗╃悊璇诲彇锛?000000 娆★級...");
    {
        const int kBenchCount = 1000000;
        ULONGLONG t0 = GetTickCount64();
        for (int i = 0; i < kBenchCount; ++i) {
            Nexus_ReadProcessPhysicalMemory(pid, mainModuleBase, (PVOID)&readValue, sizeof(readValue));
        }
        ULONGLONG t1 = GetTickCount64();
        Log("  [*] 鐗╃悊璇诲彇 %d 娆¤€楁椂锛?llu ms", kBenchCount, t1 - t0);
    }

    Log("\n[姝ラ 6] 娴嬭瘯 invlpg 璇诲彇锛?000000 娆★級...");
    {
        const int kBenchCount = 1000000;
        ULONGLONG t0 = GetTickCount64();
        for (int i = 0; i < kBenchCount; ++i) {
            Nexus_InvlpgReadProcessPhysicalMemory(pid, mainModuleBase, (PVOID)&invlpgReadValue, sizeof(invlpgReadValue));
        }
        ULONGLONG t1 = GetTickCount64();
        Log("  [*] invlpg 璇诲彇 %d 娆¤€楁椂锛?llu ms", kBenchCount, t1 - t0);
    }

    Log("\n========================================");
    Log("璇诲啓娴嬭瘯宸插畬鎴愶紝缁撴灉鎴愬姛");
    Log("========================================");
    return true;
}

void Demo_InputSimulation()
{
    std::cout << "\n========================================" << std::endl;
    std::cout << "  榧犳爣閿洏杈撳叆婕旂ず" << std::endl;
    std::cout << "========================================" << std::endl;

    std::cout << "[*] 鏈紨绀哄皢妯℃嫙榧犳爣鍜岄敭鐩樿緭鍏ャ€? << std::endl;
    std::cout << "[*] 鎸夊洖杞﹀紑濮嬶紝浣犳湁 3 绉掓椂闂村垏鎹㈠埌鐩爣绐楀彛...";
    std::cin.get();

    std::cout << "[*] 3 绉掑悗寮€濮?.." << std::endl;
    Sleep(3000);

    std::cout << "[*] Mouse relative movement test: 30 times, each move (50, 50)..." << std::endl;
    for (int i = 0; i < 30; ++i)
    {
        Nexus_MouseRelativeMovement(50, 50);
        Sleep(500);
    }

    std::cout << "[*] 榧犳爣绉诲姩鍒扮粷瀵瑰潗鏍?(500, 500)..." << std::endl;
    Nexus_MouseAbsoluteMovement(500, 500);
    Sleep(500);

    std::cout << "[*] 妯℃嫙榧犳爣鍗曞嚮..." << std::endl;
    Nexus_MouseDown();
    Sleep(50);
    Nexus_MouseUp();
    Sleep(500);

    std::cout << "[*] 妯℃嫙閿洏杈撳叆锛堢┖鏍奸敭锛?.." << std::endl;
    Nexus_KeyPressed(VK_SPACE);
    Sleep(50);
    Nexus_KeyRelease(VK_SPACE);
    Sleep(500);

    std::cout << "[*] 妯℃嫙閿洏杈撳叆锛圓 閿級..." << std::endl;
    Nexus_KeyPressed('A');
    Sleep(50);
    Nexus_KeyRelease('A');
    Sleep(500);

    std::cout << "[+] 杈撳叆妯℃嫙宸插畬鎴? << std::endl;

    std::cout << "[*] 娴嬭瘯 MouseSendInputEx锛堢Щ鍔?+ 宸﹂敭鍗曞嚮锛?.." << std::endl;
    NTSTATUS st = Nexus_MouseSendInputEx(50, 50, 0, MOUSE_MOVE_RELATIVE);
    PrintNtStatusResult("绉诲姩", st);
    Sleep(100);
    st = Nexus_MouseSendInputEx(0, 0, MOUSE_LEFT_BUTTON_DOWN, MOUSE_MOVE_RELATIVE);
    Sleep(50);
    st = Nexus_MouseSendInputEx(0, 0, MOUSE_LEFT_BUTTON_UP, MOUSE_MOVE_RELATIVE);
    PrintNtStatusResult("鍗曞嚮", st);

    std::cout << "[+] MouseSendInputEx 娴嬭瘯宸插畬鎴? << std::endl;
}

void Demo_MouseExComparison()
{
    std::cout << "\n========================================" << std::endl;
    std::cout << "  MouseEx 鎵╁睍鐗堝姣旀祴璇? << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "[*] 璇峰湪妯″潡鍒濆鍖栧拰 Nexus_InitDrv 閮芥垚鍔熷悗鍐嶈繍琛屾娴嬭瘯銆? << std::endl;
    std::cout << "[*] 鏅€氶紶鏍囧嚱鏁颁笉杩斿洖鐘舵€佺爜銆? << std::endl;
    std::cout << "[*] Nexus_MouseSendInputEx 鏄墿灞曠増锛屼細杩斿洖 NTSTATUS锛屾洿閫傚悎鍋氭祴璇曞垽鏂€? << std::endl;
    std::cout << "[*] 鎸夊洖杞﹀紑濮嬶紝闅忓悗鏈?3 绉掓椂闂村垏鎹㈠埌鐩爣绐楀彛...";
    std::cin.get();

    Sleep(3000);

    std::cout << "[1] Standard relative movement test: 30 times, each move (50, 50)" << std::endl;
    for (int i = 0; i < 30; ++i)
    {
        Nexus_MouseSendInput(50, 50, 0, MOUSE_MOVE_RELATIVE);
        Sleep(500);
    }

    std::cout << "[2] Ex relative movement test: 30 times, each move (50, 50)" << std::endl;
    NTSTATUS status = STATUS_SUCCESS;
    for (int i = 0; i < 30; ++i)
    {
        status = Nexus_MouseSendInputEx(50, 50, 0, MOUSE_MOVE_RELATIVE);
        PrintNtStatusResult("relative movement", status);
        Sleep(500);
    }

    std::cout << "[3] 鏅€氱増鐐瑰嚮锛歂exus_MouseDown / Nexus_MouseUp" << std::endl;
    Nexus_MouseDown();
    Sleep(50);
    Nexus_MouseUp();
    Sleep(300);

    std::cout << "[4] 鎵╁睍鐗堝皝瑁咃細Nexus_MouseDownEx / Nexus_MouseUpEx" << std::endl;
    Nexus_MouseDownEx();
    Sleep(50);
    Nexus_MouseUpEx();
    Sleep(300);

    std::cout << "[5] 鎵╁睍鐗堟樉寮忕姸鎬佹娴嬬偣鍑? << std::endl;
    status = Nexus_MouseSendInputEx(0, 0, MOUSE_LEFT_BUTTON_DOWN, MOUSE_MOVE_RELATIVE);
    PrintNtStatusResult("宸﹂敭鎸変笅", status);
    Sleep(50);
    status = Nexus_MouseSendInputEx(0, 0, MOUSE_LEFT_BUTTON_UP, MOUSE_MOVE_RELATIVE);
    PrintNtStatusResult("宸﹂敭鎶捣", status);

    std::cout << "[+] MouseEx 鎵╁睍鐗堝姣旀祴璇曞畬鎴愩€? << std::endl;
}

void Demo_CommStressTest()
{
    std::cout << "\n========================================" << std::endl;
    std::cout << "  閫氫俊鍘嬪姏娴嬭瘯" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "[*] 姣忔璋冪敤 Nexus_TestComm()锛岄棿闅?500 姣锛屾寜 Ctrl+C 缁堟" << std::endl;

    ULONGLONG iteration = 0;
    while (true) {
        ++iteration;
        ULONGLONG t0 = GetTickCount64();
        const bool ok = Nexus_TestComm();
        const ULONGLONG elapsed = GetTickCount64() - t0;
        std::cout << "[#" << iteration << "] 閫氫俊娴嬭瘯锛?
                  << (ok ? "鎴愬姛" : "澶辫触")
                  << "  鑰楁椂锛? << elapsed << " ms" << std::endl;
        Sleep(500);
    }
}

void WaitForExit()
{
    std::cout << "\n鎸夊洖杞﹂€€鍑?..";
    std::cin.get();
}

int main()
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    std::cout << "========================================" << std::endl;
    std::cout << "  Nexus 椹卞姩 SDK - 娴嬭瘯婕旂ず" << std::endl;
    std::cout << "========================================" << std::endl;

    std::cout << "\n[姝ラ 1] 浠庡唴瀛樺垵濮嬪寲妯″潡..." << std::endl;
    if (!Nexus_InitializeModuleWithMemory()) {
        std::cout << "[!] 浠庡唴瀛樺垵濮嬪寲妯″潡澶辫触" << std::endl;
        WaitForExit();
        return 1;
    }
    std::cout << "[+] 妯″潡鍒濆鍖栨垚鍔? << std::endl;

    std::cout << "\n[姝ラ 2] 鍒濆鍖栭┍鍔?.." << std::endl;
    const std::string card = "Test1a9a6e27d79ce";
    std::cout << "[*] 鍗″瘑锛? << card << std::endl;

    const NEXUS_STATUS status = Nexus_InitDrv(card.c_str());
    std::cout << "[*] 鍒濆鍖栫粨鏋滐細" << GetStatusString(status)
              << " (0x" << std::hex << status << std::dec << ")" << std::endl;
    if (status != NEXUS_SUCCESS) {
        std::cout << "[!] 椹卞姩鍒濆鍖栧け璐? << std::endl;
        WaitForExit();
        return 1;
    }
    std::cout << "[+] 椹卞姩鍒濆鍖栨垚鍔? << std::endl;

    std::cout << "\n[姝ラ 3] 娴嬭瘯閫氫俊..." << std::endl;
    if (!Nexus_TestComm()) {
        std::cout << "[!] 閫氫俊娴嬭瘯澶辫触" << std::endl;
        WaitForExit();
        return 1;
    }
    std::cout << "[+] 閫氫俊娴嬭瘯鎴愬姛" << std::endl;

    std::cout << "\n[姝ラ 4] 璇疯緭鍏ョ敤浜庤鍐欐祴璇曠殑鐩爣 PID锛?;
    ULONG targetPid = 0;
    std::cin >> targetPid;
    if (!std::cin || targetPid == 0) {
        std::cout << "[!] PID 鏃犳晥" << std::endl;
        return 1;
    }

    if (!TestReadWriteByPid(targetPid)) {
        std::cout << "\n[!] 璇诲啓娴嬭瘯澶辫触" << std::endl;
        return 1;
    }

    std::cout << "\n[姝ラ 5] 鏄惁杩愯榧犳爣/閿洏杈撳叆婕旂ず锛?1=鏄? 0=鍚?锛?;
    char runInputDemo = '0';
    std::cin >> runInputDemo;
    std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
    if (runInputDemo == '1') {
        Demo_InputSimulation();
    }

    std::cout << "\n[姝ラ 6] 鏄惁杩愯 MouseEx 鎵╁睍鐗堝姣旀祴璇曪紵(1=鏄? 0=鍚?锛?;
    char runMouseExDemo = '0';
    std::cin >> runMouseExDemo;
    std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
    if (runMouseExDemo == '1') {
        Demo_MouseExComparison();
    }

    std::cout << "\n[姝ラ 7] 鏄惁杩愯閫氫俊鍘嬪姏娴嬭瘯锛?1=鏄? 0=鍚?锛?;
    char runStressTest = '0';
    std::cin >> runStressTest;
    std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
    if (runStressTest == '1') {
        Demo_CommStressTest();
    }

    std::cout << "\n========================================" << std::endl;
    std::cout << "  鍏ㄩ儴娴嬭瘯瀹屾垚" << std::endl;
    std::cout << "========================================" << std::endl;
    WaitForExit();
    return 0;
}
