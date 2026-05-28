# NightPrototype

Dark Fantasy RPG Sandbox / Kenshi-like / Event-Story Driven prototype.

## Core Notes

- [[ARCHITECTURE]]
- [[ROADMAP]]

## Systems

- [[SYSTEMS/WorldEvents]]
- [[SYSTEMS/TechStack]]
- [[SYSTEMS/Encounter]]
- [[SYSTEMS/NPC]]
- [[SYSTEMS/NPCGeneration]]
- [[SYSTEMS/Combat]]
- [[SYSTEMS/EnemyAI]]
- [[SYSTEMS/EnemyGeneration]]
- [[SYSTEMS/Dialogue]]
- [[SYSTEMS/Memory]]

## Lore / World

- [[LORE/LoreCanon]]
- [[LORE/WorldBible]]
- [[LORE/FactionPolitics]]
- [[LORE/RegionsAndBiomes]]

Lore reading order:

```text
LoreCanon
-> WorldBible
-> FactionPolitics / RegionsAndBiomes
```

## Raw Notes

- [[Feature - Unrefined]]
- [[Lore - Unrefined]]
- [[Unrefined - Bible]]
- [[Tools-Techs - Unrefined]]

## Current Focus

Phase 9: Enemy AI / Encounter Behavior

Current goals:
- Enemies can start dormant.
- Encounters reveal and activate primary enemies.
- Combat outcomes create world event records.
- World events drive future memory, dialogue, nemesis, quests, and consequences.

## Core Design Rule

Gameplay identity comes from data and tags, not visual assets.

Example:
- Visual: Werewolf mesh
- Story threat: `Threat.FeralVampire`
