#include "stdafx.h"
#include "SplitHorizonSkill.h"
#include "GameData.h"
#include "BattleScene.h"
#include "Buff.h"

SplitHorizonSkill::SplitHorizonSkill()
	: m_name(U"スプリットホライズン")
	, m_description(U"安全性*100% ダメージ軽減")
	, m_mpCost(3)
	, m_skillType(SkillType::Defense)
{
}

s3d::String SplitHorizonSkill::getName() const
{
	return m_name;
}

s3d::String SplitHorizonSkill::getDescription() const
{
	return m_description;
}

int32 SplitHorizonSkill::getMPCost() const
{
	return m_mpCost;
}

SkillType SplitHorizonSkill::getSkillType() const
{
	return m_skillType;
}

bool SplitHorizonSkill::hasPriority() const { return false; }

void SplitHorizonSkill::execute(const SkillContext& context) const
{
	auto& playerInfo = context.gameData.infos();

	double reductionValue = playerInfo.security * 1.00;

	Buff buff;
	buff.name = getName();
	buff.type = BuffType::DamageReduction;
	buff.value = reductionValue;
	buff.duration = 1; // Lasts for 1 turn

	playerInfo.buffs.push_back(buff);

	// Spawn particle effect
	const Vec2 playerPos = context.battleScene.cpPlayerCharacterView.getCenterPos();
	context.battleScene.skillEffect.spawnDefenseEffect(playerPos);
}
