# NPC Generation

## Purpose

NPC generation exists to create people who belong to a world, not isolated random characters.

For this project, generated NPCs should support memory-driven dialogue, relationship-driven consequences, and future quest templates.

Core rule:

```text
NPCs should be generated with identity, household, role, relationship, faction pressure, and memory seeds.
```

Do not start with:

```text
Random name
Random face
Random job
Done
```

That creates population, but not drama.

## Why Relationships Matter

The game is an RPG sandbox where events should ripple through people.

Examples:

```text
A child is taken
-> parent reacts
-> sibling reacts
-> village leader reacts
-> witness reacts
-> quest hook appears
```

This requires NPC-to-NPC relationships.

## Relationship Graph

Think of NPCs as nodes and relationships as edges:

```text
Mara -- ParentOf --> Toma
Daren -- SpouseOf --> Mara
Ilya -- SiblingOf --> Toma
Elda -- FriendOf --> Mara
```

If Toma is taken by an enemy, the system can infer:

```text
Mara = strongest emotional reaction
Daren = anger / revenge potential
Ilya = fear / grief dialogue
Elda = clue / ritual / investigation hook
```

## First Relationship Types

Start small.

Good MVP types:

```text
ParentOf
ChildOf
SiblingOf
SpouseOf
FriendOf
RivalOf
```

Later expansion:

```text
MentorOf
OwesDebtTo
ProtectedBy
Hates
Fears
Serves
```

## Relationship Data

Relationships should have weight. A relationship is not only a label.

Suggested fields:

```text
SourceNPC
TargetNPC
RelationshipType
Strength
Trust
Fear
Anger
bPublicKnowledge
```

Examples:

```text
RelationshipType: ParentOf
Strength: 100
Trust: 90
Emotion: Love
```

```text
RelationshipType: SpouseOf
Strength: 45
Trust: 30
Emotion: Resentment
```

This lets the same event create different outcomes.

## NPC Identity

Minimum useful profile:

```text
NPCId
Name
Age
Race
SettlementId
FactionId
Role
Archetype
PersonalityTags
StatusTags
```

Example:

```text
Name: Toma
Age: 17
Race: Human
Settlement: Emberwall
Role: Water Carrier
Personality: Nervous, Kind
Faction: Human Settlement
```

## Household

NPCs should usually be generated as part of a household or social group.

Example:

```text
Household: Emberwall_House_03
- Mara, mother
- Daren, father
- Toma, son
- Ilya, younger sister
```

This gives events emotional reach.

## Social Role

Roles should matter to world consequence.

Useful early roles:

```text
Farmer
Guard
Water Carrier
Hunter
Herbalist
Blacksmith
Refugee
Child
Elder
Scout
Trader
Witch Apprentice
Hidden Dhampir
Vampire Thrall
```

Example:

```text
Water Carrier is taken
-> village water supply becomes a problem
-> rescue quest has practical value, not only emotional value
```

## Faction Ties

NPCs should be able to have public and hidden ties.

Example:

```text
PrimaryFaction: Emberwall
HiddenTie: VampireThrall
Fear: Hunters
DebtTo: Witch_Elda
```

This supports betrayal, suspicion, protection, and future faction politics.

## Personality

Use a small set first, but make traits affect reactions.

Good first traits:

```text
Brave
Coward
Protective
Greedy
Faithful
Suspicious
Kind
Cruel
Secretive
Desperate
```

Example:

```text
Brave + child taken
-> wants to help directly

Coward + child taken
-> begs player for help but refuses to leave safety

Suspicious + player fled
-> blames player more than other villagers do
```

## Memory Seeds

Generated NPCs should not always start empty.

Memory seeds provide history and future quest hooks.

Examples:

```text
LostFamilyToVampire
OwesDebtToWitch
SawWerewolfAsChild
SecretlyInfected
ServedHunterOrder
EscapedNoctisRuins
HearsMachineSignal
```

## NPC Tiers

Do not simulate every NPC equally.

## Tier 1: Story NPC

Deep profile, dialogue, memory, quest relevance.

Examples:

```text
Village elder
Witch
Hunter
Parent of victim
Important victim
```

## Tier 2: Simulated NPC

Has profile, household, role, and relationship. Dialogue can be light.

Examples:

```text
Guard
Trader
Farmer
Refugee
Sibling NPC
```

## Tier 3: Background NPC

Minimal identity only.

Examples:

```text
Market villager
Caravan worker
Background resident
```

## Template-Based Quests

Generated NPCs should feed quest templates, not freeform random quests.

Example: Missing Family

```text
Condition:
- NPC was taken
- another NPC has ParentOf / SiblingOf / SpouseOf relation
- relationship strength is high

Result:
- strongest related NPC becomes quest giver
```

Example:

```text
Toma is taken
Mara has ParentOf strength 100
Daren has ParentOf strength 75
Ilya has SiblingOf strength 80

Mara = main quest giver
Daren = revenge dialogue
Ilya = grief dialogue
```

## MVP Scope

For the first playable village:

```text
1 settlement
3 households
8 named NPCs
simple relationship graph
1 victim event
relationship-based dialogue reaction
1-2 quest templates later
```

Do not build a full NPC generator yet.

Use this document as the target shape when expanding `NPCProfile`, `RelationshipSubsystem`, `SettlementRegistry`, and future quest templates.

## Source Notes

Refined from `Feature - Unrefined.md`.
