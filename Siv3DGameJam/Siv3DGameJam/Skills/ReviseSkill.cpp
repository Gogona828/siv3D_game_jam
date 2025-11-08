#include "stdafx.h"
#include "ReviseSkill.h"
#include "GameData.h"
#include "BattleScene.h"

ReviseSkill::ReviseSkill()
	: m_name(U"リバイズ")
	, m_description(U"保守性*50%回復")
	, m_mpCost(2)
	, m_skillType(SkillType::Heal)
{
}

s3d::String ReviseSkill::getName() const
{
	return m_name;
}

String ReviseSkill::getDescription() const
{
	return m_description;
}

int32 ReviseSkill::getMPCost() const
{
	return m_mpCost;
}

SkillType ReviseSkill::getSkillType() const
{
	return m_skillType;
}

bool ReviseSkill::hasPriority() const { return false; }

void ReviseSkill::execute(const SkillContext& context) const
{
    auto& gameData = context.gameData;
    int healAmount = static_cast<int>(gameData.infos().serviceability * 0.50);
    gameData.applyPlayerHeal(healAmount);

    // Spawn particle effect
    const Vec2 playerPos = context.battleScene.cpPlayerCharacterView.getCenterPos();
    context.battleScene.skillEffect.spawnHealEffect(playerPos);
}
