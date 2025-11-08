#include "stdafx.h"
#include "DivisionByZeroSkill.h"
#include "GameData.h"
#include "Sequence.h"

DivisionByZeroSkill::DivisionByZeroSkill()
    : m_name(U"Division by Zero.")
    , m_description(U"攻撃*60%*3回")
    , m_mpCost(0)
    , m_skillType(SkillType::Attack)
{
}

s3d::String DivisionByZeroSkill::getName() const
{
	return m_name;
}

s3d::String DivisionByZeroSkill::getDescription() const
{
	return m_description;
}

int DivisionByZeroSkill::getMPCost() const
{
	return m_mpCost;
}

SkillType DivisionByZeroSkill::getSkillType() const
{
	return m_skillType;
}

bool DivisionByZeroSkill::hasPriority() const
{
    return false;
}

void DivisionByZeroSkill::execute(const SkillContext& context)const
{
    auto& gameData = context.gameData;
    int damage = static_cast<int>(gameData.bossInfos().attack * 0.60);
    int attackCount = 3;

    auto sequence = std::make_shared<Sequence>();
    for (int i = 0; i < attackCount; ++i)
    {
        sequence->addAction([&gameData, damage]()
        {
            gameData.applyPlayerDamage(damage);
        });
        sequence->addWait(0.2);
    }
    sequence->startAutoShared();
}
