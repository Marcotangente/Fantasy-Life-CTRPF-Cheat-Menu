#include <3ds.h>
#include "csvc.h"
#include <CTRPluginFramework.hpp>
#include "cheats.hpp"

#include <vector>

namespace CTRPluginFramework
{
    // This patch the NFC disabling the touchscreen when scanning an amiibo, which prevents ctrpf to be used
    static void ToggleTouchscreenForceOn(void)
    {
        static u32 original = 0;
        static u32 *patchAddress = nullptr;

        if (patchAddress && original)
        {
            *patchAddress = original;
            return;
        }

        static const std::vector<u32> pattern =
        {
            0xE59F10C0, 0xE5840004, 0xE5841000, 0xE5DD0000,
            0xE5C40008, 0xE28DD03C, 0xE8BD80F0, 0xE5D51001,
            0xE1D400D4, 0xE3510003, 0x159F0034, 0x1A000003
        };

        Handle processHandle;
        s64 textTotalSize = 0;
        s64 startAddress = 0;
        u32 *found;

        if (R_FAILED(svcOpenProcess(&processHandle, 16)))
            return;

        svcGetProcessInfo(&textTotalSize, processHandle, 0x10002);
        svcGetProcessInfo(&startAddress, processHandle, 0x10005);

        if (R_FAILED(svcMapProcessMemoryEx(CUR_PROCESS_HANDLE, 0x14000000, processHandle, (u32)startAddress, textTotalSize)))
            goto exit;

        found = (u32 *)Utils::Search<u32>(0x14000000, (u32)textTotalSize, pattern);

        if (found != nullptr)
        {
            original = found[13];
            patchAddress = (u32 *)PA_FROM_VA(found + 13);
            found[13] = 0xE1A00000;
        }

        svcUnmapProcessMemoryEx(CUR_PROCESS_HANDLE, 0x14000000, textTotalSize);

    exit:
        svcCloseHandle(processHandle);
    }

    // Called before the game starts
    void PatchProcess(FwkSettings &settings)
    {
        ToggleTouchscreenForceOn();
    }

    // Called when the game exits
    void OnProcessExit(void)
    {
        ToggleTouchscreenForceOn();
    }

    int main(void)
    {
        PluginMenu *menu = new PluginMenu(
            "Fantasy Life Cheat Menu",
            0,
            8,
            0,
            "Fantasy Life Cheat Menu\nby Marcotangente\n\nHave fun !"
        );

        // Synchronize the menu with frame events
        menu->SynchronizeWithFrame(true);

        // Add menu entries
        InitMenu(menu);

        // Run the plugin
        menu->Run();

        delete menu;
        return 0;
    }
}
