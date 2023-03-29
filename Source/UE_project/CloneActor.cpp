// Fill out your copyright notice in the Description page of Project Settings.


#include "CloneActor.h"
#include "Runtime/Engine/Classes/Engine/World.h"


AActor* UCloneActor::DuplicateActor(AActor* InputActor) // Note: put a U before the name of your library
{
	FActorSpawnParameters Params;
	Params.Template = InputActor;
	auto* Transform = &InputActor->GetTransform();
	auto ActorClass = InputActor->GetClass();
	AActor* Result = InputActor->GetWorld()->SpawnActor(ActorClass, Transform, Params);
	
	
	return Result;
}
