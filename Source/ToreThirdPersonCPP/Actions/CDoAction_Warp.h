#pragma once

#include "CoreMinimal.h"
#include "Actions/CDoAction.h"
#include "CDoAction_Warp.generated.h"


UCLASS()
class TORETHIRDPERSONCPP_API ACDoAction_Warp : public ACDoAction
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	virtual void DoAction() override;
	virtual void Begin_DoAction() override;
	virtual void End_DoAction() override;

private:
	bool GetCursorLocationAndRotation(FVector& OutLocation, FRotator& OutRotation);
	void SetPreviewMeshColor(FLinearColor InColor);
	bool IsPlayerClass();

private:
	USkeletalMeshComponent* PreviewMeshComp;

	FVector Location;
};
