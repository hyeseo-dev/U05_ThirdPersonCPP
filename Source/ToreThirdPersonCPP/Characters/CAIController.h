#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CAIController.generated.h"

class ACEnemy_AI;
class UCBehaviorComponent;
class UAIPerceptionComponent;
class UAISenseConfig_Sight;

UCLASS()
class TORETHIRDPERSONCPP_API ACAIController : public AAIController
{
	GENERATED_BODY()

public:
	ACAIController();

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

public:
	virtual void Tick(float DelaTime) override;

public:
	float GetSightRadius();
	FORCEINLINE float GetBehaviorRange() { return BehaviorRange; }

private:
	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

protected:
	UPROPERTY(VisibleDefaultsOnly)
	UAIPerceptionComponent* PerceptionComp;

	UPROPERTY(VisibleDefaultsOnly)
	UCBehaviorComponent* BehaviorComp;

private:
	UPROPERTY(EditAnywhere)
	float BehaviorRange;

private:
	ACEnemy_AI* OwnerEnemy;
	
	UAISenseConfig_Sight* Sight;
};
