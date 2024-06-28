#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/CCharacterInterface.h"
#include "CEnemy.generated.h"

class UCAttributeComponent;
class UCStateComponent;
class UCMontagesComponent;
class UCActionComponent;
class UWidgetComponent;

UCLASS()
class TORETHIRDPERSONCPP_API ACEnemy : public ACharacter, public ICCharacterInterface
{
	GENERATED_BODY()

public:
	ACEnemy();

protected:
	virtual void BeginPlay() override;

public:
	void ChangeBodyColor(FLinearColor InColor) override;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Components")
	UCAttributeComponent* AttributeComp;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Components")
	UCStateComponent* StateComp;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Components")
	UCMontagesComponent* MontagesComp;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Components")
	UCActionComponent* ActionComp;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Components")
	UWidgetComponent* NameWidgetComp;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Components")
	UWidgetComponent* HealthWidgetComp;

private:
	UMaterialInstanceDynamic* BodyMaterial;
	UMaterialInstanceDynamic* LogoMaterial;
};
