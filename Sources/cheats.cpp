#include "cheats.hpp"

namespace CTRPluginFramework
{

    Hook g_damageHook;

    void ApplyRawDamageOHKO(float damage, u32 target_stats_ptr)
    {
        if (target_stats_ptr != 0)
        {
            u32 combat_actor_ptr = target_stats_ptr - 0xD4;

            int combat_team_id = *(int*)(combat_actor_ptr + 0x554);

            // enemies have 1 in thie field
            if (combat_team_id == 1)
                damage = 99999.0f;
        }

        auto& ctx = HookContext::GetCurrent();
        // calls the ApplyRawDamage function with the modified damage
        ctx.OriginalFunction<void>(damage, target_stats_ptr);
    }

    void ToggleDamageCheat(MenuEntry *entry)
    {
        if (entry->WasJustActivated())
        {
            if (!g_damageHook.IsEnabled())
            {
                u32 applyRawDamage_Address = 0x001cc7f0;

                g_damageHook.InitializeForMitm(applyRawDamage_Address, (u32)ApplyRawDamageOHKO);
                g_damageHook.Enable();
            }
            OSD::Notify("One Hit KO enabled.");
        }
        else if (!entry->IsActivated())
        {
            g_damageHook.Disable();
            OSD::Notify("One Hit KO disabled.");
        }
    }

    void InitMenu(PluginMenu *menu)
    {
        menu->Append(new MenuEntry("One Hit KO (99999 Damage)", ToggleDamageCheat));
    }
}
