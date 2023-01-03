// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "StringUtils.generated.h"

UCLASS()
class IMMIE_API UStringUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure)
		/* Get a new string with an upper cased first letter, preserving the rest of the string. */
		static FString ToUpperFirstLetter(const FString& String);

};
