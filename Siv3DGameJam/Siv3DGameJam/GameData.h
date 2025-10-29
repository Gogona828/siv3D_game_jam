#pragma once
#include <Siv3D.hpp>
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
		int Reliability;	// 信頼性
		int Availability;	// 可用性
		int Serviceability;	// 保守性
		int Integrity;		// 保全性
		int Security;		// 安全性

		int Overload;
	};

	//データ
	CharacterStatus characterStatus;
	Array<String> eventList;
	//データリセット
	void reset()
	{
		// ゲームデータの初期化処理
		//Print << U"[デバッグ]GameDataのリセットを実行";
		characterStatus = { 0,0,0,0,0,50 };
	}
private:
	GameData() = default;
};

enum class StatusId
{
	Reliability,	// 0:信頼性
	Availability,	// 1:可用性
	Serviceability,	// 2:保守性
	Intergrity,		// 3:保全性
	Security,		// 4:安全性
	Overload		// 5:容量
};
