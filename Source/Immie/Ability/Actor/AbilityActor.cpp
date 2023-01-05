// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityActor.h"
#include <Immie/ImmieCore.h>
#include <Immie/Ability/Abilities/Ability.h>
#include <Immie/Ability/Abilities/AbilityDataObject.h>
#include <Immie/Battle/Components/DamageComponent.h>
#include <Net/UnrealNetwork.h>
#include "DummyAbilityActor.h"
#include <Immie/Battle/Team/BattleTeam.h>
#include <Kismet/GameplayStatics.h>

AAbilityActor::AAbilityActor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;
	bNetLoadOnClient = true;

	DamageComponent = CreateDefaultSubobject<UDamageComponent>(TEXT("DamageComponent"));
}

void AAbilityActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const 
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAbilityActor, SpawnedActiveStats);
	DOREPLIFETIME(AAbilityActor, ActiveStats);
}

void AAbilityActor::BeginPlay()
{
	Super::BeginPlay();
}

void AAbilityActor::InformSpawn_Implementation()
{
	const FAbilityFlags AbilityFlags = Ability->GetAbilityFlags();

	if (!IsRunningDedicatedServer()) {
		if (AbilityFlags.VisualDummy) {
			SpawnVisualDummy();
		}
	}
}

void AAbilityActor::SpawnVisualDummy()
{
	Dummy = CreateDummyActor();
}

ADummyAbilityActor* AAbilityActor::CreateDummyActor()
{
	UClass* DummyActorClass = Ability->GetAbilityDataObject()->GetDummyActorClass();
	check(IsValid(DummyActorClass));

	const FTransform SpawnTransform = GetDummySpawnTransform();

	// Uses team as owner to allow custom death animations / effects.
	ADummyAbilityActor* DummyActor = GetWorld()->SpawnActorDeferred<ADummyAbilityActor>
		(DummyActorClass, SpawnTransform, GetTeam(), nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	check(IsValid(DummyActor));
	DummyActor->SetAbilityActor(this);
	UGameplayStatics::FinishSpawningActor(DummyActor, SpawnTransform);
	return DummyActor;
}

FTransform AAbilityActor::GetDummySpawnTransform_Implementation()
{
	return GetTransform();
}

void AAbilityActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsRunningDedicatedServer()) {
		//iLog(IsValid(Ability) ? "ability component is valid on client tick" : "ability component is not valid on client tick");
	}
	
}

void AAbilityActor::SetAbility(UAbility* _Ability)
{
	Ability = _Ability;
}

void AAbilityActor::InitializeForBattle()
{
	SpawnedActiveStats = Ability->GetActiveStats();
	ActiveStats = SpawnedActiveStats;

	InformSpawn();
}

void AAbilityActor::OnActorChannelOpen(FInBunch& InBunch, UNetConnection* Connection)
{
	InBunch << Ability;
	InBunch << DamageComponent;
}

void AAbilityActor::OnSerializeNewActor(FOutBunch& OutBunch)
{
	OutBunch << Ability;
	OutBunch << DamageComponent;
}

UDamageComponent* AAbilityActor::GetDamageComponent_Implementation() const
{
	return DamageComponent;
}

int AAbilityActor::GetAbilityId() const
{
	return GetAbility()->GetAbilityId();
}

AImmieCharacter* AAbilityActor::GetImmieCharacter() const
{
	return GetAbility()->GetImmieCharacter();
}

ABattleTeam* AAbilityActor::GetTeam() const
{
	return GetAbility()->GetTeam();
}

ABattleInstance* AAbilityActor::GetBattleInstance() const
{
	return GetAbility()->GetBattleInstance();
}

bool AAbilityActor::HasBattleAuthority() const
{
	return GetAbility()->HasBattleAuthority();
}

UAbilityDataObject* AAbilityActor::GetAbilityDataObject() const
{
	return GetAbility()->GetAbilityDataObject();
}

FName AAbilityActor::GetAbilityName() const
{
	return GetAbility()->GetAbilityName();
}

FAbilityFlags AAbilityActor::GetAbilityFlags() const
{
	return GetAbility()->GetAbilityFlags();
}

float AAbilityActor::GetDamagePower() const
{
	return GetAbility()->GetDamagePower();
}

float AAbilityActor::GetDamageDuration() const
{
	return GetAbility()->GetDamageDuration();
}

float AAbilityActor::GetHealingPower() const
{
	return GetAbility()->GetHealingPower();
}

float AAbilityActor::GetSpeed() const
{
	return GetAbility()->GetSpeed();
}

float AAbilityActor::GetRange() const
{
	return GetAbility()->GetRange();
}

TArray<UImmieType*> AAbilityActor::GetType()
{
	return GetAbility()->GetType();
}

FBattleStats AAbilityActor::GetAbilityInitialStats() const
{
	return GetAbility()->GetInitialStats();
}

FBattleStats AAbilityActor::GetAbilityActiveStats() const
{
	return GetAbility()->GetActiveStats();
}

