#pragma once
# include <Siv3D.hpp>
using App = s3d::SceneManager<s3d::String, void>;
enum class TrainingState
{
	CanInputFile,
	Event,
	EndTraining
};
class BranchStatusBar
{
public:
	BranchStatusBar(Vec2 pos, double width, double height)
		: m_rect{ pos, width, height } {
	}

	void update(double value)
	{
		m_value = Clamp(value, -1.0, 1.0);
	}

	void draw() const
	{
		// 中央を軸にスケールさせたい
		const Vec2 center = m_rect.center();

		if (m_value < 0)  // 左側（負）
		{
			double rate = Clamp(-m_value, 0.0, 1.0);  // 絶対値でスケール率
			RectF filled = m_rect.stretched(-1).scaledAt(center, rate, 1.0);

			// 左に寄せる：中心から左方向へ描画されるように座標を補正
			filled.x = center.x - filled.w;

			filled.draw(Palette::Red);
		}
		else  // 右側（正）
		{
			double rate = Clamp(m_value, 0.0, 1.0);
			RectF filled = m_rect.stretched(-1).scaledAt(center, rate, 1.0);
			filled.draw(Palette::Skyblue);
		}

		// 枠線
		m_rect.drawFrame(2, Palette::White);
		s3d::RectF centerLine{ center.x - 1, m_rect.y, 2, m_rect.h };
		centerLine.draw(Palette::White);
	}

private:
	RectF m_rect;
	double m_value = 0;
};

class StatusBar
{

public:
	StatusBar(Vec2 pos, double width, double height)
		: m_rect{ pos, width, height } {
	}

	void update(double value)
	{
		m_value = Clamp(value, -1.0, 1.0);
	}

	void draw() const
	{
		// 中央を軸にスケールさせたい
		const Vec2 center = m_rect.center();
		double rate = Clamp(m_value, 0.0, 1.0);
		RectF filled = m_rect.stretched(-1).scaledAt(center, rate, 1.0);
		filled.draw(Palette::Skyblue);

		// 枠線
		m_rect.drawFrame(2, Palette::White);
	}

private:
	RectF m_rect;
	double m_value = 0;
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
	Array<FilePath> filePathList;//すでに入力されたパスリスト
	String fileExtension;//拡張子格納用
	uint64 hash;//ハッシュ値格納用

	Array<BranchStatusBar> m_bars = {
			{ Vec2{ 20, 220 }, 200, 20 },
			{ Vec2{ 20, 260 }, 200, 20 },
			{ Vec2{ 20, 300 }, 200, 20 },
			{ Vec2{ 20, 340 }, 200, 20 },
			{ Vec2{ 20, 380 }, 200, 20 },
	};
	StatusBar overloadBar{ Vec2{ 20, 440 }, 200, 20 };

	bool initialized = false;

	float timer = 0;
	float maxTimer = 10;

	void update() override;
	void draw() const override;
	void statusTable();
	void eventTable();
};
