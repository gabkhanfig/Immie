// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <Immie/ImmieCore.h>
#include <Immie/Util/Json/BlueprintJsonObject.h>
#include <Immie/Immies/SpecieDataTypes.h>
#include "ImmieObject.generated.h"

#define MAX_ABILITY_COUNT 4

class USpecieDataObject;

/**
 * 
 */
UCLASS(Blueprintable)
class IMMIE_API UImmie : public UObject
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (NoResetToDefault))
		/**/
		FName SpecieName;

	UPROPERTY()
		/* The current health of this Immie. If it ever exceeds the maximum given the specie and base stats, it should clamp to the max. */
		float Health;

	UPROPERTY()
		/**/
		int Xp;

	UPROPERTY()
		/**/
		FBaseStats StatLevels;

	UPROPERTY()
		/**/
		TArray<FName> Abilities;

	UPROPERTY()
		/**/
		FString DisplayName;

public:

	UImmie();

	void PostLoad() override;

	UFUNCTION(BlueprintCallable)
		/* Make a new Immie object instance, also setting the specie id. */
		static UImmie* NewImmieObject(UObject* Outer, FName _SpecieName);

	/* Load the data of this Immie from json. The specie id will already need to be known before hand to match the UClass, so it is passed in here to avoid extra work. */
	void LoadJsonData(const FJsonObjectBP& Json);

	/* Save this Immie's data to a json object. */
	FJsonObjectBP SaveJsonData();

	UFUNCTION(BlueprintCallable)
		/* Make a copy of this Immie object with a different outer */
		UImmie* MakeCopy(UObject* Outer);

	UFUNCTION(BlueprintCallable)
		/**/
		static TArray<UImmie*> MakeTeamCopy(const TArray<UImmie*>& Team, UObject* Outer);

	UFUNCTION(BlueprintCallable)
		/* Convert a team to a json object. */
		static FJsonObjectBP TeamToJson(const TArray<UImmie*>& Team, const FString& ArrayFieldName);

	UFUNCTION(BlueprintCallable)
		/* Convert a json object to a team of Immie objects. */
		static TArray<UImmie*> JsonToTeam(const FJsonObjectBP& TeamJsonObject, const FString& ArrayFieldName, UObject* Outer);

	/* Change the outer object that owns this Immie object instance. Useful from handing off wild Immies to the player team for example. */
	void ChangeOuter(UObject* NewOuter);

	UFUNCTION(BlueprintCallable)
		/* Releases the Immie into the wild! Frees up memory. */
		void Release();
	
	UFUNCTION(BlueprintCallable)
		/* Heal this Immie to full health. Uses system default base stats. */
		void Heal();

	UFUNCTION(BlueprintCallable)
		/* Heal this Immie to full health using custom base stats. */
		void HealFromBaseStats(uint8 HealthBaseStat);

	UFUNCTION(BlueprintPure)
		/* Get the name of this specie from it's specie id. NOT NECESSARILY display name. */
		FName GetSpecieName() const { return SpecieName; }

	UFUNCTION(BlueprintPure)
		/**/
		inline float GetHealth() const { return Health; }

	UFUNCTION(BlueprintCallable)
		/* Forcefully set this Immie's health. Is clamped. */
		void SetHealth(float NewHealth);

	UFUNCTION(BlueprintPure)
		/**/
		inline int GetXp() const { return Xp; }

	UFUNCTION(BlueprintCallable)
		/* Set the xp value for this Immie. Is clamped. */
		void SetXp(int NewXp);

	UFUNCTION(BlueprintPure)
		/**/
		uint8 GetLevel() const;

	UFUNCTION(BlueprintCallable)
		/* Set the xp value for this Immie to be the minimum required to get a certain level. Is clamped. */
		void SetLevel(uint8 NewLevel);

	UFUNCTION(BlueprintPure)
		/**/
		inline FBaseStats GetStatLevels() const { return StatLevels; }

	UFUNCTION(BlueprintPure)
		/**/
		inline TArray<FName> GetAbilityNames() const { return Abilities; }

	UFUNCTION(BlueprintCallable)
		/**/
		void SetAbilityNames(const TArray<FName>& NewAbilities);

	UFUNCTION(BlueprintPure)
		/**/
		inline FString GetDisplayName() const { return DisplayName; }

	UFUNCTION(BlueprintCallable)
		/**/
		void SetDisplayName(const FString& _DisplayName);

	UFUNCTION(BlueprintPure)
		/**/
		float GetMaxHealth(bool UseBaseStatOverride = false, uint8 BaseHealth = 0) const;

	UFUNCTION(BlueprintPure)
		/**/
		float GetAttack(bool UseBaseStatOverride = false, uint8 BaseAttack = 0) const;

	UFUNCTION(BlueprintPure)
		/**/
		float GetDefense(bool UseBaseStatOverride = false, uint8 BaseDefense = 0) const;

	UFUNCTION(BlueprintPure)
		/**/
		float GetSpeed(bool UseBaseStatOverride = false, uint8 BaseSpeed = 0) const;

	UFUNCTION(BlueprintPure)
		/* Get the 4 stats this Immie would use in battle. These are max health, unmodified attack, unmodified defense, and unmodified speed. */
		FBattleStats GetStats(bool UseBaseStatOverride = false, uint8 BaseHealth = 0, uint8 BaseAttack = 0, uint8 BaseDefense = 0, uint8 BaseSpeed = 0) const;

protected:

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Load Json Data")
		/* Blueprint-side data loading for this Immie object. Loads from the specie-specific subobject within the parent json object handled in cpp.
		Any variables defined in UImmie itself do not need to be loaded as they are handled before this event is called. */
		void BP_LoadJsonData(const FJsonObjectBP& Json);

	UFUNCTION(BlueprintNativeEvent, DisplayName = "Save Json Data")
		/* Blueprint-side data saving for this Immie object. Saves it as a subobject within the parent json object handled in cpp.
		Any variables defined in UImmie itself do not need to be saved as they are handled before this event is called. */
		FJsonObjectBP BP_SaveJsonData();

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Make Copy")
		/* Blueprint-sided data copying for making a copy of this Immie object. See UImmie::MakeCopy(). */
		void BP_MakeCopy(UImmie* Copy);
	
};
