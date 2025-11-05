#include "BossNormalAttack.h"
#include "Sequence.h"
#include "GameData.h"
using namespace s3d;

void BossNormalAttack::execute()
{
	auto sequence = std::make_shared<Sequence>();
	sequence->addAction([this]()
	{
		action();
	});
	sequence->addWait(0.6);

	sequence->startAutoShared();
}

void BossNormalAttack::action()
{
	Console << U"boss attack!!";
	GameData::getInstance().applyPlayerDamage(100);
}
