// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerPawnController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_MISTWALKER_API APlayerPawnController : public APlayerController
{
	GENERATED_BODY()
	
		APlayerPawnController();

	//input function
	virtual void SetupInputComponent() override;

	//movement input functions, pass on input values to pawns
	void MoveForwardPressed();

	void MoveForwardReleased();

	void MoveBackwardsPressed();

	void MoveBackwardsReleased();

	void MoveRightPressed();

	void MoveRightReleased();

	void MoveLeftPressed();

	void MoveLeftReleased();

	void SprintPressed();

	void SprintReleased();

	void CrouchPressed();

	//void CrouchReleased();

	void Jump();

	//interaction movement functions
	void Interact();

	//camera input functions
	void CameraXAxis(float AxisValue); 

	void CameraZAxis(float AxisValue);

	//player specific variables
	float speed = 12.0f;
};
