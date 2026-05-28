# Encounter

## Purpose

Encounters are reusable story/event templates.

An encounter is not "First Night logic". First Night is only one possible trigger context.

The same encounter type should be startable from:

- Intro scenario.
- Random encounter.
- Settlement threat.
- Player action.
- Location tension.

## Current Owner

- `ADWEncounterDirector`
- `UDWEncounterTemplateDataAsset`
- `FDWEncounterTemplate`
- `FDWEncounterInstance`
- `UDWEncounterOutcomeComponent`

## Current Flow

```text
DayNightManager
-> EncounterDirector.EvaluateEncountersWithGameplayContextAndTime
-> Choose eligible EncounterTemplate
-> Start EncounterInstance
-> Store started FDWWorldEvent
-> Find primary enemy through EnemyRoster
-> Reveal primary enemy
-> Choose default encounter target
-> Assign target to enemy AI
-> Store victim actor on EncounterInstance
-> Bind victim outcome if target has BodyHealth
-> Reveal configured follow-up actors when specific outcome happens
```

## Enemy Activation Flow

Encounter-controlled enemies should not start active by default.

Recommended enemy setup:

```text
bStartRevealed = false
DWEnemyAIComponent.bStartActive = false
```

When the encounter starts:

```text
EncounterDirector
-> Find primary enemy actor from EnemyRoster
-> Store PrimaryEnemyActor on FDWEncounterInstance
-> SetEnemyRevealed(true)
-> GetDefaultEncounterTarget()
-> EnemyAI.SetTarget(TargetActor)
```

This means the encounter owns the timing of when the threat becomes part of the story.

It also means encounter enemies do not need to auto-detect the player.

Recommended setup for story/encounter enemies:

```text
bStartRevealed = false
DWEnemyAIComponent.bStartActive = false
DWEnemyAIComponent.bAutoAcquirePlayer = false
```

If no target is available, the director can still fall back to `StartAI()`.

## Encounter Targeting

`GetDefaultEncounterTarget()` currently uses `DefaultEncounterTargetActor` if assigned.

If no explicit target actor is assigned, it falls back to the player character.

This is intentionally isolated in one helper so future encounter target selection can evolve without rewriting enemy activation.

Future target selection may use:

- player character
- victim NPC
- nearest villager
- settlement guard
- actor selected by the encounter template
- actor selected by location/story context

## Victim Targeting MVP

The current prototype supports a simple victim targeting flow:

```text
EncounterDirector.DefaultEncounterTargetActor = NPC victim
-> Encounter starts
-> Primary enemy is revealed
-> EnemyAI.SetTarget(victim)
-> Enemy chases and attacks victim
```

If the target actor has `UDWBodyHealthComponent`, the director can bind to the target's `OnDeath` delegate.

Current victim outcome mapping:

```text
Target OnDeath
-> ResolveActiveEncounter(..., World.Event.VillagerKilled)
-> Store outcome FDWWorldEvent

Target OnDowned
-> ResolveActiveEncounter(..., World.Event.VillagerTaken)
-> Store outcome FDWWorldEvent
```

The active instance stores:

```text
FDWEncounterInstance.PrimaryEnemyActor
FDWEncounterInstance.VictimActor
```

The director still keeps a weak pointer to the currently bound target delegate, but resolution now matches against `Instance.VictimActor` before applying the outcome.

## Follow-Up Reveal MVP

The current prototype can reveal follow-up actors when a victim is taken.

Current use:

```text
World.Event.VillagerTaken
-> Reveal ActorsToRevealOnVillagerTaken
-> Example: BP_BloodTrail_Emberwall becomes visible/interactable
```

This is not a full quest system.

It is a lightweight story hook:

```text
victim taken
-> NPC dialogue changes
-> clue object appears
-> player has something to inspect next
```

The reveal is configured through the EncounterDirector details panel, not hard-coded to one Blueprint.

## Resolve Flow

```text
BodyHealth OnDowned / OnDeath
-> EncounterOutcomeComponent
-> EncounterDirector.ResolveActiveEncounter
-> Apply nemesis outcome
-> Store outcome FDWWorldEvent
```

## Template Data

`FDWEncounterTemplate` contains:

- `EncounterId`
- `DisplayName`
- `PrimaryEnemyProfile`
- `EncounterGameplayTag`
- `StartedEventGameplayTag`
- `LocationGameplayTag`
- `ThreatGameplayTag`
- required/blocked world tags
- context tags
- one-shot and priority settings

## Instance Data

`FDWEncounterInstance` stores runtime state:

- instance id
- encounter id
- primary enemy id
- primary enemy actor
- victim actor
- encounter tag
- started event tag
- outcome tag
- location tag
- threat tag
- context tags
- start day/hour
- resolved state

## Current Meaning Of Primary Enemy

`PrimaryEnemyActor` means the main actor of this encounter instance.

It does not mean a location can only have one enemy.

Future encounters may include:

- primary threat
- supporting enemies
- victim NPC
- witnesses
- escape target

## Current Debug Visibility

When debug messages are enabled, activation can show:

- active encounter
- primary enemy actor
- activated enemy actor
- assigned encounter target
- bound encounter victim
- follow-up actors revealed by outcome

## Important Design Rule

Encounter core logic must not hard-code:

- FirstNight
- Vampire only
- one specific map
- one specific enemy Blueprint

## Future

- Supporting enemy list.
- Spawn director integration.
- Victim NPC selection.
- Data-driven follow-up outcome rules.
- Multi-encounter victim delegate support.
- Group outcome resolution.
- Encounter cooldown.
- Encounter recency and priority rules.
