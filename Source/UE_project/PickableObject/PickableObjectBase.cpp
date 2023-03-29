#include "PickableObjectBase.h"

APickableObjectBase::APickableObjectBase()
{
	SetReplicates(true);
	bNetLoadOnClient = true;
}

void APickableObjectBase::OnPickedUp_Implementation(AActor* playerHowPickup)
{
	m_isPickedUp = true;
	m_actorHowPickedMeUp = playerHowPickup;
}

void APickableObjectBase::OnInteracted_Implementation(const TScriptInterface<IIInteractableObject>& interactableObject) {}

void APickableObjectBase::OnDropped_Implementation(AActor* playerHowDrop, bool fromInteraction)
{
	m_isPickedUp = false;
	m_actorHowPickedMeUp = nullptr;
}

void APickableObjectBase::DropMe_Implementation()
{
	IIPickableObject::DropMe_Implementation();
}
