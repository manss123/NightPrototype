# NPC

## Purpose

NPCs are world actors that can talk, remember, react, and later drive consequences.

NPC gameplay logic should live in C++.

Blueprint NPCs should primarily contain:

- mesh
- animation
- profile reference
- visual setup
- tuning values

## Current Owner

- `ADWNPCCharacter`
- `UDWNPCProfileDataAsset`
- `UDWNPCRegistrySubsystem`
- `UDWMemoryComponent`
- `UDWBodyHealthComponent`
- `UDWBodyStateRagdollComponent`
- `UDWDialogueSelectorLibrary`

## Current Flow

```text
Player right-clicks NPC
-> PlayerController moves into interact range
-> InteractWithOption(Talk)
-> NPC stores personal memory if needed
-> PlayerController asks ShouldShowDialogue
-> PlayerController asks GetDialogueText
-> NPC selects dialogue from profile DialogueBank
-> Dialogue widget displays text
```

## Current NPC Base

`ADWNPCCharacter` provides:

- Talk option.
- NPC profile provider.
- Registry registration.
- MemoryComponent.
- BodyHealthComponent.
- BodyStateRagdollComponent.
- DialogueBank selection through NPCProfile.
- World tags from GameInstance.
- Context tags from DayNightManager.

This means NPCs can now participate in combat consequences as baseline world actors.

Blueprint NPCs should not need to manually add health/ragdoll components unless they are using a custom non-`ADWNPCCharacter` base.

## Current Data Flow

```text
NPCProfile.DialogueBank
+ GameInstance WorldGameplayTags
+ NPC MemoryComponent
+ DayNight ContextTags
-> DialogueSelector
-> selected dialogue line
```

## Personal Memory

Current test memory:

- `Memory.NPC.PlayerTalkedToMe`

This proves NPC-local memory can drive dialogue.

## Important Rule

Global player/world facts should usually be world event records or world tags.

NPC personal facts should live in the NPC memory component.

## Combat / Victim Role

NPCs can now be used as encounter targets.

Current victim flow:

```text
EncounterDirector assigns NPC as target
-> EnemyAI attacks NPC
-> NPC BodyHealth receives damage
-> NPC death can resolve encounter as World.Event.VillagerKilled
```

Important tuning:

```text
bDownedInsteadOfDeathOnVitalZero = false
-> vital part zero means Dead

bDownedInsteadOfDeathOnVitalZero = true
-> vital part zero means Downed, useful for future rescue/carry systems
```

For current victim-death testing, use `false`.

For future rescue gameplay, `true` may be useful.

## Future

- NPC relationship graph integration.
- Household profile.
- Settlement registry integration.
- Dialogue v2 event-record conditions.
- NPC emotional state.
- NPC needs.
- Auto-generated NPC MVP.
