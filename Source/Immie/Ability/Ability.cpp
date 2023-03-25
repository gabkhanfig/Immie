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
#include "AbilityDataObject.h"
#include <Immie/Immies/ImmieObject.h>
#include <Kismet/GameplayStatics.h>
#include "AbilityActor.h"
#include <Immie/Controller/Player/ImmiePlayerController.h>
#include "Camera/CameraComponent.h"

FTransform UAbility::GetAbilityActorSpawnTransform_Implementation() const
{
	FTransform SpawnTransform;
	SpawnTransform.SetTranslation(GetImmieCharacter()->GetActorLocation());
	SpawnTransform.SetRotation(GetImmieCharacter()->GetFollowCamera()->GetForwardVector().Rotation().Quaternion());
	SpawnTransform.SetScale3D(GetImmieCharacter()->GetActorScale());
	return SpawnTransform;
}

UAbility::UAbility()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);

	AbilityId = INVALID_ABILITY_ID;
	bInputHeld = false;
	CurrentCooldown = 0;
	CurrentUses = 0;
	HoldDuration = 0;

	bTypeSameAsImmie = false;
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

	if (bTypeSameAsImmie) {
		Type = IBattleActor::Execute_GetType(GetImmieCharacter());//GetImmieCharacter()->GetType();
	}
	else {
		const int AbilityTypeBitmask = AbilityDataObject->GetTypeBitmask();
		Type = GetBattleInstance()->GetBattleTypeManager()->GetTypes(AbilityTypeBitmask);
	}

	const int Level = GetImmieCharacter()->GetImmieLevel();

	//checkf(!(bCustomBaseStats && bRelativeStats), TEXT("Ability cannot have the bCustomBaseStats flag and bRelativeStats flag active simultaneously"))
	//if (bCustomBaseStats) {
	//	const FBaseStats BaseStats = AbilityDataObject->GetBaseStats();
	//	const FBaseStats StatLevels = GetImmieCharacter()->GetImmieObject()->GetStatLevels();

	//	InitialStats.Health = UFormula::HealthStat(BaseStats.Health, Level, StatLevels.Health);
	//	InitialStats.Attack = UFormula::AttackStat(BaseStats.Attack, Level, StatLevels.Attack);
	//	InitialStats.Defense = UFormula::DefenseStat(BaseStats.Defense, Level, StatLevels.Defense);
	//	InitialStats.Speed = UFormula::SpeedStat(BaseStats.Speed, StatLevels.Speed);	
	//}
	//
	//if (bRelativeStats) {
	//	const FBattleStats RelativeMultipliers = AbilityDataObject->GetRelativeStats();

	//	InitialStats.Health = GetImmieCharacter()->GetInitialStats().Health * RelativeMultipliers.Health;
	//	InitialStats.Attack = GetImmieCharacter()->GetInitialStats().Attack * RelativeMultipliers.Attack;
	//	InitialStats.Defense = GetImmieCharacter()->GetInitialStats().Defense * RelativeMultipliers.Defense;
	//	InitialStats.Speed = GetImmieCharacter()->GetInitialStats().Speed * RelativeMultipliers.Speed;
	//}
	//ActiveStats = InitialStats;

	BP_InitializeForBattle(AbilityDataObject);
}

void UAbility::SyncToClients()
{
	SyncClientAbilityData(AbilityId, InitialStats, ActiveStats, Type, CurrentCooldown, CurrentUses);
	BP_SyncToClients();
}

void UAbility::SyncClientAbilityData_Implementation(int _AbilityId, FBattleStats _InitialStats, FBattleStats _ActiveStats, const TArray<UImmieType*>& _Type, float _CurrentCooldown, int _CurrentUses)
{
	AbilityId = _AbilityId;
	AbilityDataObject = GetBattleInstance()->GetBattleAbilityManager()->GetAbilityDataObject(AbilityId);
	InitialStats = _InitialStats;
	ActiveStats = _ActiveStats;
	Type = _Type;
	CurrentCooldown = _CurrentCooldown;
	CurrentUses = _CurrentUses;
}

void UAbility::InputPress()
{
	//ExecuteInputPress();
	if (!HasBattleAuthority()) {
		ServerInputPress();
	}
	else {
		ExecuteInputPress();
	}
}

void UAbility::ExecuteInputPress()
{
	bInputHeld = true;
	HoldDuration = 0;
	BP_OnInputPress(HasBattleAuthority());
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
	//if (Cast<AController>(GetBattleInstance()->GetLocalPlayerController()) == GetImmieCharacter()->GetController()) {
	//	return;
	//}
	if (CanAbilityBeUsed()) {
		ExecuteInputPress();
	}
}

void UAbility::BP_OnInputPress_Implementation(bool HasBattleAuthority)
{
	if (!HasBattleAuthority) return;
	if (!CanAbilityBeUsed()) return;

	BP_UseAbility();
	DecrementUses();
}

void UAbility::InputRelease()
{
	if (!HasBattleAuthority()) {
		ServerInputRelease();
	}
	else {
		ExecuteInputRelease();
	}
}

void UAbility::ExecuteInputRelease()
{
	BP_OnInputRelease(HasBattleAuthority());
	bInputHeld = false;
	HoldDuration = 0;
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
	//if (Cast<AController>(GetBattleInstance()->GetLocalPlayerController()) == GetImmieCharacter()->GetController()) {
	//	return;
	//}

	ExecuteInputRelease();
}

void UAbility::BP_OnInputRelease_Implementation(bool HasBattleAuthority)
{
}

AAbilityActor* UAbility::SpawnAbilityActor(const FTransform& SpawnTransform)
{
	checkf(HasBattleAuthority(), TEXT("Ability actors can only be spawned on the server/standalone, the authoritative side."))
	return SpawnAbilityActorFromClass(GetActorClass(), SpawnTransform);
}

AAbilityActor* UAbility::SpawnAbilityActorFromClass(TSubclassOf<AAbilityActor> AbilityActorClass, const FTransform& SpawnTransform)
{
	return GetTeam()->SpawnAbilityActor(AbilityActorClass, this, SpawnTransform);
}

void UAbility::DecrementUses()
{
	SetCurrentUses(FMath::Max(0, CurrentUses - 1));
	SetCurrentCooldown(GetMaxCooldown());
}

void UAbility::StepCooldown(float DeltaTime)
{
	if (CurrentCooldown == 0) return;

	float NewCurrentCooldown = CurrentCooldown;

	NewCurrentCooldown -= DeltaTime;
	if (NewCurrentCooldown > 0) {
		SetCurrentCooldown(NewCurrentCooldown);
		return;
	}
	else {
		SetCurrentCooldown(0);
		SetCurrentUses(GetMaxUses());
	}
}

void UAbility::SetCurrentUses_Implementation(int NewCurrentUses)
{
	CurrentUses = NewCurrentUses;
}

void UAbility::SetCurrentCooldown_Implementation(float NewCurrentCooldown)
{
	CurrentCooldown = NewCurrentCooldown;
}

float UAbility::TimeDamageMultiplier_Implementation(float ElapsedTime) const
{
	return 1;
}

void UAbility::AuthorityBattleTick(float DeltaTime)
{
	if (bInputHeld) {
		HoldDuration += DeltaTime;
	}
	StepCooldown(DeltaTime);
	BP_AuthorityBattleTick(DeltaTime);
}

void UAbility::ClientBattleTick(float DeltaTime)
{
	if (bInputHeld) {
		HoldDuration += DeltaTime;
	}
	BP_ClientBattleTick(DeltaTime);
}

bool UAbility::CanAbilityBeUsed_Implementation() const
{
	if (CurrentUses > 0) {
		return true;
	}
	return false;
}

void UAbility::OnImmieCharacterDisable()
{
	bInputHeld = false;
	HoldDuration = 0;
	BP_OnImmieCharacterDisable();
}

void UAbility::EventPlayerDealtHealing_Implementation(const TScriptInterface<IBattleActor>& Target, float& Amount, FBattleDamage& Healing, bool IsOwningAbility)
{
}

void UAbility::EventPlayerDealtDamage_Implementation(const TScriptInterface<IBattleActor>& Target, float& Amount, FBattleDamage& Damage, bool IsOwningAbility)
{
}

AImmieCharacter* UAbility::GetImmieCharacter() const
{
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

