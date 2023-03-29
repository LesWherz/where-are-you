#pragma once

#include "CoreMinimal.h"
#include "EPlayerSizeState.h"
#include "PlayerCharacter.h"
#include "Components/ActorComponent.h"
#include "ACResizePlayer.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_PROJECT_API UACResizePlayer : public UActorComponent
{
	GENERATED_BODY()

public:	
	UACResizePlayer();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_ResizeIfPossible(EGunMode GunModeForResize);

	UFUNCTION(BlueprintCallable)
	bool CanBeResized(EGunMode GunModeForResize) const;
	
	UFUNCTION(BlueprintCallable)
	bool CanGrowUp() const;

	UFUNCTION(BlueprintCallable)
	bool IsResizing() const;

	FORCEINLINE EPlayerSizeState GetPlayerSizeState() const { return PlayerSizeState; }
	
protected:
	UFUNCTION(NetMulticast, Reliable)
	void All_ResizePlayer();
	
	UFUNCTION(BlueprintCallable)
	void GetVectorsLineTraceForGrowingTest(FVector& StartVector, FVector& EndVector) const;
	
protected:
	UPROPERTY(BlueprintReadWrite, Category="PlayerSizeCalculation")
	float NormalCapsuleHeight { 0.f };
	
	UPROPERTY(BlueprintReadWrite, Category="PlayerSizeCalculation")
	float NormalCapsuleRadius { 0.f };
	
	UPROPERTY(BlueprintReadWrite, Category="PlayerSizeCalculation")
	float ShrunkedCapsuleHeight { 0.f };
	
	UPROPERTY(BlueprintReadWrite, Category="PlayerSizeCalculation")
	float CurrentScaleFactor { 0.f }; // Only updated before and after resizing, not during resize process
	
	UPROPERTY(BlueprintReadWrite, Category="PlayerSizeCalculation")
	float TargetScaleFactor { 0.f };

	UPROPERTY(BlueprintReadWrite, Category="PlayerSizeCalculation")
	float TargetCapsuleRadius { 0.f };

	// ---- Default
	UPROPERTY(BlueprintReadWrite)
	float ShrunkedCapsuleRadius { 0.f };

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	float NormalScaleFactor { 1.f };
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	float ShrunkedScaleFactor { 0.025f };

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	float DurationLerpResize { 1.f };

	UPROPERTY(BlueprintReadWrite)
	float TimeLerpResize { 1.f };

	UPROPERTY(EditAnywhere)
	EPlayerSizeState PlayerSizeState { EPlayerSizeState::Normal };

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EPlayerSizeState FuturePlayerSizeState { EPlayerSizeState::Normal };

	UPROPERTY(BlueprintReadWrite)
	APlayerCharacter* PlayerOwner;
};
