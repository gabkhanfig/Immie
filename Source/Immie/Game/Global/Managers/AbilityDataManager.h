// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameDataManager.h"
#include <Immie/Ability/AbilityDataTypes.h>
#include "AbilityDataManager.generated.h"

class UAbility;
class AAbilityActor;
class UAbilityDataObject;

UCLASS()
/**/
class IMMIE_API UAbilityDataManager : public UGameDataManager
{
	GENERATED_BODY()

private:

	void SetAbilityNamesAndIds();

	virtual void LoadDefaultGameData() override;

	FString LoadAbilityJsonFileToString(FName AbilityName, const FString& FolderName);

	TSubclassOf<UAbilityDataObject> LoadAbilityDataObjectClass(FName AbilityName);

	UAbilityDataObject* RegisterAbility(UObject* Outer, FName AbilityName, const FString& JsonString);
	
public:

	UFUNCTION(BlueprintPure)
		/**/
		static TArray<FName> GetAllAbilityNames();

	UFUNCTION(BlueprintPure)
		/**/
		static bool IsValidAbilityName(FName AbilityName);

	UFUNCTION(BlueprintPure)
		/* Useful for checking if valid name without adding an FName entry. Much slower than FName access though. */
		static bool IsValidAbilityNameString(const FString& AbilityStringName);

	static TSet<FName> GetSetOfAbilityNames();

	void RegisterAbilitiesFromDisk(UObject* Outer, const FString& FolderName, TMap<FName, UAbilityDataObject*>* AbilitiesOut);

	void RegisterAbilitiesFromSerialized(UObject* Outer, const TArray<FSerializedAbilityData>& AbilityData, TMap<FName, UAbilityDataObject*>* AbilitiesOut);

	static UAbilityDataObject* GetAbilityDataObjectFromMap(TMap<FName, UAbilityDataObject*>& Map, FName AbilityName);

	UFUNCTION(BlueprintPure)
		/* Get the global ability object corresponding to the ability id. Not for use in battle. */
		UAbilityDataObject* GetAbilityDataObject(FName AbilityName);

	static UClass* GetAbilityClassFromMap(TMap<FName, UAbilityDataObject*>& Map, FName AbilityName);

	UFUNCTION(BlueprintPure)
		/**/
		UClass* GetAbilityClass(FName AbilityName);

	static UClass* GetActorClassFromMap(TMap<FName, UAbilityDataObject*>& Map, FName AbilityName);

	UFUNCTION(BlueprintPure)
		/**/
		UClass* GetActorClass(FName AbilityName);

	static FName AbilityNameFromBlueprintClassName(const FString ClassName, FString RightChop);

private:

	/* Global variable containing all of the ordered specie names. */
	static const TArray<FName> AbilityNames;
	/* Global variable that contains all FName's in SpecieNames, for fast look-up. */
	static const TSet<FName> AbilityNamesSet;
	/* Fast access to check valid ability names as strings without adding FName entries. */
	static const TSet<FString> AbilityStringNames;

	UPROPERTY()
		/**/
		TMap<FName, UAbilityDataObject*> Abilities;

};
