#include "ResizableObjectBase.h"

#include "UE_project/Character/Gun/EGunMode.h"

AResizableObject::AResizableObject()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRootComponent = CreateDefaultSubobject<USceneComponent>("SceneRootComponent");
	RootComponent = SceneRootComponent;

	SMObject = CreateDefaultSubobject<UStaticMeshComponent>("SMObject");
	SMObject->SetCollisionProfileName("BlockAllDynamic");
	SMObject->SetupAttachment(RootComponent);

	bReplicates = true;
}

void AResizableObject::BeginPlay()
{
	Super::BeginPlay();

	CurrentScaleFactor = ResizableObjectState == EObjectSizeState::Normal ?
		NormalScaleFactor :
		ResizableObjectState == EObjectSizeState::Big ? BigScaleFactor : SmallScaleFactor;
}

void AResizableObject::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(IsResizing()) // Resize logic
	{
		// ---- Lerp scale when resizing
		const float t = TimeLerpResize / DurationLerpResize;
		const float lerpedVal  = FMath::Lerp(CurrentScaleFactor, TargetScaleFactor, t);

		SetActorScale3D(FVector(lerpedVal));

		TimeLerpResize += DeltaSeconds;

		// ---- Stop resizing when it ends
		if(TimeLerpResize > DurationLerpResize)
		{
			SetActorScale3D(FVector(TargetScaleFactor));

			CurrentScaleFactor = TargetScaleFactor;
			ResizableObjectState = FutureResizableObjectState;
		}
	}
}

void AResizableObject::All_StartResizeObject_Implementation(AResizableObject* ResizableObject)
{
	ResizableObject->Client_StartResizeObject();
}

void AResizableObject::Client_StartResizeObject()
{
	TimeLerpResize = 0.f;
	ResizableObjectState = EObjectSizeState::Resizing;  // Check the Tick() function
}

void AResizableObject::All_ToggleResize_Implementation(EGunMode GunMode)
{
	const bool isBigger = GunMode == EGunMode::Resize;
	
	if((GunMode != EGunMode::Copy && GunMode != EGunMode::Resize) ||
		!CanBeResized(isBigger))
	{
		return;
	}
	
	// Init resizing
	switch (ResizableObjectState)
	{
		case EObjectSizeState::Small:
		case EObjectSizeState::Big:
			FutureResizableObjectState = EObjectSizeState::Normal;
			TargetScaleFactor = NormalScaleFactor;
			break;
					
		case EObjectSizeState::Normal:
			{
				if(isBigger)
				{
					FutureResizableObjectState = EObjectSizeState::Big;
					TargetScaleFactor = BigScaleFactor;
				}
				else
				{
					FutureResizableObjectState = EObjectSizeState::Small;
					TargetScaleFactor = SmallScaleFactor;
				}
			}
	}

	// Start resizing
	TimeLerpResize = 0.f;
	ResizableObjectState = EObjectSizeState::Resizing; // Check the Tick() function
}

bool AResizableObject::IsResizing() const
{
	return ResizableObjectState == EObjectSizeState::Resizing;
}

bool AResizableObject::CanBeResized(bool isBigger) const
{
	return !(IsResizing()	||
		(ResizableObjectState == EObjectSizeState::Big && isBigger) ||
		(ResizableObjectState == EObjectSizeState::Small && !isBigger));
}

// #ifdef WITH_EDITOR
// void AResizableObject::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
// {
// 	Super::PostEditChangeProperty(PropertyChangedEvent);
//
// 	if(ResizableObjectState == EObjectSizeState::Resizing)
// 	{
// 		ResizableObjectState = EObjectSizeState::Normal;
// 		UE_LOG(LogTemp, Warning, TEXT("You cannot set an object to 'Resizing' state !"))
// 	}
// 	
// 	switch(ResizableObjectState)
// 	{
// 		case EObjectSizeState::Small:  SetActorScale3D(FVector(SmallScaleFactor));  break;
// 		case EObjectSizeState::Normal: SetActorScale3D(FVector(NormalScaleFactor)); break;
// 		case EObjectSizeState::Big:    SetActorScale3D(FVector(BigScaleFactor));    break;
// 	}
// } 
// #endif