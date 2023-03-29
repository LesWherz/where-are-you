#pragma once

#include "CoreMinimal.h"
#include "UE_project/InteractableObjects/IInteractableObject.h"
#include "UObject/Interface.h"
#include "IPickableObject.generated.h"


UINTERFACE(BlueprintType, Category="WRU/Interactions")
class UIPickableObject : public UInterface
{
	GENERATED_BODY()
};

/** 
 * 
 */
class UE_PROJECT_API IIPickableObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="WRU/Interactions")
	bool IsPickedUp();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="WRU/Interactions")
	AActor* GetTargetActor();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="WRU/Interactions")
	void OnPickedUp(AActor* playerHowPickup);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="WRU/Interactions")
	void OnInteracted(const TScriptInterface<IIInteractableObject>& interactableObject);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="WRU/Interactions")
	void OnDropped(AActor* playerHowDrop, bool fromInteraction);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="WRU/Interactions")
	void DropMe();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="WRU/Interactions")	
	TScriptInterface<IIPickableObject> GetIPickableObject();
};
