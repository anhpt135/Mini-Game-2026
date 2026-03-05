// Điền thông báo bản quyền của bạn vào trang Mô tả của Cài đặt Dự án.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MG26_TriggerButton.generated.h"

class UBoxComponent;

// Khai báo sự kiện: Truyền ra Pawn xe mục tiêu
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTriggerActivated, APawn*, VehicleToPossess);

UCLASS()
class MINIGAME_2026_API AMG26_TriggerButton : public AActor
{
	GENERATED_BODY()
	
public:	
	// Thiết lập các giá trị mặc định cho các thuộc tính của actor này
	AMG26_TriggerButton();

	// Sự kiện để Blueprint có thể "kéo dây" vào
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnTriggerActivated OnTriggerActivated;

protected:
	// Được gọi khi trò chơi bắt đầu hoặc khi được sinh ra
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* TriggerBox;

	// Pawn để chiếm quyền điều khiển khi được kích hoạt
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	APawn* TargetPawnToPossess;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Được gọi mỗi khung hình
	virtual void Tick(float DeltaTime) override;

};
