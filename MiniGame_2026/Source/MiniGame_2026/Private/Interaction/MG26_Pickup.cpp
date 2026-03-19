// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/MG26_Pickup.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PointLightComponent.h"

// Sets default values
AMG26_Pickup::AMG26_Pickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OverlapSphere = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapSphere"));
	RootComponent = OverlapSphere;
	// Ignore Camera Collision
	OverlapSphere->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	PickupMesh->SetupAttachment(RootComponent);
	// Ignore Camera Collision
	PickupMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
	PointLight->SetupAttachment(RootComponent);

	MaxLightIntensity = 5000.f;
}

// Called when the game starts or when spawned
void AMG26_Pickup::BeginPlay()
{
	Super::BeginPlay();
	
	InitialScale = PickupMesh->GetRelativeScale3D();
	PointLight->SetIntensity(0.f);

	if (ScaleCurve && LightCurve)
	{
		FOnTimelineEvent TimelineUpdateDelegate;
		TimelineUpdateDelegate.BindUFunction(this, FName("TimelineUpdate"));

		PickupTimeline.AddInterpFloat(ScaleCurve, FOnTimelineFloat()); // Dummy float interp just to drive length
		PickupTimeline.SetTimelinePostUpdateFunc(TimelineUpdateDelegate);
	}

	OverlapSphere->OnComponentBeginOverlap.AddDynamic(this, &AMG26_Pickup::OnOverlapBegin);
	OverlapSphere->OnComponentEndOverlap.AddDynamic(this, &AMG26_Pickup::OnOverlapEnd);
}

void AMG26_Pickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && OtherActor->IsA(APawn::StaticClass()))
	{
		PickupTimeline.Play();
	}
}

void AMG26_Pickup::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this && OtherActor->IsA(APawn::StaticClass()))
	{
		PickupTimeline.Reverse();
	}
}

void AMG26_Pickup::TimelineUpdate()
{
	float CurrentPlaybackTime = PickupTimeline.GetPlaybackPosition();

	if (ScaleCurve)
	{
		float ScaleFactor = ScaleCurve->GetFloatValue(CurrentPlaybackTime);
		PickupMesh->SetRelativeScale3D(InitialScale * ScaleFactor);
	}

	if (LightCurve)
	{
		float LightIntensity = LightCurve->GetFloatValue(CurrentPlaybackTime);
		PointLight->SetIntensity(LightIntensity * MaxLightIntensity);
	}
}

// Called every frame
void AMG26_Pickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PickupTimeline.IsPlaying() || PickupTimeline.IsReversing())
	{
		PickupTimeline.TickTimeline(DeltaTime);
	}
}
