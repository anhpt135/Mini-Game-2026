// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/MG26_PawnBase.h"
#include "MG26_Pawn_03_SideScroller.generated.h"
// Class PawnSideScroll này có 2 chức năng chính
//1. Khóa mặt phẳng di chuyển của Pawn để tạo cảm giác side-scroller 2D
//2. Tạo một Widget UI khi Pawn được điều khiển, và thiết lập chế độ input để tương tác với UI đó (cho phép click chuột, hiển thị con trỏ chuột, v.v.)
UCLASS()
class MINIGAME_2026_API AMG26_Pawn_03_SideScroller : public AMG26_PawnBase
{
	GENERATED_BODY()
	
public:
	AMG26_Pawn_03_SideScroller();

	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// Ghi đè hàm di chuyển từ lớp cha để khóa mặt phẳng
	virtual void Move(const FInputActionValue& Value) override;
	
	// Ghi đè hàm nhìn để vô hiệu hóa xoay camera bằng chuột
	virtual void Look(const FInputActionValue& Value) override;

	// Lớp Widget UI để tạo khi Pawn được điều khiển
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UUserWidget> WidgetCuaTui;

private:
	// Lưu lại tham chiếu đến Widget đã tạo
	UPROPERTY()
	class UUserWidget* ConTroWidget;

	// Mặt phẳng di chuyển được tính toán lúc bắt đầu
	FPlane MovementPlane;
	
	// Hướng di chuyển "Sang Phải" tương đối so với góc nhìn ban đầu
	FVector RightDirection;

	// Góc xoay ban đầu của Pawn
	FRotator InitialRotation;
};
