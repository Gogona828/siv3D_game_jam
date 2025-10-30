#pragma once
#include <Siv3D.hpp>
#include "Common.h" 

class ResultScene : public App::Scene
{
public:
	ResultScene(const InitData& init) : App::Scene(init)
	{
		m_timer.start();
		m_background = Texture(U"assets/result/result_bg.png");
		m_restartNormalTex = Texture(U"assets/result/restart_normal.png");
		m_restartHoverTex = Texture(U"assets/result/btn_hover.png");
		m_restartRect = RectF(static_cast<float>(Scene::Center().x+280), 350.f, 300.f, 80.f);
	}

	void update() override;
	void draw() const override;

private:
	// フォント類
	Font m_titleFont{ 40, Typeface::Bold };
	Font m_labelFont{ 22 };
	Font m_textFont{ 18 };
	Font m_rankFont{ 110, Typeface::Heavy };

	// 背景とボタン
	Texture m_background;
	Texture m_restartNormalTex;
	Texture m_restartHoverTex;
	RectF m_restartRect;
	bool m_isRestartHovered = false;

	// アニメーション制御
	Stopwatch m_timer{ StartImmediately::No };
	bool m_animationSkipped = false;
	double m_elapsed = 0.0; // ←追加：経過時間を固定する
	// フェード用
	double m_fadeAlpha = 1.0;       // 1.0 = 完全黒、0.0 = フェード完了
	bool m_sequenceStarted = false;  // フェード完了後にシーケンスを開始

	// ランク
	String m_rank = String(U"A");


	// 内部関数
	double getAlpha(double start, double duration) const;
	void drawBlock(double a, const RectF& rect) const;

	ColorF GetRankColor(const String& rank) const
	{
		if (rank == U"S") return ColorF(1.0, 0.84, 0.0);
		if (rank == U"A") return ColorF(1.0, 0.4, 0.4);
		if (rank == U"B") return ColorF(0.4, 0.6, 1.0);
		if (rank == U"C") return ColorF(0.6, 1.0, 0.6);
		if (rank == U"E") return ColorF(1.0, 1.0, 1.0);
		return ColorF(0.7);
	}
};
