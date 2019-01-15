// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

class ATank;
class UTankAimingComponent;

UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	UFUNCTION(BlueprintCallable, Category = "Setup")
	ATank* GetControlledTank() const;

	UFUNCTION(BlueprintImplementableEvent, Category = "Setup")
	void FoundAimingComponent(UTankAimingComponent* AimCompRef);

private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	//Start the tank barrel so that a shot would hit where the crosshair intersects the world
	virtual void AimTowardsCrosshairs();

	//Modifies an OUT parameter, true if hit landscape
	virtual bool GetSightRayHitLocation(FVector& OutHitLocation) const;

	bool SetAim(FVector& Start, FVector& End) const;

	UPROPERTY(EditDefaultsOnly)
	float Reach = 1000000;

	UPROPERTY(EditDefaultsOnly)
	float CrossHairXLocation = 0.5f;

	UPROPERTY(EditDefaultsOnly)
	float CrossHairYLocation = 0.3f;

	bool NotMyStle(FVector LookDirection, FVector& HitLocation) const;
};
