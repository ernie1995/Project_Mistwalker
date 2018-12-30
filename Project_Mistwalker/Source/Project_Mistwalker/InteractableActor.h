// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "InteractableActor.generated.h"

UCLASS()
class PROJECT_MISTWALKER_API AInteractableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties

	AInteractableActor();

	virtual void Interaction();

	UStaticMeshComponent* ActorMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//variables
	FString Name = "Test";
	bool bLookedAt = false;
};
