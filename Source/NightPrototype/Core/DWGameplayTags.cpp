// Fill out your copyright notice in the Description page of Project Settings.


#include "DWGameplayTags.h"

namespace DWGameplayTags
{
	// Encounter
	UE_DEFINE_GAMEPLAY_TAG(Encounter_VampireRaid, "Encounter.VampireRaid");
	UE_DEFINE_GAMEPLAY_TAG(Encounter_WerewolfHunt, "Encounter.WerewolfHunt");
	UE_DEFINE_GAMEPLAY_TAG(Encounter_ZombieWander, "Encounter.ZombieWander");

	// World Event
	UE_DEFINE_GAMEPLAY_TAG(World_Event_VampireRaid_Started, "World.Event.VampireRaid.Started");
	UE_DEFINE_GAMEPLAY_TAG(World_Event_VampireRaid_Resolved, "World.Event.VampireRaid.Resolved");
	UE_DEFINE_GAMEPLAY_TAG(World_Event_PlayerFled, "World.Event.PlayerFled");
	UE_DEFINE_GAMEPLAY_TAG(World_Event_PlayerDefeated, "World.Event.PlayerDefeated");
	UE_DEFINE_GAMEPLAY_TAG(World_Event_EnemyEscaped, "World.Event.EnemyEscaped");
	UE_DEFINE_GAMEPLAY_TAG(World_Event_EnemyKilled, "World.Event.EnemyKilled");
	UE_DEFINE_GAMEPLAY_TAG(World_Event_VillagerTaken, "World.Event.VillagerTaken");
	UE_DEFINE_GAMEPLAY_TAG(World_Event_VillagerSaved, "World.Event.VillagerSaved");
	UE_DEFINE_GAMEPLAY_TAG(World_Event_VillagerKilled, "World.Event.VillagerKilled");

	// Context
	UE_DEFINE_GAMEPLAY_TAG(Context_IntroEncounter, "Context.IntroEncounter");
	UE_DEFINE_GAMEPLAY_TAG(Context_FirstNight, "Context.FirstNight");
	UE_DEFINE_GAMEPLAY_TAG(Context_Night, "Context.Night");
	UE_DEFINE_GAMEPLAY_TAG(Context_RandomEncounter, "Context.RandomEncounter");
	UE_DEFINE_GAMEPLAY_TAG(Context_SettlementThreat, "Context.SettlementThreat");
	UE_DEFINE_GAMEPLAY_TAG(Context_PlayerWitnessed, "Context.PlayerWitnessed");
	UE_DEFINE_GAMEPLAY_TAG(Context_PlayerIgnored, "Context.PlayerIgnored");

	// Location
	UE_DEFINE_GAMEPLAY_TAG(Location_Emberwall, "Location.Emberwall");
	UE_DEFINE_GAMEPLAY_TAG(Location_Emberwall_Village, "Location.Emberwall.Village");
	UE_DEFINE_GAMEPLAY_TAG(Location_Emberwall_Outskirts, "Location.Emberwall.Outskirts");
	UE_DEFINE_GAMEPLAY_TAG(Location_Emberwall_ForestEdge, "Location.Emberwall.ForestEdge");
	UE_DEFINE_GAMEPLAY_TAG(Location_Emberwall_Cemetery, "Location.Emberwall.Cemetery");

	// Threat
	UE_DEFINE_GAMEPLAY_TAG(Threat_FeralVampire, "Threat.FeralVampire");
	UE_DEFINE_GAMEPLAY_TAG(Threat_WerewolfPack, "Threat.WerewolfPack");
	UE_DEFINE_GAMEPLAY_TAG(Threat_ZombieHorde, "Threat.ZombieHorde");

	// Memory
	UE_DEFINE_GAMEPLAY_TAG(Memory_Player_InspectedCorpse, "Memory.Player.InspectedCorpse");
	UE_DEFINE_GAMEPLAY_TAG(Memory_Player_SawVampire, "Memory.Player.SawVampire");
	UE_DEFINE_GAMEPLAY_TAG(Memory_Player_FledDuringAttack, "Memory.Player.FledDuringAttack");
	UE_DEFINE_GAMEPLAY_TAG(Memory_Player_DefeatedByVampire, "Memory.Player.DefeatedByVampire");
	UE_DEFINE_GAMEPLAY_TAG(Memory_Player_KilledVampire, "Memory.Player.KilledVampire");
	UE_DEFINE_GAMEPLAY_TAG(Memory_Player_SavedVictim, "Memory.Player.SavedVictim");
	UE_DEFINE_GAMEPLAY_TAG(Memory_Player_ReadNoticeBoard, "Memory.Player.ReadNoticeBoard");
	
	UE_DEFINE_GAMEPLAY_TAG(Memory_NPC_PlayerTalkedToMe, "Memory.NPC.PlayerTalkedToMe");
	
	UE_DEFINE_GAMEPLAY_TAG(Memory_Enemy_DefeatedPlayer, "Memory.Enemy.DefeatedPlayer");
	UE_DEFINE_GAMEPLAY_TAG(Memory_Enemy_WasDamagedByPlayer, "Memory.Enemy.WasDamagedByPlayer");
	UE_DEFINE_GAMEPLAY_TAG(Memory_Enemy_WasKilledByPlayer, "Memory.Enemy.WasKilledByPlayer");
	UE_DEFINE_GAMEPLAY_TAG(Memory_Enemy_WasScarredByPlayer, "Memory.Enemy.WasScarredByPlayer");
	UE_DEFINE_GAMEPLAY_TAG(Memory_Enemy_EscapedFromPlayer, "Memory.Enemy.EscapedFromPlayer");
	UE_DEFINE_GAMEPLAY_TAG(Memory_Enemy_PlayerSavedVictumFromMe, "Memory.Enemy.PlayerSavedVictimFromMe");
	// Nemesis
	UE_DEFINE_GAMEPLAY_TAG(Nemesis_None, "Nemesis.None");
	UE_DEFINE_GAMEPLAY_TAG(Nemesis_Candidate, "Nemesis.Candidate");
	UE_DEFINE_GAMEPLAY_TAG(Nemesis_Active, "Nemesis.Active");
	UE_DEFINE_GAMEPLAY_TAG(Nemesis_Escaped, "Nemesis.Escaped");
	UE_DEFINE_GAMEPLAY_TAG(Nemesis_Scarred, "Nemesis.Scarred");
	UE_DEFINE_GAMEPLAY_TAG(Nemesis_Returned, "Nemesis.Returned");
	UE_DEFINE_GAMEPLAY_TAG(Nemesis_Defeated, "Nemesis.Defeated");
	UE_DEFINE_GAMEPLAY_TAG(Nemesis_Dead, "Nemesis.Dead");
	
	// Enemy
	UE_DEFINE_GAMEPLAY_TAG(DWGameplayTags::Enemy_Vampire_Feral, "Enemy.Vampire.Feral");
	UE_DEFINE_GAMEPLAY_TAG(DWGameplayTags::Enemy_Undead_Zombie, "Enemy.Undead.Zombie");
	UE_DEFINE_GAMEPLAY_TAG(DWGameplayTags::Enemy_Beast_Werewolf, "Enemy.Beast.Werewolf");
	
	// Role
	UE_DEFINE_GAMEPLAY_TAG(DWGameplayTags::Role_Villager, "Role.Villager");
	UE_DEFINE_GAMEPLAY_TAG(DWGameplayTags::Role_Elder, "Role.Elder");
	UE_DEFINE_GAMEPLAY_TAG(DWGameplayTags::Role_Guard, "Role.Guard");
	UE_DEFINE_GAMEPLAY_TAG(DWGameplayTags::Role_Trader, "Role.Trader");
	
	// Body
	UE_DEFINE_GAMEPLAY_TAG(DWGameplayTags::Body_Head, "Body.Head");
	UE_DEFINE_GAMEPLAY_TAG(DWGameplayTags::Body_Torso, "Body.Torso");
	UE_DEFINE_GAMEPLAY_TAG(DWGameplayTags::Body_LeftArm, "Body.LeftArm");
	UE_DEFINE_GAMEPLAY_TAG(DWGameplayTags::Body_RightArm, "Body.RightArm");
	UE_DEFINE_GAMEPLAY_TAG(DWGameplayTags::Body_LeftLeg, "Body.LeftLeg");
	UE_DEFINE_GAMEPLAY_TAG(DWGameplayTags::Body_RightLeg, "Body.RightLeg");
}
