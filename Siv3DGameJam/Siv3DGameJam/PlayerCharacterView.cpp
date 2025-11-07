# include "PlayerCharacterView.h"
# include "MasterData.h"
# include "GameData.h"
using namespace s3d;

PlayerCharacterView::PlayerCharacterView()
{
	MasterData& masterData = MasterData::getInstance();
	GameData& gameData = GameData::getInstance();

	characterTexture = Texture(masterData.getTexturePath(gameData.infos().pcTextureId));
	maxHp = gameData.infos().reliability;
	maxMp = gameData.infos().availability;
}

// ★ update() を追加
void PlayerCharacterView::update()
{
	if (!m_isActionAnimating)
	{
		return;
	}

	double t = m_actionAnimTimer.sF() / m_actionAnimDuration;
	if (t >= 1.0)
	{
		m_yOffset = 0.0;
		m_isActionAnimating = false;
	}
	else
	{
		// 0.0 -> 0.5 (中間) -> 1.0 (終了)
		// EaseOutCubic を使い、中間で -30px まで上がる
		if (t < 0.5)
		{
			// 0.0 -> 0.5 (上がる)
			m_yOffset = Math::Lerp(0.0, -30.0, EaseOutCubic(t * 2.0));
		}
		else
		{
			// 0.5 -> 1.0 (下がる)
			m_yOffset = Math::Lerp(-30.0, 0.0, EaseInCubic((t - 0.5) * 2.0));
		}
	}

	// ★ 点滅アニメーションの更新
	if (m_isBlinking)
	{
		double t = m_blinkTimer.sF();
		if (t > m_blinkDuration)
		{
			m_isBlinking = false;
			m_blinkVisible = true; // 確実に表示状態に戻す
		}
		else
		{
			// 0.1秒ごと（t * 10.0）に表示/非表示を切り替える
			m_blinkVisible = (static_cast<int>(t * 10.0) % 2 == 0);
		}
	}
}

// ★ 点滅開始トリガー
void PlayerCharacterView::playDamageBlink()
{
	if (m_isBlinking) return; // アニメーション中は無視
	m_isBlinking = true;
	m_blinkTimer.restart();
}

// ★ アニメーション開始トリガーを追加
void PlayerCharacterView::playActionAnimation()
{
	if (m_isActionAnimating) return;
	m_isActionAnimating = true;
	m_actionAnimTimer.restart();
}

void PlayerCharacterView::draw() const
{
	GameData& gameData = GameData::getInstance();

	// 親のピボット設定
	// 基本的にcharacterInfoOffsetをずらせばこのスコープ内のUIが移動する
	Rect characterInfoRect{ Arg::center(120, 400), 500 };
	// ★ 修正: m_yOffset を Translate に追加
	const Transformer2D t0{ Mat3x2::Translate(characterInfoRect.center() + Vec2(0, m_yOffset)) };
	characterTexture.resized(400, 400).draw(Arg::center(0, 0));

#pragma region HPMP
	{
		const Vec2 statusOffset = Vec2{ -100, 15 };
		const Transformer2D t1{ Mat3x2::Translate(statusOffset) };

		RectF(0, 0, 200, 80).draw(ColorF{ 0, 0, 0, 0.8 });
		font(U"HP").draw(18, Vec2{ 10, 0 }, ColorF{ 1.0 });
		RectF maxHpRect = font(U"/" + Format(maxHp)).draw(14, Arg::bottomRight(190, 24), ColorF{ 1.0 });
		font(Format(gameData.infos().reliability)).draw(18, Arg::bottomRight(maxHpRect.leftX(), 24)/*Vec2{ 125, 0 }*/, ColorF{1.0});
		RectF{ 10, 25, 180, 10 }.draw(Arg::left = ColorF{ 1, 1, 0 }, Arg::right = ColorF{ 0, 1, 0 });

		font(U"MP").draw(18, Vec2{ 10, 40 }, ColorF{ 1.0 });
		RectF maxMpRect = font(U"/" + Format(maxMp)).draw(14, Arg::bottomRight(190, 64), ColorF{ 1.0 });
		font(Format(gameData.infos().availability)).draw(18, Arg::bottomRight(maxMpRect.leftX(), 64), ColorF{1.0});
		RectF{ 10, 65, 180, 10 }.draw(Arg::left = ColorF{ 0, 1, 1 }, Arg::right = ColorF{ 0, 0, 1 });
	}
#pragma endregion

#pragma region Break
	{
		const Vec2 breakOffset = Vec2{ 60, -100 };
		const Transformer2D t1{ Mat3x2::Translate(breakOffset) };

		RectF breakRect{ Arg::center(0, 0), 60, 60 };
		breakTexture.resized(Vec2{ 118, 150 } / 2).draw(Arg::center(breakRect.center()));
		{
			Transformer2D t2{ Mat3x2::Translate(breakRect.rightX(), breakRect.bottomY()) };
			font(U"3").draw(TextStyle::Outline(0.5, ColorF{ 0 }), 40, Arg::center(0, 0));
		}
	}
#pragma endregion
}

void PlayerCharacterView::setMaxStatus(int32 max_hp, int32 max_mp)
{
	maxHp = max_hp;
	maxMp = max_mp;
}
