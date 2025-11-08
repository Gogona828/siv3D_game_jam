#include "stdafx.h"
#include "RainbowAttackSkill.h"
#include "GameData.h"
#include "Sequence.h"

RainbowAttackSkill::RainbowAttackSkill()
	: m_name(U"レインボー攻撃")
	, m_description(U"保全性*10% 7回攻撃")
	, m_mpCost(3)
	, m_skillType(SkillType::Attack)
{
}

s3d::String RainbowAttackSkill::getName() const
{
	return m_name;
}

String RainbowAttackSkill::getDescription() const
{
	return m_description;
}

int32 RainbowAttackSkill::getMPCost() const
{
	return m_mpCost;
}

SkillType RainbowAttackSkill::getSkillType() const
{
	return m_skillType;
}

bool RainbowAttackSkill::hasPriority() const { return false; }

void RainbowAttackSkill::execute(const SkillContext& context) const
{
    auto& gameData = context.gameData;
    int damage = static_cast<int>(gameData.infos().integrity * 0.10);
    int attackCount = 7;

    auto sequence = std::make_shared<Sequence>();
    for (int i = 0; i < attackCount; ++i)
    {
        sequence->addAction([&gameData, damage]()
        {
            gameData.applyBossDamage(damage);
        });
        sequence->addWait(0.2);
    }
    sequence->startAutoShared();
}
