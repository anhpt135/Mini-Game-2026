// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MG26_PostProcessWidgetBase.h"
#include "GameFramework/Pawn.h"
#include "Components/CheckBox.h"
#include "Components/Slider.h"
#include "Component/PPM_DoiMauPostProcessComponent.h"

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
		// --- Đấu dây (Bind) sự kiện từ UI vào các hàm C++ ---
		if (Check_ToggleEffect)
		{
			Check_ToggleEffect->OnCheckStateChanged.AddDynamic(this, &UMG26_PostProcessWidgetBase::HandleToggleEffectChanged);
			// Khởi tạo trạng thái ban đầu của Checkbox
			// Giả định PostProcessComponentRef có hàm IsEffectEnabled()
			// Check_ToggleEffect->SetIsChecked(PostProcessComponentRef->IsEffectEnabled());
		}

		// Giả định PostProcessComponentRef có hàm GetCurrentColor() trả về FLinearColor
		// FLinearColor CurrentColor = PostProcessComponentRef->GetCurrentColor();

		if (Slider_R)
		{
			Slider_R->OnValueChanged.AddDynamic(this, &UMG26_PostProcessWidgetBase::HandleColorSliderChanged);
			// Slider_R->SetValue(CurrentColor.R);
		}

		if (Slider_G)
		{
			Slider_G->OnValueChanged.AddDynamic(this, &UMG26_PostProcessWidgetBase::HandleColorSliderChanged);
			// Slider_G->SetValue(CurrentColor.G);
		}

		if (Slider_B)
		{
			Slider_B->OnValueChanged.AddDynamic(this, &UMG26_PostProcessWidgetBase::HandleColorSliderChanged);
			// Slider_B->SetValue(CurrentColor.B);
		}
	}
}

void UMG26_PostProcessWidgetBase::HandleToggleEffectChanged(bool bIsChecked)
{
	if (PostProcessComponentRef)
	{
		PostProcessComponentRef->TogglePostProcess(bIsChecked);
	}
}

void UMG26_PostProcessWidgetBase::HandleColorSliderChanged(float InValue)
{
	if (PostProcessComponentRef && Slider_R && Slider_G && Slider_B)
	{
		// Lấy giá trị từ cả 3 slider và gom thành một FLinearColor
		const FLinearColor NewColor(Slider_R->GetValue(), Slider_G->GetValue(), Slider_B->GetValue());
		PostProcessComponentRef->UpdateColorTinting(NewColor);
	}
}
