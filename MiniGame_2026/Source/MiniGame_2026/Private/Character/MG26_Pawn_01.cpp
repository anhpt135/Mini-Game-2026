// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MG26_Pawn_01.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

AMG26_Pawn_01::AMG26_Pawn_01()
{
	// Thiết lập các giá trị mặc định cho xe
	CurrentForwardSpeed = 0.0f;
	CurrentTurnInput = 0.0f;
	CurrentThrottleInput = 0.0f;

	// Tắt xoay theo controller vì xe sẽ tự xoay
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	// Cấu hình Spring Arm cho xe (xa hơn và có độ trễ)
	if (SpringArmComponent)
	{
		SpringArmComponent->TargetArmLength = 600.0f; // Xa hơn để nhìn bao quát
		SpringArmComponent->bEnableCameraLag = true; // Bật độ trễ camera
		SpringArmComponent->CameraLagSpeed = 3.0f;
		SpringArmComponent->bUsePawnControlRotation = true; // Vẫn cho phép xoay camera quanh xe
		SpringArmComponent->bInheritPitch = true;
		SpringArmComponent->bInheritYaw = true;
		SpringArmComponent->bInheritRoll = false;
	}

	// Vô hiệu hóa FloatingPawnMovement để tránh xung đột
	if (MovementComponent)
	{
		MovementComponent->Deactivate();
		MovementComponent->SetComponentTickEnabled(false);
	}
}

void AMG26_Pawn_01::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Gọi Super để bind các action cơ bản (Jump, Look)
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind lại MoveAction với sự kiện Completed để reset input
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MoveAction)
		{
			// Bind sự kiện Completed (nhả phím) để reset input về 0
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AMG26_Pawn_01::StopMoving);
		}
	}
}

void AMG26_Pawn_01::Move(const FInputActionValue& Value)
{
	// Lấy giá trị input (W/S là Y, A/D là X)
	FVector2D MovementVector = Value.Get<FVector2D>();
	CurrentThrottleInput = MovementVector.Y; // Ga/Phanh
	CurrentTurnInput = MovementVector.X; // Lái trái/phải
}

void AMG26_Pawn_01::StopMoving(const FInputActionValue& Value)
{
	// Reset input khi nhả phím
	CurrentThrottleInput = 0.0f;
	CurrentTurnInput = 0.0f;
}

void AMG26_Pawn_01::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 1. Xử lý Tốc độ (Ga/Phanh)
	if (CurrentThrottleInput != 0.0f)
	{
		// Kiểm tra xem có đang phanh không (Input ngược chiều với tốc độ hiện tại)
		bool bIsBraking = (CurrentThrottleInput > 0.0f && CurrentForwardSpeed < -10.0f) || 
						  (CurrentThrottleInput < 0.0f && CurrentForwardSpeed > 10.0f);

		if (bIsBraking)
		{
			// Áp dụng lực phanh (mạnh hơn gia tốc thường)
			// Dùng FMath::Sign để xác định hướng phanh (ngược chiều tốc độ)
			float BrakeDirection = -FMath::Sign(CurrentForwardSpeed);
			CurrentForwardSpeed += BrakeDirection * BrakingDeceleration * DeltaTime;
		}
		else
		{
			// Tăng tốc bình thường
			CurrentForwardSpeed += CurrentThrottleInput * Acceleration * DeltaTime;
		}
	}
	else
	{
		// Giảm tốc do ma sát khi thả ga
		if (FMath::Abs(CurrentForwardSpeed) > 10.0f)
		{
			float FrictionForce = -FMath::Sign(CurrentForwardSpeed) * Friction * 500.0f * DeltaTime;
			CurrentForwardSpeed += FrictionForce;
			
			// Nếu tốc độ quá nhỏ thì dừng hẳn để tránh trôi mãi
			if (FMath::Abs(CurrentForwardSpeed) < 20.0f)
			{
				CurrentForwardSpeed = 0.0f;
			}
		}
		else
		{
			CurrentForwardSpeed = 0.0f;
		}
	}

	// Giới hạn tốc độ tối đa
	CurrentForwardSpeed = FMath::Clamp(CurrentForwardSpeed, -MaxSpeed, MaxSpeed);

	// 2. Xử lý Xoay (Lái)
	// Chỉ xoay được khi xe đang di chuyển (hoặc xoay rất chậm khi đứng yên)
	float TurnAmount = CurrentTurnInput * TurnSpeed * DeltaTime;
	
	// Nếu đang lùi, đảo ngược hướng lái để cảm giác tự nhiên hơn
	if (CurrentForwardSpeed < -10.0f)
	{
		TurnAmount *= -1.0f;
	}

	// Xoay Actor
	if (FMath::Abs(CurrentForwardSpeed) > 10.0f) // Chỉ xoay khi có tốc độ
	{
		AddActorLocalRotation(FRotator(0.0f, TurnAmount, 0.0f));
	}

	// 3. Xử lý Di chuyển (Vị trí)
	// Tính toán vector di chuyển dựa trên hướng hiện tại của xe
	FVector ForwardDirection = GetActorForwardVector();
	FVector DesiredMovement = ForwardDirection * CurrentForwardSpeed * DeltaTime;

	// Áp dụng di chuyển (sử dụng AddActorWorldOffset thay vì AddMovementInput để kiểm soát trực tiếp)
	FHitResult Hit;
	AddActorWorldOffset(DesiredMovement, true, &Hit);

	// Xử lý va chạm đơn giản (dừng lại khi đâm)
	if (Hit.IsValidBlockingHit())
	{
		CurrentForwardSpeed = 0.0f;
	}
}
