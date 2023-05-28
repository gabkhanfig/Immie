// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <Immie/Core/Structures.h>
#include <Immie/Util/Json/BlueprintJsonObject.h>
#include <Immie/Immies/SpecieDataTypes.h>
#include "SpecieDataObject.generated.h"

class UImmieObject;
class AImmieCharacter;
class UImmieSpawnData;

UCLASS(Blueprintable)
/* Data object storing specie data. Override in blueprints for custom specie data.
Automatically loads references to other specie classes.
Ensure those classes follow correct naming conventions (example SnamdonCharacter_BP). 
The value of SpecieName MUST be set. From the above example, the name would be "snamdon". */
class IMMIE_API USpecieDataObject : public UObject
{
	GENERATED_BODY()

private:

	UPROPERTY()
		/**/
		UClass* ObjectClass;

	UPROPERTY()
		/**/
		UClass* CharacterClass;

	UPROPERTY()
		/**/
		UClass* SpawnDataClass;

protected:

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (NoResetToDefault))
		/* This must be set in blueprint. Specie name should be lower case formatted. */
		FName SpecieName;

	UPROPERTY(BlueprintReadWrite)
		/**/
		int TypeBitmask;

	UPROPERTY(BlueprintReadWrite)
		/**/
		FBaseStats BaseStats;

	UPROPERTY(BlueprintReadWrite, Category = "Ability Learnset")
		/**/
		FSpecieLearnset Learnset;

private:

	void LoadLearnsets(const FJsonObjectBP& Json);

	TArray<FName> LoadAbilitySet(const FJsonObjectBP& Json, const FString& ArrayName);

	FJsonArrayBP AbilitySetToJsonArray(const TArray<FName>& AbilitySet);

protected:

	UFUNCTION(BlueprintImplementableEvent)
		/**/
		void BP_LoadSpecieJsonData(const FJsonObjectBP& Json);

	UFUNCTION(BlueprintNativeEvent)
		/**/
		FJsonObjectBP BP_SpecieDataToJson();
	
public:

	USpecieDataObject();

	void PostLoad() override;

	/* Load the specie's relevant UClass* references. */
	void LoadClasses();

	static const FString& GetImmiesBlueprintFolder();

	static USpecieDataObject* CreateSpecieDataObject(UObject* Outer, UClass* DataObjectClass);

	UFUNCTION(BlueprintCallable)
		/**/
		void CheckClassesValid();

	void LoadSpecieJsonData(const FJsonObjectBP& Json, bool LoadJsonLearnsets = true);

	FJsonObjectBP SpecieDataToJson();

	UFUNCTION(BlueprintPure)
		/**/
		UClass* GetImmieObjectClass() const { return ObjectClass; }

	UFUNCTION(BlueprintPure)
		/**/
		UClass* GetImmieCharacterClass() const { return CharacterClass; }

	UFUNCTION(BlueprintPure)
		/**/
		UClass* GetSpawnDataClass() const { return SpawnDataClass; }

	UFUNCTION(BlueprintPure)
		/**/
		FName GetSpecieName() const { return SpecieName; }
	
	UFUNCTION(BlueprintPure)
		/**/
		int GetTypeBitmask() const { return TypeBitmask; }

	UFUNCTION(BlueprintPure)
		/**/
		FBaseStats GetBaseStats() const { return BaseStats; }

	UFUNCTION(BlueprintPure)
		/**/
		const FSpecieLearnset& GetLearnset() const { return Learnset; }
};
