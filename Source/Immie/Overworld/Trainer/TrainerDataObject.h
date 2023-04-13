// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <Immie/Util/Json/BlueprintJsonObject.h>
#include "TrainerDataObject.generated.h"

class UImmie;

/**
 * 
 */
UCLASS(Blueprintable)
class IMMIE_API UTrainerDataObject : public UObject
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		/* This must be set in blueprint. Trainer name should be lower case formatted. */
		FName TrainerName;

	UPROPERTY(BlueprintReadOnly)
		/**/
		TArray<UImmie*> Team;

	UPROPERTY(BlueprintReadOnly)
		/**/
		FString BattleIntroDialogue;

	UPROPERTY(BlueprintReadOnly)
		/**/
		FString BattleEndWinDialogue;

	UPROPERTY(BlueprintReadOnly)
		/**/
		FString BattleEndLoseDialogue;

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		/* Get a copy of the team using a specified team owner. See UImmie::MakeTeamCopy(). */
		TArray<UImmie*> GetTeamCopy(UObject* Owner);

	void LoadTrainerJsonData(const FJsonObjectBP& Json);

	static const FString& GetTrainerBlueprintFolder();

	UFUNCTION(BlueprintPure)
		/**/
		FName GetTrainerName() const { return TrainerName; }

	UFUNCTION(BlueprintPure)
		/**/
		FString GetBattleIntroDialogue() const { return BattleIntroDialogue; }

	UFUNCTION(BlueprintPure)
		/**/
		FString GetBattleEndWinDialogue() const { return BattleEndWinDialogue; }

	UFUNCTION(BlueprintPure)
		/**/
		FString GetBattleEndLoseDialogue() const { return BattleEndLoseDialogue; }

protected:

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Load Trainer Json Data")
		/* Blueprint-side data loading for this Trainer Data Object. See UTrainerDataObject::LoadTrainerJsonData(). */
		void BP_LoadTrainerJsonData(const FJsonObjectBP& Json);

};
