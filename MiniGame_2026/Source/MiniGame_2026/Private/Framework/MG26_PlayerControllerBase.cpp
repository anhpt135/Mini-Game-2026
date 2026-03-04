// Điền thông báo bản quyền của bạn vào trang Mô tả của Cài đặt Dự án.


#include "Framework/MG26_PlayerControllerBase.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Pawn.h"
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

void AMG26_PlayerControllerBase::SwitchToVehicle(APawn* NewVehiclePawn)
{
	// Kiểm tra thời gian hồi chiêu (Cooldown)
	if (!bCanTriggerPossess)
	{
		return;
	}

	// Kiểm tra xe hợp lệ
	if (!NewVehiclePawn)
	{
		return;
	}

	// Chỉ lưu Pawn nhân vật nếu chưa lên xe
	// Tránh ghi đè CharacterPawn bằng VehiclePawn
	if (VehiclePawn == nullptr)
	{
		CharacterPawn = GetPawn();
	}

	// Chuyển quyền điều khiển sang xe
	Possess(NewVehiclePawn);

	// Cập nhật tham chiếu xe hiện tại
	VehiclePawn = NewVehiclePawn;
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
