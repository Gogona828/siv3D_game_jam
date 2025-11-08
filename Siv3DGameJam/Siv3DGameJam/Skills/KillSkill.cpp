#include "stdafx.h"
#include "KillSkill.h"
#include "GameData.h"
#include "Buff.h"

KillSkill::KillSkill()
	: m_name(U"キル")
	, m_description(U"保全性*100%")
	, m_mpCost(4)
	, m_skillType(SkillType::Attack)
{
}

s3d::String KillSkill::getName() const
{
	return m_name;
}

s3d::String KillSkill::getDescription() const
{
	return m_description;
}

int32 KillSkill::getMPCost() const
{
	return m_mpCost;
}

SkillType KillSkill::getSkillType() const
{
	return m_skillType;
}

bool KillSkill::hasPriority() const
{
    return false;
}

void KillSkill::execute(const SkillContext& context) const
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
	int damage = static_cast<int>(playerInfo.integrity * 1.00 * attackMultiplier);

	// Apply damage
	gameData.applyBossDamage(damage);
}
