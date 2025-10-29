#include "stdafx.h"
#include "TrainingScene.h"
#include "GameData.h"
# include <Siv3D.hpp>
using App = s3d::SceneManager<s3d::String, void>;
void TrainingScene::update()
{
	//if (!initialized)
	//{
	//	m_bars[0].setTarget(1);
	//	m_bars[1].setTarget(-1);
	//	m_bars[2].setTarget(0.7);
	//	m_bars[3].setTarget(-0.5);
	//	overloadBar.setTarget(1);
	//	initialized = true;
	//}
	switch (m_state)
	{
	case TrainingState::CanInputFile:
		// ファイル入力ができる状態
		if (DragDrop::HasNewFilePaths())
		{
			if (auto files = DragDrop::GetDroppedFilePaths(); !files.isEmpty())
			{
				droppedFile = files.front();
				fileExtension = FileSystem::Extension(droppedFile.path);
				hash = s3d::Hash::XXHash3(droppedFile.path.narrow().data());
				size = FileSystem::FileSize(droppedFile.path);

				//TODO:重複チェック
				m_state = TrainingState::Event;
			}
		}
		break;
	case TrainingState::Event:
		//もし重複ファイルが入力されていたら弾く
		//ファイル入力が完了した状態
		statusAddData = statusTable();
		ChangeStatus(statusAddData);
		// ステータスの変動を行う
		m_bars[0].setTarget(GameData::getInstance().characterStatus.Reliability / 100.0);
		m_bars[1].setTarget(GameData::getInstance().characterStatus.Availability / 100.0);
		m_bars[2].setTarget(GameData::getInstance().characterStatus.Serviceability / 100.0);
		m_bars[3].setTarget(GameData::getInstance().characterStatus.Integrity / 100.0);
		m_bars[4].setTarget(GameData::getInstance().characterStatus.Security / 100.0);
		Print << (GameData::getInstance().characterStatus.Security / 100.0);

		overloadBar.setTarget(GameData::getInstance().characterStatus.Overload / 100.0);
		// イベント発生の状態
		// 拡張子に応じて、イベントの発生をテーブルから処理
		m_state = TrainingState::EndTraining;
		break;
	case TrainingState::EndTraining:
		//	トレーニング終了後の状態
		//	ターンを加算する
		currentTurn++;
		//	終了後、ターン数が最大に達していなければ次のターンへ
		if (currentTurn - maxTurn < 0|| GameData::getInstance().characterStatus.Overload >= 100)
		{
			m_state = TrainingState::CanInputFile;
		}
		else
		{
			//TODO:バトルシーンへ移行
			//現在はタイトルへ戻してるだけ
			changeScene(U"Title");
		}
		break;
	default:
		break;
	}

	m_bars[0].update();
	m_bars[1].update();
	m_bars[2].update();
	m_bars[3].update();
	m_bars[4].update();

	overloadBar.update();
}
void TrainingScene::draw() const
{
	const s3d::Vec2 barPos{ 100, 200 };
	const double barMaxWidth = 400.0;
	const double barHeight = 30.0;
	const double padding = 3.0; // 内側のバーとの余白
	//基本設定
	s3d::Scene::SetBackground(s3d::Palette::Gray);
	//中央にキャラクターを描画
	// 左上に残ターン数を表示
	s3d::RectF currentTurnOuterRect{ s3d::Vec2{20,20}, 190, 130};
	currentTurnOuterRect.draw(s3d::Palette::White);
	s3d::RectF currentTurnInnerRect{ s3d::Vec2{21,21}, 188, 128 };
	currentTurnInnerRect.draw(s3d::Palette::Black);
	//中身の文字描画
	font(U"決戦まで...").draw(24, Vec2{30, 40}, ColorF{1.0});
	font(U"{}ターン"_fmt(maxTurn - currentTurn)).draw(40, Vec2{ 30, 60 }, ColorF{ 1.0 });
	//左にステータスを表示
	//s3d::RectF outerRect{ barPos, barMaxWidth, barHeight };
	//outerRect.draw(s3d::Palette::Darkgray); // 背景を濃い灰色で描画

	//ステータス１
	font(U"ステータス1").draw(24, Vec2{ 20, 190 }, ColorF{ 1.0 });

	for (auto i : step(m_bars.size()))
	{
		m_bars[i].draw();
	}
	overloadBar.draw();
	//下にファイルを表示
}

Array<SystemStatusAddData> TrainingScene::statusTable()
{
	Array<SystemStatusAddData> statusAddData;
	//拡張子に応じたステータス変動の先行抽選
	if(fileExtension == U"txt")
	{
		SystemStatusAddData upStatusData;
		upStatusData.addId = static_cast<int>(StatusId::Intergrity);
		upStatusData.addValue = Random(1,10);
		statusAddData.push_back(upStatusData);
		SystemStatusAddData downStatusData;
		downStatusData.addId = static_cast<int>(StatusId::Security);
		downStatusData.addValue = Random(1, 10) * -1;
		statusAddData.push_back(downStatusData);

		SystemStatusAddData overloadStatusData;
		overloadStatusData.addId = static_cast<int>(StatusId::Overload);
		overloadStatusData.addValue = Random(5, 15);
		statusAddData.push_back(overloadStatusData);

	}
	else if (fileExtension == U"Zip")
	{
		SystemStatusAddData overloadStatusData;
		overloadStatusData.addId = static_cast<int>(StatusId::Overload);
		overloadStatusData.addValue = Random(5, 15) * -1;
		statusAddData.push_back(overloadStatusData);
	}
	else
	{
		//該当がない場合はランダムでステータス変動
		SystemStatusAddData upStatusData;
		upStatusData.addId = Random(0,5);
		upStatusData.addValue = Random(1, 10);
		statusAddData.push_back(upStatusData);
		SystemStatusAddData downStatusData;
		downStatusData.addId = Random(0, 5);
		downStatusData.addValue = Random(1, 10);
		statusAddData.push_back(downStatusData);
	}
	return statusAddData;
}
Array<SystemStatusAddData> TrainingScene::eventTable()
{
	//仮実装。ただイベントの規格が違うので修正予定
	Array<SystemStatusAddData> statusAddData;
	return statusAddData;
}

void TrainingScene::ChangeStatus(Array<SystemStatusAddData> data)
{
	for (auto& datas : data)
	{
		switch (static_cast<StatusId>(datas.addId))
		{
		case StatusId::Reliability:
			GameData::getInstance().characterStatus.Reliability += datas.addValue;
			break;
		case StatusId::Availability:
			GameData::getInstance().characterStatus.Availability += datas.addValue;
			break;
		case StatusId::Serviceability:
			GameData::getInstance().characterStatus.Serviceability += datas.addValue;
			break;
		case StatusId::Intergrity:
			GameData::getInstance().characterStatus.Integrity += datas.addValue;
			break;
		case StatusId::Security:
			GameData::getInstance().characterStatus.Security += datas.addValue;
			break;
		case StatusId::Overload:
			GameData::getInstance().characterStatus.Overload += datas.addValue;
			break;
		default:
			break;
		}
	}
}
