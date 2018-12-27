// Fill out your copyright notice in the Description page of Project Settings.

#include "MainGameMode.h"
#include "PlayerPawnController.h"
#include "PlayerPawn.h"

AMainGameMode::AMainGameMode()
	:
	AGameMode()
{
	PlayerControllerClass = APlayerPawnController::StaticClass();
	DefaultPawnClass = APlayerPawn::StaticClass();

}