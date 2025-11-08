#pragma once
#include "ISkill.h"
#include <unordered_set>

class ObtainedSkills
{
public:
	bool add(const String& skillName);
	bool has(const String& skillName) const;
	const std::unordered_set<String>& getSkillNames() const;
    void clear();

private:
	std::unordered_set<String> m_skillNames;
};
