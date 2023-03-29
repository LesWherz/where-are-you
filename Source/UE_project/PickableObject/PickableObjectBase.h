// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IPickableObject.h"
#include "GameFramework/Actor.h"
#include "PickableObjectBase.generated.h"

UCLASS(BlueprintType)
class UE_PROJECT_API APickableObjectBase : public AActor, public IIPickableObject
{
	GENERATED_BODY()
	
public:
	APickableObjectBase();
	
public:
	virtual bool IsPickedUp_Implementation() override										{ return m_isPickedUp; }
	virtual AActor* GetTargetActor_Implementation() override								{ return this; }
	virtual TScriptInterface<IIPickableObject> GetIPickableObject_Implementation() override { return this; };

	virtual void OnPickedUp_Implementation(AActor* playerHowPickup) override;
	virtual void OnInteracted_Implementation(const TScriptInterface<IIInteractableObject>& interactableObject) override;
	virtual void OnDropped_Implementation(AActor* playerHowDrop, bool fromInteraction) override;
	virtual void DropMe_Implementation() override;
	
	UPROPERTY(BlueprintReadWrite)
	bool m_isPickedUp = false;

	UPROPERTY(BlueprintReadWrite)
	AActor* m_actorHowPickedMeUp = nullptr;
};