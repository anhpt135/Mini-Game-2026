// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/MPC_ChangeColor.h"

// Sets default values for this component's properties
UMPC_ChangeColor::UMPC_ChangeColor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMPC_ChangeColor::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMPC_ChangeColor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

