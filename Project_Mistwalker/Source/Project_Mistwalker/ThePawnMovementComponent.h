// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "ThePawnMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_MISTWALKER_API UThePawnMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()
	
public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	FVector PreservedMovement;//preserves movement of pawn during fall

	FVector DesiredMovementThisFrame;
};
