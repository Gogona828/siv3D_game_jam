#pragma once
#include <Siv3D.hpp>
#include <memory>
#include "ISkill.h"
#include "SkillCatalog.h"
#include "ObtainedSkills.h"

using namespace s3d;

class SkillContainer {
public:
    static SkillContainer& getInstance();

    void configure(SkillCatalog* catalog, ObtainedSkills* obtained);

    const ISkill* getSkill(const String& skillName) const;
    bool isSkillAvailable(const String& skillName) const;
    bool addObtainedSkill(const String& skillName);

private:
    SkillContainer() = default;

    SkillCatalog* m_catalog = nullptr;
    ObtainedSkills* m_obtained = nullptr;
};
