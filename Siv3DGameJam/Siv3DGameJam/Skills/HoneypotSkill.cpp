#include "stdafx.h"
#include "HoneypotSkill.h"
#include "GameData.h"
#include "BattleScene.h"

HoneypotSkill::HoneypotSkill()
	: m_name(U"ハニーポット")
	, m_description(U"保守性*120%回復")
	, m_mpCost(4)
	, m_skillType(SkillType::Heal)
{
}

s3d::String HoneypotSkill::getName() const
{
	return m_name;
}

s3d::String HoneypotSkill::getDescription() const
{
	return m_description;
}

int32 HoneypotSkill::getMPCost() const
{
	return m_mpCost;
}

SkillType HoneypotSkill::getSkillType() const
{
	return m_skillType;
}

bool HoneypotSkill::hasPriority() const
{
    return false;
}

void HoneypotSkill::execute(const SkillContext& context) const
{
    auto& gameData = context.gameData;
    int healAmount = static_cast<int>(gameData.infos().serviceability * 1.20);
    gameData.applyPlayerHeal(healAmount);

    // Spawn particle effect
    const Vec2 playerPos = context.battleScene.cpPlayerCharacterView.getCenterPos();
    context.battleScene.skillEffect.spawnHealEffect(playerPos);
}
