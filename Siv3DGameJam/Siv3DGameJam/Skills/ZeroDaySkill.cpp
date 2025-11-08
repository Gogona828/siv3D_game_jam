#include "stdafx.h"
#include "ZeroDaySkill.h"
#include "GameData.h"

ZeroDaySkill::ZeroDaySkill()
	: m_name(U"ゼロデイ")
	, m_description(U"保全性*10% どこに置いても行動の一番最初に割り込む ブレーク1~2")
	, m_mpCost(2)
	, m_skillType(SkillType::Attack)
{
}

s3d::String ZeroDaySkill::getName() const
{
	return m_name;
}

String ZeroDaySkill::getDescription() const
{
	return m_description;
}

int32 ZeroDaySkill::getMPCost() const
{
	return m_mpCost;
}

SkillType ZeroDaySkill::getSkillType() const
{
	return m_skillType;
}

bool ZeroDaySkill::hasPriority() const { return true; }

void ZeroDaySkill::execute(const SkillContext& context) const
{
	// TODO: Implement priority effect

	auto& gameData = context.gameData;

	// Calculate damage
	int damage = static_cast<int>(gameData.infos().integrity * 0.10);
	int breakDamage = Random(1, 2);

	// Apply damage and break
	gameData.applyBossDamage(damage);
	gameData.applyBossBreakDamage(breakDamage);
}
