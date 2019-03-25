#include "GameInstanceBun.h"

UGameInstanceBun::UGameInstanceBun(){
	CountRestart = 0;
	CountJump = 0.0f;
	HP = 100.0f;
	HpPrec = 0.0f;
	JumpPrec = 0.0f;
}

void UGameInstanceBun::HpRate()
{
	HpPrec = (HP / 100);
}

void UGameInstanceBun::JumpRate()
{
	JumpPrec = (CountJump / 3);
}
