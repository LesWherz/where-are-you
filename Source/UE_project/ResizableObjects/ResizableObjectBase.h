#pragma once

#include <UE_project/Character/Gun/EGunMode.h>

#include "CoreMinimal.h"
#include "EObjectSizeState.h"
#include "GameFramework/Actor.h"
#include "UE_project/Character/Gun/EGunMode.h"
#include "ResizableObjectBase.generated.h"

UCLASS(BlueprintType)
class UE_PROJECT_API AResizableObject : public AActor
{
	GENERATED_BODY()

public:
	AResizableObject();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
//
// #ifdef WITH_EDITOR
// 	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
// #endif
	
	/**
	 * Use this method to start resizing an object. If it is not possible, no resize is processed.
	 */
	UFUNCTION(NetMulticast, Reliable)
	void All_ToggleResize(EGunMode GunMode);
	
	// ---- Getters - Setters
	UFUNCTION(BlueprintCallable)
	bool IsResizing() const;

	UFUNCTION(BlueprintCallable)
	bool CanBeResized(bool isBigger) const;
	
	virtual EObjectSizeState GetSizeState() const { return ResizableObjectState; }
	void SetState(EObjectSizeState newState) { ResizableObjectState = newState; } // TODO: Delete this function, as we resize objects, we don't set their size manually
	
	FORCEINLINE UStaticMeshComponent* GetSMObject() const { return SMObject; }
	FORCEINLINE AResizableObject*	  GetCopyParent() const { return CopyParent; }
	FORCEINLINE void SetCopyParent(AResizableObject* parent) { CopyParent = parent; }

private:
	UFUNCTION(NetMulticast, Reliable)
	void All_StartResizeObject(AResizableObject* ResizableObject);

	UFUNCTION()
	void Client_StartResizeObject();

protected:
	// ---- Blueprint components
	UPROPERTY(Category=ResizableObject, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	USceneComponent* SceneRootComponent;
	
	UPROPERTY(Category=ResizableObject, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UStaticMeshComponent* SMObject;

	// ---- General Properties
	UPROPERTY(BlueprintReadWrite)
	AResizableObject* CopyParent;

	// ---- Resize Properties
	UPROPERTY(EditAnywhere)
	EObjectSizeState ResizableObjectState { EObjectSizeState::Normal };

	UPROPERTY(BlueprintReadWrite) // Replicated
	EObjectSizeState FutureResizableObjectState; // Useful to register the future ResizableObjectState while resizing the object

	UPROPERTY(EditAnywhere)
	float SmallScaleFactor { 0.05f };

	UPROPERTY(EditAnywhere)
	float NormalScaleFactor { 1.f };

	UPROPERTY(EditAnywhere)
	float BigScaleFactor { 6.f };
	
	// ---- Lerp Resize Properties
	UPROPERTY(EditAnywhere)
	float DurationLerpResize { 0.5f };
	
	UPROPERTY(BlueprintReadWrite)
	float TimeLerpResize;
	
	UPROPERTY(BlueprintReadWrite)
	float CurrentScaleFactor; // Only updated before and after resizing, not during resize process
	
	UPROPERTY(BlueprintReadWrite) // Replicated
	float TargetScaleFactor;
};
