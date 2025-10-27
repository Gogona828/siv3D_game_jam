# pragma once
# include <Siv3D.hpp>
# include "GameData.h"
# include "VolumeSlider.h"
# include "AudioManager.h"

// main.cpp の型エイリアス App を使う
using App = s3d::SceneManager<s3d::String, void>;

class TitleScene : public App::Scene
{
private:
	s3d::Texture m_background;
	s3d::Texture m_logo;
	s3d::Texture m_startButton;
	s3d::Texture m_howToButton;
	s3d::Texture m_hoverFrame;
	s3d::Audio m_decideSE;
	s3d::Audio m_bgMusic; // タイトルBGM用

	s3d::String m_hovered;

	double m_lastClickTimeStart;
	double m_lastClickTimeHowTo;

	Vec2 m_startPos;
	Vec2 m_howToPos;

	double m_buttonScale;  // ボタンの描画倍率

	bool m_transitioning = false;
	s3d::Stopwatch m_fadeTimer{ s3d::StartImmediately::No };

	VolumeSlider m_bgmSlider{ s3d::Vec2(725, 420), 150, 10,U"BGM", 0.5 };
	VolumeSlider m_seSlider{ s3d::Vec2(725, 460), 150, 10, U"SE", 0.5 };

public:
	TitleScene(const InitData& init);  // ← コンストラクタは InitData を受け取る

	void update() override; // App::Scene に存在するメンバーを正しくオーバーライド
	void draw() const override;
};
