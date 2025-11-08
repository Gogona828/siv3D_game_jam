#include "stdafx.h"
#include "LogicalBombSkill.h"
#include "GameData.h"
#include "Buff.h"

LogicalBombSkill::LogicalBombSkill()
	: m_name(U"論理爆弾")
	, m_description(U"保全性*20% ブレーク2")
	, m_mpCost(3)
	, m_skillType(SkillType::Attack)
{
}

s3d::String LogicalBombSkill::getName() const
{
	return m_name;
}

s3d::String LogicalBombSkill::getDescription() const
{
	return m_description;
}

int32 LogicalBombSkill::getMPCost() const
{
	return m_mpCost;
}

SkillType LogicalBombSkill::getSkillType() const
{
	return m_skillType;
}

bool LogicalBombSkill::hasPriority() const
{
    return false;
}

void LogicalBombSkill::execute(const SkillContext& context) const
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
	int damage = static_cast<int>(playerInfo.integrity * 0.20 * attackMultiplier);

	// Apply damage and break
	gameData.applyBossDamage(damage);
	gameData.applyBossBreakDamage(2);
}
