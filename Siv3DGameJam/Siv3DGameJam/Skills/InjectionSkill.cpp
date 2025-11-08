#include "stdafx.h"
#include "InjectionSkill.h"
#include "GameData.h"
#include "BattleScene.h"
#include "Buff.h"

InjectionSkill::InjectionSkill()
	: m_name(U"インジェクション")
	, m_description(U"保全性*30% 敵の行動に挟まれていた場合、ブレーク3")
	, m_mpCost(3)
	, m_skillType(SkillType::Attack)
{
}

s3d::String InjectionSkill::getName() const
{
	return m_name;
}

s3d::String InjectionSkill::getDescription() const
{
	return m_description;
}

int32 InjectionSkill::getMPCost() const
{
	return m_mpCost;
}

SkillType InjectionSkill::getSkillType() const
{
	return m_skillType;
}

bool InjectionSkill::hasPriority() const
{
    return false;
}

void InjectionSkill::execute(const SkillContext& context) const
{
    auto& gameData = context.gameData;
    auto& playerInfo = gameData.infos();

    double attackMultiplier = 1.0;
    for (const auto& buff : playerInfo.buffs)
    {
        if (buff.type == BuffType::AttackMultiplier)
        {
            attackMultiplier *= buff.value;
        }
    }

    int damage = static_cast<int>(playerInfo.integrity * 0.30 * attackMultiplier);
    gameData.applyBossDamage(damage);

    int breakDamage = 0;
    if (context.slotIndex > 0 && context.slotIndex < 3)
    {
        const auto& leftSkillKey = context.battleScene.slotKeys[context.slotIndex - 1];
        const auto& rightSkillKey = context.battleScene.slotKeys[context.slotIndex + 1];
        if (leftSkillKey.starts_with(U"Boss") && rightSkillKey.starts_with(U"Boss"))
        {
            breakDamage = 3;
        }
    }
    if (breakDamage > 0)
    {
        gameData.applyBossBreakDamage(breakDamage);
    }
}
