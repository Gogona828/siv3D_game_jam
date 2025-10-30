#include "ResultScene.h"

using namespace s3d;

double ResultScene::getAlpha(double start, double duration) const
{
	if (m_animationSkipped) return 1.0;

	double t = m_timer.sF() - start;
	if (t < 0) return 0;
	if (t > duration) return 1;
	return t / duration;
}

void ResultScene::drawBlock(double a, const RectF& rect) const
{
	// a の進行に応じて透明度を調整（0.0～1.0）
	double alpha = Clamp(a, 0.0, 1.0);

	// 塗り：柔らかい白（最大でも0.08）
	double fillAlpha = Min(alpha * 0.8, 0.08);
	rect.draw(ColorF(1.0, 1.0, 1.0, fillAlpha));

	// 枠線：塗りより少し濃い白（最大0.25程度）
	double lineAlpha = Min(alpha * 1.0, 0.12);
	rect.stretched(-1).drawFrame(3, 0, ColorF(1.0, 1.0, 1.0, lineAlpha));
}

void ResultScene::update()
{
	if (getData().gameOverFlag)
	{
		m_rank = String(U"E");
	}
	// フェードイン中はシーケンスを止める
	if (!m_sequenceStarted)
	{
		m_fadeAlpha -= Scene::DeltaTime() / 1.0; // 1秒でフェードイン
		if (m_fadeAlpha <= 0.0)
		{
			m_fadeAlpha = 0.0;
			m_sequenceStarted = true;
			m_timer.restart(); // シーケンス開始
		}
		return;
	}

	// --- シーケンス更新 ---
	m_isRestartHovered = m_restartRect.mouseOver();

	// 左クリックでシーケンス全スキップ
	if (MouseL.down() && !m_animationSkipped)
	{
		m_animationSkipped = true;
	}

	// タイトルに戻るボタンダブルクリック
	static double lastClickTime = -1.0;
	if (m_isRestartHovered && MouseL.down())
	{
		double now = Scene::Time();
		if (lastClickTime > 0 && (now - lastClickTime) <= 0.3)
		{
			changeScene(U"Title", 1.0s);
			lastClickTime = -1.0;
		}
		else
		{
			lastClickTime = now;
		}
	}
}

void ResultScene::draw() const
{
	m_background.resized(Scene::Size()).draw();

	// フェード中は黒を描くだけ
	if (!m_sequenceStarted)
	{
		RectF(0, 0, Scene::Width(), Scene::Height()).draw(ColorF(0, m_fadeAlpha));
		return;
	}

	const double slideDistance = 50.0;

	// 1. Game Clear
	{
		double a = getAlpha(0.3, 0.5);
		double xOffset = (1.0 - a) * slideDistance;

		// 白線の長さ
		double lineLength = 200;

		// 左右に線を描く
		Line(Vec2(Scene::Center().x - lineLength - 200, 60),
			 Vec2(Scene::Center().x - 150, 60))
			.draw(2, ColorF(1.0, a));

		Line(Vec2(Scene::Center().x + 150, 60),
			 Vec2(Scene::Center().x + lineLength + 200, 60))
			.draw(2, ColorF(1.0, a));

		if (getData().gameOverFlag) {
			m_titleFont(U"Game Over").drawAt(Scene::Center().x + xOffset, 60, ColorF(1.0, 0, 0, a));
		}
		else {
			m_titleFont(U"Game Clear").drawAt(Scene::Center().x + xOffset, 60, ColorF(1.0, 1.0, 0, a));
		}
	}

	// 2. 育成結果ラベル
	{
		double a = getAlpha(1.0, 0.5);
		double xOffset = (1.0 - a) * slideDistance;
		m_labelFont(U"育成結果").draw(45 + xOffset, 100, ColorF(1.0, a));
	}

	// 3. 育成結果ブロック
	{
		double a = getAlpha(1.3, 0.5);
		double xOffset = (1.0 - a) * slideDistance;
		RectF blockRect(45 + xOffset, 130, 380, 300);
		drawBlock(a, blockRect);

		Texture stand(U"character.png");
		if (stand)
		{
			double maskY = stand.height() * 0.5;
			stand.draw(60 + xOffset, 160, ColorF(1.0, a));
			RectF(60.f, 160.f + static_cast<float>(maskY),
				  static_cast<float>(stand.width()), static_cast<float>(maskY))
				.draw(ColorF(0, 0, 0, 0.5 * a));
		}
	}

	// 4. 戦闘結果
	{
		double a = getAlpha(2.0, 0.5);
		double xOffset = (1.0 - a) * slideDistance;
		m_labelFont(U"戦闘結果").draw(Scene::Center().x + xOffset, 100, ColorF(1.0, a));
		drawBlock(a, RectF(Scene::Center().x + xOffset, 130, 280, 150));
	}

	// 5. 最終結果（ランク）
	{

		double a = getAlpha(3.5, 0.5);
		double xOffset = (1.0 - a) * slideDistance;
		m_labelFont(U"最終結果").drawAt(Scene::Center().x+ 340 + xOffset, 115, ColorF(1.0, a));

		double a2 = getAlpha(4.5, 0.5);
		ColorF rankColor = GetRankColor(m_rank);
		rankColor.a = a2;
		m_rankFont(m_rank).drawAt(Scene::Center().x + 340 + (1.0 - a2) * slideDistance, 190, rankColor);
		m_labelFont(U"rank").drawAt(Scene::Center().x + 340 + (1.0 - a2) * slideDistance, 260, rankColor);
	}

	// 6. ステータス文
	{
		double a = getAlpha(6.0, 0.5);
		double xOffset = (1.0 - a) * slideDistance;

		// 背景ブロック（テキストより先に描画）
		RectF statusRect(Scene::Center().x + xOffset, 290, 406, 50);
		drawBlock(a, statusRect);

		m_textFont(U"無事、ウイルスは撃退された。").draw(Scene::Center().x+20 + xOffset, 300, ColorF(1.0, a));
	}

	// 7. RESTARTボタン
	{
		double a = getAlpha(6.0, 0.5);
		if (m_isRestartHovered)
		{
			m_restartHoverTex.resized(m_restartHoverTex.size()*0.5).draw(m_restartRect.pos - Vec2{0, 0}, ColorF(1.0, a));
			m_restartNormalTex.resized(m_restartNormalTex.size() * 0.5).draw(m_restartRect.pos, ColorF(1.0, a));
		}
		else
		{
			m_restartNormalTex.resized(m_restartNormalTex.size() * 0.5).draw(m_restartRect.pos, ColorF(1.0, a));
		}
	}
}
