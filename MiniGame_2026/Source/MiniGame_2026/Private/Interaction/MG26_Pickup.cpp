// Điền thông báo bản quyền của bạn trong trang Mô tả của Project Settings.


#include "Interaction/MG26_Pickup.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/LocalLightComponent.h"

AMG26_Pickup::AMG26_Pickup()
{
	PrimaryActorTick.bCanEverTick = true;

	OverlapSphere = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapSphere"));
	RootComponent = OverlapSphere;

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	PickupMesh->SetupAttachment(RootComponent);

	PickupLight = nullptr;

	MaxLightIntensity = 5000.f;
}

void AMG26_Pickup::BeginPlay()
{
	Super::BeginPlay();
	
	InitialScale = PickupMesh->GetRelativeScale3D();

	// Tự động quét và lấy Component Đèn (Con của LocalLightComponent)
	PickupLight = Cast<ULocalLightComponent>(GetComponentByClass(ULocalLightComponent::StaticClass()));

	if (PickupLight)
	{
		PickupLight->SetIntensity(0.f);
	}

	if (ScaleCurve && LightCurve)
	{
		FOnTimelineEvent TimelineUpdateDelegate;
		TimelineUpdateDelegate.BindUFunction(this, FName("TimelineUpdate"));
		
		// Thêm float giả để Timeline biết thời gian cần chạy
		PickupTimeline.AddInterpFloat(ScaleCurve, FOnTimelineFloat()); 
		// Đăng ký gọi hàm Update liên tục mỗi frame khi Timeline chạy
		PickupTimeline.SetTimelinePostUpdateFunc(TimelineUpdateDelegate);
	}

	// Đăng ký sự kiện va chạm
	OverlapSphere->OnComponentBeginOverlap.AddDynamic(this, &AMG26_Pickup::OnOverlapBegin);
	OverlapSphere->OnComponentEndOverlap.AddDynamic(this, &AMG26_Pickup::OnOverlapEnd);
}

void AMG26_Pickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Nếu Pawn đi vào
	if (OtherActor && OtherActor != this && OtherActor->IsA(APawn::StaticClass()))
	{
		// Hủy lệnh hẹn giờ lùi lại (nếu có) phòng trường hợp người chơi đi ra rồi đi vào quá nhanh
		GetWorld()->GetTimerManager().ClearTimer(ReverseTimerHandle);
		
		// Phát Timeline theo chiều tiến
		PickupTimeline.Play();
	}
}

void AMG26_Pickup::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Nếu Pawn đi ra
	if (OtherActor && OtherActor != this && OtherActor->IsA(APawn::StaticClass()))
	{
		// Dùng Event: Tắt đèn ngay lập tức chỉ 1 lần duy nhất để tối ưu hiệu năng
		if (PickupLight)
		{
			PickupLight->SetIntensity(0.f);
		}

		// Hẹn giờ 0.5 giây sau mới chạy Timeline lùi để thu nhỏ Mesh
		GetWorld()->GetTimerManager().SetTimer(ReverseTimerHandle, this, &AMG26_Pickup::ReverseTimeline, 0.25f, false);
	}
}

void AMG26_Pickup::ReverseTimeline()
{
	PickupTimeline.Reverse();
}

void AMG26_Pickup::TimelineUpdate()
{
	float CurrentPlaybackTime = PickupTimeline.GetPlaybackPosition();

	if (ScaleCurve)
	{
		float ScaleFactor = ScaleCurve->GetFloatValue(CurrentPlaybackTime);
		PickupMesh->SetRelativeScale3D(InitialScale * ScaleFactor);
	}

	if (LightCurve && PickupLight)
	{
		// Chỉ lấy dữ liệu từ Curve để cập nhật đèn khi Timeline đang chạy tiến.
		// Bỏ qua hoàn toàn việc Update đèn khi chạy lùi (vì đèn đã được tắt cứng ở OnOverlapEnd).
		if (!PickupTimeline.IsReversing())
		{
			float LightIntensity = LightCurve->GetFloatValue(CurrentPlaybackTime);
			PickupLight->SetIntensity(LightIntensity * MaxLightIntensity);
		}
	}
}

void AMG26_Pickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Gọi TickTimeline để Timeline có thể chạy và kích hoạt Delegate
	if (PickupTimeline.IsPlaying() || PickupTimeline.IsReversing())
	{
		PickupTimeline.TickTimeline(DeltaTime);
	}
}
