#pragma once

UENUM(BlueprintType, Category="WRU/Interactions")
enum class EObjectSizeState : uint8
{
	Small	 UMETA(DisplayName = "Small"),
	Normal	 UMETA(DisplayName = "Normal"),
	Big		 UMETA(DisplayName = "Big"),
	Resizing UMETA(DisplayName = "Resizing")
}; 