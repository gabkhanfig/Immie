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

	void SetSpecieNamesAndIds();

	virtual void LoadDefaultGameData() override;

	FString LoadSpecieJsonFileToString(FName SpecieName, const FString& FolderName);

	TSubclassOf<USpecieDataObject> LoadSpecieDataObjectClass(FName SpecieName);

	USpecieDataObject* RegisterSpecie(UObject* Outer, FName SpecieName, const FString& JsonString, bool LoadLearnsets = true);

public:

	UFUNCTION(BlueprintPure)
		/**/
		static TArray<FName> GetAllSpecieNames();

	UFUNCTION(BlueprintPure)
		/**/
		static bool IsValidSpecieName(FName SpecieName);

	static TSet<FName> GetSetOfSpecieNames();

	/* Set maps to nullptr to not load that set of data. */
	void RegisterSpeciesFromDisk(UObject* Outer, const FString& FolderName, TMap<FName, USpecieDataObject*>* SpeciesOut, bool LoadLearnsets = true);

	/**/
	void RegisterSpeciesFromSerialized(UObject* Outer, const TArray<FSerializedSpecieData>& SpecieData, TMap<FName, USpecieDataObject*>* SpeciesOut, bool LoadLearnsets = true);

	static USpecieDataObject* GetSpecieDataObjectFromMap(TMap<FName, USpecieDataObject*>& Map, FName SpecieName);

	UFUNCTION(BlueprintPure)
		/**/
		USpecieDataObject* GetSpecieDataObject(FName SpecieName);

	static FName SpecieNameFromBlueprintClassName(const FString ClassName, FString RightChop);

private:

	/* Global variable containing all of the ordered specie names. */
	static const TArray<FName> SpecieNames;
	/* Global variable that contains all FName's in SpecieNames, for fast look-up. */
	static const TSet<FName> SpecieNamesSet;

	UPROPERTY()
		/**/
		TMap<FName, USpecieDataObject*> Species;

	

};
