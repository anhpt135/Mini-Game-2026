// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/MG26_PawnBase.h"
#include "MG26_Pawn_01.generated.h"

/**
 * 
 */
UCLASS()
class MINIGAME_2026_API AMG26_Pawn_01 : public AMG26_PawnBase
{
	GENERATED_BODY()

public:
	AMG26_Pawn_01();

protected:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Ghi đè hàm di chuyển từ lớp cha
	virtual void Move(const FInputActionValue& Value) override;

	// Hàm mới để reset input khi nhả phím
	void StopMoving(const FInputActionValue& Value);

protected:
	// Các thông số vật lý xe
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Physics")
	float Acceleration = 2000.0f; // Gia tốc

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Physics")
	float BrakingDeceleration = 4000.0f; // Lực phanh (lớn hơn gia tốc để dừng nhanh)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Physics")
	float MaxSpeed = 3000.0f; // Tốc độ tối đa

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Physics")
	float TurnSpeed = 100.0f; // Tốc độ xoay (độ/giây)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Physics")
	float Friction = 2.0f; // Ma sát (giảm tốc khi thả ga)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Physics")
	float DriftFactor = 0.5f; // Hệ số trượt (càng thấp càng trượt nhiều)

private:
	float CurrentForwardSpeed;
	float CurrentTurnInput;
	float CurrentThrottleInput;
};
