// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS
#define WITH_IMMIE_AUTOMATION_TESTS true
#elif
#define WITH_IMMIE_AUTOMATION_TESTS false
#endif

#if WITH_IMMIE_AUTOMATION_TESTS

/**
 * 
 */
class IMMIE_API ImmieTest
{
public:
};

#endif