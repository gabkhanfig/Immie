// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ImmieType.h"
#include "BattleTypeComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class IMMIE_API UBattleTypeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBattleTypeComponent();

	UFUNCTION(BlueprintCallable)
		/**/
		void Initialize(FTypeConstants _DamageMultipliers, const TArray<FImmieType> _TypesData);

	UFUNCTION(BlueprintPure)
		/**/
		float TotalTypeDamageMultiplier(const TArray<EImmieType>& AttackingTypes) const;

	UFUNCTION(BlueprintPure)
		/* Gets the colour of the first type. Client specific. Independent of battle conditions. */
		FLinearColor GetColor() const;

	UFUNCTION(BlueprintPure)
		/**/
		TArray<EImmieType> GetTypes() const;

	UFUNCTION(BlueprintCallable)
		/**/
		void CopyFrom(const UBattleTypeComponent* ComponentToCopy);

	/* Called by server to perform synchronizing of type data to connected clients. */
	void SyncToClients();

private:

	UFUNCTION(NetMulticast, Reliable)
		/**/
		void SetClientTypeData(FTypeBitmask _Types, const TArray<FImmieType>& _TypesData, FTypeConstants _DamageMultipliers);

private:

	UPROPERTY()
		/* Bitmask ensures no duplicate enum entries. */
		FTypeBitmask Types;

	UPROPERTY()
		TArray<FImmieType> TypesData;

	UPROPERTY()
		FTypeConstants DamageMultipliers;

};
