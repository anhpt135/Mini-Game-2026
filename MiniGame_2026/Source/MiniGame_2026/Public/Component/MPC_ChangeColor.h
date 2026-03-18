// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MPC_ChangeColor.generated.h"

class UMaterialParameterCollection;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MINIGAME_2026_API UMPC_ChangeColor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Thiết lập các giá trị mặc định cho Component này
	UMPC_ChangeColor();

protected:
	// Được gọi khi trò chơi bắt đầu
	virtual void BeginPlay() override;

	// Hàm lắng nghe và xử lý khi nhận được tín hiệu Pawn Vừa Nhảy
	UFUNCTION()
	void HandleOnPawnJumped();

	// Hàm lắng nghe và xử lý khi nhận được tín hiệu Pawn Vừa Chạm Đất
	UFUNCTION()
	void HandleOnPawnLanded();

public:	
	// Được gọi mỗi khung hình
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// File Material Parameter Collection chứa các biến toàn cục
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material|Settings")
	UMaterialParameterCollection* MyMPC;

	// Tên của biến Scalar (Alpha) trong MPC cần thay đổi
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material|Settings")
	FName AlphaParameterName = FName("AlphaCuaEm");

	// Giá trị Alpha muốn đặt khi nhân vật đang nhảy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material|Alpha Settings")
	float JumpAlpha = 1.0f;

	// Giá trị Alpha gốc muốn trả về khi nhân vật chạm đất
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material|Alpha Settings")
	float DefaultAlpha = 0.0f;

	// Hàm cốt lõi chịu trách nhiệm giao tiếp với hệ thống Material để cập nhật Alpha
	UFUNCTION(BlueprintCallable, Category = "Material")
	void UpdateMPCAlpha(float NewAlpha);
};
