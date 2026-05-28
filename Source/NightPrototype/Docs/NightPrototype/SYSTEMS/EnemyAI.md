# EnemyAI

## Purpose

Enemy AI currently provides a minimal playable threat loop.

The goal is not advanced AI yet. The goal is a readable sandbox encounter:

```text
Dormant
-> Revealed by encounter
-> Active
-> Receive target from encounter or detect player
-> Chase
-> Attack
-> Stop when target is invalid
```

## Current Owner

- `UDWEnemyAIComponent`
- `ADWEnemyCharacter`
- `ADWEncounterDirector`

## Current Features

- AI can start inactive.
- Enemy can start unrevealed.
- Unrevealed enemies are hidden, collision-disabled, and non-interactable.
- Hiding an enemy stops AI, movement, and auto attack.
- EncounterDirector reveals primary enemy.
- EncounterDirector can assign a target directly through `SetTarget()`.
- AI can be active without auto-acquiring the player.
- Enemy acquires player in detection range.
- Enemy chases player.
- Enemy starts auto attack in range.
- Current target can be read through `GetCurrentTarget()`.

## Important Settings

Enemy Blueprint:

- `bStartRevealed = false` for encounter-controlled enemies.
- `DWEnemyAIComponent.bStartActive = false` for encounter-controlled enemies.
- `DWEnemyAIComponent.bAutoAcquirePlayer = false` when the encounter should assign the target directly.

## Current States

`EDWEnemyAIState`:

- Idle
- Chasing
- Attacking

## Target Ownership

There are two valid ways for an enemy to get a target:

### Roaming / autonomous enemy

```text
bAutoAcquirePlayer = true
StartAI()
-> AI finds the player if the player is in detection range
```

### Encounter-directed enemy

```text
bAutoAcquirePlayer = false
EncounterDirector
-> SetEnemyRevealed(true)
-> EnemyAI.SetTarget(TargetActor)
-> AI chases or attacks that target immediately
```

This separation is important for sandbox encounters.

It allows future encounters to target:

- player
- victim NPC
- guard NPC
- another creature
- story-specific actor

## Known Design Meaning

Dormant/reveal is not just visual polish.

It lets the encounter system decide when an enemy becomes part of the story.

## Reveal Lifecycle

Encounter-controlled enemy setup:

```text
bStartRevealed = false
DWEnemyAIComponent.bStartActive = false
```

Runtime flow:

```text
Level loads
-> Enemy begins hidden and non-interactable
-> Enemy AI is inactive
-> Encounter starts
-> EncounterDirector calls SetEnemyRevealed(true)
-> EncounterDirector assigns target with SetTarget()
-> Enemy chases and attacks the assigned target
```

If `SetEnemyRevealed(false)` is called during runtime:

```text
Stop enemy AI
Stop movement
Stop auto attack
Hide actor
Disable collision
Disable interaction
```

## Future

- Guard idle.
- Patrol.
- Investigate.
- Flee.
- Wounded behavior.
- Group AI.
- Behavior tree only when the simple component approach is no longer enough.
