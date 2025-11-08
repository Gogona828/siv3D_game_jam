#include "stdafx.h"
#include "RollbackSkill.h"
#include "GameData.h"
#include "BattleScene.h"
#include "SkillContainer.h"

RollbackSkill::RollbackSkill()
	: m_name(U"ロールバック")
	, m_description(U"前の自分の行動をもう一度行う")
	, m_mpCost(5)
	, m_skillType(SkillType::Buff)
{
}

s3d::String RollbackSkill::getName() const
{
	return m_name;
}

String RollbackSkill::getDescription() const
{
	return m_description;
}

int32 RollbackSkill::getMPCost() const
{
	return m_mpCost;
}

SkillType RollbackSkill::getSkillType() const
{
	return m_skillType;
}

bool RollbackSkill::hasPriority() const { return false; }

void RollbackSkill::execute(const SkillContext& context) const
{
    const auto& previousSkillKey = context.battleScene.previousSlotKeys[context.slotIndex];

    if (not previousSkillKey.isEmpty() && previousSkillKey != getName())
    {
        if (const ISkill* previousSkill = SkillContainer::getInstance().getSkill(previousSkillKey))
        {
            // Note: This could still lead to complex loops, but it prevents direct self-calling.
            previousSkill->execute(context);
        }
    }

    // Spawn particle effect
    const Vec2 playerPos = context.battleScene.cpPlayerCharacterView.getCenterPos();
    context.battleScene.skillEffect.spawnBuffEffect(playerPos);
}
