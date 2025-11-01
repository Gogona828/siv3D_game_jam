# include "BattleScene.h"
# include "DropZone.h"
# include "PlayerCharacterView.h"
# include "BehaviorInfoView.h"
# include "BossCharacterView.h"
# include "GameData.h"
# include "SkillGrantService.h"
# include "SkillContainer.h"
# include "FIX.h"
# include <Siv3D.hpp>

using namespace s3d;

BattleScene::BattleScene(const InitData& init) : IScene(init)
{
	cpDropZone.configZone(0, { U"atk", U"def", U"heal", U"buff", U"pdf"});
	cpDropZone.configZone(1, { U"atk", U"def", U"heal", U"buff", U"pdf" });
	cpDropZone.configZone(2, { U"atk", U"def", U"heal", U"buff", U"pdf" });
	cpDropZone.configZone(3, { U"atk", U"def", U"heal", U"buff", U"pdf" });

	GameData& gameData = GameData::getInstance();
	gameData.rebuildPlayerInfo();
	cpPlayerCharacterView.setMaxStatus(gameData.infos().reliability, gameData.infos().availability);

	// 元画像読み込み
	glitchImage = Image(U"assets/maingame/battle/glitch.png");
	glitchTexture = Texture(glitchImage);

	currentBattleState = BattleState::Boot;
}

void BattleScene::update()
{
	cpDropZone.update();

#pragma region Glitch
	//グリッチノイズ用---------------------------------------------
	// UVスクロール速度（右に0.5px、下に0.2px/フレーム）
	uvOffset.x += 1.0;
	uvOffset.y += 0.3;

	// 画像幅・高さでループ
	const int w = glitchTexture.width();
	const int h = glitchTexture.height();
	uvOffset.x = std::fmod(uvOffset.x, static_cast<double>(w));
	uvOffset.y = std::fmod(uvOffset.y, static_cast<double>(h));
	//グリッチノイズ用---------------------------------------------
#pragma endregion

	switch (currentBattleState)
	{
		case BattleState::Boot:
			// Init処理、あるなら
			// デバッグ用
			// SkillGrantService::getInstance().resetSkillFolder();
			// SkillGrantService::getInstance().grantByKey(U"FIX.atk");
			currentBattleState = BattleState::BoxReset;
			break;

		case BattleState::BoxReset:
			cpDropZone.resetAllSlots();
			for (int i = 0; i < 4; i++)
			{
				cpDropZone.setDropBoxTexture(i, defaultPath);
			}

			currentBattleState = BattleState::EnemyActArrangement;
			break;

		case BattleState::EnemyActArrangement:
			{
				int n = Random(0, 5);
				for (int i = 0; i < 2; i++)
				{
					cpDropZone.setDropBoxTexture(BossActionPattern[n][i], bossBackPath, bossFrontPath);
					cpDropZone.assignSlot(BossActionPattern[n][i], U"assets.pdf", false);
					// TODO: 行動登録
					SkillContainer::getInstance().registerSkill(U"FIX.atk");
				}
			}

			currentBattleState = BattleState::WaitAlign;
			break;

		case BattleState::WaitAlign:
			if (cpDropZone.allComplete())
			{
				currentBattleState = BattleState::SequentialProcess;
			}
			break;
		case BattleState::SequentialProcess:
			for (int i = 0; i < 4; i++)
			{
				// TODO: 行動
			}
			currentBattleState = BattleState::BoxReset;
			currentBattleState = BattleState::GameClear;
			currentBattleState = BattleState::GameOver;
			break;
		case BattleState::GameOver:
			break;
		case BattleState::GameClear:
			break;
	}
}

void BattleScene::draw() const
{
#pragma region Draw Background
	//グリッチノイズ用---------------------------------------------
	const int blockSize = 5; // スライスの高さ
	const int w = glitchTexture.width();
	const int h = glitchTexture.height();

	for (int y = 0; y < h; y += blockSize)
	{
		// ランダム横ずれ
		const int maxOffset = 20;
		int offsetX = Random(-maxOffset, maxOffset);

		// 描画元の UV
		int srcY = (y + static_cast<int>(uvOffset.y)) % h;
		int srcX = static_cast<int>(uvOffset.x) % w;

		int remainingW = w - srcX;

		// 右端に切れ端が出る場合、左端から追加描画
		if (remainingW >= w)
		{
			glitchTexture(Rect(srcX, srcY, w, blockSize)).draw(Vec2(offsetX, y));
		}
		else
		{
			// 右端分
			glitchTexture(Rect(srcX, srcY, remainingW, blockSize))
				.draw(Vec2(offsetX, y));
			// 左端分
			glitchTexture(Rect(0, srcY, w - remainingW, blockSize))
				.draw(Vec2(offsetX + remainingW, y));
		}
	}
	//グリッチノイズ用---------------------------------------------
#pragma endregion

	cpBossCharacterView.draw();
	RectF(Arg::bottomCenter(Scene::Width()/2, Scene::Height()), Scene::Width(), 200).draw(ColorF{ 0, 0, 0, 0.4 });
	cpPlayerCharacterView.draw();
	cpDropZone.draw();
	cpBehaviorInfoView.draw();
}
