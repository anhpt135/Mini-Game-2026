// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/MPC_ChangeColor.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Materials/MaterialParameterCollection.h"
#include "Framework/MG26_PawnBase.h"

// Thiết lập các giá trị mặc định cho Component
UMPC_ChangeColor::UMPC_ChangeColor()
{
	// Tắt Tick vì Component này hoạt động hoàn toàn dựa trên sự kiện (Event-driven) để tiết kiệm hiệu năng
	PrimaryComponentTick.bCanEverTick = false;
}

// Được gọi khi trò chơi bắt đầu
void UMPC_ChangeColor::BeginPlay()
{
	Super::BeginPlay();

	// Tự động tìm Actor đang chứa Component này và ép kiểu về PawnBase
	if (AMG26_PawnBase* MyPawn = Cast<AMG26_PawnBase>(GetOwner()))
	{
		// Cắm "tai nghe" vào 2 kênh sự kiện của Pawn để lắng nghe thay đổi trạng thái
		MyPawn->OnJumped.AddDynamic(this, &UMPC_ChangeColor::HandleOnPawnJumped);
		MyPawn->OnLanded.AddDynamic(this, &UMPC_ChangeColor::HandleOnPawnLanded);
	}
}

// Được gọi mỗi khung hình (Hiện đã bị tắt)
void UMPC_ChangeColor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// Hàm này sẽ tự động chạy ngay khi nhận được tín hiệu OnJumped từ Pawn
void UMPC_ChangeColor::HandleOnPawnJumped()
{
	// Áp dụng Alpha của trạng thái nhảy
	UpdateMPCAlpha(JumpAlpha);
}

// Hàm này sẽ tự động chạy ngay khi nhận được tín hiệu OnLanded từ Pawn
void UMPC_ChangeColor::HandleOnPawnLanded()
{
	// Trả về Alpha mặc định ban đầu
	UpdateMPCAlpha(DefaultAlpha);
}

// Hàm xử lý cấp thấp, trực tiếp thay đổi giá trị trong Material Parameter Collection
void UMPC_ChangeColor::UpdateMPCAlpha(float NewAlpha)
{
	if (MyMPC)
	{
		// Cập nhật giá trị Scalar Parameter dựa trên tên biến và giá trị được truyền vào
		UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), MyMPC, AlphaParameterName, NewAlpha);
	}
}
