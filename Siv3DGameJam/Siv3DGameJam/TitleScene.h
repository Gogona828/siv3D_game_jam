#pragma once
# include <Siv3D.hpp>
using App = s3d::SceneManager<s3d::String, void>;

class TitleScene : public App::Scene
{
public:
	TitleScene(const InitData& init) : IScene(init) {}

	void update() override
	{
		if (s3d::SimpleGUI::Button(U"ゲーム開始", { 300, 300 })) {
			changeScene(U"Training"); // 公式の changeScene
		}
	}

	void draw() const override
	{
		s3d::Scene::SetBackground(s3d::Palette::Skyblue);
		s3d::FontAsset(U"Title")(U"Title Scene").drawAt(s3d::Scene::Center());
	}
};
