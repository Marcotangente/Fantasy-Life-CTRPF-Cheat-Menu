#include "cheats.hpp"

namespace CTRPluginFramework
{
    void Money(MenuEntry *entry)
    {
        u32 money = 0;

        Keyboard kb("Money");
        kb.IsHexadecimal(false);
        kb.SetSlider(0, 99999999, 100);

        if (kb.Open(money) >= 0)
            Process::Write32(0x08A47BE8, money);
    }

    void InitMenu(PluginMenu *menu)
    {
        menu->Append(new MenuEntry("Money", Money));
    }
}