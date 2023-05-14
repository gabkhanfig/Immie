// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BattleAbilityManager.generated.h"

class ABattleInstance;
class UAbility;
class UAbilityDataObject;

UCLASS( ClassGroup=(Battle), meta=(BlueprintSpawnableComponent) )
class IMMIE_API UBattleAbilityManager : public UActorComponent
{
	GENERATED_BODY()

	friend class ABattleInstance;

private:

	UPROPERTY()
		/* Owning battle instance. */
		ABattleInstance* BattleInstance;

	UPROPERTY()
		/**/
		TMap<FName, UAbilityDataObject*> AbilitiesOverride;
	
	UPROPERTY()
		/* Does this manager use it's own data or the system default. */
		uint8 bUseOverride : 1;

public:	

	UBattleAbilityManager();

	void Initialize(ABattleInstance* _BattleInstance, bool UseOverride, FString OverrideFolder);

	void LoadOverrideAbilityData(const FString& FolderName);

	void SyncToClients();

	UFUNCTION(NetMulticast, Reliable)
		/**/
		void RecieveSerializedAbilityData(ABattleInstance* _BattleInstance, const TArray<FSerializedAbilityData>& AbilityData);

	UFUNCTION(BlueprintPure)
		/**/
		UAbilityDataObject* GetAbilityDataObject(FName AbilityName);
};
