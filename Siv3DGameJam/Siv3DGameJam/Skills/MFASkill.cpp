#include "stdafx.h"
#include "MFASkill.h"
#include "GameData.h"
#include "Buff.h"
#include "BattleScene.h"

MFASkill::MFASkill()
	: m_name(U"MFA")
	, m_description(U"安全性*50%×2ターン ダメージ軽減")
	, m_mpCost(4)
	, m_skillType(SkillType::Defense)
{
}

s3d::String MFASkill::getName() const
{
	return m_name;
}

String MFASkill::getDescription() const
{
	return m_description;
}

int32 MFASkill::getMPCost() const
{
	return m_mpCost;
}

SkillType MFASkill::getSkillType() const
{
	return m_skillType;
}

bool MFASkill::hasPriority() const { return false; }

void MFASkill::execute(const SkillContext& context) const
{
	auto& playerInfo = context.gameData.infos();

	double reductionValue = playerInfo.security * 0.50;

	Buff buff;
	buff.name = getName();
	buff.type = BuffType::DamageReduction;
	buff.value = reductionValue;
	buff.duration = 2;

	playerInfo.buffs.push_back(buff);

	// Spawn particle effect
	const Vec2 playerPos = context.battleScene.cpPlayerCharacterView.getCenterPos();
	context.battleScene.skillEffect.spawnDefenseEffect(playerPos);
}
