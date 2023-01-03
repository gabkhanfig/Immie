// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <Immie/Immies/SpecieDataTypes.h>
#include "BattleSpecieManager.generated.h"

class ABattleInstance;
class UImmie;
class AImmieCharacter;
class USpecieDataObject;

UCLASS( ClassGroup=(Battle), meta=(BlueprintSpawnableComponent) )
class IMMIE_API UBattleSpecieManager : public UActorComponent
{
	GENERATED_BODY()

	friend class ABattleInstance;

private:

	UPROPERTY()
		/* Owning battle instance. */
		ABattleInstance* BattleInstance;

	UPROPERTY()
		/**/
		TMap<int, USpecieDataObject*> SpeciesOverride;

	UPROPERTY()
		/* Does this manager use it's own data or the system default. */
		uint8 bUseOverride : 1;

public:

	UBattleSpecieManager();

	void Initialize(ABattleInstance* _BattleInstance, bool UseOverride, FString OverrideFolder);

	void LoadOverrideSpecieData(const FString& FolderName);

	void SyncToClients();

	UFUNCTION(NetMulticast, Reliable)
		/**/
		void RecieveSerializedSpecieData(ABattleInstance* _BattleInstance, const TArray<FSerializedSpecieData>& SpecieData);

	UFUNCTION(BlueprintPure)
		/**/
		USpecieDataObject* GetSpecieDataObject(int SpecieId);

	UFUNCTION(BlueprintPure)
		/**/
		int GetSpecieTypeBitmask(int SpecieId);

	UFUNCTION(BlueprintPure)
		/**/
		FBaseStats GetSpecieBaseStats(int SpecieId);

		
};
