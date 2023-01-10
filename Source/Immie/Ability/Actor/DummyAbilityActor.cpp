// Fill out your copyright notice in the Description page of Project Settings.


#include "DummyAbilityActor.h"
#include "AbilityActor.h"
#include <Immie/ImmieCore.h>

// Sets default values
ADummyAbilityActor::ADummyAbilityActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = false;

	SetActorEnableCollision(false);
}

void ADummyAbilityActor::BeginPlay()
{
	Super::BeginPlay();
}

void ADummyAbilityActor::SetAbilityActor(AAbilityActor* _AbilityActor)
{
	AbilityActor = _AbilityActor;
}

// Called every frame
void ADummyAbilityActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADummyAbilityActor::OnAbilityActorDestroy()
{
	BP_OnAbilityActorDestroy();
}

