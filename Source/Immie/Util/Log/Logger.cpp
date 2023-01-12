// Fill out your copyright notice in the Description page of Project Settings.


#include "Logger.h"
#include <Immie/Core/Development.h>
#include "Kismet/KismetSystemLibrary.h"

DEFINE_LOG_CATEGORY(LogImmie);

constexpr FLinearColor ON_SCREEN_LOGGER_ERROR_COLOR = FLinearColor(0.9, 0.2, 0.2, 1);
constexpr FLinearColor ON_SCREEN_LOGGER_WARNING_COLOR = FLinearColor(0.9, 0.9, 0.3, 1);

void ULogger::DisplayLog(const FString& Message, TEnumAsByte<EiLogVerbosity> Verbosity, float Duration, FLinearColor Color)
{
#if DISPLAY_LOG_MESSAGES == true

	UWorld* World = GEngine->GetWorld();

	switch (Verbosity) {
	case LogVerbosity_Warning: 
		UKismetSystemLibrary::PrintString(World, "[WARNING]: " + Message, true, true, ON_SCREEN_LOGGER_WARNING_COLOR, Duration);
		break;
	case LogVerbosity_Error:
		UKismetSystemLibrary::PrintString(World, "[ERROR]: " + Message, true, true, ON_SCREEN_LOGGER_ERROR_COLOR, Duration);
		break;
	default:
		UKismetSystemLibrary::PrintString(World, Message, true, true, Color, Duration);
		break;
	}

#endif
}

void ULogger::Log(const FString& Message, TEnumAsByte<EiLogVerbosity> Verbosity)
{
	switch (Verbosity) {
	case LogVerbosity_Fatal:
		UE_LOG(LogImmie, Fatal, TEXT("%s"), *FString(Message));
		break;
	case LogVerbosity_Error:
		UE_LOG(LogImmie, Error, TEXT("%s"), *FString(Message));
		break;
	case LogVerbosity_Warning:
		UE_LOG(LogImmie, Warning, TEXT("%s"), *FString(Message));
		break;
	case LogVerbosity_Display:
		UE_LOG(LogImmie, Display, TEXT("%s"), *FString(Message));
		break;
	case LogVerbosity_Log:
		UE_LOG(LogImmie, Log, TEXT("%s"), *FString(Message));
		break;
	case LogVerbosity_Verbose:
		UE_LOG(LogImmie, Verbose, TEXT("%s"), *FString(Message));
		break;
	case LogVerbosity_VeryVerbose:
		UE_LOG(LogImmie, VeryVerbose, TEXT("%s"), *FString(Message));
		break;
	}
}

void ULogger::iLog(const FString& Message, TEnumAsByte<EiLogVerbosity> Verbosity, float Duration, FLinearColor Color)
{
	if (!IsRunningDedicatedServer()) {
		DisplayLog(Message, Verbosity, Duration, Color);
	}
	else {
		Log(Message, Verbosity);
	}
}
