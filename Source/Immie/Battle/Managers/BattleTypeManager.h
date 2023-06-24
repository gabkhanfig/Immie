// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <Immie/Type/TypeConstants.h>
#include <Immie/Type/ImmieType.h>
#include "BattleTypeManager.generated.h"

class ABattleInstance;
class UBattleTypeComponent;

UCLASS( ClassGroup=(Battle), meta=(BlueprintSpawnableComponent) )
class IMMIE_API UBattleTypeManager : public UActorComponent
{
	GENERATED_BODY()

	friend class ABattleInstance;

private:

	UPROPERTY()
		/* Owning battle instance. */
		ABattleInstance* BattleInstance;

	UPROPERTY()
		/**/
		TMap<EImmieType, FImmieType> TypesOverride;

	UPROPERTY()
		/**/
		FTypeConstants TypeConstantsOverride;

	UPROPERTY()
		/* Does this manager use it's own data or the system default. */
		uint8 bUseOverride : 1;

public:	

	UBattleTypeManager();

	void Initialize(ABattleInstance* _BattleInstance, bool UseOverride, FString OverrideFolder);

	void LoadOverrideTypeData(const FString& FolderName);

	void SyncToClients();

	UFUNCTION(BlueprintPure)
		/* Uses a SINGULAR type bitmask. Only one bit should be set. */
		FImmieType GetType(EImmieType Type);

	UFUNCTION(BlueprintPure)
		/**/
		FTypeConstants GetTypeConstants();

	UFUNCTION(BlueprintPure)
		/**/
		TArray<FImmieType> GetTypes(FTypeBitmask TypeBitmask);

		
};
