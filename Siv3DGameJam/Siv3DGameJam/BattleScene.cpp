# include "BattleScene.h"
# include "DropZone.h"
# include "PlayerCharacterView.h"
# include "BehaviorInfoView.h"
# include "BossCharacterView.h"
# include "GameData.h"
# include "SkillGrantService.h"
# include "SkillContainer.h"
# include "FIX.h"
# include "Sequence.h"
# include <Siv3D.hpp>

using namespace s3d;

BattleScene::BattleScene(const InitData& init) : IScene(init)
{
	m_bgMusic = Audio(U"assets/audio/bgm/bgm_battle.mp3");
	m_clickSE = s3d::Audio(U"assets/audio/se/se_click.mp3");

	// BGM再生（AudioManager 経由）
	AudioManager::Get().playBGM(m_bgMusic);

	cpDropZone.configZone(0, { U"atk", U"def", U"heal", U"buff", U"boss"});
	cpDropZone.configZone(1, { U"atk", U"def", U"heal", U"buff", U"boss" });
	cpDropZone.configZone(2, { U"atk", U"def", U"heal", U"buff", U"boss" });
	cpDropZone.configZone(3, { U"atk", U"def", U"heal", U"buff", U"boss" });

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
	// ★ 追加: Viewのupdateを呼び出す
	cpPlayerCharacterView.update();

	// ★ 追加: 被ダメージフラグを監視
	auto& gameData = GameData::getInstance();
	if (gameData.checkPlayerHitFlag())
	{
		cpPlayerCharacterView.playDamageBlink();
	}
	if (gameData.checkBossHitFlag())
	{
		//cpBossCharacterView.playDamageBlink();
	}

	if (MouseL.down()) {
		AudioManager::Get().playSE(m_clickSE);
		clickEffect.spawn(Cursor::PosF());
	}
	clickEffect.update();

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

	sequence.update(Scene::DeltaTime());

	// switchが前のケースに戻れない(?)そうなので、無理やりループを作りcontinueで回避する用
	constexpr int reenter = 1;
	for (int hops = 0; hops < reenter; hops++)
	{
		switch (currentBattleState)
		{
			case BattleState::Boot:
				// Init処理、あるなら
				// デバッグ用
				SkillGrantService::getInstance().resetSkillFolder();
				SkillGrantService::getInstance().grantByKey(U"FIX.atk");
				currentBattleState = BattleState::BoxReset;
				continue;

			case BattleState::BoxReset:
				cpDropZone.resetAllSlots();
				for (int i = 0; i < 4; i++)
				{
					cpDropZone.setDropBoxTexture(i, defaultPath);
				}

				slotKeys.assign(4, U"");

				currentBattleState = BattleState::EnemyActArrangement;
				continue;

			case BattleState::EnemyActArrangement:
				{
					int n = Random(0, 5);
					for (int i = 0; i < 2; i++)
					{
						cpDropZone.setDropBoxTexture(BossActionPattern[n][i], bossBackPath, bossFrontPath);
						// 行動の登録
						cpDropZone.assignSlot(BossActionPattern[n][i], U"BossNormalAttack.atk", false);
					}
				}

				currentBattleState = BattleState::WaitAlign;
				continue;

			case BattleState::WaitAlign:
				if (cpDropZone.allComplete())
				{
					currentBattleState = BattleState::SequentialProcess;
					
					// DropBoxの左から順に処理
					for (int slot = 0; slot < 4; ++slot)
					{
						String key = slotKeys[slot];
						if (key.isEmpty())
						{
							key = cpDropZone.getSkillKey(slot);
						}
						if (key.isEmpty())
						{
							continue;
						}
						
						sequence.addAction([key]()
						{
							//SkillContainer::getInstance().registerSkill(key);
							if (key.starts_with(U"Boss")) // ボスのスキルか？ Bossで始まっているかで判定している
							{
								// ボススキルは createKnown で生成
								if (auto sk = SkillContainer::getInstance().createKnown(key))
								{
									sk->execute();
								}
								else
								{
									// tableにも存在しない（タイプミスなど）
									Console << U"[Skill] Master list (Known) not found: " << key;
								}
							}
							else // プレイヤースキル
							{
								// プレイヤースキルは createRegistered で生成
								if (auto sk = SkillContainer::getInstance().createRegistered(key))
								{
									sk->execute();
								}
								else
								{
									// プレイヤーが習得していない
									Console << U"[Skill] Player skill (Registered) not found: " << key;
								}
							}
						});

						// 上記の処理が終わるまで待機
						sequence.addWaitForAllSequencesArmed();
					}
					
					// 4つ目の待機が終わったらターン終了へ
					sequence.onAllDone([this]() { Judge(); });
					sequence.start();
					continue;
				}
				break;
			case BattleState::SequentialProcess:
				break;
			case BattleState::GameOver:
				getData().gameOverFlag = true;
				changeScene(U"Result");
				break;
			case BattleState::GameClear:
				getData().gameOverFlag = false;
				changeScene(U"Result");
				break;
		}
	}
}

void BattleScene::Judge()
{
	// ★ 修正: 勝敗判定ロジックを追加

		// (GameData にHPを取得する関数が実装されていると仮定)
	if (GameData::getInstance().getPlayerHP() <= 0)
	{
		currentBattleState = BattleState::GameOver;
	}
	else if (GameData::getInstance().getBossHP() <= 0)
	{
		currentBattleState = BattleState::GameClear;
	}
	else
	{
		// どちらのHPも残っていれば、次のターンへ
		currentBattleState = BattleState::BoxReset;
	}

	// TODO: ゲームの分岐条件をつくる
	// currentBattleState = BattleState::GameClear;
	// currentBattleState = BattleState::GameOver;
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

	clickEffect.draw();    // クリックエフェクト描画
}
