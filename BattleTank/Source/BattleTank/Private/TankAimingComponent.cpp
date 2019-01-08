// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/TankAimingComponent.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UTankAimingComponent::SetBarrelReference(UStaticMeshComponent* BarrelToSet) {
	Barrel = BarrelToSet;
}

//Add SetTurret
void UTankAimingComponent::AimAt(FVector WorldSpaceAim, float LaunchSpeed)
{
	auto OurTankName = GetOwner()->GetName();
	if (!Barrel) { return; }
	auto BarrelLocation = Barrel->GetComponentLocation().ToString();

	FVector OutLaunchVelocity(0);
	FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));
	bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity(
		this,
		OutLaunchVelocity,
		StartLocation,
		WorldSpaceAim,
		LaunchSpeed,
		ESuggestProjVelocityTraceOption::DoNotTrace
	);
	if (bHaveAimSolution) 
	{ 
		auto AimDirection = OutLaunchVelocity.GetSafeNormal();
		MoveBarrelTowards(AimDirection);
		UE_LOG(LogTemp, Warning, TEXT("%s is Aiming at %s"), *OurTankName, *AimDirection.ToString());
	} 
}



void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection) {
	// Calculate difference between current barrel rotation and AimDirection

	auto BarrelRotator = Barrel->GetForwardVector().Rotation();
	auto AimAsRotator = AimDirection.Rotation();
	auto DeltaRotator = AimAsRotator - BarrelRotator;
	UE_LOG(LogTemp, Warning, TEXT("AimAsRotator: %s"), *AimAsRotator.ToString());

	// Move the Barrel the right amount this frame
	// Given a max elevation speed, and the frame time
}
