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
	const Font font{ FontMethod::MSDF, 48 };


	TrainingState m_state = TrainingState::CanInputFile;
	int currentTurn = 0;
	int maxTurn = 10;
	DroppedFilePath droppedFile;

	float timer = 0;
	float maxTimer = 10;
	void update() override;
	void draw() const override;
	void statusTable();
	void eventTable();
};

