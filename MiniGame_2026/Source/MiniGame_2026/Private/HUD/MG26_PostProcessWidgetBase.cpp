
#include "HUD/MG26_PostProcessWidgetBase.h"
#include "GameFramework/Pawn.h"
#include "Components/CheckBox.h"
#include "Components/Slider.h"
#include "Component/PPM_DoiMauPostProcessComponent.h"

// Hàm này lấy tham chiếu đến Component từ Pawn và đăng ký các sự kiện của UI (CheckBox và Sliders)
void UMG26_PostProcessWidgetBase::SetPostProcessComponent(UPPM_DoiMauPostProcessComponent* InComponent)
{
	PostProcessComponentRef = InComponent;

	if (PostProcessComponentRef)
	{
		// Đăng ký sự kiện CheckBox.
		if (Check_ToggleEffect)
		{
			Check_ToggleEffect->OnCheckStateChanged.AddDynamic(this, &UMG26_PostProcessWidgetBase::CallbackToggleEffectChanged);
		}
		
		// Đăng ký sự kiện Slider.
		if (Slider_R)
		{
			Slider_R->OnValueChanged.AddDynamic(this, &UMG26_PostProcessWidgetBase::CallbackColorSliderChanged);
		}
		
		if (Slider_G)
		{
			Slider_G->OnValueChanged.AddDynamic(this, &UMG26_PostProcessWidgetBase::CallbackColorSliderChanged);
		}
		
		if (Slider_B)
		{
			Slider_B->OnValueChanged.AddDynamic(this, &UMG26_PostProcessWidgetBase::CallbackColorSliderChanged);
		}
	}
}

// 2 hàm callback truyền tham số (true/false và màu) ngược lại component
void UMG26_PostProcessWidgetBase::CallbackToggleEffectChanged(bool bIsChecked)
{
	if (PostProcessComponentRef)
	{
		PostProcessComponentRef->TogglePostProcess(bIsChecked);
	}
}
void UMG26_PostProcessWidgetBase::CallbackColorSliderChanged(float InValue)
{
	if (PostProcessComponentRef && Slider_R && Slider_G && Slider_B)
	{
		const FLinearColor NewColor(Slider_R->GetValue(), Slider_G->GetValue(), Slider_B->GetValue());
		PostProcessComponentRef->UpdateColorTinting(NewColor);
	}
}
