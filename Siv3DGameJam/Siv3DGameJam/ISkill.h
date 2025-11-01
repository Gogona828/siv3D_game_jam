#pragma once
#include <Siv3D.hpp>

struct GameState { };
struct SkillContext { s3d::String invokerId; };

struct ISkill {
	virtual ~ISkill() = default;
	virtual s3d::String name() const = 0;
	virtual void execute() = 0;
};
