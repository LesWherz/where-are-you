// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildConfiguration.h"

void UBuildConfiguration::GetBuildConfiguration(EConfigurationType& Configuration)
{

#if !UE_BUILD_SHIPPING
#if WITH_EDITOR
	Configuration = EConfigurationType::UEditor;
#else
	Configuration = EConfigurationType::Build_Debug;
#endif // WITH_EDITOR
#else
	Configuration = EConfigurationType::Build_Release;
#endif
}