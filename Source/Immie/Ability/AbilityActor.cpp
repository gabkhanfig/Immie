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
#include <Immie/Type/BattleTypeComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include <Components/ArrowComponent.h>
#include "Camera/CameraComponent.h"
#include "../Battle/BattleInstance/BattleInstance.h"
#include "Camera/CameraComponent.h"
#include <Immie/Game/Global/Managers/AbilityDataManager.h>

bool AAbilityActor::HasBattleActorCollidedAlready(const TScriptInterface<IBattleActor>& BattleActor) const
{
	return BattleActorHitColliders.Contains(BattleActor.GetObject());
}

bool AAbilityActor::HasBattleActorCollidedWithCollider(const TScriptInterface<IBattleActor>& BattleActor, UPrimitiveComponent* CheckColliderHit) const
{
	UObject* BattleActorObj = BattleActor.GetObject();
	const TArray<UPrimitiveComponent*>* Found = &BattleActorHitColliders.Find(BattleActorObj)->Colliders;
	if (Found == nullptr) {
		return false;
	}

	return Found->Contains(CheckColliderHit);
}

void AAbilityActor::AddCollidedBattleActor(const TScriptInterface<IBattleActor>& BattleActor, UPrimitiveComponent* HitCollider)
{
	UObject* BattleActorObj = BattleActor.GetObject();
	TArray<UPrimitiveComponent*>* Found = &BattleActorHitColliders.Find(BattleActorObj)->Colliders;
	if (Found == nullptr) {
		FBattleActorColliderHitArray Arr;
		Arr.Colliders.Add(HitCollider);
		BattleActorHitColliders.Add(BattleActorObj, Arr);
		return;
	}
	Found->Add(HitCollider);
}

void AAbilityActor::EnableAbilityProjectileComponent(AActor* AbilityActor, UProjectileMovementComponent* ProjMovement, AImmieCharacter* ImmieCharacter, float Speed, float Gravity)
{
	checkf(ProjMovement, TEXT("Projectile movement component for ability actor must not be null"));
	ProjMovement->SetComponentTickEnabled(true);
	//const FVector Velocity = ImmieCharacter->GetFollowCamera()->GetForwardVector() * Speed;
	ProjMovement->ProjectileGravityScale = Gravity;
	ProjMovement->SetVelocityInLocalSpace({ Speed, 0, 0 });
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
	ProjectileComponent->bRotationFollowsVelocity = true;

	bShouldSpawnVisualDummy = true;
}

void AAbilityActor::PostLoad()
{
	const FName ClassName = GetClass()->GetFName();
	if (ClassName == "AbilityActor") {
		Super::PostLoad();
		return;
	}

	AbilityName = UAbilityDataManager::AbilityNameFromBlueprintClassName(ClassName.ToString(), "Actor_C");

	Super::PostLoad();
}

void AAbilityActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const 
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAbilityActor, ActiveStats);
}

void AAbilityActor::BeginPlay()
{
	Super::BeginPlay(); // Calls blueprint begin play

	if (HasBattleAuthority() && bShouldSpawnVisualDummy) {
		InformClientsSpawnDummy();
	}
	//iLog("AbilityActor::BeginPlay() cpp");
}

void AAbilityActor::InformClientsSpawnDummy_Implementation()
{
	if (!IsRunningDedicatedServer()) {
		SpawnVisualDummy();
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

	AActor* _Owner = GetImmieCharacter(); //IBattleActor::Execute_GetTeam(this)
	ADummyAbilityActor* DummyActor = GetWorld()->SpawnActorDeferred<ADummyAbilityActor>
		(DummyActorClass, SpawnTransform, _Owner, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
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
		//iLog("no battle authority");
		return;
	}

	if (OtherActor == GetImmieCharacter()) {
		//iLog("is owner immie character");
		return;
	}

	const bool Implements = OtherActor->GetClass()->ImplementsInterface(UBattleActor::StaticClass());
	if (!Implements) {
		//iLog("is not a battle actor");
		return;
	}

	TScriptInterface<IBattleActor> BattleActor = OtherActor;
	const bool IsValidAbilityCollider = IBattleActor::Execute_IsValidAbilityCollider(BattleActor.GetObject(), OtherActorComponent);// BattleActor->IsValidAbilityCollider(OtherActorComponent);
	if (!IsValidAbilityCollider) {
		//iLog("invalid ability collider");
		return;
	}

	OnBattleActorCollision(BattleActor, ThisOverlappedComponent, OtherActorComponent, IBattleActor::Execute_IsEnemy(this, BattleActor));
}

void AAbilityActor::OnBattleActorCollision_Implementation(const TScriptInterface<IBattleActor>& BattleActor, UPrimitiveComponent* ThisComponent, UPrimitiveComponent* OtherComponent, bool IsEnemy)
{
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

	BP_InitializeForBattle();
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

FBattleActorHitscanResult AAbilityActor::HitscanShotFromImmie(AImmieCharacter* Start, float MaxDistance, int TargetBitmask)
{
	const FVector StartLocation = Start->GetFollowCamera()->GetComponentLocation();
	const FVector EndLocation = StartLocation + (MaxDistance * Start->GetFollowCamera()->GetForwardVector());

	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(Start);
	
	// If it's not flagged to target enemies, set all enemy battle actors to be ingored.
	if (!(TargetBitmask & uint8(EAbilityHitscanShotTarget::Enemies))) {
		const ABattleInstance* BattleInstance = Start->GetBattleInstance();
		const int TeamCount = BattleInstance->GetTeamCount();
		for (int i = 0; i < TeamCount; i++) {
			ABattleTeam* Team = BattleInstance->GetTeam(i);
			if (Team == IBattleActor::Execute_GetTeam(Start)) continue;

			TraceParams.AddIgnoredActor(Team->GetActiveImmie());
			TArray<AAbilityActor*> AbilityActors;
			for (AActor* AsActor : AbilityActors) {
				if (IsValid(AsActor)) TraceParams.AddIgnoredActor(AsActor);
			}
		}
	}

	FHitResult OutResult;
	GetWorld()->LineTraceSingleByChannel(OutResult, StartLocation, EndLocation, ECollisionChannel::ECC_Visibility, TraceParams);

	FBattleActorHitscanResult Hitscan;
	if (OutResult.GetActor() != nullptr) {
		const bool IsBattleActor = OutResult.GetActor()->GetClass()->ImplementsInterface(UBattleActor::StaticClass());
		if (!IsBattleActor) {
			return Hitscan;
		}
		Hitscan.HitSuccess	= true;
		Hitscan.BattleActor = OutResult.GetActor();
		Hitscan.Location		= OutResult.ImpactPoint;
		Hitscan.HitCollider = OutResult.GetComponent();
		return Hitscan;
	}
	return Hitscan;
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

bool AAbilityActor::IsEnemy_Implementation(const TScriptInterface<IBattleActor>& OtherBattleActor) const
{
	const ABattleTeam* Team = IBattleActor::Execute_GetTeam(this);
	if (!IsValid(Team)) {
		return true;
	}
	bool Enemy = Team != IBattleActor::Execute_GetTeam(OtherBattleActor.GetObject());
	//if (Enemy) {
	//	iLog("enemy");
	//}
	//else {
	//	iLog("ally");
	//}
	return Team != IBattleActor::Execute_GetTeam(OtherBattleActor.GetObject());
}

FBattleStats AAbilityActor::GetInitialStats_Implementation() const
{
	return GetAbilityInitialStats();
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

UBattleTypeComponent* AAbilityActor::GetTypeComponent_Implementation() const
{
	return GetAbility()->GetTypeComponent();
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
	const float TypeDamageMultiplier = AbilityHealing.DefenderTypeComponent->TotalTypeDamageMultiplier(AbilityHealing.InstigatorType);
	return UnmodifiedDamageMultiplier * TypeDamageMultiplier;
}

float AAbilityActor::TotalDamageFromAbility_Implementation(const FAbilityInstigatorDamage& AbilityDamage) const
{
	const float UnmodifiedDamageMultiplier = UFormula::Damage(AbilityDamage.Power, AbilityDamage.AttackerStat, AbilityDamage.DefenderStat, AbilityDamage.InstigatorLevel);
	const float TypeDamageMultiplier = AbilityDamage.DefenderTypeComponent->TotalTypeDamageMultiplier(AbilityDamage.InstigatorType);
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

