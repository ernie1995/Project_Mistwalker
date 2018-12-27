// Fill out your copyright notice in the Description page of Project Settings.

#include "ThePawnMovementComponent.h"

#include "PlayerPawn.h"
#include "Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/GameFramework/Controller.h"
#include "Runtime/Engine/Classes/Components/ActorComponent.h"
#include "Runtime/Engine/Public/CollisionQueryParams.h"
#include "Runtime/Engine/Classes/GameFramework/Pawn.h"
#include "Engine/World.h"

void UThePawnMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Make sure that everything is still valid, and that we are allowed to move.
	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime))
	{
		return;
	}

	//get pawn
	APlayerPawn* Pawn = Cast<APlayerPawn>(PawnOwner);

	//check for blocking floor or ceiling
	FCollisionQueryParams Fall_TraceParams = FCollisionQueryParams(FName(TEXT("Projectile_Trace")), true, Pawn);
	Fall_TraceParams.bTraceComplex = true;
	Fall_TraceParams.bTraceAsyncScene = true;
	Fall_TraceParams.bReturnPhysicalMaterial = false;
	FHitResult Fall_Hit(ForceInit);
	FHitResult Ceiling_Hit(ForceInit);

	//check for ceiling
	GetWorld()->LineTraceSingleByChannel(Ceiling_Hit, Pawn->PawnBody->GetComponentLocation(), Pawn->PawnCamera->GetComponentLocation() + FVector(0.0f, 0.0f, 10.0f), ECC_Pawn, Fall_TraceParams);
	if (Ceiling_Hit.bBlockingHit == true)
	{
		Pawn->bJump = false;
		Pawn->JumpVector = FVector(0.0f);
		Pawn->bStandingUpBlocked = true;
	}
	else
	{
		Pawn->bStandingUpBlocked = false;
	}

	//check for floor
	GetWorld()->LineTraceSingleByChannel(Fall_Hit, Pawn->PawnBody->GetComponentLocation(), Pawn->PawnBody->GetComponentLocation() - FVector(0.0f,0.0f,20.0f), ECC_Pawn, Fall_TraceParams);
	if (Fall_Hit.bBlockingHit == false)
	{	
		SafeMoveUpdatedComponent(Pawn->FallVector, UpdatedComponent->GetComponentRotation(), true, Fall_Hit);
		if (Fall_Hit.IsValidBlockingHit())
		{
				Pawn->bFalling = false;
				Pawn->FallVector = FVector(0.0f,0.0f,-30.0f);
				Pawn->falltime = 0.0f;
				Pawn->bJump = false;
				Pawn->JumpVector = FVector(0.0f);
		}
		else
		{
			Pawn->bFalling = true;	
		}
	}
	else
	{
		Pawn->bFalling = false;
		Pawn->FallVector = FVector(0.0f, 0.0f, -30.0f);
		Pawn->falltime = 0.0f;
		Pawn->bJump = false;
		Pawn->JumpVector = FVector(0.0f);
	}

  DesiredMovementThisFrame = ConsumeInputVector();

	if (!DesiredMovementThisFrame.IsNearlyZero())
	{
		FHitResult Hit;
		SafeMoveUpdatedComponent(DesiredMovementThisFrame, UpdatedComponent->GetComponentRotation(), true, Hit);
		// If we bumped into something, try to slide along it
		if (Hit.IsValidBlockingHit())
		{
			SlideAlongSurface(DesiredMovementThisFrame, 1.f - Hit.Time, Hit.Normal, Hit);
		}
	}
};

