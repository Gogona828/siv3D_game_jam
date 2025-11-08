#include "stdafx.h"
#include "PhantomReadSkill.h"
#include "GameData.h"

int PhantomReadSkill::s_usageCount = 0;

PhantomReadSkill::PhantomReadSkill()
    : m_name(U"ファントムリード")
    , m_description(U"攻撃*20% から始まり、使用される事に10%アップ")
    , m_mpCost(0)
    , m_skillType(SkillType::Attack)
{
}

s3d::String PhantomReadSkill::getName() const
{
	return m_name;
}

s3d::String PhantomReadSkill::getDescription() const
{
	return m_description;
}

int32 PhantomReadSkill::getMPCost() const
{
	return m_mpCost;
}

SkillType PhantomReadSkill::getSkillType() const
{
	return m_skillType;
}

bool PhantomReadSkill::hasPriority() const
{
    return false;
}

void PhantomReadSkill::execute(const SkillContext& context) const
{
    auto& gameData = context.gameData;
    double multiplier = 0.20 + (s_usageCount * 0.10);
    int damage = static_cast<int>(gameData.bossInfos().attack * multiplier);
    gameData.applyPlayerDamage(damage);

    s_usageCount++;
}
