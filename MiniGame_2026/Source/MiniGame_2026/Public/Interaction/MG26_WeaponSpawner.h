// Điền thông báo bản quyền của bạn vào trang Mô tả của Cài đặt Dự án.

#pragma once

#include "CoreMinimal.h"
#include "MG26_TriggerButton.h"
#include "MG26_WeaponSpawner.generated.h"

class UNiagaraSystem;

UCLASS()
class MINIGAME_2026_API AMG26_WeaponSpawner : public AMG26_TriggerButton
{
	GENERATED_BODY()

public:
	// Thiết lập các giá trị mặc định cho các thuộc tính của actor này
	AMG26_WeaponSpawner();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	TObjectPtr<UNiagaraSystem> HieuUngKhiLumVuKhi;

protected:
	// Được gọi khi trò chơi bắt đầu hoặc khi được sinh ra
	virtual void BeginPlay() override;

	// Ghi đè hàm OnOverlapBegin từ lớp cha
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

public:
	// Được gọi mỗi khung hình
	virtual void Tick(float DeltaTime) override;
};
