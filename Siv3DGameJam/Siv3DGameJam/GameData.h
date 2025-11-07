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
enum class StatusId
{
	Reliability,	// 0:信頼性
	Availability,	// 1:可用性
	Serviceability,	// 2:保守性
	Integrity,		// 3:保全性
	Security,		// 4:安全性
	Overload,		// 5:容量
	Unknown
};
enum class CharacterType
{
	Reliability,	// 0:信頼性
	Availability,	// 1:可用性
	Serviceability,	// 2:保守性
	Integrity,		// 3:保全性
	Security,		// 4:安全性
	Overload,		// 5:容量
	Origin,			// 6:未進化
	Evoluted,		// 7:一進化
	Bug				// 8:バグキャラ
};
struct BossCharacterInfo
{
	int32 hp = 1234567;
	int32 maxHp = 1234567;
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
	CharacterType evolutedCharacterTextureId;
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

	// ★★★ ここから追加 ★★★

	// --- HP/Damage Management ---
	// (BattleScene::Judge() や ISkill から使用)

	/**
	 * @brief プレイヤーの現在HPを取得します (信頼性)
	 */
	int getPlayerHP() const
	{
		// プレイヤーHPは "信頼性 (Reliability)" を使う
		return pcInfo.reliability;
	}

	/**
	 * @brief ボスの現在HPを取得します
	 */
	int getBossHP() const
	{
		return bossInfo.hp;
	}

	/**
	 * @brief プレイヤーにダメージを与えます (HPは0未満にならない)
	 * @param damage
	 */
	void applyPlayerDamage(int damage)
	{
		pcInfo.reliability -= damage;
		if (pcInfo.reliability < 0)
		{
			pcInfo.reliability = 0;
		}
	}

	/**
	 * @brief プレイヤーを回復します (最大HPを超えない)
	 */
	void applyPlayerHeal(int amount)
	{
		// ★ 修正: pcInfo.maxReliability -> pcInfo.maxHp
		Remap(characterStatus.Reliability + amount, m_playerMaxHp, m_playerMaxHp);
	}

	/**
	 * @brief ボスにダメージを与えます (HPは0未満にならない)
	 * @param damage
	 */
	void applyBossDamage(int damage)
	{
		bossInfo.hp -= damage;
		if (bossInfo.hp < 0)
		{
			bossInfo.hp = 0;
		}
	}

	/**
	 * @brief ボスを回復します (最大HPを超えない)
	 */
	void applyBossHeal(int amount)
	{
		bossInfo.hp = Min(bossInfo.maxHp, bossInfo.hp + amount);
	}

	// ★★★ ここまで追加 ★★★

	const PlayerCharacterInfo& infos() const noexcept { return pcInfo; }
	PlayerCharacterInfo& infos() noexcept { return pcInfo; }

	const BossCharacterInfo& bossInfos() const noexcept { return bossInfo; }
	BossCharacterInfo& bossInfos() noexcept { return bossInfo; }

	inline int32 Remap(int32 x, double min, double max, bool isClamp = false)
	{
		double t = (double)(x - -100) / (double)(100 - -100);
		if (isClamp)
		{
			t = Clamp(t, 0.0, 1.0);
		}
		return (int32)(min + t * (max - min));
	}

	void rebuildPlayerInfo()
	{
		pcInfo.reliability = Remap(characterStatus.Reliability, m_playerMaxHp, m_playerMaxHp);
		pcInfo.availability = Remap(characterStatus.Availability, 0, 20);
		pcInfo.serviceability = Remap(characterStatus.Serviceability, 0, 2000);
		pcInfo.integrity = Remap(characterStatus.Integrity, 0, 10000);
		pcInfo.security = Remap(characterStatus.Security, 0, 100);
	}

	// ★ 追加: BattleSceneがフラグを確認・リセットするための関数
	bool checkPlayerHitFlag()
	{
		if (m_playerJustHit)
		{
			m_playerJustHit = false;
			return true;
		}
		return false;
	}
	bool checkBossHitFlag()
	{
		if (m_bossJustHit)
		{
			m_bossJustHit = false;
			return true;
		}
		return false;
	}

private:
	GameData() = default;
	PlayerCharacterInfo pcInfo;
	BossCharacterInfo bossInfo;

	int m_playerMaxHp = 10000;
	int m_playerMinHp = 100;

	// ★ 追加: 点滅アニメーション用フラグ
	bool m_playerJustHit = false;
	bool m_bossJustHit = false;
};
