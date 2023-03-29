// Fill out your copyright notice in the Description page of Project Settings.


#include "PCPlayerCharacter.h"

#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "GameFramework/PlayerInput.h"
#include "Kismet/GameplayStatics.h"


void APCPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	ServerInitialiseModeClients();
}

void APCPlayerCharacter::SetupInputComponent()
{
	Super::SetupInputComponent();

	//InputComponent->BindAction(TEXT("Escape_Pause"), IE_Pressed, this, &APCPlayerCharacter::StartChangeState);
	//InputComponent->BindKey(EKeys::P, IE_Released, this, &APCPlayerCharacter::StartChangeState);
}

void APCPlayerCharacter::ServerInitialiseModeClients_Implementation()
{
	auto gamemode = Cast<AGMPlayerCharacter>(UGameplayStatics::GetGameMode(this->GetWorld()));
	gamemode->All_InitialiseMode();
}

void APCPlayerCharacter::ClientsSetupModeGun_Implementation(EGunMode mode)
{
	PlayerGunMode = mode;
}

void APCPlayerCharacter::ClientSwapGun_Implementation()
{
	auto player = UGameplayStatics::GetPlayerCharacter(this->GetWorld(), 0);
	APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(player);

	//playerCharacter->ServerChangeMaterial(PlayerGunMode);
}

void APCPlayerCharacter::WaitSecondPlayer_Implementation()
{
	IsWaiting = true;

	if(IsInPause)
		return;

	ChangeState(EGameState::Wait);
}

void APCPlayerCharacter::StopWaitSecondPlayer_Implementation()
{
	IsWaiting = false;

	if(IsInPause)
		return;

	TArray<UUserWidget*> FoundWidget;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), FoundWidget,WaitForOtherPlayer);
	
	for(auto widget : FoundWidget)
	{
		widget->RemoveFromParent();
	}

	ChangeState(EGameState::InGame);
}

void APCPlayerCharacter::StartChangeState()
{
	if(IsInPause)
		ChangeState(EGameState::InGame);
	else
		ChangeState(EGameState::Pause);
}

void APCPlayerCharacter::ChangeState(EGameState state)
{
	FInputModeGameOnly InputMode;
	FInputModeUIOnly inputMode;
	UUserWidget* widget;
	
	switch (state)
	{
	case EGameState::InGame:
		SetShowMouseCursor(false);
		IsInPause = false;
		
		SetInputMode(InputMode);
		return;
		
	case EGameState::Pause:
		SetShowMouseCursor(true);
		IsInPause = true;

		if(nullptr == NetPauseMenu)
			return;
		
		widget = CreateWidget(this, NetPauseMenu);
		widget->AddToViewport();	

		inputMode.SetWidgetToFocus(widget->TakeWidget());
		SetInputMode(inputMode);
		return;
		
	case EGameState::Wait:
		SetShowMouseCursor(true);
		IsInPause = false;

		if(nullptr == WaitForOtherPlayer)
			return;
		
		widget = CreateWidget(this, WaitForOtherPlayer);
		widget->AddToViewport();	

		inputMode.SetWidgetToFocus(widget->TakeWidget());
		SetInputMode(inputMode);
	}
}

void APCPlayerCharacter::ServerChangeMode_Implementation()
{
	 auto gamemode = Cast<AGMPlayerCharacter>(UGameplayStatics::GetGameMode(this->GetWorld()));
	gamemode->Server_SwapGun(this);
}

void APCPlayerCharacter::WeaponVisibilityFalse_Implementation()
{
	auto player = UGameplayStatics::GetPlayerCharacter(this->GetWorld(), 0);
	if(nullptr == player)
		return;

	auto playerCharacter = Cast<APlayerCharacter>(player);
	//playerCharacter->SetVisibilityToFalseServer();
}

void APCPlayerCharacter::CreateLoadingScreenForAll()
{
	if(!this->IsLocalPlayerController())
		return;

	if(nullptr == W_LoadingScreen)
		return;
	
	auto widget = CreateWidget(this, W_LoadingScreen);
	widget->AddToViewport();
}

void APCPlayerCharacter::ServerChangeText_Implementation(AActor* Digicode, const FString& Text)
{
	ChangeTextDigicode(Digicode, Text);
}

void APCPlayerCharacter::RemovePlayer_Implementation()
{
	UGameplayStatics::OpenLevel(this->GetWorld(), "World/Game/Maps/Net_MainMenuLevel.Net_MainMenuLevel");
	DestroySession();
}

void APCPlayerCharacter::EjectPlayerFromPlayerList_Implementation()
{
	auto GameMode = Cast<AGMPlayerCharacter>(this->GetWorld()->GetAuthGameMode());
	//GameMode->PlayerLeave(this);
}

void APCPlayerCharacter::ServerChangeColorCableStele_Implementation(AActor* Cable, UMaterialInterface* Material,
	AActor* Stele, bool IsActive)
{
	MultiChangeColorCableStele(Cable, Material, Stele, IsActive);
}

void APCPlayerCharacter::MultiChangeColorCableStele_Implementation(AActor* Cable, UMaterialInterface* Material,
	AActor* Stele, bool IsActive)
{
	ChangeColorCableStele(Cable, Material, Stele, IsActive);
}

void APCPlayerCharacter::ServerVerifNbrSteleActivated_Implementation(AActor* Stele)
{
	VerifNbrSteleActivated(Stele);
}

void APCPlayerCharacter::ClientAddWidgetByClass_Implementation(UClass* classe)
{
	auto widget = CreateWidget(this, classe);
	widget->AddToViewport();
	MyWidget = widget;
	
}

void APCPlayerCharacter::ClientRemoveAllWidgetByClass_Implementation(UClass* classe)
{
	TArray<UUserWidget*> widgetFind;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(this->GetWorld(), widgetFind, classe);

	for(auto widget : widgetFind)
		widget->RemoveFromParent();
}

AGMPlayerCharacter* APCPlayerCharacter::GetGameMode()
{
	auto GameMode = Cast<AGMPlayerCharacter>(this->GetWorld()->GetAuthGameMode());

	if(nullptr != GameMode)
		return GameMode;

	return nullptr;
}
