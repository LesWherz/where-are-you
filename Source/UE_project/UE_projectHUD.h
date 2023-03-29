// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UE_projectHUD.generated.h"

UCLASS()
class AUE_projectHUD : public AHUD
{
	GENERATED_BODY()

public:
	AUE_projectHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;
};