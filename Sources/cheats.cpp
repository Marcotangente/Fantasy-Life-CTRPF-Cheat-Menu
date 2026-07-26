#include "cheats.hpp"

namespace CTRPluginFramework
{
    void MoneyEditor(MenuEntry *entry)
    {
        u32 money;

        Process::Read32(0x08A47BE8, money);

        Keyboard kb("Money");
        kb.IsHexadecimal(true);

        if (kb.Open(money) != 0)
            return;

        Process::Write32(0x08A47BE8, money);
    }

    void InitMenu(PluginMenu &menu)
    {
        menu += new MenuEntry("Money", MoneyEditor);
    }
}