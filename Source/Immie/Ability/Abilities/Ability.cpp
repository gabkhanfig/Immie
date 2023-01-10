// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability.h"
#include <Immie/Game/Global/Managers/AbilityDataManager.h>
#include <Immie/Immies/ImmieCharacter.h>
#include <Immie/Battle/BattleInstance/BattleInstance.h>
#include <Immie/Battle/Managers/BattleTypeManager.h>
#include <Immie/Battle/Managers/BattleAbilityManager.h>
#include <Immie/Battle/Managers/BattleSpecieManager.h>
#include <Immie/Battle/Team/BattleTeam.h>
#include <Immie/Game/Global/Managers/TypeDataManager.h>
#include <Immie/Game/Global/Managers/AbilityDataManager.h>
#include <Immie/Game/Global/Managers/SpecieDataManager.h>
#include <Immie/Type/ImmieType.h>
#include <Immie/Ability/Abilities/AbilityDataObject.h>
#include <Immie/Immies/ImmieObject.h>
#include <Kismet/GameplayStatics.h>
#include <Immie/Ability/Actor/AbilityActor.h>
#include <Immie/Controller/Player/ImmiePlayerController.h>

UAbility::UAbility()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);

	AbilityId = INVALID_ABILITY_ID;
	bInputHeld = false; 
	CurrentDelay = 0;
}

void UAbility::BeginPlay()
{
	Super::BeginPlay();
}

void UAbility::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) //i love you 
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

UAbility* UAbility::NewAbility(AImmieCharacter* Owner, int _AbilityId)
{
	UClass* AbilityClass = GetAbilityDataManager()->GetAbilityClass(_AbilityId);
	UAbility* Ability = NewObject<UAbility>(Owner, AbilityClass);
	Ability->AbilityId = _AbilityId;
	return Ability;
}

void UAbility::InitializeForBattle()
{
	AbilityDataObject = GetBattleInstance()->GetBattleAbilityManager()->GetAbilityDataObject(AbilityId);

	CurrentCooldown = AbilityDataObject->GetInitialCooldown();
	CurrentUses = AbilityDataObject->GetInitialUses();
	
	FAbilityFlags AbilityFlags = AbilityDataObject->GetAbilityFlags();

	if (AbilityFlags.SameTypeImmie) {
		Type = GetImmieCharacter()->GetType();
	}
	else {
		const int AbilityTypeBitmask = AbilityDataObject->GetTypeBitmask();
		Type = GetBattleInstance()->GetBattleTypeManager()->GetTypes(AbilityTypeBitmask);
	}

	const int Level = GetImmieCharacter()->GetImmieLevel();

	if (AbilityFlags.BaseStats) {
		const FBaseStats BaseStats = AbilityDataObject->GetBaseStats();
		const FBaseStats StatLevels = GetImmieCharacter()->GetImmieObject()->GetStatLevels();

		InitialStats.Health = UFormula::HealthStat(BaseStats.Health, Level, StatLevels.Health);
		InitialStats.Attack = UFormula::AttackStat(BaseStats.Attack, Level, StatLevels.Attack);
		InitialStats.Defense = UFormula::DefenseStat(BaseStats.Defense, Level, StatLevels.Defense);
		InitialStats.Speed = UFormula::SpeedStat(BaseStats.Speed, StatLevels.Speed);	
	}
	
	if (AbilityFlags.RelativeStats) {
		const FBattleStats RelativeMultipliers = AbilityDataObject->GetRelativeStats();

		InitialStats.Health = GetImmieCharacter()->GetInitialStats().Health * RelativeMultipliers.Health;
		InitialStats.Attack = GetImmieCharacter()->GetInitialStats().Attack * RelativeMultipliers.Attack;
		InitialStats.Defense = GetImmieCharacter()->GetInitialStats().Defense * RelativeMultipliers.Defense;
		InitialStats.Speed = GetImmieCharacter()->GetInitialStats().Speed * RelativeMultipliers.Speed;
	}
	
	ActiveStats = InitialStats;

	BP_InitializeForBattle(AbilityDataObject);
}

void UAbility::SyncToClients()
{
	SyncClientAbilityData(AbilityId, InitialStats, ActiveStats, Type, CurrentCooldown, CurrentUses, CurrentDelay);
	BP_SyncToClients();
}

void UAbility::InputPress()
{
	ExecuteInputPress();
	if (!HasBattleAuthority()) {
		ServerInputPress();
	}
}

void UAbility::SyncClientAbilityData_Implementation(int _AbilityId, FBattleStats _InitialStats, FBattleStats _ActiveStats, const TArray<UImmieType*>& _Type, float _CurrentCooldown, int _CurrentUses, float _CurrentDelay)
{
	AbilityId = _AbilityId;
	AbilityDataObject = GetBattleInstance()->GetBattleAbilityManager()->GetAbilityDataObject(AbilityId);
	InitialStats = _InitialStats;
	ActiveStats = _ActiveStats;
	Type = _Type;
	CurrentCooldown = _CurrentCooldown;
	CurrentUses = _CurrentUses;
	CurrentDelay = _CurrentDelay;
}

void UAbility::ExecuteInputPress()
{
	bInputHeld = true;
	BP_OnInputPress(HasBattleAuthority(), GetAbilityFlags());
}

void UAbility::ServerInputPress_Implementation()
{
	ExecuteInputPress();
	InformClientsInputPress();
}

void UAbility::InformClientsInputPress_Implementation()
{
	if (HasBattleAuthority()) {
		return;
	}

	// If this is multicasted back to the player who made the rpc originally, do not continue
	if (Cast<AController>(GetBattleInstance()->GetLocalPlayerController()) == GetImmieCharacter()->GetController()) {
		return;
	}

	ExecuteInputPress();
}

void UAbility::BP_OnInputPress_Implementation(bool HasBattleAuthority, FAbilityFlags AbilityFlags)
{
	if (HasBattleAuthority) {
		AAbilityActor* actor = SpawnAbilityActor(GetImmieCharacter()->GetTransform());
		//actor->DestroyAbilityActor();
	}
}

void UAbility::InputRelease()
{
	ExecuteInputRelease();
	if (!HasBattleAuthority()) {
		ServerInputRelease();
	}

}

void UAbility::ExecuteInputRelease()
{
	bInputHeld = false;
	BP_OnInputRelease(HasBattleAuthority(), GetAbilityFlags());
}

void UAbility::ServerInputRelease_Implementation()
{
	ExecuteInputRelease();
	InformClientsInputRelease();
}

void UAbility::InformClientsInputRelease_Implementation()
{
	if (HasBattleAuthority()) {
		return;
	}

	// If this is multicasted back to the player who made the rpc originally, do not continue
	if (Cast<AController>(GetBattleInstance()->GetLocalPlayerController()) == GetImmieCharacter()->GetController()) {
		return;
	}

	ExecuteInputRelease();
}

void UAbility::BP_OnInputRelease_Implementation(bool HasBattleAuthority, FAbilityFlags AbilityFlags)
{
}

AAbilityActor* UAbility::SpawnAbilityActor(const FTransform& SpawnTransform)
{
	return SpawnAbilityActorFromClass(GetActorClass(), SpawnTransform);
}

AAbilityActor* UAbility::SpawnAbilityActorFromClass(TSubclassOf<AAbilityActor> AbilityActorClass, const FTransform& SpawnTransform)
{
	return GetTeam()->SpawnAbilityActor(AbilityActorClass, this, SpawnTransform);
}

float UAbility::TimeDamageMultiplier_Implementation(float ElapsedTime) const
{
	return 1;
}

void UAbility::AuthorityBattleTick(float DeltaTime)
{
}

void UAbility::ClientBattleTick(float DeltaTime)
{
}

AImmieCharacter* UAbility::GetImmieCharacter() const
{
	//return Cast<AImmieCharacter>(GetOwner());
	// Avoid the overhead from above.
	return (AImmieCharacter*)GetOwner();
}

ABattleTeam* UAbility::GetTeam() const
{
	return IBattleActor::Execute_GetTeam(GetImmieCharacter());
}

ABattleInstance* UAbility::GetBattleInstance() const
{
	return GetTeam()->GetBattleInstance();
}

bool UAbility::HasBattleAuthority() const
{
	return GetTeam()->HasAuthority();
}

UAbilityDataObject* UAbility::GetAbilityDataObject() const
{
	return AbilityDataObject;
}

FName UAbility::GetAbilityName() const
{
	return GetAbilityDataObject()->GetAbilityName();
}

UClass* UAbility::GetActorClass() const
{
	return GetAbilityDataObject()->GetActorClass();
}

FAbilityFlags UAbility::GetAbilityFlags() const
{
	return GetAbilityDataObject()->GetAbilityFlags();
}

int UAbility::GetTypeBitmask() const
{
	return GetAbilityDataObject()->GetTypeBitmask();
}

float UAbility::GetInitialCooldown() const
{
	return GetAbilityDataObject()->GetInitialCooldown();
}

float UAbility::GetMaxCooldown() const
{
	return GetAbilityDataObject()->GetMaxCooldown();
}

int UAbility::GetInitialUses() const
{
	return GetAbilityDataObject()->GetInitialUses();
}

int UAbility::GetMaxUses() const
{
	return GetAbilityDataObject()->GetMaxUses();
}

float UAbility::GetDamagePower() const
{
	return GetAbilityDataObject()->GetDamagePower();
}

float UAbility::GetDamageDuration() const
{
	return GetAbilityDataObject()->GetDamageDuration();
}

float UAbility::GetHealingPower() const
{
	return GetAbilityDataObject()->GetHealingPower();
}

float UAbility::GetHealingDuration() const
{
	return GetAbilityDataObject()->GetHealingDuration();
}

float UAbility::GetSpeed() const
{
	return GetAbilityDataObject()->GetSpeed();
}

float UAbility::GetRange() const
{
	return GetAbilityDataObject()->GetRange();
}

float UAbility::GetMaxDelay() const
{
	return GetAbilityDataObject()->GetMaxDelay();
}

FBaseStats UAbility::GetBaseStats() const
{
	return GetAbilityDataObject()->GetBaseStats();
}

FBattleStats UAbility::GetRelativeStats() const
{
	return GetAbilityDataObject()->GetRelativeStats();
}

