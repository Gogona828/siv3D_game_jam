#include "stdafx.h"
#include "SkillContainer.h"

SkillContainer& SkillContainer::getInstance() {
    static SkillContainer instance;
    return instance;
}

void SkillContainer::configure(SkillCatalog* catalog, ObtainedSkills* obtained) {
    m_catalog = catalog;
    m_obtained = obtained;
}

const ISkill* SkillContainer::getSkill(const String& skillName) const {
    if (m_catalog && isSkillAvailable(skillName)) {
        return m_catalog->find(skillName);
    }
    // Also check for boss skills, which are not in ObtainedSkills
    if (m_catalog && skillName.starts_with(U"Boss"))
    {
		return m_catalog->find(skillName);
    }
    return nullptr;
}

bool SkillContainer::isSkillAvailable(const String& skillName) const {
    return m_obtained && m_obtained->has(skillName);
}

bool SkillContainer::addObtainedSkill(const String& skillName) {
    if (m_obtained) {
        return m_obtained->add(skillName);
    }
    return false;
}
