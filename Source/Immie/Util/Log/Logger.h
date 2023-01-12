// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Logger.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogImmie, Log, All);

UENUM(BlueprintType)
enum EiLogVerbosity
{
	/* Not used */
	LogVerbosity_NoLogging = 0									UMETA(DisplayName = "No Logging"),

	/* Always prints a fatal error to console (and log file) and crashes (even if logging is disabled) */
	LogVerbosity_Fatal = ELogVerbosity::Type::Fatal				UMETA(DisplayName = "Fatal"),

	/* Prints an error to console (and log file). Commandlets and the editor collect and report errors. Error messages result in commandlet failure. */
	LogVerbosity_Error = ELogVerbosity::Type::Error				UMETA(DisplayName = "Error"),

	/* Prints a warning to console (and log file). Commandlets and the editor collect and report warnings. Warnings can be treated as an error.*/
	LogVerbosity_Warning = ELogVerbosity::Type::Warning			UMETA(DisplayName = "Warning"),

	/* Prints a message to console (and log file) */
	LogVerbosity_Display = ELogVerbosity::Type::Display			UMETA(DisplayName = "Display"),

	/* Prints a message to a log file (does not print to console) */
	LogVerbosity_Log = ELogVerbosity::Type::Log					UMETA(DisplayName = "Log"),

	/* Prints a verbose message to a log file (if Verbose logging is enabled for the given category, usually used for detailed logging) */
	LogVerbosity_Verbose = ELogVerbosity::Type::Verbose			UMETA(DisplayName = "Verbose"),

	/* Prints a verbose message to a log file (if VeryVerbose logging is enabled, usually used for detailed logging that would otherwise spam output) */
	LogVerbosity_VeryVerbose = ELogVerbosity::Type::VeryVerbose	UMETA(DisplayName = "Very Verbose")
};

UCLASS()
class IMMIE_API ULogger : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
		/* Print a message to the display and output log with optional verbosity specifiers. */
		static void DisplayLog(const FString& Message, TEnumAsByte<EiLogVerbosity> Verbosity = LogVerbosity_Display, float Duration = 10.f, FLinearColor Color = FLinearColor(0.77, 0.3, 0.89, 1));

	UFUNCTION(BlueprintCallable)
		/* Log a message to the console / output log with optional verbosity specifiers. */
		static void Log(const FString& Message, TEnumAsByte<EiLogVerbosity> Verbosity = LogVerbosity_Display);

	UFUNCTION(BlueprintCallable, meta=(DisplayName="iLog"))
		/* Log a message. If not on a dedicated server, also prints the message to the screen. Verbosity switches can forcefully display custom message types. */
		static void iLog(const FString& Message, TEnumAsByte<EiLogVerbosity> Verbosity = LogVerbosity_Display, float Duration = 10.f, FLinearColor Color = FLinearColor(0.77, 0.3, 0.89, 1));
	
};

/* Shorthand way of calling ULogger::iLog(). */
FORCEINLINE void iLog(const FString& Message, TEnumAsByte<EiLogVerbosity> Verbosity = LogVerbosity_Display, float Duration = 10.f, FLinearColor Color = FLinearColor(0.77, 0.3, 0.89, 1)) 
{
	ULogger::iLog(Message, Verbosity, Duration, Color);
}
