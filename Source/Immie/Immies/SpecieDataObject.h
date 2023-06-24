// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <Immie/Core/Structures.h>
#include <Immie/Util/Json/BlueprintJsonObject.h>
#include <Immie/Immies/SpecieDataTypes.h>
#include <Immie/Type/ImmieType.h>
#include "SpecieDataObject.generated.h"

class UImmie;
class AImmieCharacter;
class UImmieSpawnData;

UCLASS(Blueprintable)
/* Data object storing specie data. Override in blueprints for custom specie data.
Automatically loads references to other specie classes.
Ensure those classes follow correct naming conventions (example BP_SnamdonCharacter). 
The value of SpecieName MUST be set. From the above example, the name would be "snamdon". */
class IMMIE_API USpecieDataObject : public UObject
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (NoResetToDefault))
		/* Automatically set given the name of the blueprint class. */
		FName SpecieName;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (NoResetToDefault))
		/* Automatically set given the name of the blueprint class. If the class doesn't exist, create it and restart the engine.
		Must be formatted as BP_*SpecieName*Object */
		TSubclassOf<UImmie> ObjectClass;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (NoResetToDefault))
		/* Automatically set given the name of the blueprint class. If the class doesn't exist, create it and restart the engine.
		Must be formatted as BP_*SpecieName*Character */
		TSubclassOf<AImmieCharacter> CharacterClass;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (NoResetToDefault))
		/* Automatically set given the name of the blueprint class. If the class doesn't exist, create it and restart the engine.
		Must be formatted as BP_*SpecieName*SpawnData */
		TSubclassOf<UImmieSpawnData> SpawnDataClass;

	UPROPERTY(BlueprintReadWrite)
		/**/
		FTypeBitmask Type;

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

	TSubclassOf<UImmie> FetchSpecieObjectClass() const;

	TSubclassOf<AImmieCharacter> FetchSpecieCharacterClass() const;

	TSubclassOf<UImmieSpawnData> FetchSpecieSpawnDataClass() const;

	static const FString& GetImmiesBlueprintFolder();

	static USpecieDataObject* CreateSpecieDataObject(UObject* Outer, UClass* DataObjectClass);

	void LoadSpecieJsonData(const FJsonObjectBP& Json, bool LoadJsonLearnsets = true);

	FJsonObjectBP SpecieDataToJson();

	UFUNCTION(BlueprintPure)
		/**/
		TSubclassOf<UImmie> GetImmieObjectClass() const { return ObjectClass; }

	UFUNCTION(BlueprintPure)
		/**/
		TSubclassOf<AImmieCharacter> GetImmieCharacterClass() const { return CharacterClass; }

	UFUNCTION(BlueprintPure)
		/**/
		TSubclassOf<UImmieSpawnData> GetSpawnDataClass() const { return SpawnDataClass; }

	UFUNCTION(BlueprintPure)
		/**/
		FName GetSpecieName() const { return SpecieName; }
	
	UFUNCTION(BlueprintPure)
		/**/
		FTypeBitmask GetType() const { return Type; }

	UFUNCTION(BlueprintPure)
		/**/
		FBaseStats GetBaseStats() const { return BaseStats; }

	UFUNCTION(BlueprintPure)
		/**/
		const FSpecieLearnset& GetLearnset() const { return Learnset; }
};
