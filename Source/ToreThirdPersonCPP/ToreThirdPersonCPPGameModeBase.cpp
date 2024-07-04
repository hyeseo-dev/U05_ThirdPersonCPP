#include "ToreThirdPersonCPPGameModeBase.h"
#include "Global.h"
#include "CHUD.h"

AToreThirdPersonCPPGameModeBase::AToreThirdPersonCPPGameModeBase()
{
	CHelpers::GetClass<APawn>(&DefaultPawnClass, "Blueprint'/Game/Player/BP_CPlayer.BP_CPlayer_C'");
	CHelpers::GetClass<AHUD>(&HUDClass, "Blueprint'/Game/Game/BP_HUD.BP_HUD_C'");
}
