#include "stdafx.h"
#include "DeadlockSkill.h"
#include "GameData.h"

DeadlockSkill::DeadlockSkill()
    : m_name(U"デッドロック")
    , m_description(U"攻撃*20% 攻撃が1ダメージ以上入った時、次のプレイヤーの行動をキャンセルする")
    , m_mpCost(0)
    , m_skillType(SkillType::Attack)
{
}

s3d::String DeadlockSkill::getName() const
{
	return m_name;
}

s3d::String DeadlockSkill::getDescription() const
{
	return m_description;
}

int32 DeadlockSkill::getMPCost() const
{
	return m_mpCost;
}

SkillType DeadlockSkill::getSkillType() const
{
	return m_skillType;
}

bool DeadlockSkill::hasPriority() const
{
    return false;
}

void DeadlockSkill::execute(const SkillContext& context) const
{
    auto& gameData = context.gameData;
    int damage = static_cast<int>(gameData.bossInfos().attack * 0.20);
    
    // Apply damage
    int playerHpBefore = gameData.getPlayerHP();
    gameData.applyPlayerDamage(damage);
    int playerHpAfter = gameData.getPlayerHP();

    // If at least 1 damage is dealt, cancel player's next action
    if (playerHpBefore > playerHpAfter)
    {
        gameData.infos().cancelPlayerNextAction = true;
    }
}
