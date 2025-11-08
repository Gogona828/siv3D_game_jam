#include "stdafx.h"
#include "SilverBulletSkill.h"
#include "GameData.h"

SilverBulletSkill::SilverBulletSkill()
	: m_name(U"シルバーバレット")
	, m_description(U"相手は死ぬ　※激レア")
	, m_mpCost(10)
	, m_skillType(SkillType::Attack)
{
}

s3d::String SilverBulletSkill::getName() const
{
	return m_name;
}

String SilverBulletSkill::getDescription() const
{
	return m_description;
}

int32 SilverBulletSkill::getMPCost() const
{
	return m_mpCost;
}

SkillType SilverBulletSkill::getSkillType() const
{
	return m_skillType;
}

bool SilverBulletSkill::hasPriority() const { return false; }

void SilverBulletSkill::execute(const SkillContext& context) const
{
	auto& gameData = context.gameData;

	// Deals a huge amount of damage
	gameData.applyBossDamage(100000000);
}
