// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameDataManager.h"
#include "SpawnTableManager.generated.h"

/**
 * 
 */
UCLASS()
class IMMIE_API USpawnTableManager : public UGameDataManager
{
	GENERATED_BODY()

private:

	virtual void LoadDefaultGameData() override;
	
};
