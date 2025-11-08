#include "stdafx.h"
#include "ChecksumSkill.h"
#include "GameData.h"
#include "Buff.h"
#include "BattleScene.h"

ChecksumSkill::ChecksumSkill()
	: m_name(U"チェックサム")
	, m_description(U"安全性*50% ダメージ軽減")
	, m_mpCost(1)
	, m_skillType(SkillType::Defense)
{
}

s3d::String ChecksumSkill::getName() const
{
	return m_name;
}

String ChecksumSkill::getDescription() const
{
	return m_description;
}

int32 ChecksumSkill::getMPCost() const
{
	return m_mpCost;
}

SkillType ChecksumSkill::getSkillType() const
{
	return m_skillType;
}

bool ChecksumSkill::hasPriority() const { return false; }

void ChecksumSkill::execute(const SkillContext& context) const
{
	auto& playerInfo = context.gameData.infos();

	double reductionValue = playerInfo.security * 0.50;

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
