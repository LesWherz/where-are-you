#include "PlayerCharacter.h"

#include "ACResizePlayer.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Gun/Abilities/CopyPaste.h"
#include "UE_project/ResizableObjects/ResizableObjectBase.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	Camera = CreateDefaultSubobject<UCameraComponent>("PlayerCamera");
	Camera->bUsePawnControlRotation = true;
    Camera->SetupAttachment(RootComponent);

	SKMeshArms = CreateDefaultSubobject<USkeletalMeshComponent>("SKMeshArms");
	SKMeshArms->SetCollisionProfileName("CharacterMesh");
	SKMeshArms->SetupAttachment(Camera);

	SKMeshGun = CreateDefaultSubobject<USkeletalMeshComponent>("SKMeshGun");
	SKMeshGun->SetCollisionProfileName("NoCollision");
	SKMeshGun->SetupAttachment(SKMeshArms, FName("GripPoint"));

	ShootPoint = CreateDefaultSubobject<USceneComponent>("ShootPoint");
	ShootPoint->SetupAttachment(SKMeshGun);

	ShrunkedHitbox = CreateDefaultSubobject<UBoxComponent>("ShrunkedHitbox");
	ShrunkedHitbox->SetCollisionProfileName("ShrunkedHitbox");
	ShrunkedHitbox->SetupAttachment(RootComponent);
	
	ACPhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>("ACPhysicsHandle");
	ACResizePlayer = CreateDefaultSubobject<UACResizePlayer>("ACResizePlayer");
	ACCopyPaste = CreateDefaultSubobject<UCopyPaste>("ACCopyPaste");
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	UpdatePlayerParams();
	UpdateGunMaterial();
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// ================================================================================ Inputs
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis(TEXT("Turn"),this,&APlayerCharacter::InputLookRightLeft);
	PlayerInputComponent->BindAxis(TEXT("LookUp"),this,&APlayerCharacter::InputLookUpDown);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"),this,&APlayerCharacter::InputMoveForwardBackward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"),this,&APlayerCharacter::InputMoveRightLeft);
	PlayerInputComponent->BindAxis(TEXT("MouseWheel"),this,&APlayerCharacter::InputMoveRightLeft);
	PlayerInputComponent->BindAction(TEXT("Jump"),IE_Pressed,this,&APlayerCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Interact"),IE_Pressed,this,&APlayerCharacter::InputInteract);
	PlayerInputComponent->BindAction(TEXT("Fire"),IE_Pressed,this,&APlayerCharacter::InputFire);
	PlayerInputComponent->BindAction(TEXT("CopyPasteMode"),IE_Pressed,this,&APlayerCharacter::InputCopyPastePressed);
	PlayerInputComponent->BindAction(TEXT("CopyPasteMode"),IE_Released,this,&APlayerCharacter::InputCopyPasteReleased);
	PlayerInputComponent->BindAction(TEXT("DisplayMenu"),IE_Pressed,this,&APlayerCharacter::InputDisplayMenu);
	PlayerInputComponent->BindAction(TEXT("Swap"),IE_Pressed,this,&APlayerCharacter::InputSwapGun);
}

// #ifdef WITH_EDITOR
// void APlayerCharacter::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
// {
// 	Super::PostEditChangeProperty(PropertyChangedEvent);
//
// 	UpdateGunMaterial();
// }
// #endif

// ================================================================================ Player movement
void APlayerCharacter::InputLookRightLeft(float axisValue)
{
	AddControllerYawInput(axisValue * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::InputLookUpDown(float axisValue)
{
	AddControllerPitchInput(axisValue * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::InputMoveForwardBackward(float axisValue)
{
	AddMovementInput(GetActorForwardVector() * axisValue);
}

void APlayerCharacter::InputMoveRightLeft(float axisValue)
{
	AddMovementInput(GetActorRightVector() * axisValue);
}

void APlayerCharacter::InputMouseWheel(float axisValue)
{
	if(!ACCopyPaste->PhantomIsActive())
		return;

	ACCopyPaste->MouseScrollAction(axisValue);
}

void APlayerCharacter::InputInteract()
{
	UE_LOG(LogTemp, Display, TEXT("InputInteract"));
}

void APlayerCharacter::InputFire()
{
	if(PlayerGunMode != EGunMode::Copy && PlayerGunMode != EGunMode::Resize)
		return;
	
	FVector StartShootVector;
	FVector EndShootVector;
	GetShootVectorsForLineTrace(StartShootVector, EndShootVector);

	FHitResult HitResult;
	
	FCollisionObjectQueryParams ObjectQueryParams = FCollisionObjectQueryParams(ECC_Pawn);
	ObjectQueryParams.AddObjectTypesToQuery(ShrunkedColChannel);
	
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	
	DrawDebugLine(GetWorld(), StartShootVector, EndShootVector, FColor::Red, false, 5, 0, 1);

	// TODO : Check if we really need those two line traces (can't we just try to cast to PlayerCharacter, then ResizableObject ?)
	
	// Line trace to detect player
	if(GetWorld()->LineTraceSingleByObjectType(HitResult, StartShootVector, EndShootVector, ObjectQueryParams, CollisionParams))
	{
		if(HitResult.bBlockingHit)
		{
			APlayerCharacter* HitPlayerCharacter;
			
			if(IsValid(HitPlayerCharacter = Cast<APlayerCharacter>(HitResult.Actor)))
			{
				Server_OnPlayerHit(HitPlayerCharacter, PlayerGunMode);
			}
			
			return;
		}
	}

	// Line trace to detect resizable object
	if(GetWorld()->LineTraceSingleByChannel(HitResult, StartShootVector, EndShootVector, ECC_Visibility, CollisionParams))
	{
		if(HitResult.bBlockingHit && HitResult.Actor != this)
		{
			AResizableObject* HitResizableObject;
			
			if(IsValid(HitResizableObject = Cast<AResizableObject>(HitResult.Actor)))
			{
				Server_OnResizableObjectHit(HitResizableObject, PlayerGunMode);
			}
		}
	}
}

void APlayerCharacter::InputCopyPastePressed()
{
	if(!GetIsNecessary())
		return;

	ACCopyPaste->PressedAction();
}

void APlayerCharacter::InputCopyPasteReleased()
{
	if(!GetIsNecessary())
		return;

	ACCopyPaste->ReleasedAction();
}

void APlayerCharacter::InputDisplayMenu()
{
	UE_LOG(LogTemp, Display, TEXT("InputDisplayMenu"));
}

void APlayerCharacter::InputSwapGun()
{
	Server_SwapGun(this);
}

void APlayerCharacter::Server_OnPlayerHit_Implementation(APlayerCharacter* HitPlayerCharacter, EGunMode GunMode)
{
	HitPlayerCharacter->GetACResizePlayer()->Server_ResizeIfPossible(PlayerGunMode);
}

bool APlayerCharacter::Server_OnPlayerHit_Validate(APlayerCharacter* HitPlayerCharacter, EGunMode GunMode)
{
	return true;
}

void APlayerCharacter::Server_OnResizableObjectHit_Implementation(AResizableObject* HitResizableObject, EGunMode GunMode)
{
	HitResizableObject->All_ToggleResize(GunMode);
}

bool APlayerCharacter::Server_OnResizableObjectHit_Validate(AResizableObject* HitResizableObject, EGunMode GunMode)
{
	return true;
}

// ========================= Swap gun
void APlayerCharacter::Server_SwapGun_Implementation(APlayerCharacter* PlayerCharacter)
{
	PlayerCharacter->All_SwapGun();
}

bool APlayerCharacter::Server_SwapGun_Validate(APlayerCharacter* PlayerCharacter)
{
	return true;
}

void APlayerCharacter::All_SwapGun_Implementation()
{
	PlayerGunMode =
		PlayerGunMode == EGunMode::Copy ?
		EGunMode::Resize : EGunMode::Copy;

	UpdateGunMaterial();
}

// ========================= Copy paste
void APlayerCharacter::ServerSpawnPastedObject_Implementation(UClass* classe,const FTransform& SpawnTransform, AActor* SpawnOwner, EPlayerSizeState PlayerShootSize, EObjectSizeState BaseStateObject, AResizableObject* Parent)
{
	FActorSpawnParameters Parameters;
	Parameters.Owner = SpawnOwner;
	auto PasteObjectSpawn = GetWorld()->SpawnActor(classe, &SpawnTransform, Parameters);

	PasteObjectSpawn->SetReplicates(true);

	auto PasteObject = Cast<AResizableObject>(PasteObjectSpawn);
	PasteObject->SetCopyParent(Parent);
	ACCopyPaste->ResizeObjectAccordingOnPlayerSize(PasteObject, PlayerShootSize, BaseStateObject);
	ACCopyPaste->SaveAndDeletePasteObject(Parent, PasteObject);
}


bool APlayerCharacter::ServerSpawnPastedObject_Validate(UClass* classe,const FTransform& SpawnTransform, AActor* SpawnOwner, EPlayerSizeState PlayerShootSize, EObjectSizeState BaseStateObject, AResizableObject* Parent)
{	
	return true;
}

void APlayerCharacter::ServerSetCopyObjectAndPlayerState_Implementation(AResizableObject* ObjectCopy, EPlayerSizeState PlayerSizeState, bool Copy)
{
	AllSetCopyObjectAndPlayerState_Implementation(ObjectCopy, PlayerSizeState, Copy);
}

bool APlayerCharacter::ServerSetCopyObjectAndPlayerState_Validate(AResizableObject* ObjectCopy, EPlayerSizeState PlayerSizeState, bool Copy)
{	
	return true;
}

void APlayerCharacter::AllSetCopyObjectAndPlayerState_Implementation(AResizableObject* ObjectCopy, EPlayerSizeState PlayerSizeState, bool Copy)
{
	this->ACCopyPaste->ConfigCopy(ObjectCopy, PlayerSizeState, Copy);
}

void APlayerCharacter::UpdatePlayerParams()
{
	// TODO : Player controller => Stop sprinting

	// FPlayerCharacterParams NewPlayerParams;
	//
	// if(ACResizePlayer->GetPlayerSizeState() == EPlayerSizeState::Normal)
	// {
	// 	// NewPlayerParams = Get data table row Normal
	// }
	// else
	// {
	// 	// NewPlayerParams = Get data table row Shrunked
	// }
	//
	// PlayerJumpParameters = NewPlayerParams;

	GetCharacterMovement()->GravityScale    = PlayerJumpParameters.GravityScale;
	GetCharacterMovement()->MaxAcceleration = PlayerJumpParameters.MaxAcceleration;
	GetCharacterMovement()->MaxWalkSpeed    = PlayerJumpParameters.MaxWalkSpeed;
	GetCharacterMovement()->JumpZVelocity   = PlayerJumpParameters.JumpZVelocity;
}

void APlayerCharacter::UpdateGunMaterial()
{
	SKMeshGun->SetMaterial(0, PlayerGunMode == EGunMode::Copy ? MaterialGunSmaller : MaterialGunBigger);
}

void APlayerCharacter::UpdateShrunkedCollisionCompActivation()
{
	if(ACResizePlayer->GetPlayerSizeState() == EPlayerSizeState::Normal)
	{
		ShrunkedHitbox->SetHiddenInGame(true);
		ShrunkedHitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else
	{
		ShrunkedHitbox->SetHiddenInGame(false);
		ShrunkedHitbox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void APlayerCharacter::OnResizeEnd()
{
	UpdatePlayerParams();
	UpdateShrunkedCollisionCompActivation();
}

void APlayerCharacter::GetShootVectorsForLineTrace(FVector& StartVector, FVector& EndVector) const
{
	StartVector = GetCamera()->GetComponentLocation();
	EndVector = StartVector + GetCamera()->GetForwardVector() * TraceShootLenght;
}