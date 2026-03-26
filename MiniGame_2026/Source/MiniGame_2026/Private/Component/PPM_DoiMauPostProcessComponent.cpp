// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PPM_DoiMauPostProcessComponent.h"
#include "Camera/CameraComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
// #include "Engine/Engine.h" // Không cần thiết khi dùng UE_LOG

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
		if (OwnerCamera)
		{
			UE_LOG(LogTemp, Log, TEXT("PPM_DoiMauPostProcessComponent: Found OwnerCamera!"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("PPM_DoiMauPostProcessComponent: OwnerActor found, but NO UCameraComponent!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PPM_DoiMauPostProcessComponent: GetOwner() returned NULL!"));
	}

	// 2. Tạo Dynamic Material từ Material gốc bạn thiết lập ngoài Editor
	if (BasePostProcessMat)
	{
		DynamicMat = UMaterialInstanceDynamic::Create(BasePostProcessMat, this);
		if (DynamicMat)
		{
			UE_LOG(LogTemp, Log, TEXT("PPM_DoiMauPostProcessComponent: DynamicMat created successfully!"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("PPM_DoiMauPostProcessComponent: Failed to create DynamicMat from BasePostProcessMat!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PPM_DoiMauPostProcessComponent: BasePostProcessMat is NULL in editor!"));
	}
}

// Hàm Bật/Tắt hiệu ứng, được gọi từ sự kiện OnCheckStateChanged của Checkbox
void UPPM_DoiMauPostProcessComponent::TogglePostProcess(bool bEnableEffect)
{
	if (!OwnerCamera || !DynamicMat)
	{
		UE_LOG(LogTemp, Error, TEXT("PPM_DoiMauPostProcessComponent: TogglePostProcess failed - Camera or DynamicMat is NULL!"));
		return;
	}

	if (bEnableEffect)
	{
		// Thêm hiệu ứng vào Camera với mức hiển thị (Weight) là 100%
		OwnerCamera->PostProcessSettings.AddBlendable(DynamicMat, 1.0f);
		UE_LOG(LogTemp, Log, TEXT("PPM_DoiMauPostProcessComponent: PostProcess Effect Enabled!"));
	}
	else
	{
		// Gỡ hiệu ứng ra khỏi Camera
		OwnerCamera->PostProcessSettings.RemoveBlendable(DynamicMat);
		UE_LOG(LogTemp, Log, TEXT("PPM_DoiMauPostProcessComponent: PostProcess Effect Disabled!"));
	}
}

// Hàm cập nhật màu sắc, được gọi từ sự kiện OnValueChanged của 3 thanh Slider (sau khi đã gom lại thành 1 LinearColor)
void UPPM_DoiMauPostProcessComponent::UpdateColorTinting(FLinearColor NewColor)
{
	if (!DynamicMat)
	{
		UE_LOG(LogTemp, Error, TEXT("PPM_DoiMauPostProcessComponent: UpdateColorTinting failed - DynamicMat is NULL!"));
		return;
	}
	
	// Cập nhật giá trị Vector Parameter có tên là "ColorTinting" trong Material
	DynamicMat->SetVectorParameterValue(ColorParamName, NewColor);
	UE_LOG(LogTemp, Log, TEXT("PPM_DoiMauPostProcessComponent: ColorTinting updated to R:%.2f G:%.2f B:%.2f"), NewColor.R, NewColor.G, NewColor.B);
}

// Called every frame
void UPPM_DoiMauPostProcessComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
