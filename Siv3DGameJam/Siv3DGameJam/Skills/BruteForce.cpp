#include "stdafx.h"
#include "BruteForce.h"
#include "GameData.h"
#include "Sequence.h"
#include "Buff.h"

BruteForce::BruteForce()
    : m_name(U"ブルートフォース")
    , m_description(U"保全性×15% 3~5回攻撃")
    , m_mpCost(2)
    , m_skillType(SkillType::Attack)
{
}

s3d::String BruteForce::getName() const
{
    return m_name;
}
s3d::String BruteForce::getDescription() const
{
    return m_description;
}

int32 BruteForce::getMPCost() const
{
    return m_mpCost;
}

SkillType BruteForce::getSkillType() const
{
    return m_skillType;
}

bool BruteForce::hasPriority() const
{
    return false;
}

void BruteForce::execute(const SkillContext& context) const
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

    int damage = static_cast<int>(playerInfo.integrity * 0.15 * attackMultiplier);
    int attackCount = Random(3, 5);

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
