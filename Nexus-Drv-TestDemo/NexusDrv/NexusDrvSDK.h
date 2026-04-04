/**
  * ============================================================================
  * 使用许可与免责声明
  * ============================================================================
  *
  * [合法使用] 本框架仅用于合法软件开发和安全研究，禁止用于任何非法用途
  * [责任声明] 使用者需自行承担所有使用风险及法律责任
  * [版权保护] 未经许可禁止修改、逆向、商业化本软件
  * [衍生授权] 基于本框架的衍生作品版权归各自作者所有
  * [最终解释] 使用即代表同意本声明，如有异议请立即停止使用
  *
  * ============================================================================
  */
#pragma once

// Windows系统头文件
#include <Windows.h>
#include <winternl.h>

// 禁用ntstatus.h与Windows.h的宏冲突警告
#pragma warning(push)
#pragma warning(disable: 4005)
#include <ntstatus.h>
#pragma warning(pop)

// ============================================================================
// 类型定义
// ============================================================================

// NEXUS 状态码
enum NEXUS_STATUS : LONG
{
    NEXUS_SUCCESS                           = 0x00000000L,  // 成功
    NEXUS_ERROR_NO_ADMIN                    = 0xE0000001L,  // 无管理员权限
    NEXUS_ERROR_ENV_ABNORMAL                = 0xE0000002L,  // 环境异常
    NEXUS_ERROR_UNSUPPORTED_SYSTEM          = 0xE0000003L,  // 不支持的系统版本
    NEXUS_ERROR_DRIVER_INIT_FAILED          = 0xE0000004L,  // 驱动初始化失败
    NEXUS_ERROR_PDB_LOAD                    = 0xE0000006L,  // PDB下载失败
    NEXUS_ERROR_COMM_FAILED                 = 0xE0000007L,  // 通信失败
    NEXUS_ERROR_MAP_FAILED                  = 0xE0000008L,  // 映射驱动失败
    NEXUS_ERROR_LOAD_FAILED                 = 0xE0000009L,  // 壳驱动加载失败
    NEXUS_ERROR_UNLOAD_FAILED               = 0xE000000AL,  // 壳驱动卸载失败
    NEXUS_ERROR_INVALID_PARAMETER           = 0xE000000BL,  // 无效参数
    NEXUS_ERROR_FORCR_UPDATE                = 0xE000000CL,  // 强制更新
    NEXUS_ERROR_CARD_NOT_EXIST              = 0xE0002733L,  // 卡密不存在
    NEXUS_ERROR_CARD_DISABLED               = 0xE0002734L,  // 卡密被禁用
    NEXUS_ERROR_MISSING_MACHINE_CODE        = 0xE0002735L,  // 缺少机器码参数
    NEXUS_ERROR_MACHINE_CODE_FAILED         = 0xE0002736L,  // 机器码校验失败
    NEXUS_ERROR_CARD_PARENT_DISABLED        = 0xE0002737L,  // 上级权限被关闭
    NEXUS_ERROR_CARD_EXPIRED                = 0xE0002738L,  // 卡密已到期
    NEXUS_ERROR_ALREADY_LOGGED_IN           = 0xE0002739L,  // 已在其他设备登录
    NEXUS_ERROR_IP_NOT_IN_WHITELIST         = 0xE000273AL,  // IP不在绑定列表内
    NEXUS_ERROR_IP_NOT_IN_DEVICE_LIST       = 0xE000273BL,  // IP不在设备绑定列表内
    NEXUS_ERROR_CARD_NOT_EXIST_COMMON       = 0xE000273CL,  // 卡密不存在(通用)
    NEXUS_ERROR_CARD_DISABLED_COMMON        = 0xE000273DL,  // 卡密被禁用(通用)
    NEXUS_ERROR_CARD_NOT_USED               = 0xE000273EL,  // 卡密未使用
    NEXUS_ERROR_MISSING_MACHINE_CODE_COMMON = 0xE0002740L,  // 缺少机器码参数(通用)
    NEXUS_ERROR_MACHINE_CODE_FAILED_COMMON  = 0xE0002741L,  // 机器码校验失败(通用)
    NEXUS_ERROR_TOKEN_VERIFY_FAILED         = 0xE0002742L,  // 用户token校验失败
    NEXUS_ERROR_LOGGED_IN_OTHER_DEVICE      = 0xE0002743L,  // 已在其他设备登录
    NEXUS_ERROR_LOGIN_STATE_INVALID         = 0xE0002744L,  // 登录状态失效
    NEXUS_ERROR_FORCED_OFFLINE              = 0xE0002745L,  // 被强制下线
    NEXUS_ERROR_TOKEN_INVALID               = 0xE0002746L,  // 用户token失效
    NEXUS_ERROR_INSUFFICIENT_POINTS         = 0xE000005AL,  // 积分数量不足
    NEXUS_ERROR_UNKNOWN                     = 0xE000FFFFL,  // 未知异常
};

// 状态码检查宏
#define NEXUS_SUCCESS(status) ((NEXUS_STATUS)(status) == NEXUS_STATUS::NEXUS_SUCCESS)
#define NEXUS_FAILED(status)  ((NEXUS_STATUS)(status) != NEXUS_STATUS::NEXUS_SUCCESS)


#define MOUSE_LEFT_BUTTON_DOWN   0x0001  // Left Button changed to down.
#define MOUSE_LEFT_BUTTON_UP     0x0002  // Left Button changed to up.
#define MOUSE_RIGHT_BUTTON_DOWN  0x0004  // Right Button changed to down.
#define MOUSE_RIGHT_BUTTON_UP    0x0008  // Right Button changed to up.
#define MOUSE_MIDDLE_BUTTON_DOWN 0x0010  // Middle Button changed to down.
#define MOUSE_MIDDLE_BUTTON_UP   0x0020  // Middle Button changed to up.  
// 内存分配类型
enum AllocateType
{
    ALLOC_NORMAL,                   // 默认内核API函数申请内存 - 附加方式 有VAD记录
    ALLOC_BETWEEN_LEGIT_MODULES,    // 在合法模块之间的空隙分配内存 - 无附加方式 无VAD记录
    ALLOC_WITH_HIJACK_NULL_PFN,     // 劫持空的PFN 替换我们自己申请的页表项 - 无附加方式 无VAD记录
    ALLOC_AT_LOW_ADDRESS,           // 映射user地址 手动挂页 - 无附加方式 无VAD记录
    ALLOC_AT_HIGH_ADDRESS,          // 映射kernel地址 手动挂页 - 无附加方式 无VAD记录
};





// ============================================================================
// SDK 导出接口
// ============================================================================

#define NEXUS_SDK_API extern "C" __declspec(dllexport)

#ifdef __cplusplus
extern "C" {
#endif

// ----------------------------------------------------------------------------
// 模块初始化接口
// ----------------------------------------------------------------------------


// 初始化模块（使用 PeLoader 加载 DLL 并获取函数指针）
NEXUS_SDK_API bool WINAPI Nexus_InitializeModule(const char* dllPath);
//从内存加载nexus模块（使用PeLoader加载DLL并获取函数指针）
NEXUS_SDK_API bool WINAPI Nexus_InitializeModuleWithMemory();

// ----------------------------------------------------------------------------
// 驱动管理接口
// ----------------------------------------------------------------------------

/// <summary>
/// 初始化驱动
/// </summary>
/// <param name="code">你的卡密</param>
/// <param name="init_with_pdb">是否以pdb的方式初始化/param>
/// <returns>NEXUS_STATUS 状态码</returns>
NEXUS_SDK_API NEXUS_STATUS WINAPI Nexus_InitDrv(const char* code, bool init_with_pdb =false);

/// <summary>
/// 卸载驱动
/// </summary>
/// <returns>NTSTATUS 状态码</returns>
NEXUS_SDK_API NTSTATUS WINAPI Nexus_UnloadDrv();

/// <summary>
/// 测试驱动通信
/// </summary>
/// <returns>成功返回 true，失败返回 false</returns>
NEXUS_SDK_API bool WINAPI Nexus_TestComm();

// ----------------------------------------------------------------------------
// 内存读写接口
// ----------------------------------------------------------------------------

/// <summary>
/// 初始化读写进程
/// </summary>
/// <param name="pid">进程ID</param>
/// <param name="needFuckCr3">是否需要解密CR3</param>
/// <returns>NTSTATUS 状态码</returns>
NEXUS_SDK_API NTSTATUS WINAPI Nexus_InitRWProcess(ULONG pid, BOOLEAN needFuckCr3);

/// <summary>
/// 获取进程cr3
/// </summary>
/// <param name="pid">进程id</param>
/// <param name="needFuckCr3">是否需要解密cr3</param>
/// <returns>进程的CR3值</returns>
NEXUS_SDK_API ULONG64 WINAPI Nexus_GetProcessCr3(ULONG pid, BOOLEAN needFuckCr3 = FALSE);
/// <summary>
/// 读取进程内存（物理读写方式，无附加）
/// </summary>
/// <param name="pid">进程ID</param>
/// <param name="address">目标虚拟地址</param>
/// <param name="buffer">接收数据的缓冲区</param>
/// <param name="size">读取字节数</param>
/// <returns>NTSTATUS 状态码</returns>
NEXUS_SDK_API NTSTATUS WINAPI Nexus_ReadProcessPhysicalMemory(ULONG pid, ULONG64 address, PVOID buffer, SIZE_T size);

/// <summary>
/// 写入进程内存（物理读写方式，无附加）
/// </summary>
/// <param name="pid">进程ID</param>
/// <param name="address">目标虚拟地址</param>
/// <param name="buffer">要写入的数据缓冲区</param>
/// <param name="size">写入字节数</param>
/// <returns>NTSTATUS 状态码</returns>
NEXUS_SDK_API NTSTATUS WINAPI Nexus_WriteProcessPhysicalMemory(ULONG pid, ULONG64 address, PVOID buffer, SIZE_T size);

/// <summary>
/// 刷页读物理内存（无附加）
/// </summary>
/// <param name="pid">进程ID</param>
/// <param name="address">目标虚拟地址</param>
/// <param name="buffer">接收数据的缓冲区</param>
/// <param name="size">读取字节数</param>
/// <returns>NTSTATUS 状态码</returns>
NEXUS_SDK_API NTSTATUS WINAPI Nexus_InvlpgReadProcessPhysicalMemory(ULONG pid, ULONG64 address, PVOID buffer, SIZE_T size);

/// <summary>
/// 刷页写物理内存（无附加）
/// </summary>
/// <param name="pid">进程ID</param>
/// <param name="address">目标虚拟地址</param>
/// <param name="buffer">要写入的数据缓冲区</param>
/// <param name="size">写入字节数</param>
/// <returns>NTSTATUS 状态码</returns>
NEXUS_SDK_API NTSTATUS WINAPI Nexus_InvlpgWriteProcessPhysicalMemory(ULONG pid, ULONG64 address, PVOID buffer, SIZE_T size);

// ----------------------------------------------------------------------------
// 模块信息接口
// ----------------------------------------------------------------------------

/// <summary>
/// 获取进程模块信息（附加方式）
/// </summary>
/// <param name="pid">进程ID</param>
/// <param name="retModuleAddress">返回的模块基址</param>
/// <returns>NTSTATUS 状态码</returns>
NEXUS_SDK_API NTSTATUS WINAPI Nexus_GetProcessModuleInfo_Attach(ULONG pid, PULONG64 retModuleAddress);

/// <summary>
/// 获取进程模块信息（无附加方式）
/// </summary>
/// <param name="pid">进程ID</param>
/// <param name="moduleName">模块名称</param>
/// <param name="retModuleAddress">返回的模块基址</param>
/// <param name="retModuleSize">返回的模块大小</param>
/// <returns>NTSTATUS 状态码</returns>
NEXUS_SDK_API NTSTATUS WINAPI Nexus_GetProcessModuleInfo_NoAttach(ULONG pid, PWCHAR moduleName, PULONG64 retModuleAddress, SIZE_T* retModuleSize);

/// <summary>
/// 获取内核模块信息
/// </summary>
/// <param name="moduleName">模块名称</param>
/// <param name="retModuleAddress">返回的模块基址</param>
/// <param name="retModuleSize">返回的模块大小</param>
/// <returns>NTSTATUS 状态码</returns>
NEXUS_SDK_API NTSTATUS WINAPI Nexus_GetKernelModuleInfo(PCHAR moduleName, PULONG64 retModuleAddress, SIZE_T* retModuleSize);

// ----------------------------------------------------------------------------
// 特征码搜索接口
// ----------------------------------------------------------------------------

/// <summary>
/// 在进程中进行特征码搜索（返回第一个匹配地址）
/// </summary>
/// <param name="pid">目标进程ID</param>
/// <param name="moduleName">模块名称</param>
/// <param name="pattern">特征码字节序列</param>
/// <param name="mask">特征码掩码（'x'表示匹配，'?'表示通配）</param>
/// <param name="secName">要搜索的节名称</param>
/// <param name="addr">返回找到的第一个匹配地址</param>
/// <returns>NTSTATUS 状态码</returns>
NEXUS_SDK_API NTSTATUS WINAPI Nexus_PatternSearch(ULONG pid, PWCHAR moduleName, const char* pattern, const char* mask, const char* secName, PULONG64 addr);

// ----------------------------------------------------------------------------
// 内存分配接口
// ----------------------------------------------------------------------------

/// <summary>
/// 申请进程虚拟内存
/// </summary>
/// <param name="pid">进程ID</param>
/// <param name="allocSize">申请内存大小（以0x1000字节为基本单位）</param>
/// <param name="protect">页面属性</param>
/// <param name="type">申请方式</param>
/// <param name="allocAddress">返回申请的地址</param>
/// <returns>NTSTATUS 状态码</returns>
NEXUS_SDK_API NTSTATUS WINAPI Nexus_AllocateMemory(ULONG pid, SIZE_T allocSize, ULONG protect, AllocateType type, PULONG64 allocAddress);

/// <summary>
/// 释放进程虚拟内存
/// </summary>
/// <param name="pid">进程ID</param>
/// <param name="free_address">要释放的起始地址</param>
/// <param name="memory_size">释放的内存大小</param>
/// <returns>NTSTATUS 状态码</returns>
NEXUS_SDK_API NTSTATUS WINAPI Nexus_FreeMemory(ULONG pid, ULONG64 free_address, SIZE_T memory_size);

// ----------------------------------------------------------------------------
// 注入接口
// ----------------------------------------------------------------------------

/// <summary>
/// APC注入（附加方式）
/// </summary>
/// <param name="pid">目标进程ID</param>
/// <param name="dll_buffer">DLL的内存缓冲区</param>
/// <param name="dll_size">DLL大小</param>
/// <param name="allocate_type">申请内存的方式</param>
/// <param name="allocAddr">返回的分配地址</param>
/// <returns>NTSTATUS 状态码</returns>
NEXUS_SDK_API NTSTATUS WINAPI Nexus_InjectWithApcAttach(ULONG pid, PCHAR dll_buffer, SIZE_T dll_size, AllocateType allocate_type, PULONG64 allocAddr);

/// <summary>
/// APC注入（无附加方式）
/// </summary>
/// <param name="pid">目标进程ID</param>
/// <param name="dll_buffer">DLL的内存缓冲区</param>
/// <param name="dll_size">DLL大小</param>
/// <param name="allocate_type">申请内存的方式</param>
/// <param name="allocAddr">返回的分配地址</param>
/// <returns>NTSTATUS 状态码</returns>
NEXUS_SDK_API NTSTATUS WINAPI Nexus_InjectWithApcNoAttach(ULONG pid, PCHAR dll_buffer, SIZE_T dll_size, AllocateType allocate_type, PULONG64 allocAddr);

/// <summary>
/// 劫持线程注入（附加方式）
/// </summary>
/// <param name="pid">目标进程ID</param>
/// <param name="dll_buffer">DLL的内存缓冲区</param>
/// <param name="dll_size">DLL大小</param>
/// <param name="allocate_type">申请内存的方式</param>
/// <param name="allocAddr">返回的分配地址</param>
/// <returns>NTSTATUS 状态码</returns>
NEXUS_SDK_API NTSTATUS WINAPI Nexus_InjectWithHijackThreadAttach(ULONG pid, PCHAR dll_buffer, SIZE_T dll_size, AllocateType allocate_type, PULONG64 allocAddr);

/// <summary>
/// 劫持线程注入（无附加方式）
/// </summary>
/// <param name="pid">目标进程ID</param>
/// <param name="dll_buffer">DLL的内存缓冲区</param>
/// <param name="dll_size">DLL大小</param>
/// <param name="allocate_type">申请内存的方式</param>
/// <param name="allocAddr">返回的分配地址</param>
/// <returns>NTSTATUS 状态码</returns>
NEXUS_SDK_API NTSTATUS WINAPI Nexus_InjectWithHijackThreadNoAttach(ULONG pid, PCHAR dll_buffer, SIZE_T dll_size, AllocateType allocate_type, PULONG64 allocAddr);

/// <summary>
/// 内核创建线程注入（附加方式）
/// </summary>
/// <param name="pid">目标进程ID</param>
/// <param name="dll_buffer">DLL的内存缓冲区</param>
/// <param name="dll_size">DLL大小</param>
/// <param name="allocAddr">返回的分配地址</param>
/// <returns>NTSTATUS 状态码</returns>
NEXUS_SDK_API NTSTATUS WINAPI Nexus_InjectWithRemoteThread(ULONG pid, PCHAR dll_buffer, SIZE_T dll_size, PULONG64 allocAddr);

// ----------------------------------------------------------------------------
// 输入模拟接口
// ----------------------------------------------------------------------------

/// <summary>
/// 鼠标相对移动
/// </summary>
/// <param name="LastX">X轴移动量</param>
/// <param name="LastY">Y轴移动量</param>
NEXUS_SDK_API VOID WINAPI Nexus_MouseRelativeMovement(long LastX, long LastY);

/// <summary>
/// 鼠标绝对移动
/// </summary>
/// <param name="x">X坐标</param>
/// <param name="y">Y坐标</param>
NEXUS_SDK_API VOID WINAPI Nexus_MouseAbsoluteMovement(long x, long y);

/// <summary>
/// 鼠标按下
/// </summary>
NEXUS_SDK_API VOID WINAPI Nexus_MouseDown();

/// <summary>
/// 鼠标抬起
/// </summary>
NEXUS_SDK_API VOID WINAPI Nexus_MouseUp();

/// <summary>
/// 鼠标输入（带返回值），可用于检测驱动是否成功处理
/// 鼠标输入基础版本。
/// </summary>
NEXUS_SDK_API VOID WINAPI Nexus_MouseSendInput(long LastX, long LastY, unsigned short button_flags, USHORT Flags = MOUSE_MOVE_RELATIVE);

/// <summary>
/// 键盘按下
/// </summary>
/// <param name="KeyCode">键值码</param>
NEXUS_SDK_API VOID WINAPI Nexus_KeyPressed(DWORD KeyCode);

/// <summary>
/// 键盘抬起
/// </summary>
/// <param name="KeyCode">键值码</param>
NEXUS_SDK_API VOID WINAPI Nexus_KeyRelease(DWORD KeyCode);



/// <summary>
/// 鼠标移动EX操作
/// 鼠标输入扩展版本，返回 NTSTATUS 便于测试判断。
/// </summary>
/// <param name="LastX"></param>
/// <param name="LastY"></param>
/// <param name="button_flags"></param>
/// <param name="Flags"></param>
/// <returns></returns>
NEXUS_SDK_API NTSTATUS WINAPI Nexus_MouseSendInputEx(long LastX, long LastY, unsigned short button_flags, USHORT Flags = MOUSE_MOVE_RELATIVE);

/// <summary>
/// 鼠标相对移动
/// </summary>
/// <param name="LastX"></param>
/// <param name="LastY"></param>
/// <returns></returns>
NEXUS_SDK_API VOID WINAPI Nexus_MouseRelativeMovementEx(long LastX, long LastY);

/// <summary>
/// 鼠标绝对移动
/// </summary>
/// <param name="x">X坐标</param>
/// <param name="y">Y坐标</param>
NEXUS_SDK_API VOID WINAPI Nexus_MouseAbsoluteMovementEx(long x, long y);

/// <summary>
/// 鼠标按下
/// </summary>
NEXUS_SDK_API VOID WINAPI Nexus_MouseDownEx();

/// <summary>
/// 鼠标抬起
/// </summary>
NEXUS_SDK_API VOID WINAPI Nexus_MouseUpEx();



/// <summary>
/// 检测是否为虚拟机 或者沙箱环境
/// </summary>
/// <returns></returns>
NEXUS_SDK_API bool WINAPI Nexus_CheckAntiVm();

/// <summary>
/// 检测是否被调试
/// </summary>
/// <returns></returns>
NEXUS_SDK_API bool WINAPI Nexus_CheckAntiDebug();



#ifdef __cplusplus
}
#endif
