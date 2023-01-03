// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SaveManager.generated.h"

UCLASS()
/* Class instance that handles save reading, save writing, save switching, and more.
Is not a subclass of UGameDataManager thus is not called USaveDataManager. */
class IMMIE_API USaveManager : public UObject
{
	GENERATED_BODY()

private:

	FName ActiveSave;

public:

	static const FString& GetSaveDataDir();

	FName GetActiveSave() { return ActiveSave; }

	void LoadSave(FName SaveName, bool LoadPlayerImmies = true);

	void LoadSavePlayerImmies(const FString& SavePath);

	/* Save the active save to disk. */
	void Save();

	/* Save player Immies to the active save folder to disk. */
	void SavePlayerImmies(const FString& SavePath);

	/* Create a folder and populate template json files. */
	void InitializeSave(FName SaveName);
	
};
