// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "PlayerPawn.generated.h"

UCLASS()
class PROJECT_MISTWALKER_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//main components
	UStaticMeshComponent* PawnBody;//base mesh

	UPROPERTY(EditAnywhere)
		UCapsuleComponent* CapsuleCollisionComponent;

	UPROPERTY(EditAnywhere)
		class UCameraComponent* PawnCamera;//camera component for this player pawn

	class UThePawnMovementComponent* OurMovementComponent;//movement component for collision handling (and movement)

	virtual UPawnMovementComponent* GetMovementComponent() const override;//get instance of pawnmovementcomponent

	//variables

	//movement
	FVector MovementInputVector = FVector(0.0f);//x and y axis input, unrotated
	FVector MovementVector;//final, rotated movement in x-y plane
	FVector FallVector = FVector(0.0f);
	FVector JumpVector = FVector(0.0f);
	float falltime = 0.0f;
	float sprintMaxModifier = 3.0f;
	float sprintModifier = 1.0f;
	float crouchSizeModifier = 1.0f;
	bool bJump = false;
	bool bFalling = false;
	bool bMoveForward = false;
	bool bMoveBackward = false;
	bool bMoveRight = false;
	bool bMoveLeft = false;
	bool bSprint = false;
	bool bCrouch = false;
	bool bStandingUpBlocked = false;

	//interaction
	FVector ViewVector;
	bool bInteracting = false;


	//camera
	FRotator DeltaAngle;
	FRotator CurrentAngle;
};
