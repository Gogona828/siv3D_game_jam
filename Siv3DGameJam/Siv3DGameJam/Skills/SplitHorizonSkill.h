#pragma once
#include "ISkill.h"
#include "SkillEnums.h"

class SplitHorizonSkill : public ISkill
{
public:
	SplitHorizonSkill();

	    s3d::String getName() const override;
	    s3d::String getDescription() const override;
	    int32 getMPCost() const override;
	    SkillType getSkillType() const override;
	    bool hasPriority() const override;
	
	    void execute(const SkillContext& context) const override;
private:
	String m_name;
	String m_description;
	int32 m_mpCost;
	SkillType m_skillType;
};
