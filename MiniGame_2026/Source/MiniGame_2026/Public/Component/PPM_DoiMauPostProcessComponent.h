// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PPM_DoiMauPostProcessComponent.generated.h"

class UCameraComponent;
class UMaterialInterface;
class UMaterialInstanceDynamic;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MINIGAME_2026_API UPPM_DoiMauPostProcessComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPPM_DoiMauPostProcessComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Material gốc được gán từ Editor (là cái Post Process Material của bạn)
	UPROPERTY(EditDefaultsOnly, Category = "PostProcess")
	UMaterialInterface* BasePostProcessMat;

	// Hàm bật/tắt hiệu ứng từ Checkbox trên UI
	UFUNCTION(BlueprintCallable, Category = "PostProcess")
	void TogglePostProcess(bool bEnableEffect);

	// Hàm cập nhật màu sắc ColorTinting từ 3 thanh Slider trên UI
	UFUNCTION(BlueprintCallable, Category = "PostProcess")
	void UpdateColorTinting(FLinearColor NewColor);

private:
	// Lưu trữ MID (Material Instance Dynamic) sinh ra lúc Runtime
	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMat;

	// Lưu trữ tham chiếu đến Camera của Actor
	UPROPERTY()
	UCameraComponent* OwnerCamera;

	// Tên chính xác của Vector Parameter trong Material
	const FName ColorParamName = FName("ColorTinting");
};