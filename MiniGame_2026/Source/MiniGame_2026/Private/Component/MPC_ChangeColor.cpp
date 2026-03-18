// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/MPC_ChangeColor.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Materials/MaterialParameterCollection.h"
#include "Framework/MG26_PawnBase.h"
#include "Curves/CurveFloat.h"

UMPC_ChangeColor::UMPC_ChangeColor()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMPC_ChangeColor::BeginPlay()
{
	Super::BeginPlay();

	// Đăng ký hàm callback cho biến Delegate OnJumped/Landed
	if (AMG26_PawnBase* MyPawn = Cast<AMG26_PawnBase>(GetOwner()))
	{
		MyPawn->OnJumped.AddDynamic(this, &UMPC_ChangeColor::HamCallbackNhayLen);
		MyPawn->OnLanded.AddDynamic(this, &UMPC_ChangeColor::HamCallbackChamDat);
	}

	// Điều kiện cần số 1 của Timeline: Liên kết biểu đồ CurveFloat với hàm HamCallbackTimeline
	if (CurveCuaTui)
	{
		FOnTimelineFloat TimelineProgressDelegate;
		TimelineProgressDelegate.BindDynamic(this, &UMPC_ChangeColor::HamCallbackTimeline);
		TimelineCuaTui.AddInterpFloat(CurveCuaTui, TimelineProgressDelegate);
	}
}

void UMPC_ChangeColor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// Điều kiện cần số 2 của Timeline
	TimelineCuaTui.TickTimeline(DeltaTime);
}
	// Điều kiện cần số 3 của Timeline: Khi Play hoặc Reverse....

// Hàm callback của MPC_CHangeColor kích hoạt khi nhân vật nhảy lên
void UMPC_ChangeColor::HamCallbackNhayLen()
{
	if (CurveCuaTui)
	{
		TimelineCuaTui.Play();
	}
}

// Hàm callback của MPC_CHangeColor kích hoạt khi nhân vật chạm đất
void UMPC_ChangeColor::HamCallbackChamDat()
{
	if (CurveCuaTui)
	{
		TimelineCuaTui.Reverse();
	}
}

// Hàm callback từ Timeline chuyển tham số sang cho hàm ThayDoiGiaTriMPC
void UMPC_ChangeColor::HamCallbackTimeline(float Value)
{
	ThayDoiGiaTriMPC(Value);
}

// Khai báo logic của hàm ThayDoiGiaTriMPC
void UMPC_ChangeColor::ThayDoiGiaTriMPC(float NewAlpha)
{
	if (MPCCuaTui)
	{
		UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), MPCCuaTui, TenThongSoMPC, NewAlpha);
	}
}
