// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityActor.h"
#include <Immie/ImmieCore.h>
#include <Immie/Ability/Ability.h>
#include "AbilityDataObject.h"
#include <Immie/Battle/Components/DamageComponent.h>
#include <Net/UnrealNetwork.h>
#include "DummyAbilityActor.h"
#include <Immie/Battle/Team/BattleTeam.h>
#include <Kismet/GameplayStatics.h>
#include <Immie/Immies/ImmieCharacter.h>
#include <Immie/Type/ImmieType.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include <Components/ArrowComponent.h>
#include "Camera/CameraComponent.h"

void AAbilityActor::EnableAbilityProjectileComponent(AActor* AbilityActor, UAbilityDataObject* AbilityDataObject, UProjectileMovementComponent* ProjMovement, AImmieCharacter* ImmieCharacter)
{
	checkf(ProjMovement, TEXT("Projectile movement component for ability actor must not be null"));
	ProjMovement->ProjectileGravityScale = AbilityDataObject->GetGravity();
	ProjMovement->SetComponentTickEnabled(true);
	const FVector Velocity = ImmieCharacter->GetFollowCamera()->GetComponentRotation().Vector() * AbilityDataObject->GetSpeed();
	ProjMovement->Velocity = Velocity;
}

AAbilityActor::AAbilityActor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;
	bNetLoadOnClient = true;

	RootComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("Root Component"));

	ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileComponent->UpdatedComponent = RootComponent;
	ProjectileComponent->SetComponentTickEnabled(false);
	ProjectileComponent->InitialSpeed = 0;
	ProjectileComponent->MaxSpeed = 0;
	ProjectileComponent->ProjectileGravityScale = 0;
	ProjectileComponent->bRotationFollowsVelocity;
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
		(DummyActorClass, SpawnTransform, IBattleActor::Execute_GetTeam(this), nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
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
	const bool IsValidAbilityCollider = IBattleActor::Execute_IsValidAbilityCollider(BattleActor.GetObject(), OtherActorComponent);// BattleActor->IsValidAbilityCollider(OtherActorComponent);
	if (!IsValidAbilityCollider) {
		return;
	}

	OnBattleActorCollision(BattleActor, ThisOverlappedComponent, OtherActorComponent);
}

void AAbilityActor::OnBattleActorCollision_Implementation(const TScriptInterface<IBattleActor>& BattleActor, UPrimitiveComponent* ThisComponent, UPrimitiveComponent* OtherComponent)
{
	if (IBattleActor::Execute_IsAlly(this, BattleActor)) {
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

	if (!IBattleActor::Execute_CanBeHealedByAbilityActor(Ally.GetObject(), this)) {
		return;
	}

	UDamageComponent* AllyDamageComponent = IBattleActor::Execute_GetDamageComponent(Ally.GetObject());
	if (!IsValid(AllyDamageComponent)) {
		iLog("Invalid ally damage component in AAbilityActor::OnAllyCollision().", LogVerbosity_Error);
		return;
	}

	FAbilityInstigatorDamage Healing;
	Healing.Instigator = GetAbility();
	Healing.InstigatorType = GetAbility()->GetType();
	Healing.Power = GetHealingPower();

	if (AbilityFlags.BaseStats || AbilityFlags.RelativeStats) {
		Healing.AttackerStat = IBattleActor::Execute_GetActiveStats(this).Attack;
	}
	else {
		Healing.AttackerStat = IBattleActor::Execute_GetActiveStats(GetImmieCharacter()).Attack;
	}
	
	Healing.DefenderStat = IBattleActor::Execute_GetActiveStats(Ally.GetObject()).Defense;
	Healing.DefenderType = IBattleActor::Execute_GetType(Ally.GetObject());
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

	if (!IBattleActor::Execute_CanBeDamagedByAbilityActor(Enemy.GetObject(), this)) {
		return;
	}

	UDamageComponent* EnemyDamageComponent = IBattleActor::Execute_GetDamageComponent(Enemy.GetObject());
	if (!IsValid(EnemyDamageComponent)) {
		iLog("Invalid enemy damage component in AAbilityActor::OnEnemyCollision().", LogVerbosity_Error);
		return;
	}

	FAbilityInstigatorDamage Damage;
	Damage.Instigator = GetAbility();
	Damage.InstigatorType = GetAbility()->GetType();
	Damage.Power = GetDamagePower();

	if (AbilityFlags.BaseStats || AbilityFlags.RelativeStats) {
		Damage.AttackerStat = IBattleActor::Execute_GetActiveStats(this).Attack;
	}
	else {
		Damage.AttackerStat = IBattleActor::Execute_GetActiveStats(GetImmieCharacter()).Attack;
	}

	Damage.DefenderStat = IBattleActor::Execute_GetActiveStats(Enemy.GetObject()).Defense;
	Damage.DefenderType = IBattleActor::Execute_GetType(Enemy.GetObject());
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
	IBattleActor::Execute_GetTeam(this)->RemoveAbilityActor(this);
}

void AAbilityActor::OnAbilityActorDestroy_Implementation()
{
	//ABattleTeam* _Team = GetTeam();
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

	if (GetAbilityFlags().Projectile) {
		MovementComponent = ProjectileComponent;
		AAbilityActor::EnableAbilityProjectileComponent(this, GetAbilityDataObject(), ProjectileComponent, GetImmieCharacter());
	}

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

bool AAbilityActor::IsValidAbilityCollider_Implementation(UPrimitiveComponent* Collider) const
{
	return AbilityColliders.Contains(Collider);
}

bool AAbilityActor::CanBeHealedByAbilityActor_Implementation(AAbilityActor* AbilityActor) const
{
	return true;
}

bool AAbilityActor::CanBeDamagedByAbilityActor_Implementation(AAbilityActor* AbilityActor) const
{
	return true;
}

ABattleTeam* AAbilityActor::GetTeam_Implementation() const
{
	return GetAbility()->GetTeam();
}

void AAbilityActor::AuthorityBattleTick_Implementation(float DeltaTime)
{
	BP_AuthorityBattleTick(DeltaTime);
}

void AAbilityActor::ClientBattleTick_Implementation(float DeltaTime)
{
	BP_ClientBattleTick(DeltaTime);
}

void AAbilityActor::IncreaseHealth_Implementation(float Amount)
{
	checkf(Amount >= 0, TEXT("Increasing battle actor health by negative value is not allowed"));
	ActiveStats.Health += Amount;
	if (ActiveStats.Health > GetAbility()->GetInitialStats().Health) {
		ActiveStats.Health = GetAbility()->GetInitialStats().Health;
	}
}

void AAbilityActor::DecreaseHealth_Implementation(float Amount)
{
	checkf(Amount >= 0, TEXT("Decreasing battle actor health by negative value is not allowed"));
	ActiveStats.Health -= Amount;
	if (ActiveStats.Health < 0) {
		ActiveStats.Health = 0;
		iLog("Ability actor dead");
		DestroyAbilityActor();
	}
}

bool AAbilityActor::IsAlly_Implementation(const TScriptInterface<IBattleActor>& OtherBattleActor) const
{
	const ABattleTeam* Team = IBattleActor::Execute_GetTeam(this);
	if (!IsValid(Team)) return false;
	return Team == IBattleActor::Execute_GetTeam(OtherBattleActor.GetObject());
}

FBattleStats AAbilityActor::GetInitialStats_Implementation() const
{
	return GetAbilityInitialStats();
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

TArray<UImmieType*> AAbilityActor::GetType_Implementation() const
{
	return GetAbility()->GetType();
}

FString AAbilityActor::GetDisplayName_Implementation() const
{
	return GetAbilityName().ToString();
}

void AAbilityActor::UpdateVisuals_Implementation()
{
}

FBattleStats AAbilityActor::GetActiveStats_Implementation() const
{
	return ActiveStats;
}

float AAbilityActor::TotalHealingFromAbility_Implementation(const FAbilityInstigatorDamage& AbilityHealing) const
{
	// TODO properly implement healing thats not just the opposite of damage.
	const float UnmodifiedDamageMultiplier = UFormula::Damage(AbilityHealing.Power, AbilityHealing.AttackerStat, AbilityHealing.DefenderStat, AbilityHealing.InstigatorLevel);
	const float TypeDamageMultiplier = UImmieType::TotalTypeDamageMultiplier(AbilityHealing.InstigatorType, AbilityHealing.DefenderType);
	return UnmodifiedDamageMultiplier * TypeDamageMultiplier;
}

float AAbilityActor::TotalDamageFromAbility_Implementation(const FAbilityInstigatorDamage& AbilityDamage) const
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

