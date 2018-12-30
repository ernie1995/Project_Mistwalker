// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "Runtime/Engine/Classes/Components/MeshComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "PickUpActor.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_MISTWALKER_API APickUpActor : public AInteractableActor
{
	GENERATED_BODY()

public:
	APickUpActor() :AInteractableActor()
	{
		static ConstructorHelpers::FObjectFinder <UStaticMesh> mesh(TEXT("StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'"));
		if (mesh.Succeeded())
		{
			ActorMesh->SetStaticMesh(mesh.Object);
		}
	}

	virtual void Interaction()
	{
		ActorMesh->DestroyComponent();
	}

};
