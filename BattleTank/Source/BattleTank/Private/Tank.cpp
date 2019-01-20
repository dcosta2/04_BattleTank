// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/Tank.h"
#include "Engine/World.h"

// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void ATank::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = StartingHealth;
}

float ATank::TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) {
	int32 DamagePoints = FPlatformMath::RoundToInt(DamageAmount);
	float DamageToApply = FMath::Clamp<int32>(DamagePoints, 0, CurrentHealth);
	if (DamageToApply > 0.f) {
		CurrentHealth = CurrentHealth - DamageToApply;
		if (CurrentHealth <= 0.f) { 
			UE_LOG(LogTemp, Warning, TEXT("%s has been Destroyed!"), *GetNameSafe(this));
			Destroy(); 
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("%s's CurrentHealth: %i"), *GetNameSafe(this), CurrentHealth);
	return DamageToApply;
}

float ATank::GetHealthPercent() const {
	return (float)CurrentHealth / (float)StartingHealth;
}