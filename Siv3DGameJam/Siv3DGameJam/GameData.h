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

		Array<int> toArray() const
		{
			return Array<int>
			{
				Reliability,
				Availability,
				Serviceability,
				Integrity,
				Security,
				Overload
			};
		}
	};

	//データ
	CharacterStatus characterStatus;
	Array<int> eventList;
	//データリセット
	void reset()
	{
		// ゲームデータの初期化処理
		//Print << U"[デバッグ]GameDataのリセットを実行";
		characterStatus.Reliability = 0;
		characterStatus.Availability = 0;	// 可用性
		characterStatus.Serviceability = 0;	// 保守性
		characterStatus.Integrity = 0;		// 保全性
		characterStatus.Security = 0;		// 安全性

		characterStatus.Overload = 0;

		eventList.clear();
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
