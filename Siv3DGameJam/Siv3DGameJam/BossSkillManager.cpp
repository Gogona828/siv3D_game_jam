#include "BossSkillManager.h"
#include <Siv3D.hpp>

void BossSkillManager::loadBossSkills(const String& csvFilePath)
{
	CSV csv(csvFilePath);

	if (!csv)
	{
		// ファイルが開けなかった場合のエラー処理
		Logger << U"Failed to open BossSkill.csv";
		return;
	}

	// ヘッダー行をスキップ
	for (int i = 1; i < csv.rows(); ++i)
	{
		BossSkillData skill;
		skill.name = csv[i][0]; // タスク
		skill.description = csv[i][1]; // ゲーム中のスキル説明文
		skill.specification = csv[i][2]; // 仕様指示
		skill.genre = csv[i][3]; // ジャンル

		// 抽選HPのパース
		String hpThresholdStr = csv[i][4];
		if (hpThresholdStr.ends_with(U"%~"))
		{
			hpThresholdStr.pop_back(); // '~' を削除
			hpThresholdStr.pop_back(); // '%' を削除
		}
		else if (hpThresholdStr.ends_with(U"%"))
		{
			hpThresholdStr.pop_back(); // '%' を削除
		}
		
		skill.hpThreshold = Parse<int>(hpThresholdStr);

		m_bossSkills.push_back(skill);
	}
}

Array<String> BossSkillManager::getAvailableSkills(double currentHpPercent) const
{
	Array<String> availableSkills;
	for (const auto& skill : m_bossSkills)
	{
		if (currentHpPercent <= skill.hpThreshold)
		{
			availableSkills.push_back(skill.name);
		}
	}
	return availableSkills;
}
