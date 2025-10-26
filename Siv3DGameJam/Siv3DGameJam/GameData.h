#pragma once
class GameData
{
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
		characterStatus = { 0,0,0,0,0,0 };
	}
private:
	GameData() = default;
	CharacterStatus characterStatus;
		
};

