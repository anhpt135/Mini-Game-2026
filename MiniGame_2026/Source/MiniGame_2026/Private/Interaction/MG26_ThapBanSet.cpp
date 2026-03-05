// Fill out your copyright notice in the Description page of Project Settings.

#include "Interaction/MG26_ThapBanSet.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"

// Sets default values
AMG26_ThapBanSet::AMG26_ThapBanSet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Tạo Sphere Component và set làm root
	DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionSphere"));
	RootComponent = DetectionSphere;

	// Thiết lập các thuộc tính cho sphere
	DetectionSphere->SetSphereRadius(1000.f); // Tầm bắn mặc định 1000cm (10m)
	DetectionSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	// Tạo Niagara Component
	LightningComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LightningComponent"));
	LightningComponent->SetupAttachment(RootComponent);
	LightningComponent->bAutoActivate = false; // Mặc định tắt, chỉ bật khi có mục tiêu
}

// Called when the game starts or when spawned
void AMG26_ThapBanSet::BeginPlay()
{
	Super::BeginPlay();

	// Đảm bảo tắt hiệu ứng khi bắt đầu game
	if (LightningComponent)
	{
		LightningComponent->Deactivate();
	}

	// Bind sự kiện overlap
	DetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &AMG26_ThapBanSet::OnOverlapBegin);
	DetectionSphere->OnComponentEndOverlap.AddDynamic(this, &AMG26_ThapBanSet::OnOverlapEnd);
}

void AMG26_ThapBanSet::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// Logic chỉ chạy trong Editor để Preview
#if WITH_EDITOR
	if (LightningComponent && LightningComponent->GetAsset())
	{
		// Nếu không phải đang chạy game (PIE/Standalone)
		if (!GetWorld() || !GetWorld()->IsGameWorld())
		{
			// Bật hiệu ứng để xem trước
			LightningComponent->Activate(true);

			// Đặt một vị trí mục tiêu giả định (cách tháp 500 đơn vị về phía trước)
			FVector PreviewTargetLocation = GetActorLocation() + GetActorForwardVector() * 500.f;
			LightningComponent->SetVectorParameter(LightningTargetParamName, PreviewTargetLocation);
		}
	}
#endif
}

// Called every frame
void AMG26_ThapBanSet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Nếu có mục tiêu và hiệu ứng đang chạy, cập nhật vị trí mục tiêu cho Niagara
	if (TargetActor && LightningComponent->IsActive())
	{
		FVector TargetLocation = TargetActor->GetActorLocation();
		LightningComponent->SetVectorParameter(LightningTargetParamName, TargetLocation);
	}
}

void AMG26_ThapBanSet::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Chỉ tấn công Pawn (Character hoặc Pawn thường) và khi chưa có mục tiêu
	if (OtherActor && OtherActor->IsA(APawn::StaticClass()) && !TargetActor)
	{
		// Kiểm tra xem actor có phải là pawn do người chơi điều khiển không
		APawn* PlayerPawn = Cast<APawn>(OtherActor);
		if (PlayerPawn && PlayerPawn->IsPlayerControlled())
		{
			TargetActor = PlayerPawn;

			// Bật hiệu ứng Niagara
			if (LightningComponent)
			{
				LightningComponent->Activate(true);
			}
		}
	}
}

void AMG26_ThapBanSet::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Nếu actor rời đi chính là mục tiêu hiện tại
	if (OtherActor && OtherActor == TargetActor)
	{
		// Tắt hiệu ứng Niagara
		if (LightningComponent)
		{
			LightningComponent->Deactivate();
		}

		// Xóa tham chiếu đến mục tiêu
		TargetActor = nullptr;
	}
}
