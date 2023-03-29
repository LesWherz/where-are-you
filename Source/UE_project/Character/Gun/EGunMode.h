#pragma once

UENUM(BlueprintType, Category="Project")
enum class EGunMode : uint8
{
	Resize  UMETA(DisplayName = "Resize"),
	Copy UMETA(DisplayName = "Copy"),

	/* ONLY USE THIS TO GET THE NUMBER OF MODES */
	Count         UMETA(DisplayName = "Count")
};
