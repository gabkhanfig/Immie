// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <Immie/Util/Json/BlueprintJsonObject.h>
#include "TypeConstants.h"
#include "ImmieType.generated.h"

enum EImmieType : int {
	Type_Neutral = 0,
	Type_Fire = 1,
	Type_Water = 1 << 1,
	Type_Nature = 1 << 2,
	Type_Standard = 1 << 3,
	Type_Electric = 1 << 4,
	Type_Air = 1 << 5,
	Type_Ground = 1 << 6,
	Type_Metal = 1 << 7,
	Type_Light = 1 << 8,
	Type_Dark = 1 << 9,
	Type_Dragon = 1 << 10
};

UCLASS()
class IMMIE_API UImmieType : public UActorComponent
{
	GENERATED_BODY()

private:

	UPROPERTY()
		/* Bitmask of this type */
		int TypeBitmask;

	UPROPERTY()
		/* Bitmask of this type's weaknesses */
		int Weaknesses;

	UPROPERTY()
		/* Bitmask of this type's resistances */
		int Resistances;

	UPROPERTY()
		/**/
		FTypeConstants TypeConstants;

	UPROPERTY()
		/* Color corrsponding to this type. Is local to clients. */
		FLinearColor Color;

protected:

	UFUNCTION(NetMulticast, Reliable)
		/**/
		void SetClientTypeData(int _TypeBitmask, int _Weaknesses, int _Resistances, const FTypeConstants& _TypeConstants);

public:	
	// Sets default values for this component's properties
	UImmieType();

	static UImmieType* FromJson(UObject* Outer, const FName& Name, const FJsonObjectBP& Json, const FTypeConstants& _TypeConstants);

	UFUNCTION(BlueprintPure)
		/**/
		float GetTypeEffectiveness(int AttackTypeBitmask);

	UFUNCTION(BlueprintPure)
		/**/
		static float TotalTypeDamageMultiplier(const TArray<UImmieType*>& AttackingType, const TArray<UImmieType*>& DefendingType);

	UFUNCTION(BlueprintPure)
		/**/
		FLinearColor GetColor() const { return Color; }

	UFUNCTION(BlueprintPure)
		/**/
		FName GetTypeName() const;

	/* Called by server to perform synchronizing of type data to connected clients. */
	void SyncToClients();
};
