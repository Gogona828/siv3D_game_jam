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
			{ 4, U"インジェクション.atk" },
			{ 5, U"キル.atk" },
			{ 6, U"デプロイ.atk" },
			{ 7, U"論理爆弾.atk" },
			{ 8, U"チェックサム.atk" },
			{ 9, U"ファイアウォール.atk" },
			{ 10, U"デフラグ.atk" },
			{ 11, U"ガベージコレクション.atk" },
			{ 12, U"ロールバック.atk" },
			{ 13, U"自己修復.atk" },
			{ 14, U"ハニーポット.atk" },
			{ 15, U"ゼロトラスト.atk" },
			{ 16, U"ルートキット.atk" },
			{ 17, U"バックドア.atk" },
			{ 18, U"MFA.atk" },
			{ 19, U"リファクタリング.atk" },
			{ 20, U"クリティカルチェーン.atk" },
			{ 21, U"シルバーバレット.atk" },
			{ 22, U"レインボー攻撃.atk" },
			{ 23, U"オーバークロック.atk" },
			{ 24, U"スカイスクレイパー.atk" },
			{ 25, U"HelloWorld.atk" },
			{ 26, U"RAID 5.atk" },
			{ 27, U"スプリットホライズン.atk" },
			{ 28, U"リバイズ.atk" },
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

