// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"

//Kitchen Sink Start
#include "BattleTank.h"
#include "Components/ActorComponent.h"
#include "Components/InputComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Public/TankAimingComponent.h"
//Kitchen Sink End

#define OUT

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();
	auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if (ensure(AimingComponent)) {
		FoundAimingComponent(AimingComponent);
	}
}

void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AimTowardsCrosshairs();
}

void ATankPlayerController::AimTowardsCrosshairs()
{
	if (!GetPawn()) { return; }
	auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimingComponent)) { return; }

	FVector HitLocation; // Out parameter
	if(GetSightRayHitLocation(HitLocation)) 
	{
		AimingComponent->AimAt(HitLocation);
	}
}

bool ATankPlayerController::GetSightRayHitLocation(FVector& OutHitLocation) const
{
	FVector LineStart, LineEnd;
	if (!SetAim(LineStart, LineEnd)) { return false; }

	FCollisionQueryParams TraceParameters(FName(TEXT("Landscape1")), false, GetOwner());
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByObjectType(
		HitResult,
		LineStart,
		LineEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldStatic),
		TraceParameters
	);

	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	if(!ActorHit) {
		return false;
	}
	else {
		OutHitLocation = HitResult.ImpactPoint;
		return true;
	}
}


//Find the Crosshair Position
//"De-Project" the screen position of the crosshair to a world direction
//Line-Trace alont that direction, and see what we hit (up to a max range)
bool ATankPlayerController::SetAim(FVector& Start, FVector& End) const {
	// Find the crosshair position in pixel coordinates
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);
	FVector2D ScreenLocation = FVector2D(ViewportSizeX * CrossHairXLocation, ViewportSizeY * CrossHairYLocation);
	
	FVector WorldDirection;
	if (DeprojectScreenPositionToWorld(
		ScreenLocation.X,
		ScreenLocation.Y,
		Start,
		WorldDirection
		))
	{
		End = Start + Reach * WorldDirection;
		return true;
	}
	else
	{
		return false;
	}
}


/* 
My sucessfull Challenge was different from his version

LineTraceSingleByChannel Version of this code:
*/
bool ATankPlayerController::NotMyStle(FVector LookDirection, FVector& HitLocation) const 
{
	FHitResult HitResult;
	//auto StartLocation = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation();
	auto StartLocation = PlayerCameraManager->GetCameraLocation();
	auto EndLocation = StartLocation + (LookDirection * Reach);

	if (GetWorld()->LineTraceSingleByChannel(
			HitResult,
			StartLocation,
			EndLocation,
			ECollisionChannel::ECC_Visibility)
		)
	{
		HitLocation = HitResult.Location;
		return true;
	}
	HitLocation = FVector(0.f);
	return false;
}

/*

End His version!!
*/

