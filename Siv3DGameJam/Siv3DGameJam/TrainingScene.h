#pragma once
# include <Siv3D.hpp>
using App = s3d::SceneManager<s3d::String, void>;
enum class TrainingState
{
	CanInputFile,
	Event,
	EndTraining
};
class TrainingScene : public App::Scene
{
	public
		:
	TrainingScene(const InitData& init) : IScene(init) {}
private:
	TrainingState m_state = TrainingState::CanInputFile;
	int currentTurn = 0;
	int maxTurn = 10;

	void update() override;
	void draw() const override
	{
		//基本設定
		s3d::Scene::SetBackground(s3d::Palette::Black);
		//中央にキャラクターを描画
		//左にステータスを表示
		//下にファイルを表示
	}
};

