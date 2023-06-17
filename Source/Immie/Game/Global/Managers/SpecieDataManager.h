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
		TMap<FName, USpecieDataObject*> Species;

	UPROPERTY()
		/**/
		TArray<FName> SpecieNames;

private:

	void SetSpecieNamesAndIds();

	virtual void LoadDefaultGameData() override;

	FString LoadSpecieJsonFileToString(FName SpecieName, const FString& FolderName);

	TSubclassOf<USpecieDataObject> LoadSpecieDataObjectClass(FName SpecieName);

	USpecieDataObject* RegisterSpecie(UObject* Outer, FName SpecieName, const FString& JsonString, bool LoadLearnsets = true);

public:

	UFUNCTION(BlueprintPure)
		/**/
		static TArray<FName> AllSpecieNames();

	/* Set maps to nullptr to not load that set of data. */
	void RegisterSpeciesFromDisk(UObject* Outer, const FString& FolderName, TMap<FName, USpecieDataObject*>* SpeciesOut, bool LoadLearnsets = true);

	/**/
	void RegisterSpeciesFromSerialized(UObject* Outer, const TArray<FSerializedSpecieData>& SpecieData, TMap<FName, USpecieDataObject*>* SpeciesOut, bool LoadLearnsets = true);

	static USpecieDataObject* GetSpecieDataObjectFromMap(TMap<FName, USpecieDataObject*>& Map, FName SpecieName);

	UFUNCTION(BlueprintPure)
		/**/
		USpecieDataObject* GetSpecieDataObject(FName SpecieName);

	static bool IsValidSpecieFromMap(TMap<FName, USpecieDataObject*>& Map, FName SpecieName);

	UFUNCTION(BlueprintPure)
		/**/
		bool IsValidSpecie(FName SpecieName);

	static int GetSpecieTypeBitmaskFromMap(TMap<FName, USpecieDataObject*>& Map, FName SpecieName);

	UFUNCTION(BlueprintPure)
		/**/
		int GetSpecieTypeBitmask(FName SpecieName);

	static FBaseStats GetSpecieBaseStatsFromMap(TMap<FName, USpecieDataObject*>& Map, FName SpecieName);

	UFUNCTION(BlueprintPure)
		/**/
		FBaseStats GetSpecieBaseStats(FName SpecieName);

	static UClass* GetImmieObjectClassFromMap(TMap<FName, USpecieDataObject*>& Map, FName SpecieName);

	UFUNCTION(BlueprintPure)
		/**/
		UClass* GetImmieObjectClass(FName SpecieName);

	static UClass* GetImmieCharacterClassFromMap(TMap<FName, USpecieDataObject*>& Map, FName SpecieName);
	
	UFUNCTION(BlueprintPure)
		/**/
		UClass* GetImmieCharacterClass(FName SpecieName);

	static FSpecieLearnset GetSpecieLearnsetsFromMap(TMap<FName, USpecieDataObject*>& Map, FName SpecieName);

	UFUNCTION(BlueprintPure)
		/* Avoid repeated calls due to potentially expensive multiple array copying. */
		FSpecieLearnset GetSpecieLearnsets(FName SpecieName);

	static FName SpecieNameFromBlueprintClassName(const FString ClassName, FString RightChop);


};
