
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MG26_PostProcessWidgetBase.generated.h"

// Khai báo sớm các lớp.
class UPPM_DoiMauPostProcessComponent;
class UCheckBox;
class USlider;

// Widget UI điều khiển hiệu ứng Post Process.
UCLASS()
class MINIGAME_2026_API UMG26_PostProcessWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	// Thiết lập Post Process Component và đăng ký sự kiện UI.
	void SetPostProcessComponent(UPPM_DoiMauPostProcessComponent* InComponent);

protected:
	// Con trỏ tới Post Process Component.
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Post Process")
	TObjectPtr<UPPM_DoiMauPostProcessComponent> PostProcessComponentRef;

	// Binding Widgets từ Blueprint (tên biến khớp với Designer).
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCheckBox> Check_ToggleEffect; // CheckBox bật/tắt hiệu ứng.

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USlider> Slider_R; // Slider điều chỉnh màu Đỏ.

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USlider> Slider_G; // Slider điều chỉnh màu Xanh Lá.

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USlider> Slider_B; // Slider điều chỉnh màu Xanh Dương.

	// Các hàm callback xử lý sự kiện.
	UFUNCTION()
	void CallbackToggleEffectChanged(bool bIsChecked); // Xử lý thay đổi trạng thái CheckBox bật/tắt hiệu ứng.

	UFUNCTION()
	void CallbackColorSliderChanged(float InValue); // Xử lý khi giá trị Slider màu thay đổi.
};
