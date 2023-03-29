
#include "InteractableObjectBase.h"

AInteractableObjectBase::AInteractableObjectBase()
{
	SetReplicates(true);
	bNetLoadOnClient = true;
}

void AInteractableObjectBase::OnBeginInteract_Implementation(AActor* interactionEmitter)
{
	
}

void AInteractableObjectBase::OnEndInteract_Implementation(AActor* interactionEmitter)
{
	
}

