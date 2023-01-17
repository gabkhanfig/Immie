// Fill out your copyright notice in the Description page of Project Settings.


#include "DummyAbilityActor.h"
#include "AbilityActor.h"
#include <Immie/ImmieCore.h>
#include <Immie/Ability/Abilities/AbilityDataObject.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include <Components/ArrowComponent.h>

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
	ProjectileComponent->bRotationFollowsVelocity;
}

void ADummyAbilityActor::BeginPlay()
{
	Super::BeginPlay();

	UAbilityDataObject* AbilityDataObject = AbilityActor->GetAbilityDataObject();
	if (AbilityDataObject->GetAbilityFlags().Projectile) {
		MovementComponent = ProjectileComponent;
		AAbilityActor::EnableAbilityProjectileComponent(this, AbilityDataObject, ProjectileComponent, AbilityActor->GetImmieCharacter());
	}
}

void ADummyAbilityActor::BP_OnAbilityActorDestroy_Implementation()
{
	Destroy();
}

void ADummyAbilityActor::SetAbilityActor(AAbilityActor* _AbilityActor)
{
	AbilityActor = _AbilityActor;
}

// Called every frame
void ADummyAbilityActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector location = GetActorLocation();
	iLog("Dummy location: " + location.ToString(), LogVerbosity_Display, 0);
}

void ADummyAbilityActor::OnAbilityActorDestroy()
{
	BP_OnAbilityActorDestroy();
}

