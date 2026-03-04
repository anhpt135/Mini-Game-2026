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

	// Sử dụng Cast an toàn thay vì CastChecked để tránh crash tiềm ẩn
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (SwitchToCharacterAction)
		{
			EnhancedInputComponent->BindAction(SwitchToCharacterAction, ETriggerEvent::Triggered, this, &AMG26_PlayerControllerBase::OnSwitchToCharacter);
		}
	}
}

void AMG26_PlayerControllerBase::SwitchToVehicle(APawn* NewVehiclePawn, AMG26_TriggerButton* Trigger)
{
	// Kiểm tra cờ cooldown trước khi thực hiện bất kỳ logic nào
	if (!bCanTriggerPossess)
	{
		return;
	}

	if (NewVehiclePawn)
	{
		// FIX: Chỉ lưu CharacterPawn nếu hiện tại chưa lái xe (VehiclePawn là null)
		// Điều này ngăn việc CharacterPawn bị ghi đè thành chiếc xe khi hàm này được gọi lại.
		if (VehiclePawn == nullptr)
		{
			CharacterPawn = GetPawn();
		}

		// Chiếm quyền điều khiển pawn xe
		Possess(NewVehiclePawn);

		// Lưu pawn xe
		VehiclePawn = NewVehiclePawn;
		
		// Đã loại bỏ việc gán LastActiveTrigger vì biến này không được sử dụng
	}
}

void AMG26_PlayerControllerBase::SwitchToCharacter()
{
	if (CharacterPawn)
	{
		// Chiếm quyền điều khiển pawn nhân vật
		Possess(CharacterPawn);

		// Xóa tham chiếu pawn xe để đánh dấu là đã rời xe
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
