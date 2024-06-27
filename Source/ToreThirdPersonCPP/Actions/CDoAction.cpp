#include "CDoAction.h"
#include "GameFramework/Character.h"
#include "Components/CAttributeComponent.h"
#include "Components/CStateComponent.h"

ACDoAction::ACDoAction()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACDoAction::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACDoAction::Tick(float DeltaTime)
{
}

