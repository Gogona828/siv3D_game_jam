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
			{ 1, U"FIX.atk" },
			{ 2, U"ブルートフォース.atk" },
			{ 3, U"ゼロデイ.atk" },
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
	// テクスチャIDからパスを取得
	static String getTexturePath(int textureId)
	{
		const HashTable<int, String> texturePaths = {
			{ 1, U"assets/maingame/chara_image/reliability_normal.png" },
			{ 2, U"assets/maingame/chara_image/availability_normal.png" },
			{ 3, U"assets/maingame/chara_image/serviceability_normal.png" },
			{ 4, U"assets/maingame/chara_image/integrity_normal.png" },
			{ 5, U"assets/maingame/chara_image/security_normal.png" },
			{100,U"assets/maingame/event_image/skill_atk.png"},
			{101,U"assets/maingame/event_image/skill_buff.png"},
			{102,U"assets/maingame/event_image/skill_def.png"}
		};
		if (texturePaths.contains(textureId))
		{
			return texturePaths.at(textureId);
		}
		else
		{
			return U"assets/maingame/chara_image/evolution_normal.png";
		}
	}
	// keyからテクスチャIDを取得(不整合時はNULLを返す)
	static int getTextureId(String key)
	{
		const HashTable<String, int> textureIds = {
			{ U"reliability", 1 },
			{ U"availability", 2 },
			{ U"serviceability", 3 },
			{ U"integrity", 4 },
			{ U"security", 5 },
			{ U"atk",100},
			{ U"buff",101 },
			{ U"def",102 },
		};
		if (textureIds.contains(key))
		{
			return textureIds.at(key);
		}
		else
		{
			Print << U"Error: テクスチャのKey不整合";
			return NULL;
		}
	}
	static float maxStatusValue()
	{
		return 100.0;
	}
private:
	MasterData() = default;
};

