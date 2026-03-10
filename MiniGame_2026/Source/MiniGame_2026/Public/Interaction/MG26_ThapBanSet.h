#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MG26_ThapBanSet.generated.h"

class USphereComponent;
class UNiagaraComponent;

UCLASS()
class MINIGAME_2026_API AMG26_ThapBanSet : public AActor
{
	GENERATED_BODY()

public:
	// Thiết lập giá trị mặc định cho các thuộc tính của actor này
	AMG26_ThapBanSet();

protected:
	// Được gọi khi trò chơi bắt đầu hoặc khi được sinh ra
	virtual void BeginPlay() override;

	// Được gọi khi Actor được tạo hoặc thay đổi trong Editor
	virtual void OnConstruction(const FTransform& Transform) override;

public:
	// Được gọi mỗi khung hình
	virtual void Tick(float DeltaTime) override;

protected:
	/** Vùng phát hiện mục tiêu */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USphereComponent> DetectionSphere;

	/** Component Niagara để hiển thị tia sét. Gán Asset vào đây trong Blueprint. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UNiagaraComponent> HieuUngNiagara_SetDanh;
	
	/** Component Niagara thứ 2 để hiển thị hiệu ứng trang trí (luôn chạy). */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UNiagaraComponent> HieuUngNiagara_TrangTri;

	/** Tên tham số vector trong Niagara để set vị trí mục tiêu (User Parameter) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	FName NiagaraParmPositionTarget = FName("NiagaraParm");

	/** Mục tiêu hiện tại đang bị tấn công */
	UPROPERTY()
	TObjectPtr<AActor> MucTieu;

	/** Hàm xử lý khi có Actor đi vào vùng ảnh hưởng */
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Hàm xử lý khi có Actor rời khỏi vùng ảnh hưởng */
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
