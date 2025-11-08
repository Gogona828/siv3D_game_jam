#include "stdafx.h"
#include "DefragSkill.h"
#include "GameData.h"
#include "BattleScene.h"

DefragSkill::DefragSkill()
	: m_name(U"デフラグ")
	, m_description(U"MP回復+2")
	, m_mpCost(0)
	, m_skillType(SkillType::Heal)
{
}

s3d::String DefragSkill::getName() const
{
	return m_name;
}

s3d::String DefragSkill::getDescription() const
{
	return m_description;
}

int32 DefragSkill::getMPCost() const
{
	return m_mpCost;
}

SkillType DefragSkill::getSkillType() const
{
	return m_skillType;
}

bool DefragSkill::hasPriority() const
{
    return false;
}

void DefragSkill::execute(const SkillContext& context) const
{
    auto& playerInfo = context.gameData.infos();
    playerInfo.mp = Min(playerInfo.maxMp, playerInfo.mp + 2);

    // Spawn particle effect
    const Vec2 playerPos = context.battleScene.cpPlayerCharacterView.getCenterPos();
    context.battleScene.skillEffect.spawnHealEffect(playerPos);
}
