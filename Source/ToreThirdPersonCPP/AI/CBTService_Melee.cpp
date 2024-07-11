#include "CBTService_Melee.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CPlayer.h"
#include "Components/CBehaviorComponent.h"
#include "Components/CStateComponent.h"

UCBTService_Melee::UCBTService_Melee()
{
	NodeName = "Root_Melee";
}

void UCBTService_Melee::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	ACAIController* AIC = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNull(AIC);

	UCBehaviorComponent* BehaviorComp = CHelpers::GetComponent<UCBehaviorComponent>(AIC);
	
	ACEnemy_AI* EnemyPawn = Cast<ACEnemy_AI>(AIC->GetPawn());
	CheckNull(EnemyPawn);
	UCStateComponent* StateComp = CHelpers::GetComponent<UCStateComponent>(EnemyPawn);

	//Is Hitted
	if (StateComp->IsHittedMode())
	{
		BehaviorComp->SetHittedMode();
		return;
	}

	//Try Get PlayerKey From BB
	ACPlayer* Player = BehaviorComp->GetPlayerKey();

	//Not Detected Player
	if (Player == nullptr)
	{
		BehaviorComp->SetWaitMode();
		return;
	}

	//Detected Player
	float Distance = EnemyPawn->GetDistanceTo(Player);

	if (Distance < AIC->GetBehaviorRange())
	{
		BehaviorComp->SetActionMode();
		return;
	}

	if (Distance < AIC->GetSightRadius())
	{
		BehaviorComp->SetApproachMode();
	}
}