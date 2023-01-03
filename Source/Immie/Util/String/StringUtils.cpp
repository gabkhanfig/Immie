// Fill out your copyright notice in the Description page of Project Settings.


#include "StringUtils.h"

FString UStringUtils::ToUpperFirstLetter(const FString& String)
{
	const FString FirstLetterCapital = String.Left(1).ToUpper();
	return FirstLetterCapital + String.RightChop(1);
}
