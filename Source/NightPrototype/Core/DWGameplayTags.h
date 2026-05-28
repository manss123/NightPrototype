// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NativeGameplayTags.h"

namespace DWGameplayTags
{
	// Encounter
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Encounter_VampireRaid);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Encounter_WerewolfHunt);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Encounter_ZombieWander);
	
	// World Event
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(World_Event_VampireRaid_Started);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(World_Event_VampireRaid_Resolved);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(World_Event_VampireRaid_PlayerFled);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(World_Event_PlayerDefeated);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(World_Event_EnemyEscaped);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(World_Event_EnemyKilled);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(World_Event_VillagerTaken);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(World_Event_VillagerSaved);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(World_Event_VillagerKilled);
	
	// Context
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Context_IntroEncounter);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Context_FirstNight);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Context_Night);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Context_RandomEncounter);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Context_SettlementThreat);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Context_PlayerWitnessed);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Context_PlayerIgnored);
	
	// Location
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Location_Emberwall);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Location_Emberwall_Village);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Location_Emberwall_Outskirts);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Location_Emberwall_ForestEdge);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Location_Emberwall_Cemetery);
	
	// Threat
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Threat_FeralVampire);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Threat_WerewolfPack);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Threat_ZombieHorde);
	
	// Memory
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Memory_Player_InspectedCorpse);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Memory_Player_SawVampire);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Memory_Player_FledDuringAttack);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Memory_Player_DefeatedByVampire);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Memory_Player_KilledVampire);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Memory_Player_SavedVictim);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Memory_Player_ReadNoticeBoard);
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Memory_NPC_PlayerTalkedToMe);
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Memory_Enemy_DefeatedPlayer);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Memory_Enemy_WasDamagedByPlayer);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Memory_Enemy_WasKilledByPlayer);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Memory_Enemy_WasScarredByPlayer);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Memory_Enemy_EscapedFromPlayer);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Memory_Enemy_PlayerSavedVictimFromMe);
	
	// Nemesis
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Nemesis_None);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Nemesis_Candidate);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Nemesis_Active);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Nemesis_Escaped);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Nemesis_Scarred);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Nemesis_Returned);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Nemesis_Defeated);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Nemesis_Dead);
	
	// Enemy
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Vampire_Feral);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Undead_Zombie);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Beast_Werewolf);
	
	// Role
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Role_Villager);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Role_Elder);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Role_Guard);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Role_Trader);
	
	// Body
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Body_Head);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Body_Torso);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Body_LeftArm);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Body_RightArm);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Body_LeftLeg);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Body_RightLeg);
}
