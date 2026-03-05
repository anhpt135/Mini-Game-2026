// Điền thông báo bản quyền của bạn vào trang Mô tả của Cài đặt Dự án.

#pragma once

#include "CoreMinimal.h"
#include "MG26_TriggerButton.h"
#include "MG26_ChangeSky.generated.h"

class ADirectionalLight;

UCLASS()
class MINIGAME_2026_API AMG26_ChangeSky : public AMG26_TriggerButton
{
	GENERATED_BODY()

public:
	// Thiết lập các giá trị mặc định cho các thuộc tính của actor này
	AMG26_ChangeSky();

protected:
	// Được gọi khi trò chơi bắt đầu hoặc khi được sinh ra
	virtual void BeginPlay() override;

	// Override hàm khi bước vào vùng va chạm
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	// Hàm mới xử lý khi bước ra khỏi vùng va chạm
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Sự kiện để Blueprint thực hiện logic khi vào vùng (ví dụ: Timeline đổi màu)
	UFUNCTION(BlueprintImplementableEvent, Category = "Sky Control")
	void BP_OnEnterSkyZone();

	// Sự kiện để Blueprint thực hiện logic khi ra vùng
	UFUNCTION(BlueprintImplementableEvent, Category = "Sky Control")
	void BP_OnExitSkyZone();

public:
	// Được gọi mỗi khung hình
	virtual void Tick(float DeltaTime) override;

protected:
	// Tham chiếu đến đèn mặt trời (Directional Light) để đổi màu
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sky Control")
	ADirectionalLight* SunLight;

	// Màu sắc muốn đổi sang (Mặc định là Đỏ)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sky Control")
	FLinearColor TargetSkyColor = FLinearColor::Red;

	// Biến lưu màu gốc để khôi phục (Blueprint có thể đọc để quay về màu cũ)
	UPROPERTY(BlueprintReadOnly, Category = "Sky Control")
	FLinearColor OriginalColor;
};
