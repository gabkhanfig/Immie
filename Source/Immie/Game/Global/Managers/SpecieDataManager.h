// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameDataManager.h"
#include <Immie/Immies/SpecieDataTypes.h>
#include "SpecieDataManager.generated.h"

#define INVALID_SPECIE_ID -1

class USpecieDataObject;

/**
 * 
 */
UCLASS()
class IMMIE_API USpecieDataManager : public UGameDataManager
{
	GENERATED_BODY()

private:

	UPROPERTY()
		/**/
		TMap<int, USpecieDataObject*> Species;

	UPROPERTY()
		/**/
		TArray<FName> SpecieNames;

	UPROPERTY()
		/**/
		TMap<FName, int> SpecieIds;

private:

	void SetSpecieNamesAndIds();

	virtual void LoadDefaultGameData() override;

	FString LoadSpecieJsonFileToString(FName SpecieName, const FString& FolderName);

	UClass* LoadSpecieDataObjectClass(FName SpecieName);

	USpecieDataObject* RegisterSpecie(UObject* Outer, FName SpecieName, int SpecieId, const FString& JsonString, bool LoadLearnsets = true);

public:

	/* Set maps to nullptr to not load that set of data. */
	void RegisterSpeciesFromDisk(UObject* Outer, const FString& FolderName, TMap<int, USpecieDataObject*>* SpeciesOut, bool LoadLearnsets = true);

	/**/
	void RegisterSpeciesFromSerialized(UObject* Outer, const TArray<FSerializedSpecieData>& SpecieData, TMap<int, USpecieDataObject*>* SpeciesOut, bool LoadLearnsets = true);

	static USpecieDataObject* GetSpecieDataObjectFromMap(TMap<int, USpecieDataObject*>& Map, int SpecieId);

	UFUNCTION(BlueprintPure)
		/**/
		USpecieDataObject* GetSpecieDataObject(int SpecieId);

	static int GetSpecieIdFromMap(TMap<FName, int>& Map, FName SpecieName);

	UFUNCTION(BlueprintPure)
		/**/
		int GetSpecieId(FName Name);

	static FName GetSpecieNameFromMap(TMap<int, USpecieDataObject*>& Map, int SpecieId);

	UFUNCTION(BlueprintPure)
		/**/
		FName GetSpecieName(int SpecieId);

	static bool IsValidSpecieFromMap(TMap<int, USpecieDataObject*>& Map, int SpecieId);

	UFUNCTION(BlueprintPure)
		/**/
		bool IsValidSpecie(int SpecieId);

	static int GetSpecieTypeBitmaskFromMap(TMap<int, USpecieDataObject*>& Map, int SpecieId);

	UFUNCTION(BlueprintPure)
		/**/
		int GetSpecieTypeBitmask(int SpecieId);

	static FBaseStats GetSpecieBaseStatsFromMap(TMap<int, USpecieDataObject*>& Map, int SpecieId);

	UFUNCTION(BlueprintPure)
		/**/
		FBaseStats GetSpecieBaseStats(int SpecieId);

	static UClass* GetImmieObjectClassFromMap(TMap<int, USpecieDataObject*>& Map, int SpecieId);

	UFUNCTION(BlueprintPure)
		/**/
		UClass* GetImmieObjectClass(int SpecieId);

	static UClass* GetImmieCharacterClassFromMap(TMap<int, USpecieDataObject*>& Map, int SpecieId);
	
	UFUNCTION(BlueprintPure)
		/**/
		UClass* GetImmieCharacterClass(int SpecieId);

	static FSpecieLearnset GetSpecieLearnsetsFromMap(TMap<int, USpecieDataObject*>& Map, int SpecieId);

	UFUNCTION(BlueprintPure)
		/* Avoid repeated calls due to potentially expensive multiple array copying. */
		FSpecieLearnset GetSpecieLearnsets(int SpecieId);

	
};
