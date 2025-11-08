#include "stdafx.h"
#include "FirewallSkill.h"
#include "GameData.h"
#include "Buff.h"
#include "BattleScene.h"

FirewallSkill::FirewallSkill()
	: m_name(U"ファイアウォール")
	, m_description(U"安全性*80% ダメージ軽減")
	, m_mpCost(2)
	, m_skillType(SkillType::Defense)
{
}

s3d::String FirewallSkill::getName() const
{
	return m_name;
}

String FirewallSkill::getDescription() const
{
	return m_description;
}

int32 FirewallSkill::getMPCost() const
{
	return m_mpCost;
}

SkillType FirewallSkill::getSkillType() const
{
	return m_skillType;
}

bool FirewallSkill::hasPriority() const { return false; }

void FirewallSkill::execute(const SkillContext& context) const
{
	auto& playerInfo = context.gameData.infos();

	double reductionValue = playerInfo.security * 0.80;

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
