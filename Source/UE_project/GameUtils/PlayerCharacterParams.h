#pragma once

#include "CoreMinimal.h"

#include "PlayerCharacterParams.generated.h"

USTRUCT(BlueprintType)
struct UE_PROJECT_API FPlayerCharacterParams
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GravityScale = 1.6f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxAcceleration = 2000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxWalkSpeed = 550.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float JumpZVelocity = 595.0f;
};
