#include "stdafx.h"
#include "FIX.h"
#include "GameData.h"
#include "Sequence.h"
#include "Buff.h"

FIX::FIX()
    : m_name(U"FIX")
    , m_description(U"保全性×50%")
    , m_mpCost(2)
    , m_skillType(SkillType::Attack)
{
}

s3d::String FIX::getName() const
{
    return m_name;
}

s3d::String FIX::getDescription() const
{
    return m_description;
}

int32 FIX::getMPCost() const
{
    return m_mpCost;
}

SkillType FIX::getSkillType() const
{
    return m_skillType;
}

bool FIX::hasPriority() const
{
    return false;
}

void FIX::execute(const SkillContext& context) const
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

    int damage = static_cast<int>(playerInfo.integrity * 0.50 * attackMultiplier);

    auto sequence = std::make_shared<Sequence>();
    sequence->addAction([&gameData, damage]()
    {
        gameData.applyBossDamage(damage);
    });
    sequence->addWait(0.6);
    sequence->startAutoShared();
}
