// Fill out your copyright notice in the Description page of Project Settings.


#include "DummyAbilityActor.h"
#include "AbilityActor.h"
#include <Immie/ImmieCore.h>
#include "AbilityDataObject.h"
#include <GameFramework/ProjectileMovementComponent.h>
#include <Components/ArrowComponent.h>
#include <Immie/Game/Global/Managers/AbilityDataManager.h>

// Sets default values
ADummyAbilityActor::ADummyAbilityActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = false;

	SetActorEnableCollision(false);

	RootComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("Root Component"));

	ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileComponent->UpdatedComponent = RootComponent;
	ProjectileComponent->SetComponentTickEnabled(false);
	ProjectileComponent->InitialSpeed = 0;
	ProjectileComponent->MaxSpeed = 0;
	ProjectileComponent->ProjectileGravityScale = 0;
	ProjectileComponent->bRotationFollowsVelocity = true;
}

void ADummyAbilityActor::PostLoad()
{
	const FName ClassName = GetClass()->GetFName();
	if (ClassName == "DummyAbilityActor") {
		Super::PostLoad();
		return;
	}

	AbilityName = UAbilityDataManager::AbilityNameFromBlueprintClassName(ClassName.ToString(), "DummyActor_C");

	Super::PostLoad();
}

void ADummyAbilityActor::BeginPlay()
{
	Super::BeginPlay();
}

void ADummyAbilityActor::BP_OnAbilityActorDestroy_Implementation()
{
	Destroy();
}

void ADummyAbilityActor::SetAbilityActor(AAbilityActor* _AbilityActor)
{
	AbilityActor = _AbilityActor;
}

void ADummyAbilityActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADummyAbilityActor::OnAbilityActorDestroy()
{
	BP_OnAbilityActorDestroy();
}

