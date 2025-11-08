#include "stdafx.h"
#include "SelfRepairSkill.h"
#include "GameData.h"
#include "BattleScene.h"

SelfRepairSkill::SelfRepairSkill()
	: m_name(U"自己修復")
	, m_description(U"保守性*80%回復")
	, m_mpCost(3)
	, m_skillType(SkillType::Heal)
{
}

s3d::String SelfRepairSkill::getName() const
{
	return m_name;
}

String SelfRepairSkill::getDescription() const
{
	return m_description;
}

int32 SelfRepairSkill::getMPCost() const
{
	return m_mpCost;
}

SkillType SelfRepairSkill::getSkillType() const
{
	return m_skillType;
}

bool SelfRepairSkill::hasPriority() const { return false; }

void SelfRepairSkill::execute(const SkillContext& context) const
{
    auto& gameData = context.gameData;
    int healAmount = static_cast<int>(gameData.infos().serviceability * 0.80);
    gameData.applyPlayerHeal(healAmount);

    // Spawn particle effect
    const Vec2 playerPos = context.battleScene.cpPlayerCharacterView.getCenterPos();
    context.battleScene.skillEffect.spawnHealEffect(playerPos);
}
