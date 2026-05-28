# Enemy Generation

## Purpose

Enemy generation should create threats with identity, origin, territory, behavior, weakness, and memory potential.

Core rule:

```text
Do not generate an enemy as only a monster to fight.
Generate a threat that belongs to the world.
```

## Difference From NPC Generation

NPC generation focuses on:

```text
household
role
relationship
fear
need
quest hook
```

Enemy generation focuses on:

```text
race
subtype
faction
territory
rank
behavior
weakness
hunting pattern
memory potential
nemesis potential
```

## Example Identity

Bad:

```text
Enemy_FeralVampire_01
```

Better:

```text
Name: Red-Mouthed Veyr
Race: Vampire
Subtype: Feral Low Blood
Faction: Carrion Strain
Territory: Old Drain near Emberwall
Hunting Pattern: Ambush lone humans
Weakness: Fire
Trait: Cruel, Hungry, Territorial
Nemesis Eligible: true
Memory Seed: EscapedHunterRaid
```

## Enemy Tiers

## Tier 0: Trash / Swarm Enemy

No persistent identity needed.

Examples:

```text
generic zombie
feral dead
small mutated creature
basic minion
```

Data:

```text
Race
Level
SpawnZone
BasicBehavior
Loot
```

## Tier 1: Named Enemy

Has name, trait, weakness, and small memory.

Examples:

```text
Feral vampire that attacks a village
Werewolf scout
Hunter deserter
Cult preacher
Machine-risen guard
```

## Tier 2: Nemesis Candidate

Can remember the player and return.

Examples:

```text
enemy that defeated the player
enemy that escaped wounded
enemy connected to an important NPC
enemy whose pack/faction was harmed by the player
```

Extra data:

```text
PersonalMemory
Scar / Injury
Grudge
Fear
Respect
EncounterHistory
ReturnRules
```

## Tier 3: Faction Boss / Story Enemy

Mostly handcrafted, but still data-driven.

Examples:

```text
Vampire Noble
Werewolf Pack Alpha
Hunter Inquisitor
Machine Priest
Revenant House Lord
AI Chimera Boss
```

## Generation Pipeline

Recommended long-term flow:

```text
Choose biome
-> Choose dominant faction / threat
-> Choose enemy race
-> Choose enemy archetype
-> Assign rank / tier
-> Assign territory / lair
-> Assign traits
-> Assign weakness / resistance
-> Assign behavior pattern
-> Assign memory seeds
-> Assign nemesis eligibility
-> Register to enemy roster
```

## Enemy Profile Shape

Suggested fields:

```text
EnemyId
Name
Race
Subtype
Faction
Rank
Biome
TerritoryId
LairId
Archetype
PersonalityTags
CombatTags
WeaknessTags
ResistanceTags
MemoryTags
NemesisState
SpawnRules
ReturnRules
```

## Archetypes

Archetypes keep generation controlled.

## Vampire Archetypes

```text
Night Ambusher
Blood Drunk
Stalker
Noble Agent
Scarred Revenant Vampire
```

## Werewolf Archetypes

```text
Pack Scout
Territory Guardian
Blood Frenzy Wolf
Alpha Challenger
```

## Undead Archetypes

```text
Hollow Remnant
Bound Dead
Revenant
Machine-Risen
```

## Human Enemy Archetypes

```text
Bandit Raider
Hunter Purist
Vampire Thrall
Cult Recruiter
Relic Scavenger
Desperate Villager
```

Humans can be enemies too. They should not be treated as a separate design category from threat encounters.

## Enemy Relationships

Enemies do not need deep relationship data for every case, but important enemies should connect to the world.

## Enemy-to-Faction

```text
Serves
ControlledBy
RejectedBy
HuntedBy
RivalOf
```

Example:

```text
Feral vampire is secretly used by a Noble Vampire house.
```

## Enemy-to-Enemy

Useful for packs, clans, gangs, and command structures.

```text
PackMemberOf
CreatedBy
Serves
RivalOf
```

Example:

```text
Player kills a young werewolf
-> Pack Alpha gains Memory.PlayerKilledPackMember
```

## Enemy-to-NPC

This is high-value for story-driven sandbox play.

Examples:

```text
Feral vampire was once a missing villager
Revenant was once an NPC's child
Blood thrall is still the spouse of someone in town
Broken wolf was once a hunter
```

This turns combat into emotional consequence.

## Enemy Memory Seeds

Good early examples:

```text
EscapedHunterRaid
KilledVillageGuard
WasOnceHuman
RejectedByVampireHouse
LostPackMember
CreatedInOldLab
HearsMachineSignal
BoundToOldCommand
AfraidOfFire
ScarredBySilver
```

Memory seeds can affect:

```text
dialogue
behavior
weakness response
quest hooks
nemesis rules
```

## Nemesis Eligibility

Not every enemy should become a nemesis.

Good triggers:

```text
enemy defeated player
enemy escaped after being injured
player killed someone important to enemy
enemy is connected to an important NPC
enemy has enough rank
enemy has suitable personality
enemy belongs to a revenge-capable faction
```

## Enemy Roster vs Spawn Director

Keep these separate.

## Enemy Roster

Persistent named enemies that exist in the world.

Examples:

```text
Red-Mouthed Veyr
Old Scar Wolf
Brother Halen the Burned
Machine-Risen Subject 12
```

## Spawn Director

Decides who appears and when.

Example:

```text
Night in Ashwake
-> choose generic feral vampire

If Red-Mouthed Veyr is ActiveNemesis
-> choose Veyr instead
```

## MVP Scope

For the first demo:

```text
1 generic feral vampire threat
1 named persistent threat
enemy profile data
enemy memory tags
2 nemesis triggers
1 return encounter later
```

Do not build full procedural enemy generation yet.

The immediate goal is to make one persistent enemy feel remembered.

## Source Notes

Refined from `Feature - Unrefined.md`.
