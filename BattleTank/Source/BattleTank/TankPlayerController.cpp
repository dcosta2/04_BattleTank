// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "BattleTank.h"

#define OUT

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("I'm in PlayerController BeginPlay()"));
	ATank* ControlledTank = GetControlledTank();

	if (!ControlledTank) {
		UE_LOG(LogTemp, Warning, TEXT("I haven't been able to find a controlled tank"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("I'm controlling: %s"), *(ControlledTank->GetName()));
	}
}

void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AimTowardsCrosshairs();
}

void ATankPlayerController::AimTowardsCrosshairs()
{
	if (!GetControlledTank()) { return; }

	FVector HitLocation; // Out parameter
	//UE_LOG(LogTemp, Warning, TEXT("Hitlocation: %s"), *HitLocation.ToString());

	if(GetSightRayHitLocation(HitLocation)) 
	{
		GetControlledTank()->AimAt(HitLocation, GetControlledTank()->LaunchSpeed);
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

	/*
	DrawDebugLine(
		GetWorld(),
		LineStart,
		LineEnd,
		FColor(255, 0, 0),
		false,
		0.f,
		0.f,
		2.f
	);*/
	

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

ATank* ATankPlayerController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
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

