#pragma once
#include "ISkill.h"

class BossNormalAttack : public ISkill
{
public:
	s3d::String getName() const override
	{
		return U"BossNormalAttack.atk";

	}
	void execute(const SkillContext& context) const override;

	bool hasPriority() const override { return false; }

	int32 getMPCost() const override { return 0; }//

	s3d::String getDescription() const override { return (U"ボス通常攻撃"); }
	SkillType getSkillType() const override { return SkillType::Attack; }

private:
	void action() const;
};
