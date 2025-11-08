#include "stdafx.h"
#include "SkillFileIO.h"

namespace SkillFileIO
{
	String getExtensionForSkillType(SkillType type)
	{
		switch (type)
		{
		case SkillType::Attack: return U".atk";
		case SkillType::Defense: return U".def";
		case SkillType::Heal: return U".heal";
		case SkillType::Buff: return U".buff";
		default: return U".skill";
		}
	}

	bool saveSkill(const ISkill& skill, const FilePath& dir)
	{
		FileSystem::CreateDirectories(dir);
		const String extension = getExtensionForSkillType(skill.getSkillType());
		const FilePath outPath = dir + skill.getName() + extension;
		TextWriter w(outPath);
		if (!w) return false;

		w.writeln(U"スキル名: " + skill.getName());
		w.writeln(U"スキル説明: " + skill.getDescription());
		w.writeln(U"消費MP: " + ToString(skill.getMPCost()));
		return true;
	}

	String keyFromDroppedFile(const FilePath& path)
	{
		return FileSystem::BaseName(path);
	}
}
