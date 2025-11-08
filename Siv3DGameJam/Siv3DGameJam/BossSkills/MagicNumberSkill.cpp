#include "stdafx.h"
#include "MagicNumberSkill.h"
#include "GameData.h"
#include "Sequence.h"

MagicNumberSkill::MagicNumberSkill()
    : m_name(U"マジックナンバー")
    , m_description(U"攻撃*50~90%*10")
    , m_mpCost(0)
    , m_skillType(SkillType::Attack)
{
}

s3d::String MagicNumberSkill::getName() const
{
	return m_name;
}

s3d::String MagicNumberSkill::getDescription() const
{
	return m_description;
}

int32 MagicNumberSkill::getMPCost() const
{
	return m_mpCost;
}

SkillType MagicNumberSkill::getSkillType() const
{
	return m_skillType;
}

bool MagicNumberSkill::hasPriority() const
{
    return false;
}

void MagicNumberSkill::execute(const SkillContext& context) const
{
    auto& gameData = context.gameData;
    Array<double> multipliers = { 0.50, 0.60, 0.70, 0.80, 0.90 };
    int attackCount = 10;

    auto sequence = std::make_shared<Sequence>();
    for (int i = 0; i < attackCount; ++i)
    {
        double multiplier = multipliers.choice();
        int damage = static_cast<int>(gameData.bossInfos().attack * multiplier);
        sequence->addAction([&gameData, damage]()
        {
            gameData.applyPlayerDamage(damage);
        });
        sequence->addWait(0.2);
    }
    sequence->startAutoShared();
}
