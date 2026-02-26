// Điền thông báo bản quyền của bạn vào trang Mô tả của Cài đặt Dự án.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "MG26_PlayerControllerBase.generated.h"

class UInputMappingContext;
class UInputAction;
class AMG26_TriggerButton; // Khai báo trước (Forward declaration)

/**
 * 
 */
UCLASS()
class MINIGAME_2026_API AMG26_PlayerControllerBase : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void SetupInputComponent() override;

	// Hàm chuyển quyền điều khiển sang xe
	UFUNCTION(BlueprintCallable, Category = "Vehicle")
	void SwitchToVehicle(APawn* NewVehiclePawn, AMG26_TriggerButton* Trigger);

	// Hàm chuyển quyền điều khiển lại cho nhân vật
	UFUNCTION(BlueprintCallable, Category = "Character")
	void SwitchToCharacter();

	// Cờ kiểm soát xem trigger có thể chiếm quyền điều khiển pawn hay không
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interaction")
	bool bCanTriggerPossess;

	// Hàm đặt lại cờ chiếm quyền điều khiển trigger
	void ResetTriggerPossess();

protected:
	virtual void BeginPlay() override;

	// Hành động đầu vào để chuyển lại nhân vật
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* SwitchToCharacterAction;

private:
	// Tham chiếu đến pawn nhân vật
	UPROPERTY()
	APawn* CharacterPawn;

	// Tham chiếu đến pawn xe
	UPROPERTY()
	APawn* VehiclePawn;

	// Tham chiếu đến nút trigger hoạt động cuối cùng
	UPROPERTY()
	AMG26_TriggerButton* LastActiveTrigger;

	// Handle bộ đếm thời gian để đặt lại việc chiếm quyền điều khiển trigger
	FTimerHandle TimerHandle_ResetTriggerPossess;

	void OnSwitchToCharacter(const FInputActionValue& Value);
};
