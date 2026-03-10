#include "Interaction/MG26_ThapBanSet.h"
#include "Interaction/MG26_LightningHitInterface.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/Pawn.h"

// Thiết lập giá trị mặc định
AMG26_ThapBanSet::AMG26_ThapBanSet()
{
	// Thiết lập actor này gọi Tick() mỗi khung hình. Bạn có thể tắt điều này để cải thiện hiệu suất nếu không cần thiết.
	PrimaryActorTick.bCanEverTick = true;

	// Tạo Sphere Component và set làm root
	DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionSphere"));
	RootComponent = DetectionSphere;

	// Thiết lập các thuộc tính cho sphere
	DetectionSphere->SetSphereRadius(1000.f); // Tầm bắn mặc định 1000cm (10m)
	DetectionSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	// Tạo Niagara Component thứ 2 (Hiệu ứng trang trí, luôn chạy)
	HieuUngNiagara_TrangTri = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LightningComponent_02"));
	HieuUngNiagara_TrangTri->SetupAttachment(RootComponent);
	// Mặc định bAutoActivate là true, nên nó sẽ tự chạy khi game bắt đầu

	// Tạo Niagara Component chính (Tia sét tấn công)
	HieuUngNiagara_SetDanh = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LightningComponent"));
	HieuUngNiagara_SetDanh->SetupAttachment(RootComponent);
	HieuUngNiagara_SetDanh->bAutoActivate = false; // Mặc định tắt, chỉ bật khi có mục tiêu
}

// Được gọi khi trò chơi bắt đầu hoặc khi được sinh ra
void AMG26_ThapBanSet::BeginPlay()
{
	Super::BeginPlay();

	// Đảm bảo tắt hiệu ứng tấn công khi bắt đầu game
	if (HieuUngNiagara_SetDanh)
	{
		HieuUngNiagara_SetDanh->Deactivate();
	}

	// Gắn sự kiện overlap
	DetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &AMG26_ThapBanSet::OnOverlapBegin);
	DetectionSphere->OnComponentEndOverlap.AddDynamic(this, &AMG26_ThapBanSet::OnOverlapEnd);
}

void AMG26_ThapBanSet::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// Logic chỉ chạy trong Editor để Xem trước (Preview)
#if WITH_EDITOR
	if (HieuUngNiagara_SetDanh && HieuUngNiagara_SetDanh->GetAsset())
	{
		if (GetWorld() && !GetWorld()->IsGameWorld()) // Nếu có world và world đó không phải là Gameworld thì chạy code
		{
			// Bật hiệu ứng để xem trước
			HieuUngNiagara_SetDanh->Activate(true);
		}
	}
#endif
}

// Được gọi mỗi khung hình
void AMG26_ThapBanSet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Nếu có mục tiêu và hiệu ứng đang chạy, cập nhật vị trí mục tiêu cho Niagara
	if (MucTieu && HieuUngNiagara_SetDanh->IsActive())
	{
		FVector TargetLocation = MucTieu->GetActorLocation();
		HieuUngNiagara_SetDanh->SetVectorParameter(NiagaraParmPositionTarget, TargetLocation);
	}
}

void AMG26_ThapBanSet::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Chỉ tấn công Pawn (Character hoặc Pawn thường) và khi chưa có mục tiêu
	if (OtherActor && OtherActor->IsA(APawn::StaticClass()) && !MucTieu)
	{
		// Kiểm tra xem actor có phải là pawn do người chơi điều khiển không
		APawn* PlayerPawn = Cast<APawn>(OtherActor);
		if (PlayerPawn && PlayerPawn->IsPlayerControlled())
		{
			MucTieu = PlayerPawn;

			// Bật hiệu ứng Niagara
			if (HieuUngNiagara_SetDanh)
			{
				HieuUngNiagara_SetDanh->Activate(true);
			}

			// Kiểm tra xem mục tiêu có implement Interface không
			if (MucTieu->Implements<UMG26_LightningHitInterface>())
			{
				// Gọi hàm OnLightningHit thông qua Interface
				IMG26_LightningHitInterface::Execute_OnLightningHit(MucTieu);
			}
		}
	}
}

void AMG26_ThapBanSet::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Nếu actor rời đi chính là mục tiêu hiện tại
	if (OtherActor && OtherActor == MucTieu)
	{
		// Tắt hiệu ứng Niagara
		if (HieuUngNiagara_SetDanh)
		{
			HieuUngNiagara_SetDanh->Deactivate();
		}

		// Kiểm tra xem mục tiêu có implement Interface không
		if (MucTieu && MucTieu->Implements<UMG26_LightningHitInterface>())
		{
			// Gọi hàm OnLightningStop thông qua Interface
			IMG26_LightningHitInterface::Execute_OnLightningStop(MucTieu);
		}

		// Xóa tham chiếu đến mục tiêu
		MucTieu = nullptr;
	}
}
