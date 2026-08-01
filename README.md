# Fantasy Life - CTRPF Cheat Menu (PoC)

A cheat menu for **Fantasy Life (EUR Version)** on the Nintendo 3DS, powered by [CTRPluginFramework](https://github.com/Nanquitas/CTRPluginFramework) and designed for [Luma3DS](https://github.com/LumaTeam/Luma3DS). (It can also be used with emulators like [Azahar](https://azahar-emu.org/) and [Lime3ds](https://lime3ds.net/)).

> **Note / Disclaimer:** This project serves as a **Proof of Concept (PoC)** reverse-engineering effort. Since I was personally disappointed by *Fantasy Life i*, keeping the original 3DS masterpiece alive through mods and deeper technical analysis is my way of preserving it—with the ultimate hope of seeing an unofficial PC port happen one day, a project I would gladly contribute to!

## Warning
This plugin is built strictly for the **EUR version** of Fantasy Life and will not work on other region versions due to differing memory offsets.

## Features
* **One Hit KO** (99999 damage dealt to enemies)
* **God Mode** (Player takes 0 damage)
* Built-in framework tools: Memory Searcher, Hex Editor, Action Replay Engine, and more.

## Installation
1. Download the latest `.3gx` file from the [Releases](../../releases).
2. Place the file into your SD card under `luma/plugins/0004000000113100/`.
3. Launch the game and press **Select** to open/close the cheat menu.
4. Press `A` to select/deselect a cheat.

---

## Reverse Engineering

While analyzing the game's binary in **Ghidra**, I mapped out a few crucial structures and functions regarding the combat system. *(Note: The names used below are custom labels I assigned during my analysis and are entirely unofficial).*
Here is what has been discovered so far for the EUR version:

### 1. Functions
*(Note: The following code blocks are raw decompiled pseudo-code extracted via Ghidra, using custom labels and variable names.)*

* **`ApplyRawDamage`** (`0x001cc7f0`)
The function that I hooked to make the OHKO and God Mode cheats.
```c
void ApplyRawDamage(float damage,CombatStats *target_stats)
{
  SetClampedHP(target_stats->HP - damage);
  return;
}
```
* **`SetClampedHP`** (`0x001f50d0`)
```c
int SetClampedHP(float requested_new_hp,int entity_ptr)

{
  float previous_hp;
  float max_hp;
  float maybe_hp_modifier;
  float final_new_hp;
  
  previous_hp = *(float *)(entity_ptr + 0x80);
  final_new_hp = 0.0;
  if (*(int *)(entity_ptr + 0x90) == 0) {
    max_hp = (float)FUN_001dbe94(entity_ptr + 8);
  }
  else {
    max_hp = (float)FUN_001dbe94(entity_ptr + 8);
    some_hp_modifier = (float)FUN_001e3ffc(*(undefined4 *)(entity_ptr + 0x90),10);
    max_hp = max_hp + some_hp_modifier;
  }
  if ((0.0 <= requested_new_hp) && (final_new_hp = max_hp, requested_new_hp <= max_hp)) {
    final_new_hp = requested_new_hp;
  }
  *(float *)(entity_ptr + 0x80) = final_new_hp;
  return (int)final_new_hp - (int)previous_hp;
}
```

### 2. Memory Layout & Structures

#### `CombatActor`
* **Size** `0x15cc`
* **CombatStats Offset:** `+ 0xD4` points to the `CombatStats` structure.
* **Team ID Offset:** `+ 0x554`
  * `0` = Player
  * `1` = Enemies

#### `CombatStats`
* **Size** `0x98` (I'm not sure about this, and there are subclasses of this class.)
Passed as the second argument (`target_stats_ptr`) to `ApplyRawDamage`.
* **HP Offset:** `+ 0x80` the HP value, stored as a float.

---

# CTRPF Framework Reference

A list of [all the code types supported by the ActionReplay is available here](https://gist.github.com/Nanquitas/d6c920a59c757cf7917c2bffa76de860).

## Action Replay Usage

[![Click to play on YouTube](https://img.youtube.com/vi/c2258P9wKkA/0.jpg)](https://www.youtube.com/watch?v=c2258P9wKkA)

## Screenshots

![](https://i.imgur.com/ayvcaN0.png)
![](https://i.imgur.com/9OJop01.png)
##### Search
![](https://i.imgur.com/gczUI1r.png)
![](https://i.imgur.com/Yhhcsot.png)
![](https://i.imgur.com/7lVmuDd.png)
##### Action Replay
![](https://i.imgur.com/uW6lnjm.png)
![](https://i.imgur.com/4QAEgSO.png)
![](https://i.imgur.com/bJFQw04.png)
![](https://i.imgur.com/8pZhhmo.png)
![](https://i.imgur.com/b3Gl29H.png)
##### Hex Editor
![](https://i.imgur.com/8z2DCcm.png)
![](https://i.imgur.com/t3gutXU.png)
![](https://i.imgur.com/U4ratx4.png)
![](https://i.imgur.com/uJBJZ8K.png)
