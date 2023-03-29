#include "ACResizePlayer.h"

#include "Components/CapsuleComponent.h"
#include "Gun/EGunMode.h"

UACResizePlayer::UACResizePlayer()
{
	PrimaryComponentTick.bCanEverTick = true;

	if(!IsValid(GetOwner()) || !IsValid(PlayerOwner = Cast<APlayerCharacter>(GetOwner())))
		return;
	
	NormalCapsuleHeight = PlayerOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2.f;
	ShrunkedCapsuleHeight = NormalCapsuleHeight * ShrunkedScaleFactor;
	NormalCapsuleRadius = PlayerOwner->GetCapsuleComponent()->GetScaledCapsuleRadius();
}

void UACResizePlayer::BeginPlay()
{
	Super::BeginPlay();

	CurrentScaleFactor = PlayerSizeState == EPlayerSizeState::Normal ? NormalScaleFactor : ShrunkedScaleFactor;
}

void UACResizePlayer::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if(PlayerSizeState == EPlayerSizeState::Resizing) // Resize logic
	{
		// ---- Lerp scale when resizing
		const float t = TimeLerpResize / DurationLerpResize;
		const float lerpedVal  = FMath::Lerp(CurrentScaleFactor, TargetScaleFactor, t);

		GetOwner()->SetActorScale3D(FVector(lerpedVal)); 
		UE_LOG(LogTemp, Warning, TEXT("%f"), PlayerOwner->GetActorScale3D().X);
		TimeLerpResize += DeltaTime;

		// ---- Stop resizing when it ends
		if(TimeLerpResize > DurationLerpResize)
		{
			GetOwner()->SetActorScale3D(FVector(TargetScaleFactor));
			
			CurrentScaleFactor = TargetScaleFactor;
			PlayerSizeState = FuturePlayerSizeState;

			// Does not work with PlayerOwner, probably because PlayerOwner doesn't have the ownership
			GetOwner<APlayerCharacter>()->OnResizeEnd(); 
		}
	}
}

void UACResizePlayer::Server_ResizeIfPossible_Implementation(EGunMode GunModeForResize)
{
	if(!CanBeResized(GunModeForResize))
	{
		return;
	}
	
	All_ResizePlayer();
}

bool UACResizePlayer::Server_ResizeIfPossible_Validate(EGunMode GunModeForResize)
{
	return true;
}

void UACResizePlayer::All_ResizePlayer_Implementation()
{
	// ---- Init resize
	switch(PlayerSizeState)
	{
		case EPlayerSizeState::Normal:
			TargetScaleFactor = ShrunkedScaleFactor;
			FuturePlayerSizeState = EPlayerSizeState::Shrunked;
			break;
		case EPlayerSizeState::Shrunked:
			TargetScaleFactor = NormalScaleFactor;
			FuturePlayerSizeState = EPlayerSizeState::Normal;
			break;
	}

	TimeLerpResize = 0.f;
	PlayerSizeState = EPlayerSizeState::Resizing;
	
	PlayerOwner->UpdateShrunkedCollisionCompActivation();
}

bool UACResizePlayer::CanBeResized(EGunMode GunModeForResize) const
{
	// Can resize if the player is not resizing
	// Can resize if the player is at Normal size and Gun at Shrunked mode
	// Can resize if the player is at Shrunked size and Gun at Bigger mode
	// Can resize if, when the player is shrunked, CanGrowUp()

	return !IsResizing() && (
			(PlayerSizeState == EPlayerSizeState::Shrunked && GunModeForResize == EGunMode::Resize && CanGrowUp()) ||
			 PlayerSizeState == EPlayerSizeState::Normal   && GunModeForResize == EGunMode::Copy);
}

bool UACResizePlayer::CanGrowUp() const
{
	if(PlayerSizeState == EPlayerSizeState::Normal)
		return false;
	
	FVector StartVector, EndVector;
	GetVectorsLineTraceForGrowingTest(StartVector, EndVector);

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(PlayerOwner);
	
	if(GetWorld()->LineTraceSingleByChannel(HitResult, StartVector, EndVector, ECC_Visibility, CollisionParams))
	{
		//return !HitResult.bBlockingHit;
		return true;
	}

	UE_LOG(LogTemp, Display, TEXT("Line trace in CanGrowUp() function failed !"))
	return false;
}

bool UACResizePlayer::IsResizing() const
{
	return PlayerSizeState == EPlayerSizeState::Resizing;
}

void UACResizePlayer::GetVectorsLineTraceForGrowingTest(FVector& StartVector, FVector& EndVector) const
{
	const float StartPosZ = ShrunkedCapsuleHeight / 2.f + NormalCapsuleRadius;
	const float EndPosZ   = ShrunkedCapsuleHeight / 2.f - NormalCapsuleRadius + NormalCapsuleHeight;
	const FVector CurrPlayerLocation = PlayerOwner->GetCapsuleComponent()->GetComponentLocation();

	StartVector = FVector(CurrPlayerLocation.X, CurrPlayerLocation.Y, StartPosZ);
	EndVector = FVector(CurrPlayerLocation.X, CurrPlayerLocation.Y, EndPosZ);
}