#include "cheats.hpp"

namespace CTRPluginFramework
{
    Hook g_damageHook;
    Hook g_SPHook;

    bool g_isOhkoEnabled = false;
    bool g_isGodModeEnabled = false;
    bool g_isInfiniteSPEnabled = false;

    void Hook_ApplyRawDamage(float damage, u32 target_stats_ptr)
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

    int Hook_ApplySPCost(float cost, u32 stats_ptr) {
        auto& ctx = HookContext::GetCurrent();
        cost = 0.0f;
        return ctx.OriginalFunction<int>(cost, stats_ptr);
    }

    void ToggleDamageCheat(MenuEntry *entry)
    {
        g_isOhkoEnabled = entry->IsActivated();

        if (entry->WasJustActivated())
        {
            if (!g_damageHook.IsEnabled())
            {
                g_damageHook.InitializeForMitm(0x001cc7f0, (u32)Hook_ApplyRawDamage);
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
                g_damageHook.InitializeForMitm(0x001cc7f0, (u32)Hook_ApplyRawDamage);
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

    void ToggleInfiniteSPCheat(MenuEntry *entry)
    {
        if (entry->WasJustActivated())
        {
            if (!g_SPHook.IsEnabled())
            {
                g_SPHook.InitializeForMitm(0x002ffce0, (u32)Hook_ApplySPCost);
                g_SPHook.Enable();
            }
            OSD::Notify("Infinite SP enabled.");
        }
        else if (!entry->IsActivated())
        {
            OSD::Notify("Infinite SP disabled.");
            g_SPHook.Disable();
        }
    }

    void InitMenu(PluginMenu *menu)
    {
        menu->Append(new MenuEntry("One Hit KO (99999 Damage)", ToggleDamageCheat));
        menu->Append(new MenuEntry("God Mode", ToggleGodModeCheat));
        menu->Append(new MenuEntry("Infinite SP (No SP consumption)", ToggleInfiniteSPCheat));
    }
}
