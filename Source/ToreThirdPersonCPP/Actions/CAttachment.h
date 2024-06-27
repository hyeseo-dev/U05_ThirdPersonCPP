#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CAttachment.generated.h"

class ACharacter;

UCLASS()
class TORETHIRDPERSONCPP_API ACAttachment : public AActor
{
	GENERATED_BODY()

public:
	ACAttachment();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnEquip();

	UFUNCTION(BlueprintImplementableEvent)
	void OnUnequip();

protected:
	UFUNCTION(BlueprintCallable)
	void ActionAttachTo(FName InSocketName);

private:
	UPROPERTY(VisibleDefaultsOnly)
	USceneComponent* RootComp;

protected:
	UPROPERTY(BlueprintReadOnly)
	ACharacter* OwnerCharacter;
};
