// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UE_project/Character/PlayerCharacter.h"
#include "UE_project/Character/Gun/EGunMode.h"
#include "GMPlayerCharacter.generated.h"


class APCPlayerCharacter;

/**
 * 
 */
UCLASS(BlueprintType)
class UE_PROJECT_API AGMPlayerCharacter : public AGameModeBase
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void IsEveryoneThere();

	UFUNCTION(BlueprintCallable)
	void All_InitialiseMode();

	UFUNCTION(BlueprintCallable)
	void Server_SwapGun(APCPlayerCharacter* Player);

	UFUNCTION(BlueprintCallable)
	void PlayerLeave(APCPlayerCharacter* ItemToFind);
	
	virtual void PostLogin(APlayerController* NewPlayer) override;

	UFUNCTION(BlueprintCallable)
	int GetNewId() const;

private:
	TArray<APCPlayerCharacter*> List_Player;
	TArray<EGunMode> List_ModePlayer;
	int SpawnIncrement_Dirty = 0;
	int numberOfPlayer = 0;


	mutable int IdCopyPaste = 18;
};
