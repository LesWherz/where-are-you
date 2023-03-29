#pragma once

#include "CoreMinimal.h"
#include "EGameState.h"
#include "GameFramework/PlayerController.h"
#include "Gun/EGunMode.h"
#include "UE_project/Gamemodes/GMPlayerCharacter.h"
#include "PCPlayerCharacter.generated.h"

UCLASS(BlueprintType)
class UE_PROJECT_API APCPlayerCharacter : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	
	virtual void SetupInputComponent() override;
	
public:

#pragma region Initialise Mode

	UFUNCTION(Server, Reliable)
	void ServerInitialiseModeClients();

	UFUNCTION(NetMulticast, Unreliable)
	void ClientsSetupModeGun(EGunMode mode);

#pragma endregion Initialise Mode
	
#pragma region Change Mode Double Gun

	UFUNCTION(Client, Reliable)
	void ClientSwapGun();

#pragma endregion Change Mode Double Gun

#pragma region WAIT OTHER PLAYER

	UFUNCTION(Client, Reliable)
	void WaitSecondPlayer();

	UFUNCTION(Client, Reliable)
	void StopWaitSecondPlayer();

#pragma endregion WAIT OTHER PLAYER
	
#pragma region PAUSE AND WAIT HANDLER

	void StartChangeState();

	void ChangeState(EGameState state);

#pragma endregion PAUSE AND WAIT HANDLER
	
#pragma region Swap Gun

	UFUNCTION(Server, Reliable)
	void ServerChangeMode();

#pragma endregion Swap Gun

#pragma region Change Visibility of Weapon to False

	UFUNCTION(Client, Reliable)
	void WeaponVisibilityFalse();

#pragma endregion Change Visibility of Weapon to False

#pragma region LOADING SCREEN

	void CreateLoadingScreenForAll();

#pragma endregion LOADING SCREEN

#pragma region CHANGE TEXT DIGICODE

	UFUNCTION(Server, Reliable)
	void ServerChangeText(AActor* Digicode, const FString& Text);

	UFUNCTION(BlueprintImplementableEvent)
	void ChangeTextDigicode(AActor* Digicode, const FString& Text);

#pragma endregion CHANGE TEXT DIGICODE

#pragma region Kicked from lobby (no error message actually for the kicked option)

	UFUNCTION(Client, Reliable)
	void RemovePlayer();

	UFUNCTION(BlueprintImplementableEvent)
	void DestroySession();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void EjectPlayerFromPlayerList();
	

#pragma endregion Kicked from lobby (no error message actually for the kicked option)

#pragma region Stele

	UFUNCTION(Server, Reliable)
	void ServerChangeColorCableStele(AActor* Cable, UMaterialInterface* Material, AActor* Stele, bool IsActive);

	UFUNCTION(NetMulticast, Reliable)
	void MultiChangeColorCableStele(AActor* Cable, UMaterialInterface* Material, AActor* Stele, bool IsActive);

	UFUNCTION(BlueprintImplementableEvent)
	void ChangeColorCableStele(AActor* Cable, UMaterialInterface* Material, AActor* Stele, bool IsActive);

	UFUNCTION(Server, Reliable)
	void ServerVerifNbrSteleActivated(AActor* Stele);

	UFUNCTION(BlueprintImplementableEvent)
	void VerifNbrSteleActivated(AActor* Stele);

#pragma endregion Stele

#pragma region Stele Widget

	UFUNCTION(Client, Reliable)
	void ClientAddWidgetByClass(UClass* classe);

	UFUNCTION(Client, Reliable)
	void ClientRemoveAllWidgetByClass(UClass* classe);

	UFUNCTION(BlueprintImplementableEvent)
	void ClientPCStartTimer();

	UFUNCTION(BlueprintImplementableEvent)
	void PCStopTimer();

#pragma endregion Stele Widget

#pragma region CopyPaste
	
	AGMPlayerCharacter* GetGameMode();

#pragma endregion 
	
protected:
	EGunMode PlayerGunMode { EGunMode::Resize };

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UUserWidget> W_LoadingScreen;

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UUserWidget> NetPauseMenu;

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UUserWidget> WaitForOtherPlayer;

	UPROPERTY(BlueprintReadOnly)
	UUserWidget* MyWidget;
	
	UObject* CallFrom;
	bool IsInPause = false;
	bool IsWaiting = false;
};
