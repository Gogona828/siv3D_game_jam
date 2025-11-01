#pragma once
# include "SkillCatalog.h"

class ObtainedSkills
{
public:
	bool add(const SkillRow& row) { return rows.emplace(row.key, row).second; }
	const SkillRow* getKey(const String& key) const {
		if (!rows.contains(key)) return nullptr;
		return &rows.at(key);
	}

	Array<const SkillRow*> allSkill() const {
		Array<const SkillRow*> out; out.reserve(rows.size());
		for (auto& [_, v] : rows) out << &v;
		return out;
	}

private:
	HashTable<String, SkillRow> rows;
};
