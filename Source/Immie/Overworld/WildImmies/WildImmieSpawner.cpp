// Fill out your copyright notice in the Description page of Project Settings.


#include "WildImmieSpawner.h"
#include "WildSpawnTable.h"
#include "../../Immies/ImmieSpawnData.h"
#include "../../Game/Global/Managers/SpawnTableManager.h"
#include "../../Immies/ImmieCharacter.h"
#include "../../Immies/ImmieObject.h"

// Sets default values
AWildImmieSpawner::AWildImmieSpawner()
	: SpawnTable(nullptr)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetActorEnableCollision(false);
}

void AWildImmieSpawner::StartTrackingWildImmie(AImmieCharacter* ImmieCharacter)
{
	SpawnedImmies.Add(ImmieCharacter);
}

void AWildImmieSpawner::StopTrackingWildImmie(AImmieCharacter* ImmieCharacter)
{
	SpawnedImmies.RemoveSingle(ImmieCharacter);
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
}

AImmieCharacter* AWildImmieSpawner::SpawnWildImmie(UImmieSpawnData* SpawnData, FVector Location)
{
	UImmie* ImmieObject = UImmie::NewImmieObject(this, SpawnData->GetImmieName());
	const uint8 Level = FMath::RandRange(SpawnData->GetMinLevel(), SpawnData->GetMaxLevel());
	ImmieObject->SetLevel(Level);
	ImmieObject->Heal();
	const FRotator SpawnRotation{ 0, FMath::RandRange(-1.0, 1.0), 0 };
	const FTransform SpawnTransform{ SpawnRotation, Location };
	AImmieCharacter* SpawnedCharacter = AImmieCharacter::SpawnWildImmieCharacter(this, SpawnTransform, ImmieObject);
	SpawnedImmies.Add(SpawnedCharacter);
	return SpawnedCharacter;
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

