# WorldEvents

## Purpose

World events are the game's shared story memory.

They record what happened in the world so other systems can react later.

World events should support sandbox storytelling:

```text
What happened?
Where did it happen?
What threat was involved?
Which encounter caused it?
When did it happen?
Who or what was the source?
```

## Current Owner

Temporary owner:

- `UDWGameInstance`

Important files:

- `Core/DWGameInstance.h`
- `Core/DWGameInstance.cpp`
- `Core/DWWorldEventTypes.h`

## Core Data

`FDWWorldEvent` currently stores:

- `EventId`
- `EventTag`
- `EncounterTag`
- `ThreatTag`
- `ContextTags`
- `LocationTag`
- `InstigatorTag`
- `SourceActorId`
- `GameDay`
- `GameHour`

## Current Flow

```text
Encounter starts
-> EncounterDirector creates started FDWWorldEvent

Combat outcome happens
-> EncounterOutcomeComponent reports outcome to EncounterDirector
-> EncounterDirector resolves active encounter
-> UDWGameInstance stores outcome FDWWorldEvent
```

## Query Pattern

Use generic record queries instead of specific story helpers.

Preferred:

```cpp
HasWorldEventRecordMatching(EventTag, EncounterTag, ThreatTag, LocationTag)
```

Avoid:

```cpp
HasPlayerKilledVampire()
HasPlayerDefeatedByWerewolf()
```

## Important Rule

Invalid query tags are wildcards.

Example:

```text
EventTag = World.Event.EnemyKilled
EncounterTag = invalid
ThreatTag = Threat.FeralVampire
LocationTag = invalid
```

Meaning:

```text
Has any EnemyKilled event happened involving FeralVampire threat?
```

## Future

- Move from `UDWGameInstance` to `UDWWorldEventSubsystem`.
- Add event recency queries.
- Add actor ids for instigator and target.
- Add event importance.
- Let Dialogue v2 query event records directly.
