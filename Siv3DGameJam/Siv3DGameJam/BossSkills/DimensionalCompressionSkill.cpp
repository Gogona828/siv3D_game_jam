#include "stdafx.h"
#include "DimensionalCompressionSkill.h"
#include "GameData.h"

DimensionalCompressionSkill::DimensionalCompressionSkill()
    : m_name(U"次元圧縮")
    , m_description(U"攻撃*100%")
    , m_mpCost(0)
    , m_skillType(SkillType::Attack)
{
}

s3d::String DimensionalCompressionSkill::getName() const
{
	return m_name;
}

s3d::String DimensionalCompressionSkill::getDescription() const
{
	return m_description;
}

int32 DimensionalCompressionSkill::getMPCost() const
{
	return m_mpCost;
}

SkillType DimensionalCompressionSkill::getSkillType() const
{
	return m_skillType;
}

bool DimensionalCompressionSkill::hasPriority() const
{
    return false;
}

void DimensionalCompressionSkill::execute(const SkillContext& context) const
{
    auto& gameData = context.gameData;
    int damage = static_cast<int>(gameData.bossInfos().attack * 1.00);
    gameData.applyPlayerDamage(damage);
}
