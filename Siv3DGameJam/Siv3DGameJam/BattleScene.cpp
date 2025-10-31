# include "BattleScene.h"
# include "DropZone.h"
# include "PlayerCharacterView.h"
# include "BehaviorInfoView.h"
# include "BossCharacterView.h"
# include "GameData.h"
# include <Siv3D.hpp>

using namespace s3d;

BattleScene::BattleScene(const InitData& init) : IScene(init)
{
	cpDropZone.configZone(0, { U"json" });
	cpDropZone.configZone(1, { U"json" });
	cpDropZone.configZone(2, { U"json" });
	cpDropZone.configZone(3, { U"json" });

	GameData& gameData = GameData::getInstance();
	gameData.rebuildPlayerInfo();
	cpPlayerCharacterView.setMaxStatus(gameData.infos().reliability, gameData.infos().availability);
}

void BattleScene::update()
{
	cpDropZone.update();
}

void BattleScene::draw() const
{
	cpBossCharacterView.draw();
	RectF(Arg::bottomCenter(Scene::Width()/2, Scene::Height()), Scene::Width(), 200).draw(ColorF{ 0, 0, 0, 0.4 });
	cpDropZone.draw();
	cpPlayerCharacterView.draw();
	cpBehaviorInfoView.draw();
}
