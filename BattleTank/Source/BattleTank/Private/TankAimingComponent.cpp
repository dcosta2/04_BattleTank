// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/TankAimingComponent.h"

// #include "Components/InputComponent.h"
// #include "Components/StaticMeshComponent.h"
// #include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
// #include "Public/Tank.h"
#include "TankBarrel.h" 
#include "TankTurret.h"

// Sets default values for this component's properties

UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UTankAimingComponent::Initialise(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet) {
	Barrel = BarrelToSet;
	Turret = TurretToSet;
};

void UTankAimingComponent::AimAt(FVector WorldSpaceAim, float LaunchSpeed)
{
	auto OurTankName = GetOwner()->GetName();
	if (!ensure(Barrel)) { return; }
	auto BarrelLocation = Barrel->GetComponentLocation().ToString();

	FVector OutLaunchVelocity(0);
	FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));
	bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity(
		this,
		OutLaunchVelocity,
		StartLocation,
		WorldSpaceAim,
		LaunchSpeed,
		false,
		0,
		0,
		ESuggestProjVelocityTraceOption::DoNotTrace // Parameter must be present to prevent bug
	);
	if (bHaveAimSolution) 
	{ 
		auto AimDirection = OutLaunchVelocity.GetSafeNormal();
		MoveBarrelTowards(AimDirection);
		//UE_LOG(LogTemp, Warning, TEXT("%s is Aiming at %s"), *OurTankName, *AimDirection.ToString());
		auto Time = GetWorld()->GetTimeSeconds();
		
	}
	else {
		auto Time = GetWorld()->GetTimeSeconds();
		//UE_LOG(LogTemp, Warning, TEXT("%f: No Aim Solve Found"), Time);
	}
}



void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection) {
	// Calculate difference between current barrel rotation and AimDirection
	if (!ensure(Barrel && Turret)) { return; }
	auto BarrelRotator = Barrel->GetForwardVector().Rotation();
	auto AimAsRotator = AimDirection.Rotation();
	auto DeltaRotator = AimAsRotator - BarrelRotator;
	//UE_LOG(LogTemp, Warning, TEXT("AimAsRotator: %s"), *AimAsRotator.ToString());
	
	Barrel->Elevate(DeltaRotator.Pitch);
	Turret->Rotate(DeltaRotator.Yaw);
}
