// Copyright Epic Games, Inc. All Rights Reserved.

#include "UE_projectGameMode.h"
#include "UE_projectHUD.h"
#include "GameFramework/GameStateBase.h"
#include "UObject/ConstructorHelpers.h"

AUE_projectGameMode::AUE_projectGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Assets/Character/Blueprint/BP_PlayerProto"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
	// PlayerControllerClass = ACustomPlayerController::StaticClass();

	// use our custom HUD class
	HUDClass = AUE_projectHUD::StaticClass();

	List_ModePlayer.Init(EGunMode::Copy, 3); // TODO : Use EGunMode::Count
	List_ModePlayer.Add(EGunMode::Resize);
	List_ModePlayer.Add(EGunMode::Copy);
}

void AUE_projectGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// ACustomPlayerController* player = Cast<ACustomPlayerController>(NewPlayer);
	// int index = List_Player.AddUnique(player);
	//
	// GunMode_Legacy mode = List_ModePlayer[index];
	// player->Client_SetupModeGun_Implementation(mode);
}

void AUE_projectGameMode::Server_SwapGun_Implementation()
{
	// GunMode_Legacy temp = List_ModePlayer[0];
	// List_ModePlayer[0] = List_ModePlayer[1];
	// List_ModePlayer[1] = temp;
	//
	// int size = List_Player.Num();
	// for (int i = 0; i < size; i++)
	// {
	// 	List_Player[i]->Client_SetupModeGun_Implementation(List_ModePlayer[i]);
	// 	List_Player[i]->Client_SwapGun_Implementation(List_Player[i]);
	// }
}
