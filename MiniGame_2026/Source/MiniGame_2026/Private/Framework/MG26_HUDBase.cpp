// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/MG26_HUDBase.h"
#include "Blueprint/UserWidget.h"

void AMG26_HUDBase::BeginPlay()
{
	Super::BeginPlay();

	if (MainWidgetClass)
	{
		MainWidget = CreateWidget<UUserWidget>(GetWorld(), MainWidgetClass);
		if (MainWidget)
		{
			MainWidget->AddToViewport();
		}
	}
}
