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
	const Font font{ FontMethod::MSDF, 48 };

	DropZone cpDropZone;
	PlayerCharacterView cpPlayerCharacterView;
	BehaviorInfoView cpBehaviorInfoView;
	BossCharacterView cpBossCharacterView;

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

public:
	enum class State
	{
		WaitingFill,
		Executing,
		TurnEnd,
		Error,
	};

	int turnCount = 0;

	//explicit BattleScene(DropZone& dz);
	BattleScene(const InitData& init);
	void update() override;
	void draw() const override;
};

