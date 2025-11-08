#include "stdafx.h"
#include "OverflowSkill.h"
#include "GameData.h"

OverflowSkill::OverflowSkill()
    : m_name(U"オーバーフロー")
    , m_description(U"攻撃*10% ブレーク2")
    , m_mpCost(0)
    , m_skillType(SkillType::Attack)
{
}

s3d::String OverflowSkill::getName() const
{
	return m_name;
}

s3d::String OverflowSkill::getDescription() const
{
	return m_description;
}

int32 OverflowSkill::getMPCost() const
{
	return m_mpCost;
}

SkillType OverflowSkill::getSkillType() const
{
	return m_skillType;
}

bool OverflowSkill::hasPriority() const
{
    return false;
}

void OverflowSkill::execute(const SkillContext& context) const
{
    auto& gameData = context.gameData;
    int damage = static_cast<int>(gameData.bossInfos().attack * 0.10);
    gameData.applyPlayerDamage(damage);
    gameData.applyPlayerBreakDamage(2);
}
