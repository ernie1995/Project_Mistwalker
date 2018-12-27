// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerPawnController.h"
#include "PlayerPawn.h"

APlayerPawnController::APlayerPawnController()
{

}



void APlayerPawnController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//movement
	InputComponent->BindAction("Forward", IE_Pressed, this, &APlayerPawnController::MoveForwardPressed);
	InputComponent->BindAction("Forward", IE_Released, this, &APlayerPawnController::MoveForwardReleased);
	InputComponent->BindAction("Backward", IE_Pressed, this, &APlayerPawnController::MoveBackwardsPressed);
	InputComponent->BindAction("Backward", IE_Released, this, &APlayerPawnController::MoveBackwardsReleased);
	InputComponent->BindAction("Right", IE_Pressed, this, &APlayerPawnController::MoveRightPressed);
	InputComponent->BindAction("Right", IE_Released, this, &APlayerPawnController::MoveRightReleased);
	InputComponent->BindAction("Left", IE_Pressed, this, &APlayerPawnController::MoveLeftPressed);
	InputComponent->BindAction("Left", IE_Released, this, &APlayerPawnController::MoveLeftReleased);
	InputComponent->BindAction("Sprint", IE_Pressed, this, &APlayerPawnController::SprintPressed);
	InputComponent->BindAction("Sprint", IE_Released, this, &APlayerPawnController::SprintReleased);

	InputComponent->BindAction("Jump", IE_Pressed, this, &APlayerPawnController::Jump);

	InputComponent->BindAction("Crouch", IE_Pressed, this, &APlayerPawnController::CrouchPressed);

	//interaction
	InputComponent->BindAction("Interaction", IE_Pressed, this, &APlayerPawnController::Interact);

	//camera
	InputComponent->BindAxis("MouseX", this, &APlayerPawnController::CameraXAxis);
	InputComponent->BindAxis("MouseY", this, &APlayerPawnController::CameraZAxis);
}

void APlayerPawnController::MoveForwardPressed()
{
	APlayerPawn* Pawn = Cast<APlayerPawn>(GetPawn());
	if (Pawn)
	{
		Pawn->bMoveForward = true;
		Pawn->MovementInputVector.X = 1.0f;
	}
}

void APlayerPawnController::MoveForwardReleased()
{
	APlayerPawn* Pawn = Cast<APlayerPawn>(GetPawn());
	if (Pawn)
	{
		Pawn->bMoveForward = false;
		if (Pawn->bMoveBackward == false && Pawn->bMoveForward == false)
		{
			Pawn->MovementInputVector.X = 0.0f;
		}
	}
}

void APlayerPawnController::MoveBackwardsPressed()
{
	APlayerPawn* Pawn = Cast<APlayerPawn>(GetPawn());
	if (Pawn)
	{
		Pawn->bMoveBackward = true;
		Pawn->MovementInputVector.X = -1.0f;
	}
}

void APlayerPawnController::MoveBackwardsReleased()
{
	APlayerPawn* Pawn = Cast<APlayerPawn>(GetPawn());
	if (Pawn)
	{
		Pawn->bMoveBackward = false;
		if (Pawn->bMoveBackward == false && Pawn->bMoveForward == false)
		{
			Pawn->MovementInputVector.X = 0.0f;
		}
	}
}

void APlayerPawnController::MoveRightPressed()
{
	APlayerPawn* Pawn = Cast<APlayerPawn>(GetPawn());
	if (Pawn)
	{
		Pawn->bMoveRight = true;
		Pawn->MovementInputVector.Y = 1.0f;
	}
}

void APlayerPawnController::MoveRightReleased()
{
	APlayerPawn* Pawn = Cast<APlayerPawn>(GetPawn());
	if (Pawn)
	{
		Pawn->bMoveRight = false;
		if (Pawn->bMoveRight == false && Pawn->bMoveLeft == false)
		{
			Pawn->MovementInputVector.Y = 0.0f;
		}
	}
}

void APlayerPawnController::MoveLeftPressed()
{
	APlayerPawn* Pawn = Cast<APlayerPawn>(GetPawn());
	if (Pawn)
	{
		Pawn->bMoveLeft = true;
		Pawn->MovementInputVector.Y = -1.0f;
	}
}

void APlayerPawnController::MoveLeftReleased()
{
	APlayerPawn* Pawn = Cast<APlayerPawn>(GetPawn());
	if (Pawn)
	{
		Pawn->bMoveLeft = false;
		if (Pawn->bMoveRight == false && Pawn->bMoveLeft == false)
		{
			Pawn->MovementInputVector.Y = 0.0f;
		}
	}
}

void APlayerPawnController::SprintPressed()
{
	APlayerPawn* Pawn = Cast<APlayerPawn>(GetPawn());
	if (Pawn)
	{
		Pawn->bSprint = true;
	}
}

void APlayerPawnController::SprintReleased()
{
	APlayerPawn* Pawn = Cast<APlayerPawn>(GetPawn());
	if (Pawn)
	{
		Pawn->bSprint = false;
	}
}

void APlayerPawnController::CrouchPressed()
{
	APlayerPawn* Pawn = Cast<APlayerPawn>(GetPawn());
	if (Pawn)
	{
		if (Pawn->bCrouch == false)
		{
			if (Pawn->bFalling == false)
			{
				Pawn->bCrouch = true;
			}
		}
		else
		{
			if (Pawn->bStandingUpBlocked == false)
			{
				Pawn->bCrouch = false;
			}
		}
	}
}


void APlayerPawnController::Jump()
{
	APlayerPawn* Pawn = Cast<APlayerPawn>(GetPawn());
	if (Pawn)
	{
		if (Pawn->bFalling == false)
		{
			Pawn->bJump = true;
			Pawn->FallVector=FVector(0.0f);
			if (Pawn->bCrouch)//jump lower if crouching
			{
				Pawn->JumpVector = FVector(0.0f, 0.0f, 1.0f);
			}
			else
			{
				Pawn->JumpVector = FVector(0.0f, 0.0f, 4.0f);
			}
		}
	}
}

void APlayerPawnController::Interact()
{
	APlayerPawn* Pawn = Cast<APlayerPawn>(GetPawn());
	if (Pawn)
	{
		Pawn->bInteracting = true;
	}
}

void APlayerPawnController::CameraXAxis(float AxisValue)
{
	APlayerPawn* Pawn = Cast<APlayerPawn>(GetPawn());
	if (Pawn)
	{
		Pawn->DeltaAngle.Pitch = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 1.5f;
	}
}

void APlayerPawnController::CameraZAxis(float AxisValue)
{
	APlayerPawn* Pawn = Cast<APlayerPawn>(GetPawn());
	if (Pawn)
	{
		Pawn->DeltaAngle.Yaw = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 1.5f;
	}
}
