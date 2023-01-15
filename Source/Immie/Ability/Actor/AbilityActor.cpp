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
#include <Immie/Immies/ImmieCharacter.h>
#include <Immie/Type/ImmieType.h>

AAbilityActor::AAbilityActor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;
	bNetLoadOnClient = true;
}

void AAbilityActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const 
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

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

void AAbilityActor::AddAbilityCollider(UPrimitiveComponent* AbilityCollider)
{
	AbilityColliders.Add(AbilityCollider);
}

void AAbilityActor::OnCollision(UPrimitiveComponent* ThisOverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherActorComponent)
{
	if (!HasBattleAuthority()) {
		return;
	}

	const bool Implements = OtherActor->GetClass()->ImplementsInterface(UBattleActor::StaticClass());
	if (!Implements) {
		return;
	}

	TScriptInterface<IBattleActor> BattleActor = OtherActor;
	const bool IsValidAbilityCollider = BattleActor->IsValidAbilityCollider(OtherActorComponent);
	if (!IsValidAbilityCollider) {
		return;
	}

	OnBattleActorCollision(BattleActor, ThisOverlappedComponent, OtherActorComponent);
}

void AAbilityActor::OnBattleActorCollision_Implementation(const TScriptInterface<IBattleActor>& BattleActor, UPrimitiveComponent* ThisComponent, UPrimitiveComponent* OtherComponent)
{
	if (IsAlly(BattleActor)) {
		OnAllyCollision(BattleActor, ThisComponent, OtherComponent);
	}
	else {
		OnEnemyCollision(BattleActor, ThisComponent, OtherComponent);
	}
}

void AAbilityActor::OnAllyCollision_Implementation(const TScriptInterface<IBattleActor>& Ally, UPrimitiveComponent* ThisComponent, UPrimitiveComponent* OtherComponent)
{
	const FAbilityFlags AbilityFlags = GetAbilityFlags();
	if (!AbilityFlags.DoesHealing) {
		return;
	}

	if (Ally->CanBeHealedByAbilityActor(this)) {
		return;
	}

	UDamageComponent* AllyDamageComponent = Ally->GetDamageComponent();
	if (!IsValid(AllyDamageComponent)) {
		iLog("Invalid ally damage component in AAbilityActor::OnAllyCollision().", LogVerbosity_Error);
		return;
	}

	FAbilityInstigatorDamage Healing;
	Healing.Instigator = GetAbility();
	Healing.InstigatorType = GetAbility()->GetType();
	Healing.Power = GetHealingPower();

	if (AbilityFlags.BaseStats || AbilityFlags.RelativeStats) {
		Healing.AttackerStat = this->GetActiveStats().Attack;
	}
	else {
		Healing.AttackerStat = GetImmieCharacter()->GetActiveStats().Attack;
	}
	
	Healing.DefenderStat = Ally->GetActiveStats().Defense;
	Healing.DefenderType = Ally->GetType();
	Healing.Duration = GetAbility()->GetHealingDuration();
	Healing.InstigatorLevel = GetImmieCharacter()->GetImmieLevel();
	Healing.Multiplier = 1;
	AllyDamageComponent->AddHealing(Healing);

	DestroyAbilityActor();
}

void AAbilityActor::OnEnemyCollision_Implementation(const TScriptInterface<IBattleActor>& Enemy, UPrimitiveComponent* ThisComponent, UPrimitiveComponent* OtherComponent)
{
	const FAbilityFlags AbilityFlags = GetAbilityFlags();
	if (!AbilityFlags.DoesDamage) {
		return;
	}

	if (!Enemy->CanBeDamagedByAbilityActor(this)) {
		return;
	}

	UDamageComponent* EnemyDamageComponent = Enemy->GetDamageComponent();
	if (!IsValid(EnemyDamageComponent)) {
		iLog("Invalid enemy damage component in AAbilityActor::OnEnemyCollision().", LogVerbosity_Error);
		return;
	}

	FAbilityInstigatorDamage Damage;
	Damage.Instigator = GetAbility();
	Damage.InstigatorType = GetAbility()->GetType();
	Damage.Power = GetDamagePower();

	if (AbilityFlags.BaseStats || AbilityFlags.RelativeStats) {
		Damage.AttackerStat = this->GetActiveStats().Attack;
	}
	else {
		Damage.AttackerStat = GetImmieCharacter()->GetActiveStats().Attack;
	}

	Damage.DefenderStat = Enemy->GetActiveStats().Defense;
	Damage.DefenderType = Enemy->GetType();
	Damage.Duration = GetAbility()->GetHealingDuration();
	Damage.InstigatorLevel = GetImmieCharacter()->GetImmieLevel();
	Damage.Multiplier = 1;
	EnemyDamageComponent->AddDamage(Damage);

	DestroyAbilityActor();
}

void AAbilityActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAbilityActor::DestroyAbilityActor()
{
	GetTeam()->RemoveAbilityActor(this);
}

void AAbilityActor::OnAbilityActorDestroy_Implementation()
{
	ABattleTeam* _Team = GetTeam();
	checkf(IsValid(_Team), TEXT("Team is not valid when ability actor is attempting to be destroyed."));
	if (IsValid(Dummy)) {
		Dummy->OnAbilityActorDestroy();
	}
	BP_OnAbilityActorDestroy();
}

void AAbilityActor::SetAbility(UAbility* _Ability)
{
	Ability = _Ability;
}

void AAbilityActor::SetSpawnedActiveStats(FBattleStats _SpawnedActiveStats)
{
	SpawnedActiveStats = _SpawnedActiveStats;
	ActiveStats = SpawnedActiveStats;
}

void AAbilityActor::SetDamageComponent(UDamageComponent* _DamageComponent)
{
	DamageComponent = _DamageComponent;
}

void AAbilityActor::InitializeForBattle()
{
	ActiveStats = SpawnedActiveStats;

	InformSpawn();
}

void AAbilityActor::OnActorChannelOpen(FInBunch& InBunch, UNetConnection* Connection)
{
	InBunch << Ability;
	InBunch << DamageComponent;
	InBunch << SpawnedActiveStats.Health;
	InBunch << SpawnedActiveStats.Attack;
	InBunch << SpawnedActiveStats.Defense;
	InBunch << SpawnedActiveStats.Speed;
}

void AAbilityActor::OnSerializeNewActor(FOutBunch& OutBunch)
{
	OutBunch << Ability;
	OutBunch << DamageComponent;
	OutBunch << SpawnedActiveStats.Health;
	OutBunch << SpawnedActiveStats.Attack;
	OutBunch << SpawnedActiveStats.Defense;
	OutBunch << SpawnedActiveStats.Speed;
}

UDamageComponent* AAbilityActor::GetDamageComponent() const
{
	return DamageComponent;
}

bool AAbilityActor::IsValidAbilityCollider(UPrimitiveComponent* Collider) const
{
	return AbilityColliders.Contains(Collider);
}

bool AAbilityActor::CanBeHealedByAbilityActor(AAbilityActor* AbilityActor) const
{
	return true;
}

bool AAbilityActor::CanBeDamagedByAbilityActor(AAbilityActor* AbilityActor) const
{
	return true;
}

ABattleTeam* AAbilityActor::GetTeam() const
{
	return GetAbility()->GetTeam();
}

void AAbilityActor::AuthorityBattleTick(float DeltaTime)
{
}

void AAbilityActor::ClientBattleTick(float DeltaTime)
{
}

void AAbilityActor::IncreaseHealth(float Amount)
{
	checkf(Amount >= 0, TEXT("Increasing battle actor health by negative value is not allowed"));
	ActiveStats.Health += Amount;
	if (ActiveStats.Health > GetAbility()->GetInitialStats().Health) {
		ActiveStats.Health = GetAbility()->GetInitialStats().Health;
	}
}

void AAbilityActor::DecreaseHealth(float Amount)
{
	checkf(Amount >= 0, TEXT("Decreasing battle actor health by negative value is not allowed"));
	ActiveStats.Health -= Amount;
	if (ActiveStats.Health < 0) {
		ActiveStats.Health = 0;
		iLog("Ability actor dead");
		DestroyAbilityActor();
	}
}

int AAbilityActor::GetAbilityId() const
{
	return GetAbility()->GetAbilityId();
}

AImmieCharacter* AAbilityActor::GetImmieCharacter() const
{
	return GetAbility()->GetImmieCharacter();
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

float AAbilityActor::GetHealingDuration() const
{
	return GetAbility()->GetHealingDuration();
}

float AAbilityActor::GetSpeed() const
{
	return GetAbility()->GetSpeed();
}

float AAbilityActor::GetRange() const
{
	return GetAbility()->GetRange();
}

TArray<UImmieType*> AAbilityActor::GetType() const
{
	return GetAbility()->GetType();
}

void AAbilityActor::UpdateVisuals()
{
}

FBattleStats AAbilityActor::GetActiveStats() const
{
	return ActiveStats;
}

float AAbilityActor::TotalHealingFromAbility(const FAbilityInstigatorDamage& AbilityHealing) const
{
	// TODO properly implement healing thats not just the opposite of damage.
	const float UnmodifiedDamageMultiplier = UFormula::Damage(AbilityHealing.Power, AbilityHealing.AttackerStat, AbilityHealing.DefenderStat, AbilityHealing.InstigatorLevel);
	const float TypeDamageMultiplier = UImmieType::TotalTypeDamageMultiplier(AbilityHealing.InstigatorType, AbilityHealing.DefenderType);
	return UnmodifiedDamageMultiplier * TypeDamageMultiplier;
}

float AAbilityActor::TotalDamageFromAbility(const FAbilityInstigatorDamage& AbilityDamage) const
{
	const float UnmodifiedDamageMultiplier = UFormula::Damage(AbilityDamage.Power, AbilityDamage.AttackerStat, AbilityDamage.DefenderStat, AbilityDamage.InstigatorLevel);
	const float TypeDamageMultiplier = UImmieType::TotalTypeDamageMultiplier(AbilityDamage.InstigatorType, AbilityDamage.DefenderType);
	return UnmodifiedDamageMultiplier * TypeDamageMultiplier;
}

FBattleStats AAbilityActor::GetAbilityInitialStats() const
{
	return GetAbility()->GetInitialStats();
}

FBattleStats AAbilityActor::GetAbilityActiveStats() const
{
	return GetAbility()->GetActiveStats();
}

