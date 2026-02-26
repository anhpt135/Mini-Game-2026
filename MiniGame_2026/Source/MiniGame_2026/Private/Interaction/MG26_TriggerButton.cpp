// Điền thông báo bản quyền của bạn vào trang Mô tả của Cài đặt Dự án.


#include "Interaction/MG26_TriggerButton.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Pawn.h"
#include "Framework/MG26_PlayerControllerBase.h"

// Thiết lập các giá trị mặc định
AMG26_TriggerButton::AMG26_TriggerButton()
{
 	// Đặt actor này gọi Tick() mỗi khung hình. Bạn có thể tắt tính năng này để cải thiện hiệu suất nếu không cần.
	PrimaryActorTick.bCanEverTick = true;

	// Tạo hộp kích hoạt (trigger box)
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;

	// Tạo thành phần lưới (mesh component)
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	// Liên kết sự kiện chồng lấp (overlap event)
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AMG26_TriggerButton::OnOverlapBegin);
}

// Được gọi khi trò chơi bắt đầu hoặc khi được sinh ra
void AMG26_TriggerButton::BeginPlay()
{
	Super::BeginPlay();
	
}

// Được gọi mỗi khung hình
void AMG26_TriggerButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMG26_TriggerButton::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		// Kiểm tra xem actor chồng lấp có phải là một pawn không
		APawn* OverlappingPawn = Cast<APawn>(OtherActor);
		if (OverlappingPawn)
		{
			// Lấy bộ điều khiển người chơi (player controller)
			AMG26_PlayerControllerBase* PlayerController = Cast<AMG26_PlayerControllerBase>(OverlappingPawn->GetController());
			if (PlayerController && PlayerController->bCanTriggerPossess)
			{
				// Chuyển sang pawn xe
				if (TargetPawnToPossess)
				{
					PlayerController->SwitchToVehicle(TargetPawnToPossess, this);
				}
			}
		}
	}
}
