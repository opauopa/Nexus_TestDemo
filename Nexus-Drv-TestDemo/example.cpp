#include <iostream>
#include <string>
#include <Windows.h>

#include "NexusDrv/NexusDrvSDK.h"

const char* GetStatusString(NEXUS_STATUS status)
{
    switch (status) {
    case NEXUS_SUCCESS: return "成功";
    case NEXUS_ERROR_NO_ADMIN: return "缺少管理员权限";
    case NEXUS_ERROR_ENV_ABNORMAL: return "环境异常";
    case NEXUS_ERROR_UNSUPPORTED_SYSTEM: return "系统版本不受支持";
    case NEXUS_ERROR_DRIVER_INIT_FAILED: return "驱动初始化失败";
    case NEXUS_ERROR_PDB_LOAD: return "PDB 加载失败";
    case NEXUS_ERROR_COMM_FAILED: return "通信失败";
    case NEXUS_ERROR_MAP_FAILED: return "映射失败";
    case NEXUS_ERROR_LOAD_FAILED: return "加载失败";
    case NEXUS_ERROR_UNLOAD_FAILED: return "卸载失败";
    case NEXUS_ERROR_INVALID_PARAMETER: return "参数无效";
    default: return "未知状态";
    }
}

// 示例 1: 读取目标进程内存
void Example_ReadMemory(ULONG pid)
{
    std::cout << "\n=== 示例: 读取进程内存 ===" << std::endl;

    NTSTATUS status = Nexus_InitRWProcess(pid, FALSE);
    if (!NT_SUCCESS(status)) {
        std::cout << "[!] 初始化进程读写环境失败: 0x" << std::hex << status << std::endl;
        return;
    }

    BYTE buffer[256] = { 0 };
    ULONG64 address = 0x400000;
    status = Nexus_ReadProcessMemory(pid, address, buffer, sizeof(buffer));

    if (NT_SUCCESS(status)) {
        std::cout << "[+] 内存读取成功" << std::endl;
        std::cout << "[*] 前 16 个字节: ";
        for (int i = 0; i < 16; i++) {
            printf("%02X ", buffer[i]);
        }
        std::cout << std::endl;
    }
    else {
        std::cout << "[!] 读取内存失败: 0x" << std::hex << status << std::endl;
    }
}

// 示例 2: 获取模块信息
void Example_GetModuleInfo(ULONG pid)
{
    std::cout << "\n=== 示例: 获取模块信息 ===" << std::endl;

    ULONG64 moduleBase = 0;
    SIZE_T moduleSize = 0;

    NTSTATUS status = Nexus_GetProcessModuleInfo_NoAttach(
        pid,
        L"kernel32.dll",
        &moduleBase,
        &moduleSize
    );

    if (NT_SUCCESS(status)) {
        std::cout << "[+] 模块查询成功" << std::endl;
        std::cout << "[*] 基址: 0x" << std::hex << moduleBase << std::endl;
        std::cout << "[*] 大小: 0x" << std::hex << moduleSize << std::endl;
    }
    else {
        std::cout << "[!] 获取模块信息失败: 0x" << std::hex << status << std::endl;
    }
}

// 示例 3: 特征码搜索
void Example_PatternSearch(ULONG pid)
{
    std::cout << "\n=== 示例: 特征码搜索 ===" << std::endl;

    const char* pattern = "\x48\x89\x5C\x24\x08";
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
        std::cout << "[+] 特征码搜索成功" << std::endl;
        std::cout << "[*] 地址: 0x" << std::hex << foundAddr << std::endl;
    }
    else {
        std::cout << "[!] 未找到特征码或搜索失败: 0x" << std::hex << status << std::endl;
    }
}

// 示例 4: 远程内存分配
void Example_AllocateMemory(ULONG pid)
{
    std::cout << "\n=== 示例: 远程内存分配 ===" << std::endl;

    ULONG64 allocAddr = 0;

    NTSTATUS status = Nexus_AllocateMemory(
        pid,
        0x1000,
        PAGE_EXECUTE_READWRITE,
        ALLOC_NORMAL,
        &allocAddr
    );

    if (NT_SUCCESS(status)) {
        std::cout << "[+] 内存分配成功" << std::endl;
        std::cout << "[*] 地址: 0x" << std::hex << allocAddr << std::endl;

        BYTE data[] = { 0x90, 0x90, 0x90, 0xC3 };
        status = Nexus_WriteProcessMemory(pid, allocAddr, data, sizeof(data));

        if (NT_SUCCESS(status)) {
            std::cout << "[+] 数据写入成功" << std::endl;
        }

        status = Nexus_FreeMemory(pid, allocAddr, 0x1000);
        if (NT_SUCCESS(status)) {
            std::cout << "[+] 内存释放成功" << std::endl;
        }
    }
    else {
        std::cout << "[!] 内存分配失败: 0x" << std::hex << status << std::endl;
    }
}

// 示例 5: 输入模拟
void Example_InputSimulation()
{
    std::cout << "\n=== 示例: 输入模拟 ===" << std::endl;

    std::cout << "[*] 鼠标相对移动测试: 循环 30 次，每次移动 (50, 50)..." << std::endl;
    for (int i = 0; i < 30; ++i)
    {
        Nexus_MouseRelativeMovement(50, 50);
        Sleep(500);
    }

    std::cout << "[*] 正在模拟空格键..." << std::endl;
    Nexus_KeyPressed(VK_SPACE);
    Sleep(50);
    Nexus_KeyRelease(VK_SPACE);

    std::cout << "[+] 输入模拟已完成" << std::endl;
}

int main()
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    std::cout << "========================================" << std::endl;
    std::cout << "  Nexus 驱动 SDK - 进阶示例" << std::endl;
    std::cout << "========================================" << std::endl;

    std::cout << "\n[*] 正在测试通信..." << std::endl;
    bool commResult = Nexus_TestComm();
    std::cout << "[*] 通信结果: " << (commResult ? "成功" : "失败") << std::endl;

    std::cout << "\n[*] 请输入授权码: ";
    std::string authCode;
    std::getline(std::cin, authCode);

    if (authCode.empty()) {
        std::cout << "[!] 未输入授权码，程序退出" << std::endl;
        return 1;
    }

    std::cout << "\n[*] 正在初始化驱动..." << std::endl;
    NEXUS_STATUS status = Nexus_InitDrv(authCode.c_str(), INIT_VIA_NORMAL);
    std::cout << "[*] 初始化结果: " << GetStatusString(status) << std::endl;

    if (status != NEXUS_SUCCESS) {
        std::cout << "[!] 驱动初始化失败" << std::endl;
        std::cout << "\n按回车退出...";
        std::cin.get();
        return 1;
    }

    std::cout << "[+] 驱动初始化成功" << std::endl;

    commResult = Nexus_TestComm();
    std::cout << "[*] 初始化后的通信结果: " << (commResult ? "成功" : "失败") << std::endl;

    std::cout << "\n[*] 请输入目标 PID(输入 0 跳过进程相关示例): ";
    ULONG pid = 0;
    std::cin >> pid;

    if (pid > 0) {
        Example_ReadMemory(pid);
        Example_GetModuleInfo(pid);
        Example_PatternSearch(pid);
        Example_AllocateMemory(pid);
    }

    std::cout << "\n[*] 是否运行输入模拟示例?(1=是, 0=否): ";
    char choice = '0';
    std::cin >> choice;
    if (choice == '1') {
        Example_InputSimulation();
    }

    std::cout << "\n[*] 正在卸载驱动..." << std::endl;
    NTSTATUS ntStatus = Nexus_UnloadDrv();
    if (NT_SUCCESS(ntStatus)) {
        std::cout << "[+] 驱动卸载成功" << std::endl;
    }
    else {
        std::cout << "[!] 驱动卸载失败: 0x" << std::hex << ntStatus << std::endl;
    }

    std::cout << "\n按回车退出...";
    std::cin.ignore();
    std::cin.get();

    return 0;
}
