// Fill out your copyright notice in the Description page of Project Settings.


#include "CopyPaste.h"

#include "CollisionDebugDrawingPublic.h"

const TMap<KeySize, FPasteSize> UCopyPaste::mapPasteSize =
{
			{ {0,0,2} , { EObjectSizeState::Normal, 1.0f} },
			{ {0,0,3} , { EObjectSizeState::Big, 6.0f} },
			{ {0,1,2} , { EObjectSizeState::Small, 0.05f} },
			{ {0,1,3} , { EObjectSizeState::Normal, 1.0f} },
			{ {1,0,1} , { EObjectSizeState::Normal, 1.0f} },
			{ {1,0,2} , { EObjectSizeState::Normal, 1.0f} },
			{ {1,0,3} , { EObjectSizeState::Normal, 1.0f} },
			{ {1,1,1} , { EObjectSizeState::Small, 0.05f} },
			{ {1,1,2} , { EObjectSizeState::Normal, 1.0f} },
			{ {1,1,3} , { EObjectSizeState::Big, 6.0f} }
};

// Sets default values for this component's properties
UCopyPaste::UCopyPaste()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	if(!IsValid(GetOwner()) || !IsValid(PlayerOwner = Cast<APlayerCharacter>(GetOwner())))
		return;
}

// Called when the game starts
void UCopyPaste::BeginPlay()
{
	Super::BeginPlay();

	PlayerOwner = Cast<APlayerCharacter>(GetOwner());
	CopyAvailable = true;
}


// Called every frame
void UCopyPaste::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(!ObjectPhantomIsVisible)
		return;

	if(nullptr != PhantomObject)
	{
		auto PhantomPos = GetPositionPhantom();
		PlayerOwner->GetACPhysicsHandle()->SetTargetLocation(PhantomPos);
		return;
	}

	FTransform transformPhantom(GetEndLineTracePaste());
	CreatePhantomObject(CopyObject->GetClass(), transformPhantom);

	PlayerOwner->GetACPhysicsHandle()->GrabComponentAtLocation(PhantomObject->GetSMObject(), "", PhantomObject->GetSMObject()->GetComponentLocation());
}

FVector UCopyPaste::GetPositionPhantom()
{
	FHitResult hit;
	FCollisionObjectQueryParams ObjectQueryParams; ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);

	FCollisionQueryParams QueryParams; QueryParams.AddIgnoredActor(PasteObject);
	
	this->GetWorld()->LineTraceSingleByObjectType(hit, GetStartLineTrace(), GetEndLineTracePaste(), ObjectQueryParams, QueryParams);

	if(!hit.bBlockingHit)
		return GetEndLineTracePaste();
	
	return hit.GetActor()->GetActorLocation();

	
}

void UCopyPaste::SaveAndDeletePasteObject(AResizableObject* Parent, AResizableObject* ToAdd)
{
	if(!IsValid(ToAdd))
		return;

	auto ObjectPasteFind = GetObjectPasteByClass(Parent);
	if(nullptr != ObjectPasteFind)
		ObjectPasteFind->Destroy();

	AddObjectPaste(Parent, ToAdd);
}

void UCopyPaste::CreatePhantomObject(UClass* ObjectClass, FTransform SpawnTransform)
{
	FActorSpawnParameters SpawnParams = FActorSpawnParameters();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Owner = PlayerOwner;
	AActor* ActorSpawn = this->GetWorld()->SpawnActor(ObjectClass, &SpawnTransform, SpawnParams);

	if(!IsValid(ActorSpawn))
		return;

	PhantomObject = Cast<AResizableObject, AActor>(ActorSpawn);

	if(!IsValid(PhantomObject))
		return;
	
	PhantomObject->SetReplicates(false);

	auto playerSize = GetPlayerSize();
	auto objectSize = GetObjectCopySize();

	auto testPlayerSize = (int32)playerSize;
	auto testObjectSize = (int32)objectSize;

	ResizeObjectAccordingOnPlayerSize(PhantomObject, playerSize, objectSize);

	CustomPhantomObject(PhantomObject->GetSMObject());

	PhantomObject->GetSMObject()->SetSimulatePhysics(true);
	
}

void UCopyPaste::CustomPhantomObject(UPrimitiveComponent* Actor)
{
	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterial(TEXT("/Game/StarterContent/Props/Materials/M_StatueGlass.M_StatueGlass"));
	if(FoundMaterial.Succeeded())
	{
		Actor->SetMaterial(0, FoundMaterial.Object);
	}

	Actor->SetCollisionObjectType(ECC_PhysicsBody);
	Actor->SetCollisionResponseToAllChannels(ECR_Ignore);
}

void UCopyPaste::ResizeObjectAccordingOnPlayerSize(AResizableObject* ObjectToResize, EPlayerSizeState PlayerSootSize,
	EObjectSizeState BaseResizableState)
{
	KeySize key = KeySize((int32)SizePlayerCopyObject, (int32)PlayerSootSize, (int32)BaseResizableState);
	FPasteSize newSize = *mapPasteSize.Find(key);

	ObjectToResize->SetState(newSize._state);
	ObjectToResize->SetActorScale3D(ObjectToResize->GetActorScale3D() * newSize._factor);
}

AResizableObject* UCopyPaste::GetParent() const
{
	if(IsValid(CopyObject->GetCopyParent()))
		return CopyObject->GetCopyParent();

	return CopyObject;
}

void UCopyPaste::PressedAction()
{
	if(!CopyAvailable)
	{
		ObjectPhantomIsVisible = true;
		return;
	}

	UE_LOG(LogTemp, Error, TEXT("Pressed Action"));

	FHitResult result;
	this->GetWorld()->LineTraceSingleByChannel(result, GetStartLineTrace(), GetEndLineTraceCopy(), ECC_Visibility);
	if(!result.IsValidBlockingHit() || !CopyAvailable)
		return;
	

	auto object = Cast<AResizableObject>(result.GetActor());

	if(nullptr == object)
		return;
	
	if(object->GetSizeState() == EObjectSizeState::Resizing)
		return;

	ConfigCopy(object, GetPlayerSize(), false);
	PlayerOwner->ServerSetCopyObjectAndPlayerState_Implementation(CopyObject, SizePlayerCopyObject, CopyAvailable);
}

void UCopyPaste::ReleasedAction()
{
	FHitResult result;
	this->GetWorld()->LineTraceSingleByChannel(result, GetStartLineTrace(), GetEndLineTracePaste(), ECC_Visibility);
	if(result.GetActor() == CopyObject)
		return;

	if(!IsValid(PhantomObject))
		return;

	PlayerOwner->GetACPhysicsHandle()->ReleaseComponent();
	ObjectPhantomIsVisible = false;
	PhantomObject->Destroy();

	auto PhantomTransform = FTransform(GetPositionPhantom());
	PlayerOwner->ServerSpawnPastedObject_Implementation(CopyObject->GetClass(), PhantomTransform, PlayerOwner, GetPlayerSize(), CopyObject->GetSizeState(), GetParent());	
}

void UCopyPaste::MouseScrollAction(float axisValue)
{
	ShootDistance = CalculShootDistance(axisValue);
	if(ShootDistance > MinimumScrollValue)
		return;

	ShootDistance = MinimumScrollValue;
}

void UCopyPaste::ConfigCopy(AResizableObject* copyObject, EPlayerSizeState size, bool copyAvailable)
{
	this->CopyObject = copyObject;
	this->SizePlayerCopyObject = size;
	this->CopyAvailable = copyAvailable;
}


