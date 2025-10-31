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

	// 元画像読み込み
	glitchImage = Image(U"assets/maingame/battle/glitch.png");
	glitchTexture = Texture(glitchImage);
}

void BattleScene::update()
{
	cpDropZone.update();

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
}

void BattleScene::draw() const
{
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


	cpBossCharacterView.draw();
	RectF(Arg::bottomCenter(Scene::Width()/2, Scene::Height()), Scene::Width(), 200).draw(ColorF{ 0, 0, 0, 0.4 });
	cpDropZone.draw();
	cpPlayerCharacterView.draw();
	cpBehaviorInfoView.draw();
}
