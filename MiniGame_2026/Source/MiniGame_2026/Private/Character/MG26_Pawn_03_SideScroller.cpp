// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MG26_Pawn_03_SideScroller.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "HUD/MG26_PostProcessWidgetBase.h" // Thêm include cho widget C++ của bạn
#include "Component/PPM_DoiMauPostProcessComponent.h" // Thêm include cho component của bạn

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
		
		// Tắt collision của camera để không bị giật nếu có vật cản phía trước
		SpringArmComponent->bDoCollisionTest = false; 
	}
}
// Hàm này được gọi khi Pawn được điều khiển bởi một Controller (thường là PlayerController). Nó sẽ tạo Widget UI và thiết lập chế độ input để tương tác với UI đó.
void AMG26_Pawn_03_SideScroller::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (WidgetCuaTui)
	{
		APlayerController* PlayerController = Cast<APlayerController>(NewController);
		if (PlayerController)
		{
			ConTroWidget = CreateWidget<UUserWidget>(PlayerController, WidgetCuaTui);
			if (ConTroWidget)
			{
				ConTroWidget->AddToViewport();

				// Chuyển sang chế độ input cho phép tương tác với UI
				FInputModeGameAndUI InputModeData;
				InputModeData.SetWidgetToFocus(ConTroWidget->TakeWidget());
				InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
				
				// Áp dụng chế độ input mới cho PlayerController
				PlayerController->SetInputMode(InputModeData);
				PlayerController->bShowMouseCursor = true;
				PlayerController->bEnableClickEvents = true;
				PlayerController->bEnableMouseOverEvents = true;

				// --- Gán PostProcessComponent cho Widget ---
				if (UMG26_PostProcessWidgetBase* PostProcessWidget = Cast<UMG26_PostProcessWidgetBase>(ConTroWidget))
				{
					if (UPPM_DoiMauPostProcessComponent* PPMComponent = FindComponentByClass<UPPM_DoiMauPostProcessComponent>())
					{
						PostProcessWidget->SetPostProcessComponent(PPMComponent);
					}
				}
			}
		}
	}
}
// Hàm này được gọi khi Pawn không còn được điều khiển nữa. Nó sẽ dọn dẹp Widget UI và khôi phục trạng thái input về mặc định (chỉ cho game, ẩn con trỏ chuột).
void AMG26_Pawn_03_SideScroller::UnPossessed()
{
	// Lấy PlayerController TRƯỚC khi gọi hàm cha, vì nó sẽ bị xóa sau đó
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	
	Super::UnPossessed();

	// Dọn dẹp Widget
	if (ConTroWidget)
	{
		ConTroWidget->RemoveFromParent();
		ConTroWidget = nullptr;
	}

	// Khôi phục trạng thái input chỉ cho game và ẩn con trỏ chuột
	if (PlayerController)
	{
		FInputModeGameOnly InputModeData;
		PlayerController->SetInputMode(InputModeData);
		PlayerController->bShowMouseCursor = false;
		PlayerController->bEnableClickEvents = false;
		PlayerController->bEnableMouseOverEvents = false;
	}
}

void AMG26_Pawn_03_SideScroller::BeginPlay()
{
	Super::BeginPlay();

	// --- TÍNH TOÁN TRƯỚC MỌI THỨ DỰA TRÊN GÓC XOAY TRONG EDITOR ---

	// Lưu lại góc xoay ban đầu của Pawn
	InitialRotation = GetActorRotation();

	// Vector chỉ hướng "vào trong" màn hình (vuông góc với hướng nhìn của camera)
	// Đây chính là pháp tuyến của mặt phẳng di chuyển
	FVector PlaneNormal = InitialRotation.RotateVector(FVector::RightVector); // FVector(0,1,0)

	// Tạo mặt phẳng di chuyển dựa trên vị trí và pháp tuyến
	MovementPlane = FPlane(GetActorLocation(), PlaneNormal);

	// Vector chỉ hướng "sang phải" trên màn hình
	RightDirection = InitialRotation.RotateVector(FVector::ForwardVector); // FVector(1,0,0)
}

void AMG26_Pawn_03_SideScroller::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Chiếu vị trí hiện tại của Pawn lên mặt phẳng di chuyển đã tính toán
	// Điều này đảm bảo Pawn luôn bị "hút" về đúng mặt phẳng, dù nó xoay góc nào
	FVector ProjectedLocation = FPlane::PointPlaneProject(GetActorLocation(), MovementPlane);
	SetActorLocation(ProjectedLocation);
}

void AMG26_Pawn_03_SideScroller::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>(); // Đã sửa F2D thành FVector2D

	if (Controller != nullptr)
	{
		// Di chuyển theo hướng "Sang Phải" đã được tính toán trước
		AddMovementInput(RightDirection, MovementVector.X);
		
		// Xoay nhân vật tương đối so với góc xoay ban đầu
		if (MovementVector.X > 0.1f)
		{
			// Nhìn về hướng "Sang Phải"
			SetActorRotation(InitialRotation); 
		}
		else if (MovementVector.X < -0.1f)
		{
			// Nhìn về hướng "Sang Trái" (xoay 180 độ so với hướng ban đầu)
			FRotator LeftRotation = InitialRotation;
			LeftRotation.Yaw += 180.0f;
			SetActorRotation(LeftRotation);
		}
	}
}

void AMG26_Pawn_03_SideScroller::Look(const FInputActionValue& Value)
{
	// Bỏ trống hàm này! Camera 2D Side-Scroller thì không cho phép người chơi tự xoay bằng chuột.
}
