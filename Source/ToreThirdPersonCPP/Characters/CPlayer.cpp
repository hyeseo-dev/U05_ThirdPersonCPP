#include "CPlayer.h"
#include "Global.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Components/CAttributeComponent.h"
#include "Components/COptionComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/CActionComponent.h"
#include "Blueprint/UserWidget.h"
#include "Actions/CActionData.h"
#include "Actions/CAction.h"


ACPlayer::ACPlayer()
{
	//Enable Tick
	PrimaryActorTick.bCanEverTick = true;

	// Create Scene Component
	CHelpers::CreateSceneComponent(this, &SpringArmComp, "SpringArmComp", GetMesh());
	CHelpers::CreateSceneComponent(this, &CameraComp, "CameraComp", SpringArmComp);

	//Create Actor Component
	CHelpers::CreateActorComponent(this, &ActionComp, "ActionComp");
	CHelpers::CreateActorComponent(this, &MontagesComp, "MontagesComp");
	CHelpers::CreateActorComponent(this, &AttributeComp, "AttributeComp");
	CHelpers::CreateActorComponent(this, &OptionComp, "OptionComp");
	CHelpers::CreateActorComponent(this, &StateComp, "StateComp");


	TSubclassOf<UUserWidget> WeaponWidgetClass;
	CHelpers::GetClass(&WeaponWidgetClass, "/Game/Widgets/WB_WeaponIcon");
	

	WeaponWidget = CreateWidget(GetWorld(), WeaponWidgetClass);

	//Component Settings
	//-> MeshComp
	GetMesh()->SetRelativeLocation(FVector(0, 0, -88));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	USkeletalMesh* MeshAsset;
	CHelpers::GetAsset(&MeshAsset, "/Game/Character/Mesh/SK_Mannequin");
	GetMesh()->SetSkeletalMesh(MeshAsset);

	TSubclassOf<UAnimInstance> AnimInstanceClass;
	CHelpers::GetClass<UAnimInstance>(&AnimInstanceClass, "/Game/Player/ABP_CPlayer");
	GetMesh()->SetAnimInstanceClass(AnimInstanceClass);

	//-> SpringArmComp
	SpringArmComp->SetRelativeLocation(FVector(0, 0, 140));
	SpringArmComp->SetRelativeRotation(FRotator(0, 90, 0));
	SpringArmComp->TargetArmLength = 200.0f;
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->bEnableCameraLag = true;

	//-> Movmement
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->MaxWalkSpeed = AttributeComp->GetSprintSpeed();
	GetCharacterMovement()->bOrientRotationToMovement = true;

	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);
}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	StateComp->OnStateTypeChanged.AddDynamic(this, &ACPlayer::OnStateTypeChanged);

	UMaterialInstanceConstant* BodyMaterialAsset;
	UMaterialInstanceConstant* LogoMaterialAsset;

	CHelpers::GetAssetDynamic(&BodyMaterialAsset, "/Game/Character/Materials/MI_UE4Man_Body");
	CHelpers::GetAssetDynamic(&LogoMaterialAsset, "/Game/Character/Materials/M_UE4Man_ChestLogo");

	BodyMaterial = UMaterialInstanceDynamic::Create(BodyMaterialAsset, this);
	LogoMaterial = UMaterialInstanceDynamic::Create(LogoMaterialAsset, this);

	GetMesh()->SetMaterial(0, BodyMaterial);
	GetMesh()->SetMaterial(1, LogoMaterial);

	if (WeaponWidget != nullptr)
	{
		WeaponWidget->AddToViewport();
		WeaponWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	ActionComp->SetUnarmedMode();
}

void ACPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACPlayer::ChangeBodyColor(FLinearColor InColor)
{
	BodyMaterial->SetVectorParameterValue("BodyColor", InColor);
	LogoMaterial->SetVectorParameterValue("BodyColor", InColor);
}

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACPlayer::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACPlayer::OnMoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ACPlayer::OnTurn);
	PlayerInputComponent->BindAxis("LookUp", this, &ACPlayer::OnLookUp);
	PlayerInputComponent->BindAxis("Zoom", this, &ACPlayer::OnZoom);

	PlayerInputComponent->BindAction("Walk", EInputEvent::IE_Pressed, this, &ACPlayer::OnWalk);
	PlayerInputComponent->BindAction("Walk", EInputEvent::IE_Released, this, &ACPlayer::OffWalk);
	PlayerInputComponent->BindAction("Evade", EInputEvent::IE_Pressed, this, &ACPlayer::OnEvade);
	PlayerInputComponent->BindAction("Fist", EInputEvent::IE_Pressed, this, &ACPlayer::OnFist);
	PlayerInputComponent->BindAction("OneHand", EInputEvent::IE_Pressed, this, &ACPlayer::OnOneHand);
	PlayerInputComponent->BindAction("TwoHand", EInputEvent::IE_Pressed, this, &ACPlayer::OnTwoHand);
	PlayerInputComponent->BindAction("MagicBall", EInputEvent::IE_Pressed, this, &ACPlayer::OnMagicBall);
	PlayerInputComponent->BindAction("Warp", EInputEvent::IE_Pressed, this, &ACPlayer::OnWarp);
	PlayerInputComponent->BindAction("Whirlwind", EInputEvent::IE_Pressed, this, &ACPlayer::OnWhirlWind);

	PlayerInputComponent->BindAction("PrimaryAction", EInputEvent::IE_Pressed, this, &ACPlayer::OnPrimaryAction);
	PlayerInputComponent->BindAction("SecondaryAction", EInputEvent::IE_Pressed, this, &ACPlayer::OnSecondaryAction);
	PlayerInputComponent->BindAction("SecondaryAction", EInputEvent::IE_Released, this, &ACPlayer::OffSecondaryAction);
	PlayerInputComponent->BindAction("ChangeWeapon", EInputEvent::IE_Pressed, this, &ACPlayer::OnChangeWeapon);
	PlayerInputComponent->BindAction("ChangeWeapon", EInputEvent::IE_Released, this, &ACPlayer::OffChangeWeapon);

}

FGenericTeamId ACPlayer::GetGenericTeamId() const
{
	return FGenericTeamId(TeamID);
}

void ACPlayer::OnMoveForward(float Axis)
{
	CheckFalse(AttributeComp->IsCanMove());

	FRotator ControlRotation = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(ControlRotation).GetForwardVector();

	AddMovementInput(direction, Axis);
}

void ACPlayer::OnMoveRight(float Axis)
{
	CheckFalse(AttributeComp->IsCanMove());

	FRotator ControlRotation = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(ControlRotation).GetRightVector();

	AddMovementInput(direction, Axis);
}

void ACPlayer::OnTurn(float Axis)
{
	float Rate = Axis* OptionComp->GetMouseXRate()* GetWorld()->GetDeltaSeconds();

	AddControllerYawInput(Rate);
}

void ACPlayer::OnLookUp(float Axis)
{
	float Rate = Axis * OptionComp->GetMouseYRate() * GetWorld()->GetDeltaSeconds();

	AddControllerPitchInput(Rate);
}

void ACPlayer::OnZoom(float Axis)
{
	float Rate = Axis * OptionComp->GetZoomSpeed() * GetWorld()->GetDeltaSeconds();

	SpringArmComp->TargetArmLength += Rate;
	SpringArmComp->TargetArmLength = FMath::Clamp(SpringArmComp->TargetArmLength, OptionComp->GetZoomMin(), OptionComp->GetZoomMax());
}

void ACPlayer::OnWalk()
{
	GetCharacterMovement()->MaxWalkSpeed = AttributeComp->GetWalkSpeed();
}

void ACPlayer::OffWalk()
{
	GetCharacterMovement()->MaxWalkSpeed = AttributeComp->GetSprintSpeed();
}

void ACPlayer::OnEvade()
{
	CheckFalse(StateComp->IsIdleMode());
	CheckFalse(AttributeComp->IsCanMove());

	if (InputComponent->GetAxisValue("MoveForward") < 0.f)
	{

		StateComp->SetBackstepMode();
		return;
	}

	StateComp->SetRollMode();
}

void ACPlayer::OnFist()
{
	CheckFalse(StateComp->IsIdleMode());

	ActionComp->SetFistMode();
}

void ACPlayer::OnOneHand()
{
	CheckFalse(StateComp->IsIdleMode());

	ActionComp->SetOneHandMode();
}

void ACPlayer::OnTwoHand()
{
	CheckFalse(StateComp->IsIdleMode());

	ActionComp->SetTwoHandMode();
}

void ACPlayer::OnMagicBall()
{
	CheckFalse(StateComp->IsIdleMode());

	ActionComp->SetMagicBallMode();
}

void ACPlayer::OnWarp()
{
	CheckFalse(StateComp->IsIdleMode());

	ActionComp->SetWarpMode();
}

void ACPlayer::OnWhirlWind()
{
	CheckFalse(StateComp->IsIdleMode());

	ActionComp->SetWhirlWindMode();
}

void ACPlayer::OnPrimaryAction()
{
	ActionComp->DoAction();
}

void ACPlayer::OnSecondaryAction()
{
	ActionComp->DoSubAction(true);
}

void ACPlayer::OffSecondaryAction()
{
	ActionComp->DoSubAction(false);
}

void ACPlayer::OnChangeWeapon()
{
	CheckFalse(StateComp->IsIdleMode());

	WeaponWidget->SetVisibility(ESlateVisibility::Visible);
	APlayerController* APC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	APC->bShowMouseCursor = true;
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.1f);
}

void ACPlayer::OffChangeWeapon()
{
	WeaponWidget->SetVisibility(ESlateVisibility::Hidden);
	APlayerController* APC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	APC->bShowMouseCursor = false;
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
}

void ACPlayer::Begin_Roll()
{
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	FVector Start = GetActorLocation();
	FVector Target;

	if (GetVelocity().IsNearlyZero())
	{
		Target = Start + CameraComp->GetForwardVector().GetSafeNormal2D();
	}
	else
	{
		Target = Start + GetVelocity().GetSafeNormal2D();
	}

	FRotator ForceRotation = UKismetMathLibrary::FindLookAtRotation(Start, Target);
	SetActorRotation(ForceRotation);

	MontagesComp->PlayRoll();
}

void ACPlayer::Begin_Backstep()
{
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	MontagesComp->PlayBackstep();
}

void ACPlayer::End_Roll()
{
	if (ActionComp->GetCurrentActionDataAsset() == nullptr)
	{
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
	else if (ActionComp->GetCurrentActionDataAsset()->EquipmentData.bLookForward == true)
	{
		bUseControllerRotationYaw = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}

	StateComp->SetIdleMode();
}

void ACPlayer::End_Backstep()
{
	if (ActionComp->GetCurrentActionDataAsset() == nullptr)
	{
		bUseControllerRotationYaw = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}
	else if (ActionComp->GetCurrentActionDataAsset()->EquipmentData.bLookForward == false)
	{
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}

	StateComp->SetIdleMode();
}

void ACPlayer::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
	case EStateType::Roll:
	{
		Begin_Roll();
	}
	break;
	case EStateType::Backstep:
	{
		Begin_Backstep();
	}
	break;
	}
}

