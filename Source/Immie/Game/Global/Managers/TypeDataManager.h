// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameDataManager.h"
#include <Immie/Type/TypeConstants.h>
#include <Immie/Type/ImmieType.h>
#include "TypeDataManager.generated.h"

class UBattleTypeComponent;

/**
 * 
 */
UCLASS()
class IMMIE_API UTypeDataManager : public UGameDataManager
{
	GENERATED_BODY()

protected:

	virtual void LoadDefaultGameData() override;

public:

	UTypeDataManager();

	static void RegisterTypeConstants(const FString& FolderName, FTypeConstants& TypeConstantsOut);

	static void RegisterTypes(UObject* Outer, const FString& FolderName, TMap<EImmieType, FImmieType>* TypesOut, const FTypeConstants& TypeConsts);

	static void RegisterType(const FName& Name, UObject* Outer, const FString& FolderName, TMap<EImmieType, FImmieType>* TypesOut, const FTypeConstants& TypeConsts);

	UFUNCTION(BlueprintPure)
		/**/
		static bool IsValidTypeName(FName TypeName);

	UFUNCTION(BlueprintPure)
		/* Useful for checking if valid name without adding an FName entry. Much slower than FName access though. */
		static bool IsValidTypeNameString(FString TypeNameString);

	UFUNCTION(BlueprintPure)
		/**/
		static EImmieType GetTypeEnum(FName TypeName);

	UFUNCTION(BlueprintPure)
		/**/
		static FName GetTypeName(EImmieType Type);

	UFUNCTION(BlueprintPure)
		/**/
		FLinearColor GetTypeColor(EImmieType Type);

	UFUNCTION(BlueprintPure)
		/**/
		FImmieType GetType(EImmieType Type);

	UFUNCTION(BlueprintPure)
		/**/
		FTypeConstants GetTypeConstants() const { return TypeConstants; }
	
private:

	/* Global variable that contains all FName's in SpecieNames, for fast look-up. */
	static const TMap<FName, EImmieType> TypeEnums;
	/* Index using EImmieType cast to int */
	static const TArray<FName> TypeNames;
	/* Fast access to check valid type names as strings without adding FName entries. */
	static const TSet<FString> TypeStringNames;

	UPROPERTY()
		/* Map of all battle types and their relevant data */
		TMap<EImmieType, FImmieType> Types;

	UPROPERTY()
		/**/
		FTypeConstants TypeConstants;

protected:

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		/**/
		FLinearColor NeutralColor;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		/**/
		FLinearColor FireColor;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		/**/
		FLinearColor WaterColor;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		/**/
		FLinearColor NatureColor;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		/**/
		FLinearColor StandardColor;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		/**/
		FLinearColor ElectricColor;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		/**/
		FLinearColor AirColor;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		/**/
		FLinearColor GroundColor;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		/**/
		FLinearColor MetalColor;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		/**/
		FLinearColor LightColor;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		/**/
		FLinearColor DarkColor;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		/**/
		FLinearColor DragonColor;

};
