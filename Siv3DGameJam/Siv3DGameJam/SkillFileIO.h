#pragma once
#include "SkillCatalog.h"

namespace SkillFileIO
{

	inline bool saveVisibleSkillFile(const SkillRow& row, const FilePath& dir = U"assets/skill/") {
		FileSystem::CreateDirectories(dir);
		const FilePath outPath = dir + row.key;
		TextWriter w(outPath);
		if (!w) return false;
		
		w.writeln(U"スキル名: " + row.name);
		w.writeln(U"スキル説明: " + row.descriptionText);
		return true;
	}

	// D&Dされたスキルファイルから key を復元（= ファイル名）
	inline String keyFromDroppedFile(const FilePath& path) {
		const auto base = FileSystem::BaseName(path);
		const auto ext = FileSystem::Extension(path);
		return base + U"." + ext;
	}

}
