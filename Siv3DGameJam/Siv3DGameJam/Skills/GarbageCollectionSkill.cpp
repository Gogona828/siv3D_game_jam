#include "stdafx.h"
#include "GarbageCollectionSkill.h"
#include "GameData.h"
#include "BattleScene.h"

GarbageCollectionSkill::GarbageCollectionSkill()
	: m_name(U"ガベージコレクション")
	, m_description(U"MP回復+5")
	, m_mpCost(0)
	, m_skillType(SkillType::Heal)
{
}

s3d::String GarbageCollectionSkill::getName() const
{
	return m_name;
}

s3d::String GarbageCollectionSkill::getDescription() const
{
	return m_description;
}

int32 GarbageCollectionSkill::getMPCost() const
{
	return m_mpCost;
}

SkillType GarbageCollectionSkill::getSkillType() const
{
	return m_skillType;
}

bool GarbageCollectionSkill::hasPriority() const
{
    return false;
}

void GarbageCollectionSkill::execute(const SkillContext& context) const
{
    auto& playerInfo = context.gameData.infos();
    playerInfo.mp = Min(playerInfo.maxMp, playerInfo.mp + 5);

    // Spawn particle effect
    const Vec2 playerPos = context.battleScene.cpPlayerCharacterView.getCenterPos();
    context.battleScene.skillEffect.spawnHealEffect(playerPos);
}
