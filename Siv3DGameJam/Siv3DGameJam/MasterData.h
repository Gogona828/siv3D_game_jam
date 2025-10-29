#pragma once
#include <Siv3D.hpp>
class MasterData
{
public:
	static MasterData& getInstance()
	{
		static MasterData instance;
		return instance;
	}
	static String getSkillName(int skillId)
	{
		const HashTable<int, String> skillNames = {
			{ 1, U"攻撃" },
			{ 2, U"二回攻撃" },
			{ 3, U"最強攻撃" },
		};
		if (skillNames.contains(skillId))
		{
			return skillNames.at(skillId);
		}
		else
		{
			return U"不明なスキル";
		}
	}
private:
	MasterData() = default;
};

