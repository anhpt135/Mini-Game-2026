// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MG26_Pawn_03_SideScroller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "InputActionValue.h"

AMG26_Pawn_03_SideScroller::AMG26_Pawn_03_SideScroller()
{
	// 1. Tắt xoay Pawn bằng chuột (Vì đây là game 2D)
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	if (SpringArmComponent)
	{
		// 2. Chốt cứng Camera không cho xoay theo người chơi
		SpringArmComponent->bUsePawnControlRotation = false;
		
		// 3. Kéo xa Camera ra một chút (bạn có thể chỉnh lại trên Blueprint)
		SpringArmComponent->TargetArmLength = 1000.0f; 
		
		// 4. Xoay Camera nằm ngang góc -90 độ so với nhân vật (nhìn dọc theo trục Y, bao quát mặt phẳng XZ)
		SpringArmComponent->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
		
		// Tắt collision của camera để không bị giật nếu có vật cản phía trước
		SpringArmComponent->bDoCollisionTest = false; 
	}
}

void AMG26_Pawn_03_SideScroller::BeginPlay()
{
	Super::BeginPlay();

	// Lưu lại tọa độ trục Y hiện tại của Pawn khi bắt đầu màn chơi
	LockedYPosition = GetActorLocation().Y;
}

void AMG26_Pawn_03_SideScroller::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Ép Pawn luôn nằm trên trục Y đã khóa
	FVector CurrentLocation = GetActorLocation();
	
	// Chỉ set lại vị trí nếu bị lọt ra khỏi mặt phẳng
	if (!FMath::IsNearlyEqual(CurrentLocation.Y, LockedYPosition, 0.1f))
	{
		CurrentLocation.Y = LockedYPosition;
		SetActorLocation(CurrentLocation);
	}
}

void AMG26_Pawn_03_SideScroller::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// Cố định hướng di chuyển ngang là trục X của thế giới (World X)
		FVector ForwardDirection = FVector(1.0f, 0.0f, 0.0f);
		
		// Bỏ qua Input Y (tiến/lùi), chỉ dùng Input X (trái/phải)
		AddMovementInput(ForwardDirection, MovementVector.X);
		
		// Ép nhân vật quay mặt sang trái/phải tương ứng với hướng đi
		if (MovementVector.X > 0.1f)
		{
			SetActorRotation(FRotator(0.0f, 0.0f, 0.0f)); // Nhìn phải
		}
		else if (MovementVector.X < -0.1f)
		{
			SetActorRotation(FRotator(0.0f, 180.0f, 0.0f)); // Nhìn trái
		}
	}
}

void AMG26_Pawn_03_SideScroller::Look(const FInputActionValue& Value)
{
	// Bỏ trống hàm này! Camera 2D Side-Scroller thì không cho phép người chơi tự xoay bằng chuột.
}
