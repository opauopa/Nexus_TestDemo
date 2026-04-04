.版本 2
.支持库 spec

.程序集 NexusDrv_EModule

.全局变量 g_hNexus, 长整数型
.全局变量 g_fnInitDrv, 长整数型
.全局变量 g_fnUnloadDrv, 长整数型
.全局变量 g_fnTestComm, 长整数型
.全局变量 g_fnInitializeModule, 长整数型
.全局变量 g_fnInitializeModuleWithMemory, 长整数型
.全局变量 g_fnInitRWProcess, 长整数型
.全局变量 g_fnGetProcessCr3, 长整数型
.全局变量 g_fnReadPhysical, 长整数型
.全局变量 g_fnWritePhysical, 长整数型
.全局变量 g_fnInvlpgReadPhysical, 长整数型
.全局变量 g_fnInvlpgWritePhysical, 长整数型
.全局变量 g_fnGetModuleAttach, 长整数型
.全局变量 g_fnGetModuleNoAttachA, 长整数型
.全局变量 g_fnGetKernelModuleInfo, 长整数型
.全局变量 g_fnPatternSearchA, 长整数型
.全局变量 g_fnAllocateMemoryForE, 长整数型
.全局变量 g_fnFreeMemory, 长整数型
.全局变量 g_fnInjectApcAttachForE, 长整数型
.全局变量 g_fnInjectApcNoAttachForE, 长整数型
.全局变量 g_fnInjectHijackAttachForE, 长整数型
.全局变量 g_fnInjectHijackNoAttachForE, 长整数型
.全局变量 g_fnInjectRemoteThread, 长整数型
.全局变量 g_fnMouseRelativeMovement, 长整数型
.全局变量 g_fnMouseAbsoluteMovement, 长整数型
.全局变量 g_fnMouseDown, 长整数型
.全局变量 g_fnMouseUp, 长整数型
.全局变量 g_fnMouseSendInput, 长整数型
.全局变量 g_fnMouseSendInputEx, 长整数型
.全局变量 g_fnMouseRelativeMovementEx, 长整数型
.全局变量 g_fnMouseAbsoluteMovementEx, 长整数型
.全局变量 g_fnMouseDownEx, 长整数型
.全局变量 g_fnMouseUpEx, 长整数型
.全局变量 g_fnKeyPressed, 长整数型
.全局变量 g_fnKeyRelease, 长整数型
.全局变量 g_fnCheckAntiVm, 长整数型
.全局变量 g_fnCheckAntiDebug, 长整数型
.全局变量 g_fnGetLastCallStatus, 长整数型
.全局变量 g_lastNtStatus, 整数型
.全局变量 g_lastInitStatus, 整数型
.全局变量 g_lastSize, 长整数型

.DLL命令 LoadLibraryA, 长整数型, "kernel32.dll", "LoadLibraryA"
    .参数 lpLibFileName, 文本型

.DLL命令 FreeLibrary, 整数型, "kernel32.dll", "FreeLibrary"
    .参数 hLibModule, 长整数型

.DLL命令 GetProcAddress, 长整数型, "kernel32.dll", "GetProcAddress"
    .参数 hModule, 长整数型
    .参数 lpProcName, 文本型

.DLL命令 CallWindowProcA, 长整数型, "user32.dll", "CallWindowProcA"
    .参数 lpPrevWndFunc, 长整数型
    .参数 hWnd, 长整数型
    .参数 Msg, 长整数型
    .参数 wParam, 长整数型
    .参数 lParam, 长整数型

.子程序 _重置函数地址
    g_hNexus ＝ 0
    g_fnInitDrv ＝ 0
    g_fnUnloadDrv ＝ 0
    g_fnTestComm ＝ 0
    g_fnInitializeModule ＝ 0
    g_fnInitializeModuleWithMemory ＝ 0
    g_fnInitRWProcess ＝ 0
    g_fnGetProcessCr3 ＝ 0
    g_fnReadPhysical ＝ 0
    g_fnWritePhysical ＝ 0
    g_fnInvlpgReadPhysical ＝ 0
    g_fnInvlpgWritePhysical ＝ 0
    g_fnGetModuleAttach ＝ 0
    g_fnGetModuleNoAttachA ＝ 0
    g_fnGetKernelModuleInfo ＝ 0
    g_fnPatternSearchA ＝ 0
    g_fnAllocateMemoryForE ＝ 0
    g_fnFreeMemory ＝ 0
    g_fnInjectApcAttachForE ＝ 0
    g_fnInjectApcNoAttachForE ＝ 0
    g_fnInjectHijackAttachForE ＝ 0
    g_fnInjectHijackNoAttachForE ＝ 0
    g_fnInjectRemoteThread ＝ 0
    g_fnMouseRelativeMovement ＝ 0
    g_fnMouseAbsoluteMovement ＝ 0
    g_fnMouseDown ＝ 0
    g_fnMouseUp ＝ 0
    g_fnMouseSendInput ＝ 0
    g_fnMouseSendInputEx ＝ 0
    g_fnMouseRelativeMovementEx ＝ 0
    g_fnMouseAbsoluteMovementEx ＝ 0
    g_fnMouseDownEx ＝ 0
    g_fnMouseUpEx ＝ 0
    g_fnKeyPressed ＝ 0
    g_fnKeyRelease ＝ 0
    g_fnCheckAntiVm ＝ 0
    g_fnCheckAntiDebug ＝ 0
    g_fnGetLastCallStatus ＝ 0
    g_lastNtStatus ＝ 0
    g_lastInitStatus ＝ 0
    g_lastSize ＝ 0

.子程序 _取导出地址, 长整数型
    .参数 导出名, 文本型
    返回 (GetProcAddress (g_hNexus, 导出名))

.子程序 _调用0, 长整数型
    .参数 函数地址, 长整数型
    返回 (CallWindowProcA (函数地址, 0, 0, 0, 0))

.子程序 _调用2, 长整数型
    .参数 函数地址, 长整数型
    .参数 参数1, 长整数型
    .参数 参数2, 长整数型
    返回 (CallWindowProcA (函数地址, 参数1, 参数2, 0, 0))

.子程序 _调用1, 长整数型
    .参数 函数地址, 长整数型
    .参数 参数1, 长整数型
    返回 (_调用2 (函数地址, 参数1, 0))

.子程序 _调用4, 长整数型
    .参数 函数地址, 长整数型
    .参数 参数1, 长整数型
    .参数 参数2, 长整数型
    .参数 参数3, 长整数型
    .参数 参数4, 长整数型
    返回 (CallWindowProcA (函数地址, 参数1, 参数2, 参数3, 参数4))

.子程序 _调用3, 长整数型
    .参数 函数地址, 长整数型
    .参数 参数1, 长整数型
    .参数 参数2, 长整数型
    .参数 参数3, 长整数型
    返回 (_调用4 (函数地址, 参数1, 参数2, 参数3, 0))

.子程序 _文本转Ansi字节集, 字节集
    .参数 文本内容, 文本型
    .局部变量 临时字节集, 字节集

    临时字节集 ＝ 到字节集 (文本内容)
    临时字节集 ＝ 临时字节集 ＋ { 0 }
    返回 (临时字节集)

.子程序 _刷新扩展状态
    .如果真 (g_fnGetLastCallStatus ≠ 0)
        g_lastNtStatus ＝ _调用0 (g_fnGetLastCallStatus)
    .如果真结束

.子程序 NT状态成功, 逻辑型, 公开
    .参数 状态值, 整数型
    返回 (状态值 ≥ 0)

.子程序 取最后NT状态, 整数型, 公开
    返回 (g_lastNtStatus)

.子程序 取最后初始化状态, 整数型, 公开
    返回 (g_lastInitStatus)

.子程序 模块是否已加载, 逻辑型, 公开
    返回 (g_hNexus ≠ 0)

.子程序 驱动_加载, 逻辑型, 公开
    .参数 DLL路径, 文本型

    .如果真 (g_hNexus ≠ 0)
        返回 (真)
    .如果真结束

    g_hNexus ＝ LoadLibraryA (DLL路径)
    .如果真 (g_hNexus ＝ 0)
        返回 (假)
    .如果真结束

    g_fnInitializeModule ＝ _取导出地址 ("Nexus_InitializeModule")
    g_fnInitializeModuleWithMemory ＝ _取导出地址 ("Nexus_InitializeModuleWithMemory")
    g_fnInitDrv ＝ _取导出地址 ("Nexus_InitDrv")
    g_fnUnloadDrv ＝ _取导出地址 ("Nexus_UnloadDrv")
    g_fnTestComm ＝ _取导出地址 ("Nexus_TestComm")
    g_fnInitRWProcess ＝ _取导出地址 ("Nexus_InitRWProcess")
    g_fnGetProcessCr3 ＝ _取导出地址 ("Nexus_GetProcessCr3")
    g_fnReadPhysical ＝ _取导出地址 ("Nexus_ReadProcessPhysicalMemory")
    g_fnWritePhysical ＝ _取导出地址 ("Nexus_WriteProcessPhysicalMemory")
    g_fnInvlpgReadPhysical ＝ _取导出地址 ("Nexus_InvlpgReadProcessPhysicalMemory")
    g_fnInvlpgWritePhysical ＝ _取导出地址 ("Nexus_InvlpgWriteProcessPhysicalMemory")
    g_fnGetModuleAttach ＝ _取导出地址 ("Nexus_GetProcessModuleInfo_Attach")
    g_fnGetModuleNoAttachA ＝ _取导出地址 ("Nexus_GetProcessModuleInfo_NoAttachA")
    g_fnGetKernelModuleInfo ＝ _取导出地址 ("Nexus_GetKernelModuleInfo")
    g_fnPatternSearchA ＝ _取导出地址 ("Nexus_PatternSearchA")
    g_fnAllocateMemoryForE ＝ _取导出地址 ("Nexus_AllocateMemoryForE")
    g_fnFreeMemory ＝ _取导出地址 ("Nexus_FreeMemory")
    g_fnInjectApcAttachForE ＝ _取导出地址 ("Nexus_InjectWithApcAttachForE")
    g_fnInjectApcNoAttachForE ＝ _取导出地址 ("Nexus_InjectWithApcNoAttachForE")
    g_fnInjectHijackAttachForE ＝ _取导出地址 ("Nexus_InjectWithHijackThreadAttachForE")
    g_fnInjectHijackNoAttachForE ＝ _取导出地址 ("Nexus_InjectWithHijackThreadNoAttachForE")
    g_fnInjectRemoteThread ＝ _取导出地址 ("Nexus_InjectWithRemoteThread")
    g_fnMouseRelativeMovement ＝ _取导出地址 ("Nexus_MouseRelativeMovement")
    g_fnMouseAbsoluteMovement ＝ _取导出地址 ("Nexus_MouseAbsoluteMovement")
    g_fnMouseDown ＝ _取导出地址 ("Nexus_MouseDown")
    g_fnMouseUp ＝ _取导出地址 ("Nexus_MouseUp")
    g_fnMouseSendInput ＝ _取导出地址 ("Nexus_MouseSendInput")
    g_fnMouseSendInputEx ＝ _取导出地址 ("Nexus_MouseSendInputEx")
    g_fnMouseRelativeMovementEx ＝ _取导出地址 ("Nexus_MouseRelativeMovementEx")
    g_fnMouseAbsoluteMovementEx ＝ _取导出地址 ("Nexus_MouseAbsoluteMovementEx")
    g_fnMouseDownEx ＝ _取导出地址 ("Nexus_MouseDownEx")
    g_fnMouseUpEx ＝ _取导出地址 ("Nexus_MouseUpEx")
    g_fnKeyPressed ＝ _取导出地址 ("Nexus_KeyPressed")
    g_fnKeyRelease ＝ _取导出地址 ("Nexus_KeyRelease")
    g_fnCheckAntiVm ＝ _取导出地址 ("Nexus_CheckAntiVm")
    g_fnCheckAntiDebug ＝ _取导出地址 ("Nexus_CheckAntiDebug")
    g_fnGetLastCallStatus ＝ _取导出地址 ("Nexus_GetLastCallStatus")

    .如果真 (g_fnInitDrv ＝ 0 或 g_fnUnloadDrv ＝ 0 或 g_fnTestComm ＝ 0 或 g_fnInitRWProcess ＝ 0 或 g_fnReadPhysical ＝ 0 或 g_fnWritePhysical ＝ 0)
        驱动_卸载 ()
        返回 (假)
    .如果真结束

    返回 (真)

.子程序 驱动_卸载, 逻辑型, 公开
    .局部变量 返回值, 整数型

    返回值 ＝ 真
    .如果真 (g_hNexus ≠ 0)
        .如果真 (g_fnUnloadDrv ≠ 0)
            g_lastNtStatus ＝ _调用0 (g_fnUnloadDrv)
        .如果真结束

        返回值 ＝ FreeLibrary (g_hNexus)
    .如果真结束

    _重置函数地址 ()
    返回 (返回值 ≠ 0)

.子程序 驱动_卸载驱动, 逻辑型, 公开
    返回 (驱动_卸载 ())

.子程序 驱动_验证, 逻辑型, 公开
    .参数 卡密, 文本型
    .参数 使用PDB方式, 逻辑型, 可空
    .局部变量 卡密字节集, 字节集

    .如果真 (g_fnInitDrv ＝ 0)
        g_lastInitStatus ＝ -1
        返回 (假)
    .如果真结束

    卡密字节集 ＝ _文本转Ansi字节集 (卡密)
    g_lastInitStatus ＝ _调用2 (g_fnInitDrv, 取变量数据地址 (卡密字节集), 使用PDB方式)
    返回 (g_lastInitStatus ＝ 0)

.子程序 驱动_安装驱动, 逻辑型, 公开
    .参数 DLL路径, 文本型
    .参数 卡密, 文本型
    .参数 使用PDB方式, 逻辑型, 可空

    .如果真 (驱动_加载 (DLL路径) ＝ 假)
        返回 (假)
    .如果真结束

    .如果真 (驱动_验证 (卡密, 使用PDB方式) ＝ 假)
        驱动_卸载 ()
        返回 (假)
    .如果真结束

    .如果真 (驱动_通信测试 () ＝ 假)
        驱动_卸载 ()
        返回 (假)
    .如果真结束

    返回 (真)

.子程序 驱动_通信测试, 逻辑型, 公开
    .如果真 (g_fnTestComm ＝ 0)
        返回 (假)
    .如果真结束

    返回 (_调用0 (g_fnTestComm) ≠ 0)

.子程序 进程_初始化读写, 逻辑型, 公开
    .参数 进程ID, 整数型
    .参数 解CR3, 逻辑型, 可空

    .如果真 (g_fnInitRWProcess ＝ 0)
        g_lastNtStatus ＝ -1
        返回 (假)
    .如果真结束

    g_lastNtStatus ＝ _调用2 (g_fnInitRWProcess, 进程ID, 解CR3)
    返回 (NT状态成功 (g_lastNtStatus))

.子程序 进程_附加, 逻辑型, 公开
    .参数 进程ID, 整数型
    .参数 解CR3, 逻辑型, 可空
    返回 (进程_初始化读写 (进程ID, 解CR3))

.子程序 读字节集, 字节集, 公开
    .参数 进程ID, 整数型
    .参数 地址, 长整数型
    .参数 长度, 整数型
    .局部变量 缓冲区, 字节集

    .如果真 (g_fnReadPhysical ＝ 0 或 长度 ≤ 0)
        g_lastNtStatus ＝ -1
        返回 ({})
    .如果真结束

    缓冲区 ＝ 取空白字节集 (长度)
    g_lastNtStatus ＝ _调用4 (g_fnReadPhysical, 进程ID, 地址, 取变量数据地址 (缓冲区), 长度)

    .如果真 (NT状态成功 (g_lastNtStatus) ＝ 假)
        返回 ({})
    .如果真结束

    返回 (缓冲区)

.子程序 写字节集, 逻辑型, 公开
    .参数 进程ID, 整数型
    .参数 地址, 长整数型
    .参数 数据, 字节集
    .局部变量 数据长度, 整数型

    数据长度 ＝ 取字节集长度 (数据)
    .如果真 (g_fnWritePhysical ＝ 0 或 数据长度 ≤ 0)
        g_lastNtStatus ＝ -1
        返回 (假)
    .如果真结束

    g_lastNtStatus ＝ _调用4 (g_fnWritePhysical, 进程ID, 地址, 取变量数据地址 (数据), 数据长度)
    返回 (NT状态成功 (g_lastNtStatus))

.子程序 内存_读字节集, 字节集, 公开
    .参数 进程ID, 整数型
    .参数 地址, 长整数型
    .参数 长度, 整数型
    返回 (读字节集 (进程ID, 地址, 长度))

.子程序 内存_写字节集, 逻辑型, 公开
    .参数 进程ID, 整数型
    .参数 地址, 长整数型
    .参数 数据, 字节集
    返回 (写字节集 (进程ID, 地址, 数据))

.子程序 读整数型, 整数型, 公开
    .参数 进程ID, 整数型
    .参数 地址, 长整数型
    .局部变量 缓冲, 整数型

    缓冲 ＝ 0
    .如果真 (g_fnReadPhysical ＝ 0)
        g_lastNtStatus ＝ -1
        返回 (0)
    .如果真结束

    g_lastNtStatus ＝ _调用4 (g_fnReadPhysical, 进程ID, 地址, 取变量地址 (缓冲), 4)
    .如果真 (NT状态成功 (g_lastNtStatus) ＝ 假)
        返回 (0)
    .如果真结束

    返回 (缓冲)

.子程序 写整数型, 逻辑型, 公开
    .参数 进程ID, 整数型
    .参数 地址, 长整数型
    .参数 数值, 整数型
    .局部变量 缓冲, 整数型

    缓冲 ＝ 数值
    .如果真 (g_fnWritePhysical ＝ 0)
        g_lastNtStatus ＝ -1
        返回 (假)
    .如果真结束

    g_lastNtStatus ＝ _调用4 (g_fnWritePhysical, 进程ID, 地址, 取变量地址 (缓冲), 4)
    返回 (NT状态成功 (g_lastNtStatus))

.子程序 内存_读整数型, 整数型, 公开
    .参数 进程ID, 整数型
    .参数 地址, 长整数型
    返回 (读整数型 (进程ID, 地址))

.子程序 内存_写整数型, 逻辑型, 公开
    .参数 进程ID, 整数型
    .参数 地址, 长整数型
    .参数 数值, 整数型
    返回 (写整数型 (进程ID, 地址, 数值))

.子程序 读长整数, 长整数型, 公开
    .参数 进程ID, 整数型
    .参数 地址, 长整数型
    .局部变量 缓冲, 长整数型

    缓冲 ＝ 0
    .如果真 (g_fnReadPhysical ＝ 0)
        g_lastNtStatus ＝ -1
        返回 (0)
    .如果真结束

    g_lastNtStatus ＝ _调用4 (g_fnReadPhysical, 进程ID, 地址, 取变量地址 (缓冲), 8)
    .如果真 (NT状态成功 (g_lastNtStatus) ＝ 假)
        返回 (0)
    .如果真结束

    返回 (缓冲)

.子程序 写长整数, 逻辑型, 公开
    .参数 进程ID, 整数型
    .参数 地址, 长整数型
    .参数 数值, 长整数型
    .局部变量 缓冲, 长整数型

    缓冲 ＝ 数值
    .如果真 (g_fnWritePhysical ＝ 0)
        g_lastNtStatus ＝ -1
        返回 (假)
    .如果真结束

    g_lastNtStatus ＝ _调用4 (g_fnWritePhysical, 进程ID, 地址, 取变量地址 (缓冲), 8)
    返回 (NT状态成功 (g_lastNtStatus))

.子程序 内存_读长整数型, 长整数型, 公开
    .参数 进程ID, 整数型
    .参数 地址, 长整数型
    返回 (读长整数 (进程ID, 地址))

.子程序 内存_写长整数型, 逻辑型, 公开
    .参数 进程ID, 整数型
    .参数 地址, 长整数型
    .参数 数值, 长整数型
    返回 (写长整数 (进程ID, 地址, 数值))

.子程序 读浮点数, 小数型, 公开
    .参数 进程ID, 整数型
    .参数 地址, 长整数型
    .局部变量 缓冲, 小数型

    缓冲 ＝ 0
    .如果真 (g_fnReadPhysical ＝ 0)
        g_lastNtStatus ＝ -1
        返回 (0)
    .如果真结束

    g_lastNtStatus ＝ _调用4 (g_fnReadPhysical, 进程ID, 地址, 取变量地址 (缓冲), 4)
    .如果真 (NT状态成功 (g_lastNtStatus) ＝ 假)
        返回 (0)
    .如果真结束

    返回 (缓冲)

.子程序 写浮点数, 逻辑型, 公开
    .参数 进程ID, 整数型
    .参数 地址, 长整数型
    .参数 数值, 小数型
    .局部变量 缓冲, 小数型

    缓冲 ＝ 数值
    .如果真 (g_fnWritePhysical ＝ 0)
        g_lastNtStatus ＝ -1
        返回 (假)
    .如果真结束

    g_lastNtStatus ＝ _调用4 (g_fnWritePhysical, 进程ID, 地址, 取变量地址 (缓冲), 4)
    返回 (NT状态成功 (g_lastNtStatus))

.子程序 内存_读小数型, 小数型, 公开
    .参数 进程ID, 整数型
    .参数 地址, 长整数型
    返回 (读浮点数 (进程ID, 地址))

.子程序 内存_写小数型, 逻辑型, 公开
    .参数 进程ID, 整数型
    .参数 地址, 长整数型
    .参数 数值, 小数型
    返回 (写浮点数 (进程ID, 地址, 数值))

.子程序 读4字节, 整数型, 公开
    .参数 进程ID, 整数型
    .参数 地址, 长整数型
    返回 (读整数型 (进程ID, 地址))

.子程序 写4字节, 逻辑型, 公开
    .参数 进程ID, 整数型
    .参数 地址, 长整数型
    .参数 数值, 整数型
    返回 (写整数型 (进程ID, 地址, 数值))

.子程序 读8字节, 长整数型, 公开
    .参数 进程ID, 整数型
    .参数 地址, 长整数型
    返回 (读长整数 (进程ID, 地址))

.子程序 写8字节, 逻辑型, 公开
    .参数 进程ID, 整数型
    .参数 地址, 长整数型
    .参数 数值, 长整数型
    返回 (写长整数 (进程ID, 地址, 数值))

.子程序 内存_读4字节, 整数型, 公开
    .参数 进程ID, 整数型
    .参数 地址, 长整数型
    返回 (读4字节 (进程ID, 地址))

.子程序 内存_写4字节, 逻辑型, 公开
    .参数 进程ID, 整数型
    .参数 地址, 长整数型
    .参数 数值, 整数型
    返回 (写4字节 (进程ID, 地址, 数值))

.子程序 内存_读8字节, 长整数型, 公开
    .参数 进程ID, 整数型
    .参数 地址, 长整数型
    返回 (读8字节 (进程ID, 地址))

.子程序 内存_写8字节, 逻辑型, 公开
    .参数 进程ID, 整数型
    .参数 地址, 长整数型
    .参数 数值, 长整数型
    返回 (写8字节 (进程ID, 地址, 数值))

.子程序 取最后大小, 长整数型, 公开
    返回 (g_lastSize)

.子程序 模块_初始化, 逻辑型, 公开
    .参数 DLL路径, 文本型
    .局部变量 路径字节集, 字节集

    .如果真 (g_fnInitializeModule ＝ 0)
        返回 (假)
    .如果真结束

    路径字节集 ＝ _文本转Ansi字节集 (DLL路径)
    返回 (_调用1 (g_fnInitializeModule, 取变量数据地址 (路径字节集)) ≠ 0)

.子程序 模块_内存初始化, 逻辑型, 公开
    .如果真 (g_fnInitializeModuleWithMemory ＝ 0)
        返回 (假)
    .如果真结束

    返回 (_调用0 (g_fnInitializeModuleWithMemory) ≠ 0)

.子程序 进程_取CR3, 长整数型, 公开
    .参数 进程ID, 整数型
    .参数 解CR3, 逻辑型, 可空

    .如果真 (g_fnGetProcessCr3 ＝ 0)
        返回 (0)
    .如果真结束

    返回 (_调用2 (g_fnGetProcessCr3, 进程ID, 解CR3))

.子程序 内存_刷页读字节集, 字节集, 公开
    .参数 进程ID, 整数型
    .参数 地址, 长整数型
    .参数 长度, 整数型
    .局部变量 缓冲区, 字节集

    .如果真 (g_fnInvlpgReadPhysical ＝ 0 或 长度 ≤ 0)
        g_lastNtStatus ＝ -1
        返回 ({})
    .如果真结束

    缓冲区 ＝ 取空白字节集 (长度)
    g_lastNtStatus ＝ _调用4 (g_fnInvlpgReadPhysical, 进程ID, 地址, 取变量数据地址 (缓冲区), 长度)
    .如果真 (NT状态成功 (g_lastNtStatus) ＝ 假)
        返回 ({})
    .如果真结束

    返回 (缓冲区)

.子程序 内存_刷页写字节集, 逻辑型, 公开
    .参数 进程ID, 整数型
    .参数 地址, 长整数型
    .参数 数据, 字节集
    .局部变量 数据长度, 整数型

    数据长度 ＝ 取字节集长度 (数据)
    .如果真 (g_fnInvlpgWritePhysical ＝ 0 或 数据长度 ≤ 0)
        g_lastNtStatus ＝ -1
        返回 (假)
    .如果真结束

    g_lastNtStatus ＝ _调用4 (g_fnInvlpgWritePhysical, 进程ID, 地址, 取变量数据地址 (数据), 数据长度)
    返回 (NT状态成功 (g_lastNtStatus))

.子程序 模块_取主模块地址_附加, 长整数型, 公开
    .参数 进程ID, 整数型
    .局部变量 模块地址, 长整数型

    模块地址 ＝ 0
    g_lastSize ＝ 0
    .如果真 (g_fnGetModuleAttach ＝ 0)
        g_lastNtStatus ＝ -1
        返回 (0)
    .如果真结束

    g_lastNtStatus ＝ _调用2 (g_fnGetModuleAttach, 进程ID, 取变量地址 (模块地址))
    .如果真 (NT状态成功 (g_lastNtStatus) ＝ 假)
        返回 (0)
    .如果真结束

    返回 (模块地址)

.子程序 模块_取模块地址, 长整数型, 公开
    .参数 进程ID, 整数型
    .参数 模块名, 文本型
    .局部变量 模块地址, 长整数型
    .局部变量 模块大小, 长整数型
    .局部变量 模块名字节集, 字节集

    模块地址 ＝ 0
    模块大小 ＝ 0
    g_lastSize ＝ 0
    .如果真 (g_fnGetModuleNoAttachA ＝ 0)
        g_lastNtStatus ＝ -1
        返回 (0)
    .如果真结束

    模块名字节集 ＝ _文本转Ansi字节集 (模块名)
    g_lastNtStatus ＝ _调用4 (g_fnGetModuleNoAttachA, 进程ID, 取变量数据地址 (模块名字节集), 取变量地址 (模块地址), 取变量地址 (模块大小))
    g_lastSize ＝ 模块大小
    .如果真 (NT状态成功 (g_lastNtStatus) ＝ 假)
        返回 (0)
    .如果真结束

    返回 (模块地址)

.子程序 模块_取内核模块地址, 长整数型, 公开
    .参数 模块名, 文本型
    .局部变量 模块地址, 长整数型
    .局部变量 模块大小, 长整数型
    .局部变量 模块名字节集, 字节集

    模块地址 ＝ 0
    模块大小 ＝ 0
    g_lastSize ＝ 0
    .如果真 (g_fnGetKernelModuleInfo ＝ 0)
        g_lastNtStatus ＝ -1
        返回 (0)
    .如果真结束

    模块名字节集 ＝ _文本转Ansi字节集 (模块名)
    g_lastNtStatus ＝ _调用3 (g_fnGetKernelModuleInfo, 取变量数据地址 (模块名字节集), 取变量地址 (模块地址), 取变量地址 (模块大小))
    g_lastSize ＝ 模块大小
    .如果真 (NT状态成功 (g_lastNtStatus) ＝ 假)
        返回 (0)
    .如果真结束

    返回 (模块地址)

.子程序 特征_搜索, 长整数型, 公开
    .参数 进程ID, 整数型
    .参数 模块名, 文本型
    .参数 特征码, 文本型
    .参数 掩码, 文本型
    .局部变量 模块名字节集, 字节集
    .局部变量 特征码字节集, 字节集
    .局部变量 掩码字节集, 字节集
    .局部变量 返回地址, 长整数型

    .如果真 (g_fnPatternSearchA ＝ 0)
        g_lastNtStatus ＝ -1
        返回 (0)
    .如果真结束

    模块名字节集 ＝ _文本转Ansi字节集 (模块名)
    特征码字节集 ＝ _文本转Ansi字节集 (特征码)
    掩码字节集 ＝ _文本转Ansi字节集 (掩码)
    返回地址 ＝ _调用4 (g_fnPatternSearchA, 进程ID, 取变量数据地址 (模块名字节集), 取变量数据地址 (特征码字节集), 取变量数据地址 (掩码字节集))
    _刷新扩展状态 ()
    返回 (返回地址)

.子程序 内存_申请, 长整数型, 公开
    .参数 进程ID, 整数型
    .参数 大小, 长整数型
    .参数 保护属性, 整数型
    .参数 申请类型, 整数型
    .局部变量 返回地址, 长整数型

    .如果真 (g_fnAllocateMemoryForE ＝ 0)
        g_lastNtStatus ＝ -1
        返回 (0)
    .如果真结束

    返回地址 ＝ _调用4 (g_fnAllocateMemoryForE, 进程ID, 大小, 保护属性, 申请类型)
    _刷新扩展状态 ()
    返回 (返回地址)

.子程序 内存_释放, 逻辑型, 公开
    .参数 进程ID, 整数型
    .参数 地址, 长整数型
    .参数 大小, 长整数型

    .如果真 (g_fnFreeMemory ＝ 0)
        g_lastNtStatus ＝ -1
        返回 (假)
    .如果真结束

    g_lastNtStatus ＝ _调用3 (g_fnFreeMemory, 进程ID, 地址, 大小)
    返回 (NT状态成功 (g_lastNtStatus))

.子程序 注入_APC附加, 长整数型, 公开
    .参数 进程ID, 整数型
    .参数 DLL数据, 字节集
    .参数 申请类型, 整数型
    .局部变量 返回地址, 长整数型

    .如果真 (g_fnInjectApcAttachForE ＝ 0 或 取字节集长度 (DLL数据) ≤ 0)
        g_lastNtStatus ＝ -1
        返回 (0)
    .如果真结束

    返回地址 ＝ _调用4 (g_fnInjectApcAttachForE, 进程ID, 取变量数据地址 (DLL数据), 取字节集长度 (DLL数据), 申请类型)
    _刷新扩展状态 ()
    返回 (返回地址)

.子程序 注入_APC无附加, 长整数型, 公开
    .参数 进程ID, 整数型
    .参数 DLL数据, 字节集
    .参数 申请类型, 整数型
    .局部变量 返回地址, 长整数型

    .如果真 (g_fnInjectApcNoAttachForE ＝ 0 或 取字节集长度 (DLL数据) ≤ 0)
        g_lastNtStatus ＝ -1
        返回 (0)
    .如果真结束

    返回地址 ＝ _调用4 (g_fnInjectApcNoAttachForE, 进程ID, 取变量数据地址 (DLL数据), 取字节集长度 (DLL数据), 申请类型)
    _刷新扩展状态 ()
    返回 (返回地址)

.子程序 注入_劫持线程附加, 长整数型, 公开
    .参数 进程ID, 整数型
    .参数 DLL数据, 字节集
    .参数 申请类型, 整数型
    .局部变量 返回地址, 长整数型

    .如果真 (g_fnInjectHijackAttachForE ＝ 0 或 取字节集长度 (DLL数据) ≤ 0)
        g_lastNtStatus ＝ -1
        返回 (0)
    .如果真结束

    返回地址 ＝ _调用4 (g_fnInjectHijackAttachForE, 进程ID, 取变量数据地址 (DLL数据), 取字节集长度 (DLL数据), 申请类型)
    _刷新扩展状态 ()
    返回 (返回地址)

.子程序 注入_劫持线程无附加, 长整数型, 公开
    .参数 进程ID, 整数型
    .参数 DLL数据, 字节集
    .参数 申请类型, 整数型
    .局部变量 返回地址, 长整数型

    .如果真 (g_fnInjectHijackNoAttachForE ＝ 0 或 取字节集长度 (DLL数据) ≤ 0)
        g_lastNtStatus ＝ -1
        返回 (0)
    .如果真结束

    返回地址 ＝ _调用4 (g_fnInjectHijackNoAttachForE, 进程ID, 取变量数据地址 (DLL数据), 取字节集长度 (DLL数据), 申请类型)
    _刷新扩展状态 ()
    返回 (返回地址)

.子程序 注入_远程线程, 长整数型, 公开
    .参数 进程ID, 整数型
    .参数 DLL数据, 字节集
    .局部变量 返回地址, 长整数型

    返回地址 ＝ 0
    .如果真 (g_fnInjectRemoteThread ＝ 0 或 取字节集长度 (DLL数据) ≤ 0)
        g_lastNtStatus ＝ -1
        返回 (0)
    .如果真结束

    g_lastNtStatus ＝ _调用4 (g_fnInjectRemoteThread, 进程ID, 取变量数据地址 (DLL数据), 取字节集长度 (DLL数据), 取变量地址 (返回地址))
    .如果真 (NT状态成功 (g_lastNtStatus) ＝ 假)
        返回 (0)
    .如果真结束

    返回 (返回地址)

.子程序 输入_鼠标相对移动, , 公开
    .参数 偏移X, 整数型
    .参数 偏移Y, 整数型
    .如果真 (g_fnMouseRelativeMovement ≠ 0)
        _调用2 (g_fnMouseRelativeMovement, 偏移X, 偏移Y)
    .如果真结束

.子程序 输入_鼠标绝对移动, , 公开
    .参数 坐标X, 整数型
    .参数 坐标Y, 整数型
    .如果真 (g_fnMouseAbsoluteMovement ≠ 0)
        _调用2 (g_fnMouseAbsoluteMovement, 坐标X, 坐标Y)
    .如果真结束

.子程序 输入_鼠标按下, , 公开
    .如果真 (g_fnMouseDown ≠ 0)
        _调用0 (g_fnMouseDown)
    .如果真结束

.子程序 输入_鼠标抬起, , 公开
    .如果真 (g_fnMouseUp ≠ 0)
        _调用0 (g_fnMouseUp)
    .如果真结束

.子程序 输入_鼠标发送, , 公开
    .参数 偏移X, 整数型
    .参数 偏移Y, 整数型
    .参数 按键标志, 整数型
    .参数 移动标志, 整数型
    .如果真 (g_fnMouseSendInput ≠ 0)
        _调用4 (g_fnMouseSendInput, 偏移X, 偏移Y, 按键标志, 移动标志)
    .如果真结束

.子程序 输入_鼠标发送Ex, 逻辑型, 公开
    .参数 偏移X, 整数型
    .参数 偏移Y, 整数型
    .参数 按键标志, 整数型
    .参数 移动标志, 整数型
    .如果真 (g_fnMouseSendInputEx ＝ 0)
        g_lastNtStatus ＝ -1
        返回 (假)
    .如果真结束

    g_lastNtStatus ＝ _调用4 (g_fnMouseSendInputEx, 偏移X, 偏移Y, 按键标志, 移动标志)
    返回 (NT状态成功 (g_lastNtStatus))

.子程序 输入_鼠标相对移动Ex, , 公开
    .参数 偏移X, 整数型
    .参数 偏移Y, 整数型
    .如果真 (g_fnMouseRelativeMovementEx ≠ 0)
        _调用2 (g_fnMouseRelativeMovementEx, 偏移X, 偏移Y)
    .如果真结束

.子程序 输入_鼠标绝对移动Ex, , 公开
    .参数 坐标X, 整数型
    .参数 坐标Y, 整数型
    .如果真 (g_fnMouseAbsoluteMovementEx ≠ 0)
        _调用2 (g_fnMouseAbsoluteMovementEx, 坐标X, 坐标Y)
    .如果真结束

.子程序 输入_鼠标按下Ex, , 公开
    .如果真 (g_fnMouseDownEx ≠ 0)
        _调用0 (g_fnMouseDownEx)
    .如果真结束

.子程序 输入_鼠标抬起Ex, , 公开
    .如果真 (g_fnMouseUpEx ≠ 0)
        _调用0 (g_fnMouseUpEx)
    .如果真结束

.子程序 输入_键盘按下, , 公开
    .参数 键码, 整数型
    .如果真 (g_fnKeyPressed ≠ 0)
        _调用1 (g_fnKeyPressed, 键码)
    .如果真结束

.子程序 输入_键盘抬起, , 公开
    .参数 键码, 整数型
    .如果真 (g_fnKeyRelease ≠ 0)
        _调用1 (g_fnKeyRelease, 键码)
    .如果真结束

.子程序 环境_检测虚拟机, 逻辑型, 公开
    .如果真 (g_fnCheckAntiVm ＝ 0)
        返回 (假)
    .如果真结束
    返回 (_调用0 (g_fnCheckAntiVm) ≠ 0)

.子程序 环境_检测调试, 逻辑型, 公开
    .如果真 (g_fnCheckAntiDebug ＝ 0)
        返回 (假)
    .如果真结束
    返回 (_调用0 (g_fnCheckAntiDebug) ≠ 0)

.子程序 使用示例
    .局部变量 状态OK, 逻辑型
    .局部变量 PID, 整数型
    .局部变量 地址, 长整数型
    .局部变量 整数值, 整数型
    .局部变量 长整数值, 长整数型
    .局部变量 浮点值, 小数型
    .局部变量 数据, 字节集

    PID ＝ 1234
    地址 ＝ 4194304

    状态OK ＝ 驱动_加载 ("Nexus-Drv-MapperLib.vmp.dll")
    .如果真 (状态OK ＝ 假)
        调试输出 ("驱动DLL加载失败")
        返回 ()
    .如果真结束

    状态OK ＝ 驱动_验证 ("你的卡密", 假)
    .如果真 (状态OK ＝ 假)
        调试输出 ("驱动验证失败，状态码=", 取最后初始化状态 ())
        返回 ()
    .如果真结束

    状态OK ＝ 驱动_通信测试 ()
    .如果真 (状态OK ＝ 假)
        调试输出 ("驱动通信失败")
        返回 ()
    .如果真结束

    状态OK ＝ 进程_初始化读写 (PID, 假)
    .如果真 (状态OK ＝ 假)
        调试输出 ("进程初始化失败，NT状态=", 取最后NT状态 ())
        返回 ()
    .如果真结束

    整数值 ＝ 读整数型 (PID, 地址)
    长整数值 ＝ 读长整数 (PID, 地址)
    浮点值 ＝ 读浮点数 (PID, 地址)
    数据 ＝ 读字节集 (PID, 地址, 16)

    调试输出 ("整数值=", 整数值)
    调试输出 ("长整数值=", 长整数值)
    调试输出 ("浮点值=", 浮点值)
    调试输出 ("字节集长度=", 取字节集长度 (数据))
