// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/MG26_PawnBase.h"


// Sets default values
AMG26_PawnBase::AMG26_PawnBase()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMG26_PawnBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMG26_PawnBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMG26_PawnBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

