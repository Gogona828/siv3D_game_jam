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
# include "BossSkillManager.h"
# include <Siv3D.hpp>

using namespace s3d;

BattleScene::BattleScene(const InitData& init) : IScene(init)
{
	m_bgMusic = Audio(U"assets/audio/bgm/bgm_battle.mp3");
	m_clickSE = s3d::Audio(U"assets/audio/se/se_click.mp3");

	// BGM再生（AudioManager 経由）
	AudioManager::Get().playBGM(m_bgMusic);

	BossSkillManager::getInstance().loadBossSkills(U"BossSkill.csv");

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
	skillEffect.update();

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
				SkillGrantService::getInstance().grantSkill(U"FIX.atk");
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
					// ボスがブレーク状態なら行動不能
					if (GameData::getInstance().bossInfos().isBroken)
					{
						// 何もしない
						Console << U"ボスはブレークしている！";
					}
					else
					{
						auto& bossInfo = GameData::getInstance().bossInfos();
						double hpPercent = static_cast<double>(bossInfo.hp) / bossInfo.maxHp * 100.0;

						Array<String> availableSkills = BossSkillManager::getInstance().getAvailableSkills(hpPercent);
						
						String selectedSkillKey = U"BossNormalAttack.atk"; // デフォルトの攻撃
						if (turnCount > 0 && turnCount % 15 == 0)
						{
							// TODO: カーネルパニックのスキルキーを正しく設定する
							selectedSkillKey = U"KernelPanic.boss"; // 仮のキー
						}
						else if (not availableSkills.isEmpty())
						{
							selectedSkillKey = availableSkills.choice();
						}

						int n = Random(0, 5);
						for (int i = 0; i < 2; i++)
						{
							const int slotIndex = BossActionPattern[n][i];
							cpDropZone.setDropBoxTexture(slotIndex, bossBackPath, bossFrontPath);
							slotKeys[slotIndex] = selectedSkillKey;
						}
					}
				}

				currentBattleState = BattleState::WaitAlign;
				continue;

			case BattleState::WaitAlign:
				if (cpDropZone.allComplete())
				{
					currentBattleState = BattleState::SequentialProcess;
					
					Array<std::pair<String, int>> prioritySkills;
					Array<std::pair<String, int>> normalSkills;

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
						
						                        if (const ISkill* skill = SkillContainer::getInstance().getSkill(key))
						                        {
							if (skill->hasPriority())
							{
								prioritySkills.push_back({key, slot});
							}
							else
							{
								normalSkills.push_back({key, slot});
							}
						}
					}

					auto addSkillToAction = [this](const String& key, int slot)
					{
						sequence.addAction([this, key, slot]()
						{
							if (const ISkill* skill = SkillContainer::getInstance().getSkill(key))
							{
								auto& playerInfo = GameData::getInstance().infos();

								// Boss skills don't have MP cost
								if (key.includes(U".boss") || key.starts_with(U"Boss"))
								{
									SkillContext context{ GameData::getInstance(), *this, slot };
									skill->execute(context);
								}
								else if (playerInfo.mp >= skill->getMPCost())
								{
									playerInfo.mp -= skill->getMPCost();

									cpPlayerCharacterView.playAttackAnimation();

									SkillContext context{ GameData::getInstance(), *this, slot };
									skill->execute(context);
								}
								else
								{
									// Not enough MP, maybe play a sound or show a message
									Console << U"Not enough MP for skill: " << key;
								}
							}
							else
							{
								Console << U"[Skill] Skill not found or not available: " << key;
							}
						});
						sequence.addWaitForAllSequencesArmed();
					};

					for (const auto& skillInfo : prioritySkills)
					{
						addSkillToAction(skillInfo.first, skillInfo.second);
					}

					for (const auto& skillInfo : normalSkills)
					{
						addSkillToAction(skillInfo.first, skillInfo.second);
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
	// Decrement buff durations
	auto& playerInfo = GameData::getInstance().infos();
	for (auto it = playerInfo.buffs.begin(); it != playerInfo.buffs.end(); )
	{
		it->duration--;
		if (it->duration <= 0)
		{
			it = playerInfo.buffs.erase(it);
		}
		else
		{
			++it;
		}
	}

	// Store current slot keys for Rollback
	previousSlotKeys = slotKeys;

	// Handle player action cancellation
	if (GameData::getInstance().infos().cancelPlayerNextAction)
	{
		slotKeys.assign(4, U""); // Clear player's slots
		GameData::getInstance().infos().cancelPlayerNextAction = false; // Reset flag
	}

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
		// ★ブレーク状態をリセット
		GameData::getInstance().resetBreakStatus();

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
	skillEffect.draw();
}
