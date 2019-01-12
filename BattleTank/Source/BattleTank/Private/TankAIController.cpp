// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/TankAIController.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "BattleTank.h"
#include "Public/Tank.h"
#include "TankPlayerController.h"


void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
}


void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto PlayerTank = Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());
	auto ControlledTank = Cast<ATank>(GetPawn());

	if (PlayerTank) 
	{
		ControlledTank->AimAt(PlayerTank->GetActorLocation(), ControlledTank->LaunchSpeed);
		ControlledTank->Fire();
	}
}