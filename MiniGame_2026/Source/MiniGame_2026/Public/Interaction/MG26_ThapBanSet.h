// Fill out your copyright notice in the Description page of Project Settings.

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
	// Sets default values for this actor's properties
	AMG26_ThapBanSet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Được gọi khi Actor được tạo hoặc thay đổi trong Editor
	virtual void OnConstruction(const FTransform& Transform) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	/** Vùng phát hiện mục tiêu */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* DetectionSphere;

	/** Component Niagara để hiển thị tia sét. Gán Asset vào đây trong Blueprint. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UNiagaraComponent* LightningComponent;

	/** Tên tham số vector trong Niagara để set vị trí mục tiêu (User Parameter) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	FName LightningTargetParamName = FName("User.TargetLocation");

	/** Mục tiêu hiện tại đang bị tấn công */
	UPROPERTY()
	AActor* TargetActor;

	/** Hàm xử lý khi có Actor đi vào vùng ảnh hưởng */
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Hàm xử lý khi có Actor rời khỏi vùng ảnh hưởng */
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
