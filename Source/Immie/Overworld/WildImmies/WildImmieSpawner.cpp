// Fill out your copyright notice in the Description page of Project Settings.


#include "WildImmieSpawner.h"
#include "WildSpawnTable.h"
#include "ImmieSpawnData.h"
#include "../../Game/Global/Managers/SpawnTableManager.h"
#include "../../Immies/ImmieCharacter.h"
#include "../../Immies/ImmieObject.h"

// Sets default values
AWildImmieSpawner::AWildImmieSpawner()
	: SpawnTable(nullptr)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWildImmieSpawner::BeginPlay()
{
	checkf(SpawnTableName != FName(), TEXT("SpawnTableName must be set for WildImmieSpawner"));
	
	SpawnTable = GetSpawnTableManager()->GetImmieSpawnTable(SpawnTableName);
	if (SpawnTable == nullptr) {
		iLog("[AWildImmieSpawner]: SpawnTableName of " + SpawnTableName.ToString() + " is not a valid spawn table. Disabling ticking on this spawner and not executing begin play.", LogVerbosity_Error);
		SetActorTickEnabled(false);
		return;
	}

	Super::BeginPlay();

	UImmie* ImmieObject = UImmie::NewImmieObject(this, 0);

	AImmieCharacter* SpawnedImmie = AImmieCharacter::SpawnWildImmieCharacter(this, GetActorTransform(), ImmieObject);
	SpawnedImmies.Add(SpawnedImmie);
}

// Called every frame
void AWildImmieSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (int i = 0; i < SpawnedImmies.Num(); i++) {
		if (!IsValid(SpawnedImmies[i])) {
			SpawnedImmies.RemoveAt(i);
			i--;
			continue;
		}

		SpawnedImmies[i]->WildTick(DeltaTime);
	}

}

