#include "stdafx.h"
#include "OverclockSkill.h"
#include "GameData.h"
#include "Buff.h"
#include "BattleScene.h"

OverclockSkill::OverclockSkill()
	: m_name(U"オーバークロック")
	, m_description(U"自分のHPを8割削る 攻撃時に2.0倍バフ*3ターン")
	, m_mpCost(4)
	, m_skillType(SkillType::Buff)
{
}

s3d::String OverclockSkill::getName() const
{
	return m_name;
}

String OverclockSkill::getDescription() const
{
	return m_description;
}

int32 OverclockSkill::getMPCost() const
{
	return m_mpCost;
}

SkillType OverclockSkill::getSkillType() const
{
	return m_skillType;
}

bool OverclockSkill::hasPriority() const { return false; }

void OverclockSkill::execute(const SkillContext& context) const
{
    auto& gameData = context.gameData;
    auto& playerInfo = gameData.infos();

    // Sacrifice 80% of current HP
    int hpCost = static_cast<int>(playerInfo.reliability * 0.80);
    gameData.applyPlayerDamage(hpCost);

    // Apply buff
    Buff buff;
    	buff.name = getName();    buff.type = BuffType::AttackMultiplier;
    buff.value = 2.0;
    buff.duration = 3;

    playerInfo.buffs.push_back(buff);

    // Spawn particle effect
    const Vec2 playerPos = context.battleScene.cpPlayerCharacterView.getCenterPos();
    context.battleScene.skillEffect.spawnBuffEffect(playerPos);
}
