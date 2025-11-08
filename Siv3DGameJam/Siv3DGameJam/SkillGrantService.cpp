#include "stdafx.h"
#include "SkillGrantService.h"

SkillGrantService& SkillGrantService::getInstance()
{
	static SkillGrantService instance;
	return instance;
}

void SkillGrantService::configure(SkillCatalog* catalog, ObtainedSkills* obtained)
{
	m_catalog = catalog;
	m_obtained = obtained;
}

bool SkillGrantService::isReady() const
{
	return m_catalog != nullptr && m_obtained != nullptr;
}

bool SkillGrantService::grantSkill(const String& skillName)
{
	if (!isReady()) return false;

	const ISkill* skill = m_catalog->find(skillName);
	if (!skill) return false;

	const bool fileSaved = SkillFileIO::saveSkill(*skill);
	const bool addedToList = m_obtained->add(skillName);

	return fileSaved && addedToList;
}

void SkillGrantService::resetSkillFolder()
{
	const FilePath dir = U"assets/skill/";
	if (FileSystem::Exists(dir))
	{
		// The second argument `true` is for recursive deletion.
		FileSystem::Remove(dir, s3d::AllowUndo::No);
	}
	FileSystem::CreateDirectories(dir);
}
