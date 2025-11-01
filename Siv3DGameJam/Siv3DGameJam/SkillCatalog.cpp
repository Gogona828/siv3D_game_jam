#include "SkillCatalog.h"
#include <Siv3D.hpp>
using namespace s3d;

bool SkillCatalog::loadCSV(const FilePath& csvPath)
{
	rows.clear();
	keepCols.sort();

	const CSV csv(csvPath);
	if (not csv) return false;

	const int32 yStart = 1;
	const int32 rowCount = csv.rows();

	for (int i = yStart; i < rowCount; i++)
	{
		SkillRow skillRow;
		for (const auto j : keepCols)
		{
			if (j < csv.columns(i))
			{
				skillRow.columns << csv[i][j].trimmed();
			}
		}

		if (keepCols.includes(0)) skillRow.name = csv[i][0].trimmed();
		if (keepCols.includes(1)) skillRow.descriptionText = csv[i][1].trimmed();
		if (keepCols.includes(4)) skillRow.genre = csv[i][4].trimmed();
		if (keepCols.includes(5)) skillRow.abstructTargetEvent = csv[i][5].trimmed();

		if (skillRow.name.isEmpty()) continue;

		skillRow.key = skillRow.name + U"." + skillRow.genre;
		rows[skillRow.key] = std::move(skillRow);
	}

    return true;
}

const SkillRow* SkillCatalog::findKey(const String& key) const
{
	if (!rows.contains(key)) return nullptr;
	return &rows.at(key);
}

Array<String> SkillCatalog::keys() const
{
	Array<String> _keys;
	_keys.reserve(rows.size());
	for (auto& [_key, _] : rows)
	{
		_keys << _key;
	}
	return _keys;
}
