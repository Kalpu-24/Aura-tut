// Copyright 2025 Kalp Games, All rights reserved.


#include "AuraGameplayTags.h"

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Attributes_Primary_Strength, "Attributes.Primary.Strength", "Increases Physical Damage")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Attributes_Primary_Intelligence, "Attributes.Primary.Intelligence", "Increases Magical Damage")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Attributes_Primary_Resilience, "Attributes.Primary.Resilience", "Increases Armor and Armor Penetration")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Attributes_Primary_Vigor, "Attributes.Primary.Vigor", "Increases Health")

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Attributes_Secondary_Armor, "Attributes.Secondary.Armor", "Reduces Damage Taken, Improves Block Chance")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Attributes_Secondary_ArmorPenetration, "Attributes.Secondary.ArmorPenetration", "Ignore a portion of the target's armor, Increasing Hit Chance")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Attributes_Secondary_BlockRate, "Attributes.Secondary.BlockRate", "Chance to block incoming damage in half")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Attributes_Secondary_CritRate, "Attributes.Secondary.CritRate", "Chance to deal double damage, critical hit bonus")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Attributes_Secondary_CritDamage, "Attributes.Secondary.CritDamage", "Bonus damage dealt on critical hits")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Attributes_Secondary_CritRes, "Attributes.Secondary.CritRes", "Reduces the chance of receiving critical hits")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Attributes_Secondary_HealthRegen, "Attributes.Secondary.HealthRegen", "Amount of health regenerated per second")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Attributes_Secondary_ManaRegen, "Attributes.Secondary.ManaRegen", "Amount of mana regenerated per second")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Attributes_Secondary_MaxHealth, "Attributes.Secondary.MaxHealth", "Maximum amount of health obtainable")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Attributes_Secondary_MaxMana, "Attributes.Secondary.MaxMana", "Maximum amount of mana obtainable")

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Attributes_Resistance_Fire, "Attributes.Resistance.Fire", "Fire Resistance tag")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Attributes_Resistance_Arcane, "Attributes.Resistance.Arcane", "Arcane Resistance tag")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Attributes_Resistance_Lightning, "Attributes.Resistance.Lightning", "Lightning Resistance tag")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Attributes_Resistance_Physical, "Attributes.Resistance.Physical", "Physical Resistance tag")

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Attributes_Meta_IncomingXP, "Attributes.Meta.IncomingXP", "Incoming XP tag")

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_InputTag_LMB, "InputTag.LMB", "Input Tag for Left Mouse Button")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_InputTag_RMB, "InputTag.RMB", "Input Tag for Right Mouse Button")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_InputTag_1, "InputTag.1", "Input Tag for 1 Key")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_InputTag_2, "InputTag.2", "Input Tag for 2 Key")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_InputTag_3, "InputTag.3", "Input Tag for 3 Key")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_InputTag_4, "InputTag.4", "Input Tag for 4 Key")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_InputTag_Passive_1, "InputTag.Passive.1", "Input Tag for Passive 1")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_InputTag_Passive_2, "InputTag.Passive.2", "Input Tag for Passive 2")

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Damage, "Damage", "Damage tag for set by caller use")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_DamageType_Fire, "DamageType.Fire", "Fire DamageType tag for set by caller use")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_DamageType_Arcane, "DamageType.Arcane", "Arcane DamageType tag for set by caller use")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_DamageType_Lightning, "DamageType.Lightning", "Lightning DamageType tag for set by caller use")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_DamageType_Physical, "DamageType.Physical", "Physical DamageType tag for set by caller use")

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Abilities_Attack, "Abilities.Attack", "Attack Ability tag")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Abilities_Summon, "Abilities.Summon", "Summon Ability tag")

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Abilities_HitReact, "Abilities.HitReact", "Hit React Ability tag")

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Abilities_Status_Locked, "Abilities.Status.Locked", "Locked Ability tag")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Abilities_Status_Eligible, "Abilities.Status.Eligible", "Eligible Ability tag")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Abilities_Status_Unlocked, "Abilities.Status.Unlocked", "Unlocked Ability tag")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Abilities_Status_Equipped, "Abilities.Status.Equipped", "Equipped Ability tag")

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Abilities_Types_Offensive, "Abilities.Types.Offensive", "Offensive Ability tag")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Abilities_Types_Passive, "Abilities.Types.Passive", "Passive Ability tag")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Abilities_Types_None, "Abilities.Types.None", "None Ability tag")

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Abilities_Fire_Firebolt, "Abilities.Fire.Firebolt", "Firebolt Ability tag")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Abilities_Lightning_Electrocute, "Abilities.Lightning.Electrocute", "Electrocute Ability tag")

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Cooldown_Fire_Firebolt, "Cooldown.Fire.Firebolt", "Firebolt Cooldown tag")

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Montage_Attack_1, "Montage.Attack.1", "Montage Attack 1 tag")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Montage_Attack_2, "Montage.Attack.2", "Montage Attack 2 tag")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Montage_Attack_3, "Montage.Attack.3", "Montage Attack 3 tag")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Montage_Attack_4, "Montage.Attack.4", "Montage Attack 4 tag")

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_CombatSocket_Weapon, "CombatSocket.Weapon", "Montage Attack Weapon tag")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_CombatSocket_RightHand, "CombatSocket.RightHand", "Montage Attack RightHand tag")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_CombatSocket_LeftHand, "CombatSocket.LeftHand", "Montage Attack LeftHand tag")
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_CombatSocket_Tail, "CombatSocket.Tail", "Montage Attack Tail tag")

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Effect_HitReact, "Effect.HitReact", "Tag for Hit React Effect")