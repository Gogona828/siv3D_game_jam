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

class BattleScene : public App::Scene
{
private:
	const Font font{ FontMethod::MSDF, 48 };

	DropZone cpDropZone;
	PlayerCharacterView cpPlayerCharacterView;
	BehaviorInfoView cpBehaviorInfoView;
	BossCharacterView cpBossCharacterView;

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

