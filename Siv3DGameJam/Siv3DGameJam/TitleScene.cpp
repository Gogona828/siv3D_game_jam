# include "TitleScene.h"
# include <Siv3D.hpp>

TitleScene::TitleScene(const InitData& init)
	: IScene{ init } // SceneManager対応の初期化
{
	m_background = s3d::Texture(U"assets/title_bg.png");
	m_logo = s3d::Texture(U"assets/title_logo.png");
	m_startButton = s3d::Texture(U"assets/btn_start.png");
	m_howToButton = s3d::Texture(U"assets/btn_howto.png");
	m_hoverFrame = s3d::Texture(U"assets/btn_hover.png");

	m_decideSE = s3d::Audio(U"assets/audio/se/se_decide.mp3");
	// BGM読み込み
	m_bgMusic = Audio(U"assets/audio/bgm/bgm_title.mp3");

	m_lastClickTimeStart = 0;
	m_lastClickTimeHowTo = 0;

	m_startPos = Vec2{ 260, 300 }; //448,252が半分
	m_howToPos = Vec2{ 470, 300 };

	// ボタンの描画倍率（0.5 = 半分サイズ）
	m_buttonScale = 0.5;

	if (!m_bgMusic.isEmpty())
	{
		m_bgMusic.setVolume(0.1); // 50%音量
		m_bgMusic.setLoop(true);  // ループ再生
		m_bgMusic.play();
	}
}

void TitleScene::update()
{
	using namespace s3d;

	constexpr double doubleClickInterval = 0.3;

	const RectF startRect{ m_startPos, m_startButton.size() * m_buttonScale };
	const RectF howToRect{ m_howToPos, m_howToButton.size() * m_buttonScale };

	if (startRect.mouseOver())
	{
		m_hovered = U"Start";

		if (MouseL.down())
		{
			double now = Scene::Time();
			if (now - m_lastClickTimeStart < doubleClickInterval)
			{
				m_decideSE.playOneShot();
				m_fadeTimer.restart();
				m_transitioning = true;
			}
			m_lastClickTimeStart = now;
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
					System::LaunchFile(mdFile);
				}
			}
			m_lastClickTimeHowTo = now;
		}
	}
	else
	{
		m_hovered.clear();
	}

	if (m_transitioning && m_fadeTimer.sF() > 1.0)
	{
		m_bgMusic.stop();          // 遷移前に止める
		changeScene(U"Training"); // ← App::Scene から継承されている
	}
}

void TitleScene::draw() const
{
	using namespace s3d;

	m_background.resized(Scene::Size()).draw();
	m_logo.resized(400).drawAt(Scene::CenterF().x, 150);


	// ボタン描画（リサイズ反映）
	m_startButton.resized(m_startButton.size() * m_buttonScale).draw(m_startPos);
	m_howToButton.resized(m_howToButton.size() * m_buttonScale).draw(m_howToPos);

	if (m_hovered == U"Start")
	{
		m_hoverFrame.resized(m_hoverFrame.size()*m_buttonScale).draw(m_startPos);
	}
	else if (m_hovered == U"HowTo")
	{
		m_hoverFrame.resized(m_hoverFrame.size() * m_buttonScale).draw(m_howToPos);
	}

	if (m_transitioning)
	{
		const double t = m_fadeTimer.sF();
		RectF(Scene::Size()).draw(ColorF{ 0.0, Min(t, 1.0) });
	}
}
