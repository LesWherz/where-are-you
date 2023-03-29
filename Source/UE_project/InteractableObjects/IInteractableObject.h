#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IInteractableObject.generated.h"

UINTERFACE(BlueprintType, Category="WRU/Interactions")
class UIInteractableObject : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UE_PROJECT_API IIInteractableObject
{
	GENERATED_BODY()
public:
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="WRU/Interactions")
	void OnBeginInteract(AActor* interactionEmitter);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="WRU/Interactions")
	void OnEndInteract(AActor* interactionEmitter);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="WRU/Interactions")	
	TScriptInterface<IIInteractableObject> GetIInteractableObject();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="WRU/Interactions")
	AActor* GetTargetActor();
};
