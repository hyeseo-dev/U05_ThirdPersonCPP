#include "ToreThirdPersonCPPGameModeBase.h"
#include "Global.h"

AToreThirdPersonCPPGameModeBase::AToreThirdPersonCPPGameModeBase()
{
	CHelpers::GetClass<APawn>(&DefaultPawnClass, "Blueprint'/Game/Player/BP_CPlayer.BP_CPlayer_C'");
}
