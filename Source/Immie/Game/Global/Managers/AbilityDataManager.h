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
		TMap<int, UAbilityDataObject*> Abilities;

	UPROPERTY()
		/**/
		TArray<FName> AbilityNames;

	UPROPERTY()
		/**/
		TMap<FName, int> AbilityIds;

private:

	void SetAbilityNamesAndIds();

	virtual void LoadDefaultGameData() override;

	FString LoadAbilityJsonFileToString(FName AbilityName, const FString& FolderName);

	UClass* LoadAbilityDataObjectClass(FName AbilityName);

	UAbilityDataObject* RegisterAbility(UObject* Outer, FName AbilityName, int AbilityId, const FString& JsonString);
	
public:

	void RegisterAbilitiesFromDisk(UObject* Outer, const FString& FolderName, TMap<int, UAbilityDataObject*>* AbilitiesOut);

	void RegisterAbilitiesFromSerialized(UObject* Outer, const TArray<FSerializedAbilityData>& AbilityData, TMap<int, UAbilityDataObject*>* AbilitiesOut);

	static UAbilityDataObject* GetAbilityDataObjectFromMap(TMap<int, UAbilityDataObject*>& Map, int AbilityId);

	UFUNCTION(BlueprintPure)
		/* Get the global ability object corresponding to the ability id. Not for use in battle. */
		UAbilityDataObject* GetAbilityDataObject(int AbilityId);

	static int GetAbilityIdFromMap(TMap<FName, int>& Map, FName AbilityName);

	UFUNCTION(BlueprintPure)
		/**/
		int GetAbilityId(FName AbilityName);

	static FName GetAbilityNameFromMap(TMap<int, UAbilityDataObject*>& Map, int AbilityId);

	UFUNCTION(BlueprintPure)
		/**/
		FName GetAbilityName(int AbilityId);

	static bool IsValidAbilityFromMap(TMap<int, UAbilityDataObject*>& Map, int AbilityId);

	UFUNCTION(BlueprintPure)
		/**/
		bool IsValidAbility(int AbilityId);

	static UClass* GetAbilityClassFromMap(TMap<int, UAbilityDataObject*>& Map, int AbilityId);

	UFUNCTION(BlueprintPure)
		/**/
		UClass* GetAbilityClass(int AbilityId);

	static UClass* GetActorClassFromMap(TMap<int, UAbilityDataObject*>& Map, int AbilityId);

	UFUNCTION(BlueprintPure)
		/**/
		UClass* GetActorClass(int AbilityId);
};
