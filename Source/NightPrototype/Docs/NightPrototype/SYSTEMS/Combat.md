# Combat

## Purpose

Combat provides a small RPG melee loop that can feed world events and consequences.

The goal is not deep combat yet. The goal is a stable sandbox interaction:

```text
Select target
-> Move into range
-> Auto attack
-> Apply body-part damage
-> Downed / death outcome
-> Encounter consequence
```

## Current Owner Classes

- `UDWCombatComponent`
- `UDWBodyHealthComponent`
- `UDWBodyStateRagdollComponent`
- `UDWEncounterOutcomeComponent`

## Current Features

- Auto attack target.
- Attack range.
- Attack cooldown.
- Attack speed multiplier.
- Damage multiplier.
- Body-part health.
- Downed state.
- Recovery state.
- Ragdoll on downed/death.
- Get-up recovery.

## Body State Ladder

```text
Healthy
-> Injured
-> Downed / Unconscious
-> Dying
-> Dead
```

## Important Fixes Already Made

- Auto attack stops when target is out of range.
- Combat target validation rejects incapacitated targets.
- Enemy does not keep attacking a downed player.
- Prompt is hidden while player input is locked.
- Ragdoll recovery trace was shortened to avoid placing the player on roofs or on top of enemies.

## Current Limitations

- Damage timing is timer-based, not animation-notify-based.
- Player unarmed animations exist but are not fully connected to hit timing.
- Werewolf animations exist but are not fully connected to hit timing.

## Future

- Attack montage.
- Hit frame AnimNotify.
- Stamina or fatigue.
- Weapon damage types.
- Body-part hit selection.
- Armor or protection layer.
