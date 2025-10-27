#include "stdafx.h"
#include "TrainingScene.h"
#include "GameData.h"
# include <Siv3D.hpp>
using App = s3d::SceneManager<s3d::String, void>;
void TrainingScene::update()
{
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

				m_state = TrainingState::Event;
			}
		}
		break;
	case TrainingState::Event:
		//ファイル入力が完了した状態
		// ステータスの変動を行う
		//　時間取得　s3D::Scene::DeltaTime()で取得可能
		// s3d::lerp(開始値, 終了値, 進捗度);でステータスをアニメーションさせる
		// イベント発生の状態
		// 拡張子に応じて、イベントの発生をテーブルから処理
		break;
	case TrainingState::EndTraining:
		//	トレーニング終了後の状態
		//	ターンを加算アニメーションする
		//	終了後、ターン数が最大に達していなければ次のターンへ
		//	最大に達していればバトルへ移行
		//　パラメータの容量がマックスでもバトルへ移行する
		break;
	default:
		break;
	}

	m_bars[0].update(GameData::getInstance().characterStatus.Reliability / 100);
	m_bars[1].update(GameData::getInstance().characterStatus.Availability / 100);
	m_bars[2].update(GameData::getInstance().characterStatus.Serviceability / 100);
	m_bars[3].update(GameData::getInstance().characterStatus.Integrity / 100);
	m_bars[4].update(GameData::getInstance().characterStatus.Security / 100);

	overloadBar.update(GameData::getInstance().characterStatus.Overload / 100);
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

void TrainingScene::statusTable()
{

}
void TrainingScene::eventTable()
{

}
