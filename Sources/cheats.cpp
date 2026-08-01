#include "cheats.hpp"

namespace CTRPluginFramework
{
    Hook g_damageHook;

    bool g_isOhkoEnabled = false;
    bool g_isGodModeEnabled = false;

    void ApplyRawDamageHandler(float damage, u32 target_stats_ptr)
    {
        if (target_stats_ptr != 0)
        {
            u32 combat_actor_ptr = target_stats_ptr - 0xD4;
            int combat_team_id = *(int*)(combat_actor_ptr + 0x554);

            // player has combat team 0
            if (g_isGodModeEnabled && combat_team_id == 0)
                damage = 0.0f;
            // enemies have combat team 1
            else if (g_isOhkoEnabled && combat_team_id == 1)
                damage = 99999.0f;
        }

        auto& ctx = HookContext::GetCurrent();
        ctx.OriginalFunction<void>(damage, target_stats_ptr);
    }

    void ToggleDamageCheat(MenuEntry *entry)
    {
        g_isOhkoEnabled = entry->IsActivated();

        if (entry->WasJustActivated())
        {
            if (!g_damageHook.IsEnabled())
            {
                u32 applyRawDamage_Address = 0x001cc7f0; 
                g_damageHook.InitializeForMitm(applyRawDamage_Address, (u32)ApplyRawDamageHandler);
                g_damageHook.Enable();
            }
            OSD::Notify("One Hit KO enabled.");
        }
        else if (!entry->IsActivated())
        {
            OSD::Notify("One Hit KO disabled.");
            if (!g_isOhkoEnabled && !g_isGodModeEnabled)
                g_damageHook.Disable();
        }
    }

    void ToggleGodModeCheat(MenuEntry *entry)
    {
        g_isGodModeEnabled = entry->IsActivated();

        if (entry->WasJustActivated())
        {
            if (!g_damageHook.IsEnabled())
            {
                g_damageHook.InitializeForMitm(0x001cc7f0, (u32)ApplyRawDamageHandler);
                g_damageHook.Enable();
            }
            OSD::Notify("God Mode enabled.");
        }
        else if (!entry->IsActivated())
        {
            OSD::Notify("God Mode disabled.");
            if (!g_isOhkoEnabled && !g_isGodModeEnabled)
                g_damageHook.Disable();
        }
    }

    void InitMenu(PluginMenu *menu)
    {
        menu->Append(new MenuEntry("One Hit KO (99999 Damage)", ToggleDamageCheat));
        menu->Append(new MenuEntry("God Mode", ToggleGodModeCheat));
    }
}
