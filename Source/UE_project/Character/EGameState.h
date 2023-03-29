#pragma once

UENUM(BlueprintType, Category="Project")
enum class EGameState : uint8
{
	InGame = 0 UMETA(DisplayName = "InGame"),
	Pause UMETA(DisplayName = "Pause"),
	Wait UMETA(DisplayName = "Wait")
};
