// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MG26_Pawn_02.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

AMG26_Pawn_02::AMG26_Pawn_02()
{
	// Tắt xoay theo controller vì trực thăng sẽ tự xoay
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	// Cấu hình Spring Arm cho trực thăng (xa hơn và cao hơn)
	if (SpringArmComponent)
	{
		SpringArmComponent->TargetArmLength = 800.0f; // Xa hơn để nhìn bao quát
		SpringArmComponent->SocketOffset = FVector(0.0f, 0.0f, 200.0f); // Cao hơn một chút
		SpringArmComponent->bEnableCameraLag = true; // Bật độ trễ camera
		SpringArmComponent->CameraLagSpeed = 3.0f;
		SpringArmComponent->bUsePawnControlRotation = true; // Vẫn cho phép xoay camera quanh xe
		SpringArmComponent->bInheritPitch = true;
		SpringArmComponent->bInheritYaw = true;
		SpringArmComponent->bInheritRoll = false;
	}

	// Vô hiệu hóa FloatingPawnMovement để tự xử lý vật lý
	if (MovementComponent)
	{
		MovementComponent->Deactivate();
		MovementComponent->SetComponentTickEnabled(false);
	}

	// Khởi tạo giá trị
	TargetForwardInput = 0.0f;
	TargetTurnInput = 0.0f;
	bIsAscending = false;
	CurrentForwardSpeed = 0.0f;
	CurrentVerticalSpeed = 0.0f;
	CurrentYawSpeed = 0.0f;
}

void AMG26_Pawn_02::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Gọi Super để bind các action cơ bản (Look)
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Bind MoveAction (W/S/A/D)
		if (MoveAction)
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMG26_Pawn_02::Move);
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AMG26_Pawn_02::Move); // Reset khi thả phím
		}

		// Bind JumpAction (Space) cho bay lên
		if (JumpAction)
		{
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AMG26_Pawn_02::StartAscend);
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AMG26_Pawn_02::StopAscend);
		}
	}
}

void AMG26_Pawn_02::Move(const FInputActionValue& Value)
{
	// Lấy giá trị input (W/S là Y, A/D là X)
	FVector2D MovementVector = Value.Get<FVector2D>();
	TargetForwardInput = MovementVector.Y; // Bay tới/lùi
	TargetTurnInput = MovementVector.X; // Xoay trái/phải
}

void AMG26_Pawn_02::StartAscend(const FInputActionValue& Value)
{
	bIsAscending = true;
}

void AMG26_Pawn_02::StopAscend(const FInputActionValue& Value)
{
	bIsAscending = false;
}

void AMG26_Pawn_02::Tick(float DeltaTime)
{
	// QUAN TRỌNG: KHÔNG gọi Super::Tick(DeltaTime) ở đây
	// Vì Base class sẽ áp dụng trọng lực riêng gây xung đột với logic bay của chúng ta.
	
	// 1. Xử lý Bay Lên / Xuống (Vertical Movement)
	float CurrentAltitude = GetActorLocation().Z;

	if (bIsAscending)
	{
		// Kiểm tra độ cao tối đa
		if (CurrentAltitude < MaxAltitude)
		{
			// Đang giữ Space và chưa đạt độ cao tối đa: Tăng tốc độ bay lên
			CurrentVerticalSpeed += LiftAcceleration * DeltaTime;
		}
		else
		{
			// Đã đạt độ cao tối đa: Giảm tốc độ bay lên về 0 hoặc đẩy nhẹ xuống
			if (CurrentVerticalSpeed > 0.0f)
			{
				CurrentVerticalSpeed -= LiftAcceleration * DeltaTime; // Giảm tốc từ từ cho mượt
				if (CurrentVerticalSpeed < 0.0f) CurrentVerticalSpeed = 0.0f;
			}
		}
	}
	else
	{
		// Thả Space: Bị trọng lực kéo xuống
		CurrentVerticalSpeed -= GravityForce * DeltaTime;
	}

	// Giới hạn tốc độ lên/xuống
	CurrentVerticalSpeed = FMath::Clamp(CurrentVerticalSpeed, -MaxVerticalSpeed, MaxVerticalSpeed);

	// Kiểm tra va chạm mặt đất để dừng rơi
	bool bIsLanded = false;
	FHitResult GroundHit;
	FVector Start = GetActorLocation();
	// SỬA LỖI: Tính toán độ dài tia dựa trên chiều cao Capsule + một khoảng dư (ví dụ 10 đơn vị)
	float TraceDistance = CapsuleComponent->GetScaledCapsuleHalfHeight() + 10.0f;
	FVector End = Start - FVector(0.0f, 0.0f, TraceDistance); 
	
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(GroundHit, Start, End, ECC_Visibility, Params))
	{
		// Nếu chạm đất và đang rơi xuống (hoặc đứng yên) -> Dừng lại
		// Nới lỏng điều kiện < 0.0f thành <= 10.0f để bắt dính tốt hơn
		if (CurrentVerticalSpeed <= 10.0f)
		{
			CurrentVerticalSpeed = 0.0f;
			bIsLanded = true;
			// Đẩy nhẹ lên để không bị chìm xuống đất
			SetActorLocation(GroundHit.Location + FVector(0.0f, 0.0f, CapsuleComponent->GetScaledCapsuleHalfHeight()));
			
			// GIẢM TỐC ĐỘ DI CHUYỂN KHI TIẾP ĐẤT (Ma sát mặt đất)
			// Giúp xe dừng lại nhanh hơn và giảm độ nghiêng (Pitch)
			CurrentForwardSpeed = FMath::FInterpTo(CurrentForwardSpeed, 0.0f, DeltaTime, 5.0f);
		}
	}

	// 2. Xử lý Bay Tới / Lùi (Forward Movement)
	// Tăng/giảm tốc độ dựa trên input
	if (TargetForwardInput != 0.0f)
	{
		CurrentForwardSpeed += TargetForwardInput * Acceleration * DeltaTime;
	}
	else
	{
		// Giảm tốc độ khi không nhấn phím (ma sát không khí)
		if (FMath::Abs(CurrentForwardSpeed) > 10.0f)
		{
			CurrentForwardSpeed -= FMath::Sign(CurrentForwardSpeed) * (Acceleration * 0.5f) * DeltaTime;
		}
		else
		{
			// Nếu không phải đang ở dưới đất (đã xử lý ở trên) thì mới set về 0 ở đây
			if (!bIsLanded) 
			{
				CurrentForwardSpeed = 0.0f;
			}
		}
	}
	CurrentForwardSpeed = FMath::Clamp(CurrentForwardSpeed, -MaxForwardSpeed, MaxForwardSpeed);

	// 3. Xử lý Xoay (Yaw Rotation)
	float TurnAmount = TargetTurnInput * TurnSpeed * DeltaTime;
	AddActorLocalRotation(FRotator(0.0f, TurnAmount, 0.0f));

	// 4. Áp dụng Di chuyển (Position Update)
	FVector ForwardDir = GetActorForwardVector();
	FVector VerticalDir = FVector::UpVector;
	
	FVector DeltaLocation = (ForwardDir * CurrentForwardSpeed * DeltaTime) + (VerticalDir * CurrentVerticalSpeed * DeltaTime);
	
	FHitResult MoveHit;
	AddActorWorldOffset(DeltaLocation, true, &MoveHit);

	// Xử lý va chạm khi bay (dừng lại nếu đâm vào tường)
	if (MoveHit.IsValidBlockingHit())
	{
		// Nếu đâm ngang -> Dừng bay tới
		if (FMath::Abs(MoveHit.ImpactNormal.Z) < 0.5f) 
		{
			CurrentForwardSpeed = 0.0f;
		}
		// Nếu đâm trần -> Dừng bay lên
		else if (MoveHit.ImpactNormal.Z < -0.5f && CurrentVerticalSpeed > 0.0f)
		{
			CurrentVerticalSpeed = 0.0f;
		}
	}

	// 5. Visual Banking (Nghiêng xe để tạo cảm giác bay)
	// Nghiêng mũi xuống khi bay tới (Pitch)
	float TargetPitch = CurrentForwardSpeed / MaxForwardSpeed * -15.0f; // Tối đa nghiêng 15 độ
	// Nghiêng cánh khi xoay (Roll)
	float TargetRoll = TargetTurnInput * 20.0f; // Nghiêng 20 độ khi xoay

	// Nếu đang ở dưới đất, ép về 0 nhanh hơn
	if (bIsLanded)
	{
		TargetPitch = 0.0f;
		TargetRoll = 0.0f;
	}

	FRotator CurrentRot = GetActorRotation();
	// Tăng tốc độ hồi phục: 6.0f khi bay, 15.0f khi ở dưới đất
	float InterpSpeed = bIsLanded ? 15.0f : 6.0f;
	FRotator NewRot = FMath::RInterpTo(CurrentRot, FRotator(TargetPitch, CurrentRot.Yaw, TargetRoll), DeltaTime, InterpSpeed);
	
	// Set Rotation mới (giữ nguyên Yaw đã tính ở bước 3)
	SetActorRotation(FRotator(NewRot.Pitch, GetActorRotation().Yaw, NewRot.Roll));
}
