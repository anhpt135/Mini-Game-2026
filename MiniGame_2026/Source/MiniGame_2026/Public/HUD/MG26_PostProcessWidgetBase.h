// Fill out your copyright notice in the Description page of Project Settings.
//1. Mục tiêu là tạo ra con trỏ địa chỉ tên là PostProcessComponentRef trỏ về Component PostProcess của Pawn/Character
//2. Thông qua con trỏ tiếp cận Component và truyền giá trị từ UI qua Component
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MG26_PostProcessWidgetBase.generated.h"

// Forward Declarations
class UPPM_DoiMauPostProcessComponent;
class UCheckBox;
class USlider;

/**
 * Lớp cha C++ cho Widget quản lý Post Process
 */
UCLASS()
class MINIGAME_2026_API UMG26_PostProcessWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	 * @brief Thiết lập con trỏ tới component backend và đấu dây các sự kiện UI.
	 * @param InComponent Con trỏ tới Post Process Component trên Pawn.
	 */
	void SetPostProcessComponent(UPPM_DoiMauPostProcessComponent* InComponent);

protected:
	// ~~~ Vòng đời Widget ~~~
	virtual void NativeConstruct() override;

	// ~~~ Con trỏ Backend ~~~
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Post Process")
	TObjectPtr<UPPM_DoiMauPostProcessComponent> PostProcessComponentRef;

	// ==========================================
	// BINDING WIDGETS TỪ BLUEPRINT
	// Tên biến phải CHÍNH XÁC với tên Widget trong Designer
	// ==========================================
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCheckBox> Check_ToggleEffect;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USlider> Slider_R;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USlider> Slider_G;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USlider> Slider_B;

	// ==========================================
	// CÁC HÀM XỬ LÝ SỰ KIỆN (DELEGATES)
	// ==========================================
	UFUNCTION()
	void HandleToggleEffectChanged(bool bIsChecked);

	UFUNCTION()
	void HandleColorSliderChanged(float InValue);
};
