# Dialogue

## Purpose

Dialogue presents world state, memories, and consequences to the player.

Dialogue should be data-driven through DialogueBank DataAssets.

## Current Owner

- `UDWDialogueBankDataAsset`
- `FDWDialogueLine`
- `UDWDialogueSelectorLibrary`
- `ADWNPCCharacter`
- `UDWDialogueWidget`

## Current Dialogue Conditions

`FDWDialogueLine` currently supports:

- `RequiredWorldTags`
- `RequiredMemoryTags`
- `RequiredContextTags`
- `RequiredWorldEventTag`
- `RequiredEncounterTag`
- `RequiredThreatTag`
- `RequiredLocationTag`
- `Priority`

## Current Selection Rule

The selector chooses the matching line with the highest priority.

This supports:

- normal line
- night line
- inspected corpse line
- player defeated line
- enemy killed line
- villager killed line
- villager taken line
- precise event-record lines filtered by event / encounter / threat / location

## Event Record Conditions

Dialogue can now query structured `FDWWorldEvent` records through `UDWGameInstance::HasWorldEventRecordMatching`.

If none of the event-record condition fields are set, the line behaves like a normal line.

If any event-record condition field is set, the selector requires a matching world event record.

Examples:

```text
RequiredWorldEventTag = World.Event.VillagerKilled
-> matches any villager killed event
```

```text
RequiredWorldEventTag = World.Event.VillagerKilled
RequiredThreatTag = Threat.FeralVampire
-> matches only villager killed events involving that threat
```

```text
RequiredWorldEventTag = World.Event.VillagerTaken
RequiredLocationTag = Location.Emberwall
-> matches villager taken events in Emberwall or child locations if the record location matches by parent tag
```

## Current Limitation

Dialogue still selects one best line by priority.

There is not yet a conversation tree, topic system, speaker emotional state, or one-shot dialogue consumption.

Those should come later, after event-record dialogue proves useful in the demo loop.
