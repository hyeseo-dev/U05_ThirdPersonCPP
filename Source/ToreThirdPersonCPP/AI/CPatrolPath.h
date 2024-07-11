#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPatrolPath.generated.h"

class USplineComponent;
class UTextRenderComponent;

UCLASS()
class TORETHIRDPERSONCPP_API ACPatrolPath : public AActor
{
	GENERATED_BODY()
	
public:	
	ACPatrolPath();

	virtual void OnConstruction(const FTransform& Transform) override;

public:
	FORCEINLINE USplineComponent* GetSplineComp() { return SplineComp; }

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleDefaultsOnly)
	USceneComponent* RootComp;

	UPROPERTY(VisibleDefaultsOnly)
	USplineComponent* SplineComp;

	UPROPERTY(VisibleDefaultsOnly)
	UTextRenderComponent* TextRenderComp;

	UPROPERTY(EditInstanceOnly)
	bool bClosedLoop;
};
