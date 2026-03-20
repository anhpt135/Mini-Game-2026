// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "MG26_Pickup.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class ULocalLightComponent;
class UCurveFloat;

UCLASS()
class MINIGAME_2026_API AMG26_Pickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMG26_Pickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* OverlapSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* PickupMesh;

	// Component đèn sẽ được tìm tự động (có thể là PointLight hoặc SpotLight)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	ULocalLightComponent* PickupLight;

	// Cấu trúc Timeline
	FTimeline PickupTimeline;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UCurveFloat* ScaleCurve;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UCurveFloat* LightCurve;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	float MaxLightIntensity;

	FVector InitialScale;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void TimelineUpdate();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
