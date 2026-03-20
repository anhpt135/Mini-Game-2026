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
	// Thiết lập các giá trị mặc định cho các thuộc tính của Actor này
	AMG26_Pickup();

protected:
	// Được gọi khi trò chơi bắt đầu hoặc khi Actor được sinh ra (spawned)
	virtual void BeginPlay() override;

	// Component hình cầu dùng để kiểm tra va chạm (Overlap)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* OverlapSphere;

	// Component hiển thị hình ảnh 3D (Mesh) của vật phẩm
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* PickupMesh;

	// Component đèn sẽ được tìm tự động (có thể là PointLight hoặc SpotLight)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	ULocalLightComponent* PickupLight;

	// Cấu trúc Timeline dùng để tạo hiệu ứng hoạt ảnh (animation) theo thời gian
	FTimeline PickupTimeline;

	// Đường cong (Curve) định nghĩa sự thay đổi kích thước (Scale) của vật phẩm
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UCurveFloat* ScaleCurve;

	// Đường cong (Curve) định nghĩa sự thay đổi độ sáng (Intensity) của đèn
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UCurveFloat* LightCurve;

	// Độ sáng tối đa của đèn khi được kích hoạt
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	float MaxLightIntensity;

	// Lưu trữ kích thước ban đầu của vật phẩm
	FVector InitialScale;

	// Khai báo hàm Callback khi có một Actor khác bắt đầu đi vào vùng Sphere (Overlap)
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Khai báo hàm Callback khi một Actor khác đi ra khỏi vùng Sphere
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Hàm được gọi mỗi khi Timeline cập nhật (chạy theo từng frame)
	UFUNCTION()
	void TimelineUpdate();

public:	
	// Được gọi mỗi frame (khung hình)
	virtual void Tick(float DeltaTime) override;

};
