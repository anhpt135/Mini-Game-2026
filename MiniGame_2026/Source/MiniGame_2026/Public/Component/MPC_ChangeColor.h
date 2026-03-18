// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "MPC_ChangeColor.generated.h"

class UMaterialParameterCollection;
class UCurveFloat;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MINIGAME_2026_API UMPC_ChangeColor : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMPC_ChangeColor();

protected:
	virtual void BeginPlay() override;

	// Hàm Callback được kích hoạt khi Pawn nhảy
	UFUNCTION()
	void HamCallbackNhayLen();

	// Hàm Callback được kích hoạt khi Pawn chạm đất
	UFUNCTION()
	void HamCallbackChamDat();

	// Hàm callback của Timeline
	UFUNCTION()
	void HamCallbackTimeline(float Value);

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Thiết lập nơi chứa MPC và tên thông số của nó 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material|Settings")
	UMaterialParameterCollection* MPCCuaTui;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material|Settings")
	FName TenThongSoMPC = FName("AlphaCuaEm");

	// Biến con trỏ biểu đồ CURVE
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material|Alpha Settings")
	UCurveFloat* CurveCuaTui;

	// Hàm thay đổi giá trị của MPC
	UFUNCTION(BlueprintCallable, Category = "Material")
	void ThayDoiGiaTriMPC(float NewAlpha);

private:
	FTimeline TimelineCuaTui;
};
