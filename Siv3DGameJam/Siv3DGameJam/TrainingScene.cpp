#include "stdafx.h"
#include "TrainingScene.h"
# include <Siv3D.hpp>
using App = s3d::SceneManager<s3d::String, void>;
void TrainingScene::update()
{
	switch (m_state)
	{
	case TrainingState::CanInputFile:
		// ファイル入力ができる状態
		// ファイルが入力されたらステータス数値を変動させる
		break;
	case TrainingState::Event:
		// イベント発生中の状態
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
}
