#include "stdafx.h"
#include "TrainingScene.h"
#include "GameData.h"
#include "MasterData.h"
# include <Siv3D.hpp>
#include "Common.h"
#include "JsonReader.h"
#include "SkillGrantService.h"
#include "CharacterStatusChangeTable.h"

using namespace UIElement;

TrainingScene::TrainingScene(const InitData& init)
	: IScene{ init } // SceneManager対応の初期化
{
	m_background = s3d::Texture(U"assets/maingame/training/training_bg.png");
	m_dropshadow = s3d::Texture(U"assets/maingame/training/training_dropshadow.png");;
	m_training_guide = s3d::Texture(U"assets/maingame/training/trainig_guide.png");;

	m_btn_howto = s3d::Texture(U"assets/maingame/training/btn_howto.png");;
	m_btn_explorer = s3d::Texture(U"assets/maingame/training/btn_explorer.png");;
	m_btn_restart = s3d::Texture(U"assets/result/restart_normal.png");;
	// ★ 追加: スキャンボタンのテクスチャ（explorerのものを流用）
	m_btn_scan = s3d::Texture(U"assets/maingame/training/btn_explorer.png");;
	m_btn_hover = s3d::Texture(U"assets/maingame/training/btn_hover.png");;
	eventBackGround = s3d::Texture(U"assets/maingame/event_image/event_bg.png");

	cutinTexture = Texture(U"assets/maingame/Event_image/origin_cutin.png");

	m_bgMusic = Audio(U"assets/audio/bgm/bgm_training.mp3");
	m_clickSE = s3d::Audio(U"assets/audio/se/se_click.mp3");
	m_eatSE = s3d::Audio(U"assets/audio/se/se_eat.mp3");
	m_cutinSE = s3d::Audio(U"assets/audio/se/se_cutin.mp3");
	m_growSE = s3d::Audio(U"assets/audio/se/se_grow.mp3");
	m_btnSE = s3d::Audio(U"assets/audio/se/se_reboot.mp3");

	m_explorerPos = Vec2(780, 20);
	m_howToPos = Vec2(780, 120);
	m_restartPos = Vec2(780, 220);
	// ★ 追加: スキャンボタンの座標 (Restartの下)
	m_scanPos = Vec2(780, 320); // 280 + 130
	// ボタンの描画倍率（0.5 = 半分サイズ）
	m_buttonScale = 0.4;

	// BGM再生（AudioManager 経由）
	AudioManager::Get().playBGM(m_bgMusic);

	// SKillフォルダの初期化
	SkillGrantService::getInstance().resetSkillFolder();

	// ★ 追加: アニメーション用配列の初期化
	m_animatedStatusValues.resize(6);
	m_targetStatusValues.resize(6);
	m_statusChanges.resize(6, 0);

	// 現在のGameDataの値で初期化
	const auto& status = GameData::getInstance().characterStatus.toArray();
	for (auto i : step(status.size()))
	{
		m_animatedStatusValues[i] = status[i];
		m_targetStatusValues[i] = status[i];
	}
	m_animatedStatusValues[5] = GameData::getInstance().characterStatus.Overload;
	m_targetStatusValues[5] = GameData::getInstance().characterStatus.Overload;

	// ★ 追加: バツボタンの初期化
	m_closeButtonRect = RectF(Scene::Width() - 50, 10, 40, 40);
}

void TrainingScene::update()
{
	if (MouseL.down()) {
		AudioManager::Get().playSE(m_clickSE);
		clickEffect.spawn(Cursor::PosF());
	}

	clickEffect.update();

	if (!initialized)
	{
		GameData::getInstance().reset();
		GameData::getInstance().evolutedCharacterTextureId = static_cast<CharacterType>(0);
		initialized = true;
	}

	constexpr double doubleClickInterval = 0.3;

	const RectF explorerRect{ m_explorerPos, m_btn_explorer.size() * m_buttonScale };
	const RectF howToRect{ m_howToPos, m_btn_howto.size() * m_buttonScale };
	const RectF restartRect{ m_restartPos, m_btn_restart.size() * m_buttonScale };

	// ★ 追加: スキャンボタンのRect
	const RectF scanButtonRect{ m_scanPos, m_btn_scan.size() * m_buttonScale };

	// ★ 修正: スキャンモード中はボタン操作を無効化
	if (m_fileScanMode == FileScanMode::Inactive)
	{
		if (explorerRect.mouseOver())
		{
			m_hovered = U"Explorer";

			if (MouseL.down())
			{
				double now = Scene::Time();
				if (now - m_lastClickTimeExplorer < doubleClickInterval)
				{
					AudioManager::Get().playSE(m_btnSE);

					system("explorer.exe");
				}
				m_lastClickTimeExplorer = now;
			}
		}
		else if (howToRect.mouseOver())
		{
			m_hovered = U"HowTo";

			if (MouseL.down())
			{
				double now = Scene::Time();
				if (now - m_lastClickTimeHowTo < doubleClickInterval)
				{
					const FilePath mdFile = U"assets/howto.md";
					if (FileSystem::Exists(mdFile))
					{
						AudioManager::Get().playSE(m_btnSE);
						System::LaunchFile(mdFile);
					}
				}
				m_lastClickTimeHowTo = now;
			}
		}
		else if (restartRect.mouseOver())
		{
			m_hovered = U"Restart";

			if (MouseL.down())
			{
				double now = Scene::Time();
				if (now - m_lastClickTimeRestart < doubleClickInterval)
				{
					AudioManager::Get().playSE(m_btnSE);
					changeScene(U"Title", 1.0s);
				}
				m_lastClickTimeRestart = now;
			}
		}
		// ★ 追加: スキャンボタンの処理
		else if (scanButtonRect.mouseOver())
		{
			m_hovered = U"Scan"; // ホバー状態を追加

			if (MouseL.down())
			{
				double now = Scene::Time();
				if (now - m_lastClickTimeScan < doubleClickInterval)
				{
					AudioManager::Get().playSE(m_btnSE);
					m_fileScanMode = FileScanMode::WaitingDrop; // スキャンモード起動
				}
				m_lastClickTimeScan = now;
			}
		}
		else
		{
			m_hovered.clear();
		}
	}


	if (cutinPlaying)
	{
		cutinTimer += Scene::DeltaTime();
		if (cutinTimer >= cutinDuration)
		{
			cutinTimer = 0.0;
			cutinPlaying = false;
			cutinWaiting = true; // 表示だけの状態に移行
		}
	}
	if (cutinWaiting)
	{
		waitAfterCutin += Scene::DeltaTime();
		if (waitAfterCutin >= waitTimeAfterCutin)
		{
			cutinWaiting = false;
			cutinClosing = true; // 閉じるアニメに移行
			cutinTimer = 0.0;
		}
	}
	if (cutinClosing)
	{
		cutinTimer += Scene::DeltaTime();
		if (cutinTimer >= cutinDuration)
		{
			cutinClosing = false; // 完全に終了
			cutinTimer = 0.0;
			// 次のイベント処理へ移行
			eventDrawing = true;

			eventDrawing = true;

			// イベント画面描画用のテクスチャとテキストを設定
			switch (nowEventType)
			{
			case EventType::Attack:
				SetEventData(U"assets/maingame/event_image/event_1.png", U"処理速度が向上した！");
				break;
			case EventType::Heal:
				SetEventData(U"assets/maingame/event_image/event_2.png", U"自己修復作業を行った！");
				break;
			case EventType::Buff:
				SetEventData(U"assets/maingame/event_image/event_3.png", U"脆弱性を修正した！");
				break;
			case EventType::Debuff:
				SetEventData(U"assets/maingame/event_image/event_4.png", U"バグった！！！！");
				break;
			default:
				break;
			}
		}
	}

	// ★ 修正: 育成用のファイル入力処理をスキャンモード中は停止
	if (m_fileScanMode == FileScanMode::Inactive)
	{
		switch (m_state)
		{
		case TrainingState::CanInputFile:
		{
			// ファイル入力ができる状態
			if (DragDrop::HasNewFilePaths())
			{
				if (auto files = DragDrop::GetDroppedFilePaths(); !files.isEmpty())
				{
					AudioManager::Get().playSE(m_eatSE);

					droppedFile = files.front();
					fileExtension = FileSystem::Extension(droppedFile.path);
					hash = s3d::Hash::XXHash3(droppedFile.path.narrow().data());
					size = FileSystem::FileSize(droppedFile.path);

					// ファイルドロップキャラアニメ
					m_characterAnimTimer = 0.0;
					m_characterAnimPlaying = true;

					//ファイルドロップアニメ用------------------------------------
					// ファイルアイコンまたは汎用画像を設定
					Texture iconTex;

					// 拡張子に応じて適当なアイコンを差し替えてもOK
					if (fileExtension == U"png" || fileExtension == U"jpg")
					{
						iconTex = Texture(droppedFile.path);
					}
					else
					{
						iconTex = Texture(U"assets/maingame/training/file.png"); // 汎用アイコン
					}

					// ドロップアニメを生成
					DropAnim anim;
					anim.texture = iconTex;
					anim.pos = Cursor::PosF();
					dropAnims << anim;
					//ファイルドロップアニメ用------------------------------------

					//TODO:重複チェック
					m_state = TrainingState::Event;
				}
			}
			break;
		}
		case TrainingState::Event:
		{
			//もし重複ファイルが入力されていたら弾く
			if (filePathList.contains(droppedFile.path))
			{
				//弾く処理
				m_state = TrainingState::CanInputFile;
				break;
			}
			else
			{
				//重複していなければリストに追加
				filePathList.push_back(droppedFile.path);
			}
			// 1. これから変動する値を計算
			CharacterStatusChangeTable characterStatusChangeTable(fileExtension, size, hash);
			statusAddData = characterStatusChangeTable.statusTable();

			// 2. アニメーション用の変数を設定
			m_statusChanges.fill(0); // 変化量リセット
			for (const auto& data : statusAddData)
			{
				m_statusChanges[data.addId] = data.addValue;
			}

			// 3. GameDataを即座に更新 (ChangeStatusがclampを行うため)
			ChangeStatus(statusAddData);
			// 4. アニメーションの「開始値」と「目標値」を再設定
			const auto& statusArray = GameData::getInstance().characterStatus.toArray();
			for (auto i : step(statusArray.size())) // 0-4
			{
				m_targetStatusValues[i] = statusArray[i]; // 目標値 (GameDataの最新値)
				m_animatedStatusValues[i] = m_targetStatusValues[i] - m_statusChanges[i]; // 開始値 (目標値 - 変化量)
			}
			// Overload (index 5)
			m_targetStatusValues[5] = GameData::getInstance().characterStatus.Overload;
			m_animatedStatusValues[5] = m_targetStatusValues[5] - m_statusChanges[5];


			// 5. ゲージのターゲットを設定 (これは既存のままでOK)
			m_bars[0].setValue(GameData::getInstance().characterStatus.Reliability / MasterData::maxStatusValue());
			m_bars[1].setValue(GameData::getInstance().characterStatus.Availability / MasterData::maxStatusValue());
			m_bars[2].setValue(GameData::getInstance().characterStatus.Serviceability / MasterData::maxStatusValue());
			m_bars[3].setValue(GameData::getInstance().characterStatus.Integrity / MasterData::maxStatusValue());
			m_bars[4].setValue(GameData::getInstance().characterStatus.Security / MasterData::maxStatusValue());
			overloadBar.setTarget(GameData::getInstance().characterStatus.Overload / MasterData::maxStatusValue());

			// 6. アニメーション開始
			m_statusAnimTimer.restart();
			m_state = TrainingState::AnimatingStatus; // ★ 新しい状態に移行
			AudioManager::Get().playSE(m_growSE);
			break;
		}
			// ★ 追加: ステータスアニメーションを再生する状態
		case TrainingState::AnimatingStatus:
		{
			// 0.0 (開始) から 1.0 (終了) までの進捗
			const double t = Min(m_statusAnimTimer.sF() / m_statusAnimDuration, 1.0);
			const double easeT = EaseOutCubic(t); // イージングを適用

			// 中間値を計算 (Lerp: 線形補間)
			for (auto i : step(m_animatedStatusValues.size())) // 0-5
			{
				double startVal = m_targetStatusValues[i] - m_statusChanges[i];
				m_animatedStatusValues[i] = Math::Lerp(startVal, m_targetStatusValues[i], easeT);
			}

			// アニメーションが完了したら AfterEvent (カットイン) へ
			if (t >= 1.0)
			{
				m_statusChanges.fill(0); // 変化量リセット (色を戻すため)
				m_state = TrainingState::AfterEvent;
			}
			break;
		}
		case TrainingState::AfterEvent:
		{
			// カットイン開始前の処理
			if (!cutinStarted && !m_characterAnimPlaying)
			{

				cutinStarted = true;     // 二重再生防止
				cutinPlaying = true;     // カットイン再生フラグON
				cutinTimer = 0.0;        // タイマーリセット
				waitAfterCutin = 0.0;    // 待機タイマーリセット

				// イベント種類の抽選
				nowEventType = eventTypeTable();
				//抽選確率をJsonから取得
				int eventCount = 1;
				Array<int> eventProbabilities;

				// イベント発生数の抽選確率をJsonから取得
				String jsonPath = U"assets/maingame/training/data/{}.json"_fmt(fileExtension);
				if (JsonReader::readData(jsonPath, U"EventCountProbability", eventProbabilities))
				{
					int currentProb = 0;
					int rand = Random(1, 100);
					for (size_t i = 0; i < eventProbabilities.size(); i++)
					{
						currentProb += eventProbabilities[i];
						if (rand <= currentProb)
						{

							eventCount = static_cast<int>(i);
							break;
						}
					}
				}
				if (nowEventType == EventType::None)
				{
					eventDrawing = false;
					cutinStarted = false; // 次回のカットインのためにリセット
					m_state = TrainingState::EndTraining;
				}
				// イベントタイプが None でなければ、ここでSEを鳴らす
				else if (nowEventType != EventType::None)
				{
					AudioManager::Get().playSE(m_cutinSE);
				}
				Array<int> eventIdArray = eventIdTable(nowEventType, eventCount + 1);
				GameData::getInstance().eventList = eventIdArray;
				getItemUnits.clear();
				for (auto i : GameData::getInstance().eventList)
				{
					getItemUnits.push_back(GetItemViewUnit(i));
				}
				for (auto i : eventIdArray)
				{
					SkillGrantService::getInstance().grantByKey(MasterData::getSkillName(i));
				}
				if (nowEventType == EventType::None)
				{
					// カットイン関連フラグをリセット
					cutinPlaying = false;
					cutinWaiting = false;
					cutinClosing = false;
					cutinStarted = false;

					m_state = TrainingState::EndTraining;
					break;
				}
			}


			// カットイン再生中
			if (cutinPlaying)
			{
				double t = cutinTimer / cutinDuration;
				Vec2 center = Scene::CenterF();

				// 画像をシーン横幅に合わせる
				double scale = static_cast<double>(Scene::Width()) / cutinTexture.width();

				// リサイズ後の縦幅
				double resizedHeight = cutinTexture.height() * scale;

				// 上下に広がるアニメ
				double halfH = resizedHeight / 2 * t;
				double topY = center.y - halfH;
				double bottomY = center.y + halfH;

				// 縦方向の描画範囲を srcRect で切り出す
				int srcH = static_cast<int>(cutinTexture.height() * t);
				Rect srcRect(0, 0, cutinTexture.width(), srcH);

				// 描画
				cutinTexture(srcRect).resized(Scene::Width(), srcH * scale).drawAt(center.x, topY + (srcH * scale) / 2);
			}

			// カットイン終了後の待機時間
			if (!cutinPlaying && !eventDrawing && !cutinClosing && !cutinWaiting)
			{
				waitAfterCutin += Scene::DeltaTime();
			}

			// イベント画面描画中
			if (eventDrawing)
			{
				if (MouseL.down())
				{
					eventDrawing = false;
					cutinStarted = false; // 次回のカットインのためにリセット
					m_state = TrainingState::EndTraining;
				}
			}
			break;
		}
		case TrainingState::EndTraining:
			//	トレーニング終了後の状態
			//	ターンを加算する
			currentTurn++;
			//	終了後、ターン数が最大に達していなければ次のターンへ
			if (maxTurn - currentTurn > 0 && GameData::getInstance().characterStatus.Overload < 100)
			{
				m_state = TrainingState::CanInputFile;
				if (currentTurn == 3 && !evoluted)
				{
					SetCharacter(U"assets/maingame/chara_image/evolution_normal.png");
					GameData::getInstance().evolutedCharacterTextureId = static_cast<CharacterType>(7);

					// ★ここでカットイン画像更新
					UpdateCutinTexture();
				}
				if (!evoluted)
				{
					for (int i = 0; i < GameData::getInstance().characterStatus.toArray().size(); i++)
					{
						if (GameData::getInstance().characterStatus.toArray()[i] / MasterData::maxStatusValue() >= 0.7)
						{
							characterTextureId = i + 1;
							SetCharacter(MasterData::getTexturePath(i + 1));
							evoluted = true;
							GameData::getInstance().evolutedCharacterTextureId = static_cast<CharacterType>(i);

							// ★ここでカットイン画像更新
							UpdateCutinTexture();
							break;
						}
					}
				}
				int count = 0;
				for (int i = 0; i < GameData::getInstance().characterStatus.toArray().size(); i++)
				{
					if (GameData::getInstance().characterStatus.toArray()[i] < 0)
					{
						count++;
					}
				}
				if (count >= 3)
				{
					characterTextureId = 0;
					SetCharacter(U"assets/maingame/chara_image/bug_normal.png");
					GameData::getInstance().evolutedCharacterTextureId = static_cast<CharacterType>(8);

					// ★ここでカットイン画像更新
					UpdateCutinTexture();
				}
			}
			else
			{
				GameData& gameData = GameData::getInstance();
				gameData.infos().pcTextureId = characterTextureId;
				// バトルシーンへ移行
				changeScene(U"Battle");
			}
			break;
		default:
			break;
		}
	}

	m_bars[0].update();
	m_bars[1].update();
	m_bars[2].update();
	m_bars[3].update();
	m_bars[4].update();

	overloadBar.update();

	// キャラクター拡縮アニメーション進行
	if (m_characterAnimPlaying)
	{
		m_characterAnimTimer += Scene::DeltaTime();
		if (m_characterAnimTimer >= m_characterAnimDuration)
		{
			m_characterAnimTimer = m_characterAnimDuration;
			m_characterAnimPlaying = false; // 終了
		}
	}

	//ファイルドロップアニメ用------------------------------------
	for (auto& anim : dropAnims)
	{
		anim.time += Scene::DeltaTime();
		anim.pos.y -= 60 * Scene::DeltaTime();     // 上に移動
		anim.scale = Math::Lerp(1.0, 0.4, anim.time / 1.0); // 縮小
		anim.alpha = Math::Lerp(1.0, 0.0, anim.time / 1.0); // フェードアウト
	}
	// 1秒経過したものを削除
	dropAnims.remove_if([](const DropAnim& a) { return a.time > 1.0; });
	//ファイルドロップアニメ用------------------------------------

	// ★ 追加: スキャンモード専用の更新処理 (update関数の最後に追加)
	if (m_fileScanMode != FileScanMode::Inactive)
	{
		// バツボタンでモード終了
		if (m_closeButtonRect.leftClicked())
		{
			m_fileScanMode = FileScanMode::Inactive;
			m_scannedFileInfo = ScannedFileInfo(); // 情報をリセット
		}

		// ★ 修正: Scene::DragDrop() ではなく、既存のAPI (DragDrop::HasNewFilePaths) を使用
		if (DragDrop::HasNewFilePaths())
		{
			// 既存の m_eatSE を流用
			AudioManager::Get().playSE(m_eatSE);

			if (auto files = DragDrop::GetDroppedFilePaths(); !files.isEmpty())
			{
				const FilePath droppedFilePath = files.front().path; // 最初のファイル (例: C:\test.cpp)
				// 1. ドロップされたファイル自体の情報を先に保存
				m_scannedFileInfo.fileName = FileSystem::FileName(droppedFilePath); // (例: "test.cpp")
				const String droppedFileExt = FileSystem::Extension(droppedFilePath); // (例: "cpp")
				m_scannedFileInfo.extension = droppedFileExt;

				// 2. 拡張子に対応するデータJSONのパスを組み立てる
				// (statusTable() と同じロジック)
				String dataJsonPath = U"assets/maingame/training/data/{}.json"_fmt(droppedFileExt);

				// 3. データJSONを読み込む
				if (loadScanJson(dataJsonPath))
				{
					// 成功: loadScanJson が upParam などを設定した
					m_fileScanMode = FileScanMode::Displaying;
				}
				else
				{
					// 失敗: 対応するJSONがない (不明な拡張子扱い)
					m_scannedFileInfo.upParam = U"-";
					m_scannedFileInfo.downParam = U"-";
					m_scannedFileInfo.eventProbability = U"中";
					m_fileScanMode = FileScanMode::Displaying;
				}
			}
		}
	}
}
void TrainingScene::draw() const
{
	m_background.resized(Scene::Size()).draw();
	m_dropshadow.resized(350).drawAt(Scene::CenterF().x, Scene::CenterF().y+200);
	m_training_guide.resized(500).draw(260,  22);


	//m_btn_hover = s3d::Texture(U"assets/maingame/training/btn_hover.png");;

	const s3d::Vec2 barPos{ 100, 200 };
	const double barMaxWidth = 400.0;
	const double barHeight = 30.0;
	const double padding = 3.0; // 内側のバーとの余白
	//基本設定
	s3d::Scene::SetBackground(s3d::Palette::Gray);

	// 中央にキャラクターを描画
	const double baseScale = 0.3;
	double scale = baseScale;

	if (m_characterAnimPlaying)
	{
		double t = m_characterAnimTimer / m_characterAnimDuration;
		scale = baseScale + 0.03 * EaseOutBack(1.0 - Abs(1.0 - t * 2.0));
	}

	// 描画位置（画面中央下70pxに配置）
	Vec2 drawPos = Vec2{ Scene::CenterF().x, Scene::Height() +10 };

	// テクスチャサイズ（scale込み）
	Vec2 scaledSize = Vec2{ characterTexture.width() * scale, characterTexture.height() * scale };

	// drawAt は「中心基準」なので、下中央にしたい場合はオフセット調整
	characterTexture.scaled(scale).drawAt(drawPos.x, drawPos.y - scaledSize.y / 2);

	// ボタン描画（リサイズ反映）
	m_btn_explorer.resized(m_btn_explorer.size() * m_buttonScale).draw(m_explorerPos);
	m_btn_howto.resized(m_btn_howto.size() * m_buttonScale).draw(m_howToPos);
	m_btn_restart.resized(m_btn_restart.size() * m_buttonScale).draw(m_restartPos);
	// ★ 追加: スキャンボタンの描画
	m_btn_scan.resized(m_btn_scan.size() * m_buttonScale).draw(m_scanPos);

	if (m_hovered == U"Explorer")
	{
		m_btn_hover.resized(m_btn_hover.size() * m_buttonScale).draw(m_explorerPos);
	}
	else if (m_hovered == U"HowTo")
	{
		m_btn_hover.resized(m_btn_hover.size() * m_buttonScale).draw(m_howToPos);
	}
	else if (m_hovered == U"Restart")
	{
		m_btn_hover.resized(m_btn_restart.size() * m_buttonScale).draw(m_restartPos);
	}
	// ★ 追加: スキャンボタンのホバー
	else if (m_hovered == U"Scan")
	{
		// m_btn_scan のサイズでホバーテクスチャを描画
		m_btn_hover.resized(m_btn_scan.size() * m_buttonScale).draw(m_scanPos);
	}

	// 左上に残ターン数を表示
	s3d::RectF currentTurnInnerRect{ s3d::Vec2{5,21}, 230, 100 };
	currentTurnInnerRect.draw(ColorF(0.0, 0.0, 0.0, 0.6));
	//中身の文字描画
	font(U"読み取れるファイル残数").drawAt(16, Vec2{120, 50}, ColorF{1.0});
	font(Format(U"{}"_fmt(maxTurn - currentTurn))).draw(TextStyle::Outline(0.5, s3d::Palette::White),38, Arg::topCenter(120, 65), ColorF{ 1.0 });
	//左にステータスを表示
	s3d::RectF statusBackground{ s3d::Vec2{5,150}, 230, 340 };
	statusBackground.draw(ColorF(0.0,0.0,0.0,0.6));
	font(U"メモリ").draw(18, Vec2{ 20, 430 }, ColorF{ 1.0 });

	// ★ 修正: ステータス数値 (カウントアップ/ダウン描画)
	const Array<double> textYPos = { 160, 210, 260, 310, 360 };
	for (auto i : step(textYPos.size())) // 0-4
	{
		const int change = m_statusChanges[i];
		ColorF color = Palette::White;
		if (change > 0) color = Palette::Cyan; // カウントアップ
		if (change < 0) color = Palette::Red;  // カウントダウン

		font(Format(static_cast<int>(m_animatedStatusValues[i])))
			.draw(20, Arg::topRight(210, textYPos[i]), color);
	}

	// ★ 修正: Overload数値
	{
		const int change = m_statusChanges[5];
		ColorF color = Palette::White;
		if (change > 0) color = Palette::Red;  // Overload増加は赤
		if (change < 0) color = Palette::Cyan; // Overload減少は青

		font(Format(static_cast<int>(m_animatedStatusValues[5])) + U"%")
			.draw(20, Arg::topRight(210, 430), color);
	}

	//ガイドメッセージ
	font(U"ファイルをドラッグアンドドロップしてください").draw(16, Vec2{ 280, 35 }, ColorF{ 1.0 });

	for (auto i : step(m_bars.size()))
	{
		m_bars[i].draw();
	}
	overloadBar.draw();	

	if (eventDrawing)
	{
		eventBackGround.resized(896, 504).drawAt(Scene::CenterF());	//フェード背景

		//イベント発生装飾バー
		s3d::RectF leftbar{ s3d::Vec2{46,60}, 105, 2 };				
		leftbar.draw(s3d::Palette::White);
		font(U"イベント発生").draw(TextStyle::Outline(0.2,s3d::Palette::Yellow), 28, Vec2{ 160, 38 }, ColorF{ 1.0 });
		s3d::RectF rightbar{ s3d::Vec2{339,60}, 105, 2 };
		rightbar.draw(s3d::Palette::White);
		
		eventTexture.resized(580).drawAt(245, 200);

		font(U"{}"_fmt(eventText)).draw(24, Vec2{ 44, 350 }, ColorF{ 1.0 });

		RectF getSkillTitleBackground{ s3d::Vec2{ 560, 38 }, 250, 50 };
		getSkillTitleBackground.draw(ColorF{1,1,1,0.5});
		font(U"入手物").draw(TextStyle::Outline(0.2, s3d::Palette::Yellow), 28, Vec2{ 640, 43 }, ColorF{ 1.0 });
		font(U"クリックで閉じる").draw(18, Vec2{ 375, 460 }, Palette::White);

		for(int i = 0; i < getItemUnits.size(); i++)
		{
			getItemUnits[i].draw(Vec2{ 570, 100 + i * 70 });
		}
	}
	//ファイルドロップアニメ用------------------------------------
	for (const auto& anim : dropAnims)
	{
		anim.texture.resized(128 * anim.scale).drawAt(anim.pos, ColorF(1.0, anim.alpha));
	}
	//ファイルドロップアニメ用------------------------------------

	if (cutinPlaying || cutinWaiting)
	{
		double t = cutinPlaying ? (cutinTimer / cutinDuration) : 1.0; // 待機中は最大値
		Vec2 center = Scene::CenterF();
		int srcH = static_cast<int>(cutinTexture.height() * t);
		int srcY = cutinTexture.height() / 2 - srcH / 2;
		Rect srcRect(0, srcY, cutinTexture.width(), srcH);

		cutinTexture(srcRect)
			.resized(Scene::Width(), srcH * (Scene::Width() / (double)cutinTexture.width()))
			.drawAt(center);
	}

	// 消去用（逆再生）
	if (cutinClosing)
	{
		double t = cutinTimer / cutinDuration;
		double revT = 1.0 - t;
		Vec2 center = Scene::CenterF();
		int srcH = static_cast<int>(cutinTexture.height() * revT);
		int srcY = cutinTexture.height() / 2 - srcH / 2;
		Rect srcRect(0, srcY, cutinTexture.width(), srcH);

		cutinTexture(srcRect)
			.resized(Scene::Width(), srcH * (Scene::Width() / (double)cutinTexture.width()))
			.drawAt(center);
	}

	// ★ 追加: スキャンモードのUI描画 (draw関数の最後に描画)
	if (m_fileScanMode != FileScanMode::Inactive)
	{
		// 画面全体を黒半透明で覆う
		RectF(Scene::Size()).draw(ColorF(0.0, 0.9));

		// バツボタンを描画
		m_closeButtonRect.draw(ColorF(1.0, 0.2, 0.2, 0.5)); // ホバー用に背景を薄く
		if (m_closeButtonRect.mouseOver())
		{
			m_closeButtonRect.draw(ColorF(1.0, 0.2, 0.2, 0.8)); // ホバーで濃く
		}

		// バツ印の座標を計算
		// m_closeButtonRect.pos が (Scene::Width() - 50, 10)
		const Vec2 topLeft = m_closeButtonRect.pos;
		const Vec2 p1 = topLeft.movedBy(10, 10); // 左上
		const Vec2 p2 = topLeft.movedBy(30, 30); // 右下
		const Vec2 p3 = topLeft.movedBy(10, 30); // 左下
		const Vec2 p4 = topLeft.movedBy(30, 10); // 右上

		// 2本の線を直接画面に描画
		Line(p1, p2).draw(3, Palette::White);
		Line(p3, p4).draw(3, Palette::White);

		if (m_fileScanMode == FileScanMode::WaitingDrop)
		{
			// ドロップ待ちテキスト
			font(U"ファイルをドラッグアンドドロップでスキャン").drawAt(TextStyle::Outline(0.5, Palette::Black), 32, Scene::Center().x, Scene::Height() - 100, ColorF(1.0));
		}
		else if (m_fileScanMode == FileScanMode::Displaying)
		{
			// スキャン結果の表示
			const Vec2 basePos = Scene::Center().movedBy(0, -150);
			const Font& titleFont = font; // (既存の font を流用)
			const Font& largeFont = font; // (既存の font を流用。本当は大きい方が良い)

			// ファイル名（不明な拡張子の場合はタイトル）
			String title = m_scannedFileInfo.fileName;
			if (m_scannedFileInfo.upParam == U"-") // 不明な拡張子の判定
			{
				title = U"不明な拡張子";
			}

			titleFont(title).drawAt(TextStyle::Outline(0.5, Palette::Black), 40, basePos.x, basePos.y, Palette::White);

			largeFont(U"拡張子: {}"_fmt(m_scannedFileInfo.extension)).drawAt(TextStyle::Outline(0.5, Palette::Black), 28, basePos.x, basePos.y + 80, Palette::White);
			largeFont(U"上がりやすい: {}"_fmt(m_scannedFileInfo.upParam)).drawAt(TextStyle::Outline(0.5, Palette::Black), 28, basePos.x, basePos.y + 140, Palette::Orange);
			largeFont(U"下がりやすい: {}"_fmt(m_scannedFileInfo.downParam)).drawAt(TextStyle::Outline(0.5, Palette::Black), 28, basePos.x, basePos.y + 200, Palette::Cyan);
			largeFont(U"イベント発生率: {}"_fmt(m_scannedFileInfo.eventProbability)).drawAt(TextStyle::Outline(0.5, Palette::Black), 28, basePos.x, basePos.y + 260, Palette::Yellow);

			largeFont(U"ファイルをドラッグアンドドロップ").drawAt(TextStyle::Outline(0.5, Palette::Black), 18, basePos.x, basePos.y + 350, Palette::White);
		}
	}

	clickEffect.draw();    // クリックエフェクト描画
}

EventType TrainingScene::eventTypeTable()
{
	// =================================================================
		// 優先度1: OverloadによるDebuff抽選
		// =================================================================
	{
		GameData& gameData = GameData::getInstance();
		const double currentOverload = gameData.characterStatus.Overload;
		const double maxOverload = MasterData::maxStatusValue(); // (例: 100.0)

		// ★ 修正: 確率が上昇し始めるしきい値（最大値の半分）
		const double threshold = maxOverload / 2.0; // (例: 50.0)

		// Overloadがしきい値（半分）を超えている場合のみ、Debuff抽選を行う
		if (currentOverload > threshold)
		{
			// ★ 修正: 確率を計算（50% -> 0%, 100% -> 100% の線形補間）
			// Math::Map(入力値, 入力最小, 入力最大, 出力最小, 出力最大)
			const double debuffProbability = Math::Map(
				currentOverload,
				threshold,     // 50.0 (この値の時 0.0 になる)
				maxOverload,   // 100.0 (この値の時 1.0 になる)
				0.0,           // 出力最小 (0%)
				1.0            // 出力最大 (100%)
			);

			// 確率（debuffProbability）で抽選
			if (RandomBool(debuffProbability))
			{
				//Print << U"OverloadによりDebuffイベント発生 (Overload: {}, 確率: {}%)"_fmt(currentOverload, debuffProbability * 100);
				return EventType::Debuff;
			}
		}
		// Overloadが threshold 以下の場合は、Debuff抽選自体がスキップされる (確率 0%)
	}

	// =================================================================
	// 優先度2: Debuffに選ばれなかった場合、ファイルベースの抽選を行う
	// =================================================================
	EventType eventType = EventType::None; // デフォルトはNone

	String jsonPath = U"assets/maingame/training/data/{}.json"_fmt(fileExtension);
	Array<int> eventProbability;

	if (JsonReader::readData(jsonPath, U"EventFireProbability", eventProbability))
	{
		int probability = Random(1, 100);

		// 1. イベントがそもそも発生するかの判定
		if (100 - probability >= eventProbability[0])
		{
			return EventType::None; // 発生しない
		}

		// 2. 発生する場合、イベントの種類を抽選 (Debuffを除く)
		Array<int> eventTypeProbability;
		if (JsonReader::readData(jsonPath, U"EventTypeProbability", eventTypeProbability))
		{
			// ★ 修正: Attack, Heal, Buff のみの確率で重み付き抽選

			// 元のJSONにDebuff(4番目)が含まれていると仮定
			if (eventTypeProbability.size() < 3)
			{
				// データが不正な場合は抽選失敗
				return EventType::None;
			}

			// Attack, Heal, Buff のみの合計ウェイトを計算
			int allWeight = eventTypeProbability[0] + eventTypeProbability[1] + eventTypeProbability[2];

			if (allWeight <= 0)
			{
				// 抽選対象がない
				return EventType::None;
			}

			int rnd = Random(1, allWeight);
			int cumulativeWeight = 0;

			// ★ 修正: i < 3 (Debuffの手前まで) でループ
			for (size_t i = 0; i < 3; i++)
			{
				cumulativeWeight += eventTypeProbability[i];
				if (rnd <= cumulativeWeight)
				{
					switch (i)
					{
					case 0: return EventType::Attack;
					case 1: return EventType::Heal;
					case 2: return EventType::Buff;
					}
				}
			}
		}
		else
		{
			// ★ 修正: JSONが読めなかった場合のフォールバック (Debuffを除く)
			int eventProb = Random(1, 3); // 1～3 (Attack, Heal, Buff)
			if (eventProb == 1) return EventType::Attack;
			if (eventProb == 2) return EventType::Heal;
			if (eventProb == 3) return EventType::Buff;
		}
	}
	else
	{
		// ★ 修正: JSONが読めなかった場合のフォールバック (Debuffを除く)
		int eventProb = Random(1, 3); // 1～3 (Attack, Heal, Buff)
		if (eventProb == 1) return EventType::Attack;
		if (eventProb == 2) return EventType::Heal;
		if (eventProb == 3) return EventType::Buff;
	}

	return EventType::None; // ここに到達した場合（抽選ミスなど
}
Array<int> TrainingScene::eventIdTable(EventType type,int n)
{
	Array<int> eventData;
	String baseSkillChoiceProbabilityJsonPath = U"assets/maingame/training/probabilityData/BaseChoiceSkillProbability.json";
	String SkillChoiceProbabilityForEventTypeJsonPath = U"assets/maingame/training/probabilityData/SkillChoiceProbabilityForEventType.json";
	Array<int> baseSkillChoiceProbabilitys;
	Array<int> skillChoiceProbabilitysForEventType;
	//テーブルデータがそろわない場合はFastReturn
	if (!JsonReader::readData(baseSkillChoiceProbabilityJsonPath, U"BaseChoiceSkillProbability", baseSkillChoiceProbabilitys))
	{
		Print << U"データ取得に失敗しました。イベント抽選を中止します。1";
		return eventData;
	}

	//n回分のイベントIDを抽選
	for(int i = 0; i < n; i++)
	{
		switch (type)
		{
		case EventType::Attack:
			if(!JsonReader::readData(SkillChoiceProbabilityForEventTypeJsonPath, U"Attack", skillChoiceProbabilitysForEventType))
			{
				Print << U"データ取得に失敗しました。イベント抽選を中止します。2";
				return eventData;
			}
			break;
		case EventType::Heal:
			if (!JsonReader::readData(SkillChoiceProbabilityForEventTypeJsonPath, U"Heal", skillChoiceProbabilitysForEventType))
			{
				Print << U"データ取得に失敗しました。イベント抽選を中止します。3";
				return eventData;
			}
			break;
		case EventType::Buff:
			if (!JsonReader::readData(SkillChoiceProbabilityForEventTypeJsonPath, U"Buff", skillChoiceProbabilitysForEventType))
			{
				Print << U"データ取得に失敗しました。イベント抽選を中止します。4";
				return eventData;
			}
			break;
		case EventType::Debuff:
			if (!JsonReader::readData(SkillChoiceProbabilityForEventTypeJsonPath, U"Debuff", skillChoiceProbabilitysForEventType))
			{
				Print << U"データ取得に失敗しました。イベント抽選を中止します。";
				return eventData;
			}
			break;
		default:
			return eventData;
			break;
		}

		for (auto j : skillChoiceProbabilitysForEventType)
		{
			int rnd = Random(1, 100);
			if(rnd <= baseSkillChoiceProbabilitys[j - 1])
			{
				if(eventData.size() > 6)
				{
					//イベントが多すぎる場合は打ち切り
					return eventData;
				}
				eventData.push_back(j);
			}
		}
	}

	return eventData;
}
void TrainingScene::ChangeStatus(Array<SystemStatusAddData> data)
{
	for (auto& datas : data)
	{
		switch (static_cast<StatusId>(datas.addId))
		{
		case StatusId::Reliability:
			GameData::getInstance().characterStatus.Reliability += static_cast<int>(
				Math::Clamp(static_cast<float>(datas.addValue),
					MasterData::maxStatusValue() * -1 - GameData::getInstance().characterStatus.Reliability,
					MasterData::maxStatusValue() - GameData::getInstance().characterStatus.Reliability));
			break;
		case StatusId::Availability:
			GameData::getInstance().characterStatus.Availability += static_cast<int>(
				Math::Clamp(static_cast<float>(datas.addValue),
					MasterData::maxStatusValue() * -1 - GameData::getInstance().characterStatus.Availability,
					MasterData::maxStatusValue() - GameData::getInstance().characterStatus.Availability));
			break;
		case StatusId::Serviceability:
			GameData::getInstance().characterStatus.Serviceability += static_cast<int>(
				Math::Clamp(static_cast<float>(datas.addValue),
					MasterData::maxStatusValue() * -1 - GameData::getInstance().characterStatus.Serviceability,
					MasterData::maxStatusValue() - GameData::getInstance().characterStatus.Serviceability));
			break;
		case StatusId::Integrity:
			GameData::getInstance().characterStatus.Integrity += static_cast<int>(
				Math::Clamp(static_cast<float>(datas.addValue),
					MasterData::maxStatusValue() * -1 - GameData::getInstance().characterStatus.Integrity,
					MasterData::maxStatusValue() - GameData::getInstance().characterStatus.Integrity));
			break;
		case StatusId::Security:
			GameData::getInstance().characterStatus.Security += static_cast<int>(
				Math::Clamp(static_cast<float>(datas.addValue),
					MasterData::maxStatusValue() * -1 - GameData::getInstance().characterStatus.Security,
					MasterData::maxStatusValue() - GameData::getInstance().characterStatus.Security));
			break;
		case StatusId::Overload:
		{
			float clampmin = GameData::getInstance().characterStatus.Overload * -1;
			float clampmax = MasterData::maxStatusValue() - GameData::getInstance().characterStatus.Overload;

			GameData::getInstance().characterStatus.Overload += static_cast<int>(
				Math::Clamp(static_cast<float>(datas.addValue),
					GameData::getInstance().characterStatus.Overload * -1,
					MasterData::maxStatusValue() - GameData::getInstance().characterStatus.Overload));
			break;
		}
		default:
			break;
		}
	}
}
void TrainingScene::SetCharacter(String path)
{
	//キャラクター画像の設定
	characterImagePath = path;
	characterTexture = Texture(characterImagePath);
}

void TrainingScene::UpdateCutinTexture()
{
	const auto& gameData = GameData::getInstance();
	const CharacterType type = gameData.evolutedCharacterTextureId;

	// evolutedCharacterTextureId に応じたパスを取得
	FilePath cutinPath;

	switch (type)
	{
	case CharacterType::Reliability:
		cutinPath = U"assets/maingame/Event_image/reliability_cutin.png";
		break;
	case CharacterType::Availability:
		cutinPath = U"assets/maingame/Event_image/availability_cutin.png";
		break;
	case CharacterType::Serviceability:
		cutinPath = U"assets/maingame/Event_image/serviceability_cutin.png";
		break;
	case CharacterType::Integrity:
		cutinPath = U"assets/maingame/Event_image/integrity_cutin.png";
		break;
	case CharacterType::Security:
		cutinPath = U"assets/maingame/Event_image/security_cutin.png";
		break;
	case CharacterType::Origin:
		cutinPath = U"assets/maingame/Event_image/origin_cutin.png";
		break;
	case CharacterType::Evoluted:
		cutinPath = U"assets/maingame/Event_image/evolution_cutin.png";
		break;
	case CharacterType::Bug:
		cutinPath = U"assets/maingame/Event_image/bug_cutin.png";
		break;
	default:
		cutinPath = U"assets/maingame/Event_image/reliability_cutin.png";
		break;
	}

	cutinTexture = Texture(cutinPath);
}

// TrainingScene.cpp の末尾に追加

// 配列の中で最も大きい値を持つ要素のインデックスを返す
int getIndexOfMaxValue(const Array<int>& arr)
{
	if (arr.isEmpty()) return -1;
	int maxVal = -1;
	int maxIndex = -1;
	for (size_t i = 0; i < arr.size(); ++i)
	{
		if (arr[i] > maxVal)
		{
			maxVal = arr[i];
			maxIndex = static_cast<int>(i);
		}
	}
	return maxIndex;
}

// インデックスからパラメータ名を取得 (draw()関数内の描画名を参考)
String TrainingScene::getParamNameByIndex(int index) const
{
	switch (index)
	{
		// 既存の draw() での表示名に合わせる
	case 0: return U"信頼性";
	case 1: return U"可用性";
	case 2: return U"保守性";
	case 3: return U"保全性";
	case 4: return U"安全性";
	default: return U"-";
	}
}

// イベント発生確率の配列から「低/中/高」を判定
String TrainingScene::getEventProbabilityString(const Array<int>& probabilityArray) const
{
	if (probabilityArray.isEmpty())
	{
		return U"中"; // 不明
	}

	// eventTypeTable()のロジックを流用 (JSONの値は「発生しない」確率)
	// (100 - X) が発生確率
	const int probValue = probabilityArray[0];

	if ((100 - probValue) >= 70) return U"高"; // 発生確率 70%以上
	if ((100 - probValue) >= 30) return U"中"; // 発生確率 30%以上
	return U"低"; // 発生確率 30%未満
}

// ★ 修正: 既存の JsonReader::readData に 100% 準拠する
bool TrainingScene::loadScanJson(const FilePath& path)
{
	// path は "assets/maingame/training/data/cpp.json" など
	const String filePathString = path;

	// 読み込みが *すべて* 成功したか
	bool success = true;

	// --- 1. 上がりやすいパラメータ (UpStatusProbability) ---
	Array<int> upProb;
	if (JsonReader::readData(filePathString, U"UpStatusProbability", upProb))
	{
		m_scannedFileInfo.upParam = getParamNameByIndex(getIndexOfMaxValue(upProb));
	}
	else {
		m_scannedFileInfo.upParam = U"-";
		success = false; // 読み込み失敗
	}

	// --- 2. 下がりやすいパラメータ (DownStatusProbability) ---
	Array<int> downProb;
	if (JsonReader::readData(filePathString, U"DownStatusProbability", downProb))
	{
		m_scannedFileInfo.downParam = getParamNameByIndex(getIndexOfMaxValue(downProb));
	}
	else {
		m_scannedFileInfo.downParam = U"-";
		success = false; // 読み込み失敗
	}

	// --- 3. イベント発生確率 (EventFireProbability) ---
	Array<int> eventProb;
	if (JsonReader::readData(filePathString, U"EventFireProbability", eventProb))
	{
		m_scannedFileInfo.eventProbability = getEventProbabilityString(eventProb);
	}
	else {
		// 失敗時はデフォルト値（"中"）
		m_scannedFileInfo.eventProbability = U"中";
		success = false; // 読み込み失敗
	}

	// 1つでもキーの読み込みに失敗したら false を返す
	return success;
}
