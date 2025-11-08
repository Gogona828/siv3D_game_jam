#include "stdafx.h"
#include "HelloWorldSkill.h"
#include "GameData.h"
#include "Buff.h"

HelloWorldSkill::HelloWorldSkill()
	: m_name(U"HelloWorld")
	, m_description(U"保全性*1% ブレーク1")
	, m_mpCost(0)
	, m_skillType(SkillType::Attack)
{
}

s3d::String HelloWorldSkill::getName() const
{
	return m_name;
}

s3d::String HelloWorldSkill::getDescription() const
{
	return m_description;
}

int32 HelloWorldSkill::getMPCost() const
{
	return m_mpCost;
}

SkillType HelloWorldSkill::getSkillType() const
{
	return m_skillType;
}

bool HelloWorldSkill::hasPriority() const
{
    return false;
}

void HelloWorldSkill::execute(const SkillContext& context) const
{
	auto& gameData = context.gameData;
	auto& playerInfo = gameData.infos();

	double attackMultiplier = 1.0;
	for (const auto& buff : playerInfo.buffs)
	{
		if (buff.type == BuffType::AttackMultiplier)
		{
			attackMultiplier *= buff.value;
		}
	}

	// Calculate damage
	int damage = static_cast<int>(playerInfo.integrity * 0.01 * attackMultiplier);

	// Apply damage and break
	gameData.applyBossDamage(damage);
	gameData.applyBossBreakDamage(1);
}
