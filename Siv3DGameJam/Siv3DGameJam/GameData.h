#pragma once
class GameData
{
private:
	GameData() = default;

public:
	static GameData& getInstance()
	{
		static GameData instance;
		return instance;
	}
	struct CharacterStatus
	{
		int Reliability;
		int Availability;
		int Serviceability;
		int Integrity;
		int Security;

		int Overload;
	};

	//データ

	//データリセット
	void reset()
	{
		// ゲームデータの初期化処理
		//Print << U"[デバッグ]GameDataのリセットを実行";
	}
		
};

