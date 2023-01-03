// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <Immie/ImmieCore.h>
#include <Immie/Util/Json/BlueprintJsonObject.h>
#include "GameDataManager.generated.h"

UCLASS()
class IMMIE_API UGameDataManager : public UObject
{
	GENERATED_BODY()

	friend class UImmieGameInstance;

protected:

	bool bLoadedData;

protected:

	static const FString& GetGameDataDir();

	void Load();

	virtual void LoadDefaultGameData();

	void Reload();
	
};
