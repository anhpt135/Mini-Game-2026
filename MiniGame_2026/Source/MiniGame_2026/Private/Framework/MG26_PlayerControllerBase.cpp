// Điền thông báo bản quyền của bạn vào trang Mô tả của Cài đặt Dự án.


#include "Framework/MG26_PlayerControllerBase.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Pawn.h"
#include "Interaction/MG26_TriggerButton.h"
#include "TimerManager.h"

void AMG26_PlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();

	bCanTriggerPossess = true;

	// Thêm Bối cảnh Ánh xạ Đầu vào (Input Mapping Context)
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (InputMappingContext)
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}

void AMG26_PlayerControllerBase::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		if (SwitchToCharacterAction)
		{
			EnhancedInputComponent->BindAction(SwitchToCharacterAction, ETriggerEvent::Triggered, this, &AMG26_PlayerControllerBase::OnSwitchToCharacter);
		}
	}
}

void AMG26_PlayerControllerBase::SwitchToVehicle(APawn* NewVehiclePawn, AMG26_TriggerButton* Trigger)
{
	if (NewVehiclePawn)
	{
		// Lưu pawn nhân vật hiện tại
		CharacterPawn = GetPawn();

		// Chiếm quyền điều khiển pawn xe
		Possess(NewVehiclePawn);

		// Lưu pawn xe
		VehiclePawn = NewVehiclePawn;

		// Lưu nút trigger
		LastActiveTrigger = Trigger;
	}
}

void AMG26_PlayerControllerBase::SwitchToCharacter()
{
	if (CharacterPawn)
	{
		// Chiếm quyền điều khiển pawn nhân vật
		Possess(CharacterPawn);

		// Xóa tham chiếu pawn xe
		VehiclePawn = nullptr;

		// Tạm thời vô hiệu hóa việc chiếm quyền điều khiển trigger
		bCanTriggerPossess = false;

		// Đặt bộ đếm thời gian để kích hoạt lại việc chiếm quyền điều khiển trigger sau một khoảng trễ ngắn
		GetWorldTimerManager().SetTimer(TimerHandle_ResetTriggerPossess, this, &AMG26_PlayerControllerBase::ResetTriggerPossess, 2.0f, false);
	}
}

void AMG26_PlayerControllerBase::ResetTriggerPossess()
{
	bCanTriggerPossess = true;
}

void AMG26_PlayerControllerBase::OnSwitchToCharacter(const FInputActionValue& Value)
{
	SwitchToCharacter();
}
