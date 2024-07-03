#include "CDoAction_MagicBall.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CAttributeComponent.h"
#include "Components/CStateComponent.h"

void ACDoAction_MagicBall::BeginPlay()
{
	Super::BeginPlay();
}

void ACDoAction_MagicBall::DoAction()
{
	Super::DoAction();
	CheckFalse(Datas.Num() > 0);

	StateComp->SetActionMode();
	OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRate, Datas[0].StartSection);

	Datas[0].bCanMove ? AttributeComp->SetMove() : AttributeComp->SetStop();
}

void ACDoAction_MagicBall::Begin_DoAction()
{
	Super::Begin_DoAction();
}

void ACDoAction_MagicBall::End_DoAction()
{
	Super::End_DoAction();

	StateComp->SetIdleMode();
	AttributeComp->SetMove();
}
