#include "stdafx.h"
#include "Raid5Skill.h"
#include "GameData.h"
#include "Buff.h"
#include "BattleScene.h"

Raid5Skill::Raid5Skill()
	: m_name(U"RAID 5")
	, m_description(U"安全性*10%×5ターン ダメージ軽減")
	, m_mpCost(2)
	, m_skillType(SkillType::Defense)
{
}

s3d::String Raid5Skill::getName() const
{
	return m_name;
}

String Raid5Skill::getDescription() const
{
	return m_description;
}

int32 Raid5Skill::getMPCost() const
{
	return m_mpCost;
}

SkillType Raid5Skill::getSkillType() const
{
	return m_skillType;
}

bool Raid5Skill::hasPriority() const { return false; }

void Raid5Skill::execute(const SkillContext& context) const
{
	auto& playerInfo = context.gameData.infos();

	double reductionValue = playerInfo.security * 0.10;

	Buff buff;
	buff.name = getName();
	buff.type = BuffType::DamageReduction;
	buff.value = reductionValue;
	buff.duration = 5;

	playerInfo.buffs.push_back(buff);

	// Spawn particle effect
	const Vec2 playerPos = context.battleScene.cpPlayerCharacterView.getCenterPos();
	context.battleScene.skillEffect.spawnDefenseEffect(playerPos);
}
