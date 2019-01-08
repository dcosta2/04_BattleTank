// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Public/Tank.h"
//Kitchen Sink Start
#include "Components/ActorComponent.h"
#include "Components/InputComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
//Kitchen Sink End

#include "TankPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()
	
private:
	ATank* GetControlledTank() const;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	//Start the tank barrel so that a shot would hit where the crosshair intersects the world
	virtual void AimTowardsCrosshairs();

	//Return an OUT parameter, true if hit landscape
	virtual bool GetSightRayHitLocation(FVector& OutHitLocation) const;

	bool SetAim(FVector& Start, FVector& End) const;

	UPROPERTY(EditAnywhere)
	float Reach = 1000000;

	UPROPERTY(EditAnywhere)
	float CrossHairXLocation = 0.5f;

	UPROPERTY(EditAnywhere)
	float CrossHairYLocation = 0.3f;

	bool NotMyStle(FVector LookDirection, FVector& HitLocation) const;
};
