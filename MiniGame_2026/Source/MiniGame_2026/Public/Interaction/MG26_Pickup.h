// Điền thông báo bản quyền của bạn trong trang Mô tả của Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "MG26_Pickup.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class ULocalLightComponent;
class UCurveFloat;

UCLASS()
class MINIGAME_2026_API AMG26_Pickup : public AActor
{
	GENERATED_BODY()
	
public:	
	AMG26_Pickup();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* OverlapSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* PickupMesh;

	// Đèn sẽ được code tự động tìm (PointLight hoặc SpotLight)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	ULocalLightComponent* PickupLight;

	// Timeline điều khiển hiệu ứng hoạt ảnh
	FTimeline PickupTimeline;

	// Curve điều khiển Scale
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UCurveFloat* ScaleCurve;

	// Curve điều khiển độ sáng của đèn
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UCurveFloat* LightCurve;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	float MaxLightIntensity;

	FVector InitialScale;

	// Bộ đếm thời gian (Timer) dùng để trì hoãn việc Reverse
	FTimerHandle ReverseTimerHandle;

	// Xử lý sự kiện khi có Actor đi vào/ra vùng Overlap
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Hàm Delegate được gọi mỗi frame khi Timeline đang chạy
	UFUNCTION()
	void TimelineUpdate();

	// Hàm được gọi sau khi hết thời gian Delay (0.5s)
	UFUNCTION()
	void ReverseTimeline();

public:	
	virtual void Tick(float DeltaTime) override;

};
