// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerPawn.h"
#include "Camera/CameraComponent.h"
#include "InteractableActor.h"
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
	CapsuleCollisionComponent->InitCapsuleSize(50.0f, 90.0f);
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

	FRotator Rotation = PawnCamera->GetComponentRotation();
	if (bFalling == false)
	{
		ChangeVelocity();
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

	//interaction handling
	ViewVector = Rotation.RotateVector(FVector(200.0f, 0.0f, 0.0f));
	FCollisionQueryParams View_TraceParams = FCollisionQueryParams(FName(TEXT("View_TraceParams")), true, this);
	View_TraceParams.bTraceComplex = true;
	View_TraceParams.bTraceAsyncScene = true;
	View_TraceParams.bReturnPhysicalMaterial = false;
	FHitResult View_Hit(ForceInit);

	//check if player looks at something nearby
	GetWorld()->LineTraceSingleByChannel(View_Hit, PawnCamera->GetComponentLocation(), PawnCamera->GetComponentLocation() + ViewVector, ECC_Pawn, View_TraceParams);
	if (View_Hit.bBlockingHit == true)
	{
		AActor* BlockingActor = View_Hit.GetActor();
		if (BlockingActor)
		{
			//hit evaluation
			if (BlockingActor->IsA(AInteractableActor::StaticClass()) && bInteracting == true)
			{
				bInteracting = false;
				AInteractableActor* InteractedActor = Cast<AInteractableActor>(BlockingActor);
				InteractedActor->Interaction();
			}
		}
	}
}

void APlayerPawn::ChangeVelocity()
{
	if (bMoveForward)
	{
		if (MovementInputVector.X < 1.0f)
		{
			if (MovementInputVector.X >= 0.0f)
			{
				MovementInputVector.X += standardAcceleration;
			}
			else
			{
				MovementInputVector.X += directionChangeAcceleration;
			}
		}
	}
	else	if (bMoveBackward)	//walk slower backwards
	{	
		if (MovementInputVector.X > -0.6f)
		{
			if (MovementInputVector.X <= 0.0f)
			{
				MovementInputVector.X -= standardAcceleration;
			}
			else
			{
				MovementInputVector.X -= directionChangeAcceleration;
			}
		}
	}
	else
	{
		if (MovementInputVector.X < -standardAcceleration && MovementInputVector.X > standardAcceleration)
		{
			if (MovementInputVector.X < -standardAcceleration)
			{
				MovementInputVector.X += standardAcceleration;
			}
			if (MovementInputVector.X > standardAcceleration)
			{
				MovementInputVector.X -= standardAcceleration;
			}
		}
		else
		{
			MovementInputVector.X = 0.0f;
		}
	}
	//walk a bit slower backwards
	if (bMoveRight)
	{
		if (MovementInputVector.Y<0.8f)
		{
			if (MovementInputVector.Y >= 0.0f)
			{
				MovementInputVector.Y += standardAcceleration;
			}
			else
			{
				MovementInputVector.Y += directionChangeAcceleration;
			}
		}
	}
	else if (bMoveLeft)
	{
		if (MovementInputVector.Y > -0.8f)
		{
			if (MovementInputVector.Y <= 0.0f)
			{
				MovementInputVector.Y -= standardAcceleration;
			}
			else
			{
				MovementInputVector.Y -= directionChangeAcceleration;
			}
		}
	}
	else
	{
		if (MovementInputVector.Y < -standardAcceleration && MovementInputVector.Y > standardAcceleration)
		{
			if (MovementInputVector.Y < -standardAcceleration)
			{
				MovementInputVector.Y += standardAcceleration;
			}
			if (MovementInputVector.Y > standardAcceleration)
			{
				MovementInputVector.Y -= standardAcceleration;
			}
		}
		else
		{
			MovementInputVector.Y = 0.0f;
		}
	}
}

UPawnMovementComponent* APlayerPawn::GetMovementComponent() const
{
	return OurMovementComponent;
}
