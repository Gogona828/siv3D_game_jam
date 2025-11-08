#pragma once
#include <Siv3D.hpp>
#include <memory>
#include <unordered_map>
#include "ISkill.h"

class SkillCatalog
{
public:
	SkillCatalog();
	~SkillCatalog();

	bool load();
	const ISkill* find(const String& name) const;
	const std::unordered_map<String, std::unique_ptr<ISkill>>& getSkills() const;

private:
	std::unordered_map<String, std::unique_ptr<ISkill>> m_skills;
};
