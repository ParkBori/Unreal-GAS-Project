// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NativeGameplayTags.h"
/**
 * 
 */
namespace Quest
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Quest_MainStory_001);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Quest_MainStory_002);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Quest_MainStory_003);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Quest_Kill_Soldier_001);
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Quest_Location_001);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Quest_Interact_001);
}

namespace Item
{
	//UE_DECLARE_GAMEPLAY_TAG_EXTERN(Item_Equipment);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Item_Equipment_Armor_Head_001);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Item_Equipment_Armor_Cuirass_001);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Item_Equipment_Armor_Pants_001);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Item_Equipment_Armor_Mask_001);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Item_Equipment_Armor_Gloves_001);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Item_Equipment_Armor_Belt_001);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Item_Equipment_Armor_Boots_001);
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Item_Equipment_Weapon);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Item_Equipment_Weapon_Katana_001);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Item_Equipment_Weapon_GreateSword_001);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Item_Consumable_001);
}


namespace State
{
	// 전투 관련
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Invincible);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Attacking);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Blocking);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Dodging);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Parrying);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Groggy);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_DrinkingPotion);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Dead);
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Hitting);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_BlockReacting);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_ParryReactiing);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_ParryWindow);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_CancelWindow);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Cancelable);

	// 막을수 없는 치명적인 공격시 부여하는 태그
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Critical);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_SuperArmor);

	// AI 에서 공격을 감지하기 위한 태그
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Attack_Active);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_CanFinisher);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_ActivateFinisher);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_ActivateFinished);




	// 전투 어빌리티에 모두 부여
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Combat);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Targeted);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_WeaponActivated);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Debuffable);

	// AI 가 타겟을 가지고 있는지 확인 
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_HasTarget);
	
	// 점프
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Jump);
}

namespace Effect
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effect_Param);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effect_Param_Time);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effect_Param_DmgCoefficient);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effect_Param_Knockback);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effect_Param_DamageRate);

	
}

namespace Debuff
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effect_Debuff_Bleeding);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effect_Debuff_Burn);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effect_Debuff_Poison);
}


/**
* Evet_어빌리티 이름 (실제 어빌리티와 동일)
*/
namespace Event
{
	// 어빌리티 트리거
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Ability_Combat_ParryReaction);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Ability_Combat_ParriedReaction);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Ability_Combat_BlockReaction);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Ability_Combat_Hit);


	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_BlockEnd);

	// 적 몽타주 재생 트리거 
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Enemy_PlayNextMontage);

	// 공격판정 이벤트 트리거
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_StartAttackHitCheck);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_EndAttackHitCheck);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Ability_Finisher);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Ability_Finished);
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Ability_Death);


	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Ability_ItemActivationTime);
}

/**
* Ability_분야_어빌리티이름 (실제 어빌리티와 동일)
* 
* 어빌리티가 소유하는 태그
*/
namespace Ability
{
	// Combat Player
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Combat);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Combat_Block);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Combat_BlockReaction);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Combat_Dodge);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Combat_Hit);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Combat_ParryReaction);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Combat_ParriedReaction);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Combat_NormalAttack);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Combat_AvoidAndAttack);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Combat_BlockCrushAttack);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Combat_RushAttack);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Combat_Groggy);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Combat_Finisher);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Combat_Finished);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Combat_DrinkPotion);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Death);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Jump);

}

/**
* 서브 태그
*/
namespace Sub
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Left);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Right);
}

/**
* 큐 작동 태그
*/
namespace GameplayCue
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_Debuff_Bleeding);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_Debuff_Burn);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_Debuff_Poison);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_Parry);
}
