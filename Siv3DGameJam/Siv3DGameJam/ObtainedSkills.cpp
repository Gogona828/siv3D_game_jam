#include "stdafx.h"
#include "ObtainedSkills.h"

bool ObtainedSkills::add(const String& skillName)
{
	return m_skillNames.insert(skillName).second;
}

bool ObtainedSkills::has(const String& skillName) const
{
	return m_skillNames.count(skillName) > 0;
}

const std::unordered_set<String>& ObtainedSkills::getSkillNames() const
{
	return m_skillNames;
}

void ObtainedSkills::clear()
{
    m_skillNames.clear();
}
