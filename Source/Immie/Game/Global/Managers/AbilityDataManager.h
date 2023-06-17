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

	UPROPERTY()
		/**/
		TMap<FName, UAbilityDataObject*> Abilities;

	UPROPERTY()
		/**/
		TArray<FName> AbilityNames;

private:

	void SetAbilityNamesAndIds();

	virtual void LoadDefaultGameData() override;

	FString LoadAbilityJsonFileToString(FName AbilityName, const FString& FolderName);

	TSubclassOf<UAbilityDataObject> LoadAbilityDataObjectClass(FName AbilityName);

	UAbilityDataObject* RegisterAbility(UObject* Outer, FName AbilityName, const FString& JsonString);
	
public:

	UFUNCTION(BlueprintPure)
		/**/
		static TArray<FName> AllAbilityNames();

	void RegisterAbilitiesFromDisk(UObject* Outer, const FString& FolderName, TMap<FName, UAbilityDataObject*>* AbilitiesOut);

	void RegisterAbilitiesFromSerialized(UObject* Outer, const TArray<FSerializedAbilityData>& AbilityData, TMap<FName, UAbilityDataObject*>* AbilitiesOut);

	static UAbilityDataObject* GetAbilityDataObjectFromMap(TMap<FName, UAbilityDataObject*>& Map, FName AbilityName);

	UFUNCTION(BlueprintPure)
		/* Get the global ability object corresponding to the ability id. Not for use in battle. */
		UAbilityDataObject* GetAbilityDataObject(FName AbilityName);

	static bool IsValidAbilityFromMap(TMap<FName, UAbilityDataObject*>& Map, FName AbilityName);

	UFUNCTION(BlueprintPure)
		/**/
		bool IsValidAbility(FName AbilityName);

	static UClass* GetAbilityClassFromMap(TMap<FName, UAbilityDataObject*>& Map, FName AbilityName);

	UFUNCTION(BlueprintPure)
		/**/
		UClass* GetAbilityClass(FName AbilityName);

	static UClass* GetActorClassFromMap(TMap<FName, UAbilityDataObject*>& Map, FName AbilityName);

	UFUNCTION(BlueprintPure)
		/**/
		UClass* GetActorClass(FName AbilityName);

	static FName AbilityNameFromBlueprintClassName(const FString ClassName, FString RightChop);
};
