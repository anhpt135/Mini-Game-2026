#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MG26_LightningHitInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UMG26_LightningHitInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface cho các actor có thể bị tháp sét bắn trúng
 */
class MINIGAME_2026_API IMG26_LightningHitInterface
{
	GENERATED_BODY()

public:
	/** 
	 * Sự kiện khi bị sét đánh trúng.
	 * Implement trong Blueprint hoặc C++ của đối tượng bị bắn.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Lightning Interaction")
	void OnLightningHit();

	/** 
	 * Sự kiện khi sét ngừng đánh.
	 * Implement trong Blueprint hoặc C++ của đối tượng bị bắn.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Lightning Interaction")
	void OnLightningStop();
};
