// Fill out your copyright notice in the Description page of Project Settings.


#include "WarriorAnimInstance.h"
#include "GameFramework/PawnMovementComponent.h"

void UWarriorAnimInstance::NativeInitializeAnimation()
{
	if (!Pawn)
	{
		Pawn = TryGetPawnOwner();
	}
}

void UWarriorAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (!Pawn)
	{
		Pawn = TryGetPawnOwner();
	}

	if (Pawn)
	{
		FVector Speed = Pawn->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0);
		MovementSpeed = LateralSpeed.Size();
		IsFalling = Pawn->GetMovementComponent()->IsFalling();
	}
}