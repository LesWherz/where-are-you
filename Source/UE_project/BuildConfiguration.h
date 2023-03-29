// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BuildConfiguration.generated.h"

UENUM()
enum class EConfigurationType : uint8
{
	UEditor,
	Build_Debug,
	Build_Release
};

/**
 * 
 */
UCLASS()
class UE_PROJECT_API UBuildConfiguration : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	/*
	    Return the FString of the current configuration
	    - TEST : UE4 Editor
	    - DEVELOPMENT : Build in debug
	    - SHIPPING : Build in release
	*/
	UFUNCTION(Category = "Utility", BlueprintCallable, meta=(ExpandEnumAsExecs="Configuration"))
	static void GetBuildConfiguration(EConfigurationType& Configuration);
};
