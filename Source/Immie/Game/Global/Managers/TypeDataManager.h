// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameDataManager.h"
#include <Immie/Type/TypeConstants.h>
#include "TypeDataManager.generated.h"

class UImmieType;

/**
 * 
 */
UCLASS()
class IMMIE_API UTypeDataManager : public UGameDataManager
{
	GENERATED_BODY()

private:

	UPROPERTY()
		/* Map of all battle types and their relevant data */
		TMap<int, UImmieType*> Types;

	UPROPERTY()
		/**/
		TMap<int, FName> TypeNames;

	UPROPERTY()
		/**/
		TMap<FName, int> TypeBitmasks;

	UPROPERTY()
		/**/
		FTypeConstants TypeConstants;

private:

	void MapTypeBitmasks();

protected:

	virtual void LoadDefaultGameData() override;

public:

	void RegisterTypeConstants(const FString& FolderName, FTypeConstants& TypeConstantsOut);

	void RegisterTypes(UObject* Outer, const FString& FolderName, TMap<int, UImmieType*>* TypesOut, const FTypeConstants& TypeConsts);

	void RegisterType(const FName& Name, UObject* Outer, const FString& FolderName, TMap<int, UImmieType*>* TypesOut, const FTypeConstants& TypeConsts);

	UFUNCTION(BlueprintPure)
		/**/
		int GetTypeBitmask(const FName& Name);

	UFUNCTION(BlueprintPure)
		/**/
		FName GetTypeName(int TypeBitmask);

	UFUNCTION(BlueprintPure)
		/**/
		bool IsValidType(int TypeBitmask);

	UFUNCTION(BlueprintPure)
		/**/
		UImmieType* GetType(int TypeBitmask);

	UFUNCTION(BlueprintPure)
		/**/
		UImmieType* GetTypeFromName(const FName& Name);

	UFUNCTION(BlueprintPure)
		/**/
		TArray<FName> GetTypeNames(int TypeBitmask);

	UFUNCTION(BlueprintPure)
		/**/
		FTypeConstants GetTypeConstants() const { return TypeConstants; }

	UFUNCTION(BlueprintPure)
		/**/
		TArray<UImmieType*> GetTypes(int TypeBitmask);

	static TArray<UImmieType*> GetMultipleTypesFromMap(int TypesBitmask, TMap<int, UImmieType*>& Map);

	/* Assumes the objects held within the array use "Type" as the field to read from. */
	int GetTypeBitmaskFromJsonArray(const FJsonArrayBP& JsonArray);
	
};
