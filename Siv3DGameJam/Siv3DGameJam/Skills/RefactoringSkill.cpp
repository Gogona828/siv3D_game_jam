#include "stdafx.h"
#include "RefactoringSkill.h"
#include "GameData.h"
#include "BattleScene.h"

RefactoringSkill::RefactoringSkill()
	: m_name(U"リファクタリング")
	, m_description(U"MP回復+10")
	, m_mpCost(0)
	, m_skillType(SkillType::Heal)
{
}

s3d::String RefactoringSkill::getName() const
{
	return m_name;
}

String RefactoringSkill::getDescription() const
{
	return m_description;
}

int32 RefactoringSkill::getMPCost() const
{
	return m_mpCost;
}

SkillType RefactoringSkill::getSkillType() const
{
	return m_skillType;
}

bool RefactoringSkill::hasPriority() const { return false; }

void RefactoringSkill::execute(const SkillContext& context) const
{
    auto& playerInfo = context.gameData.infos();
    playerInfo.mp = Min(playerInfo.maxMp, playerInfo.mp + 10);

    // Spawn particle effect
    const Vec2 playerPos = context.battleScene.cpPlayerCharacterView.getCenterPos();
    context.battleScene.skillEffect.spawnHealEffect(playerPos);
}
