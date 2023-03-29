#pragma once

UENUM(BlueprintType, Category="Project")
enum class EPlayerSizeState : uint8
{
	Normal = 0 UMETA(DisplayName = "NormalSize"),
	Shrunked UMETA(DisplayName = "ShrunkedSize"),
	Resizing UMETA(DisplayName = "Resizing")
};
