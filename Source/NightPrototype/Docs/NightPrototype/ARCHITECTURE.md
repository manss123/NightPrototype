# ARCHITECTURE

## Core Direction

NightPrototype is a Dark Fantasy RPG Sandbox inspired by Kenshi.

The game is Event/Story Driven. World events create memory, relationship changes, dialogue changes, nemesis state, quests, and consequences.

The game should feel like a living sandbox, not a scripted one-off RPG sequence.

## Design Rules

- C++ owns gameplay logic.
- Blueprint owns data, references, tuning, visuals, and temporary debug.
- Avoid hard-coded FirstNight logic.
- Avoid Vampire-only logic unless it is temporary prototype data.
- Systems must be reusable across encounter types.
- Visual assets do not define gameplay identity.
- Gameplay identity comes from GameplayTags, DataAssets, profiles, and event records.
- Prefer generic event records over one helper function per story moment.

## Event Model

Preferred flow:

```text
World Event
-> Memory
-> Relationship / Emotion / Need
-> Dialogue / Quest / Encounter / Consequence
-> New World Event
```

## Runtime System Map

- [[SYSTEMS/WorldEvents]]
- [[SYSTEMS/TechStack]]
- [[SYSTEMS/Encounter]]
- [[SYSTEMS/NPC]]
- [[SYSTEMS/Combat]]
- [[SYSTEMS/EnemyAI]]
- [[SYSTEMS/Dialogue]]
- [[SYSTEMS/Memory]]

## C++ / Blueprint Boundary

C++ should contain:
- Interaction rules
- Combat rules
- Health and body state rules
- Enemy AI state rules
- Encounter evaluation and activation
- World event storage and query logic
- Dialogue selection logic

Blueprint should contain:
- Meshes
- Animation references
- Widget classes
- DataAsset references
- Tuning values
- Visual setup
- Temporary debug wiring

## Current Core Loop

```text
DayNightManager
-> EncounterDirector
-> Encounter Template
-> Primary Enemy lookup through EnemyRoster
-> Enemy reveal
-> Enemy AI activation
-> Combat
-> BodyHealth outcome
-> EncounterOutcomeComponent
-> EncounterDirector resolves outcome
-> GameInstance records FDWWorldEvent
-> NPC dialogue can react to world tags
```

## Not Now

- Full quest generator
- NPC schedule simulation
- Large faction war simulation
- Economy simulation
- Character creator
- Large skill tree
- Full behavior tree AI
- Full dialogue authoring tool
