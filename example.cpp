#include <iostream>
#include <string>
#include <Windows.h>

#include "NexusDrv/NexusDrvSDK.h"

// 杈呭姪鍑芥暟锛氳繑鍥炰腑鏂囩姸鎬佽鏄庛€?const char* GetStatusString(NEXUS_STATUS status)
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

// 绀轰緥 1锛氳鍙栬繘绋嬪唴瀛樸€?void Example_ReadMemory(ULONG pid)
{
    std::cout << "\n=== 绀轰緥锛氳鍙栬繘绋嬪唴瀛?===" << std::endl;

    // 鍒濆鍖栫洰鏍囪繘绋嬬殑璇诲啓鐜銆?    NTSTATUS status = Nexus_InitRWProcess(pid, FALSE);
    if (!NT_SUCCESS(status)) {
        std::cout << "[!] 鍒濆鍖栬鍐欒繘绋嬪け璐ワ細0x" << std::hex << status << std::endl;
        return;
    }

    // 璇诲彇绀轰緥鍦板潃澶勭殑鍐呭瓨鍐呭銆?    BYTE buffer[256] = { 0 };
    ULONG64 address = 0x400000;
    status = Nexus_ReadProcessMemory(pid, address, buffer, sizeof(buffer));

    if (NT_SUCCESS(status)) {
        std::cout << "[+] 鍐呭瓨璇诲彇鎴愬姛" << std::endl;
        std::cout << "[*] 鍓?16 涓瓧鑺傦細";
        for (int i = 0; i < 16; i++) {
            printf("%02X ", buffer[i]);
        }
        std::cout << std::endl;
    }
    else {
        std::cout << "[!] 璇诲彇鍐呭瓨澶辫触锛?x" << std::hex << status << std::endl;
    }
}

// 绀轰緥 2锛氳幏鍙栨ā鍧椾俊鎭€?void Example_GetModuleInfo(ULONG pid)
{
    std::cout << "\n=== 绀轰緥锛氳幏鍙栨ā鍧椾俊鎭?===" << std::endl;

    ULONG64 moduleBase = 0;
    SIZE_T moduleSize = 0;

    NTSTATUS status = Nexus_GetProcessModuleInfo_NoAttach(
        pid,
        L"kernel32.dll",
        &moduleBase,
        &moduleSize
    );

    if (NT_SUCCESS(status)) {
        std::cout << "[+] 妯″潡鏌ユ壘鎴愬姛" << std::endl;
        std::cout << "[*] 鍩哄潃锛?x" << std::hex << moduleBase << std::endl;
        std::cout << "[*] 澶у皬锛?x" << std::hex << moduleSize << std::endl;
    }
    else {
        std::cout << "[!] 鑾峰彇妯″潡淇℃伅澶辫触锛?x" << std::hex << status << std::endl;
    }
}

// 绀轰緥 3锛氱壒寰佺爜鎼滅储銆?void Example_PatternSearch(ULONG pid)
{
    std::cout << "\n=== 绀轰緥锛氱壒寰佺爜鎼滅储 ===" << std::endl;

    // 鍦ㄧ洰鏍囨ā鍧椾腑鎼滅储绀轰緥鐗瑰緛鐮併€?    const char* pattern = "\x48\x89\x5C\x24\x08";
    const char* mask = "xxxxx";
    ULONG64 foundAddr = 0;

    NTSTATUS status = Nexus_PatternSearch(
        pid,
        L"notepad.exe",
        pattern,
        mask,
        ".text",
        &foundAddr
    );

    if (NT_SUCCESS(status)) {
        std::cout << "[+] 鐗瑰緛鐮佹悳绱㈡垚鍔? << std::endl;
        std::cout << "[*] 鍦板潃锛?x" << std::hex << foundAddr << std::endl;
    }
    else {
        std::cout << "[!] 鏈壘鍒扮壒寰佺爜鎴栨悳绱㈠け璐ワ細0x" << std::hex << status << std::endl;
    }
}

// 绀轰緥 4锛氳繙绋嬪唴瀛樺垎閰嶃€?void Example_AllocateMemory(ULONG pid)
{
    std::cout << "\n=== 绀轰緥锛氬唴瀛樺垎閰?===" << std::endl;

    ULONG64 allocAddr = 0;

    // 鍦ㄧ洰鏍囪繘绋嬩腑鐢宠涓€椤靛彲鎵ц鍙鍐欏唴瀛樸€?    NTSTATUS status = Nexus_AllocateMemory(
        pid,
        0x1000,
        PAGE_EXECUTE_READWRITE,
        ALLOC_NORMAL,
        &allocAddr
    );

    if (NT_SUCCESS(status)) {
        std::cout << "[+] 鍐呭瓨鍒嗛厤鎴愬姛" << std::endl;
        std::cout << "[*] 鍦板潃锛?x" << std::hex << allocAddr << std::endl;

        // 鍐欏叆涓€娈电ず渚嬫満鍣ㄧ爜銆?        BYTE data[] = { 0x90, 0x90, 0x90, 0xC3 };
        status = Nexus_WriteProcessMemory(pid, allocAddr, data, sizeof(data));

        if (NT_SUCCESS(status)) {
            std::cout << "[+] 鏁版嵁鍐欏叆鎴愬姛" << std::endl;
        }

        // 閲婃斁姝ゅ墠鐢宠鐨勮繙绋嬪唴瀛樸€?        status = Nexus_FreeMemory(pid, allocAddr, 0x1000);
        if (NT_SUCCESS(status)) {
            std::cout << "[+] 鍐呭瓨閲婃斁鎴愬姛" << std::endl;
        }
    }
    else {
        std::cout << "[!] 鍒嗛厤鍐呭瓨澶辫触锛?x" << std::hex << status << std::endl;
    }
}

// 绀轰緥 5锛氳緭鍏ユā鎷熴€?void Example_InputSimulation()
{
    std::cout << "\n=== 绀轰緥锛氳緭鍏ユā鎷?===" << std::endl;

    std::cout << "[*] Testing mouse relative movement: 30 times, each move (50, 50)..." << std::endl;
    for (int i = 0; i < 30; ++i)
    {
        Nexus_MouseRelativeMovement(50, 50);
        Sleep(500);
    }

    std::cout << "[*] 姝ｅ湪妯℃嫙鎸夐敭..." << std::endl;
    Nexus_KeyPressed(VK_SPACE);
    Sleep(50);
    Nexus_KeyRelease(VK_SPACE);

    std::cout << "[+] 杈撳叆妯℃嫙宸插畬鎴? << std::endl;
}

int main()
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    std::cout << "========================================" << std::endl;
    std::cout << "  Nexus 椹卞姩 SDK - 杩涢樁绀轰緥" << std::endl;
    std::cout << "========================================" << std::endl;

    // 鍏堣繘琛岄€氫俊娴嬭瘯锛岀‘璁ゅ熀纭€璋冪敤閾炬甯搞€?    std::cout << "\n[*] 姝ｅ湪娴嬭瘯閫氫俊..." << std::endl;
    bool commResult = Nexus_TestComm();
    std::cout << "[*] 閫氫俊缁撴灉锛? << (commResult ? "鎴愬姛" : "澶辫触") << std::endl;

    // 璇诲彇鐢ㄦ埛杈撳叆鐨勬巿鏉冪爜骞跺垵濮嬪寲椹卞姩銆?    std::cout << "\n[*] 璇疯緭鍏ユ巿鏉冪爜锛?;
    std::string authCode;
    std::getline(std::cin, authCode);

    if (authCode.empty()) {
        std::cout << "[!] 鏈緭鍏ユ巿鏉冪爜锛岀▼搴忛€€鍑? << std::endl;
        return 1;
    }

    std::cout << "\n[*] 姝ｅ湪鍒濆鍖栭┍鍔?.." << std::endl;
    NEXUS_STATUS status = Nexus_InitDrv(authCode.c_str(), INIT_VIA_NORMAL);
    std::cout << "[*] 鍒濆鍖栫粨鏋滐細" << GetStatusString(status) << std::endl;

    if (status != NEXUS_SUCCESS) {
        std::cout << "[!] 椹卞姩鍒濆鍖栧け璐? << std::endl;
        std::cout << "\n鎸夊洖杞﹂€€鍑?..";
        std::cin.get();
        return 1;
    }

    std::cout << "[+] 椹卞姩鍒濆鍖栨垚鍔? << std::endl;

    // 鍐嶆閫氫俊娴嬭瘯锛岀‘璁ら┍鍔ㄥ垵濮嬪寲鍚庣殑閫氫俊鐘舵€佹甯搞€?    commResult = Nexus_TestComm();
    std::cout << "[*] 鍒濆鍖栧悗鐨勯€氫俊缁撴灉锛? << (commResult ? "鎴愬姛" : "澶辫触") << std::endl;

    // 鑾峰彇鐩爣杩涚▼ PID锛屽喅瀹氭槸鍚︽墽琛岃繘绋嬬浉鍏崇ず渚嬨€?    std::cout << "\n[*] 璇疯緭鍏ョ洰鏍囪繘绋?PID锛堣緭鍏?0 璺宠繃鐩稿叧绀轰緥锛夛細";
    ULONG pid = 0;
    std::cin >> pid;

    if (pid > 0) {
        // 渚濇鎵ц闇€瑕佺洰鏍囪繘绋嬬殑绀轰緥銆?        Example_ReadMemory(pid);
        Example_GetModuleInfo(pid);
        Example_PatternSearch(pid);
        Example_AllocateMemory(pid);
    }

    // 杈撳叆妯℃嫙涓嶄緷璧?PID锛屽崟鐙闂槸鍚﹁繍琛屻€?    std::cout << "\n[*] 鏄惁杩愯杈撳叆妯℃嫙绀轰緥锛燂紙1=鏄紝0=鍚︼級锛?;
    char choice = '0';
    std::cin >> choice;
    if (choice == '1') {
        Example_InputSimulation();
    }

    // 婕旂ず缁撴潫鍚庡嵏杞介┍鍔ㄣ€?    std::cout << "\n[*] 姝ｅ湪鍗歌浇椹卞姩..." << std::endl;
    NTSTATUS ntStatus = Nexus_UnloadDrv();
    if (NT_SUCCESS(ntStatus)) {
        std::cout << "[+] 椹卞姩鍗歌浇鎴愬姛" << std::endl;
    }
    else {
        std::cout << "[!] 椹卞姩鍗歌浇澶辫触锛?x" << std::hex << ntStatus << std::endl;
    }

    std::cout << "\n鎸夊洖杞﹂€€鍑?..";
    std::cin.ignore();
    std::cin.get();

    return 0;
}
