// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerPawn.h"
#include "Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "Runtime/Engine/Classes/Materials/Material.h"
#include "Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "ThePawnMovementComponent.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//collision box
	CapsuleCollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RootComponent"));
	RootComponent = CapsuleCollisionComponent;
	CapsuleCollisionComponent->InitCapsuleSize(80.0f, 10.0f);
	CapsuleCollisionComponent->SetCollisionProfileName(TEXT("Pawn"));

	//mesh
	PawnBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PawnBody"));
	static ConstructorHelpers::FObjectFinder <UStaticMesh> BaseMesh1(TEXT("StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'"));
	if (BaseMesh1.Succeeded())
	{
		PawnBody->SetStaticMesh(BaseMesh1.Object);
		PawnBody->AttachTo(RootComponent);
		PawnBody->bOwnerNoSee = true;
	}

	//camera
	PawnCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("OurCamera"));
	PawnCamera->AttachTo(RootComponent);
	PawnCamera->bAbsoluteLocation = false;
	PawnCamera->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
	PawnCamera->bAbsoluteRotation = true;

	//movement component
	OurMovementComponent = CreateDefaultSubobject<UThePawnMovementComponent>(TEXT("CustomMovementComponent"));
	OurMovementComponent->UpdatedComponent = RootComponent;
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//movement handling

	//adjust sprint multiplier
	if (bSprint == true)
	{
		if (sprintModifier < sprintMaxModifier)
		{
			sprintModifier += 2 * DeltaTime;
		}
	}
	else
	{
		sprintModifier = 1.0f;
	}

	if (bFalling == false)
	{
		FRotator Rotation = PawnCamera->GetComponentRotation();
		if (bCrouch == false)
		{
			MovementVector = Rotation.RotateVector(MovementInputVector)*sprintModifier; //move slower when crouching in general
		}
		else
		{
			MovementVector = Rotation.RotateVector(MovementInputVector)*sprintModifier*0.5f;
		}
	}

	//size adjustments for crouching (probably just temporary)
	if (bCrouch == true)//size up after slide
	{
		if (crouchSizeModifier > 0.6f)
		{		
			crouchSizeModifier -= DeltaTime;
		 RootComponent->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f*crouchSizeModifier));
	  }
	}
	else
	{
		if (crouchSizeModifier < 1.f)
		{
			crouchSizeModifier += DeltaTime;
			RootComponent->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f*crouchSizeModifier));
		}
	}

	OurMovementComponent->AddInputVector(FVector(MovementVector.X, MovementVector.Y, 0.0f));

	if (bFalling == true || bJump == true)
	{
		falltime += 0.01;
		FallVector.Z = (-9.81f*falltime*falltime*0.5f + 0.5f*9.81f*(falltime - 0.01f)*(falltime - 0.01f))*100.0f+JumpVector.Z;
	}

	//camera handling
	CurrentAngle.Pitch = FMath::Clamp(CurrentAngle.Pitch, -90.0f, 90.0f);
	CurrentAngle += DeltaAngle;
	PawnCamera->SetRelativeRotation(CurrentAngle);
}

UPawnMovementComponent* APlayerPawn::GetMovementComponent() const
{
	return OurMovementComponent;
}
