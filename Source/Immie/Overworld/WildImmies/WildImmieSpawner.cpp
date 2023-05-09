// Fill out your copyright notice in the Description page of Project Settings.


#include "WildImmieSpawner.h"
#include "WildSpawnTable.h"
#include "ImmieSpawnData.h"
#include "../../Game/Global/Managers/SpawnTableManager.h"

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
	
	USpawnTableManager* SpawnTableManager = GetSpawnTableManager();
	if (!SpawnTableManager->IsValidSpawnTable(SpawnTableName)) {
		iLog("[AWildImmieSpawner]: SpawnTableName of " + SpawnTableName.ToString() + " is not a valid spawn table", LogVerbosity_Error);
	}
	SpawnTable = SpawnTableManager->GetImmieSpawnTable(SpawnTableName);

	Super::BeginPlay();
	
}

// Called every frame
void AWildImmieSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

