# Tech Stack

## Purpose

This document defines which Unreal Engine tools fit NightPrototype and when they should be used.

Core rule:

```text
Use proven Unreal systems to make NPCs, enemies, memory, dialogue, and encounters feel alive.
Do not use real machine learning for the current prototype.
```

The goal is not technical novelty.

The goal is:

```text
data-driven behavior
debuggable systems
small maintainable prototype steps
event/story-driven sandbox gameplay
```

## Current Project Direction

NightPrototype should prioritize:

```text
Gameplay Tags
Data Assets
Data Tables when useful
simple C++ components
World Event records
Memory components
Dialogue selector
Enemy roster
Encounter director
clear debug tools
```

Blueprint should mainly hold:

```text
asset references
DataAsset references
tuning values
visual setup
temporary debug
```

C++ should hold:

```text
gameplay rules
state transitions
validation logic
selection logic
combat rules
encounter rules
memory and dialogue query logic
```

## Use Now

## Gameplay Tags

Gameplay Tags are the backbone of data-driven logic.

Use for:

```text
World.Event.*
Memory.*
Encounter.*
Threat.*
Location.*
Context.*
Faction.*
Body.*
Damage.*
Weakness.*
```

Current project already uses tags for:

```text
world events
memory
dialogue conditions
encounter context
threat identity
location identity
body parts
```

Design rule:

```text
Prefer tags and event records over hard-coded booleans.
```

## Data Assets

Use Data Assets for structured, authored data.

Good fits:

```text
Dialogue Bank
NPC Profile
Enemy Profile
Encounter Template
Enemy Archetype
Spawn Rule later
Nemesis Return Rule later
Settlement Preset later
```

Current project already uses Data Assets for:

```text
DialogueBank
NPCProfile
EnemyProfile
EncounterTemplate
```

Design rule:

```text
Use Data Assets when data has structure, references, and designer-tuned fields.
```

## Data Tables

Use Data Tables only when the data is broad and table-shaped.

Good future fits:

```text
name pools
generic bark lines
enemy taunt pools
memory reaction line pools
loot lists
simple spawn weights
```

Do not force current structured assets into Data Tables.

## C++ Components

Use C++ components for reusable gameplay behavior.

Current good examples:

```text
UDWMemoryComponent
UDWBodyHealthComponent
UDWBodyStateRagdollComponent
UDWCombatComponent
UDWEnemyAIComponent
UDWEncounterOutcomeComponent
```

Design rule:

```text
If many Blueprints need the same gameplay behavior, move it into a C++ component or base class.
```

## Navigation System / NavMesh

Use NavMesh for normal click-to-move, chase, flee, patrol, and village movement.

Current use:

```text
player click-to-move
enemy chase
interaction approach
```

Future use:

```text
villager flee
guard patrol
return to lair
rescue target movement
```

Design rule:

```text
Use NavMesh for intentional navigation.
Keep direct cursor-follow movement only for specific player-control needs.
```

## Use Soon

## Gameplay Debugger

High-value next debug tool.

Suggested custom categories:

```text
DW Memory
DW Dialogue
DW Encounter
DW Enemy
DW NPC
```

Target debug view when looking at an NPC:

```text
NPCId
Profile
Current memories
World event matches
Selected dialogue line
Current encounter role
Health state
```

Why useful:

```text
The project is becoming data-driven.
Data-driven bugs are hard to see without runtime inspection.
```

## Data Validation

Use when Dialogue Banks, Profiles, Encounter Templates, and Tags grow.

Good validation rules:

```text
DialogueLine must have LineId.
Dialogue line should have text.
Dialogue condition lines should not accidentally have no conditions and high priority.
NPCProfile must have NPCId.
EnemyProfile must have EnemyId.
EncounterTemplate must have EncounterId and EncounterGameplayTag.
EncounterTemplate with PrimaryEnemyProfile should resolve to a registered enemy in test maps.
Relationship edge should not reference missing NPC ids.
```

Do not build validation before data volume becomes painful.

But add it before content authoring becomes large.

## AI Perception

Use after the current simple enemy AI loop is stable.

Good first senses:

```text
Sight
Hearing
Damage
```

Use cases:

```text
enemy sees player
villager sees corpse
hunter hears combat
enemy reacts to being damaged
werewolf detects territory intrusion later
```

Current project does not need full perception yet because encounter-directed targeting already works.

## StateTree

Use StateTree for high-level AI behavior when simple C++ AI starts to branch too much.

Good future states:

```text
Villager:
Idle
Work
HideAtNight
Panic
ReactToEvent
TalkToPlayer

Enemy:
Dormant
Lurk
Stalk
Chase
Attack
Retreat
ReturnToLair
```

Recommended timing:

```text
After enemy/victim outcome loop is stable.
Before complex NPC routines or vampire stalk/ambush behavior.
```

## EQS

Use EQS when AI needs to choose positions intelligently.

Good future queries:

```text
find hiding spot
find ambush spot
find retreat point
find line-of-sight attack position
find safe villager flee location
```

Do not use EQS yet for basic chase/attack.

## Smart Objects

Use Smart Objects when village routine starts.

Good examples:

```text
well
bed
campfire
work bench
watch tower
shrine
market stall
gate lever
corpse burn pit
hiding spot
```

Design value:

```text
The world advertises usable actions.
NPC logic does not need to hard-code every destination.
```

Recommended timing:

```text
After NPC relationship/dialogue/event reaction is useful.
Before full daily schedules.
```

## Use Later

## Behavior Tree + Blackboard

Use for tactical combat only when combat decisions outgrow simple C++ or StateTree.

Possible future use:

```text
flank
dodge
retreat
call allies
choose melee/ranged/special attack
guard investigation
```

Current recommendation:

```text
Do not start with Behavior Tree.
Use simple C++ AI and later StateTree first.
```

## Gameplay Ability System

Useful later if combat/status effects become deep.

Possible future effects:

```text
fire damage
silver damage
UV burn
bleeding
infection
fear
curse
regeneration
stamina effects
```

Current recommendation:

```text
Use lightweight Damage/Status components first.
Consider GAS only when effect stacking and ability rules become too complex.
```

## Mass Entity / Mass Gameplay

Use only for large crowds or large-scale simulation.

Potential later use:

```text
large city background population
zombie hordes
caravan crowd simulation
background NPC LOD
large faction movement
```

Current recommendation:

```text
Do not use Mass for Emberwall-scale prototype.
Actors and components are enough.
```

## Visual Logger

Use later or when AI behavior becomes hard to debug from on-screen messages.

Good for:

```text
AI target decisions
perception events
chosen EQS points
encounter activation
memory changes
spawn director choices
```

Gameplay Debugger is probably the better first debug investment for this project.

## Suggested Tool Adoption Order

Current and near-term order:

```text
1. Gameplay Tags
2. Data Assets
3. C++ gameplay components
4. World Event records
5. Dialogue selector with event-record conditions
6. Gameplay Debugger categories
7. Data Validation
8. AI Perception
9. StateTree
10. Smart Objects
11. EQS
12. Behavior Tree / Blackboard if needed
13. GAS if effects become complex
14. Mass only for large-scale simulation
```

## What Not To Do Now

Avoid:

```text
real machine learning
large behavior trees
full NPC daily routine system
Mass Entity for small village NPCs
GAS before status effects need it
full faction simulation
procedural world tech
large quest generation framework
```

These can all distract from the current proof:

```text
event happens
world records it
NPC remembers/reacts
enemy remembers/reacts
player sees consequence
```

## Current Best Next Tool Investment

The strongest near-term tool investment is:

```text
Gameplay Debugger category for DW systems
```

Suggested first category:

```text
DW Dialogue
```

Show:

```text
selected NPC
NPC profile id
current dialogue line id
world event tags
matching event record condition
memory tags
context tags
```

This will help debug the exact system currently being expanded.

## Source Notes

Refined from `Tools-Techs - Unrefined.md`.
