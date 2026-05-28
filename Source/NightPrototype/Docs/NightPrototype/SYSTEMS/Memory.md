# Memory

## Purpose

Memory stores facts known by a specific actor.

World events are global story records.

Memory is local to an actor.

## Current Owner

- `UDWMemoryComponent`
- `FDWMemoryEntry`

## Current Flow

```text
Actor experiences or is told something
-> AddMemoryByTag
-> Dialogue selector can require that memory
```

## Current Examples

Player/world-level facts currently often live as world tags:

- `Memory.Player.InspectedCorpse`
- `Memory.Player.SawVampire`
- `Memory.Player.DefeatedByVampire`
- `Memory.Player.KilledVampire`

NPC-local memory:

- `Memory.NPC.PlayerTalkedToMe`

Enemy-local memory:

- `Memory.Enemy.DefeatedPlayer`
- `Memory.Enemy.WasKilledByPlayer`

## Important Distinction

Use world events for things that happened in the world.

Use memory components for things an actor knows, remembers, believes, or personally experienced.

## Future

- Character Memory Component v0.2.
- Memory source actor.
- Memory strength decay.
- Witness-based memory propagation.
- Rumor propagation.
- Relationship changes derived from memory.
