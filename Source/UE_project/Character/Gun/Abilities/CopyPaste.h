#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../PlayerCharacter.h"
#include "UE_project/Character/ACResizePlayer.h"
#include "UE_project/Character/EPlayerSizeState.h"
#include "UE_project/ResizableObjects/EObjectSizeState.h"
#include "UE_project/ResizableObjects/ResizableObjectBase.h"
#include "CopyPaste.generated.h"

using KeySize = FIntVector;

USTRUCT()
struct UE_PROJECT_API FPasteSize
{
	GENERATED_BODY()

	UPROPERTY()
	EObjectSizeState _state;

	UPROPERTY()
	float _factor;
		
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_PROJECT_API UCopyPaste : public UActorComponent
{
	GENERATED_BODY()

	friend class APlayerCharacter;

public:	
	// Sets default values for this component's properties
	UCopyPaste();

private:
	FVector GetStartLineTrace() { return PlayerOwner->GetCamera()->GetComponentLocation(); }
	FVector GetEndLineTraceCopy() { return PlayerOwner->GetCamera()->GetComponentLocation() + PlayerOwner->GetCamera()->GetForwardVector() * TNumericLimits<int>::Max(); }
	FVector GetEndLineTracePaste() { return PlayerOwner->GetCamera()->GetComponentLocation() + PlayerOwner->GetCamera()->GetForwardVector() * ShootDistance; }

	AResizableObject* GetObjectPasteByClass(AResizableObject* actor) { return *ObjectsPaste.Find(actor); }
	FVector GetPositionPhantom();
	float CalculShootDistance(float inputAxis) { return ShootDistance + inputAxis * SpeedScrolling; }
	EPlayerSizeState GetPlayerSize() { return PlayerOwner->GetACResizePlayer()->GetPlayerSizeState(); }
	EObjectSizeState GetObjectCopySize() { return CopyObject->GetSizeState(); }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void PressedAction();

	UFUNCTION()
	void ReleasedAction();
	void MouseScrollAction(float axisValue);
	void ConfigCopy(AResizableObject* CopyObject, EPlayerSizeState size, bool copyAvailable);

private:
	void SaveAndDeletePasteObject(AResizableObject* Parent, AResizableObject* ToAdd);
	void AddObjectPaste(AResizableObject* Parent, AResizableObject* ToAdd) { ObjectsPaste[Parent] = ToAdd; }
	void CreatePhantomObject(UClass* ObjectClass, FTransform SpawnTransform);
	void CustomPhantomObject(UPrimitiveComponent* Actor);
	bool PhantomIsActive() { return !CopyAvailable && ObjectPhantomIsVisible; }
	void ResizeObjectAccordingOnPlayerSize(AResizableObject* ObjectToResize, EPlayerSizeState PlayerSootSize, EObjectSizeState BaseResizableState);
	AResizableObject* GetParent() const;
	

private:
	APlayerCharacter* PlayerOwner;
	bool CopyAvailable = true;
	bool ObjectPhantomIsVisible = false;
	EPlayerSizeState SizePlayerCopyObject;

	AResizableObject* CopyObject;
	AResizableObject* PasteObject;
	AResizableObject* PhantomObject;

	TMap<AResizableObject*, AResizableObject*> ObjectsPaste;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Editable, meta = (AllowPrivateAccess = "true"))
	float ShootDistanceBase = 800.0f;
	
	float ShootDistance = ShootDistanceBase;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Editable, meta = (AllowPrivateAccess = "true"))
	float SpeedScrolling = 50.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Editable, meta = (AllowPrivateAccess = "true"))
	float MinimumScrollValue = 250.0f;
	
	static const TMap<KeySize, FPasteSize> mapPasteSize;
};
