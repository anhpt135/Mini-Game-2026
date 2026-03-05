// Điền thông báo bản quyền của bạn vào trang Mô tả của Cài đặt Dự án.


#include "Interaction/MG26_ChangeSky.h"
#include "Engine/DirectionalLight.h"
#include "Components/LightComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Thiết lập các giá trị mặc định
AMG26_ChangeSky::AMG26_ChangeSky()
{
	// Đặt actor này gọi Tick() mỗi khung hình. Bạn có thể tắt tính năng này để cải thiện hiệu suất nếu không cần.
	PrimaryActorTick.bCanEverTick = true;
}

// Được gọi khi trò chơi bắt đầu hoặc khi được sinh ra
void AMG26_ChangeSky::BeginPlay()
{
	Super::BeginPlay();

	// Nếu chưa gán đèn trong Editor, tự tìm đèn DirectionalLight đầu tiên trong level
	if (!SunLight)
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADirectionalLight::StaticClass(), FoundActors);
		if (FoundActors.Num() > 0)
		{
			SunLight = Cast<ADirectionalLight>(FoundActors[0]);
		}
	}

	// Lưu màu gốc nếu tìm thấy đèn
	if (SunLight && SunLight->GetLightComponent())
	{
		OriginalColor = SunLight->GetLightComponent()->GetLightColor();
	}

	// Đăng ký sự kiện khi rời khỏi vùng va chạm
	if (TriggerBox)
	{
		TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AMG26_ChangeSky::OnOverlapEnd);
	}
}

// Được gọi mỗi khung hình
void AMG26_ChangeSky::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMG26_ChangeSky::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Gọi logic của cha trước (nếu cần)
	Super::OnOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	// Kiểm tra nếu là Pawn (người chơi)
	if (OtherActor && OtherActor != this && Cast<APawn>(OtherActor))
	{
		if (SunLight && SunLight->GetLightComponent())
		{
			// Đổi màu đèn sang màu mục tiêu (Đỏ)
			SunLight->GetLightComponent()->SetLightColor(TargetSkyColor);
		}
	}
}

void AMG26_ChangeSky::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Kiểm tra nếu là Pawn (người chơi)
	if (OtherActor && OtherActor != this && Cast<APawn>(OtherActor))
	{
		if (SunLight && SunLight->GetLightComponent())
		{
			// Trả lại màu gốc
			SunLight->GetLightComponent()->SetLightColor(OriginalColor);
		}
	}
}
