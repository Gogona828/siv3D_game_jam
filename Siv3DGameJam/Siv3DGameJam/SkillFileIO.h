#pragma once
#include "ISkill.h"

namespace SkillFileIO
{
	String getExtensionForSkillType(SkillType type);
	bool saveSkill(const ISkill& skill, const FilePath& dir = U"assets/skill/");
	String keyFromDroppedFile(const FilePath& path);
}