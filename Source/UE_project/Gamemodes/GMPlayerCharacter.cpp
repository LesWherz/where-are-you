#include "GMPlayerCharacter.h"
#include "UE_project/Character/PCPlayerCharacter.h"

void AGMPlayerCharacter::IsEveryoneThere()
{
	if(numberOfPlayer == 2)
	{
		//for (auto Player : List_Player)
			//Player->StopWaitSecondPlayer();
	}
	else
	{
		//for (auto Player : List_Player)
			//Player->WaitSecondPlayer();
	}
}

void AGMPlayerCharacter::All_InitialiseMode()
{
	if(List_Player.Num() < 2)
		return;

	for(int i = 0; i < List_Player.Num(); i++)
	{
		//List_Player[i]->ClientSetupModeGun(List_ModePlayer[i]);
		//List_Player[i]->ClientSwapGun();
	}
}

void AGMPlayerCharacter::Server_SwapGun(APCPlayerCharacter* Player)
{
	auto index = List_Player.Find(Player);
	if(index == INDEX_NONE)
		return;

	auto modePlayer = List_ModePlayer[index];
	switch (modePlayer)
	{
		case EGunMode::Copy:
			List_ModePlayer[index] = EGunMode::Resize;
			break;

		case EGunMode::Resize:
			List_ModePlayer[index] = EGunMode::Copy;
			break;

		/*case EGunMode::Copy:
			List_ModePlayer[index] = EGunMode::Copy;
			break;*/
	}

	//Player->ClientSetupModeGun(List_ModePlayer[index]);
	//Player->ClientSwapGun();
}

void AGMPlayerCharacter::PlayerLeave(APCPlayerCharacter* ItemToFind)
{
	auto index = List_Player.Find(ItemToFind);
	if(index == INDEX_NONE)
		return;

	List_Player.RemoveAt(index);
	numberOfPlayer--;

	IsEveryoneThere();
}

void AGMPlayerCharacter::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	auto Player = Cast<APCPlayerCharacter>(NewPlayer);
	List_Player.AddUnique(Player);
	numberOfPlayer++;

	IsEveryoneThere();
}

int AGMPlayerCharacter::GetNewId() const
{
	return ++IdCopyPaste;
}
