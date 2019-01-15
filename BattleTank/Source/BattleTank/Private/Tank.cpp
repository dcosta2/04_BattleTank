// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/Tank.h"
#include "TankAimingComponent.h"
#include "TankTurret.h"
#include "TankBarrel.h"
#include "TankTrack.h"
#include "Projectile.h"
#include "Components/InputComponent.h"

// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	//TankAimingComponent = CreateDefaultSubobject<UTankAimingComponent>(FName("Aiming Component"));
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Tank Begin Play"));
	LastFireTime = FPlatformTime::Seconds();
}


void ATank::Fire()
{
	auto Barrel = FindComponentByClass<UTankAimingComponent>()->Barrel;
	if (!ensure(Barrel)) { return; }
	bool isReloaded = (FPlatformTime::Seconds() - LastFireTime) > ReloadTimeInSeconds;
	if (isReloaded)
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
	TankAimingComponent = FindComponentByClass<UTankAimingComponent>();
	if (!ensure(TankAimingComponent)) { return;  }
	TankAimingComponent->AimAt(HitLocation, LaunchSpeed);
}