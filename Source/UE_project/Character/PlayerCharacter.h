#pragma once

#include "CoreMinimal.h"
#include "EPlayerSizeState.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Gun/EGunMode.h"
#include "UE_project/GameUtils/PlayerCharacterParams.h"
#include "UE_project/ResizableObjects/EObjectSizeState.h"
#include "UE_project/ResizableObjects/ResizableObjectBase.h"
#include "PlayerCharacter.generated.h" 

class UACResizePlayer;
class UCopyPaste;
class APatchedUECharacter;

UCLASS(BlueprintType)
class UE_PROJECT_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()
	
public:
	APlayerCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
//
// #ifdef WITH_EDITOR
// 	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
// #endif
	
	// ---- Inputs
	void InputLookRightLeft(float axisValue);
	void InputLookUpDown(float axisValue);
	void InputMoveForwardBackward(float axisValue);
	void InputMoveRightLeft(float axisValue);
	void InputMouseWheel(float axisValue);
	void InputInteract();
	void InputFire();
	void InputCopyPastePressed();
	void InputCopyPasteReleased();
	void InputDisplayMenu();
	void InputSwapGun();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_OnResizableObjectHit(AResizableObject* HitResizableObject, EGunMode GunMode);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_OnPlayerHit(APlayerCharacter* HitPlayerCharacter, EGunMode GunMode);
	
	// ---- Client - Server Interactions
	// TODO : Swap gun into PlayerController or PlayerGameMode ?
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SwapGun(APlayerCharacter* PlayerCharacter);

	UFUNCTION(NetMulticast, Reliable)
	void All_SwapGun();
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSpawnPastedObject(UClass* classe,const FTransform& SpawnTransform, AActor* SpawnOwner, EPlayerSizeState PlayerShootSize, EObjectSizeState BaseStateObject, AResizableObject* Parent);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetCopyObjectAndPlayerState(AResizableObject* ObjectCopy, EPlayerSizeState PlayerSizeState, bool Copy);

	UFUNCTION(NetMulticast, Reliable)
	void AllSetCopyObjectAndPlayerState(AResizableObject* ObjectCopy, EPlayerSizeState PlayerSizeState, bool Copy);

public:
	// ---- Getters
	UFUNCTION(BlueprintCallable)
	void GetShootVectorsForLineTrace(FVector& StartVector, FVector& EndVector) const;

	UFUNCTION(BlueprintCallable)
	void UpdatePlayerParams();

	UFUNCTION(BlueprintCallable)
	void UpdateGunMaterial();
	
	UFUNCTION(BlueprintCallable)
	void UpdateShrunkedCollisionCompActivation();

	UFUNCTION(BlueprintCallable)
	void OnResizeEnd();
	
	FORCEINLINE UCameraComponent*		 GetCamera() const { return Camera; }
	FORCEINLINE USkeletalMeshComponent*  GetSKMeshArms() const { return SKMeshArms; }
	FORCEINLINE USkeletalMeshComponent*  GetSKMeshGun() const { return SKMeshGun; }
	FORCEINLINE USceneComponent*		 GetShootPoint() const { return ShootPoint; }
	FORCEINLINE UBoxComponent*		     GetShrunkedLineTracesBoxCol() const { return ShrunkedHitbox; }
	FORCEINLINE UPhysicsHandleComponent* GetACPhysicsHandle() const { return ACPhysicsHandle; }
	FORCEINLINE UACResizePlayer*		 GetACResizePlayer() const { return ACResizePlayer; }
	FORCEINLINE UCopyPaste*				 GetACCopyPaste() const { return ACCopyPaste; }
	FORCEINLINE EGunMode				 GetGunMode() const { return PlayerGunMode; }

private:
	bool GetIsNecessary() { return GetSKMeshGun()->IsVisible(); }

protected:
	// ---- Blueprint components
	UPROPERTY(Category=PlayerCharacter, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UCameraComponent* Camera;
	
	UPROPERTY(Category=PlayerCharacter, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	USkeletalMeshComponent* SKMeshArms;

	UPROPERTY(Category=PlayerCharacter, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	USkeletalMeshComponent* SKMeshGun;

	UPROPERTY(Category=PlayerCharacter, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	USceneComponent* ShootPoint;
	
	UPROPERTY(Category=PlayerCharacter, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UBoxComponent* ShrunkedHitbox;
	
	// ---- Actor components
	UPROPERTY(Category=PlayerCharacter, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UPhysicsHandleComponent* ACPhysicsHandle;
	
	UPROPERTY(Category=PlayerCharacter, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UACResizePlayer* ACResizePlayer;
	
	UPROPERTY(Category=PlayerCharacter, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UCopyPaste* ACCopyPaste;
	
	// ---- Attributes
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = "true"))
	TEnumAsByte<ECollisionChannel> ShrunkedColChannel;
	
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = "true"))
	UMaterialInterface* MaterialGunBigger;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = "true"))
	UMaterialInterface* MaterialGunSmaller;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float BaseTurnRate = 45.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float BaseLookUpRate = 45.f ;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float TraceShootLenght = 2000.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FPlayerCharacterParams PlayerJumpParameters;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    EGunMode PlayerGunMode { EGunMode::Resize };
};
