// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <Immie/Util/Json/BlueprintJsonObject.h>
#include "TypeConstants.h"
#include "ImmieType.generated.h"

UENUM(BlueprintType)
enum class EImmieType : uint8 {
	Neutral		= 0,
	Fire			= 1,
	Water			= 2,
	Nature		= 3,
	Standard	= 4,
	Electric	= 5,
	Air				= 6,
	Ground		= 7,
	Metal			= 8,
	Light			= 9,
	Dark			= 10,
	Dragon		= 11
};

USTRUCT(BlueprintType)
/* Structure containing the bitmask data for a type.
For blueprint use, see UTypeBitmaskBlueprintWrapper and it's functions to access FTypeBitmask functions. */
struct FTypeBitmask {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
		int Bitmask;

	FTypeBitmask();

	void AddType(EImmieType Type);

	void RemoveType(EImmieType Type);

	bool HasType(EImmieType Type) const;

	TArray<EImmieType> GetTypes() const;

	void AddTypes(const TArray<EImmieType>& Types);

	int TypeCount() const;

	void Clear();

	static FTypeBitmask Combine(const FTypeBitmask Left, const FTypeBitmask Right);

	static FTypeBitmask FromJsonTypesArrayField(const FJsonArrayBP& JsonArray);
};

UCLASS()
class IMMIE_API UTypeBitmaskBlueprintWrapper : public UBlueprintFunctionLibrary {
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable)
		static FTypeBitmask AddType(UPARAM(ref) FTypeBitmask& Bitmask, EImmieType Type) { Bitmask.AddType(Type); return Bitmask; }

	UFUNCTION(BlueprintCallable)
		static FTypeBitmask RemoveType(UPARAM(ref) FTypeBitmask& Bitmask, EImmieType Type) { Bitmask.RemoveType(Type); return Bitmask; }

	UFUNCTION(BlueprintPure)
		static bool HasType(FTypeBitmask Bitmask, EImmieType Type) { return Bitmask.HasType(Type); }

	UFUNCTION(BlueprintCallable)
		static FTypeBitmask AddTypes(UPARAM(ref) FTypeBitmask& Bitmask, const TArray<EImmieType>& Types) { Bitmask.AddTypes(Types); return Bitmask; }

	UFUNCTION(BlueprintPure)
		static int TypeCount(FTypeBitmask Bitmask) { return Bitmask.TypeCount(); }

	UFUNCTION(BlueprintCallable)
		static FTypeBitmask Clear(FTypeBitmask& Bitmask) { Bitmask.Clear(); return Bitmask; }

	UFUNCTION(BlueprintPure)
		static FTypeBitmask Combine(FTypeBitmask First, FTypeBitmask Second) { return FTypeBitmask::Combine(First, Second); }

};

USTRUCT(BlueprintType)
/* Structure containing bitmasks for a type, and its weakness and resistances. */
struct FImmieType {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
		FTypeBitmask Type;

	UPROPERTY(BlueprintReadWrite)
		FTypeBitmask Weaknesses;

	UPROPERTY(BlueprintReadWrite)
		FTypeBitmask Resistances;

	static FImmieType FromJson(EImmieType Type, const FJsonObjectBP& Json);

	FJsonObjectBP ToJson();

};