// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/Tank.h"
#include "TankAimingComponent.h"
#include "TankTurret.h"
#include "TankBarrel.h"
#include "TankTrack.h"
#include "Projectile.h"
#include "Components/InputComponent.h"
#include "TankMovementComponent.h"

// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	TankAimingComponent = CreateDefaultSubobject<UTankAimingComponent>(FName("Aiming Component"));
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();
	LastFireTime = FPlatformTime::Seconds();
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATank::SetBarrelReference(UTankBarrel * BarrelToSet)
{
	TankAimingComponent->SetBarrelReference(BarrelToSet);
	Barrel = BarrelToSet;
}

void ATank::SetTurretReference(UTankTurret * TurretToSet)
{
	TankAimingComponent->SetTurretReference(TurretToSet);
}

void ATank::SetLeftTrack(UTankTrack* TrackToSet)
{
	LeftTrack = TrackToSet;
}

void ATank::SetRightTrack(UTankTrack* TrackToSet)
{
	RightTrack = TrackToSet;
}



void ATank::Fire()
{
	bool isReloaded = (FPlatformTime::Seconds() - LastFireTime) > ReloadTimeInSeconds;
	if (Barrel && isReloaded)
	{
		auto Projectile = GetWorld()->SpawnActor<AProjectile>(
			ProjectileBlueprint, 
			Barrel->GetSocketLocation(FName("Projectile")),
			Barrel->GetSocketRotation(FName("Projectile"))
			);
		Projectile->LaunchProjectile(LaunchSpeed);
		LastFireTime = FPlatformTime::Seconds();
	}
}

void ATank::AimAt(FVector HitLocation, float LaunchSpeed)
{
	TankAimingComponent->AimAt(HitLocation, LaunchSpeed);
}