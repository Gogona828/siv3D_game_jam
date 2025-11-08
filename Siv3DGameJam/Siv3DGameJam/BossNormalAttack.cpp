#include "BossNormalAttack.h"
#include "Sequence.h"
#include "GameData.h"

void BossNormalAttack::execute(const SkillContext& context) const
{
	auto sequence = std::make_shared<Sequence>();
	sequence->addAction([this]()
	{
		action();
	});
	sequence->addWait(0.6);

	sequence->startAutoShared();
}

void BossNormalAttack::action() const
{
	// Use GameData from the context to apply damage
	GameData::getInstance().applyPlayerDamage(100);
}
