// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PPM_DoiMauPostProcessComponent.h"
#include "Camera/CameraComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

// Sets default values for this component's properties
UPPM_DoiMauPostProcessComponent::UPPM_DoiMauPostProcessComponent()
{
	// Tắt tick đi để tiết kiệm hiệu năng, vì ta chỉ cập nhật khi UI gọi hàm
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UPPM_DoiMauPostProcessComponent::BeginPlay()
{
	Super::BeginPlay();

	// 1. Tìm Camera Component trên Actor đang sở hữu Component này (thường là Player Character/Pawn)
	AActor* OwnerActor = GetOwner();
	if (OwnerActor)
	{
		OwnerCamera = OwnerActor->FindComponentByClass<UCameraComponent>();
	}

	// 2. Tạo Dynamic Material từ Material gốc bạn thiết lập ngoài Editor
	if (BasePostProcessMat)
	{
		DynamicMat = UMaterialInstanceDynamic::Create(BasePostProcessMat, this);
	}
}

// Nhận true/false từ Widget để bật/tắt hiệu ứng Post Process trên Camera
void UPPM_DoiMauPostProcessComponent::TogglePostProcess(bool bEnableEffect)
{
	if (!OwnerCamera || !DynamicMat)
	{
		return;
	}

	if (bEnableEffect)
	{
		// Thêm hiệu ứng vào Camera với mức hiển thị (Weight) là 100%
		OwnerCamera->PostProcessSettings.AddBlendable(DynamicMat, 1.0f);
	}
	else
	{
		// Gỡ hiệu ứng ra khỏi Camera
		OwnerCamera->PostProcessSettings.RemoveBlendable(DynamicMat);
	}
}

// Hàm này nhận màu mới từ Widget và cập nhật giá trị Vector Parameter trong Material để thay đổi màu sắc của hiệu ứng Post Process
void UPPM_DoiMauPostProcessComponent::UpdateColorTinting(FLinearColor NewColor)
{
	if (!DynamicMat)
	{
		return;
	}
	
	// Cập nhật giá trị Vector Parameter có tên là "ColorTinting" trong Material
	DynamicMat->SetVectorParameterValue(ColorParamName, NewColor);
}

// Called every frame
void UPPM_DoiMauPostProcessComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
