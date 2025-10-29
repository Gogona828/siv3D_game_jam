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
	// テクスチャIDからパスを取得
	static String getTexturePath(int textureId)
	{
		const HashTable<int, String> texturePaths = {
			{ 1, U"assets/maingame/chara_image/reliability_normal.png" },
			{ 2, U"assets/maingame/chara_image/availability_normal.png" },
			{ 3, U"assets/maingame/chara_image/serviceability_normal.png" },
			{ 4, U"assets/maingame/chara_image/integrity_normal.png" },
			{ 5, U"assets/maingame/chara_image/security_normal.png" },
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

