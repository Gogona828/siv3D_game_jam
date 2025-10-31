#pragma once
#include <Siv3D.hpp>

struct PlayerCharacterInfo
{
	int32 pcTextureId = 0;
	int32 reliability = 0;
	int32 availability = 0;
	int32 serviceability = 0;
	int32 integrity = 0;
	int32 security = 0;
};

struct BossCharacterInfo
{
	int32 hp = 1234567;
	int32 breakValue = 3;
};

struct ResultInfo
{
	int32 elapsedTurn = 0;
};

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

	const PlayerCharacterInfo& infos() const noexcept { return pcInfo; }
	PlayerCharacterInfo& infos() noexcept { return pcInfo; }

	const BossCharacterInfo& bossInfos() const noexcept { return bossInfo; }
	BossCharacterInfo& bossInfos() noexcept { return bossInfo; }

	inline int32 Remap(int32 x, double min, double max, bool isClamp = false)
	{
		double t = (x - -100) / (100 - -100);
		if (isClamp)
		{
			t = Clamp(t, 0.0, 1.0);
		}
		return (int32)(min + t * (max - min));
	}

	void rebuildPlayerInfo()
	{
		pcInfo.reliability = Remap(characterStatus.Reliability, 100, 10000);
		pcInfo.availability = Remap(characterStatus.Availability, 0, 20);
		pcInfo.serviceability = Remap(characterStatus.Serviceability, 0, 2000);
		pcInfo.integrity = Remap(characterStatus.Integrity, 0, 10000);
		pcInfo.security = Remap(characterStatus.Security, 0, 100);
	}
private:
	GameData() = default;
	PlayerCharacterInfo pcInfo;
	BossCharacterInfo bossInfo;
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
