#pragma once
#include "SkillCatalog.h"
#include "ObtainedSkills.h"
#include "SkillFileIO.h"
#include <filesystem>

using namespace s3d;

class SkillGrantService
{
public:
	static SkillGrantService& getInstance();

	void configure(SkillCatalog* catalog, ObtainedSkills* obtained);

	bool isReady() const;

	bool grantSkill(const String& skillName);

	void resetSkillFolder();

	SkillGrantService(const SkillGrantService&) = delete;
	SkillGrantService& operator=(const SkillGrantService&) = delete;

private:
	SkillGrantService() = default;
	~SkillGrantService() = default;

	SkillCatalog* m_catalog = nullptr;
	ObtainedSkills* m_obtained = nullptr;
};
