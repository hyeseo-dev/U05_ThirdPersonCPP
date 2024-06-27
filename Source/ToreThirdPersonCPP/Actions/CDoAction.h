#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CDoAction.generated.h"

class ACharacter;
class UCAttributeComponent;
class UCStateComponent;

UCLASS()
class TORETHIRDPERSONCPP_API ACDoAction : public AActor
{
	GENERATED_BODY()
	
public:	
	ACDoAction();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:	
	virtual void DoAction() {};
	virtual void Begin_DoAction() {};
	virtual void End_DoAction() {};

protected:
	UPROPERTY(BlueprintReadOnly)
	ACharacter* OnwerCharacter;

	UPROPERTY(BlueprintReadOnly)
	UCAttributeComponent* AttributeComp;

	UPROPERTY(BlueprintReadOnly)
	UCStateComponent* StateComp;
};
