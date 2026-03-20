// Điền thông báo bản quyền của bạn trong trang Mô tả của Project Settings.


#include "Interaction/MG26_Pickup.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/LocalLightComponent.h"

// Thiết lập các giá trị mặc định
AMG26_Pickup::AMG26_Pickup()
{
 	// Thiết lập Actor này gọi hàm Tick() mỗi frame. Bạn có thể tắt nếu không cần để tối ưu hiệu suất.
	PrimaryActorTick.bCanEverTick = true;

	// Tạo Component Sphere để kiểm tra va chạm
	OverlapSphere = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapSphere"));
	RootComponent = OverlapSphere;

	// Tạo Component Static Mesh để hiển thị hình ảnh vật phẩm
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	PickupMesh->SetupAttachment(RootComponent);

	// Chúng ta không tạo cứng PointLight hay SpotLight trong C++ nữa.
	// Bạn sẽ chủ động thêm Component (PointLight hoặc SpotLight) trực tiếp vào Blueprint
	// C++ sẽ tự động tìm thấy cái đèn đó lúc BeginPlay()
	PickupLight = nullptr;

	// Đặt giá trị độ sáng tối đa mặc định
	MaxLightIntensity = 5000.f;
}

void AMG26_Pickup::BeginPlay()
{
	Super::BeginPlay();
	
	// Lưu lại kích thước ban đầu của Mesh để sử dụng cho animation
	InitialScale = PickupMesh->GetRelativeScale3D();

	// Tự động tìm Component đèn (LocalLightComponent - Cha của PointLight & SpotLight) có trong Actor
	PickupLight = Cast<ULocalLightComponent>(GetComponentByClass(ULocalLightComponent::StaticClass()));

	// Nếu tìm thấy đèn, tắt độ sáng lúc mới bắt đầu
	if (PickupLight)
	{
		PickupLight->SetIntensity(0.f);
	}

	// Thiết lập Timeline nếu các Curve (đường cong) đã được gán
	if (ScaleCurve && LightCurve)
	{
		FOnTimelineEvent TimelineUpdateDelegate;
		TimelineUpdateDelegate.BindUFunction(this, FName("TimelineUpdate"));

		// Thêm một giá trị float giả để Timeline có thể chạy và lấy độ dài thời gian từ ScaleCurve
		PickupTimeline.AddInterpFloat(ScaleCurve, FOnTimelineFloat()); 
		// Gán hàm cập nhật (TimelineUpdate) sẽ chạy mỗi khi Timeline thay đổi
		PickupTimeline.SetTimelinePostUpdateFunc(TimelineUpdateDelegate);
	}

	// Đăng ký các sự kiện Overlap (bắt đầu và kết thúc) với các hàm tương ứng
	OverlapSphere->OnComponentBeginOverlap.AddDynamic(this, &AMG26_Pickup::OnOverlapBegin);
	OverlapSphere->OnComponentEndOverlap.AddDynamic(this, &AMG26_Pickup::OnOverlapEnd);
}

void AMG26_Pickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Nếu Actor chạm vào là một Pawn (ví dụ: Nhân vật người chơi), thì phát Timeline tiến tới
	if (OtherActor && OtherActor != this && OtherActor->IsA(APawn::StaticClass()))
	{
		PickupTimeline.Play();
	}
}

void AMG26_Pickup::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Nếu Pawn đi ra khỏi vùng va chạm, thì phát Timeline lùi lại (để đảo ngược hiệu ứng)
	if (OtherActor && OtherActor != this && OtherActor->IsA(APawn::StaticClass()))
	{
		PickupTimeline.Reverse();
	}
}

void AMG26_Pickup::TimelineUpdate()
{
	// Lấy thời gian hiện tại của Timeline
	float CurrentPlaybackTime = PickupTimeline.GetPlaybackPosition();

	// Cập nhật kích thước (Scale) của Mesh dựa trên giá trị của ScaleCurve tại thời điểm hiện tại
	if (ScaleCurve)
	{
		float ScaleFactor = ScaleCurve->GetFloatValue(CurrentPlaybackTime);
		PickupMesh->SetRelativeScale3D(InitialScale * ScaleFactor);
	}

	// Cập nhật độ sáng của đèn dựa trên giá trị của LightCurve tại thời điểm hiện tại
	if (LightCurve && PickupLight)
	{
		float LightIntensity = LightCurve->GetFloatValue(CurrentPlaybackTime);
		PickupLight->SetIntensity(LightIntensity * MaxLightIntensity);
	}
}

void AMG26_Pickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Nếu Timeline đang phát (tiến hoặc lùi), tiến hành cập nhật Timeline theo thời gian
	if (PickupTimeline.IsPlaying() || PickupTimeline.IsReversing())
	{
		PickupTimeline.TickTimeline(DeltaTime);
	}
}
