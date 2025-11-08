#pragma once
#include <Siv3D.hpp>
#include "SkillEnums.h"

// Forward declarations to avoid circular dependencies
class GameData;
class BattleScene;

struct SkillContext {
	GameData& gameData;
	BattleScene& battleScene;
	int32 slotIndex;
};

struct ISkill {
	virtual ~ISkill() = default;
	virtual s3d::String getName() const = 0;
	virtual s3d::String getDescription() const = 0;
	virtual void execute(const SkillContext& context) const = 0;
	virtual bool hasPriority() const = 0;
	virtual int32 getMPCost() const = 0;
	virtual SkillType getSkillType() const = 0;
};
