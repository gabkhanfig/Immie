// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Immie/ImmieCore.h>
#include "ImmieBattleHud.generated.h"

class AImmieCharacter;
class UImmieObject;
class UAbility;
class UBattleTypeComponent;

/**
 * 
 */
UCLASS()
class IMMIE_API UImmieBattleHud : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY()
		/**/
		AImmieCharacter* ImmieCharacter;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Color")
		/**/
		FLinearColor FullHealthbar;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Color")
		/**/
		FLinearColor HalfHealthbar;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Color")
		/**/
		FLinearColor QuarterHealthbar;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Color")
		/**/
		FLinearColor NoneHealthbar;

public:

	UImmieBattleHud(const FObjectInitializer& ObjectInitializer);

	void SetImmieCharacter(AImmieCharacter* _ImmieCharacter);

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Initialize")
		/**/
		void BP_Initialize();

	UFUNCTION(BlueprintPure)
		/**/
		FORCEINLINE AImmieCharacter* GetImmieCharacter() const { return ImmieCharacter; }

	UFUNCTION(BlueprintPure, Category = "Immie")
		/* Get the value between 0 and 1 that the Immie's current health is to max. */
		float GetHealthPercent() const;

	UFUNCTION(BlueprintPure, Category = "Immie")
		/* Get the Immie's display name. */
		FString GetDisplayName() const;

	UFUNCTION(BlueprintPure, Category = "Immie")
		/**/
		FString GetCurrentHealthString();

	UFUNCTION(BlueprintPure, Category = "Immie")
		/**/
		FString GetMaxHealthString();

	UFUNCTION(BlueprintPure, Category = "Type")
		/**/
		FLinearColor GetTypeColor(UBattleTypeComponent* Type) const;

	UFUNCTION(BlueprintPure, Category = "Ability")
		/**/
		UAbility* GetAbility(int AbilityIndex) const;

	UFUNCTION(BlueprintPure, Category = "Ability")
		/* Get the ability name as a formatted string. */
		FString GetAbilityName(const UAbility* Ability) const;
	
	UFUNCTION(BlueprintPure, Category = "Ability")
		/**/
		bool IsAbilityOnCooldown(const UAbility* Ability) const;

	UFUNCTION(BlueprintPure, Category = "Ability")
		/**/
		float GetAbilityCooldownPercent(const UAbility* Ability) const;

	UFUNCTION(BlueprintPure, Category = "Ability")
		/**/
		int GetAbilityCurrentUses(const UAbility* Ability) const;

	UFUNCTION(BlueprintPure, Category = "Ability")
		/**/
		int GetAbilityMaxUses(const UAbility* Ability) const;

	UFUNCTION(BlueprintPure, Category = "Ability")
		/**/
		FString GetAbilityCurrentCooldownString(const UAbility* Ability) const;

	UFUNCTION(BlueprintPure, Category = "Immie")
		/**/
		FLinearColor GetColorForImmieHealth() const;

	UFUNCTION(BlueprintPure)
		/**/
		FLinearColor GetColorForHealthPercent(float Percent) const;
	
};
