// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Character/Gun/EGunMode.h"
#include "GameFramework/GameModeBase.h"
#include "UE_projectGameMode.generated.h"

// FIXME : Delete this file and his dependencies

UCLASS(minimalapi)
class AUE_projectGameMode : public AGameModeBase
{
	GENERATED_BODY()

private:
	TArray<EGunMode> List_ModePlayer;

public:
	AUE_projectGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;

	UFUNCTION(Server, Unreliable)
	void Server_SwapGun();
};



