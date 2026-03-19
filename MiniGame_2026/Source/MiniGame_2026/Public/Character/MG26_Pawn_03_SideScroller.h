// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/MG26_PawnBase.h"
#include "MG26_Pawn_03_SideScroller.generated.h"

/**
 * 
 */
UCLASS()
class MINIGAME_2026_API AMG26_Pawn_03_SideScroller : public AMG26_PawnBase
{
	GENERATED_BODY()
	
public:
	AMG26_Pawn_03_SideScroller();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// Ghi đè hàm di chuyển từ lớp cha để khóa mặt phẳng
	virtual void Move(const FInputActionValue& Value) override;
	
	// Ghi đè hàm nhìn để vô hiệu hóa xoay camera bằng chuột
	virtual void Look(const FInputActionValue& Value) override;

private:
	// Mặt phẳng di chuyển được tính toán lúc bắt đầu
	FPlane MovementPlane;
	
	// Hướng di chuyển "Sang Phải" tương đối so với góc nhìn ban đầu
	FVector RightDirection;

	// Góc xoay ban đầu của Pawn
	FRotator InitialRotation;
};
