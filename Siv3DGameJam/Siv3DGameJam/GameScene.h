#pragma once
# include <Siv3D.hpp>
#include "Common.h" 

class GameScene : public App::Scene
{
public:
	GameScene(const InitData& init) : IScene(init) {}

	void update() override
	{
		if (s3d::SimpleGUI::Button(U"タイトルへ", { 300, 300 })) {
			changeScene(U"Title");
		}
	}

	void draw() const override
	{
		s3d::Scene::SetBackground(s3d::Palette::Gray);
		s3d::FontAsset(U"Game")(U"Game Scene").drawAt({ 60, 40 });
	}
};
