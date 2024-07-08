#pragma once

#include "CoreMinimal.h"
#include "Actions/CDoAction.h"
#include "CDoAction_Whirlwind.generated.h"

class UParticleSystemComponent;
class UBoxComponent;
class ACharacter;


UCLASS()
class TORETHIRDPERSONCPP_API ACDoAction_Whirlwind : public ACDoAction
{
	GENERATED_BODY()

public:
	ACDoAction_Whirlwind();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	virtual void DoAction() override;
	virtual void Begin_DoAction() override;
	virtual void End_DoAction() override;

	virtual void Abort() override;

public:
	virtual void OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InCauser, ACharacter* InOtherCharacter) override;
	virtual void OnAttachmentEndOverlap(ACharacter* InAttacker, AActor* InCauser, ACharacter* InOtherCharacter) override;

private:
	UFUNCTION()
	void TickDamage();

	UFUNCTION()
	void Finish();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Whirlwind")
	float DamageToTime;

	UPROPERTY(EditDefaultsOnly, Category = "Whirlwind")
	float ActiveTime;

	UPROPERTY(EditDefaultsOnly, Category = "Whirlwind")
	float AroundSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Whirlwind")
	float Radius;

	UPROPERTY(VisibleInstanceOnly)
	UParticleSystemComponent* AttachedParticleComp;

private:
	UBoxComponent* BoxComp;
	bool bActivating;

	TArray<ACharacter*> HittedCharacters;
	float CurrentYaw;
};
