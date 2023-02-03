// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Interfaces/BattleActor.h"
#include "FloatingBattleHealthbar.generated.h"

/**
 * 
 */
UCLASS()
class IMMIE_API UFloatingBattleHealthbar : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	UPROPERTY()
		/**/
		TScriptInterface<IBattleActor> BattleActor;

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

	UFloatingBattleHealthbar(const FObjectInitializer& ObjectInitializer);

	void SetBattleActor(TScriptInterface<IBattleActor> NewBattleActor);

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Initialize")
		/**/
		void BP_Initialize();

	UFUNCTION(BlueprintPure)
		/**/
		TScriptInterface<IBattleActor> GetBattleActor() const { return BattleActor; }

	UFUNCTION(BlueprintPure, Category = "Health")
		/**/
		float GetCurrentHealth() const;

	UFUNCTION(BlueprintPure, Category = "Health")
		/**/
		float GetMaxHealth() const;

	UFUNCTION(BlueprintPure, Category = "Health")
		/**/
		float GetHealthPercent() const;

	UFUNCTION(BlueprintPure)
		/**/
		FString GetDisplayName() const;

	UFUNCTION(BlueprintPure, Category = "Type")
		/**/
		FLinearColor GetTypeColor(UImmieType* Type) const;

	UFUNCTION(BlueprintPure)
		/**/
		FLinearColor GetColorHealthPercent(float Percent) const;

};
