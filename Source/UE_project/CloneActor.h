// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CloneActor.generated.h"

/**
 * 
 */
UCLASS()
class UE_PROJECT_API UCloneActor : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	UFUNCTION(BlueprintCallable, Category = "ActorFuncions", meta = (WorldContext = WorldContextObject))
        static AActor* DuplicateActor(AActor* InputActor);
	
};
