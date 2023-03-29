// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Materials/MaterialInstance.h"
#include "PBStaticPermutation.generated.h"

UCLASS()
class PANORAMAKE_API UPBStaticPermutation : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
		UFUNCTION(BlueprintCallable, Category = "PanoraMake")
		static void SetStaticBool(UMaterialInstance* Mat, FName Name, bool Value);
};