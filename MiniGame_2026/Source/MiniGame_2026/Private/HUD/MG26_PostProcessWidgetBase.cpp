// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MG26_PostProcessWidgetBase.h"
#include "GameFramework/Pawn.h"
#include "Components/CheckBox.h"
#include "Components/Slider.h"
#include "Component/PPM_DoiMauPostProcessComponent.h"
// #include "Engine/Engine.h" // Không cần thiết khi dùng UE_LOG

void UMG26_PostProcessWidgetBase::NativeConstruct()
{
	// Luôn gọi hàm của lớp cha trước tiên!
	Super::NativeConstruct();

	// Logic tìm kiếm PostProcessComponentRef và bind sự kiện đã được chuyển sang SetPostProcessComponent()
	// để đảm bảo component đã sẵn sàng khi widget được khởi tạo.
}

void UMG26_PostProcessWidgetBase::SetPostProcessComponent(UPPM_DoiMauPostProcessComponent* InComponent)
{
	PostProcessComponentRef = InComponent;

	if (PostProcessComponentRef)
	{
		UE_LOG(LogTemp, Log, TEXT("UMG26_PostProcessWidgetBase: SetPostProcessComponent - PostProcessComponentRef set successfully!"));

		// --- Đấu dây (Bind) sự kiện từ UI vào các hàm C++ ---
		if (Check_ToggleEffect)
		{
			Check_ToggleEffect->OnCheckStateChanged.AddDynamic(this, &UMG26_PostProcessWidgetBase::HandleToggleEffectChanged);
			UE_LOG(LogTemp, Log, TEXT("UMG26_PostProcessWidgetBase: Bind thanh cong: Check_ToggleEffect"));
			// Khởi tạo trạng thái ban đầu của Checkbox
			// Giả định PostProcessComponentRef có hàm IsEffectEnabled()
			// Check_ToggleEffect->SetIsChecked(PostProcessComponentRef->IsEffectEnabled());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("UMG26_PostProcessWidgetBase: ERROR BindWidget: Khong tim thay Check_ToggleEffect trong Blueprint!"));
		}

		// Giả định PostProcessComponentRef có hàm GetCurrentColor() trả về FLinearColor
		// FLinearColor CurrentColor = PostProcessComponentRef->GetCurrentColor();

		if (Slider_R)
		{
			Slider_R->OnValueChanged.AddDynamic(this, &UMG26_PostProcessWidgetBase::HandleColorSliderChanged);
			// Slider_R->SetValue(CurrentColor.R);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("UMG26_PostProcessWidgetBase: ERROR BindWidget: Khong tim thay Slider_R!"));
		}

		if (Slider_G)
		{
			Slider_G->OnValueChanged.AddDynamic(this, &UMG26_PostProcessWidgetBase::HandleColorSliderChanged);
			// Slider_G->SetValue(CurrentColor.G);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("UMG26_PostProcessWidgetBase: ERROR BindWidget: Khong tim thay Slider_G!"));
		}

		if (Slider_B)
		{
			Slider_B->OnValueChanged.AddDynamic(this, &UMG26_PostProcessWidgetBase::HandleColorSliderChanged);
			// Slider_B->SetValue(CurrentColor.B);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("UMG26_PostProcessWidgetBase: ERROR BindWidget: Khong tim thay Slider_B!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMG26_PostProcessWidgetBase: SetPostProcessComponent - InComponent is NULL!"));
	}
}

void UMG26_PostProcessWidgetBase::HandleToggleEffectChanged(bool bIsChecked)
{
	UE_LOG(LogTemp, Log, TEXT("UMG26_PostProcessWidgetBase: EVENT TRIGGERED: Checkbox changed to %d"), bIsChecked);

	if (PostProcessComponentRef)
	{
		PostProcessComponentRef->TogglePostProcess(bIsChecked);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMG26_PostProcessWidgetBase: HandleToggleEffectChanged failed - PostProcessComponentRef is NULL!"));
	}
}

void UMG26_PostProcessWidgetBase::HandleColorSliderChanged(float InValue)
{
	if (PostProcessComponentRef && Slider_R && Slider_G && Slider_B)
	{
		// Lấy giá trị từ cả 3 slider và gom thành một FLinearColor
		const FLinearColor NewColor(Slider_R->GetValue(), Slider_G->GetValue(), Slider_B->GetValue());
		PostProcessComponentRef->UpdateColorTinting(NewColor);
		UE_LOG(LogTemp, Log, TEXT("UMG26_PostProcessWidgetBase: Calling UpdateColorTinting with R:%.2f G:%.2f B:%.2f"), NewColor.R, NewColor.G, NewColor.B);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMG26_PostProcessWidgetBase: HandleColorSliderChanged failed - PostProcessComponentRef or Sliders are NULL!"));
	}
}
