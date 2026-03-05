// Điền thông báo bản quyền của bạn vào trang Mô tả của Cài đặt Dự án.


#include "Interaction/MG26_WeaponSpawner.h"
#include "NiagaraFunctionLibrary.h"


// Thiết lập các giá trị mặc định
AMG26_WeaponSpawner::AMG26_WeaponSpawner()
{
	// Thiết lập actor này để gọi Tick() mỗi khung hình. Bạn có thể tắt tính năng này để cải thiện hiệu suất nếu không cần.
	PrimaryActorTick.bCanEverTick = true;
}

// Được gọi khi trò chơi bắt đầu hoặc khi được sinh ra
void AMG26_WeaponSpawner::BeginPlay()
{
	Super::BeginPlay();
}

// Được gọi mỗi khung hình
void AMG26_WeaponSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMG26_WeaponSpawner::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this))
	{
		APawn* OverlappingPawn = Cast<APawn>(OtherActor);
		if (OverlappingPawn)
		{
			// Gọi sự kiện Blueprint để xử lý hiệu ứng
			OnWeaponPickedUp(OverlappingPawn);
			
			// Gọi logic của lớp cơ sở để kích hoạt sự kiện/chiếm hữu
			Super::OnOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
			
			// Dùng lệnh dưới nếu muốn Các điểm sinh vũ khí thường biến mất hoặc bị vô hiệu hóa sau khi sử dụng
			//Destroy();
		}
	}
}
