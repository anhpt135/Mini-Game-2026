// Điền thông báo bản quyền của bạn vào trang Mô tả của Cài đặt Dự án.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "MG26_PawnBase.generated.h"

class UInputMappingContext;
class UInputAction;
class UCameraComponent;
class USpringArmComponent;
class UCapsuleComponent;
class UFloatingPawnMovement;

UCLASS()
class MINIGAME_2026_API AMG26_PawnBase : public APawn
{
	GENERATED_BODY()

public:
	// Thiết lập các giá trị mặc định cho các thuộc tính của pawn này
	AMG26_PawnBase();

protected:
	// Được gọi khi trò chơi bắt đầu hoặc khi được sinh ra
	virtual void BeginPlay() override;

public:
	// Được gọi mỗi khung hình
	virtual void Tick(float DeltaTime) override;

	// Được gọi để liên kết chức năng với đầu vào
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	/** Các component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UFloatingPawnMovement* MovementComponent;

	/** Bối cảnh ánh xạ đầu vào */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	/** Hành động đầu vào Nhảy */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* JumpAction;

	/** Hành động đầu vào Di chuyển */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;

	/** Hành động đầu vào Nhìn */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;

	/** Cài đặt trọng lực và nhảy */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float JumpVelocity = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float GravityStrength = 980.0f;

private:
	float VerticalVelocity;
	bool bIsGrounded;

protected:
	/** Được gọi cho đầu vào di chuyển */
	virtual void Move(const FInputActionValue& Value);

	/** Được gọi cho đầu vào nhìn */
	virtual void Look(const FInputActionValue& Value);

	/** Được gọi cho đầu vào nhảy */
	void Jump(const FInputActionValue& Value);
};
