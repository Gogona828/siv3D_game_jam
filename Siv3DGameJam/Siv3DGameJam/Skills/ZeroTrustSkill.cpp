#include "stdafx.h"
#include "ZeroTrustSkill.h"
#include "GameData.h"
#include "Buff.h"
#include "BattleScene.h"

ZeroTrustSkill::ZeroTrustSkill()
	: m_name(U"ゼロトラスト")
	, m_description(U"自分のHPを3割削る 攻撃時に1.5倍バフ*3ターン")
	, m_mpCost(4)
	, m_skillType(SkillType::Buff)
{
}

s3d::String ZeroTrustSkill::getName() const
{
	return m_name;
}

String ZeroTrustSkill::getDescription() const
{
	return m_description;
}

int32 ZeroTrustSkill::getMPCost() const
{
	return m_mpCost;
}

SkillType ZeroTrustSkill::getSkillType() const
{
	return m_skillType;
}

bool ZeroTrustSkill::hasPriority() const { return false; }

void ZeroTrustSkill::execute(const SkillContext& context) const
{
    auto& gameData = context.gameData;
    auto& playerInfo = gameData.infos();

    // Sacrifice 30% of current HP
    int hpCost = static_cast<int>(playerInfo.reliability * 0.30);
    gameData.applyPlayerDamage(hpCost);

    // Apply buff
    Buff buff;
    buff.name = getName();
    buff.type = BuffType::AttackMultiplier;
    buff.value = 1.5;
    buff.duration = 3;

    playerInfo.buffs.push_back(buff);

    // Spawn particle effect
    const Vec2 playerPos = context.battleScene.cpPlayerCharacterView.getCenterPos();
    context.battleScene.skillEffect.spawnBuffEffect(playerPos);
}
