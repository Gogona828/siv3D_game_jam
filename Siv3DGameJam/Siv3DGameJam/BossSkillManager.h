#pragma once
#include <Siv3D.hpp>

struct BossSkillData
{
	String name;
	String description;
	String specification;
	String genre;
	int hpThreshold; // HP for selection
};

class BossSkillManager
{
public:
	static BossSkillManager& getInstance()
	{
		static BossSkillManager instance;
		return instance;
	}

	void loadBossSkills(const String& csvFilePath);

	Array<String> getAvailableSkills(double currentHpPercent) const;

private:
	BossSkillManager() = default;
	Array<BossSkillData> m_bossSkills;
};
