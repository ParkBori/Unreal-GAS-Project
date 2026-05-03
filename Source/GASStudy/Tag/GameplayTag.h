
#pragma once

#include "GameplayTagContainer.h"

#define TAG_ACTOR_ROATE FGameplayTag::RequestGameplayTag(FName("Actor.Action.Rotate"))
#define TAG_ACTOR_ROTATING FGameplayTag::RequestGameplayTag(FName("Actor.State.Rotating"))
#define TAG_DATA_DAMAGE FGameplayTag::RequestGameplayTag(FName("Data.Damage"))

#define TAG_NORMALATTACK FGameplayTag::RequestGameplayTag(FName("Character.Action.NormalAttack"))
#define TAG_CHARACTER_ACTION_DODGE FGameplayTag::RequestGameplayTag(FName("Character.Action.Dodge"))

#define TAG_CHARACTER_STATE_ISJUMPING FGameplayTag::RequestGameplayTag(FName("Character.State.IsJumping"))
#define TAG_CHARACTER_STATE_ISDASHING FGameplayTag::RequestGameplayTag(FName("Character.State.IsDashing"))
#define TAG_CHARACTER_STATE_ISATTACKING FGameplayTag::RequestGameplayTag(FName("Character.State.IsAttacking"))
#define TAG_CHARACTER_STATE_ISDEAD FGameplayTag::RequestGameplayTag(FName("Character.State.IsDead"))
#define TAG_CHARACTER_STATE_INVINSIBLE FGameplayTag::RequestGameplayTag(FName("Character.State.Invinsible"))
#define TAG_CHARACTER_STATE_IsBlocking FGameplayTag::RequestGameplayTag(FName("Character.State.IsBlocking"))
#define TAG_CHARACTER_STATE_IsParrying FGameplayTag::RequestGameplayTag(FName("Character.State.IsParrying"))
#define TAG_CHARACTER_STATE_ISDODGING FGameplayTag::RequestGameplayTag(FName("Character.State.IsDodging"))
#define TAG_CHARACTER_STATE_CANDODGE FGameplayTag::RequestGameplayTag(FName("Character.State.CanDodge"))

#define TAG_CHARACTER_ATTACK_CANBLOCK FGameplayTag::RequestGameplayTag(FName("Character.Attack.CanBlock"))
#define TAG_CHARACTER_ATTACK_CANPARRY FGameplayTag::RequestGameplayTag(FName("Character.Attack.CanParry"))

#define TAG_CHARACTER_ATTACK_COMBOATTACK FGameplayTag::RequestGameplayTag(FName("Character.Attack.ComboAttack"))
#define TAG_CHARACTER_ATTACK_COMMANDATTACK FGameplayTag::RequestGameplayTag(FName("Character.Attack.CommandAttack"))
#define TAG_CHARACTER_ATTACK_DASHDODGEATTACK FGameplayTag::RequestGameplayTag(FName("Character.Attack.DashDodgeAttack"))
#define TAG_CHARACTER_ATTACK_JUMPATTACK FGameplayTag::RequestGameplayTag(FName("Character.Attack.JumpAttack"))
#define TAG_CHARACTER_ATTACK_STINGATTACK FGameplayTag::RequestGameplayTag(FName("Character.Attack.StingAttack"))

#define TAG_CHARACTER_ANIMATION_STOPMOTION FGameplayTag::RequestGameplayTag(FName("Character.Animation.CanStopMotion"))

#define GAMEPLAYCUE_ATTACKHIT FGameplayTag::RequestGameplayTag(FName("GameplayCue.Character.AttackHit"))
#define GAMEPLAYCUE_CAMERASHAKE FGameplayTag::RequestGameplayTag(FName("GameplayCue.CameraShake"))

#define TAG_CHARACTER_ACTION_HITREACTION FGameplayTag::RequestGameplayTag(FName("Character.Action.HitReaction"))
#define TAG_CHARACTER_ACTION_PARRYREACTION FGameplayTag::RequestGameplayTag(FName("Character.Action.ParryReaction"))
#define TAG_CHARACTER_ACTION_PARRYEDREACTION FGameplayTag::RequestGameplayTag(FName("Character.Action.ParryedReaction"))
#define TAG_CHARACTER_ACTION_BLOCK FGameplayTag::RequestGameplayTag(FName("Character.Action.Block"))



#define TAG_CHARACTER_ACTION_DASH FGameplayTag::RequestGameplayTag(FName("Character.Action.Dash"))

#define	GAMEPLAYCUE_CHARACTER_ATTACKHIT FGameplayTag::RequestGameplayTag(FName("GameplayCue.Character.AttackHit"))


// 코드 구조 변경우 태그 위에 꺼는 수정해야함

#define	TAG_DAMGE_MELEE FGameplayTag::RequestGameplayTag(FName("Damage.Melee"))

#define	TAG_COMBAT_MELEE_BLOCK FGameplayTag::RequestGameplayTag(FName("Combat.Melee.Block"))
#define	TAG_COMBAT_MELEE_PARRY FGameplayTag::RequestGameplayTag(FName("Combat.Melee.Parry"))



#define	TAG_EVENT_PARRY FGameplayTag::RequestGameplayTag(FName("Event.Parry"))
#define	TAG_EVENT_PARRYED FGameplayTag::RequestGameplayTag(FName("Event.Parryed"))
#define	TAG_EVENT_BLOCK FGameplayTag::RequestGameplayTag(FName("Event.Block"))
#define	TAG_EVENT_BLOCKREINPUT FGameplayTag::RequestGameplayTag(FName("Event.BlockReInput"))



#define	TAG_CANENTER_ATTACK FGameplayTag::RequestGameplayTag(FName("CanEnterAttack"))



#define	TAG_EVENT_BLOCKEND FGameplayTag::RequestGameplayTag(FName("Event.BlockEnd"))
#define	TAG_EVENT_BLOCKDIRECTEND FGameplayTag::RequestGameplayTag(FName("Event.BlockDirectEnd"))



#define	TAG_COMBAT_MELEE_ATTACK_LIGHT FGameplayTag::RequestGameplayTag(FName("Combat.Melee.Attack.Light"))



#define TAG_EVENT_ATTACKREINPUT FGameplayTag::RequestGameplayTag(FName("Event.AttackReInput"))
#define TAG_EVENT_HIT FGameplayTag::RequestGameplayTag(FName("Event.Hit"))


#define TAG_EVENT_ENEMYATTACK FGameplayTag::RequestGameplayTag(FName("Event.EnemyAttack"))
#define TAG_EVENT_BLOCKINGDAMAGED FGameplayTag::RequestGameplayTag(FName("Event.BlockingDamaged"))




#define TAG_EVENT_DashStart FGameplayTag::RequestGameplayTag(FName("Event.DashStart"))
#define TAG_EVENT_DashEnd FGameplayTag::RequestGameplayTag(FName("Event.DashEnd"))
#define TAG_EVENT_ENEMYSPECIALATTACK FGameplayTag::RequestGameplayTag(FName("Event.EnemySpecialAttack"))
#define TAG_EVENT_SPAWNABILITY FGameplayTag::RequestGameplayTag(FName("Event.SpawAbility"))

#define TAG_EVENT_FINISHER FGameplayTag::RequestGameplayTag(FName("Event.Finisher"))
#define TAG_EVENT_FINISHERINPUT FGameplayTag::RequestGameplayTag(FName("Event.FinisherInput"))




#define TAG_COMBAT_HIT FGameplayTag::RequestGameplayTag(FName("Combat.Hit"))
#define TAG_COMBAT FGameplayTag::RequestGameplayTag(FName("Combat"))
#define TAG_COMBAT_MELEE FGameplayTag::RequestGameplayTag(FName("Combat.Melee"))

#define TAG_COMBAT_MELEE_SPECAILATTACK FGameplayTag::RequestGameplayTag(FName("Combat.Melee.SpecialAttack"))





#define TAG_ANIMATION_PARRYSUCCESSMOTION FGameplayTag::RequestGameplayTag(FName("Animation.ParrySuccessMotion"))
#define TAG_COMBAT_SUCCESSPARRY FGameplayTag::RequestGameplayTag(FName("Combat.SuccessParry"))

#define TAG_LOCOMOTION_DODGE FGameplayTag::RequestGameplayTag(FName("Locomotion.Dodge"))

#define TAG_ENEMY_COMBAT_SPECIALATTACK FGameplayTag::RequestGameplayTag(FName("Enemy.Combat.SpecialAttack"))

#define TAG_ENEMY_COMBAT_SPECIALATTACK FGameplayTag::RequestGameplayTag(FName("Enemy.Combat.SpecialAttack"))

#define TAG_PLAYER_COMBAT_FINISHER FGameplayTag::RequestGameplayTag(FName("Player.Combat.Finisher"))

#define TAG_PLAYER_RESURRECTION FGameplayTag::RequestGameplayTag(FName("Player.Resurrection"))

// Attacked 
#define TAG_ATTACKEDCOUNTER FGameplayTag::RequestGameplayTag(FName("AttackedCounter"))


// state
#define TAG_STATE_ATTACKING FGameplayTag::RequestGameplayTag(FName("State.Attacking"))
#define TAG_STATE_SPECIALATTACKING FGameplayTag::RequestGameplayTag(FName("State.SpecialAttacking"))
#define TAG_STATE_CANCOUNTER FGameplayTag::RequestGameplayTag(FName("State.CanCounter"))
#define TAG_STATE_DODGINE FGameplayTag::RequestGameplayTag(FName("State.Dodging"))
#define TAG_STATE_PARRYING FGameplayTag::RequestGameplayTag(FName("State.Parrying"))
#define TAG_STATE_BLOCKING FGameplayTag::RequestGameplayTag(FName("State.Blocking"))
#define TAG_STATE_BLOCK FGameplayTag::RequestGameplayTag(FName("State.Block"))
#define TAG_STATE_CANPARRY FGameplayTag::RequestGameplayTag(FName("State.CanParry"))
#define TAG_STATE_HIT FGameplayTag::RequestGameplayTag(FName("State.Hit"))
#define TAG_STATE_IGNOREHITREACTION FGameplayTag::RequestGameplayTag(FName("State.IgnoreHitReaction"))
#define TAG_STATE_TARGETED FGameplayTag::RequestGameplayTag(FName("State.Targeted"))
#define TAG_STATE_MAXPOSTURE FGameplayTag::RequestGameplayTag(FName("State.MaxPosture"))
#define TAG_STATE_DEATH FGameplayTag::RequestGameplayTag(FName("State.Death"))
#define TAG_STATE_FINISHED FGameplayTag::RequestGameplayTag(FName("State.Finished"))


// Test
#define TAG_EVENT_TESTEVENT FGameplayTag::RequestGameplayTag(FName("Event.TestEvent"))


// Item
#define TAG_ITEM_EQUIPMENT FGameplayTag::RequestGameplayTag(FName("Item.Equipment"))
#define TAG_ITEM_INVENTORY FGameplayTag::RequestGameplayTag(FName("Item.Inventory"))

#define TAG_ITEM_EQUIPMENT_WEAPON FGameplayTag::RequestGameplayTag(FName("Item.Equipment.Weapon"))

#define TAG_ITEM_EQUIPMENT_ARMOR_CURIASS	FGameplayTag::RequestGameplayTag(FName("Item.Equipment.Armor.Cuirass"))
#define TAG_ITEM_EQUIPMENT_ARMOR_FULLBODY	FGameplayTag::RequestGameplayTag(FName("Item.Equipment.Armor.FullBody"))
#define TAG_ITEM_EQUIPMENT_ARMOR_HEAD		FGameplayTag::RequestGameplayTag(FName("Item.Equipment.Armor.Head"))
#define TAG_ITEM_EQUIPMENT_ARMOR_MASK		FGameplayTag::RequestGameplayTag(FName("Item.Equipment.Armor.Mask"))
#define TAG_ITEM_EQUIPMENT_ARMOR_PANTS		FGameplayTag::RequestGameplayTag(FName("Item.Equipment.Armor.Pants"))

#define TAG_ITEM_EQUIPMENT_ARMOR_BELT		FGameplayTag::RequestGameplayTag(FName("Item.Equipment.Armor.Belt"))
#define TAG_ITEM_EQUIPMENT_ARMOR_BOOTS		FGameplayTag::RequestGameplayTag(FName("Item.Equipment.Armor.Boots"))
#define TAG_ITEM_EQUIPMENT_ARMOR_GLOVES		FGameplayTag::RequestGameplayTag(FName("Item.Equipment.Armor.Gloves"))
