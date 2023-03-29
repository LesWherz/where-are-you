#pragma once

#include "CoreMinimal.h"
#include "IInteractableObject.h"
#include "GameFramework/Actor.h"
#include "InteractableObjectBase.generated.h"

UCLASS(Blueprintable, Category="Project")
class UE_PROJECT_API AInteractableObjectBase : public AActor, public IIInteractableObject
{
	GENERATED_BODY()

public:
	AInteractableObjectBase();

public:
	virtual void OnBeginInteract_Implementation(AActor* interactionEmitter) override;
	virtual void OnEndInteract_Implementation(AActor* interactionEmitter) override;
	
	virtual TScriptInterface<IIInteractableObject> GetIInteractableObject_Implementation() override { return this; }
	virtual AActor* GetTargetActor_Implementation() override { return this; }
};