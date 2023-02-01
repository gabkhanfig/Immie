// Fill out your copyright notice in the Description page of Project Settings.


#include "ImmieMovementComponent.h"

UImmieMovementComponent::UImmieMovementComponent()
{
	bOrientRotationToMovement = true;
	RotationRate = FRotator(0.0f, 900.0f, 0.0f); 
	JumpZVelocity = 1200.f;
	AirControl = 0.6;
	GravityScale = 3.f;

	MaxWalkSpeed = 900.f;
	MaxFlySpeed = 500.f;
}
