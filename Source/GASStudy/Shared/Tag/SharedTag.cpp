// Fill out your copyright notice in the Description page of Project Settings.


#include "Shared/Tag/SharedTag.h"


namespace Quest
{
	// Quest
	UE_DEFINE_GAMEPLAY_TAG(Quest_MainStory_001, "Quest.MainStory.001");
	UE_DEFINE_GAMEPLAY_TAG(Quest_MainStory_002, "Quest.MainStory.002");
	UE_DEFINE_GAMEPLAY_TAG(Quest_MainStory_003, "Quest.MainStory.003");

	UE_DEFINE_GAMEPLAY_TAG(Quest_Kill_Soldier_001, "Quest.Kill.Soldier.001");

	UE_DEFINE_GAMEPLAY_TAG(Quest_Location_001, "Quest.Location.001");
	
	UE_DEFINE_GAMEPLAY_TAG(Quest_Interact_001, "Quest.Interact.001");
}

namespace Item
{
	//UE_DEFINE_GAMEPLAY_TAG(Item_Equipment, "Item.Equipment");

	UE_DEFINE_GAMEPLAY_TAG(Item_Equipment_Armor_Head_001, "Item.Equipment.Armor.Head.001");
	UE_DEFINE_GAMEPLAY_TAG(Item_Equipment_Armor_Cuirass_001, "Item.Equipment.Armor.Cuirass.001");
	UE_DEFINE_GAMEPLAY_TAG(Item_Equipment_Armor_Pants_001, "Item.Equipment.Armor.Pants.001");
	UE_DEFINE_GAMEPLAY_TAG(Item_Equipment_Armor_Mask_001, "Item.Equipment.Armor.Mask.001");
	UE_DEFINE_GAMEPLAY_TAG(Item_Equipment_Armor_Gloves_001, "Item.Equipment.Armor.Gloves.001");
	UE_DEFINE_GAMEPLAY_TAG(Item_Equipment_Armor_Belt_001, "Item.Equipment.Armor.Belt.001");
	UE_DEFINE_GAMEPLAY_TAG(Item_Equipment_Armor_Boots_001, "Item.Equipment.Armor.Boots.001");
	
	
	UE_DEFINE_GAMEPLAY_TAG(Item_Equipment_Weapon, "Item.Equipment.Weapon");
	UE_DEFINE_GAMEPLAY_TAG(Item_Equipment_Weapon_Katana_001, "Item.Equipment.Weapon.Katana.001");
	UE_DEFINE_GAMEPLAY_TAG(Item_Equipment_Weapon_GreateSword_001, "Item.Equipment.Weapon.GreateSword.001");

	UE_DEFINE_GAMEPLAY_TAG(Item_Consumable_001, "Item.Consumable.001");
}

namespace State
{
	UE_DEFINE_GAMEPLAY_TAG(State_Invincible, "State.Invincible");
	UE_DEFINE_GAMEPLAY_TAG(State_Attacking, "State.Attacking");
	UE_DEFINE_GAMEPLAY_TAG(State_Blocking, "State.Blocking");
	UE_DEFINE_GAMEPLAY_TAG(State_Dodging, "State.Dodging");
	UE_DEFINE_GAMEPLAY_TAG(State_Parrying, "State.Parrying");
	UE_DEFINE_GAMEPLAY_TAG(State_Groggy, "State.Groggy");
	UE_DEFINE_GAMEPLAY_TAG(State_DrinkingPotion, "State.DrinkingPotion");
	UE_DEFINE_GAMEPLAY_TAG(State_Dead, "State.Dead");

	UE_DEFINE_GAMEPLAY_TAG(State_Hitting, "State.Hitting");
	UE_DEFINE_GAMEPLAY_TAG(State_BlockReacting, "State.BlockReacting");
	UE_DEFINE_GAMEPLAY_TAG(State_ParryReactiing, "State.ParryReactiing");

	
	UE_DEFINE_GAMEPLAY_TAG(State_ParryWindow, "State.ParryWindow");
	UE_DEFINE_GAMEPLAY_TAG(State_CancelWindow, "State.CancelWindow");
	UE_DEFINE_GAMEPLAY_TAG(State_Cancelable, "State.Cancelable");

	UE_DEFINE_GAMEPLAY_TAG(State_Critical, "State.Critical");

	UE_DEFINE_GAMEPLAY_TAG(State_SuperArmor, "State.SuperArmor");

	UE_DEFINE_GAMEPLAY_TAG(State_Attack_Active, "State.Attack.Active");

	UE_DEFINE_GAMEPLAY_TAG(State_CanFinisher, "State.CanFinisher");
	
	UE_DEFINE_GAMEPLAY_TAG(State_ActivateFinisher, "State.ActivateFinisher");
	UE_DEFINE_GAMEPLAY_TAG(State_ActivateFinished, "State.ActivateFinished");

	UE_DEFINE_GAMEPLAY_TAG(State_Combat, "State.Combat");

	UE_DEFINE_GAMEPLAY_TAG(State_Targeted, "State.Targeted");
	UE_DEFINE_GAMEPLAY_TAG(State_WeaponActivated, "State.WeaponActivated");
	UE_DEFINE_GAMEPLAY_TAG(State_Debuffable, "State.Debuffable");

	UE_DEFINE_GAMEPLAY_TAG(State_HasTarget, "State.HasTarget");

	UE_DEFINE_GAMEPLAY_TAG(State_Jump, "State.Jump");
}

namespace Effect
{
	// Effect 공용 세팅
	UE_DEFINE_GAMEPLAY_TAG(Effect_Param, "Effect.Param");
	UE_DEFINE_GAMEPLAY_TAG(Effect_Param_Time, "Effect.Param.Time");
	UE_DEFINE_GAMEPLAY_TAG(Effect_Param_DmgCoefficient, "Effect.Param.DmgCoefficient");
	UE_DEFINE_GAMEPLAY_TAG(Effect_Param_Knockback, "Effect.Param.Knockback");
	UE_DEFINE_GAMEPLAY_TAG(Effect_Param_DamageRate, "Effect.Param.DamageRate");
}

namespace Debuff
{
	UE_DEFINE_GAMEPLAY_TAG(Effect_Debuff_Bleeding, "Effect.Debuff.Bleeding");
	UE_DEFINE_GAMEPLAY_TAG(Effect_Debuff_Burn, "Effect.Debuff.Burn");
	UE_DEFINE_GAMEPLAY_TAG(Effect_Debuff_Poison, "Effect.Debuff.Poison");
}

namespace Event
{
	UE_DEFINE_GAMEPLAY_TAG(Event_BlockEnd, "Event.BlockEnd");


	UE_DEFINE_GAMEPLAY_TAG(Event_Ability_Combat_ParryReaction, "Event.Ability.Combat.ParryReaction");
	UE_DEFINE_GAMEPLAY_TAG(Event_Ability_Combat_ParriedReaction, "Event.Ability.Combat.ParriedReaction");
	UE_DEFINE_GAMEPLAY_TAG(Event_Ability_Combat_BlockReaction, "Event.Ability.Combat.BlockReaction");
	UE_DEFINE_GAMEPLAY_TAG(Event_Ability_Combat_Hit, "Event.Ability.Combat.Hit");

	UE_DEFINE_GAMEPLAY_TAG(Event_Enemy_PlayNextMontage, "Event.Enemy.PlayNextMontage");

	UE_DEFINE_GAMEPLAY_TAG(Event_StartAttackHitCheck, "Event.StartAttackHitCheck");
	UE_DEFINE_GAMEPLAY_TAG(Event_EndAttackHitCheck, "Event.EndAttackHitCheck");

	UE_DEFINE_GAMEPLAY_TAG(Event_Ability_Finisher, "Eent.Ability.Finisher");
	UE_DEFINE_GAMEPLAY_TAG(Event_Ability_Finished, "Event.Ability.Finished");

	UE_DEFINE_GAMEPLAY_TAG(Event_Ability_Death, "Event.Ability.Death");

	UE_DEFINE_GAMEPLAY_TAG(Event_Ability_ItemActivationTime, "Event.Ability.ItemActivationTime");
}

namespace Ability
{
	UE_DEFINE_GAMEPLAY_TAG(Ability_Combat, "Ability.Combat");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Combat_Block, "Ability.Combat.Block");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Combat_BlockReaction, "Ability.Combat.BlockReaction");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Combat_Dodge, "Ability.Combat.Dodge");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Combat_Hit, "Ability.Combat.Hit");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Combat_ParryReaction, "Ability.Combat.ParryReaction");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Combat_ParriedReaction, "Ability.Combat.ParriedReaction");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Combat_NormalAttack, "Ability.Combat.NormalAttack");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Combat_AvoidAndAttack, "Ability.Combat.AvoidAndAttack");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Combat_BlockCrushAttack, "Ability.Combat.BlockCrushAttack");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Combat_RushAttack, "Ability.Combat.RushAttack");

	UE_DEFINE_GAMEPLAY_TAG(Ability_Combat_Groggy, "Ability.Combat.Groggy");

	UE_DEFINE_GAMEPLAY_TAG(Ability_Combat_Finisher, "Ability.Combat.Finisher");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Combat_Finished, "Ability.Combat.Finished");

	UE_DEFINE_GAMEPLAY_TAG(Ability_Death, "Ability.Death");

	UE_DEFINE_GAMEPLAY_TAG(Ability_Combat_DrinkPotion, "Ability.Combat.DrinkPotion");

	UE_DEFINE_GAMEPLAY_TAG(Ability_Jump, "Ability.Jump");

}

namespace Sub
{
	UE_DEFINE_GAMEPLAY_TAG(Left, "Left");
	UE_DEFINE_GAMEPLAY_TAG(Right, "Right");
}

namespace GameplayCue
{
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Debuff_Bleeding, "GameplayCue.Debuff.Bleeding");
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Debuff_Burn, "GameplayCue.Debuff.Burn");
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Debuff_Poison, "GameplayCue.Debuff.Poison");

	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Parry, "GameplayCue.Parry");

}
