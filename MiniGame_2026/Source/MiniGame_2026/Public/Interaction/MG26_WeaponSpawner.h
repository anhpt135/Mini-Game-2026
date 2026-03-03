// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MG26_TriggerButton.h"
#include "MG26_WeaponSpawner.generated.h"

UCLASS()
class MINIGAME_2026_API AMG26_WeaponSpawner : public AMG26_TriggerButton
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMG26_WeaponSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
