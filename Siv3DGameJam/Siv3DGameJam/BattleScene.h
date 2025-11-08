#pragma once
# include <Siv3D.hpp>
# include "Common.h" 
# include "GameData.h"
# include "VolumeSlider.h"
# include "MasterData.h"
# include "AudioManager.h"
# include "DropZone.h"
# include "PlayerCharacterView.h"
# include "BehaviorInfoView.h"
# include "BossCharacterView.h"
#include "ClickEffect.h"
#include "Sequence.h"
# include "SkillEffect.h"
# include "BossSkillManager.h"

enum class BattleState
{
	Boot,
	BoxReset,
	EnemyActArrangement,
	WaitAlign,
	SequentialProcess,
	GameOver,
	GameClear,
};

class BattleScene : public App::Scene
{
private:
	//追加---
	s3d::Audio m_clickSE;
	s3d::Audio m_bgMusic; // タイトルBGM用

	ClickEffect clickEffect;
	//追加---

	const Font font{ FontMethod::MSDF, 48 };

	DropZone cpDropZone;
	BehaviorInfoView cpBehaviorInfoView;
	BossCharacterView cpBossCharacterView;

public:
	PlayerCharacterView cpPlayerCharacterView;
private:

	Image glitchImage;
	Texture glitchTexture;

	Vec2 uvOffset{ 0, 0 }; // UVスクロール用

	BattleState currentBattleState;

	String defaultPath = U"assets/maingame/battle/battle_druganddrop.png";
	String bossBackPath = U"assets/maingame/battle/battle_bossborder.png";
	String bossFrontPath = U"assets/maingame/battle/battle_atkicon_white.png";

	Array<Array<int>> BossActionPattern =
	{
		{ 0, 1 },
		{ 0, 2 },
		{ 0, 3 },
		{ 1, 2 },
		{ 1, 3 },
		{ 2, 3 },
	};

	Sequence sequence;
	void Judge();

public:
	SkillEffect skillEffect;
	enum class State
	{
		WaitingFill,
		Executing,
		TurnEnd,
		Error,
	};

	int turnCount = 0;

	// 4つのスロットに、あとで実行するスキルキーを保持する
	Array<s3d::String> slotKeys{ U"", U"", U"", U"" };
	Array<s3d::String> previousSlotKeys{ U"", U"", U"", U"" };

	//explicit BattleScene(DropZone& dz);
	BattleScene(const InitData& init);
	void update() override;
	void draw() const override;
};
