// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/MG26_PawnBase.h"
#include "MG26_Pawn_02.generated.h"

/**
 * Pawn điều khiển kiểu Trực thăng (Helicopter)
 */
UCLASS()
class MINIGAME_2026_API AMG26_Pawn_02 : public AMG26_PawnBase
{
	GENERATED_BODY()

public:
	AMG26_Pawn_02();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Override Move từ Base để xử lý bay tới/lùi và xoay
	virtual void Move(const FInputActionValue& Value) override;

	// Xử lý bay lên (dùng Jump Action)
	void StartAscend(const FInputActionValue& Value);
	void StopAscend(const FInputActionValue& Value);

private:
	// Input State
	float TargetForwardInput;
	float TargetTurnInput;
	bool bIsAscending;

	// Physics State
	float CurrentForwardSpeed;
	float CurrentVerticalSpeed;
	float CurrentYawSpeed;

	// Configuration
	UPROPERTY(EditAnywhere, Category = "Helicopter Config")
	float Acceleration = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Helicopter Config")
	float MaxForwardSpeed = 1500.0f;

	UPROPERTY(EditAnywhere, Category = "Helicopter Config")
	float TurnSpeed = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Helicopter Config")
	float LiftAcceleration = 1500.0f; // Gia tốc bay lên

	UPROPERTY(EditAnywhere, Category = "Helicopter Config")
	float GravityForce = 980.0f; // Trọng lực kéo xuống

	UPROPERTY(EditAnywhere, Category = "Helicopter Config")
	float MaxVerticalSpeed = 1000.0f; // Tốc độ lên/xuống tối đa

	UPROPERTY(EditAnywhere, Category = "Helicopter Config")
	float MaxAltitude = 2000.0f; // Độ cao tối đa (tính từ Z=0 hoặc vị trí bắt đầu tùy logic)
};
