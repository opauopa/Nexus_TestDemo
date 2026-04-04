# NexusDrv E Language Module

`NexusDrv_E_Module.e` was adjusted to follow the user's existing E-call style, and now also exposes a `UN_E_CALL`-like alias layer.

Internal loading still uses:

- `LoadLibraryA`
- `GetProcAddress`
- `FreeLibrary`
- `CallWindowProcA`

Resolved Nexus exports:

- `Nexus_InitDrv`
- `Nexus_UnloadDrv`
- `Nexus_TestComm`
- `Nexus_InitRWProcess`
- `Nexus_ReadProcessPhysicalMemory`
- `Nexus_WriteProcessPhysicalMemory`

Public APIs now provide both the earlier ArS-like naming and a UN-style alias layer:

- `驱动_加载`
- `驱动_卸载`
- `驱动_安装驱动`
- `驱动_卸载驱动`
- `驱动_验证`
- `驱动_通信测试`
- `进程_初始化读写`
- `进程_附加`
- `读整数型`
- `写整数型`
- `读长整数`
- `写长整数`
- `读浮点数`
- `写浮点数`
- `读字节集`
- `写字节集`
- `读4字节`
- `写4字节`
- `读8字节`
- `写8字节`
- `内存_读整数型`
- `内存_写整数型`
- `内存_读长整数型`
- `内存_写长整数型`
- `内存_读小数型`
- `内存_写小数型`
- `内存_读字节集`
- `内存_写字节集`
- `内存_读4字节`
- `内存_写4字节`
- `内存_读8字节`
- `内存_写8字节`
- `进程_取CR3`
- `内存_刷页读字节集`
- `内存_刷页写字节集`
- `模块_取主模块地址_附加`
- `模块_取模块地址`
- `模块_取内核模块地址`
- `特征_搜索`
- `内存_申请`
- `内存_释放`
- `注入_APC附加`
- `注入_APC无附加`
- `注入_劫持线程附加`
- `注入_劫持线程无附加`
- `注入_远程线程`
- `输入_鼠标相对移动`
- `输入_鼠标绝对移动`
- `输入_鼠标按下`
- `输入_鼠标抬起`
- `输入_鼠标发送`
- `输入_鼠标发送Ex`
- `输入_鼠标相对移动Ex`
- `输入_鼠标绝对移动Ex`
- `输入_鼠标按下Ex`
- `输入_鼠标抬起Ex`
- `输入_键盘按下`
- `输入_键盘抬起`
- `环境_检测虚拟机`
- `环境_检测调试`

Typical call order:

1. `驱动_安装驱动`
2. `进程_附加`
3. Read/write memory
4. `驱动_卸载驱动`

Important:

- This repository's DLL is `x64`.
- Traditional 32-bit E programs cannot load a 64-bit DLL with `LoadLibraryA`.
- If your E environment only builds 32-bit executables, this module cannot be used directly.
- The wrapper uses `Nexus_ReadProcessPhysicalMemory` and `Nexus_WriteProcessPhysicalMemory`, because those are the exports declared in `NexusDrv/NexusDrvSDK.h` and used by `main.cpp`.
- `读浮点数` and `写浮点数` are implemented as 4-byte float access.
- `驱动_安装驱动` is a convenience wrapper around `驱动_加载 + 驱动_验证 + 驱动_通信测试`.
- `模块_取模块地址` / `特征_搜索` / `内存_申请` / `注入_APC附加` / `注入_APC无附加` / `注入_劫持线程附加` / `注入_劫持线程无附加` rely on the new helper exports added in `NexusDrvSDK.cpp` and require a rebuilt DLL.
- The current checked-in `Nexus-Drv-MapperLib.vmp.dll` does not yet contain those helper exports.

There is also a separate usage sample in `Nexus_UN_E_Call_Demo.e`.

The code was not compiled in this workspace because there is no E-language compiler installed here.
