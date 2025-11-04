#pragma once
#include "ISkill.h"
using namespace s3d;

class BossNormalAttack : public ISkill
{
	String name() const override
	{
		return U"BossNormalAttack.atk";

	}
	void execute() override;

private:
	void action();
};
