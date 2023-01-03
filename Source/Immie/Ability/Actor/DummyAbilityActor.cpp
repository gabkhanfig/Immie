// Fill out your copyright notice in the Description page of Project Settings.


#include "DummyAbilityActor.h"
#include "AbilityActor.h"

// Sets default values
ADummyAbilityActor::ADummyAbilityActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADummyAbilityActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADummyAbilityActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

