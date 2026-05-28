# ROADMAP

## Current Position

The project is currently between:

- Phase 6.5: Consequence System MVP
- Phase 9: Enemy AI Basic

The current priority is to make encounter behavior feel like an event-driven sandbox encounter before expanding the consequence system further.

## Completed MVPs

### Phase 1: Stabilize Interaction Core

- Right-click move.
- Hold right-click follow cursor.
- Right-click interact.
- Pending interact with timeout.
- Interaction prompt.
- Options menu.
- Dialogue widget.
- UI/input conflict handling.

### Phase 2: Interaction Options Menu

- Dynamic options from interactables.
- Option button selection returns action to C++.
- Menu/prompt conflict resolved.

### Phase 3: Object Types / World Interaction

- Crate.
- Notice board.
- Corpse.
- Door.
- Campfire.
- Interactable actors use reusable C++ base.

### Phase 3.5-3.13: Data, Memory, Dialogue, Registry, Enemy/Nemesis MVPs

- Gameplay tags.
- Dialogue bank.
- Dialogue selector.
- NPC profile.
- NPC registry.
- Relationship subsystem MVP.
- Enemy profile.
- Enemy roster.
- Enemy memory.
- Nemesis-lite state.

### Phase 4: Village Blockout

- `L_VillageBlockout`.
- Village layout.
- NavMesh.
- Lighting basics.
- Camera rotate debug.

### Phase 5: Day/Night + World State

- Day/night time.
- Day/night transitions.
- Sun/moon/sky light tuning.
- Context tags for night/first night.

### Phase 6: Threat Encounter Template MVP

- Encounter template DataAsset.
- Encounter instance.
- EncounterDirector.
- Started event record.
- Outcome event record.

### Phase 6.5: Consequence Foundation

- Combat outcomes become world event records.
- Event records include event, encounter, threat, location, context, source, day/hour.
- Generic record query exists in `UDWGameInstance`.
- NPC dialogue can react to world tags.
- NPC dialogue can react to precise world event records.
- Victim death can resolve an active encounter as `World.Event.VillagerKilled`.
- Victim downed can resolve an active encounter as `World.Event.VillagerTaken`.
- `VillagerTaken` can reveal follow-up clue actors such as a blood trail.

### Phase 7: Health / Damage / Downed State

- Body-part health.
- Healthy / Injured / Downed / Unconscious / Dying / Dead state ladder.
- Ragdoll on downed/death.
- Recovery and get-up.
- Recovery grace.

### Phase 8: Player Attack

- Right-click attack.
- Auto attack loop.
- Attack range.
- Attack speed tuning.
- Target validation.

### Phase 9: Enemy AI Basic

- Enemy detects player.
- Enemy chases.
- Enemy attacks.
- Enemy can start dormant.
- Encounter can reveal and activate primary enemy.
- Encounter can assign target directly to enemy AI.
- Enemy AI supports active-without-auto-acquire mode.
- Encounter target can be an NPC victim.

## Current Focus

Phase 9 polish:

- Enemy dormant/reveal/active lifecycle.
- Encounter-driven activation.
- Encounter-driven target assignment.
- Victim targeting MVP.
- Victim death outcome MVP.
- Victim downed/taken outcome MVP.
- Follow-up clue reveal MVP.
- Cleaner enemy behavior during encounters.
- Keep AI simple before adding patrol or behavior trees.

## Next Likely Steps

1. Add NPC dialogue lines using event + threat/location conditions.
2. Add inspect result/memory for follow-up clue objects.
3. Add simple idle/guard behavior after encounter outcomes are stable.
4. Later: multi-encounter victim delegate support.
5. Later: conversation topics / quest template integration.

## Design Constraints

- Keep systems generic.
- Do not hard-code FirstNight as a permanent dependency.
- Do not hard-code Vampire-only logic in core systems.
- A visual stand-in asset may represent a different story threat.
- Prefer event records over specific helper functions.
- Use the tech adoption guidance in `SYSTEMS/TechStack` before adding large Unreal systems.
