#pragma once
# include <Siv3D.hpp>
#include "Common.h" 
enum class TrainingState
{
	CanInputFile,
	Event,
	AfterEvent,
	EndTraining
};
struct SystemStatusAddData
{
	int addId = -1;
	int addValue = 0;
};
enum class EventType
{
	Attack,
	Heal,
	Buff,
	Debuff,
	None
};

class BranchStatusBar
{
public:
	BranchStatusBar(Vec2 pos, double width, double height)
		: m_rect{ pos, width, height } {
	}

	void update()
	{
		if(!m_animating)
			return;
		m_timer += s3d::Scene::DeltaTime();
		double t = Clamp(m_timer / 0.5, 0.0, 1.0);  // 0.5秒でアニメーション完了
		m_value = Math::Lerp(m_startValue, m_targetValue, t);

		if (t >= 1.0)
		{
			m_animating = false;
		}
	}

	void draw() const
	{
		const Vec2 center = m_rect.center();
		const double halfW = m_rect.w * 0.5;

		if (m_value < 0)
		{
			// 左向き（赤）
			const double rate = Clamp(-m_value, 0.0, 1.0);
			const double w = halfW * rate;

			// 中心から左に伸ばす
			RectF filled(center.x - w, m_rect.y, w, m_rect.h);
			filled.draw(Palette::Red);
		}
		else
		{
			// 右向き（青）
			const double rate = Clamp(m_value, 0.0, 1.0);
			const double w = halfW * rate;

			// 中心から右に伸ばす
			RectF filled(center.x, m_rect.y, w, m_rect.h);
			filled.draw(Palette::Skyblue);
		}
		//中央分離帯
		RectF filled(center.x - 1.0, m_rect.y - 3.0, 2, m_rect.h + 6.0);
		filled.draw(Palette::White);
		// 枠線
		m_rect.drawFrame(2, Palette::White);
	}
	void setTarget(double value)
	{
		m_startValue = m_value;
		m_targetValue = Clamp(value, -1.0, 1.0);
		m_timer = 0.0;
		m_animating = true;
	}

private:
	RectF m_rect;
	double m_value = 0;
	double m_startValue = 0;
	double m_targetValue = 0;
	double m_timer = 0.0;
	bool m_animating = false;

	s3d::Texture m_background;
};

class StatusBar
{
public:
	StatusBar(Vec2 pos, double width, double height)
		: m_rect{ pos, width, height } {
	}

	void update()
	{
		if (!m_animating)
			return;
		m_timer += s3d::Scene::DeltaTime();
		double t = Clamp(m_timer / 0.5, 0.0, 1.0);  // 0.5秒でアニメーション完了
		m_value = Math::Lerp(m_startValue, m_targetValue, t);

		if (t >= 1.0)
		{
			m_animating = false;
		}
	}

	void draw() const
	{
		const double halfW = m_rect.w;
		// 右向き（青）
		const double rate = Clamp(m_value, 0.0, 1.0);
		const double w = halfW * rate;

		// 中心から右に伸ばす
		RectF filled(m_rect.x, m_rect.y, w, m_rect.h);
		filled.draw(Palette::Skyblue);

		// 枠線
		m_rect.drawFrame(2, Palette::White);
	}
	void setTarget(double value)
	{
		m_startValue = m_value;
		m_targetValue = Clamp(value, 0.0, 1.0);
		m_timer = 0.0;
	}

private:
	RectF m_rect;
	double m_value = 0;
	double m_startValue = 0;
	double m_targetValue = 0;
	double m_timer = 0.0;
	bool m_animating = false;
};




class TrainingScene : public App::Scene
{
public:
	TrainingScene(const InitData& init);
private:
	s3d::Texture m_background;
	s3d::Texture m_dropshadow;
	s3d::Texture m_training_guide;

	s3d::Texture m_btn_howto;
	s3d::Texture m_btn_explorer;
	s3d::Texture m_btn_hover;

	const Font font{ FontMethod::MSDF, 48 };
	String characterImagePath = U"assets/maingame/chara_image/origin_normal.png";
	Texture characterTexture = s3d::Texture(characterImagePath);
	TrainingState m_state = TrainingState::CanInputFile;
	bool evoluted = false;

	int currentTurn = 0;
	int maxTurn = 10;

	//入力されたファイルデータ
	DroppedFilePath droppedFile;
	Array<FilePath> filePathList;//すでに入力されたパスリスト
	String fileExtension;//拡張子格納用
	int size = 0;//ファイルサイズ格納用
	uint64 hash;//ハッシュ値格納用
	Array<SystemStatusAddData> statusAddData;

	Array<BranchStatusBar> m_bars = {
			{ Vec2{ 20, 220 }, 200, 20 },
			{ Vec2{ 20, 260 }, 200, 20 },
			{ Vec2{ 20, 300 }, 200, 20 },
			{ Vec2{ 20, 340 }, 200, 20 },
			{ Vec2{ 20, 380 }, 200, 20 },
	};
	StatusBar overloadBar{ Vec2{ 20, 440 }, 200, 20 };

	bool initialized = false;

	//アニメーション用タイマー
	float timer = 0;
	float maxTimer = 10;
	float cutinTimer = 0;
	float cutinMaxTimer = 1;

	//イベント描画用
	bool eventDrawing = false;

	void update() override;
	void draw() const override;
	Array<SystemStatusAddData> statusTable();
	EventType eventTypeTable();
	Array<int> eventIdTable(EventType type,int n);
	void ChangeStatus(Array<SystemStatusAddData> data);
	void SetCharacter(String path);

	int32 characterTextureId = 0;
};
