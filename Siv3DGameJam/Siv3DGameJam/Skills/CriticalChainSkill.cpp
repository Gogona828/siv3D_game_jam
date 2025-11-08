#include "stdafx.h"
#include "CriticalChainSkill.h"
#include "GameData.h"
#include "Buff.h"
#include "BattleScene.h"

CriticalChainSkill::CriticalChainSkill()
    : m_name(U"クリティカルチェーン")
    , m_description(U"攻撃時に1.25倍バフ*2ターン")
    , m_mpCost(3)
    , m_skillType(SkillType::Buff)
{
}

s3d::String CriticalChainSkill::getName() const
{
    return m_name;
}

String CriticalChainSkill::getDescription() const
{
    return m_description;
}

int32 CriticalChainSkill::getMPCost() const
{
    return m_mpCost;
}

SkillType CriticalChainSkill::getSkillType() const
{
    return m_skillType;
}

bool CriticalChainSkill::hasPriority() const { return false; }

void CriticalChainSkill::execute(const SkillContext& context) const
{
    auto& playerInfo = context.gameData.infos();

    Buff buff;
    buff.name = getName();
    buff.type = BuffType::AttackMultiplier;
    buff.value = 1.25;
    buff.duration = 2;

    playerInfo.buffs.push_back(buff);

    // Spawn particle effect
    const Vec2 playerPos = context.battleScene.cpPlayerCharacterView.getCenterPos();
    context.battleScene.skillEffect.spawnBuffEffect(playerPos);
}
