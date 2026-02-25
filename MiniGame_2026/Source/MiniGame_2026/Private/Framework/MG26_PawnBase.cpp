// Điền thông báo bản quyền của bạn vào trang Mô tả của Cài đặt Dự án.


#include "Framework/MG26_PawnBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"

// Thiết lập các giá trị mặc định
AMG26_PawnBase::AMG26_PawnBase()
{
	// Đặt pawn này gọi Tick() mỗi khung hình. Bạn có thể tắt tính năng này để cải thiện hiệu suất nếu không cần.
	PrimaryActorTick.bCanEverTick = true;

	// Tạo Capsule Component
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->InitCapsuleSize(42.f, 96.0f);
	CapsuleComponent->SetCollisionProfileName(TEXT("Pawn"));
	RootComponent = CapsuleComponent;

	// Tạo Spring Arm Component
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(CapsuleComponent); // Gắn Spring Arm vào Capsule
	SpringArmComponent->TargetArmLength = 300.0f; // Khoảng cách từ camera đến nhân vật
	SpringArmComponent->bUsePawnControlRotation = true; // Xoay arm dựa trên controller

	// Tạo Camera Component
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	// QUAN TRỌNG: Gắn Camera làm con của Spring Arm tại socket cuối cùng
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName); 
	CameraComponent->bUsePawnControlRotation = false; // Camera không cần xoay độc lập vì Spring Arm đã xoay

	// Tạo Movement Component
	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));
	MovementComponent->UpdatedComponent = CapsuleComponent;

	// Cho phép Pawn xoay theo Controller
	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = false; // Thường thì Pawn không cần xoay Pitch (ngửa lên/xuống), chỉ Camera xoay
	bUseControllerRotationRoll = false;

	VerticalVelocity = 0.0f;
	bIsGrounded = false;
	JumpVelocity = 600.0f;
	GravityStrength = 980.0f;
}

// Được gọi khi trò chơi bắt đầu hoặc khi được sinh ra
void AMG26_PawnBase::BeginPlay()
{
	Super::BeginPlay();

	// Thêm Bối cảnh Ánh xạ Đầu vào
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			if (DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
}

// Được gọi mỗi khung hình
void AMG26_PawnBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Triển khai trọng lực đơn giản
	// Kiểm tra xem chúng ta có đang ở trên mặt đất không
	FHitResult Hit;
	FVector Start = GetActorLocation();
	FVector End = Start - FVector(0.0f, 0.0f, CapsuleComponent->GetScaledCapsuleHalfHeight() + 2.0f); // Dịch chuyển một khoảng nhỏ xuống dưới
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHitGround = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);

	if (bHitGround && VerticalVelocity <= 0.0f)
	{
		bIsGrounded = true;
		VerticalVelocity = 0.0f;
	}
	else
	{
		bIsGrounded = false;
		VerticalVelocity -= GravityStrength * DeltaTime;
	}

	// Áp dụng chuyển động theo chiều dọc
	if (VerticalVelocity != 0.0f)
	{
		FVector DeltaLocation = FVector(0.0f, 0.0f, VerticalVelocity * DeltaTime);
		AddActorLocalOffset(DeltaLocation, true);
	}
}

// Được gọi để liên kết chức năng với đầu vào
void AMG26_PawnBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Thiết lập các liên kết hành động
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Nhảy
		if (JumpAction)
		{
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AMG26_PawnBase::Jump);
		}

		// Di chuyển
		if (MoveAction)
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMG26_PawnBase::Move);
		}

		// Nhìn
		if (LookAction)
		{
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMG26_PawnBase::Look);
		}
	}
}

void AMG26_PawnBase::Move(const FInputActionValue& Value)
{
	// đầu vào là một Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// tìm ra hướng nào là phía trước
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// lấy vector phía trước
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// lấy vector bên phải
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// thêm chuyển động
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AMG26_PawnBase::Look(const FInputActionValue& Value)
{
	// đầu vào là một Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// thêm đầu vào yaw và pitch cho controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AMG26_PawnBase::Jump(const FInputActionValue& Value)
{
	if (bIsGrounded)
	{
		VerticalVelocity = JumpVelocity;
		bIsGrounded = false;
	}
}
